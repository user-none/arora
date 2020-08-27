/*
   Copyright (C) 2009, Torch Mobile Inc. and Linden Research, Inc. All rights reserved.
*/

/* ***** BEGIN LICENSE BLOCK *****
 * Version: MPL 1.1/GPL 2.0/LGPL 2.1
 *
 * The contents of this file are subject to the Mozilla Public License Version
 * 1.1 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code is Torch Mobile Inc. (http://www.torchmobile.com/) code
 *
 * The Initial Developer of the Original Code is:
 *   Benjamin Meyer (benjamin.meyer@torchmobile.com)
 *
 * Alternatively, the contents of this file may be used under the terms of
 * either the GNU General Public License Version 2 or later (the "GPL"), or
 * the GNU Lesser General Public License Version 2.1 or later (the "LGPL"),
 * in which case the provisions of the GPL or the LGPL are applicable instead
 * of those above. If you wish to allow use of your version of this file only
 * under the terms of either the GPL or the LGPL, and not to allow others to
 * use your version of this file under the terms of the MPL, indicate your
 * decision by deleting the provisions above and replace them with the notice
 * and other provisions required by the GPL or the LGPL. If you do not delete
 * the provisions above, a recipient may use your version of this file under
 * the terms of any one of the MPL, the GPL or the LGPL.
 *
 * ***** END LICENSE BLOCK ***** */

#ifndef TRIE_H
#define TRIE_H

//#define TRIE_DEBUG

#include <qstringlist.h>

#if defined(TRIE_DEBUG)
#include <qdebug.h>
#endif

/*
    A Trie tree (prefix tree) where the lookup takes m in the worst case.

    The key is stored in _reverse_ order

    Example:
    Keys: x,a y,a

    Trie:
    a
    | \
    x  y
*/

template<class T>
class Trie {
public:
    Trie();
    ~Trie();

    void clear();
    void insert(const QStringList &key, const T &value);
    bool remove(const QStringList &key, const T &value);
    QList<T> find(const QStringList &key) const;
    QList<T> all() const;

    inline bool contains(const QStringList &key) const;
    inline bool isEmpty() const { return children.isEmpty() && values.isEmpty(); }

private:
    const Trie<T>* walkTo(const QStringList &key) const;
    Trie<T>* walkTo(const QStringList &key, bool create = false);

    template<class T1> friend QDataStream &operator<<(QDataStream &, const Trie<T1>&);
    template<class T1> friend QDataStream &operator>>(QDataStream &, Trie<T1>&);

    QList<T> values;
    QStringList childrenKeys;
    QList<Trie<T> > children;
};

template<class T>
Trie<T>::Trie() {
}

template<class T>
Trie<T>::~Trie() {
}

template<class T>
void Trie<T>::clear() {
#if defined(TRIE_DEBUG)
    qDebug() << "Trie::" << __FUNCTION__;
#endif
    values.clear();
    childrenKeys.clear();
    children.clear();
}

template<class T>
bool Trie<T>::contains(const QStringList &key) const {
    return walkTo(key);
}

template<class T>
void Trie<T>::insert(const QStringList &key, const T &value) {
#if defined(TRIE_DEBUG)
    qDebug() << "Trie::" << __FUNCTION__ << key << value;
#endif
    Trie<T> *node = walkTo(key, true);
    if (node)
        node->values.append(value);
}

template<class T>
bool Trie<T>::remove(const QStringList &key, const T &value) {
#if defined(TRIE_DEBUG)
    qDebug() << "Trie::" << __FUNCTION__ << key << value;
#endif
    Trie<T> *node = walkTo(key, true);
    if (node) {
        bool removed = node->values.removeOne(value);
        if (!removed)
            return false;

        // A faster implementation of removing nodes up the tree
        // can be created if profile shows this to be slow
        QStringList subKey = key;
        while (node->values.isEmpty()
               && node->children.isEmpty()
               && !subKey.isEmpty()) {
            QString currentLevelKey = subKey.first();
            QStringList parentKey = subKey.mid(1);
            Trie<T> *parent = walkTo(parentKey, false);
            Q_ASSERT(parent);
            QStringList::iterator iterator;
            iterator = qBinaryFind(parent->childrenKeys.begin(),
                                   parent->childrenKeys.end(),
                                   currentLevelKey);
            Q_ASSERT(iterator != parent->childrenKeys.end());
            int index = iterator - parent->childrenKeys.begin();
            parent->children.removeAt(index);
            parent->childrenKeys.removeAt(index);

            node = parent;
            subKey = parentKey;
        }
        return removed;
    }
    return false;
}

template<class T>
QList<T> Trie<T>::find(const QStringList &key) const {
#if defined(TRIE_DEBUG)
    qDebug() << "Trie::" << __FUNCTION__ << key;
#endif
    const Trie<T> *node = walkTo(key);
    if (node)
        return node->values;
    return QList<T>();
}

template<class T>
QList<T> Trie<T>::all() const {
#if defined(TRIE_DEBUG)
    qDebug() << "Trie::" << __FUNCTION__;
#endif
    QList<T> all = values;
    for (int i = 0; i < children.count(); ++i)
        all += children[i].all();
    return all;
}

template<class T>
QDataStream &operator<<(QDataStream &out, const Trie<T>&trie) {
    out << trie.values;
    out << trie.childrenKeys;
    out << trie.children;
    Q_ASSERT(trie.childrenKeys.count() == trie.children.count());
    return out;
}

template<class T>
QDataStream &operator>>(QDataStream &in, Trie<T> &trie) {
    trie.clear();
    in >> trie.values;
    in >> trie.childrenKeys;
    in >> trie.children;
    Q_ASSERT(trie.childrenKeys.count() == trie.children.count());
    return in;
}

// Very fast const walk
template<class T>
const Trie<T>* Trie<T>::walkTo(const QStringList &key) const {
    const Trie<T> *node = this;
    QStringList::const_iterator childIterator;
    QStringList::const_iterator begin, end;

    int depth = key.count() - 1;
    while (depth >= 0) {
        const QString currentLevelKey = key.at(depth--);
        begin = node->childrenKeys.constBegin();
        end = node->childrenKeys.constEnd();
        childIterator = qBinaryFind(begin, end, currentLevelKey);
        if (childIterator == end)
            return 0;
        node = &node->children.at(childIterator - begin);
    }
    return node;
}

template<class T>
Trie<T>* Trie<T>::walkTo(const QStringList &key, bool create) {
    QStringList::iterator iterator;
    Trie<T> *node = this;
    QStringList::iterator begin, end;
    int depth = key.count() - 1;
    while (depth >= 0) {
        const QString currentLevelKey = key.at(depth--);
        begin = node->childrenKeys.begin();
        end = node->childrenKeys.end();
        iterator = qBinaryFind(begin, end, currentLevelKey);
#if defined(TRIE_DEBUG)
        qDebug() << "\t" << node << key << currentLevelKey << node->childrenKeys;
#endif
        int index = -1;
        if (iterator == end) {
            if (!create)
                return 0;
            iterator = qLowerBound(begin,
                                   end,
                                   currentLevelKey);
            index = iterator - begin;
            node->childrenKeys.insert(iterator, currentLevelKey);
            node->children.insert(index, Trie<T>());
        } else {
            index = iterator - begin;
        }
        Q_ASSERT(index >= 0 && index < node->children.count());
        node = &node->children[index];
    }
    return node;
}

#endif


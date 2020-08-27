/**
 * Copyright (c) 2009, Benjamin C. Meyer  <ben@meyerhome.net>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Benjamin Meyer nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#ifndef AUTOFILLMANAGER_H
#define AUTOFILLMANAGER_H

#include <qobject.h>

#include <qnetworkrequest.h>

class QWebPage;
class AutoSaver;
class AutoFillManager : public QObject
{
    Q_OBJECT

signals:
    void autoFillChanged();

public:
    typedef QPair<QString, QString> Element;
    class Form {
    public:
        bool isValid() { return !elements.isEmpty(); }
        static void load(QDataStream &in, Form &subscription);
        static void save(QDataStream &out, const Form &subscription);

        QList<Element> elements;
        QUrl url;
        QString name;
        bool hasAPassword;
    };

    AutoFillManager(QObject *parent = 0);
    ~AutoFillManager();

    void loadSettings();

    void post(const QNetworkRequest &request, const QByteArray &outgoingData);
    void fill(QWebPage *page) const;

    void setForms(const QList<Form> &forms);
    QList<Form> forms() const;

private slots:
    void save() const;

private:
    Form findForm(QWebPage *page, const QByteArray &outgoingData) const;
    static QUrl stripUrl(const QUrl &url);
    static QString autoFillDataFile();
    bool allowedToAutoFill(bool password) const;
    QList<AutoFillManager::Form> fetchForms(const QUrl &url) const;

    void saveFormData() const;
    void loadFormData();

    bool m_savePasswordForms;
    bool m_allowAutoCompleteOff;

    QList<Form> m_forms;
    QList<QUrl> m_never;
    AutoSaver *m_saveTimer;
};

QDataStream &operator<<(QDataStream &, const AutoFillManager::Form &form);
QDataStream &operator>>(QDataStream &, AutoFillManager::Form &form);

#endif // AUTOFILLMANAGER_H


/**
 * Copyright (c) 2009, Benjamin C. Meyer
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

#include <qapplication.h>
#include <qmenubar.h>
#include <qdirmodel.h>
#include <qmainwindow.h>

#include <qdebug.h>

#include <modelmenu.h>

class Menu : public ModelMenu
{
    Q_OBJECT

public:
    Menu(QWidget *parent);

protected:
    bool prePopulated();
    void postPopulated();
    ModelMenu *createBaseMenu();

private slots:
    void activated2(const QModelIndex &index);

};

Menu::Menu(QWidget *parent)
    : ModelMenu(parent)
{
    connect(this, SIGNAL(activated(const QModelIndex &)),
            this, SLOT(activated2(const QModelIndex &)));
}

bool Menu::prePopulated()
{
    qDebug() << __FUNCTION__;
    return ModelMenu::prePopulated();
}

void Menu::postPopulated()
{
    qDebug() << __FUNCTION__;
    return ModelMenu::postPopulated();
}

ModelMenu *Menu::createBaseMenu()
{
    qDebug() << __FUNCTION__;
    return new Menu(this);
    return ModelMenu::createBaseMenu();
}

void Menu::activated2(const QModelIndex &index)
{
    qDebug() << __FUNCTION__ << index.data() << this;
}

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    QMainWindow mainWindow;
    Menu *menu = new Menu(&mainWindow);
    menu->setTitle("Test");
    QDirModel *model = new QDirModel(menu);
    menu->setModel(model);
    menu->setRootIndex(model->index(QDir::homePath()));
    mainWindow.menuBar()->addMenu(menu);
    mainWindow.show();
    return app.exec();
}

#include "main_modelmenu.moc"


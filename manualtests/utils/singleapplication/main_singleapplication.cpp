/**
 * Copyright (c) 2008, Benjamin C. Meyer
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

#include <qplaintextedit.h>
#include <qdebug.h>
#include <qlocalsocket.h>
#include <qtextstream.h>

#include <singleapplication.h>

class PlainTextEdit : public QPlainTextEdit {
    Q_OBJECT

public:
    PlainTextEdit(QWidget *parent = 0)
        : QPlainTextEdit(parent) { }

public slots:
    void messageReceived(QLocalSocket *socket) {
        QString message;
        QTextStream stream(socket);
        stream >> message;
        appendPlainText(message);
    }

};

int main(int argc, char **argv)
{
    SingleApplication app(argc, argv);
    app.setApplicationName("testapp");
    if (app.arguments().count() > 1
        && app.sendMessage(app.arguments().last().toUtf8()))
        return 0;

    PlainTextEdit plainTextEdit;
    plainTextEdit.show();
    if (!app.startSingleServer())
        qWarning() << "Error starting server";
    app.connect(&app, SIGNAL(messageReceived(QLocalSocket *)),
                &plainTextEdit, SLOT(messageReceived(QLocalSocket *)));
    return app.exec();
}

#include "main_singleapplication.moc"


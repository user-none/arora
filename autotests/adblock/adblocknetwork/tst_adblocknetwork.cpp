/**
 * Copyright (c) 2009, Benjamin C. Meyer <ben@meyerhome.net>
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

#include <qtest.h>

#include "adblocknetwork.h"
#include "adblockmanager.h"
#include "adblocksubscription.h"
#include "adblockrule.h"

#include <qnetworkrequest.h>

class tst_AdBlockNetwork : public QObject
{
    Q_OBJECT

public slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

private slots:
    void adblocknetwork_data();
    void adblocknetwork();

    void data();

    void enabled_data();
    void enabled();

    void block_data();
    void block();
};

// Subclass that exposes the protected functions.
class SubAdBlockNetwork : public AdBlockNetwork
{
public:

};

// This will be called before the first test function is executed.
// It is only called once.
void tst_AdBlockNetwork::initTestCase()
{
}

// This will be called after the last test function is executed.
// It is only called once.
void tst_AdBlockNetwork::cleanupTestCase()
{
}

// This will be called before each test function is executed.
void tst_AdBlockNetwork::init()
{
}

// This will be called after every test function.
void tst_AdBlockNetwork::cleanup()
{
    AdBlockManager *manager = AdBlockManager::instance();
    QList<AdBlockSubscription*> list = manager->subscriptions();
    foreach (AdBlockSubscription *s, list)
        manager->removeSubscription(s);
}

void tst_AdBlockNetwork::adblocknetwork_data()
{
}

void tst_AdBlockNetwork::adblocknetwork()
{
    SubAdBlockNetwork network;
    QCOMPARE(network.block(QNetworkRequest()), (QNetworkReply*)0);
}

void tst_AdBlockNetwork::data()
{
    SubAdBlockNetwork network;

    AdBlockManager *manager = AdBlockManager::instance();
    manager->setEnabled(true);

    AdBlockSubscription *subscription = new AdBlockSubscription(QUrl(), manager);
    subscription->setEnabled(true);
    manager->addSubscription(subscription);

    AdBlockRule rule("/");
    rule.setEnabled(true);
    subscription->addRule(rule);

    QNetworkReply *reply = 0;
    QNetworkRequest request = QNetworkRequest(QUrl("data://foobar"));

    reply = network.block(request);
    QVERIFY(!reply);
}

void tst_AdBlockNetwork::enabled_data()
{
    QTest::addColumn<bool>("enableManager");
    QTest::addColumn<bool>("enableSubscription");
    QTest::addColumn<bool>("enableRule");
    QTest::addColumn<bool>("block");
    QTest::newRow("null") << true << true << true << true;
    QTest::newRow("m") << false << true << true << false;
    QTest::newRow("s") << true << false << true << false;
    QTest::newRow("r") << true << true << false << false;
}

void tst_AdBlockNetwork::enabled()
{
    QFETCH(bool, enableManager);
    QFETCH(bool, enableSubscription);
    QFETCH(bool, enableRule);
    QFETCH(bool, block);

    SubAdBlockNetwork network;

    AdBlockManager *manager = AdBlockManager::instance();
    manager->setEnabled(enableManager);

    AdBlockSubscription *subscription = new AdBlockSubscription(QUrl(), manager);
    subscription->setEnabled(enableSubscription);
    manager->addSubscription(subscription);

    AdBlockRule rule("/");
    rule.setEnabled(enableRule);
    subscription->addRule(rule);

    QNetworkReply *reply = 0;
    QNetworkRequest grequest = QNetworkRequest(QUrl("http://www.google.com"));

    reply = network.block(grequest);
    QCOMPARE((reply != 0), block);
}

// check that block block and !block blocks other sites
void tst_AdBlockNetwork::block_data()
{
    QTest::addColumn<QString>("ruleList");
    QTest::addColumn<QUrl>("url");
    QTest::addColumn<bool>("block");

    QTest::newRow("null") << QString()
                          << QUrl()
                          << false;

    QUrl google("http://www.google.com");
    QTest::newRow("google") << QString("/")
                          << google
                          << true;

    // defining exception rules
    QTest::newRow("exception0") << QString("@@advice,advice")
                                << QUrl("http://example.com/advice.html")
                                << false;
    QTest::newRow("exception1") << QString("@@|http://example.com")
                                << QUrl("http://example.com/advice.html")
                                << false;
    QTest::newRow("exception2") << QString("@@http://example.com")
                                << QUrl("http://example.com/advice.html")
                                << false;

    QTest::newRow("order0") << QString("advice,@@advice")
                            << QUrl("http://example.com/advice.html")
                            << false;
}

// public QNetworkReply *block(QNetworkRequest const &request)
void tst_AdBlockNetwork::block()
{
    QFETCH(QString, ruleList);
    QFETCH(QUrl, url);
    QFETCH(bool, block);

    SubAdBlockNetwork network;

    AdBlockManager *manager = AdBlockManager::instance();
    manager->setEnabled(true);

    AdBlockSubscription *subscription = new AdBlockSubscription(QUrl(), manager);
    subscription->setEnabled(true);
    manager->addSubscription(subscription);

    QStringList rules = ruleList.split(",");
    foreach (const QString &rule, rules)
        subscription->addRule(AdBlockRule(rule));

    QNetworkReply *reply = network.block(QNetworkRequest(url));
    bool blocked = (reply != 0);
    QCOMPARE(blocked, block);
}

QTEST_MAIN(tst_AdBlockNetwork)
#include "tst_adblocknetwork.moc"


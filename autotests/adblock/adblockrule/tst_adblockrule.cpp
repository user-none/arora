/*
 * Copyright 2009 Benjamin C. Meyer <ben@meyerhome.net>
 * Copyright 2009 Zsombor Gegesy <gzsombor@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA  02110-1301  USA
 */

#include <qtest.h>

#include "adblockrule.h"

#include <qbuffer.h>
#include <qdebug.h>

class tst_AdBlockRule : public QObject
{
    Q_OBJECT

public slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

private slots:
    void regexpCreation_data();
    void regexpCreation();
    void networkMatch_data();
    void networkMatch();

};

// Subclass that exposes the protected functions.
class SubAdBlockRule : public AdBlockRule
{
public:
    SubAdBlockRule(const QString &filter);
};

SubAdBlockRule::SubAdBlockRule(const QString &filter)
    : AdBlockRule(filter)
{
}

// This will be called before the first test function is executed.
// It is only called once.
void tst_AdBlockRule::initTestCase()
{
}

// This will be called after the last test function is executed.
// It is only called once.
void tst_AdBlockRule::cleanupTestCase()
{
}

// This will be called before each test function is executed.
void tst_AdBlockRule::init()
{
}

// This will be called after every test function.
void tst_AdBlockRule::cleanup()
{
}

void tst_AdBlockRule::networkMatch_data()
{
    QTest::addColumn<QString>("filter");
    QTest::addColumn<QUrl>("url");
    QTest::addColumn<bool>("networkMatch");
    QTest::newRow("null") << QString() << QUrl() << false;

    //Examples taken from http://adblockplus.org/en/filters

    // Basic filter rules
    QTest::newRow("basic0") << QString("http://example.com/ads/banner123.gif")
                            << QUrl("http://example.com/ads/banner123.gif")
                            << true;
    QTest::newRow("basic1") << QString("http://example.com/ads/banner*.gif")
                            << QUrl("http://example.com/ads/banner123.gif")
                            << true;
    QTest::newRow("basic2") << QString("http://example.com/ads/*")
                            << QUrl("http://example.com/ads/banner123.gif")
                            << true;
    QTest::newRow("basic3") << QString("http://example.com/*")
                            << QUrl("http://example.com/ads/banner123.gif")
                            << true;

    // defining exception rules
    QTest::newRow("exception0") << QString("@@advice")
                                << QUrl("http://example.com/advice.html")
                                << true;
    QTest::newRow("exception1") << QString("@@|http://example.com")
                                << QUrl("http://example.com/advice.html")
                                << true;
    QTest::newRow("exception2") << QString("@@http://example.com")
                                << QUrl("http://example.com/advice.html")
                                << true;

    // matching at beginning/end of an address
    QTest::newRow("m0") << QString("ad")
                        << QUrl("http://example.com/advice.html")
                        << true;
    QTest::newRow("m1") << QString("*ad*")
                        << QUrl("http://example.com/advice.html")
                        << true;
    QTest::newRow("m2") << QString("swf")
                        << QUrl("http://example.com/swf/index.html")
                        << true;
    QTest::newRow("m3") << QString("swf|")
                    << QUrl("http://example.com/annoyingflash.swf")
                    << true;
    QTest::newRow("m4") << QString("swf|")
                    << QUrl("http://example.com/swf/index.html")
                    << false;

    QTest::newRow("m5") << QString("http://baddomain.example/")
                    << QUrl("http://baddomain.example/banner.gif")
                    << true;
    QTest::newRow("m6") << QString("http://baddomain.example/")
                    << QUrl("http://gooddomain.example/analyze?http://baddomain.example.")
                    << false;

    QTest::newRow("m7") << QString("||example.com/banner.gif")
                    << QUrl("http://example.com/banner.gif")
                    << true;
    QTest::newRow("m8") << QString("||example.com/banner.gif")
                    << QUrl("https://example.com/banner.gif")
                    << true;
    QTest::newRow("m9") << QString("||example.com/banner.gif")
                    << QUrl("http://www.example.com/banner.gif")
                    << true;

    QTest::newRow("m10") << QString("||example.com/banner.gif")
                    << QUrl("http://badexample.com/banner.gif")
                    << false;
    QTest::newRow("m10") << QString("||example.com/banner.gif")
                    << QUrl("http://gooddomain.example/analyze?http://example.com/banner.gif")
                    << false;

    QTest::newRow("m11") << QString("/getad.php|")
                    << QUrl("http://adblockplus.mozdev.org/easylist/easylist.txt")
                    << false;

    // Matching separators
    QTest::newRow("sep_1") << QString("a^")
                         << QUrl("a/")
                         << true;
    QTest::newRow("sep_2") << QString("a^")
                         << QUrl("a:")
                         << true;
    QTest::newRow("sep_3") << QString("a^")
                         << QUrl("a.")
                         << false;
    QTest::newRow("sep_4") << QString("a^")
                         << QUrl("a?")
                         << true;
    QTest::newRow("sep_5") << QString("a^")
                         << QUrl("a=")
                         << true;
    QTest::newRow("sep_6") << QString("a^")
                         << QUrl("a&")
                         << true;

    QTest::newRow("s1_s") << QString("com^")
                         << QUrl("com:")
                         << true;
    QTest::newRow("s2_s") << QString("ab^cd")
                         << QUrl("ab.cd")
                         << false;

    // Marking separate characters
    QTest::newRow("s0") << QString("http://example.com^")
                         << QUrl("http://example.com/")
                         << true;
    QTest::newRow("s1") << QString("http://example.com^")
                         << QUrl("http://example.com:8000/")
                         << true;
    QTest::newRow("s2") << QString("http://example.com^")
                         << QUrl("http://example.com.ar/")
                         << false;
    QTest::newRow("s3") << QString("^example.com^")
                         << QUrl("http://example.com:8000/foo.bar?a=12&b=%D1%82%D0%B5%D1%81%D1%82")
                         << true;
    QTest::newRow("s4") << QString("^%D1^")
                         << QUrl::fromEncoded("http://example.com:8000/foo.bar?a=12&b=%D1")
                         << true;
    QTest::newRow("s5") << QString("^foo.bar^")
                         << QUrl("http://example.com:8000/foo.bar?a=12&b=%D1%82%D0%B5%D1%81%D1%82")
                         << true;
    QTest::newRow("s6") << QString("^%D1%82%D0%B5%D1%81%D1%82^")
                         << QUrl::fromEncoded("http://example.com:8000/foo.bar?a=12&b=%D1%82%D0%B5%D1%81%D1%82")
                         << true;
    // Comments
    QTest::newRow("c0") << QString("!foo.bar")
                        << QUrl("!foo.bar")
                        << false;
    QTest::newRow("c1") << QString("!foo.bar")
                        << QUrl("foo.bar")
                        << false;

    // Specifying filter options
    // type
    QTest::newRow("o0") << QString("*/ads/*$script,image,background,stylesheet,object,xbl,ping,xmlhttprequest,object-subrequest,object-subrequest,dtd,subdocument,document,other")
                        << QUrl("foo.bar/ads/foo.jpg")
                        << false;
    // Inverse type
    QTest::newRow("o1") << QString("*/ads/*$~script, ~image, ~background, ~stylesheet, ~object, ~xbl, ~ping, ~xmlhttprequest, ~object-subrequest, ~dtd, ~subdocument, ~document, ~other")
                        << QUrl("foo.bar/ads/foo.jpg")
                        << false;
    // Restriction to third-party/first-party requests
    QTest::newRow("o2") << QString("*/ads/*$third-party")
                        << QUrl("foo.bar/ads/foo.jpg")
                        << false;
    QTest::newRow("o3") << QString("*/ads/*$first-party")
                        << QUrl("foo.bar/ads/foo.jpg")
                        << false;
    // Domain restrictions
    QTest::newRow("o4") << QString("*/ads/*$domain=example.com|example.net")
                        << QUrl("http://example.com/ads/foo.jpg")
                        << true;
    QTest::newRow("o5") << QString("*/ads/*$domain=example.com")
                        << QUrl("http://foo.com/ads/foo.jpg")
                        << false;

    QTest::newRow("o6") << QString("*/ads/*$domain=~example.com")
                        << QUrl("http://foo.com/ads/foo.jpg")
                        << true;
    QTest::newRow("o7") << QString("*/ads/*$domain=~example.com")
                        << QUrl("http://example.com/ads/foo.jpg")
                        << false;
    QTest::newRow("o8") << QString("*/ads/*$domain=example.com|~foo.example.com")
                        << QUrl("http://example.com/ads/foo.jpg")
                        << true;
    QTest::newRow("o9") << QString("*/ads/*$domain=example.com|~foo.example.com")
                        << QUrl("http://foo.example.com/ads/foo.jpg")
                        << false;
    QTest::newRow("o10") << QString("*/ads/*$domain=example.com|~foo.example.com")
                        << QUrl("http://bar.example.com/ads/foo.jpg")
                        << true;
    // match-case
    QTest::newRow("o11") << QString("*/BannerAd.gif$match-case")
                         << QUrl("http://example.com/BannerAd.gif")
                         << true;
    QTest::newRow("o12") << QString("*/BannerAd.gif$match-case")
                         << QUrl("http://example.com/bannerad.gif")
                         << false;
    // collapse
    // TODO test collapse somehow
    QTest::newRow("o13") << QString("*/BannerAd.gif$collapse")
                         << QUrl("http://example.com/bannerad.gif")
                         << false;
    QTest::newRow("o14") << QString("*/BannerAd.gif$~collapse")
                         << QUrl("http://example.com/bannerad.gif")
                         << false;
    // Regular expressions
    QTest::newRow("r0") << QString("/banner\\d+/")
                         << QUrl("banner123")
                         << true;
    QTest::newRow("r1") << QString("/banner\\d+/")
                         << QUrl("banner321")
                         << true;
    QTest::newRow("r2") << QString("/banner\\d+/")
                         << QUrl("banners")
                         << false;

    // See the AdBlockPage autotest for CSS testing
    // Basic Element hiding the network matching should never pass
    QTest::newRow("e0") << QString("##div.textad")
                        << QUrl()
                        << false;
    QTest::newRow("e1") << QString("##div#sponsorad")
                        << QUrl()
                        << false;
    QTest::newRow("e1") << QString("##*#sponsorad")
                        << QUrl()
                        << false;
    QTest::newRow("e1") << QString("##textad")
                        << QUrl()
                        << false;
    QTest::newRow("e1") << QString("example.com##*.sponsor")
                        << QUrl("example.com")
                        << false;
    QTest::newRow("e1") << QString("example.com,example.net##*.sponsor")
                        << QUrl("example.com")
                        << false;

    // Seen on the internet
    QTest::newRow("i0") << QString("||snap.com^$third-party")
                        << QUrl("http://spa.snap.com/snap_preview_anywhere.js?ap=1&key=89743df349c6c38afc3094e9566cb98e&sb=1&link_icon=off&domain=pub-6332280-www.techcrunch.com")
                        << true;

    QTest::newRow("i1") << QString("|http://ads.")
                        << QUrl("http://ads.cnn.com/html.ng/site=cnn&cnn_pagetype=main&cnn_position=336x280_adlinks&cnn_rollup=homepage&page.allowcompete=yes&params.styles=fs&tile=8837285713521&domId=463050")
                        << true;

    QTest::newRow("i2") << QString("/adverti$~object_subrequest,~stylesheet,domain=~advertise4free.org|~amarillas.cl|~bnet.com|~catalysttelecom.com|~cod4central.com|~scansource.com|~scansourcecommunications.com|~scansourcesecurity.com")
                        << QUrl("http://i2.cdn.turner.com/cnn/.element/img/2.0/content/ads/advertisement.gif")
                        << true;

    QTest::newRow("i3") << QString("/adspace")
                        << QUrl("http://i.cdn.turner.com/cnn/cnn_adspaces/cnn_adspaces.js")
                        << false;
}

// public bool networkMatch(const QUrl &url) const
void tst_AdBlockRule::networkMatch()
{
    QFETCH(QString, filter);
    QFETCH(QUrl, url);
    QFETCH(bool, networkMatch);

    SubAdBlockRule AdBlockRule(filter);
    bool result = AdBlockRule.networkMatch(url.toEncoded());
    if (result != networkMatch) {
        qDebug() << "Match error.";
        qDebug() << "\tFilter:" << filter;
        qDebug() << "\tUrl: " << url;
        qDebug() << "\tResult : " << (result ? "match" : "NOT match");
    }
    QCOMPARE(AdBlockRule.networkMatch(url.toEncoded()), networkMatch);
}

void tst_AdBlockRule::regexpCreation_data()
{
    QTest::addColumn<QString>("input");
    QTest::addColumn<QString>("output");

    QTest::newRow("null") << QString("test") << QString("test");
    QTest::newRow("m1") << QString("abc*") << QString("abc");
    QTest::newRow("m2") << QString("*abc*") << QString("abc");
    QTest::newRow("m3") << QString("abc**") << QString("abc");
    QTest::newRow("m4") << QString("abc***") << QString("abc");
    QTest::newRow("m5") << QString("**abc**") << QString("abc");
    QTest::newRow("m6") << QString("**a*bc**") << QString("a.*bc");
    QTest::newRow("h1") << QString("abc.com") << QString("abc\\.com");

    QTest::newRow("a2") << QString("abc^|def") << QString("abc(?:[^\\w\\d\\-.%]|$)\\|def");
    QTest::newRow("a3") << QString("^|def") << QString("(?:[^\\w\\d\\-.%]|$)\\|def");

    QTest::newRow("s1") << QString("de|f") << QString("de\\|f");

    QTest::newRow("e1") << QString("|abc") << QString("^abc");
    QTest::newRow("e2") << QString("d|abc") << QString("d\\|abc");
    QTest::newRow("e3") << QString("abc|") << QString("abc$");

    QTest::newRow("z1") << QString("^abc") << QString("(?:[^\\w\\d\\-.%]|$)abc");
    QTest::newRow("z2") << QString("a^bc") << QString("a(?:[^\\w\\d\\-.%]|$)bc");
    QTest::newRow("z3") << QString("abc^|") << QString("abc(?:[^\\w\\d\\-.%]|$)");
    QTest::newRow("z4") << QString("http://example.com^")
            << QString("http\\:\\/\\/example\\.com(?:[^\\w\\d\\-.%]|$)");


    QTest::newRow("d0") << QString("||example.com")
            << QString("^[\\w\\-]+:\\/+(?!\\/)(?:[^\\/]+\\.)?example\\.com");
    QTest::newRow("d1") << QString("||example.com/banner.gif")
            << QString("^[\\w\\-]+:\\/+(?!\\/)(?:[^\\/]+\\.)?example\\.com\\/banner\\.gif");
}

void tst_AdBlockRule::regexpCreation()
{
     QFETCH(QString, input);
     QFETCH(QString, output);

     SubAdBlockRule rule(input);
     QCOMPARE(rule.regExpPattern(), output);
}

QTEST_MAIN(tst_AdBlockRule)
#include "tst_adblockrule.moc"


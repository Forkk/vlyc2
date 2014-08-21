/*****************************************************************************
 * vlyc2 - A Desktop YouTube client
 * Copyright (C) 2013 Orochimarufan <orochimarufan.x3@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *****************************************************************************/

#include <QApplication>
#include <QNetworkProxy>

#include <iostream>

#include "vlyc.h"
#include "logic/VlycDBus.h"
#include "gui/mainwindow.h"
#include "util/cmdparser.h"

#include <VlcInstance.h>


int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    app.setApplicationName("vlyc2");
    app.setApplicationDisplayName("VLYC²");
    app.setApplicationVersion("0.9.10");
    app.setOrganizationName("Orochimarufan");
    app.setOrganizationDomain("oro.sodimm.me");
    app.setWindowIcon(QIcon(":/vlyc/res/vlyc2.png"));

    CmdParser::Parser parser(CmdParser::FlagStyle::GNU, CmdParser::ArgumentStyle::SpaceAndEquals);

    parser.newArgument("mrl", QString());
    parser.addDocumentation("mrl", "The MRL to play after startup");

    parser.newOption("proxy", QString());
    parser.addDocumentation("proxy", "Use a proxy (scheme://user:pwd@host:port)");

    parser.newSwitch("help", false);
    parser.addFlag("help", 'h');
    parser.addDocumentation("help", "Show this help");

    QHash<QString,QVariant> args = parser.parse(app.arguments());

    if (args["help"].toBool())
    {
        std::cout << qPrintable(parser.compileHelp(app.arguments()[0]));
        return 0;
    }

    QStringList libvlc_args;

    if (!args["proxy"].toString().isNull())
    {
        QUrl proxy(args["proxy"].toString());
        if (proxy.scheme().toLower().startsWith("socks"))
        {
            libvlc_args << QStringLiteral("--socks=%1:%2").arg(proxy.host()).arg(proxy.port());
            if (!proxy.userName().isEmpty())
                libvlc_args << QStringLiteral("--socks-user=%1").arg(proxy.userName());
            if (!proxy.password().isEmpty())
                libvlc_args << QStringLiteral("--socks-pwd=%1").arg(proxy.password());
        }
        else if (proxy.scheme().toLower() == "http")
        {
            if (proxy.userName().isEmpty())
                libvlc_args << QStringLiteral("--http-proxy=%1:%2").arg(proxy.host()).arg(proxy.port(8080));
            else
            {
                libvlc_args << QStringLiteral("--http-proxy=http://%3@%1:%2").arg(proxy.host()).arg(proxy.port(8080)).arg(proxy.userName());
                if (!proxy.password().isEmpty())
                    libvlc_args << QStringLiteral("--http-proxy-pwd=%1").arg(proxy.password());
            }
        }
        else
        {
            std::cerr << "Proxy scheme " << qPrintable(proxy.scheme()) << " not supported (SOCKS and HTTP are)" << std::endl;
            args["proxy"] = QString();
        }
    }

    VlcInstance::initGlobalInstance(libvlc_args);

    VlycApp vlyc;

    QString mrl = args["mrl"].toString();
    if (mrl.startsWith("vlyc:"))
        mrl = mrl.mid(5);

    // Initialize the DBus connection.
    // TODO: Command line argument to disable this.
    if (!vlyc.dbus()->tryRegister())
    {
        // If another process is running and we've got a URL to play, connect
        // to it and tell it to play the URL.
        if (vlyc.dbus()->alreadyRunning() && !mrl.isEmpty())
        {
            printf("Sending URL to existing Vlyc instance.\n");
            vlyc.dbus()->playRunning(mrl);
            return 0;
        }
        else
        {
            fprintf(stderr, "Failed to register DBus service.\n");
            return 1;
        }
    }


    app.setProperty("vlyc", QVariant::fromValue((QObject*)&vlyc));

    if (!args["proxy"].toString().isNull())
    {
        QUrl proxyUrl(args["proxy"].toString());
        QNetworkProxy::ProxyType type;
        if (proxyUrl.scheme().toLower() == "http")
            type = QNetworkProxy::HttpProxy;
        else if (proxyUrl.scheme().toLower() == "socks")
            type = QNetworkProxy::Socks5Proxy;
        else if (proxyUrl.scheme().toLower() == "https")
            type = QNetworkProxy::HttpProxy;
        else
        {
            std::cerr << "Unknown Proxy scheme: " << proxyUrl.scheme().toLocal8Bit().constData() << std::endl;
            type = QNetworkProxy::NoProxy;
        }
        QNetworkProxy proxy(type, proxyUrl.host(), proxyUrl.port(8080), proxyUrl.userName(), proxyUrl.password());
        QNetworkProxy::setApplicationProxy(proxy);
        std::cerr << "Proxy: " << qPrintable(proxy.hostName()) << ":" << qPrintable(QString::number(proxy.port())) << std::endl;
    }

    vlyc.window()->show();

    if (!mrl.isEmpty())
        QMetaObject::invokeMethod(&vlyc, "play", Qt::QueuedConnection, Q_ARG(QUrl, mrl));

    return app.exec();
}

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
/* taken from https://github.com/yuyichao/qtcurve-qt5/blob/master/style/xcb_utils.cpp */
/***************************************************************************
 *   Copyright (C) 2013~2013 by Yichao Yu                                  *
 *   yyc1992@gmail.com                                                     *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.              *
 ***************************************************************************/

#include "vlyc_xcb.h"
#include <qpa/qplatformnativeinterface.h>

namespace XCB {

xcb_connection_t *_getConnection()
{
    if (!qApp)
        return nullptr;
    QPlatformNativeInterface *native = qApp->platformNativeInterface();
    if (!native)
        return nullptr;

    void *connection = native->nativeResourceForWindow(QByteArray("connection"), 0);
    return reinterpret_cast<xcb_connection_t *>(connection);
}

void getAtoms(size_t n, xcb_atom_t *atoms, const char *const names[], bool create)
{
    xcb_connection_t *conn = XCB::getConnection();
    xcb_intern_atom_cookie_t *cookies =
        (xcb_intern_atom_cookie_t*)malloc(sizeof(xcb_intern_atom_cookie_t) * n);
    for (size_t i = 0;i < n;i++) {
        cookies[i] = xcb_intern_atom(conn, create, strlen(names[i]), names[i]);
    }
    memset(atoms, 0, sizeof(xcb_atom_t) * n);
    for (size_t i = 0;i < n;i++) {
        xcb_intern_atom_reply_t *r =
            xcb_intern_atom_reply(conn, cookies[i], 0);
        if (r) {
            atoms[i] = r->atom;
            free(r);
        }
    }
    free(cookies);
}

void setWMClass(WId window, QString name, QString two)
{
    QByteArray b = name.toLocal8Bit();
    b.append('\0');
    b.append(two.toLocal8Bit());
    static const auto atom = XCB::getAtom("WM_CLASS");
    XcbCallVoid(change_property, XCB_PROP_MODE_REPLACE,
                window, atom, XCB_ATOM_STRING, 8, b.count(),
                b.constData());
}

}

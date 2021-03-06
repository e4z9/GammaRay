/*
  remoteviewinterface.cpp

  This file is part of GammaRay, the Qt application inspection and
  manipulation tool.

  Copyright (C) 2015-2018 Klarälvdalens Datakonsult AB, a KDAB Group company, info@kdab.com
  Author: Volker Krause <volker.krause@kdab.com>

  Licensees holding valid commercial KDAB GammaRay licenses may use this file in
  accordance with GammaRay Commercial License Agreement provided with the Software.

  Contact info@kdab.com if any conditions of this licensing are not clear to you.

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "remoteviewinterface.h"
#include "streamoperators.h"

#include <common/objectbroker.h>
#include <common/remoteviewframe.h>

using namespace GammaRay;
QT_BEGIN_NAMESPACE
GAMMARAY_ENUM_STREAM_OPERATORS(RemoteViewInterface::RequestMode)

QDataStream &operator<<(QDataStream &s, Qt::TouchPointStates states)
{
    return s << (int)states;
}

QDataStream &operator>>(QDataStream &s, Qt::TouchPointStates &states)
{
    int st;
    s >> st;
    states = Qt::TouchPointStates(st);
    return s;
}

#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
QDataStream &operator<<(QDataStream &s, QTouchEvent::TouchPoint::InfoFlags flags)
{
    return s << (int)flags;
}

QDataStream &operator>>(QDataStream &s, QTouchEvent::TouchPoint::InfoFlags &flags)
{
    int f;
    s >> f;
    flags = QTouchEvent::TouchPoint::InfoFlags(f);
    return s;
}
#endif

QDataStream &operator<<(QDataStream &s, const QList<QTouchEvent::TouchPoint> &points)
{
    s << points.count();
    foreach (const auto &p, points) {
        s << p.id();
        s << p.state();
        s << p.rect() << p.sceneRect() << p.screenRect();
        s << p.normalizedPos();
        s << p.startPos() << p.startScenePos() << p.startScreenPos() << p.startNormalizedPos();
        s << p.lastPos() << p.lastScenePos() << p.lastScreenPos() << p.lastNormalizedPos();
        s << p.pressure();
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
        s << p.velocity();
        s << p.flags();
        s << p.rawScreenPositions();
#endif
    }
    return s;
}

template<class T>
void setPointValue(QDataStream &s, QTouchEvent::TouchPoint &p, void (QTouchEvent::TouchPoint::*func)(T))
{
    typename std::decay<T>::type value;
    s >> value;
    (p.*func)(value);
}

QDataStream &operator>>(QDataStream &s, QList<QTouchEvent::TouchPoint> &points)
{
    int count;
    s >> count;
    points.reserve(count);
    for (int i = 0; i < count; ++i) {
        QTouchEvent::TouchPoint p;

        setPointValue(s, p, &QTouchEvent::TouchPoint::setId);
        setPointValue(s, p, &QTouchEvent::TouchPoint::setState);

        setPointValue(s, p, &QTouchEvent::TouchPoint::setRect);
        setPointValue(s, p, &QTouchEvent::TouchPoint::setSceneRect);
        setPointValue(s, p, &QTouchEvent::TouchPoint::setScreenRect);

        setPointValue(s, p, &QTouchEvent::TouchPoint::setNormalizedPos);

        setPointValue(s, p, &QTouchEvent::TouchPoint::setStartPos);
        setPointValue(s, p, &QTouchEvent::TouchPoint::setStartScenePos);
        setPointValue(s, p, &QTouchEvent::TouchPoint::setStartScreenPos);
        setPointValue(s, p, &QTouchEvent::TouchPoint::setStartNormalizedPos);

        setPointValue(s, p, &QTouchEvent::TouchPoint::setLastPos);
        setPointValue(s, p, &QTouchEvent::TouchPoint::setLastScenePos);
        setPointValue(s, p, &QTouchEvent::TouchPoint::setLastScreenPos);
        setPointValue(s, p, &QTouchEvent::TouchPoint::setLastNormalizedPos);

        setPointValue(s, p, &QTouchEvent::TouchPoint::setPressure);
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
        setPointValue(s, p, &QTouchEvent::TouchPoint::setVelocity);

        setPointValue(s, p, &QTouchEvent::TouchPoint::setFlags);
        setPointValue(s, p, &QTouchEvent::TouchPoint::setRawScreenPositions);
#endif

        points.append(p);
    }
    return s;
}

QT_END_NAMESPACE


RemoteViewInterface::RemoteViewInterface(const QString &name, QObject *parent)
    : QObject(parent)
    , m_name(name)
{
    ObjectBroker::registerObject(name, this);

    qRegisterMetaType<QTouchEvent::TouchPoint>();
    qRegisterMetaType<QList<QTouchEvent::TouchPoint >>();

    qRegisterMetaType<RequestMode>();
    qRegisterMetaTypeStreamOperators<RequestMode>();
    qRegisterMetaTypeStreamOperators<GammaRay::RemoteViewFrame>();
    qRegisterMetaTypeStreamOperators<Qt::TouchPointStates>();
    qRegisterMetaTypeStreamOperators<QList<QTouchEvent::TouchPoint>>();
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
    qRegisterMetaTypeStreamOperators<QTouchEvent::TouchPoint::InfoFlags>();
#endif
}

QString RemoteViewInterface::name() const
{
    return m_name;
}

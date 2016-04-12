/*
  sourcelocation.h

  This file is part of GammaRay, the Qt application inspection and
  manipulation tool.

  Copyright (C) 2016 Klarälvdalens Datakonsult AB, a KDAB Group company, info@kdab.com
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

#ifndef GAMMARAY_SOURCELOCATION_H
#define GAMMARAY_SOURCELOCATION_H

#include "gammaray_common_export.h"

#include <QMetaType>
#include <QDataStream>
#include <QString>

class QUrl;

namespace GammaRay {

/** @brief Specifies a source code location. */
class GAMMARAY_COMMON_EXPORT SourceLocation
{
public:
    SourceLocation();
    explicit SourceLocation(const QString &fileName, int line = -1, int column = -1);
    explicit SourceLocation(const QUrl &fileUrl, int line = -1, int column = -1);
    ~SourceLocation();

    bool isValid() const;

    QString fileName() const;
    void setFileName(const QString &fileName);
    void setFileName(const QUrl &fileUrl);

    int line() const;
    void setLine(int line);

    int column() const;
    void setColumn(int column);

    QString displayString() const;

private:
    friend GAMMARAY_COMMON_EXPORT QDataStream &operator>>(QDataStream &in, SourceLocation &location);

    QString m_fileName; // ### QUrl?
    int m_line;
    int m_column;
};

GAMMARAY_COMMON_EXPORT QDataStream &operator<<(QDataStream &out, const SourceLocation &location);
GAMMARAY_COMMON_EXPORT QDataStream &operator>>(QDataStream &in, SourceLocation &location);

}

Q_DECLARE_METATYPE(GammaRay::SourceLocation)

#endif // GAMMARAY_SOURCELOCATION_H
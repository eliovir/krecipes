/***************************************************************************
 *   Copyright (C) 2003 by                                                 *
 *   Unai Garro (ugarro@users.sourceforge.net)                             *
 *   Cyril Bosselut (bosselut@b1project.com)                               *
 *   Jason Kivlighn (mizunoami44@users.sourceforge.net)                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef ELEMENT_H
#define ELEMENT_H
#include <qstring.h>
/**
@author Unai Garro
*/
class Element
{
public:
    Element();
    Element( const QString &name, int id = -1 );
    Element(const Element &el);

    ~Element();
    QString name;
    int id;
    Element& operator=(const Element &el);

    /** Compare two elements by their id */
    bool operator==(const Element &) const;
    
    /** Compare (sort) by name */
    bool operator<(const Element &e) const { return name < e.name; }
};

#endif

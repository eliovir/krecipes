/***************************************************************************
 *   Copyright (C) 2003 by                                                 *
 *   Jason Kivlighn (mizunoami44@users.sourceforge.net)                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef MMFEXPORTER_H
#define MMFEXPORTER_H

#include "baseexporter.h"

#include <qstringlist.h>

class RecipeDB;

/**
  * Export class for the Meal-Master file format
  * @author Jason Kivlighn
  *
  * Note: This format does not handle all the properties of recipes.
  *       Data lost in export to this format include:
  *           -Recipe photo
  *           -Authors
  *           -5 category maximum
  *           -Title is limited to 60 characters
  *           -Servings are limited to the range of 0-9999
  *           -Units are limited: If a given unit does not have a
  *            cooresponding MM abbrev., otherwise it will be
  *            exported without a unit.
  */
class MMFExporter : public BaseExporter
{
public:
	MMFExporter( RecipeDB *, const QString&, const QString );
	virtual ~MMFExporter();

	virtual QString createContent( const QPtrList<Recipe> & );

protected:
	virtual QString extensions() const{ return ".mmf"; }

private:
	void writeMMFHeader( QString &content, Recipe *recipe );
	void writeMMFIngredients( QString &content, Recipe *recipe );
	void writeMMFDirections( QString &content, Recipe *recipe );

	QStringList wrapText( const QString& str, int at) const;
};

#endif //MMFEXPORTER_H
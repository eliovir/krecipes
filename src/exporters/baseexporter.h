/***************************************************************************
 *   Copyright (C) 2003 by                                                 *
 *   Cyril Bosselut (bosselut@b1project.com)                               *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef BASEEXPORTER_H
#define BASEEXPORTER_H

#include <qvaluelist.h>
#include <qstringlist.h>
#include <qptrlist.h>

#include "DBBackend/recipedb.h"
#include "datablocks/recipelist.h"

class QFile;
class RecipeDB;

class BaseExporter
{
public:
	BaseExporter( RecipeDB *, const QString &file, const QString format );
	virtual ~BaseExporter();

	void exporter( const QValueList<int>& ids);

protected:
	virtual void saveToFile( const QPtrList<Recipe> & ) = 0;

	QFile* file;
	QString format;
	QString filename;
	QStringList possible_formats;

private:
	bool createFile();

	RecipeDB *database;
};

#endif //BASEEXPORTER_H
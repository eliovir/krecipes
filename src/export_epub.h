/***************************************************************************
 *   Copyright © 2014 Eliovir <eliovir@gmail.com>                          *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

// NB: check the generated EPUB file with https://github.com/IDPF/epubcheck

#ifndef EXPORT_EPUB_H
#define EXPORT_EPUB_H

#include <QObject>
#include "backends/recipedb.h"
#include "exporters/htmlexporter.h"
// KConfigGroup
#include <kconfiggroup.h>
#include <kglobal.h>
#include <kaboutdata.h>
#include <KZip> // http://api.kde.org/4.0-api/kdelibs-apidocs/kio/html/classKZip.html#_details
#include <iostream>
#include <map>

class QString;

class ExportEpub : public HTMLExporter {
public:
    ExportEpub();
    int save(QString filepath);
    ~ExportEpub();

private:
    // Internal variables
    RecipeDB *database;
    QString dbType;
    QString encoding;
    /**
     * path => mimetype
     */
    std::map<QString,QString> images;
    QString language;
    RecipeList recipe_list;
    QString title;
    QString uuid;
    /**
     * title => file
     */
    std::map<QString, QString> recipes;
    // Internal methods
    QString checkCorrectDBType(KConfigGroup &config);
    QString createContentOpf();
    QString createTitlepage();
    QString createTocNcx();
    void initDatabase();
    bool zipAddString(KZip *zip, QString content, std::string filename);
};

#endif //EXPORT_EPUB_H

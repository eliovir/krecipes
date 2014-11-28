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
    
    /**
     * Title of the EPub book.
     */
    QString title;
    
    /**
     * UUID of the EPub book.
     */
    QString uuid;
    
    /**
     * title => file
     */
    std::multimap<QString, QString> files;
    
    /**
     * Author => (title => file)
     */
    std::map<Element, std::multimap<QString, QString > > recipesByAuthor;
    
    /**
     * Category => (title => file)
     */
    std::map<Element, std::multimap<QString, QString > > recipesByCategory;
    
    // Internal methods
    
    QString checkCorrectDBType(KConfigGroup &config);
    QString createContentOpf();
    QString createTitlepage();
    QString createTocNcx();
    
    /**
     * All all pages to the ZIP for the type ("aut", "cat").
     * 
     * @param zip Archive
     * @param type "aut" or "cat"
     * @param recipesByType list of recipes for this type
     */
    void createTypePages(KZip *zip, std::string type, std::map<Element, std::multimap<QString, QString > > recipesByType);
    
    void initDatabase();
    
    /**
     * Create filename for the Element according to its type and id.
     * 
     * @param type "aut", "cat" or "recipe"
     * @param id ID of the element
     * @return file name with extension (".html")
     */
    QString typeFileName(std::string type, int id);
    
    bool zipAddString(KZip *zip, QString content, QString filename);
};

#endif //EXPORT_EPUB_H

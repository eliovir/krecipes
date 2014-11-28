/***************************************************************************
 *   Copyright © 2014 Eliovir <eliovir@gmail.com>                          *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

// NB: check the generated EPUB file with https://github.com/IDPF/epubcheck

#include "export_epub.h"

#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QIODevice>
#include <QTextStream>
#include <QUuid>

#include <kglobal.h>
#include <KLocale> // i18n
#include <ktemporaryfile.h> // http://api.kde.org/4.0-api/kdelibs-apidocs/kdecore/html/classKTemporaryFile.html
#include <KDateTime>

using std::cerr;
using std::cout;
using std::endl;

ExportEpub::ExportEpub() : HTMLExporter("index", ".html") {
    initDatabase();
    KLocale*loc = KGlobal::locale();
    encoding = loc->encoding();
    language = loc->language();
    title = i18n("Recipes");
    uuid = QUuid::createUuid();
    m_templateContent = "\
<?xml version='1.0' encoding='utf-8'?>\n\
<html xmlns='http://www.w3.org/1999/xhtml'>\n\
  <head>\n\
    <meta name='Generator' content='KRecipes'/>\n\
    <title>**TITLE**</title>\n\
    <meta content='http://www.w3.org/1999/xhtml; charset=utf-8' http-equiv='Content-Type'/>\n\
    <link href='stylesheet.css' type='text/css' rel='stylesheet'/>\n\
  </head>\n\
  <body>\n\
<div><a href=\"titlepage.xhtml\">&lt;&lt;</a></div>\n\
<h1>**TITLE**</h1>\n\
\n\
<div><img src=\"**PHOTO**\" alt=\"photo\" class=\"photo\"/></div>\n\
\n\
<div class=\"overall_rating\">**OVERALL_RATING**</div>\n\
<div class=\"categories\">**CATEGORIES**</div>\n\
<div class=\"authors\">**AUTHORS**</div>\n\
<div class=\"yield\">**YIELD**</div>\n\
<div class=\"prep_time\">**PREP_TIME**</div>\n\
\n\
<div class=\"ingredients\">**INGREDIENTS**</div>\n\
\n\
<div class=\"instructions\">**INSTRUCTIONS**</div>\n\
\n\
<div class=\"properties\">**PROPERTIES**</div>\n\
\n\
<div class=\"ratings\">**RATINGS**</div>\n\
</body></html>";
}

void ExportEpub::createTypePages(KZip *zip, std::string type, std::map<Element, std::multimap<QString, QString > > recipesByType) {
    for (std::map<Element, std::multimap<QString, QString > >::iterator cat_it = recipesByType.begin(); cat_it != recipesByType.end(); ++cat_it) {
        QString content;
        content = "<?xml version='1.0' encoding='UTF-8'?>\n";
        content += "<html xmlns=\"http://www.w3.org/1999/xhtml\" xml:lang=\"" + language + "\">\n";
        content += "  <head>\n";
        content += "    <meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\"/>\n";
        content += "    <meta name=\"calibre:cover\" content=\"true\"/>\n";
        content += "    <title>" + cat_it->first.name + "</title>\n";
        content += "  </head>\n";
        content += "  <body>\n";
        content += "    <div><a href=\"titlepage.xhtml\">&lt;&lt;</a></div>\n";
        content += "    <h1>" + cat_it->first.name + "</h1>\n";
        content += "    <ul>\n";
        for (std::map<QString, QString>::iterator recipe_it = cat_it->second.begin(); recipe_it != cat_it->second.end(); ++recipe_it) {
            content += "      <li><a href=\"" + recipe_it->second + "\">" + recipe_it->first + "</a></li>\n";
        }
        content += "    </ul>\n";
        content += "  </body>\n";
        content += "</html>";
        QString filename = typeFileName(type, cat_it->first.id);
        zipAddString(zip, content, filename);
        files.insert(std::pair<QString, QString>(cat_it->first.name, filename));
    }
}

QString ExportEpub::createContentOpf() {
    // http://www.numericus-glutinator.org/fabrication-livre-numerique/epub-content-opf/311
    QDate date = QDate::currentDate();
    QTime time = QTime::currentTime();
    QString timestamp = date.toString("yyyy-MM-dd") + "T" + time.toString("hh:mm:ss") + "+00:00";
    QString content;
    content = "<?xml version='1.0' encoding='UTF-8'?>\n";
    content += "<package xmlns=\"http://www.idpf.org/2007/opf\" unique-identifier=\"uuid_id\" version=\"2.0\">\n";
    content += "  <metadata xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns:opf=\"http://www.idpf.org/2007/opf\" xmlns:dcterms=\"http://purl.org/dc/terms/\" xmlns:calibre=\"http://calibre.kovidgoyal.net/2009/metadata\" xmlns:dc=\"http://purl.org/dc/elements/1.1/\">\n";
    content += "    <dc:publisher>KRecipes</dc:publisher>\n";
    content += "    <meta name=\"calibre:title_sort\" content=\"" + title + "\"/>\n";
    content += "    <dc:description></dc:description>\n";
    content += "    <dc:language>" + language + "</dc:language>\n";
    //content += "    <dc:creator opf:file-as=\"" + author + "\" opf:role=\"aut\">" + author + "</dc:creator>\n";
    content += "    <meta name=\"calibre:timestamp\" content=\"" + timestamp + "\"/>\n";
    content += "    <dc:title>" + title + "</dc:title>\n";
    //content += "    <meta name=\"cover\" content=\"cover\"/>\n";
    content += "    <dc:date>" + timestamp + "</dc:date>\n";
    content += "    <dc:contributor opf:role=\"bkp\">KRecipes</dc:contributor>\n";
    //content += "    <dc:identifier opf:scheme=\"ISBN\">2277116335</dc:identifier>\n";
    content += "    <dc:identifier id=\"uuid_id\" opf:scheme=\"uuid\">" + uuid + "</dc:identifier>\n";
    //content += "    <dc:subject>SF</dc:subject>\n";
    content += "  </metadata>\n";
    content += "  <manifest>\n";
    //content += "    <item href=\"cover.jpeg\" id=\"cover\" media-type=\"image/jpeg\"/>\n";
    // images
    int order = 0;
    for (std::map<QString, QString>::iterator image_it = images.begin(); image_it != images.end(); ++image_it) {
        content += "    <item href=\"" + image_it->first + "\" id=\"image" + QString::number(++order) + "\" media-type=\"" + image_it->second + "\"/>\n";
    }
    // recipes
    for (std::map<QString, QString>::iterator recipe_it = files.begin(); recipe_it != files.end(); ++recipe_it) {
        QString id = recipe_it->second;
        id.replace(".html", "");
        content += "    <item href=\"" + recipe_it->second + "\" id=\"" + id + "\" media-type=\"application/xhtml+xml\"/>\n";
    }

    content += "    <item href=\"stylesheet.css\" id=\"css\" media-type=\"text/css\"/>\n";
    content += "    <item href=\"titlepage.xhtml\" id=\"titlepage\" media-type=\"application/xhtml+xml\"/>\n";
    content += "    <item href=\"toc.ncx\" media-type=\"application/x-dtbncx+xml\" id=\"ncx\"/>\n";
    content += "  </manifest>\n";

    content += "  <spine toc=\"ncx\">\n";
    content += "    <itemref idref=\"titlepage\"/>\n";
    // recipes
    for (std::map<QString, QString>::iterator recipe_it = files.begin(); recipe_it != files.end(); ++recipe_it) {
        content += "    <itemref idref=\"" + recipe_it->second.replace(".html", "") + "\"/>\n";
    }
    content += "  </spine>\n";
    content += "  <guide>\n";
    content += "    <reference href=\"titlepage.xhtml\" type=\"cover\" title=\"Cover\"/>\n";
    content += "  </guide>\n";
    content += "</package>";
    return content;
}

QString ExportEpub::createTitlepage() {
    KDateTime date = KDateTime::currentDateTime(KDateTime::LocalZone);
    QString content;
    content = "<?xml version='1.0' encoding='UTF-8'?>\n";
    content += "<html xmlns=\"http://www.w3.org/1999/xhtml\" xml:lang=\"" + language + "\">\n";
    content += "  <head>\n";
    content += "    <meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\"/>\n";
    content += "    <meta name=\"calibre:cover\" content=\"true\"/>\n";
    content += "    <title>" + title + "</title>\n";
    content += "  </head>\n";
    content += "  <body>\n";
    content += "    <h1>" + title + "</h1>\n";
    content += "    <h2>" + i18n("Categories") + "</h2>\n";
    content += "    <ul>\n";
    for (std::map<Element, std::multimap<QString, QString > >::iterator cat_it = recipesByCategory.begin(); cat_it != recipesByCategory.end(); ++cat_it) {
        content += "      <li><a href=\"" + typeFileName("cat", cat_it->first.id) + "\">" + cat_it->first.name + " (" + QString::number(cat_it->second.size()) + ")</a></li>\n";
    }
    content += "    </ul>\n";
    content += "    <h2>" + i18n("Authors") + "</h2>\n";
    content += "    <ul>\n";
    for (std::map<Element, std::multimap<QString, QString > >::iterator aut_it = recipesByAuthor.begin(); aut_it != recipesByAuthor.end(); ++aut_it) {
        content += "      <li><a href=\"" + typeFileName("aut", aut_it->first.id) + "\">" + aut_it->first.name + " (" + QString::number(aut_it->second.size()) + ")</a></li>\n";
    }
    content += "    </ul>\n";
    content += QString("    <p>%1 %2</p>\n")
            .arg(i18nc("EBook created", "Created:"))
            .arg(KGlobal::locale()->formatDateTime(date));
    content += "  </body>\n";
    content += "</html>";
    return content;
}

QString ExportEpub::createTocNcx() {
    QString content;
    content = "<?xml version='1.0' encoding='UTF-8'?>\n";
    content += "<ncx xmlns=\"http://www.daisy.org/z3986/2005/ncx/\" version=\"2005-1\" xml:lang=\"" + language + "\">\n";
    content += "  <head>\n";
    content += "    <meta content=\"" + uuid + "\" name=\"dtb:uid\"/>\n";
    content += "    <meta content=\"2\" name=\"dtb:depth\"/>\n";
    content += "    <meta content=\"KRecipes\" name=\"dtb:generator\"/>\n";
    content += "    <meta content=\"0\" name=\"dtb:totalPageCount\"/>\n";
    content += "    <meta content=\"0\" name=\"dtb:maxPageNumber\"/>\n";
    content += "  </head>\n";
    content += "  <docTitle>\n";
    content += "    <text>" + title + "</text>\n";
    content += "  </docTitle>\n";
    content += "  <navMap>\n";
    int order = 0;
    std::map<QString, QString> recipeOrder;
    for (std::map<Element, std::multimap<QString, QString > >::iterator cat_it = recipesByCategory.begin(); cat_it != recipesByCategory.end(); ++cat_it) {
        QString cat_uuid = QUuid::createUuid();
        content += "    <navPoint id=\"" + cat_uuid + "\" playOrder=\"" + QString::number(++order) + "\">\n";
        content += "      <navLabel>\n";
        content += "        <text>" + cat_it->first.name + "</text>\n";
        content += "      </navLabel>\n";
        content += "      <content src=\"" + typeFileName("cat", cat_it->first.id) + "\"/>\n";
        for (std::map<QString, QString>::iterator recipe_it = cat_it->second.begin(); recipe_it != cat_it->second.end(); ++recipe_it) {
            QString recipe_uuid = QUuid::createUuid();
            if (recipeOrder.count(recipe_it->second) == 0) {
                recipeOrder[recipe_it->second] = QString::number(++order);
            }
            content += "      <navPoint id=\"" + recipe_uuid + "\" playOrder=\"" + recipeOrder[recipe_it->second] + "\">\n";
            content += "        <navLabel>\n";
            content += "          <text>" + recipe_it->first + "</text>\n";
            content += "        </navLabel>\n";
            content += "        <content src=\"" + recipe_it->second + "\"/>\n";
            content += "      </navPoint>\n";
        }
        content += "    </navPoint>\n";
    }
    content += "  </navMap>\n";
    content += "</ncx>\n";
    return content;
}

int ExportEpub::save(QString filepath) {
    std::cout << "Exporting to EPub " << filepath.toUtf8().constData() << std::endl;
    if (QFile::exists(filepath)) {
        std::cout << "Overwriting " << filepath.toUtf8().constData() << std::endl;
    }
    KZip *zip = new KZip(filepath);
    bool result = zip->open(QIODevice::WriteOnly);
    if (!result) {
        std::cerr << "Can't open " << filepath.toUtf8().constData() << " for writting!" << std::endl;
        return 1;
    }
    zip->setCompression(KZip::DeflateCompression);

    // mimetype
    if (!zipAddString(zip, QString("application/epub+zip"), "mimetype")) {
        return 1;
    }

    QString content;
    // META-INF/container.xml
    content = "<?xml version=\"1.0\"?>\n";
    content += "<container version=\"1.0\" xmlns=\"urn:oasis:names:tc:opendocument:xmlns:container\">\n";
    content += "   <rootfiles>\n";
    content += "      <rootfile full-path=\"content.opf\" media-type=\"application/oebps-package+xml\"/>\n";
    content += "   </rootfiles>\n";
    content += "</container>";
    if (!zipAddString(zip, content, "META-INF/container.xml")) {
        return 1;
    }

    // stylesheet.css
    content = "/* stylesheet.css */\n\
.ingredients {\n\
    clear: both;\n\
}\n\
.nobr {\n\
    white-space: nowrap;\n\
}\
.photo {\n\
    float: left;\n\
}\
";
    if (!zipAddString(zip, content, "stylesheet.css")) {
        return 1;
    }

    // recipes
    for (RecipeList::iterator recipe_it = recipe_list.begin(); recipe_it != recipe_list.end(); ++recipe_it) {
        // One .html file per recipe
        content = HTMLExporter::createContent(*recipe_it);
        QString recipeFileName = QString("recipe%1.html").arg(recipe_it->recipeID, 4, 10, QChar('0'));
        if (!zipAddString(zip, content, recipeFileName)) {
            return 1;
        }
        // photo
        QString image_url = storePhoto(*recipe_it);
        QFileInfo image_path(image_url);
        QString image_relativepath = image_path.dir().dirName() + QDir::separator() + image_path.fileName();
        if (images.count(image_relativepath) == 0) {
            zip->addLocalFile(image_url, image_relativepath);
            images[image_relativepath] = "image/png";
        }
        QFile::remove(image_url);
        // OPF : List files
        files.insert(std::pair<QString, QString>(recipe_it->title, recipeFileName));
        // TOC: recipes ordered by author
        for (ElementList::const_iterator cat_it = recipe_it->authorList.begin(); cat_it != recipe_it->authorList.end(); ++cat_it) {
            recipesByAuthor[*cat_it].insert(std::pair<QString, QString>(recipe_it->title, recipeFileName));
        }
        // TOC: recipes ordered by category
        for (ElementList::const_iterator cat_it = recipe_it->categoryList.begin(); cat_it != recipe_it->categoryList.end(); ++cat_it) {
            recipesByCategory[*cat_it].insert(std::pair<QString, QString>(recipe_it->title, recipeFileName));
        }
    }
    // remove also photo directory
    QFileInfo fi(fileName());
    QDir dir(".");
    dir.rmdir(fi.absolutePath() + '/' + fi.baseName() + "_photos/");

    createTypePages(zip, "cat", recipesByCategory);
    createTypePages(zip, "aut", recipesByAuthor);

    // toc.ncx
    if (!zipAddString(zip, createTocNcx(), "toc.ncx")) {
        return 1;
    }

    // content.opf
    if (!zipAddString(zip, createContentOpf(), "content.opf")) {
        return 1;
    }

    // titlepage.xhtml
    if (!zipAddString(zip, createTitlepage(), "titlepage.xhtml")) {
        return 1;
    }

    zip->close();

    std::cout << "Exporting to EPub " << filepath.toUtf8().constData() << " done" << std::endl;
    return 0;
}

ExportEpub::~ExportEpub() {
}

QString ExportEpub::typeFileName(std::string type, int id) {
    return QString("%1%2.html").arg(type.c_str()).arg(id, 4, 10, QChar('0'));
}

// Copied from KRecipesView

void ExportEpub::initDatabase() {
    std::cout << "Initializing database" << std::endl;
    KConfigGroup config = KGlobal::config()->group("DBType");
    dbType = checkCorrectDBType(config);

    // Open the database
    database = RecipeDB::createDatabase(dbType);
    if (!database) {
        // No DB type has been enabled(should not happen at all, but just in case)
        std::cerr << "Code error. No DB support was built in. Exiting" << std::endl;
        exit(1);
    }

    database->connect();
    if (!database->ok()) {
        // Ask the user if he wants to rerun the wizard
        std::cerr << "Unable to open database : " << database->dbErr.toUtf8().constData() << std::endl;
        exit(1);
    }

    std::cout << "Nb of recipes : " << database->getCount("recipes") << std::endl;
    QList<int> ids = QList<int>();
    database->loadRecipes(&recipe_list, RecipeDB::All, ids);
}

QString ExportEpub::checkCorrectDBType(KConfigGroup &config) {
    dbType = config.readEntry("Type", "SQLite");

    if (dbType != "SQLite" && dbType != "MySQL" && dbType != "PostgreSQL") {
        std::cerr << "Unsupported database type. Database must be either MySQL, SQLite, or PostgreSQL." << std::endl;
        exit(1);
    }
    return dbType;
}

//-

bool ExportEpub::zipAddString(KZip *zip, QString content, QString filename) {
    QFile* tmpFile;
    tmpFile = new KTemporaryFile();
    if (!tmpFile->open(QIODevice::WriteOnly)) {
        std::cerr << "Can't open " << tmpFile->fileName().constData() << " for writting!" << std::endl;
        return false;
    }
    QTextStream stream(tmpFile);
    stream.setCodec("UTF-8");
    stream << content;
    tmpFile->close();
    const bool fileAdded = zip->addLocalFile(tmpFile->fileName(), filename);
    tmpFile->remove();
    if (!fileAdded) {
        std::cerr << "Can't add " << tmpFile->fileName().constData() << " to " << filename.constData() << " in zip file!" << std::endl;
        return false;
    }
    return true;
}

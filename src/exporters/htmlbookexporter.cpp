/***************************************************************************
*   Copyright (C) 2006 by                                                 *
*   Jason Kivlighn (jkivlighn@gmail.com)                                  *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
***************************************************************************/

#include "htmlbookexporter.h"

#include <qfile.h>
#include <qstylesheet.h>

#include <kdebug.h>

#include "backends/recipedb.h"
#include "datablocks/categorytree.h"

HTMLBookExporter::HTMLBookExporter( CategoryTree *categories, const QString& basedir, const QString &format ) :
		HTMLExporter( basedir+"/index", format ), m_categories(categories), m_basedir(basedir)
{
}

HTMLBookExporter::~HTMLBookExporter()
{
}

int HTMLBookExporter::headerFlags() const
{
	return RecipeDB::Categories | RecipeDB::Title;
}

QString HTMLBookExporter::createContent( const RecipeList& recipes )
{
	RecipeList::const_iterator recipe_it;
	for ( recipe_it = recipes.begin(); recipe_it != recipes.end(); ++recipe_it ) {
		for ( ElementList::const_iterator cat_it = ( *recipe_it ).categoryList.begin(); cat_it != ( *recipe_it ).categoryList.end(); ++cat_it ) {
			QMap<QString,QTextStream*>::iterator stream_it = fileMap.find( (*cat_it).name );
			(**stream_it) << "<br /><br />";
			(**stream_it) << QString("<a name=\""+(*recipe_it).title+"\" />");
			(**stream_it) << HTMLExporter::createContent(*recipe_it);
			(**stream_it) << QString("[ <a href=\"#top\">Top</a> ]");
			(**stream_it) << QString("[ <a href=\"index.html\">Back</a> ]");
			(**stream_it) << "<br /><br />";
		}
	}

	return QString::null;
}

QString HTMLBookExporter::createHeader( const RecipeList &list )
{
	QString output = HTMLExporter::createHeader(list);

	QString catLinks;
	QTextStream catLinksStream(&catLinks,IO_WriteOnly);
	createCategoryStructure(catLinksStream,list);

	return output+"<h1>Krecipes Recipes</h1><div>"+catLinks+"</li></ul></div>";
}

QString HTMLBookExporter::createFooter()
{
	QMap<QString,QTextStream*>::const_iterator it;
	for ( it = fileMap.begin(); it != fileMap.end(); ++it ) {
		(**it) << HTMLExporter::createFooter();

		(*it)->device()->close();

		//does it matter the order of deletion here?
		QIODevice *file = (*it)->device();
		delete *it;
		delete file;
	}

	QString output = HTMLExporter::createFooter();
	return output;
}

void HTMLBookExporter::createCategoryStructure( QTextStream &xml, const RecipeList &recipes )
{
	QValueList<int> categoriesUsed;
	for ( RecipeList::const_iterator recipe_it = recipes.begin(); recipe_it != recipes.end(); ++recipe_it ) {
		for ( ElementList::const_iterator cat_it = ( *recipe_it ).categoryList.begin(); cat_it != ( *recipe_it ).categoryList.end(); ++cat_it ) {
			QMap<QString,QTextStream*>::iterator stream_it = fileMap.find( (*cat_it).name );
			if ( categoriesUsed.find( ( *cat_it ).id ) == categoriesUsed.end() ) {
				categoriesUsed << ( *cat_it ).id;

				QString catPageName = m_basedir+"/"+escape((*cat_it).name)+".html";
				QFile *catPage = new QFile( catPageName );
				catPage->open( IO_WriteOnly );
				QTextStream *stream = new QTextStream( catPage );
				stream->setEncoding( QTextStream::UnicodeUTF8 );
				(*stream) << HTMLExporter::createHeader(recipes);
				(*stream) << QString("<a name=\"top\" />");
				(*stream) << "<h1>"<<(*cat_it).name<<"</h1>";

				stream_it = fileMap.insert((*cat_it).name,stream);
			}
			(**stream_it) << QString("[ <a href=\"#" + (*recipe_it).title + "\">" + (*recipe_it).title + "</a> ]");
		}
	}

	if ( !categoriesUsed.empty() ) {
		//only keep the relevant category structure
		removeIfUnused( categoriesUsed, m_categories );

		xml << "<ul>\n";
		writeCategoryStructure( xml, m_categories );
		xml << "</ul>\n";
	}
}

bool HTMLBookExporter::removeIfUnused( const QValueList<int> &cat_ids, CategoryTree *parent, bool parent_should_show )
{
	for ( CategoryTree * it = parent->firstChild(); it; it = it->nextSibling() ) {
		if ( cat_ids.find( it->category.id ) != cat_ids.end() ) {
			parent_should_show = true;
			removeIfUnused( cat_ids, it, true ); //still recurse, but doesn't affect 'parent'
		}
		else {
			bool result = removeIfUnused( cat_ids, it );
			if ( parent_should_show == false )
				parent_should_show = result;
		}
	}

	if ( !parent_should_show && parent->category.id != -1 ) {
		//FIXME: CategoryTree is broken when deleting items
		//delete parent;

		parent->category.id = -2; //temporary workaround
	}

	return parent_should_show;
}

void HTMLBookExporter::writeCategoryStructure( QTextStream &xml, const CategoryTree *categoryTree )
{
	if ( categoryTree->category.id != -2 ) {
		if ( categoryTree->category.id != -1 ) {
			QString catPageName = QStyleSheet::escape(categoryTree->category.name)+".html";

			xml << "\t<li>\n\t\t<a href=\""+catPageName+"\">"+QStyleSheet::escape( categoryTree->category.name ).replace("\"","&quot;") + "</a>\n";
		}
	
		for ( CategoryTree * child_it = categoryTree->firstChild(); child_it; child_it = child_it->nextSibling() ) {
			if ( categoryTree->parent() != 0 )
				xml << "<ul>\n";
			writeCategoryStructure( xml, child_it );
			if ( categoryTree->parent() != 0 )
				xml << "</ul>\n";
		}
	
		if ( categoryTree->category.id != -1 )
			xml << "\t</li>\n";
	}
}
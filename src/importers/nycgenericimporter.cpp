/***************************************************************************
 *   Copyright (C) 2003 by krecipes.sourceforge.net authors                *
 *                                                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "nycgenericimporter.h"

#include <kapplication.h>

#include <qfile.h>
#include <qtextstream.h>
#include <qstringlist.h>

#include "mixednumber.h"
#include "recipe.h"

/* SAMPLE:
Apple Cider Pie

apple, fruits, pies

1 cup sugar
1 cup apples; cored & sliced
1 cup apple cider
1 teaspoon cornstarch
1 tablespoon lemon juice
1 teaspoon vanilla
1 teaspoon margarine
1 pie crust, double
1/2 teaspoon Cinnamon

Combine sugar and cider in a sauce pan. Bring to a boil and add apples. Simmer, stirring occasionally, for 5 minutes. Remove from stove. Drain juice off to measure 1 1/2 cups. (If you do not have enough juice, add some cider).


In a saucepan, combine juice with cornstarch, lemon juice, vanilla, and butter. Bring to a boil and stir until it thickens and turns clear. Sprinkle 1/4 Tsp. of cinnamon over bottom pie crust, add drained apples, and cider mixture. Sprinkle with remaining 1/4 tsp. cinnamon. Top with pie crust. Slash the crust. Bake at 400 degrees for 45-50 minutes, until golden brown.


WARNING: I used a deep dish pie pan, but still it bubbled over, so put something under it to catch the drips!

Contributor: Senora Hauser
Yield:  1 serving
Preparation Time: 00:05

 NYC Nutrilink: N5504^19335,N1767^09004,N2032^09400,N5614^20027
 NYC Nutrilink: N1896^09153,N221^02050,N599^04522,N5063^18402
 NYC Nutrilink: N182^02010

** Exported from Now You're Cooking! v5.59 **
*/

NYCGenericImporter::NYCGenericImporter( const QString &file ) : BaseImporter()
{
	first = true;

	resetVars();

	QFile input( file );
	qDebug("loading file: %s",file.latin1());
	if ( input.open( IO_ReadOnly ) )
	{
		QTextStream stream( &input );
		qDebug("file opened");

		while ( !stream.atEnd() )
		{
			if ( stream.readLine().startsWith("@@@@@") )
			{
				importNYCGeneric( stream );
				break;
			}
		}
		error_code = 0;
	}
	else
		error_code = FileOpenError;
}

NYCGenericImporter::~NYCGenericImporter()
{
}

void NYCGenericImporter::importNYCGeneric( QTextStream &stream )
{
	kapp->processEvents(); //don't want the user to think its frozen... especially for files with thousands of recipes

	QString current;

	stream.skipWhiteSpace();

	//title
	while ( (current = stream.readLine() ) != "" && !stream.atEnd() )
		m_title = current;
	qDebug("Found title: %s",m_title.latin1());

	//categories
	while ( (current = stream.readLine() ) != "" && !stream.atEnd() )
	{
		if ( current[0].isNumber() ){ loadIngredientLine(current); break; } //oops, this is really an ingredient line (there was no category line)

		QStringList categories = QStringList::split(',',current);
		for ( QStringList::const_iterator it = categories.begin(); it != categories.end(); ++it )
		{
			Element new_cat;
			new_cat.name = QString(*it).stripWhiteSpace();
			qDebug("Found category: %s", new_cat.name.latin1() );
			m_categories.add( new_cat );
		}
	}

	//ingredients
	while ( (current = stream.readLine() ) != "" && !stream.atEnd() )
		loadIngredientLine(current);

	//everything else is the instructions
	bool found_next;
	while ( !(found_next = ( current = stream.readLine() ).startsWith("@@@@@")) && !stream.atEnd() )
	{
		if ( current.startsWith("Contributor:") )
		{
			Element new_author;
			new_author.name = current.mid(current.find(':')+1,current.length()).stripWhiteSpace();
			qDebug("Found author: %s", new_author.name.latin1() );
			m_authors.add( new_author );
		}
		else if ( current.startsWith("Preparation Time:") )
		{
			;//TODO: what do we do with this?
		}
		else if ( current.startsWith("Yield:") )
		{
			m_servings = current.mid(current.find(':'),current.length()).toInt();
			qDebug("Found servings: %d", m_servings );
		}
		else
		{
			qDebug("Found instruction line: %s",current.latin1());
			m_instructions += current+"\n";
		}
	}

	m_instructions = m_instructions.stripWhiteSpace();
	putDataInRecipe();

	if ( found_next )
		importNYCGeneric(stream);
}

void NYCGenericImporter::putDataInRecipe()
{
	//create the recipe
	Recipe *new_recipe = new Recipe;
	new_recipe->persons = m_servings;
	new_recipe->title = m_title;
	new_recipe->instructions = m_instructions;
	new_recipe->ingList = m_ingredients;
	new_recipe->categoryList = m_categories;
	new_recipe->authorList = m_authors;
	new_recipe->recipeID = -1;

	//put it in the recipe list
	add( new_recipe );

	//reset for the next recipe to use these variables
	resetVars();
}

void NYCGenericImporter::resetVars()
{
	m_ingredients.empty();
	m_authors.clear();
	m_categories.clear();

	m_servings = 0;

	m_title = QString::null;
	m_instructions = QString::null;
}

/*
fix headers: ----SOUP----
fix for ingredients like:
salt to taste
pepper to taste
*/
void NYCGenericImporter::loadIngredientLine( const QString &line )
{
	QString current = line;

	// We can't really do anything with the ingredient header... just add it as an ingredient for now
	if ( current.contains("-----") )
	{
		Ingredient new_ingredient;
		new_ingredient.amount = 0;
		new_ingredient.units = "";
		new_ingredient.name = current.stripWhiteSpace();

		m_ingredients.add(new_ingredient);
		qDebug("Found ingredient header %s",new_ingredient.name.latin1());
		return;
	}

	MixedNumber amount(0,0,1);
	QString unit;
	QString name;
	QString prep;

	QStringList ingredient_line = QStringList::split(' ', current );

	if ( !ingredient_line.empty() ) //probably an unnecessary check... but to be safe
	{
		bool ok;
		MixedNumber test_amount = MixedNumber::fromString(ingredient_line[0],&ok);
		if ( ok )
		{
			amount = amount + test_amount;
			ingredient_line.pop_front();
		}
	}
	if ( !ingredient_line.empty() ) //probably an unnecessary check... but to be safe
	{
		bool ok;
		MixedNumber test_amount = MixedNumber::fromString(ingredient_line[0],&ok);
		if ( ok )
		{
			amount = amount + test_amount;
			ingredient_line.pop_front();
		}
	}

	name = ingredient_line.join(" ");

	/*uncomment this when Krecipes uses preparation method
	int prep_sep_index = name.find(QRegExp("[,;]"));
	name = name.left( prep_sep_index );
	prep = name.mid( prep_sep_index, name.length() );
	*/

	Ingredient new_ingredient;
	new_ingredient.amount = amount.toDouble();
	new_ingredient.units = unit;
	new_ingredient.name = name;

	m_ingredients.add(new_ingredient);
	qDebug("Found ingredient: amount=%f, unit:%s, name:%s",
	  new_ingredient.amount,
	  new_ingredient.units.latin1(),
	  new_ingredient.name.latin1());
}


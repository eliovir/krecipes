/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Jason Kivlighn (jkivlighn@gmail.com)                                  *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
***************************************************************************/

#include <kapplication.h>

#include <qstring.h>
#include <qfile.h>

#include <iostream>

#include "kreimporter.h"
#include "kreexport.h"
#include "importertest.h"
#include "exportertest.h"

int
main(int argc, char *argv[])
{
	KApplication a(argc, argv, "kretest");

	printf("Creating KreImporter.\n");
	KreImporter importer;

	printf("Parsing kretest.txt.\n");
	QStringList files; files << "kretest.txt";
	importer.parseFiles(files);

	Recipe recipe;
	recipe.title = "Cookies_Test";
	recipe.yield.amount = 2;
	recipe.yield.amount_offset = 1;
	recipe.yield.type = "dozen";
	recipe.categoryList.append( Element("Snacks",1) );
	recipe.categoryList.append( Element("Cookies & Squares",2) );
	recipe.instructions = 
		"Drop by spoonful on greased cookie sheet.  Bake in moderate oven.";
	recipe.prepTime = QTime(0,30);
	if ( !recipe.photo.load( "test_photo.jpg", "JPEG" ) ) {
		printf("Unable to load test_photo.jpg\n");
		exit(1);
	}

	recipe.authorList.append( Element("Colleen Beamer") );
	recipe.authorList.append( Element("Mona Beamer") );

	Ingredient ing;
	ing.name = "granulated sugar";
	ing.amount = 0.75;
	ing.amount_offset = 0.25;
	ing.units.name = "c.";
	ing.groupID = 0; ing.group = "Dry Ingredients";
	recipe.ingList.append( ing );

	Ingredient ing2;
	ing2.name = "brown sugar";
	ing2.amount = 1;
	ing2.amount_offset = 0;
	ing2.units.name = "c.";
	ing2.groupID = 0; ing2.group = "Dry Ingredients";
	recipe.ingList.append( ing2 );

	Ingredient ing3;
	ing3.name = "all-purpose flour";
	ing3.amount = 2;
	ing3.units.plural = "c.";
	ing3.groupID = 0; ing3.group = "Dry Ingredients";
	recipe.ingList.append( ing3 );

	Ingredient ing4;
	ing4.name = "baking soda";
	ing4.amount = 1;
	ing4.amount_offset = 0;
	ing4.units.name = "tsp.";
	ing4.groupID = 0; ing4.group = "Dry Ingredients";
	recipe.ingList.append( ing4 );

	Ingredient ing8;
	ing8.name = "shortening";
	ing8.amount = 1;
	ing8.amount_offset = 0;
	ing8.units.name = "c.";
	ing8.prepMethodList.append( Element("softened") );
	ing8.prepMethodList.append( Element("at room temperature") );
	ing8.groupID = 1; ing8.group = "Fat & Liquids";
	recipe.ingList.append( ing8 );

	Ingredient ing6;
	ing6.name = "peanut butter";
	ing6.amount = 1;
	ing6.amount_offset = 0;
	ing6.units.name = "c.";
	ing6.groupID = 1; ing6.group = "Fat & Liquids";
	recipe.ingList.append( ing6 );

	Ingredient ing5;
	ing5.name = "eggs";
	ing5.amount = 2;
	ing5.amount_offset = 0;
	ing5.units.plural = "";
	ing5.groupID = 1; ing5.group = "Fat & Liquids";
	recipe.ingList.append( ing5 );

	Ingredient ing7;
	ing7.name = "vanilla extract";
	ing7.amount = 1;
	ing7.amount_offset = 0;
	ing7.units.name = "tsp.";
	ing7.groupID = 1; ing7.group = "Fat & Liquids";
	recipe.ingList.append( ing7 );

	CategoryTree *catTree = new CategoryTree;
	(void)catTree->add( Element("Cookies & Squares",2) );
	(void)catTree->add( Element("Snacks",1) );
	
	check( importer, recipe );
	check( importer, catTree );


	RecipeList recipeList;
	recipeList.append(recipe);
	recipeList.append(recipe);

	printf("Creating KreExporter.\n");
	KreExporter exporter(catTree,"not needed",".kreml");
	check( exporter, recipeList );
	printf("Successfully exported recipes to test.txt.\n");

	printf("Creating KreImporter to test exported recipes.\n");
	KreImporter importer2;

	printf("Parsing test.txt.\n");
	QStringList files2; files2 << "test.txt";
	importer2.parseFiles(files2);
	QFile::remove("test.txt");
	check( importer2, recipe );
	check( importer2, catTree );
	printf("Recipe export successful.\n");

	printf("*** Krecipes importer and exporter passed the tests :-) ***\n");
}
/***************************************************************************
 *   Copyright (C) 2003 by                                                 *
 *   Unai Garro (ugarro@users.sourceforge.net)                             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#ifndef INGREDIENTMATCHERDIALOG_H
#define INGREDIENTMATCHERDIALOG_H

#include "element.h"
#include "recipe.h"

#include <qlistview.h>
#include <qpushbutton.h>
#include <qvbox.h>

class KreListView;
class RecipeDB;


/**
@author Unai Garro
*/

class IngredientListItem:public QCheckListItem
{
public:
	IngredientListItem(QListView* qlv, const Element &ing ):QCheckListItem(qlv,QString::null,QCheckListItem::CheckBox)
	{
	// Initialize the ingredient data with the the property data
		ingStored=new Element();
		ingStored->id=ing.id;
		ingStored->name=ing.name;
	}

	~IngredientListItem(void)
	{
	delete ingStored;
	}
	int id(void){return ingStored->id;}
	QString name(void){return ingStored->name;}

private:
	Element *ingStored;

public:
	virtual QString text(int column) const
		{
		if (column==1) return(ingStored->name);
		else return(QString::null);
		}
};

class RecipeListItem:public QListViewItem
{
public:
	RecipeListItem(QListView* qlv, const Recipe &r ):QListViewItem(qlv)
	{
	// Initialize the ingredient  data with the the property data
		recipeStored=new Recipe();
		recipeStored->recipeID=r.recipeID;
		recipeStored->title=r.title;
	}

	~RecipeListItem(void)
	{
	delete recipeStored;
	}

private:
	Recipe *recipeStored;

public:
	virtual QString text(int column) const
		{
		if (column==0) return(recipeStored->title);
		else return(QString::null);
		}
};

class IngredientMatcherDialog:public QVBox{

Q_OBJECT

public:

    IngredientMatcherDialog(QWidget *parent, RecipeDB* db);
    ~IngredientMatcherDialog();
    void reloadIngredients (void);

private:
	//Private variables
	RecipeDB *database;
	
	//Widgets
	KreListView *ingredientListView;
	KreListView *recipeListView;
	QPushButton *okButton;
	QPushButton *clearButton;
private slots:
	void findRecipes(void);

};
#endif
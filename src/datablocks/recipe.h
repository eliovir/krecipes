/***************************************************************************
*   Copyright © 2003-2004 Unai Garro <ugarro@users.sourceforge.net>       *
*   Copyright © 2003-2004 Jason Kivlighn <jkivlighn@gmail.com>            *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
***************************************************************************/
#ifndef RECIPE_H
#define RECIPE_H

#include <QString>
#include <QPixmap>
#include <QDateTime>

#include "ingredientlist.h"
#include "datablocks/rating.h"
#include "datablocks/elementlist.h"
#include "datablocks/ingredientpropertylist.h"

class Yield
{
public:
	Yield() : amount(0), amount_offset(0), type(QString::null), type_id(-1){}

	QString amountToString() const;
	QString toString() const;

	double amount;
	double amount_offset;
	QString type;
	int type_id;
};

/**
@author Unai Garro
*/
class Recipe
{
public:
	Recipe();
	~Recipe();
	// Public variables

	int recipeID;
	Yield yield;
	QString title;
	QString instructions;
	QPixmap photo;
	IngredientList ingList;
	ElementList categoryList; // id+name
	ElementList authorList; //authors' id+name
	QTime prepTime;

	QDateTime ctime;
	QDateTime mtime;
	QDateTime atime;

	RatingList ratingList;
	IngredientPropertyList properties;

	// Public methods
	void empty( void );



};

#endif

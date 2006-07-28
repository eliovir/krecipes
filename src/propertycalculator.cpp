/***************************************************************************
*   Copyright (C) 2003 by Unai Garro                                      *
*   ugarro@users.sourceforge.net                                          *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
***************************************************************************/
#include "propertycalculator.h"

#include <math.h> // For fabs()

#include <kdebug.h>

#include "backends/recipedb.h"
#include "datablocks/elementlist.h"
#include "datablocks/ingredientpropertylist.h"
#include "datablocks/recipe.h"

bool autoConvert( RecipeDB* database, const Ingredient &from, const Ingredient &to, Ingredient &result )
{
	RecipeDB::ConversionStatus status = database->convertIngredientUnits( from, to.units, result );
	bool converted = status == RecipeDB::Success || status == RecipeDB::MismatchedPrepMethodUsingApprox;

	if ( converted )  // There is a ratio
	{
		double ratio = result.amount / from.amount;

		if ( ratio > 1 )  // Convert to unit 1, since unit1 is bigger
		{
			result.units = from.units;
			result.amount = from.amount + to.amount / ratio;
		}
		else { //Convert to unit2, since unit2 is bigger (just add, units are now correct)
			result.amount += to.amount;
		}
		return true;
	}
	else
		return false;
}

/*
** Version with database I/O. DB must be provided
*/

void calculateProperties( Recipe& recipe, RecipeDB* database )
{
	recipe.properties.clear();
	// Note that recipePropertyList is not attached to any ingredient. It's just the total of the recipe
	IngredientPropertyList ingredientPropertyList; // property list for each ingredient

	int ingredientNo = 1;

	for ( IngredientList::const_iterator ing_it = recipe.ingList.begin(); ing_it != recipe.ingList.end(); ++ing_it ) {
		database->loadProperties( &ingredientPropertyList, ( *ing_it ).ingredientID );
		ingredientPropertyList.divide( recipe.yield.amount ); // calculates properties per yield unit
		addPropertyToList( database, &recipe.properties, ingredientPropertyList, *ing_it, ingredientNo );
		ingredientNo++;
	}
}


void addPropertyToList( RecipeDB *database, IngredientPropertyList *recipePropertyList, IngredientPropertyList &ingPropertyList, const Ingredient &ing, int ingredientNo )
{
	QMap<int,double> ratioCache; //unit->ratio

	IngredientPropertyList::const_iterator prop_it;
	for ( prop_it = ingPropertyList.begin(); prop_it != ingPropertyList.end(); ++prop_it ) {
		// Find if property was listed before
		int pos = recipePropertyList->findIndex( *prop_it );
		if ( pos >= 0 )  //Exists. Add to it
		{
			IngredientPropertyList::iterator rec_property_it = recipePropertyList->at( pos );
			Ingredient result;

			bool converted;
			QMap<int,double>::const_iterator cache_it = ratioCache.find((*prop_it).perUnit.id);
			if ( cache_it == ratioCache.end() ) {
				RecipeDB::ConversionStatus status = database->convertIngredientUnits( ing, (*prop_it).perUnit, result );
				converted = status == RecipeDB::Success || status == RecipeDB::MismatchedPrepMethodUsingApprox;

				if ( converted )
					ratioCache.insert((*prop_it).perUnit.id,result.amount / ing.amount);
				else
					ratioCache.insert((*prop_it).perUnit.id,-1);
			}
			else {
				result.units = (*prop_it).perUnit;
				result.amount = ing.amount * (*cache_it);
				converted = result.amount > 0;
			}

			if ( converted )  // Could convert units to perUnit
				(*rec_property_it).amount += ( (*prop_it).amount ) * result.amount;
		}
		else // Append new property
		{
			IngredientProperty property;
			property.id = (*prop_it).id;
			property.name = (*prop_it).name;
			property.perUnit.id = -1; // It's not per unit, it's total sum of the recipe
			property.perUnit.name = QString::null; // "
			property.units = (*prop_it).units;

			Ingredient result;
			bool converted;
			QMap<int,double>::const_iterator cache_it = ratioCache.find((*prop_it).perUnit.id);
			if ( cache_it == ratioCache.end() ) {
				RecipeDB::ConversionStatus status = database->convertIngredientUnits( ing, (*prop_it).perUnit, result );
				converted = status == RecipeDB::Success || status == RecipeDB::MismatchedPrepMethodUsingApprox;
				if ( converted )
					ratioCache.insert((*prop_it).perUnit.id,result.amount / ing.amount);
				else
					ratioCache.insert((*prop_it).perUnit.id,-1);
			}
			else {
				result.units = (*prop_it).perUnit;
				result.amount = ing.amount * (*cache_it);
				converted = result.amount > 0;
			}

			if ( converted )  // Could convert units to perUnit
			{
				property.amount = ( (*prop_it).amount ) * result.amount;
				recipePropertyList->append( property );
			}
		}
	}
}

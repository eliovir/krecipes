/***************************************************************************
 *   Copyright (C) 2003 by Unai Garro                                      *
 *   ugarro@users.sourceforge.net                                          *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "ingredientproperty.h"

IngredientProperty::IngredientProperty()
{
id=-1;
ingredientID=-1;
amount=0.0;
}

IngredientProperty::IngredientProperty(const QString &_name, const QString &_units, int _id) :
  name(_name),
  units(_units),
  id(_id)
{
}

IngredientProperty::~IngredientProperty()
{
}


/***************************************************************************
*   Copyright (C) 2003 by Unai Garro                                      *
*   ugarro@users.sourceforge.net                                          *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
***************************************************************************/
#ifndef INGREDIENTPROPERTYLIST_H
#define INGREDIENTPROPERTYLIST_H
#include <qptrlist.h>
#include "datablocks/ingredientproperty.h"

/**
@author Unai Garro
*/

class PropertyPtrList: public QPtrList <IngredientProperty>
{
public:
	PropertyPtrList()
	{
		setAutoDelete( true );
	};
	~PropertyPtrList()
	{}
	;
protected:
	virtual int compareItems( QPtrCollection::Item item1, QPtrCollection::Item item2 )
	{
		return ( ( ( IngredientProperty* ) item1 ) ->id - ( ( IngredientProperty* ) item2 ) ->id );
	};

};


class IngredientPropertyList
{
public:
	IngredientPropertyList();

	~IngredientPropertyList();

	void add
		( IngredientProperty &element );
	void append( IngredientProperty *property );
	IngredientProperty* at( int pos );
	void clear( void );
	int count( void );
	void divide( int persons );

	int find( IngredientProperty* it );
	int find( int id );
	int findByName( const QString & );
	void filter( int ingredientID, IngredientPropertyList *filteredList );
	IngredientProperty* getFirst( void );
	IngredientProperty* getNext( void );
	IngredientProperty* getElement( int index );
	bool isEmpty( void );
	void remove
		( IngredientProperty* ip );

private:
	PropertyPtrList list;

};

#endif
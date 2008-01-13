/***************************************************************************
*   Copyright (C) 2003 by                                                 *
*   Unai Garro (ugarro@users.sourceforge.net)                             *
*   Cyril Bosselut (bosselut@b1project.com)                               *
*   Jason Kivlighn (jkivlighn@gmail.com)                                  *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
***************************************************************************/

/**
@author Unai Garro
*/

#ifndef CREATEPROPERTYDIALOG_H
#define CREATEPROPERTYDIALOG_H

#include <qcombobox.h>
#include <qlayout.h>
#include <qpushbutton.h>
#include <q3groupbox.h>
#include <q3vbox.h>
#include <qlabel.h>

#include <klineedit.h>
#include <kdialog.h>

#include "datablocks/unit.h"

class CreatePropertyDialog : public KDialog
{
public:
	CreatePropertyDialog( QWidget *parent, UnitList *list );
	~CreatePropertyDialog();
	QString newPropertyName( void );
	QString newUnitsName( void );


private:
	//Widgets
	Q3GroupBox *box;
	KLineEdit *propertyNameEdit;
	KLineEdit *propertyUnits;
	QLabel *nameEditText;
	QLabel *unitsText;

	//Internal variables
	UnitList *unitList;

	//Methods
	void loadUnits();
};

#endif

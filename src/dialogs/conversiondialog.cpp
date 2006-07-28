/***************************************************************************
*   Copyright (C) 2006 by                                                 *
*   Jason Kivlighn (jkivlighn@gmail.com)                                  *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
***************************************************************************/

#include "conversiondialog.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <kcombobox.h>
#include <qlabel.h>
#include <klineedit.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qvbox.h>

#include <kcombobox.h>
#include <klineedit.h>
#include <klocale.h>

#include "backends/recipedb.h"
#include "widgets/unitcombobox.h"
#include "widgets/ingredientcombobox.h"
#include "widgets/prepmethodcombobox.h"
#include "widgets/fractioninput.h"

ConversionDialog::ConversionDialog( QWidget* parent, RecipeDB *db, const char* name )
		: KDialogBase( parent, name, false, i18n( "Measurement Converter" ),
		    KDialogBase::Close | KDialogBase::User1 | KDialogBase::Help, KDialogBase::Close ),
		m_database(db)
{
	setHelp("measurement-converter");
	setButtonText( KDialogBase::User1, i18n("Convert") );

	setSizeGripEnabled( TRUE );

	QVBox *page = makeVBoxMainWidget();
	
	QHBox *vbox = new QVBox(page);

	QHBox *fromTopBox = new QHBox(vbox);
	convertLabel = new QLabel( fromTopBox, "convertLabel" );

	amountEdit = new FractionInput( fromTopBox );

	fromUnitBox = new UnitComboBox( fromTopBox, db );
	fromUnitBox->reload();
	fromTopBox->setStretchFactor( fromUnitBox, 2 );
	fromTopBox->setSpacing(3);

	QHBox *fromBottomBox = new QHBox(vbox);
	
	ingredientBox = new IngredientComboBox( FALSE, fromBottomBox, db, i18n( "--Ingredient (optional)--" ) );
	ingredientBox->reload();

	prepMethodBox = new PrepMethodComboBox( false, fromBottomBox, db, i18n( "-No Preparation-" ) );
	prepMethodBox->reload();
	fromBottomBox->setSpacing(3);
	
	QHBox *toBox = new QHBox(vbox);

	toLabel = new QLabel( toBox, "toLabel" );

	toUnitBox = new UnitComboBox( toBox, db );
	toUnitBox->reload();
	toBox->setStretchFactor( toUnitBox, 2 );
	toBox->setSpacing(8);

	QHBox *resultBox = new QHBox(vbox);
	resultLabel = new QLabel( resultBox, "resultLabel" );
	resultText = new QLabel( resultBox, "resultText" );
	resultBox->setStretchFactor( resultText, 2 );

	languageChange();

	setInitialSize( QSize(300, 200).expandedTo(minimumSizeHint()) );
	
	// signals and slots connections
	connect ( this, SIGNAL( closeClicked() ), this, SLOT( accept() ) );
}

ConversionDialog::~ConversionDialog()
{
}

void ConversionDialog::languageChange()
{
	convertLabel->setText( i18n( "Convert" ) );
	toLabel->setText( i18n( "To" ) );
	resultLabel->setText( i18n( "<b>Result:</b>" ) );
	resultText->setText( QString::null );
}

void ConversionDialog::slotUser1()
{
	convert();
}

void ConversionDialog::convert()
{
	Ingredient result, ing;
	Unit unit = m_database->unitName(toUnitBox->id(toUnitBox->currentItem()));

	ing.amount = amountEdit->value().toDouble();
	ing.ingredientID = ingredientBox->id(ingredientBox->currentItem());
	ing.units = m_database->unitName(fromUnitBox->id(fromUnitBox->currentItem()));

	int prepID = prepMethodBox->id(prepMethodBox->currentItem());
	if ( prepID != -1 )
		ing.prepMethodList.append(Element(QString::null,prepID));

	switch ( m_database->convertIngredientUnits( ing, unit, result ) ) {
	case RecipeDB::Success:
		resultLabel->setText( i18n( "<b>Result:</b>" ) );
		resultText->setText(QString::number(result.amount)+" "+((result.amount>1)?result.units.plural:result.units.name));
		break;
	case RecipeDB::MismatchedPrepMethod:
		resultLabel->setText( i18n( "<b>Approximated result:</b>" ) );
		resultText->setText(QString::number(result.amount)+" "+((result.amount>1)?result.units.plural:result.units.name));
		break;
	case RecipeDB::MissingUnitConversion:
		resultLabel->setText( i18n( "<b>Error:</b>" ) );
		resultText->setText( i18n("Missing unit conversion") );
		break;
	case RecipeDB::MissingIngredientWeight:
		resultLabel->setText( i18n( "<b>Error:</b>" ) );
		resultText->setText( i18n("No ingredient weight available") );
		break;
	case RecipeDB::MissingIngredient:
		resultLabel->setText( i18n( "<b>Error:</b>" ) );
		resultText->setText( i18n("Ingredient required for conversion") );
		break;
	case RecipeDB::InvalidTypes:
		resultLabel->setText( i18n( "<b>Error:</b>" ) );
		resultText->setText( i18n("Impossible unit conversion based on unit types") );
		break;
	}
}

#include "conversiondialog.moc"
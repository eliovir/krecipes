/***************************************************************************
*   Copyright (C) 2005 by                                                 *
*   Jason Kivlighn (jkivlighn@gmail.com)                                  *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
***************************************************************************/

#include "ingredientparserdialog.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qtextedit.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qheader.h>
#include <qapplication.h>
#include <qclipboard.h>

#include <klocale.h>
#include <kdebug.h>
#include <klistview.h>
#include <kpushbutton.h>
#include <kmessagebox.h>
#include <kaction.h>
#include <kpopupmenu.h>

#include "datablocks/mixednumber.h"
#include "widgets/inglistviewitem.h"

IngredientParserDialog::IngredientParserDialog( const UnitList &units, QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl ), m_unitList(units)
{
	if ( !name )
		setName( "IngredientParserDialog" );
	IngredientParserDialogLayout = new QHBoxLayout( this, 11, 6, "IngredientParserDialogLayout"); 
	
	layout4 = new QVBoxLayout( 0, 0, 6, "layout4"); 
	
	textLabel1 = new QLabel( this, "textLabel1" );
	textLabel1->setTextFormat( QLabel::RichText );
	layout4->addWidget( textLabel1 );
	
	ingredientTextEdit = new QTextEdit( this, "ingredientTextEdit" );
	ingredientTextEdit->setTextFormat( QTextEdit::PlainText );
	layout4->addWidget( ingredientTextEdit );
	
	parseButton = new KPushButton( this, "parseButton" );
	layout4->addWidget( parseButton );

	previewLabel = new QLabel( this, "previewLabel" );
	previewLabel->setTextFormat( QLabel::RichText );
	layout4->addWidget( previewLabel );
	
	previewIngView = new KListView( this, "previewIngView" );
	previewIngView->setSorting(-1);
	previewIngView->addColumn( i18n( "Ingredient" ) );
	previewIngView->addColumn( i18n( "Amount" ) );
	previewIngView->addColumn( i18n( "Unit" ) );
	previewIngView->addColumn( i18n( "Preparation Method" ) );
	layout4->addWidget( previewIngView );
	IngredientParserDialogLayout->addLayout( layout4 );
	
	Layout5 = new QVBoxLayout( 0, 0, 6, "Layout5"); 
	
	buttonOk = new QPushButton( this, "buttonOk" );
	buttonOk->setAutoDefault( TRUE );
	buttonOk->setDefault( TRUE );
	buttonOk->setEnabled( false );
	Layout5->addWidget( buttonOk );
	
	buttonCancel = new QPushButton( this, "buttonCancel" );
	buttonCancel->setAutoDefault( TRUE );
	Layout5->addWidget( buttonCancel );
	
	//buttonHelp = new QPushButton( this, "buttonHelp" );
	//buttonHelp->setAutoDefault( TRUE );
	//Layout5->addWidget( buttonHelp );
	Spacer1 = new QSpacerItem( 20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
	Layout5->addItem( Spacer1 );

	buttonGroup = new QPushButton( this, "buttonGroup" );
	QWhatsThis::add( buttonGroup, 
			i18n("If an ingredient header is detected as an ingredient, select it and click this button so that Krecipes will recognize it as a header.  All the ingredients below the header will be included within that group.\n\nAlternatively, if you select multiple ingredients and click this button, those ingredients will be grouped together.")
	);
	Layout5->addWidget( buttonGroup );

	IngredientParserDialogLayout->addLayout( Layout5 );
	languageChange();
	resize( QSize(577, 371).expandedTo(minimumSizeHint()) );
	clearWState( WState_Polished );

	previewIngView->setItemsRenameable( true );
	previewIngView->setRenameable( 0, true );
	previewIngView->setRenameable( 1, true );
	previewIngView->setRenameable( 2, true );
	previewIngView->setRenameable( 3, true );

	previewIngView->setSelectionMode( QListView::Extended );

	ingredientTextEdit->setText( QApplication::clipboard()->text() );
	ingredientTextEdit->selectAll();

	KPopupMenu *kpop = new KPopupMenu( previewIngView );
	kpop->insertItem( i18n( "&Delete" ), this, SLOT( removeIngredient() ), Key_Delete );
	kpop->insertItem( i18n("&Group Ingredients") , this, SLOT( convertToHeader() ) );
 
	connect( parseButton, SIGNAL(clicked()), this, SLOT(parseText()) );
	connect( buttonGroup, SIGNAL(clicked()), this, SLOT(convertToHeader()) );

	connect( buttonOk, SIGNAL(clicked()), this, SLOT(accept()) );
	connect( buttonCancel, SIGNAL(clicked()), this, SLOT(reject()) );
	//connect( buttonHelp, SIGNAL(clicked()), this, SLOT(dunno()) );
}

IngredientParserDialog::~IngredientParserDialog()
{
    // no need to delete child widgets, Qt does it all for us
}

void IngredientParserDialog::languageChange()
{
	setCaption( i18n( "Ingredient Parser" ) );
	textLabel1->setText( i18n( "To use: Paste a list of ingredient below, click \"Parse Text\", and then you may correct any incorrectly parsed ingredients.<br><b>Caution: Fields will be truncated if longer than the database allows</b>" ) );
	previewLabel->setText( i18n("Ingredients as understood by Krecipes:") );
	parseButton->setText( i18n( "Parse Text" ) );
	previewIngView->header()->setLabel( 0, i18n( "Ingredient" ) );
	previewIngView->header()->setLabel( 1, i18n( "Amount" ) );
	previewIngView->header()->setLabel( 2, i18n( "Unit" ) );
	previewIngView->header()->setLabel( 3, i18n( "Preparation Method" ) );
	buttonOk->setText( i18n( "&OK" ) );
	buttonOk->setAccel( QKeySequence( QString::null ) );
	buttonCancel->setText( i18n( "&Cancel" ) );
	buttonCancel->setAccel( QKeySequence( QString::null ) );
	buttonGroup->setText( i18n("&Group Ingredients") );
	//buttonHelp->setText( i18n( "&Help" ) );
	//buttonHelp->setAccel( QKeySequence( i18n( "F1" ) ) );
}

void IngredientParserDialog::accept()
{
	for ( QListViewItem *it = previewIngView->firstChild(); it; it = it->nextSibling() ) {
		if ( it->rtti() == INGGRPLISTVIEWITEM_RTTI ) {
			QString group = ((IngGrpListViewItem*)it)->group();
			for ( IngListViewItem *sub_it = (IngListViewItem*)it->firstChild(); sub_it; sub_it = (IngListViewItem*)sub_it->nextSibling() ) {
				Ingredient ing = sub_it->ingredient();
				ing.group = group;
				m_ingList.append(ing);
			}
		}
		else
			m_ingList.append(((IngListViewItem*)it)->ingredient());
	}

	QDialog::accept();
}

void IngredientParserDialog::removeIngredient()
{
	delete previewIngView->selectedItem();
	if ( !previewIngView->firstChild() )
		buttonOk->setEnabled( false );
}

void IngredientParserDialog::convertToHeader()
{
	QPtrList<QListViewItem> items = previewIngView->selectedItems();
	if ( items.count() == 0 )
		return;
	else if ( items.count() > 1 )
		convertToHeader(items);
	else { //items.count = 1
		QListViewItem *item = items.first();
		if ( item->rtti() == INGLISTVIEWITEM_RTTI ) {
			QListViewItem *new_item = new IngGrpListViewItem(previewIngView,
			(item->parent())?item->parent():item,
			((IngListViewItem*)item)->ingredient().name, -1);
	
			QListViewItem *next_sibling;
			QListViewItem *last_item = 0;
			for ( QListViewItem * it = (item->parent())?item->nextSibling():new_item->nextSibling(); it; it = next_sibling ) {
				if ( it->rtti() == INGGRPLISTVIEWITEM_RTTI )
					break;
	
				next_sibling = it->nextSibling(); //get the next sibling of this item before we move it
	
				if ( it->parent() )
					it->parent()->takeItem(it);
				else
					previewIngView->takeItem( it );
	
				new_item->insertItem( it );
	
				if ( last_item )
					it->moveItem( last_item );
				last_item = it;
			}
	
			new_item->setOpen(true);
	
			delete item;
		}
	}
}

void IngredientParserDialog::convertToHeader( const QPtrList<QListViewItem> &items )
{
	if ( items.count() > 0 ) {
		QPtrListIterator<QListViewItem> it(items);
		QListViewItem *item = it.current();

		if ( item->rtti() != INGLISTVIEWITEM_RTTI )
			return;

		QString group = ((IngListViewItem*)item)->ingredient().name;
		QListViewItem *ingGroupItem = new IngGrpListViewItem(previewIngView,
		   (item->parent())?item->parent():item, group, -1);
		delete item; //delete the ingredient header which was detected as an ingredient
		++it;

		QListViewItem *last_item = 0;
		while ( (item = it.current()) != 0 ) {
			//ignore anything that isn't an ingredient (e.g. headers)
			if ( item->rtti() == INGLISTVIEWITEM_RTTI ) { 
				if ( item->parent() )
					item->parent()->takeItem(item);
				else
					previewIngView->takeItem( item );
	
				ingGroupItem->insertItem( item );
	
				if ( last_item )
					item->moveItem( last_item );
				last_item = item;
			}

			++it;
		}

		ingGroupItem->setOpen(true);
		previewIngView->clearSelection();
	}
}

void IngredientParserDialog::parseText()
{
	previewIngView->clear();

	QListViewItem *last_item = 0;

	int line_num = 0;
	QStringList ingredients = QStringList::split("\n",ingredientTextEdit->text());
	for ( QStringList::const_iterator it = ingredients.begin(); it != ingredients.end(); ++it ) {
		QString line = (*it).simplifyWhiteSpace();

		++line_num;
		int format_at = 0;
		Ingredient ing;


		//======amount======//
		int first_space = line.find( " ", format_at+1 );
		if ( first_space == -1 )
			first_space = line.length();

		int second_space = line.find( " ", first_space+1 );
		if ( second_space == -1 )
			second_space = line.length();

		Ingredient i;
		bool ok;
		i.setAmount(line.mid(format_at,second_space-format_at),&ok);
		if ( !ok ) {
			i.setAmount(line.mid(format_at,first_space-format_at),&ok);
			if ( ok ) format_at = first_space+1;
		}
		else
			format_at = second_space+1;

		if ( ok ) {
			ing.amount = i.amount;
			ing.amount_offset = i.amount_offset;
		}
		else
			kdDebug()<<"no amount on line "<<line_num<<endl;


		//======unit======//
		first_space = line.find( " ", format_at+1 );
		if ( first_space == -1 )
			first_space = line.length();

		bool isUnit = false;
		QString unitCheck = line.mid(format_at,first_space-format_at);

		for ( UnitList::const_iterator unit_it = m_unitList.begin(); unit_it != m_unitList.end(); ++unit_it ) {
			if ( (*unit_it).name == unitCheck || (*unit_it).plural == unitCheck ) {
				isUnit = true;
				format_at = first_space+1;
				break;
			}
		}

		if ( isUnit ) {
			ing.units.name = unitCheck;
			ing.units.plural = unitCheck;
		}
		else
			kdDebug()<<"no unit on line "<<line_num<<endl;


		//======ingredient======//
		int ing_end = line.find( QRegExp("(,|;)"), format_at+1 );
		if ( ing_end == -1 )
			ing_end = line.length();

		ing.name = line.mid(format_at,ing_end-format_at);
		format_at = ing_end+2;


		//======prep method======//
		int end = line.length();
		ing.prepMethodList = ElementList::split(",",line.mid(format_at,end-format_at));

		last_item = new IngListViewItem(previewIngView,last_item,ing);
		buttonOk->setEnabled( true );
	}
}

#include "ingredientparserdialog.moc"

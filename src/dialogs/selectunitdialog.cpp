/***************************************************************************
 *   Copyright (C) 2003 by                                                 *
 *   Unai Garro (ugarro@users.sourceforge.net)                             *
 *   Cyril Bosselut (bosselut@b1project.com)                               *
 *   Jason Kivlighn (mizunoami44@users.sourceforge.net)                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/

#include "selectunitdialog.h"

#include <kconfig.h>
#include <kglobal.h>
#include <klocale.h>

SelectUnitDialog::SelectUnitDialog(QWidget* parent,const UnitList &unitList)
: QDialog(parent)
{
container=new QVBoxLayout(this,5,5);

box=new QGroupBox(this);
box->setTitle(i18n("Choose Unit"));
box->setColumnLayout(0, Qt::Vertical );
QVBoxLayout *boxLayout = new QVBoxLayout( box->layout() );

unitChooseView=new KListView(box);

KConfig *config = KGlobal::config();
config->setGroup( "Advanced" );
bool show_id = config->readBoolEntry("ShowID",false);
unitChooseView->addColumn( i18n("Id"), show_id ? -1 : 0 );

unitChooseView->addColumn(i18n("Unit"));
unitChooseView->setGeometry( QRect( 5, 30, 180, 250 ) );
unitChooseView->setAllColumnsShowFocus(true);
boxLayout->addWidget( unitChooseView );

QHBoxLayout *buttonsLayout = new QHBoxLayout( this, 0, 6 );
okButton=new QPushButton(box);
okButton->setGeometry( QRect( 5, 300, 100, 20 ) );
okButton->setText(i18n("&OK"));
okButton->setFlat(true);
buttonsLayout->addWidget( okButton );

cancelButton=new QPushButton(box);
cancelButton->setGeometry( QRect( 110, 300, 60, 20 ) );
cancelButton->setText(i18n("&Cancel"));
cancelButton->setFlat(true);
buttonsLayout->addWidget( cancelButton );

boxLayout->addLayout( buttonsLayout );
container->addWidget(box);
resize(QSize(200,350));

loadUnits(unitList);
connect (okButton,SIGNAL(clicked()),this,SLOT(accept()));
connect (cancelButton,SIGNAL(clicked()),this,SLOT(reject()));
}


SelectUnitDialog::~SelectUnitDialog()
{
}

int SelectUnitDialog::unitID(void)
{

QListViewItem *it;
if ( (it=unitChooseView->selectedItem()) )
{
return(it->text(0).toInt());
}
else return(-1);
}

void SelectUnitDialog::loadUnits(const UnitList &unitList)
{
for ( UnitList::const_iterator unit_it = unitList.begin(); unit_it != unitList.end(); ++unit_it )
{
(void)new QListViewItem(unitChooseView,QString::number((*unit_it).id),(*unit_it).name);
}
}


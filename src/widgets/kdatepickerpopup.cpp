/*
  This file is part of libkdepim.

  Copyright (c) 2004 Bram Schoenmakers <bramschoenmakers@kde.nl>

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Library General Public
  License as published by the Free Software Foundation; either
  version 2 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Library General Public License for more details.

  You should have received a copy of the GNU Library General Public License
  along with this library; see the file COPYING.LIB.  If not, write to
  the Free Software Foundation, Inc., 51 Franklin Steet, Fifth Floor,
  Boston, MA  02110-1301  USA
*/

#include <qdatetime.h>
#include <q3popupmenu.h>

#include <klocale.h>

#include "kdatepickerpopup.h"

KDatePickerPopup::KDatePickerPopup( int items, const QDate &date, QWidget *parent )
  : QMenu( parent )
{
  mItems = items;

  mDatePicker = new KDatePicker( this );
  mDatePicker->setCloseButton( false );

  connect( mDatePicker, SIGNAL( dateEntered( QDate ) ),
           SLOT( slotDateChanged( QDate ) ) );
  connect( mDatePicker, SIGNAL( dateSelected( QDate ) ),
           SLOT( slotDateChanged( QDate ) ) );

  mDatePicker->setDate( date );

  buildMenu();
}

void KDatePickerPopup::buildMenu()
{
  if ( isVisible() ) return;
  clear();

  if ( mItems & DatePicker ) {

    // KDE4 need to be done
    //insertItem( mDatePicker );

    if ( ( mItems & NoDate ) || ( mItems & Words ) )
      addSeparator();
  }

  if ( mItems & Words ) {
    insertItem( i18n("&Today"), this, SLOT( slotToday() ) );
    insertItem( i18n("&Yesterday"), this, SLOT( slotYesterday() ) );
    insertItem( i18n("Last &Week"), this, SLOT( slotLastWeek() ) );
    insertItem( i18n("Last M&onth"), this, SLOT( slotLastMonth() ) );

    if ( mItems & NoDate )
      addSeparator();
  }

  if ( mItems & NoDate )
    insertItem( i18n("No Date"), this, SLOT( slotNoDate() ) );
}

KDatePicker *KDatePickerPopup::datePicker() const
{
  return mDatePicker;
}

void KDatePickerPopup::setDate( const QDate &date )
{
  mDatePicker->setDate( date );
}

#if 0
void KDatePickerPopup::setItems( int items )
{
  mItems = items;
  buildMenu();
}
#endif

void KDatePickerPopup::slotDateChanged( QDate date )
{
  emit dateChanged( date );
  hide();
}

void KDatePickerPopup::slotToday()
{
  emit dateChanged( QDate::currentDate() );
}

void KDatePickerPopup::slotYesterday()
{
  emit dateChanged( QDate::currentDate().addDays( -1 ) );
}

void KDatePickerPopup::slotNoDate()
{
  emit dateChanged( QDate() );
}

void KDatePickerPopup::slotLastWeek()
{
  emit dateChanged( QDate::currentDate().addDays( -7 ) );
}

void KDatePickerPopup::slotLastMonth()
{
  emit dateChanged( QDate::currentDate().addMonths( -1 ) );
}

#include "kdatepickerpopup.moc"

/***************************************************************************
 *   Copyright (C) 2003 by Unai Garro                                      *
 *   ugarro@users.sourceforge.net                                                       *
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
#include <qgroupbox.h>
#include <qvbox.h>
#include <qdialog.h>
#include <qlabel.h>
#include <klineedit.h>
#include <elementlist.h>


class CreatePropertyDialog:public QDialog{
public:
    CreatePropertyDialog(ElementList *list);
    ~CreatePropertyDialog();
    QString newPropertyName(void);
    QString newUnitsName(void);


private:
  //Widgets
  QVBoxLayout *container;
  QGroupBox *box;
  QVBox *vbox;
  KLineEdit *propertyNameEdit;
  KLineEdit *propertyUnits;
  QPushButton* okButton;
  QPushButton* cancelButton;
  QLabel *nameEditText;
  QLabel *unitsText;

  //Internal variables
  ElementList *unitList;

  //Methods
  void loadUnits();
};

#endif

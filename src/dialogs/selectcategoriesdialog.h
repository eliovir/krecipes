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

#ifndef SELECTCATEGORIESDIALOG_H
#define SELECTCATEGORIESDIALOG_H

#include <qdialog.h>
#include <qlayout.h>
#include <qpushbutton.h>
#include <klistview.h>

#include "elementlist.h"

class CategoryTree;
class CategoryCheckListItem;
class RecipeDB;

/**
@author Unai Garro
*/
class SelectCategoriesDialog:public QDialog{

Q_OBJECT

public:

    SelectCategoriesDialog(QWidget *parent, const CategoryTree *categoryTree,const QMap<Element,bool> &selected, RecipeDB* db);
    ~SelectCategoriesDialog();
    void getSelectedCategories(ElementList *selected,CategoryCheckListItem *parent=0);
private:

	//Widgets
	KListView *categoryListView;
	QGridLayout *layout;
	QPushButton *okButton;
	QPushButton *cancelButton;

	//Variables
	RecipeDB *database;
	
	//Private methods
	void loadCategories(const CategoryTree *categoryTree, const QMap<Element,bool> &selected, CategoryCheckListItem *parent=0 );
	
private slots:
	void createNewCategory(void);
};

#endif

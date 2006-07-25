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

#ifndef SELECTAUTHORSDIALOG_H
#define SELECTAUTHORSDIALOG_H

#include <qlayout.h>
#include <qpushbutton.h>

#include <kdialogbase.h>
#include <kiconloader.h>
#include <klistview.h>
#include <kcombobox.h>

#include "datablocks/elementlist.h"

class RecipeDB;

/**
@author Unai Garro
*/
class SelectAuthorsDialog: public KDialogBase
{

	Q_OBJECT

public:

	SelectAuthorsDialog( QWidget *parent, const ElementList &currentAuthors, RecipeDB *db );
	~SelectAuthorsDialog();
	void getSelectedAuthors( ElementList *newAuthors );

private:

	//Widgets
	KComboBox *authorsCombo;
	KListView *authorListView;
	QPushButton *okButton;
	QPushButton *cancelButton;
	QPushButton *addAuthorButton;
	QPushButton *removeAuthorButton;
	KIconLoader *il;

	//Variables
	ElementList authorList;
	RecipeDB *database;

	//Private methods
	void loadAuthors( const ElementList &authorList );
	void createNewAuthorIfNecessary( void );
	void reloadAuthorsCombo( void );
private slots:
	void addAuthor( void );
	void removeAuthor( void );
};

#endif

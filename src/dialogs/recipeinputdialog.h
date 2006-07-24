/***************************************************************************
*   Copyright (C) 2003-2005 by                                            *
*   Unai Garro (ugarro@users.sourceforge.net)                             *
*   Cyril Bosselut (bosselut@b1project.com)                               *
*   Jason Kivlighn (jkivlighn@gmail.com)                                  *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
***************************************************************************/

#ifndef RECIPEINPUTDIALOG_H
#define RECIPEINPUTDIALOG_H

#include <kdialog.h>
#include <ktextedit.h>
#include <klineedit.h>
#include <kcombobox.h>
#include <klistview.h>
#include <knuminput.h>
#include <kpushbutton.h>
#include <kiconloader.h>
#include <kled.h>

#include <qlabel.h>
#include <qgroupbox.h>
#include <qmap.h>
#include <qobject.h>
#include <qtabwidget.h>
#include <qtoolbutton.h>
#include <qvbox.h>

#include "datablocks/elementlist.h"

class QTabWidget;
class QTimeEdit;
class QDragEvent;
class QButtonGroup;
class QWidgetStack;
class QTextEdit;

class KreTextEdit;
class KWidgetListbox;
class KDialogBase;

class ImageDropLabel;
class Recipe;
class ElementList;
class RecipeDB;
class FractionInput;
class Ingredient;
class Rating;
class RatingDisplayWidget;
class IngredientInputWidget;
class ClickableLed;

/**
@author Unai Garro
*/
class RecipeInputDialog: public QVBox
{
	Q_OBJECT

public:

	RecipeInputDialog( QWidget* parent, RecipeDB* db );
	void loadRecipe( int recipeID );
	~RecipeInputDialog();
	void newRecipe( void );
	bool everythingSaved();
	void reload( void );
	int loadedRecipeID() const;

private:

	// Internal Data
	Recipe *loadedRecipe; //Loaded Recipe
	RecipeDB *database;
	bool changedSignalEnabled;
	bool unsavedChanges;

	// Widgets
	QTabWidget* tabWidget;
	QGroupBox* recipeTab;
	QGroupBox* instructionsTab;

	//Recipe Photo
	ImageDropLabel *photoLabel;
	QPixmap sourcePhoto;
	QPushButton *changePhotoButton;

	//Recipe Body
	KreTextEdit* instructionsEdit;
	QLabel* titleLabel;
	KLineEdit* titleEdit;

	//Additional recipe data
	QLabel* yieldLabel;
	FractionInput* yieldNumInput;
	KLineEdit* yieldTypeEdit;
	QTimeEdit *prepTimeEdit;
	QLabel* authorLabel;
	KLineEdit* authorShow;
	QPushButton* addAuthorButton;
	QLabel* categoryLabel;
	KLineEdit* categoryShow;
	QPushButton* addCategoryButton;

	//Ingredient inputs
	KListView* ingredientList;
	QGroupBox *ingredientGBox;
	IngredientInputWidget *ingInput;
	ClickableLed *propertyStatusLed;
	QLabel *propertyStatusLabel;
	QPushButton *propertyStatusButton;
	QTextEdit *statusTextView;
	KDialogBase *propertyStatusDialog;

	// Buttons to move ingredients up & down...
	KPushButton* upButton;
	KPushButton* downButton;
	KPushButton* removeButton;
	KPushButton* addButton;
	KPushButton* ingParserButton;

	//Function buttons
	QGroupBox* functionsBox;
	QToolButton* saveButton;
	QToolButton* closeButton;
	QToolButton* showButton;
	QToolButton* resizeButton;

	QToolButton* spellCheckButton;

	KWidgetListbox *ratingListDisplayWidget;

	QMap<int,QString> propertyStatusMap;

	// Internal functions
	int createNewYieldIfNecessary( const QString &yield );
	void saveRecipe( void );
	void showCategories( void );
	void showAuthors( void );
	int ingItemIndex( QListView *listview, const QListViewItem *item ) const;
	void addRating( const Rating &rating, RatingDisplayWidget *item );
	QString statusMessage() const;

	// Signals & Slots

private slots:
	void changePhoto( void );
	void clearPhoto( void );
	void moveIngredientUp( void );
	void moveIngredientDown( void );
	void removeIngredient( void );
	void syncListView( QListViewItem* it, const QString &new_text, int col );
	void recipeChanged( void );
	void recipeChanged( const QString &t );
	void enableChangedSignal( bool en = true );
	void addCategory( void );
	void addAuthor( void );
	void enableSaveButton( bool enabled );
	void closeOptions( void );
	void showRecipe( void );
	void prepTitleChanged( const QString &title );
	void recipeRemoved( int id );
	void slotIngredientParser();
	void slotAddRating();
	void slotEditRating();
	void slotRemoveRating();
	void addIngredient( const Ingredient &ing );
	void addIngredientHeader( const Element &header );
	void updatePropertyStatus();
	void updatePropertyStatus( const Ingredient &ing, bool updateIndicator );
	void showStatusIndicator();

public slots:
	bool save ( void ); // Activated when krecipes.cpp sends signal save()
	void spellCheck( void );
	void resizeRecipe( void );

signals:
	void changed( void );
	void closeRecipe( void );
	void createButton( QWidget* w, const QString &title );
	void enableSaveOption( bool en = true );
	void showRecipe( int recipeID ); //Indicates krecipesview to show it
	void titleChanged( const QString &title );


};

class ClickableLed : public KLed
{
Q_OBJECT

public:
	ClickableLed( QWidget *parent );

protected:
	virtual void mouseReleaseEvent( QMouseEvent* );

signals:
	void clicked();
};

class ImageDropLabel : public QLabel
{
	Q_OBJECT

public:
	ImageDropLabel( QWidget *parent, QPixmap &_sourcePhoto );

signals:
	void changed();

protected:
	void dragEnterEvent( QDragEnterEvent* event );
	void dropEvent( QDropEvent* event );

private:
	QPixmap &sourcePhoto;
};

#endif

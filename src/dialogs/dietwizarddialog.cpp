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

#include "dietwizarddialog.h"
#include "DBBackend/recipedb.h"
#include "editbox.h"
#include <qheader.h>
#include <qlayout.h>
#include <qpainter.h>
#include <kapp.h>
#include <kglobalsettings.h>
#include <kiconloader.h>
#include <klocale.h>


DietWizardDialog::DietWizardDialog(QWidget *parent,RecipeDB *db):QVBox(parent)
{
// Initialize internal variables
database=db;
mealNumber=1;

//Design the dialog
setSpacing(10);
// Options Box
optionsBox=new QHBox(this);

	sliderBox=new QVGroupBox(i18n("Meals per day"),optionsBox);
	mealNumberLabel=new QLabel(sliderBox);
	mealNumberLabel->setText("- 1 -");
	mealNumberLabel->setAlignment(Qt::AlignHCenter);
	mealNumberSelector=new QSlider(sliderBox);

	mealNumberSelector->setOrientation(Qt::Horizontal);
	mealNumberSelector->setRange(1,10);
	mealNumberSelector->setSteps(1,1);
	mealNumberSelector->setTickmarks(QSlider::Below);
	mealNumberSelector->setFixedWidth(100);

// Tabs
mealTabs=new QTabWidget(this);
mealTabs->setMargin(20);

// Button bar
KIconLoader il;

buttonBox=new QHBox(this);

okButton=new QPushButton(buttonBox);
okButton->setIconSet(il.loadIconSet("button_ok", KIcon::Small));
okButton->setText(i18n("Create the diet"));

// Create Tabs
newTab(i18n("Meal 1"));

// Initialize data
reload();

// Signals & Slots
connect(mealNumberSelector,SIGNAL(valueChanged(int)),this,SLOT(changeMealNumber(int)));

}


DietWizardDialog::~DietWizardDialog()
{
}

void DietWizardDialog::reload(void)
{
database->loadCategories(&categoriesList);
database->loadProperties(&constraintList);
int pgcount=0;
for (MealInput *tab=(MealInput *) (mealTabs->page(pgcount));pgcount<mealTabs->count(); pgcount++)
	tab->reload(categoriesList,constraintList);
}

void DietWizardDialog::newTab(const QString &name)
{
mealTab=new MealInput(mealTabs);
mealTab->reload(categoriesList,constraintList);
mealTabs->addTab(mealTab,name);
mealTabs->setCurrentPage(mealTabs->indexOf(mealTab));
}

void DietWizardDialog::changeMealNumber(int mn)
{
if (mn>mealNumber)
	{
	 newTab(i18n("Meal %1").arg(mn));
	 mealNumber++;
	 mealNumberLabel->setText(QString("- %1 -").arg(mn));
	 }
}

class ConstraintsListItem:public QCheckListItem{
public:
	ConstraintsListItem(QListView* klv, IngredientProperty *ip ):QCheckListItem(klv,QString::null,QCheckListItem::CheckBox){ ipStored=new IngredientProperty(ip);}
private:
	IngredientProperty *ipStored;

public:
	virtual QString text(int column) const
		{
		if (column==1) return(QString("0.0"));
		else if (column==2) return(ipStored->name);
		else if (column==3) return(QString("0.0"));
		else return(QString::null);
		}
	~ConstraintsListItem(void)
	{
	delete ipStored;
	}
};

class CategoriesListItem:public QCheckListItem{
public:
	CategoriesListItem(QListView* klv, QString name ):QCheckListItem(klv,QString::null,QCheckListItem::CheckBox){nameStored=name;}
	~CategoriesListItem(void){}
	virtual QString text(int column) const
		{
		if (column==1) return(nameStored);
		else return(QString::null);
		}
private: QString nameStored;

};


MealInput::MealInput(QWidget *parent):QWidget(parent)
{

QVBoxLayout *layout=new QVBoxLayout(this);
layout->setSpacing(20);

	// Options box

mealOptions=new QHBox(this);
mealOptions->setSpacing(10);
layout->addWidget(mealOptions);

	// Number of dishes input
dishNumberBox=new QHBox(mealOptions); dishNumberBox->setSpacing(10);
dishNumberLabel=new QLabel(i18n("No. of dishes: "),dishNumberBox);
dishNumberInput=new QSpinBox(dishNumberBox); dishNumberInput->setMinValue(1); dishNumberInput->setMaxValue(10);
dishNumberBox->setSizePolicy(QSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum));

	// Toolbar

toolBar=new QHGroupBox(mealOptions);
toolBar->setFrameStyle (QFrame::Panel | QFrame::Sunken);
toolBar->setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::Minimum));

	// Next dish/ Previous dish buttons

buttonPrev=new QToolButton(toolBar); buttonPrev->setText(i18n("Prev"));
buttonNext=new QToolButton(toolBar); buttonNext->setText(i18n("Next"));

	//Horizontal Box to hold the KListView's
listBox=new QHGroupBox(i18n("Dish Characteristics"),this);
layout->addWidget(listBox);

	// Dish id
dishTitle=new DishTitle(listBox);

	//Categories list
categoriesView=new KListView(listBox);
categoriesView->addColumn("*");
categoriesView->addColumn(i18n("Category"));

	//Constraints list
constraintsView=new KListView(listBox);
constraintsView->addColumn(i18n("Enabled"));
constraintsView->addColumn(i18n("Min. Value"));
constraintsView->addColumn(i18n("Property"));
constraintsView->addColumn(i18n("Max. Value"));

	// KDoubleInput based edit boxes
constraintsEditBox1=new EditBox(this);
constraintsEditBox1->hide();
constraintsEditBox2=new EditBox(this);
constraintsEditBox2->hide();


// Connect Signals & Slots
connect(constraintsView,SIGNAL(executed(QListViewItem*)),this,SLOT(insertConstraintsEditBoxes(QListViewItem*)));

}

MealInput::~MealInput()
{
}

void MealInput::reload(ElementList &categoriesList,IngredientPropertyList &constraintList)
{
categoriesView->clear();
constraintsView->clear();

	//Load the possible constraints (properties) list
for (IngredientProperty *ip=constraintList.getFirst(); ip; ip=constraintList.getNext())
{
ConstraintsListItem *it=new ConstraintsListItem(constraintsView,ip);
constraintsView->insertItem(it);
}

	//Load the categories list
for (Element *el=categoriesList.getFirst(); el; el=categoriesList.getNext())
{
CategoriesListItem *it=new CategoriesListItem(categoriesView,el->name);
categoriesView->insertItem(it);
}

}


void MealInput::insertConstraintsEditBoxes(QListViewItem* it)
{

QRect r;

// Constraints Box1
r=constraintsView->header()->sectionRect(1); //Set column's size() and position with at adistance equal to the distance between the qlistview and the header;
r.moveBy(this->pos().x()+constraintsView->pos().x(),this->pos().y()+constraintsView->pos().y()); // Move to the position of qlistview header in that column
r.moveBy(0,r.height()+constraintsView->itemRect(it).y()); //Move down to the item, note that its height is same as header's right now.

r.setHeight(it->height()); // Set the item's height
constraintsEditBox1->setGeometry(r);


//Constraints Box2
r=constraintsView->header()->sectionRect(3); //Set column's size() and position with at adistance equal to the distance between the qlistview and the header;

r.moveBy(this->pos().x()+constraintsView->pos().x(),this->pos().y()+constraintsView->pos().y()); // Move to the position of qlistview
r.moveBy(0,r.height()+constraintsView->itemRect(it).y()); //Move down to the item, note that its height is same as header's right now.

r.setHeight(it->height()); // Set the item's height
constraintsEditBox2->setGeometry(r);

// Show Boxes
constraintsEditBox1->show();
constraintsEditBox2->show();
}

DishTitle::DishTitle(QWidget *parent):QWidget(parent)
{
}

DishTitle::~DishTitle()
{
}

void DishTitle::paintEvent(QPaintEvent *p )
{

    QPainter painter(this);

    // First draw the decoration
    painter.setPen(KGlobalSettings::activeTitleColor());
    painter.setBrush(QBrush(KGlobalSettings::activeTitleColor()));
    painter.drawRoundRect(0,20,30,height()-40,50,50.0/(height()-40)*35.0);

    // Now draw the text
    QFont titleFont=KGlobalSettings::windowTitleFont ();
    titleFont.setPointSize(15);
    painter.setFont(titleFont);
    painter.rotate(-90);
    painter.setPen(QColor(0x00,0x00,0x00));
    painter.drawText(0,0,-height(),30,AlignCenter,"Dish 1");
    painter.setPen(QColor(0xFF,0xFF,0xFF));
    painter.drawText(-1,-1,-height()-1,29,AlignCenter,"Dish 1");

    painter.end();
}
QSize DishTitle::sizeHint () const
{
return(QSize(40,200));
}

QSize DishTitle::minimumSizeHint() const
{
return(QSize(40,200));
}
/***************************************************************************
*   Copyright © 2012 José Manuel Santamaría Lema <panfaust@gmail.com>     *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
***************************************************************************/

#ifndef KREGENERICLISTWIDGET_H
#define KREGENERICLISTWIDGET_H

#include <QWidget>

//FIXME: Remove this header and uncomment ReloadFlags below
// whenever we can get rid of the obsolete DBListviewBase.
#include "dblistviewbase.h"

class QStandardItemModel;
class QSortFilterProxyModel;
class QString;
class QModelIndex;
class QPoint;

class Element;
class RecipeDB;

namespace Ui {
	class KreGenericListWidget;
}

/* enum ReloadFlags {
	Load,			// Only performs the reload if the list hasn't already been loaded.
	ReloadIfPopulated,	// Only performs the reload if the list has been loaded.
	ForceReload		// Load/reload the list regardless of whether or not it's been loaded.
};*/

class KreGenericListWidget : public QWidget
{
	Q_OBJECT

public:
	KreGenericListWidget( QWidget *parent, RecipeDB *db );
	virtual ~KreGenericListWidget();

	void setSearchAsYouType( bool value );
	void setCurrentLimit( int value );
	void reload( ReloadFlags flags );
	int currentRow();
	void edit( int row );
	int selectedRowId();

signals:
	void contextMenuRequested( const QModelIndex & index, const QPoint & point);
	void itemsChanged( const QModelIndex & topLeft, const QModelIndex & bottomRight);

protected slots:
	void setFilter();
	void setFilter( const QString & filter );
	void activatePreviousPage();
	void activateNextPage();
	void contextMenuSlot( const QPoint & point );

protected:
	virtual void load(int limit, int offset) = 0;

	bool m_searchAsYouType;

	int m_currentLimit;
	int m_currentOffset;

	Ui::KreGenericListWidget *ui;
	QStandardItemModel *m_sourceModel;
	QSortFilterProxyModel *m_proxyModel;
	RecipeDB *m_database;
};


#endif //KREGENERICLISTWIDGET_H
/***************************************************************************
*   Copyright (C) 2003 by Jason Kivlighn                                  *
*   mizunoami44@users.sourceforge.net                                     *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
***************************************************************************/

#ifndef SETUPDISPLAY_H
#define SETUPDISPLAY_H

#include <khtml_part.h>
#include <dom/html_element.h>
#include <dom/css_stylesheet.h>

#include <qdom.h>
#include <qwidget.h>
#include <qmap.h>

#include "datablocks/recipe.h"
#include "widgets/dragarea.h"
#include "datablocks/kreborder.h"
#include "klomanager.h"

#include <math.h>

class KPopupMenu;

class QAction;
class QLabel;
class QWidget;

class StyleSheet;

class KreDisplayItem
{
public:
	KreDisplayItem( const QString &id, const QString &name );

	void clear();

	QString nodeId;
	QString name;
	KreBorder border;
	int alignment;
	int columns;
	bool show;
	QColor backgroundColor;
	QColor textColor;
	QFont font;
};

typedef QMap< KreDisplayItem*, unsigned int > PropertiesMap;

/** @brief A KHTMLPart for editing specific CSS properties
  *
  * Set up the items of a recipe for display.
  *
  * @author Jason Kivlighn
  */
class SetupDisplay : public KHTMLPart, protected KLOManager
{
	Q_OBJECT

public:
	SetupDisplay( const Recipe &, QWidget *parent );
	~SetupDisplay();

	enum Properties { None = 0, BackgroundColor = 1, TextColor = 2, Font = 4, Visibility = 8, Alignment = 32, Columns = 64, Border = 128 };

	void saveLayout( const QString & );
	void loadLayout( const QString & );
	void loadTemplate( const QString &filename );

	bool hasChanges() const
	{
		return has_changes;
	}

	void setItemShown( KreDisplayItem *item, bool visible );

	const PropertiesMap properties() const
	{
		return * box_properties;
	}

signals:
	void itemVisibilityChanged( KreDisplayItem *, bool );

protected:
	virtual void begin (const KURL &url=KURL(), int xOffset=0, int yOffset=0);

	virtual void loadBackgroundColor( const QString &obj, const QColor& );
	virtual void loadFont( const QString &obj, const QFont& );
	virtual void loadTextColor( const QString &obj, const QColor& );
	virtual void loadVisibility( const QString &obj, bool );
	virtual void loadAlignment( const QString &obj, int );
	virtual void loadBorder( const QString &obj, const KreBorder& );
	virtual void loadColumns( const QString &obj, int );

	virtual void beginObject( const QString &obj );
	virtual void endObject();

protected slots:
	void nodeClicked(const QString &url,const QPoint &point);
	void changeMade();

	//slots to set properties of item boxes
	void setBackgroundColor();
	void setBorder();
	void setColumns();
	void setTextColor();
	void setFont();
	void setShown( int id );
	void setAlignment( QAction * );

private:
	PropertiesMap *box_properties;
	QMap<QString, KreDisplayItem*> *node_item_map;

	bool has_changes;

	// Methods
	void applyStylesheet();
	void loadPageLayout( const QDomElement &tag );
	void loadHTMLView( const QString &templateFile = QString::null, const QString &styleFile = QString::null );

	void createItem( const QString &id, const QString &name, unsigned int properties );

	//the name of the element under the mouse on a right-click
	QString m_currNodeId;

	//the item corresponding to the current point of processing the KLO
	KreDisplayItem *m_currentItem;

	KPopupMenu *popup;
	DOM::CSSStyleSheet m_styleSheet;
	Recipe m_sample;

	QString m_activeTemplate;
};

#endif //SETUPDISPLAY_H


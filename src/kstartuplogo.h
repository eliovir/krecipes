/***************************************************************************
*   Copyright (C) 2003                                                    *
*                                                                         *
*   Unai Garro (ugarro@users.sourceforge.net)                             *
*   Cyril Bosselut (bosselut@b1project.com)                               *
*   Jason Kivlighn (mizunoami44@users.sourceforge.net)                    *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
*   Based on kstartuplogo from Umbrello http://uml.sourceforge.net        *
***************************************************************************/


#ifndef KSTARTUPLOGO_H
#define KSTARTUPLOGO_H

#include <qwidget.h>

#include <kapplication.h>
#include <kstandarddirs.h>

/**
 * Displays a startup splash screen
 */
class KStartupLogo : public QWidget
{
	Q_OBJECT
public:
	KStartupLogo( QWidget *parent = 0, const char *name = 0 );
	~KStartupLogo();
	void setHideEnabled( bool bEnabled )
	{
		m_bReadyToHide = bEnabled;
	};
protected:
	virtual void mousePressEvent( QMouseEvent* );
	QRect splashScreenDesktopGeometry() const;
	bool m_bReadyToHide;
};

#endif





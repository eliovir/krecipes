
/***************************************************************************
*   Copyright (C) 2003-2005 by                                            *
*   Unai Garro (ugarro@users.sourceforge.net)                             *
*   Jason Kivlighn (jkivlighn@gmail.com)                                  *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
***************************************************************************/

#include "krecipes.h"

#include <iostream>

#include <kuniqueapplication.h>
#include <kaboutdata.h>
#include <kcmdlineargs.h>
#include <klocale.h>

#include "convert_sqlite3.h"

static const char *version = "1.0";

static KCmdLineOptions options;

int main( int argc, char **argv )
{
	options.add("convert-sqlite3", ki18n("Convert the current SQLite 2.x database to SQLite 3 and exit") , 0 );
	options.add( 0, ki18n(""), 0 );    


	KAboutData about( "krecipes", 0, ki18n( "Krecipes" ), version, ki18n( "The KDE Cookbook" ), KAboutData::License_GPL, ki18n( "(C) 2003 Unai Garro\n(C) 2004-2006 Jason Kivlighn\n\n___________\n\n\nThis product is RecipeML compatible.\n You can get more information about this file format in:\n http://www.formatdata.com/recipeml" ), ki18n(""), "http://krecipes.sourceforge.net/", "jkivlighn@gmail.com" );
	about.addAuthor( ki18n("Unai Garro"), ki18n(""), "ugarro@users.sourceforge.net", 0 );
	about.addAuthor( ki18n("Jason Kivlighn"), ki18n(""), "jkivlighn@gmail.com", 0 );
	about.addAuthor( ki18n("Cyril Bosselut"), ki18n(""), "bosselut@b1project.com", "http://b1project.com" );

	about.addCredit( ki18n("Colleen Beamer"), ki18n("Testing, bug reports, suggestions"), "colleen.beamer@gmail.com", 0 );
	about.addCredit( ki18n("Robert Wadley"), ki18n("Icons and artwork"), "rob@robntina.fastmail.us", 0 );
	about.addCredit( ki18n("Daniel Sauvé"), ki18n("Porting to KDE4"), "megametres@gmail.com", "http://metres.homelinux.com" );

	about.setTranslator( ki18n( "INSERT YOUR NAME HERE" ), ki18n( "INSERT YOUR EMAIL ADDRESS" ) );
	KCmdLineArgs::init( argc, argv, &about );
	KCmdLineArgs::addCmdLineOptions( options );
	KUniqueApplication::addCmdLineOptions();

	if ( !KUniqueApplication::start() ) {
		std::cout << "Krecipes is already running!" << std::endl;
		return 0;
	}

	KUniqueApplication app;

	// see if we are starting with session management
	if ( app.isSessionRestored() ) {
		RESTORE( Krecipes );
	}
	else {
		// no session.. just start up normally
		KCmdLineArgs *args = KCmdLineArgs::parsedArgs();

		QApplication::flush();

		if ( args->isSet("convert-sqlite3") ) {
			ConvertSQLite3();
			return 0;
		}

		Krecipes * widget = new Krecipes;
		app.setTopWidget( widget );
		widget->show();

		args->clear();
	}

	return app.exec();
}


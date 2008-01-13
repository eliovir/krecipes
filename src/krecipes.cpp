/***************************************************************************
*   Copyright (C) 2003 by                                                 *
*   Unai Garro (ugarro@users.sourceforge.net)                             *
*                                                                         *
*   Copyright (C) 2003-2005 by                                            *
*   Jason Kivlighn (jkivlighn@gmail.com)                                  *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
***************************************************************************/

#include "pref.h"
#include "krecipes.h"
#include "krecipesview.h"

#include "dialogs/recipeviewdialog.h"
#include "dialogs/recipeinputdialog.h"
#include "dialogs/selectrecipedialog.h"
#include "dialogs/ingredientsdialog.h"
#include "dialogs/propertiesdialog.h"
#include "dialogs/shoppinglistdialog.h"
#include "dialogs/categorieseditordialog.h"
#include "dialogs/authorsdialog.h"
#include "dialogs/unitsdialog.h"
#include "dialogs/ingredientmatcherdialog.h"
#include "dialogs/dbimportdialog.h"
#include "dialogs/pagesetupdialog.h"
#include "dialogs/recipeimportdialog.h"
#include "dialogs/similarcategoriesdialog.h"
#include "dialogs/conversiondialog.h"

#include "importers/kreimporter.h"
#include "importers/mmfimporter.h"
#include "importers/mx2importer.h"
#include "importers/mxpimporter.h"
#include "importers/nycgenericimporter.h"
#include "importers/recipemlimporter.h"
#include "importers/rezkonvimporter.h"
#include "importers/kredbimporter.h"

#include "datablocks/recipe.h"
#include "backends/recipedb.h"
#include "backends/progressinterface.h"

#include <q3dragobject.h>
#include <qpainter.h>
#include <q3paintdevicemetrics.h>
#include <qmessagebox.h>
//Added by qt3to4:
#include <Q3ValueList>
#include <QLabel>
#include <Q3Frame>
#include <QDragEnterEvent>
#include <Q3VBoxLayout>

#include <kprogressdialog.h>
#include <kmessagebox.h>
#include <kglobal.h>
#include <klocale.h>
#include <kiconloader.h>
#include <kmenubar.h>
#include <kstatusbar.h>
#include <kshortcutsdialog.h>
#include <kaction.h>
#include <kfiledialog.h>
#include <kconfig.h>
#include <kcursor.h>

#include <ktoolbar.h>
#include <kedittoolbar.h>
#include <kstdaccel.h>
#include <ktoggleaction.h>
#include <kstandardaction.h> 
#include <kactioncollection.h>
//Settings headers
#include <kdeversion.h>
#include <KShortcutsDialog>


Krecipes::Krecipes()
		: KMainWindow( 0 ),
		m_view( new KrecipesView( this ) )
{
	this->setObjectName( "Krecipes" );
	// accept dnd
	setAcceptDrops( true );

	// tell the KMainWindow that this is indeed the main widget
	setCentralWidget( m_view );

	// then, setup our actions
	setupActions();

	// and a status bar
	statusBar() ->show();

	// apply the saved mainwindow settings, if any, and ask the mainwindow
	// to automatically save settings if changed: window size, toolbar
	// position, icon size, etc.
	setAutoSaveSettings();


	// allow the view to change the statusbar and caption
	connect( m_view, SIGNAL( signalChangeStatusbar( const QString& ) ),
	         this, SLOT( changeStatusbar( const QString& ) ) );
	connect( m_view, SIGNAL( signalChangeCaption( const QString& ) ),
	         this, SLOT( changeCaption( const QString& ) ) );

	connect( m_view, SIGNAL( panelShown( KrePanel, bool ) ), SLOT( updateActions( KrePanel, bool ) ) );
	connect( m_view, SIGNAL( panelShown( KrePanel, bool ) ), SLOT( updateActions( KrePanel, bool ) ) );

	connect( m_view, SIGNAL( recipeSelected(bool) ), SLOT( recipeSelected(bool) ) );


	// Enable/Disable the Save Button (Initialize disabled, and connect signal)

	connect( m_view, SIGNAL( enableSaveOption( bool ) ), this, SLOT( enableSaveOption( bool ) ) );

	enableSaveOption( false ); // Disables saving initially
	recipeSelected( false ); //nothing is selected initially

	parsing_file_dlg = new KDialog( this );
	parsing_file_dlg->setObjectName( "parsing_file_dlg" );
	parsing_file_dlg->setModal( true );
	parsing_file_dlg->setWindowFlags ( Qt::WX11BypassWM );
	QLabel *parsing_file_dlg_label = new QLabel( i18n( "Gathering recipe data from file.\nPlease wait..." ), parsing_file_dlg );
	parsing_file_dlg_label->setFrameStyle( Q3Frame::Box | Q3Frame::Raised );
	( new Q3VBoxLayout( parsing_file_dlg ) ) ->addWidget( parsing_file_dlg_label );
	parsing_file_dlg->adjustSize();
	//parsing_file_dlg->setFixedSize(parsing_file_dlg->size());

	convertDialog = new ConversionDialog(this,m_view->database);
}

Krecipes::~Krecipes()
{}

void Krecipes::updateActions( KrePanel panel, bool show )
{
	switch ( panel ) {
	case RecipeView: {
			exportAction->setEnabled( show );
			printAction->setEnabled( show );
			reloadAction->setEnabled( show );
			copyToClipboardAction->setEnabled( show );

			//can't edit when there are multiple recipes loaded
			if ( show && m_view->viewPanel->recipesLoaded() == 1 ) {
				editAction->setEnabled( true );
			}
			else
				editAction->setEnabled( false );

			break;
		}
	case SelectP: {
			exportAction->setEnabled( show );
			editAction->setEnabled( show );
			copyToClipboardAction->setEnabled( show );
			break;
		}
	default:
		break;
	}
}

void Krecipes::recipeSelected( bool selected )
{
	copyToClipboardAction->setEnabled( selected );
	editAction->setEnabled( selected );
}

void Krecipes::setupActions()
{
	printAction = KStandardAction::print( this, SLOT( filePrint() ), actionCollection() );
	reloadAction = new KAction( i18n( "Reloa&d" ), "view-refresh", Key_F5, m_view, SLOT( reloadDisplay() ), actionCollection(), "reload_action" );

	editAction = new KAction( i18n( "&Edit Recipe" ), "edit", CTRL + Key_E,
	                          m_view, SLOT( editRecipe() ),
	                          actionCollection(), "edit_action" );

	(void) new KAction( i18n( "&Measurement Converter" ), "", CTRL + Key_M,
	                          this, SLOT( conversionToolSlot() ),
	                          actionCollection(), "converter_action" );

	KConfigGroup *config = KGlobal::config()->setGroup("Advanced");
	if ( config->readBoolEntry("UnhideMergeTools",false) ) {
		( void ) new KAction( i18n( "&Merge Similar Categories..." ), "categories", CTRL + Key_M,
					this, SLOT( mergeSimilarCategories() ),
					actionCollection(), "merge_categories_action" );
	
		( void ) new KAction( i18n( "&Merge Similar Ingredients..." ), "ingredients", CTRL + Key_M,
					this, SLOT( mergeSimilarIngredients() ),
					actionCollection(), "merge_ingredients_action" );
	}

	KAction *action = KStandardAction::openNew( this, SLOT( fileNew() ), actionCollection() );
	action->setText( i18n( "&New Recipe" ) );

	saveAction = KStandardAction::save( this, SLOT( fileSave() ), actionCollection() );

	KStandardAction::quit( kapp, SLOT( quit() ), actionCollection() );

	m_toolbarAction = KStandardAction::showToolbar( this, SLOT( optionsShowToolbar() ), actionCollection() );
	m_statusbarAction = KStandardAction::showStatusbar( this, SLOT( optionsShowStatusbar() ), actionCollection() );

	KStandardAction::keyBindings( this, SLOT( optionsConfigureKeys() ), actionCollection() );
	KStandardAction::configureToolbars( this, SLOT( optionsConfigureToolbars() ), actionCollection() );
	KStandardAction::preferences( this, SLOT( optionsPreferences() ), actionCollection() );

	( void ) new KAction( i18n( "Import from File..." ), CTRL + Key_I,
	                      this, SLOT( import() ),
	                      actionCollection(), "import_action" );

	( void ) new KAction( i18n( "Import from Database..." ), 0,
	                      this, SLOT( kreDBImport() ),
	                      actionCollection(), "import_db_action" );

	exportAction = new KAction( i18n( "Export..." ), 0,
	                            this, SLOT( fileExport() ),
	                            actionCollection(), "export_action" );

	copyToClipboardAction = new KAction( i18n( "&Copy to Clipboard" ), "edit-copy",
	                            CTRL + Key_C,
	                            this, SLOT( fileToClipboard() ),
	                            actionCollection(), "copy_to_clipboard_action" );

	( void ) new KAction( i18n( "Page Setup..." ), 0,
	                      this, SLOT( pageSetupSlot() ),
	                      actionCollection(), "page_setup_action" );

	( void ) new KAction( i18n( "Print Setup..." ), 0,
	                      this, SLOT( printSetupSlot() ),
	                      actionCollection(), "print_setup_action" );

	( void ) new KAction( i18n( "Backup..." ), "krecipes_file", 0,
	                      this, SLOT( backupSlot() ),
	                      actionCollection(), "backup_action" );

	( void ) new KAction( i18n( "Restore..." ), 0,
	                      this, SLOT( restoreSlot() ),
	                      actionCollection(), "restore_action" );

	updateActions( SelectP, true );
	updateActions( RecipeView, false );

	createGUI();
}

void Krecipes::saveProperties( KConfigGroup& )
{
	// the 'config' object points to the session managed
	// config file.  anything you write here will be available
	// later when this app is restored

	//if (!m_view->currentURL().isNull())
	//  config->writeEntry("lastURL", m_view->currentURL());
}

void Krecipes::readProperties( KConfigGroup& )
{
	// the 'config' object points to the session managed
	// config file.  this function is automatically called whenever
	// the app is being restored.  read in here whatever you wrote
	// in 'saveProperties'

	//QString url = config->readEntry("lastURL");

	//if (!url.isNull())
	//  m_view->openURL(KUrl(url));
}

void Krecipes::dragEnterEvent( QDragEnterEvent *event )
{
	// accept uri drops only
	event->accept( Q3UriDrag::canDecode( event ) );
}


void Krecipes::fileNew()
{

	// Create a new element (Element depends on active panel. New recipe by default)
	m_view->createNewElement();
}

void Krecipes::fileOpen()
{
	// this slot is called whenever the File->Open menu is selected,
	// the Open shortcut is pressed (usually CTRL+O) or the Open toolbar
	// button is clicked
	/*
	    // this brings up the generic open dialog
	    KUrl url = KUrlRequesterDlg::getURL(QString::null, this, i18n("Open Location") );
	*/ 
	// standard filedialog
	/*KUrl url = KFileDialog::getOpenUrl(QString::null, QString::null, this, i18n("Open Location"));
	if (!url.isEmpty())
	    m_view->openURL(url);*/
}

void Krecipes::fileSave()
{
	// this slot is called whenever the File->Save menu is selected,
	// the Save shortcut is pressed (usually CTRL+S) or the Save toolbar
	// button is clicked
	m_view->save();
}

void Krecipes::fileExport()
{
	// this slot is called whenever the File->Export menu is selected,
	m_view->exportRecipe();
}

void Krecipes::fileToClipboard()
{
	m_view->exportToClipboard();
}

void Krecipes::filePrint()
{
	m_view->print();
}

void Krecipes::import()
{
	KFileDialog file_dialog( QString::null,
	                         "*.kre *.kreml|Krecipes (*.kre, *.kreml)\n"
	                         "*.mx2|MasterCook (*.mx2)\n"
	                         "*.mxp *.txt|MasterCook Export (*.mxp, *.txt)\n"
	                         "*.mmf *.txt|Meal-Master (*.mmf, *.txt)\n"
	                         "*.txt|\"Now You're Cooking\" Generic Export (*.txt)\n"
	                         "*.xml *.recipeml|RecipeML (*.xml, *.recipeml)\n"
	                         "*.rk *.txt|Rezkonv (*.rk, *.txt)",
	                         this,
	                         true
	                       );
	file_dialog.setObjectName( "file_dialog" );
	file_dialog.setMode( KFile::Files );

	if ( file_dialog.exec() == KFileDialog::Accepted ) {
		QStringList warnings_list;

		QString selected_filter = file_dialog.currentFilter();

		BaseImporter *importer;
		if ( selected_filter == "*.mxp *.txt" )
			importer = new MXPImporter();
		else if ( selected_filter == "*.mmf *.txt" )
			importer = new MMFImporter();
		else if ( selected_filter == "*.txt" )
			importer = new NYCGenericImporter();
		else if ( selected_filter == "*.mx2" )
			importer = new MX2Importer();
		else if ( selected_filter == "*.kre *.kreml" )
			importer = new KreImporter();
		else if ( selected_filter == "*.xml *.recipeml" )
			importer = new RecipeMLImporter();
		else if ( selected_filter == "*.rk *.txt" )
			importer = new RezkonvImporter();
		else {
			KMessageBox::sorry( this,
			                    QString( i18n( "Filter \"%1\" not recognized.\n"
			                                   "Please select one of the provided filters." ) ).arg( selected_filter ),
			                    i18n( "Unrecognized Filter" )
			                  );
			import(); //let's try again :)
			return ;
		}

		parsing_file_dlg->show();
		KApplication::setOverrideCursor( Qt::WaitCursor );
		importer->parseFiles( file_dialog.selectedFiles() );
		parsing_file_dlg->hide();
		KApplication::restoreOverrideCursor();

		KConfigGroup * config = KGlobal::config()->setGroup( "Import" );
		bool direct = config->readEntry( "DirectImport", false );
		if ( !direct ) {
			RecipeImportDialog import_dialog( importer->recipeList() );
	
			if ( import_dialog.exec() != QDialog::Accepted ) {
				delete importer;
				return;
			}
			else
				importer->setRecipeList( import_dialog.getSelectedRecipes() );
		}

		importer->import(m_view->database);
		//m_view->database->import( importer ); //TODO TESTS: Do it this way

		if ( !importer->getMessages().isEmpty() ) {
			KTextEdit * warningEdit = new KTextEdit( this );
			warningEdit->setTextFormat( Qt::RichText );
			warningEdit->setText( QString( i18n( "NOTE: We recommend that all recipes generating warnings be checked to ensure that they were properly imported, and no loss of recipe data has occurred.<br><br>" ) ) + importer->getMessages() );
			warningEdit->setReadOnly( true );

			KDialog showWarningsDlg( KDialog::Swallow, i18n( "Import Warnings" ), KDialog::Ok, KDialog::Default, this );
			showWarningsDlg.setMainWidget( warningEdit ); //KDialog will delete warningEdit for us
			showWarningsDlg.resize( QSize( 550, 250 ) );
			showWarningsDlg.exec();
		}

		delete importer;
	}
}

void Krecipes::kreDBImport()
{
	DBImportDialog importOptions;
	if ( importOptions.exec() == QDialog::Accepted ) {
		//Get all params, even if we don't use them
		QString host, user, pass, table;
		int port;
		importOptions.serverParams( host, user, pass, port, table );

		KreDBImporter importer( importOptions.dbType(), host, user, pass, port ); //last 4 params may or may not be even used (depends on dbType)

		parsing_file_dlg->show();
		KApplication::setOverrideCursor( Qt::WaitCursor );
		QStringList tables;
		if ( importOptions.dbType() == "SQLite" )
			tables << importOptions.dbFile();
		else //MySQL or PostgreSQL
			tables << table;
		importer.parseFiles( tables );
		parsing_file_dlg->hide();
		KApplication::restoreOverrideCursor();

		KConfigGroup * config = KGlobal::config()->setGroup( "Import" );
		bool direct = config->readEntry( "DirectImport", false );
		if ( !direct ) {
			RecipeImportDialog import_dialog( importer.recipeList() );
	
			if ( import_dialog.exec() != QDialog::Accepted ) {
				return;
			}
			else
				importer.setRecipeList( import_dialog.getSelectedRecipes() );
		}

		QString error = importer.getErrorMsg();
		if ( !error.isEmpty() ) {
			KMessageBox::sorry( this, error );
		}
		else
			importer.import(m_view->database);
	}
}

void Krecipes::pageSetupSlot()
{
	Recipe recipe;
	m_view->selectPanel->getCurrentRecipe( &recipe );

	KConfig *config = KGlobal::config();
	config->setGroup("Page Setup");
	QString layout = config->readEntry( "Layout", locate( "appdata", "layouts/None.klo" ) );
	QString printLayout = config->readEntry( "PrintLayout", locate( "appdata", "layouts/None.klo" ) );

	if ( layout == printLayout ) {
		KMessageBox::information( this, i18n("The recipe print and view layouts use the same file for their style, meaning changing one view's look changes them both.  If this is not the behavior you desire, load one style and save it under a different name."),
		QString::null, "sharedLayoutWarning" );
	}

	PageSetupDialog page_setup( this, recipe );
	page_setup.exec();
}

void Krecipes::printSetupSlot()
{
	Recipe recipe;
	m_view->selectPanel->getCurrentRecipe( &recipe );

	KConfig *config = KGlobal::config();
	config->setGroup("Page Setup");
	QString layout = config->readEntry( "Layout", locate( "appdata", "layouts/None.klo" ) );
	QString printLayout = config->readEntry( "PrintLayout", locate( "appdata", "layouts/None.klo" ) );

	if ( layout == printLayout ) {
		KMessageBox::information( this, i18n("The recipe print and view layouts use the same file for their style, meaning changing one view's look changes them both.  If this is not the behavior you desire, load one style and save it under a different name."),
		QString::null, "sharedLayoutWarning" );
	}

	PageSetupDialog pageSetup( this, recipe, "Print" );
	pageSetup.exec();
}

void Krecipes::conversionToolSlot()
{
	convertDialog->show();
}

void Krecipes::backupSlot()
{
	QString fileName = KFileDialog::getSaveFileName(QString::null,
		QString("*.krecbk|%1 (*.krecbk)").arg("Krecipes Backup File"),
		this,i18n("Save Backup As..."));

	int overwrite = KMessageBox::Yes;
	if ( QFile::exists(fileName) ) {
		overwrite = KMessageBox::warningYesNo( this, QString( i18n( "File \"%1\" exists.  Are you sure you want to overwrite it?" ) ).arg( fileName ) );
	}

	if ( !fileName.isNull() && overwrite == KMessageBox::Yes ) {
		ProgressInterface pi(this);
		pi.listenOn(m_view->database);

		QString errMsg;
		if ( !m_view->database->backup( fileName, &errMsg ) )
			KMessageBox::error( this, errMsg, i18n("Backup Failed") );
	}
}

void Krecipes::restoreSlot()
{
	QString filename = KFileDialog::getOpenFileName(QString::null,
		QString("*.krecbk|%1 (*.krecbk)").arg(i18n("Krecipes Backup File")),
		this,i18n("Restore Backup"));

	if ( !filename.isNull() ) {
		switch ( KMessageBox::warningContinueCancel(this,i18n("<b>Restoring this file will erase ALL data currently in the database!</b><br /><br />If you want to keep the recipes in your database, click \"Cancel\" and first export your recipes.  These can then be imported once the restore is complete.<br /><br />Are you sure you want to proceed?"),QString::null,KStandardGuiItem::cont(), KStandardGuiItem::cancel(), "RestoreWarning") ) {
		case KMessageBox::Continue: {
			ProgressInterface pi(this);
			pi.listenOn(m_view->database);

			QString errMsg;
			if ( m_view->database->restore( filename, &errMsg ) )
				KMessageBox::information(this,i18n("Restore successful."));
			else
				KMessageBox::error( this, errMsg, i18n("Restore Failed") );

			pi.listenOn(0);
			m_view->reload();
		}
		case KMessageBox::Cancel:
		default: break;
		}
	}
}

void Krecipes::mergeSimilarCategories()
{
	ElementList categories;
	m_view->database->loadCategories(&categories);
	SimilarCategoriesDialog dlg(categories,this);
	if ( dlg.exec() == QDialog::Accepted ) {
		Q3ValueList<int> ids = dlg.matches();
		QString name = dlg.element();

		int id = m_view->database->findExistingCategoryByName(name);
		if ( id == -1 ) {
			m_view->database->createNewCategory(name);
			id = m_view->database->lastInsertID();
		}

		for ( Q3ValueList<int>::const_iterator it = ids.begin(); it != ids.end(); ++it ) {
			if ( id != *it )
				m_view->database->mergeCategories(id, *it);
		}
	}
}

void Krecipes::mergeSimilarIngredients()
{
	ElementList ingredients;
	m_view->database->loadIngredients(&ingredients);
	SimilarCategoriesDialog dlg(ingredients,this);
	if ( dlg.exec() == QDialog::Accepted ) {
		Q3ValueList<int> ids = dlg.matches();
		QString name = dlg.element();

		if ( ids.isEmpty() || name.isEmpty() ) return;

		int id = m_view->database->findExistingIngredientByName(name);
		if ( id == -1 ) {
			m_view->database->createNewIngredient(name);
			id = m_view->database->lastInsertID();
		}

		for ( Q3ValueList<int>::const_iterator it = ids.begin(); it != ids.end(); ++it ) {
			if ( id != *it )
				m_view->database->mergeIngredients(id, *it);
		}
	}
}

//return true to close app
bool Krecipes::queryClose()
{
	if ( !m_view->inputPanel->everythingSaved() ) {
		switch ( KMessageBox::questionYesNoCancel( this,
		         i18n( "A recipe contains unsaved changes.\n"
		               "Do you want to save the changes before exiting?" ),
		         i18n( "Unsaved Changes" ) ) ) {
		case KMessageBox::Yes:
			return m_view->save();
		case KMessageBox::No:
			return true;
		case KMessageBox::Cancel:
			return false;
		default:
			return true;
		}
	}
	else
		return true;
}

void Krecipes::optionsShowToolbar()
{
	// this is all very cut and paste code for showing/hiding the
	// toolbar
	if ( m_toolbarAction->isChecked() )
		toolBar() ->show();
	else
		toolBar() ->hide();
}

void Krecipes::optionsShowStatusbar()
{
	// this is all very cut and paste code for showing/hiding the
	// statusbar
	if ( m_statusbarAction->isChecked() )
		statusBar() ->show();
	else
		statusBar() ->hide();
}

void Krecipes::optionsConfigureKeys()
{
#if KDE_IS_VERSION(3,1,92 )
	// for KDE 3.2: KKeyDialog::configureKeys is deprecated
	KShortcutsDialog::configure( actionCollection(), this, true );
#else

	KKeyDialog::configureKeys( actionCollection(), "krecipesui.rc" );
#endif

}

void Krecipes::optionsConfigureToolbars()
{
	// use the standard toolbar editor
#if defined(KDE_MAKE_VERSION)
# if KDE_VERSION >= KDE_MAKE_VERSION(3,1,0)
	saveMainWindowSettings( KGlobal::config(), autoSaveGroup() );
# else

	saveMainWindowSettings( KGlobal::config() );
# endif
#else

	saveMainWindowSettings( KGlobal::config() );
#endif

	KEditToolBar dlg( actionCollection() );
	connect( &dlg, SIGNAL( newToolbarConfig() ), this, SLOT( newToolbarConfig() ) );
	dlg.exec();
}

void Krecipes::newToolbarConfig()
{
	// this slot is called when user clicks "Ok" or "Apply" in the toolbar editor.
	// recreate our GUI, and re-apply the settings (e.g. "text under icons", etc.)
	createGUI();

#if defined(KDE_MAKE_VERSION)
# if KDE_VERSION >= KDE_MAKE_VERSION(3,1,0)

	applyMainWindowSettings( KGlobal::config(), autoSaveGroup() );
# else

	applyMainWindowSettings( KGlobal::config() );
# endif
#else

	applyMainWindowSettings( KGlobal::config() );
#endif
}

void Krecipes::optionsPreferences()
{

	// popup some sort of preference dialog, here
	KrecipesPreferences dlg( this );
	if ( dlg.exec() ) {}

}

void Krecipes::changeStatusbar( const QString& text )
{
	// display the text on the statusbar
	statusBar() ->message( text );
}

void Krecipes::changeCaption( const QString& text )
{
	// display the text on the caption
	setCaption( text );
}
void Krecipes::enableSaveOption( bool en )
{
	saveAction->setEnabled( en );
}

#include "krecipes.moc"

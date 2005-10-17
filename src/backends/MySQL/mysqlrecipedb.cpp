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

#include "mysqlrecipedb.h"

#include <kdebug.h>
#include <kstandarddirs.h>
#include <ktempfile.h>
#include <klocale.h>
#include <kconfig.h>
#include <kglobal.h>

MySQLRecipeDB::MySQLRecipeDB( const QString &host, const QString &user, const QString &pass, const QString &DBname ) : QSqlRecipeDB( host, user, pass, DBname )
{}

MySQLRecipeDB::~MySQLRecipeDB()
{}

void MySQLRecipeDB::createDB()
{
	QString real_db_name = database->databaseName();

	//we have to be connected to some database in order to create the Krecipes database
	//so long as the permissions given are allowed access to "mysql', this works
	database->setDatabaseName( "mysql" );
	if ( database->open() ) {
		// Create the Database (Note: needs permissions)
		//FIXME: I've noticed certain characters cause this to fail (such as '-').  Somehow let the user know.
		QSqlQuery query( QString( "CREATE DATABASE %1" ).arg( real_db_name ), database );
		if ( !query.isActive() )
			kdDebug() << "create query failed: " << database->lastError().databaseText() << endl;

		database->close();
	}
	else
		kdDebug() << "create open failed: " << database->lastError().databaseText() << endl;

	database->setDatabaseName( real_db_name );
}

QStringList MySQLRecipeDB::backupCommand() const
{
	KConfig *config = KGlobal::config();
	config->setGroup("Server");

	QStringList command;
	command<<config->readEntry( "MySQLDumpPath", "mysqldump" )<<"-q";

	QString pass = config->readEntry("Password", QString::null);
	if ( !pass.isEmpty() )
		command<<"-p"+pass;

	QString user = config->readEntry("Username", QString::null);
	command<<"-u"+user;

	command<<database->databaseName();
	return command;
}

QStringList MySQLRecipeDB::restoreCommand() const
{
	KConfig *config = KGlobal::config();
	config->setGroup("Server");

	QStringList command;
	command<<config->readEntry( "MySQLPath", "mysql" );

	QString pass = config->readEntry("Password", QString::null);
	if ( !pass.isEmpty() )
		command<<"-p"+pass;

	QString user = config->readEntry("Username", QString::null);
	command<<"-u"+user;

	command<<database->databaseName();
	return command;
}

void MySQLRecipeDB::createTable( const QString &tableName )
{

	QStringList commands;

	if ( tableName == "recipes" )
		commands << QString( "CREATE TABLE recipes (id INTEGER NOT NULL AUTO_INCREMENT,title VARCHAR(%1), yield_amount FLOAT, yield_amount_offset FLOAT, yield_type_id int(11) DEFAULT '-1', instructions TEXT, photo BLOB, prep_time TIME, ctime TIMESTAMP, mtime TIMESTAMP, atime TIMESTAMP,  PRIMARY KEY (id));" ).arg( maxRecipeTitleLength() );

	else if ( tableName == "ingredients" )
		commands << QString( "CREATE TABLE ingredients (id INTEGER NOT NULL AUTO_INCREMENT, name VARCHAR(%1), PRIMARY KEY (id));" ).arg( maxIngredientNameLength() );

	else if ( tableName == "ingredient_list" )
		commands << "CREATE TABLE ingredient_list (id INTEGER, recipe_id INTEGER, ingredient_id INTEGER, amount FLOAT, amount_offset FLOAT, unit_id INTEGER, order_index INTEGER, group_id INTEGER, PRIMARY KEY(id), INDEX ridil_index(recipe_id), INDEX iidil_index(ingredient_id), INDEX gidil_index(group_id));";

	else if ( tableName == "unit_list" )
		commands << "CREATE TABLE unit_list (ingredient_id INTEGER, unit_id INTEGER);";

	else if ( tableName == "units" )
		commands << QString( "CREATE TABLE units (id INTEGER NOT NULL AUTO_INCREMENT, name VARCHAR(%1), plural VARCHAR(%2), PRIMARY KEY (id));" ).arg( maxUnitNameLength() ).arg( maxUnitNameLength() );

	else if ( tableName == "prep_methods" )
		commands << QString( "CREATE TABLE prep_methods (id INTEGER NOT NULL AUTO_INCREMENT, name VARCHAR(%1), PRIMARY KEY (id));" ).arg( maxPrepMethodNameLength() );

	else if ( tableName == "prep_method_list" )
		commands << "CREATE TABLE prep_method_list (ingredient_list_id int(11) NOT NULL,prep_method_id int(11) NOT NULL, order_index int(11), INDEX  iid_index (ingredient_list_id), INDEX pid_index (prep_method_id));";

	else if ( tableName == "ingredient_info" )
		commands << "CREATE TABLE ingredient_info (ingredient_id INTEGER, property_id INTEGER, amount FLOAT, per_units INTEGER);";

	else if ( tableName == "ingredient_properties" )
		commands << "CREATE TABLE ingredient_properties (id INTEGER NOT NULL AUTO_INCREMENT,name VARCHAR(20), units VARCHAR(20), PRIMARY KEY (id));";

	else if ( tableName == "units_conversion" )
		commands << "CREATE TABLE units_conversion (unit1_id INTEGER, unit2_id INTEGER, ratio FLOAT);";

	else if ( tableName == "categories" )
		commands << QString( "CREATE TABLE categories (id int(11) NOT NULL auto_increment, name varchar(%1) default NULL, parent_id int(11) NOT NULL default -1, PRIMARY KEY (id));" ).arg( maxCategoryNameLength() );

	else if ( tableName == "category_list" )
		commands << "CREATE TABLE category_list (recipe_id int(11) NOT NULL,category_id int(11) NOT NULL, INDEX  rid_index (recipe_id), INDEX cid_index (category_id));";

	else if ( tableName == "authors" )
		commands << QString( "CREATE TABLE authors (id int(11) NOT NULL auto_increment, name varchar(%1) default NULL,PRIMARY KEY (id));" ).arg( maxAuthorNameLength() );

	else if ( tableName == "author_list" )
		commands << "CREATE TABLE author_list (recipe_id int(11) NOT NULL,author_id int(11) NOT NULL);";

	else if ( tableName == "db_info" ) {
		commands << "CREATE TABLE db_info (ver FLOAT NOT NULL,generated_by varchar(200) default NULL);";
		commands << QString( "INSERT INTO db_info VALUES(%1,'Krecipes %2');" ).arg( latestDBVersion() ).arg( krecipes_version() );
	}
	else if ( tableName == "ingredient_groups" ) {
		commands << QString( "CREATE TABLE `ingredient_groups` (`id` int(11) NOT NULL auto_increment, `name` varchar(%1), PRIMARY KEY (`id`));" ).arg( maxIngGroupNameLength() );
	}
	else if ( tableName == "yield_types" ) {
		commands << QString( "CREATE TABLE `yield_types` (`id` int(11) NOT NULL auto_increment, `name` varchar(%1), PRIMARY KEY (`id`));" ).arg( 20 );
	}

	else if ( tableName == "ratings" )
		commands << "CREATE TABLE ratings (id INTEGER NOT NULL AUTO_INCREMENT, recipe_id int(11) NOT NULL, comment TEXT, rater TEXT, created TIMESTAMP, PRIMARY KEY (id));";

	else if ( tableName == "rating_criteria" )
		commands << "CREATE TABLE rating_criteria (id INTEGER NOT NULL AUTO_INCREMENT, name TEXT, PRIMARY KEY (id));";

	else if ( tableName == "rating_criterion_list" )
		commands << "CREATE TABLE rating_criterion_list (rating_id INTEGER NOT NULL, rating_criterion_id INTEGER, stars FLOAT);";

	else
		return ;

	QSqlQuery databaseToCreate( QString::null, database );

	// execute the queries
	for ( QStringList::const_iterator it = commands.begin(); it != commands.end(); ++it )
		databaseToCreate.exec( ( *it ) );
}

void MySQLRecipeDB::portOldDatabases( float version )
{
	kdDebug() << "Current database version is..." << version << "\n";
	QString command;

	// Note that version no. means the version in which this DB structure
	// was introduced.  To work with SVN users, the database will be incrementally
	// upgraded for each change made between releases (e.g. 0.81, 0.82,... are
	// what will become 0.9)

	if ( qRound(version*10) < 3 ) 	// The database was generated with a version older than v 0.3. First update to 0.3 version
	{

		// Add new columns to existing tables (creating new tables is not necessary. Integrity check does that before)
		command = "ALTER TABLE recipes ADD COLUMN persons int(11) AFTER title;";
		QSqlQuery tableToAlter( command, database );

		// Set the version to the new one (0.3)

		command = "DELETE FROM db_info;"; // Remove previous version records if they exist
		tableToAlter.exec( command );
		command = "INSERT INTO db_info VALUES(0.3,'Krecipes 0.4');"; // Set the new version
		tableToAlter.exec( command );
	}

	if ( qRound(version*10) < 4 )   // Upgrade to the current DB version 0.4
	{

		// Add new columns to existing tables (creating any new tables is not necessary. Integrity check does that before)
		command = "ALTER TABLE ingredient_list ADD COLUMN order_index int(11) AFTER unit_id;";
		QSqlQuery tableToAlter( command, database );

		// Missing indexes in the previous versions
		command = "CREATE index rid_index ON category_list(recipe_id)";
		tableToAlter.exec( command );

		command = "CREATE index cid_index ON category_list(category_id)";
		tableToAlter.exec( command );

		command = "CREATE index ridil_index ON ingredient_list(recipe_id)";
		tableToAlter.exec( command );

		command = "CREATE index iidil_index ON ingredient_list(ingredient_id)";
		tableToAlter.exec( command );

		// Port data

		//*1:: Recipes have always category -1 to speed up searches (no JOINs needed)
		command = "SELECT r.id FROM recipes r;"; // Find all recipes
		QSqlQuery categoryToAdd( QString::null, database );
		tableToAlter.exec( command );
		if ( tableToAlter.isActive() )
		{
			while ( tableToAlter.next() ) {
				int recipeId = tableToAlter.value( 0 ).toInt();
				QString cCommand = QString( "INSERT INTO category_list VALUES (%1,-1);" ).arg( recipeId );
				categoryToAdd.exec( cCommand );

				emit progress();
			}
		}

		// Set the version to the new one (0.4)

		command = "DELETE FROM db_info;"; // Remove previous version records if they exist
		tableToAlter.exec( command );
		command = "INSERT INTO db_info VALUES(0.4,'Krecipes 0.4');"; // Set the new version
		tableToAlter.exec( command );
	}

	if ( qRound(version*10) < 5 ) {
		command = QString( "CREATE TABLE prep_methods (id INTEGER NOT NULL AUTO_INCREMENT, name VARCHAR(%1), PRIMARY KEY (id));" ).arg( maxPrepMethodNameLength() );
		QSqlQuery tableToAlter( command, database );

		command = "ALTER TABLE ingredient_list ADD COLUMN prep_method_id int(11) AFTER unit_id;";
		tableToAlter.exec( command );
		command = "UPDATE ingredient_list SET prep_method_id=-1 WHERE prep_method_id IS NULL;";
		tableToAlter.exec( command );

		command = "ALTER TABLE authors MODIFY name VARCHAR(50);";
		tableToAlter.exec( command );
		command = "ALTER TABLE categories MODIFY name VARCHAR(40);";
		tableToAlter.exec( command );

		// Set the version to the new one (0.5)
		command = "DELETE FROM db_info;"; // Remove previous version records if they exist
		tableToAlter.exec( command );
		command = "INSERT INTO db_info VALUES(0.5,'Krecipes 0.5');";
		tableToAlter.exec( command );
	}

	if ( qRound(version*10) < 6 ) {
		command = "ALTER TABLE categories ADD COLUMN parent_id int(11) NOT NULL default '-1' AFTER name;";
		QSqlQuery tableToAlter( command, database );

		command = "DELETE FROM db_info;"; // Remove previous version records if they exist
		tableToAlter.exec( command );
		command = "INSERT INTO db_info VALUES(0.6,'Krecipes 0.6');";
		tableToAlter.exec( command );
	}

	if ( qRound(version*100) < 61 ) {
		QString command = "ALTER TABLE `recipes` ADD COLUMN `prep_time` TIME DEFAULT NULL";
		QSqlQuery tableToAlter( command, database );

		command = "DELETE FROM db_info;"; // Remove previous version records if they exist
		tableToAlter.exec( command );
		command = "INSERT INTO db_info VALUES(0.61,'Krecipes 0.6');";
		tableToAlter.exec( command );
	}

	if ( qRound(version*100) < 62 ) {
		QString command = "ALTER TABLE `ingredient_list` ADD COLUMN `group_id` int(11) default '-1' AFTER order_index;";
		QSqlQuery tableToAlter( command, database );

		command = "DELETE FROM db_info;"; // Remove previous version records if they exist
		tableToAlter.exec( command );
		command = "INSERT INTO db_info VALUES(0.62,'Krecipes 0.7');";
		tableToAlter.exec( command );
	}

	if ( qRound(version*100) < 63 ) {
		QString command = "ALTER TABLE `units` ADD COLUMN `plural` varchar(20) DEFAULT NULL AFTER name;";
		QSqlQuery tableToAlter( command, database );

		QSqlQuery result( "SELECT id,name FROM units WHERE plural IS NULL", database );
		if ( result.isActive() ) {
			while ( result.next() ) {
				command = "UPDATE units SET plural='" + result.value( 1 ).toString() + "' WHERE id=" + QString::number( result.value( 0 ).toInt() );
				QSqlQuery query( command, database );

				emit progress();
			}
		}

		command = "DELETE FROM db_info;"; // Remove previous version records if they exist
		tableToAlter.exec( command );
		command = "INSERT INTO db_info VALUES(0.63,'Krecipes 0.7');";
		tableToAlter.exec( command );
	}

	if ( qRound(version*10) < 7 ) { //simply call 0.63 -> 0.7
		QString command = "UPDATE db_info SET ver='0.7';";
		QSqlQuery query( command, database );
	}

	if ( qRound(version*100) < 81 ) {
		QString command = "ALTER TABLE `ingredient_list` ADD COLUMN `amount_offset` FLOAT DEFAULT '0' AFTER amount;";
		QSqlQuery tableToAlter( command, database );

		command = "UPDATE db_info SET ver='0.81',generated_by='Krecipes SVN (20050816)';";
		tableToAlter.exec( command );
	}

	if ( qRound(version*100) < 82 ) {
		QString command = "ALTER TABLE `recipes` ADD COLUMN `yield_amount` FLOAT DEFAULT '0' AFTER persons;";
		QSqlQuery tableToAlter( command, database );

		command = "ALTER TABLE `recipes` ADD COLUMN `yield_amount_offset` FLOAT DEFAULT '0' AFTER yield_amount;";
		tableToAlter.exec(command);

		command = "ALTER TABLE `recipes` ADD COLUMN `yield_type_id` INTEGER DEFAULT '-1' AFTER yield_amount_offset;";
		tableToAlter.exec(command);

		QSqlQuery result( "SELECT id,persons FROM recipes", database );
		if ( result.isActive() ) {
			while ( result.next() ) {
				command = "UPDATE recipes SET yield_amount='" + QString::number( result.value( 1 ).toInt() ) + "' WHERE id=" + QString::number( result.value( 0 ).toInt() );
				QSqlQuery query( command, database );

				emit progress();
			}
		}

		command = "ALTER TABLE `recipes` DROP COLUMN `persons`;";
		tableToAlter.exec( command );

		command = "UPDATE db_info SET ver='0.82',generated_by='Krecipes SVN (20050902)';";
		tableToAlter.exec( command );
	}

	if ( qRound(version*100) < 83 ) {
		database->transaction();

		//====add a id columns to 'ingredient_list' to identify it for the prep method list
		database->exec( "RENAME TABLE ingredient_list TO ingredient_list_copy;" );
		database->exec( "CREATE TABLE ingredient_list (id INTEGER NOT NULL AUTO_INCREMENT, recipe_id INTEGER, ingredient_id INTEGER, amount FLOAT, amount_offset FLOAT, unit_id INTEGER, order_index INTEGER, group_id INTEGER, PRIMARY KEY(id), INDEX ridil_index(recipe_id), INDEX iidil_index(ingredient_id));" );

		QSqlQuery copyQuery = database->exec( "SELECT recipe_id,ingredient_id,amount,amount_offset,unit_id,prep_method_id,order_index,group_id FROM ingredient_list_copy" );
		if ( copyQuery.isActive() ) {
			while ( copyQuery.next() ) {
				QSqlQuery query(database);
 				query.prepare( "INSERT INTO ingredient_list VALUES (NULL, ?, ?, ?, ?, ?, ?, ?)" );
				query.addBindValue( copyQuery.value( 0 ) );
				query.addBindValue( copyQuery.value( 1 ) );
				query.addBindValue( copyQuery.value( 2 ) );
				query.addBindValue( copyQuery.value( 3 ) );
				query.addBindValue( copyQuery.value( 4 ) );
				query.addBindValue( copyQuery.value( 6 ) );
				query.addBindValue( copyQuery.value( 7 ) );
				query.exec();

				int prep_method_id = copyQuery.value( 5 ).toInt();
				if ( prep_method_id != -1 ) {
					query.prepare( "INSERT INTO prep_method_list VALUES (?, ?, ?);" );
					query.addBindValue( lastInsertID() );
					query.addBindValue( prep_method_id );
					query.addBindValue( 1 );
					query.exec();
				}

				emit progress();
			}
		}
		database->exec( "DROP TABLE ingredient_list_copy" );

		database->exec( "UPDATE db_info SET ver='0.83',generated_by='Krecipes SVN (20050909)';" );

		if ( !database->commit() )
			kdDebug()<<"Update to 0.83 failed.  Maybe you should try again."<<endl;
	}

	if ( qRound(version*100) < 84 ) {
		database->transaction();

		database->exec( "ALTER TABLE recipes ADD COLUMN ctime TIMESTAMP;" );
		database->exec( "ALTER TABLE recipes ADD COLUMN mtime TIMESTAMP;" );
		database->exec( "ALTER TABLE recipes ADD COLUMN atime TIMESTAMP;" );

		database->exec( "UPDATE recipes SET ctime=CURRENT_TIMESTAMP, mtime=CURRENT_TIMESTAMP, atime=CURRENT_TIMESTAMP;" );

		database->exec( "UPDATE db_info SET ver='0.84',generated_by='Krecipes SVN (20050913)';" );

		if ( !database->commit() )
			kdDebug()<<"Update to 0.84 failed.  Maybe you should try again."<<endl;
	}

	if ( qRound(version*100) < 85 ) {
		database->transaction();

		QSqlQuery query( "SELECT id,photo FROM recipes", database );
	
		if ( query.isActive() ) {
			while ( query.next() ) {
				storePhoto( query.value(0).toInt(), query.value(1).toByteArray() );

				emit progress();
			}
		}


		database->exec( "UPDATE db_info SET ver='0.85',generated_by='Krecipes SVN (20050926)';" );
		if ( !database->commit() )
			kdDebug()<<"Update to 0.85 failed.  Maybe you should try again."<<endl;
	}

	if ( qRound(version*100) < 86 ) {
		database->transaction();

		database->exec( "ALTER TABLE ingredient_list ADD INDEX (group_id)" );

		QSqlQuery query( "SELECT id,name FROM ingredient_groups ORDER BY name", database );

		QString last;
		int lastID;
		if ( query.isActive() ) {
			while ( query.next() ) {
				QString name = query.value(1).toString();
				int id = query.value(0).toInt();
				if ( last == name ) {
					QString command = QString("UPDATE ingredient_list SET group_id=%1 WHERE group_id=%2").arg(lastID).arg(id);
					database->exec(command);

					command = QString("DELETE FROM ingredient_groups WHERE id=%1").arg(id);
					database->exec(command);
				}
				last = name;
				lastID = id;

				emit progress();
			}
		}

		database->exec( "UPDATE db_info SET ver='0.86',generated_by='Krecipes SVN (20050928)';" );
		if ( !database->commit() )
			kdDebug()<<"Update to 0.86 failed.  Maybe you should try again."<<endl;
	}

	if ( qRound(version*100) < 87 ) {
		database->exec( "UPDATE db_info SET ver='0.87',generated_by='Krecipes SVN (20051014)'" );
	}
}

int MySQLRecipeDB::lastInsertID()
{
	QSqlQuery lastInsertID( "SELECT LAST_INSERT_ID();", database );

	int id = -1;
	if ( lastInsertID.isActive() && lastInsertID.next() )
		id = lastInsertID.value( 0 ).toInt();

	return id;
}

void MySQLRecipeDB::givePermissions( const QString &dbName, const QString &username, const QString &password, const QString &clientHost )
{
	QString command;

	if ( !password.isEmpty() )
		command = QString( "GRANT ALL ON %1.* TO '%2'@'%3' IDENTIFIED BY '%4';" ).arg( dbName ).arg( username ).arg( clientHost ).arg( password );
	else
		command = QString( "GRANT ALL ON %1.* TO '%2'@'%3';" ).arg( dbName ).arg( username ).arg( clientHost );

	kdDebug() << "I'm doing the query to setup permissions\n";

	QSqlQuery permissionsToSet( command, database );
}

#include "mysqlrecipedb.moc"

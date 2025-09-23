/*
This file contains database initialization and setup functions.
It handles database creation, table creation, and initial setup.
*/
#ifndef SQL_SETUP_H
#define SQL_SETUP_H

#include <sqlite3.h>
#include <string>
#include <stdexcept>

namespace sql {
    
    // Custom exception class for database setup errors
    class DatabaseSetupException : public std::runtime_error {
    public:
        DatabaseSetupException(const std::string& message) : std::runtime_error(message) {}
    };
    
    // Database path - corrected to be relative to project root
    extern const std::string dbpath;
    
    // Run database migrations
    bool run_migrations();
    
    // Initialize the database and create tables if they don't exist
    bool initialize_database();
    
    // Create all necessary tables
    bool create_tables();
    
    // Check if database exists
    bool database_exists();
    
    // Get database version (for future migrations)
    int get_database_version();
    
    // Set database version
    bool set_database_version(int version);
    
} // namespace sql

#endif // SQL_SETUP_H

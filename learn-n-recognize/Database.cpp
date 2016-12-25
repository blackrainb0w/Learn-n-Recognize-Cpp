//
//  Database.cpp
//  learn-n-recognize
//
//  Created by Samuel Prevost on 22/12/2016.
//  Copyright © 2016 Samuel Prevost. All rights reserved.
//

#include "Database.hpp"
#include <iostream>

// Open already existing one
Database::Database(string pathToDB){
    if(!this->open(pathToDB))
        ErrorAccessDBMessage(sqlite3_errmsg(db));
    SuccessAccessDBMessage();
}

// Initialize a new one
Database::Database(){
    if(!this->create(DEFAULT_DB_PATH, this->db))
        ErrorCreateDBMessage(sqlite3_errmsg(db));
    SuccessCreateDBMessage();
    
}

Database::~Database(){
    this->close();
}
    
bool Database::open(string path){
    return sqlite3_open_v2(path.c_str(), &this->db, SQLITE_OPEN_READWRITE, NULL) == SQLITE_OK ? true : false;
}

bool Database::create(string path, sqlite3* db){
    string req = "CREATE TABLE `subjects` (`id`  INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE, `name`	varchar(255) UNIQUE);";
    sqlite3_stmt* stmt;
    if (sqlite3_open_v2(path.c_str(), &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL) == SQLITE_OK){
        sqlite3_prepare(db, req.c_str(), -1, &stmt, NULL);
        
        if(sqlite3_step(stmt) != SQLITE_DONE)
            return false;
    }
    else
        return false;
    
    sqlite3_finalize(stmt);
    return true;
}

//    int emptyCallback(void *NotUsed, int argc, char **argv, char **azColName){
//        return 0;
//    }

int Database::insertSubject(string subjectName){
    string sql = "INSERT INTO subjects (NAME) VALUES (\"" + subjectName + "\");";
    char *zErrMsg = 0;
    /* Execute SQL statement */
    int rc = sqlite3_exec(this->db, sql.c_str(), NULL, 0, &zErrMsg);
    if( rc != SQLITE_OK ){
        sqlite3_free(zErrMsg);
        return NULL;
    }else{
        return getSubjectID(subjectName);
    }
}

vector<vector<string>> Database::query(string query){
    sqlite3_stmt *statement;
    vector<vector<string>> results;
    
    if(sqlite3_prepare_v2(this->db, query.c_str(), -1, &statement, 0) == SQLITE_OK)
    {
        int cols = sqlite3_column_count(statement);
        int result = 0;
        while(true)
        {
            result = sqlite3_step(statement);
            
            if(result == SQLITE_ROW)
            {
                vector<string> values;
                for(int col = 0; col < cols; col++)
                {
                    string val;
                    char * ptr = (char*)sqlite3_column_text(statement, col);
                    ptr ? val = ptr : val = "";
                    values.push_back(val);
                }
                results.push_back(values);
            }
            else
            {
                break;  
            }

        }
        
        sqlite3_finalize(statement);
    }
    
    // If there is an error, SQLITE_ERROR will be the first value of the first column of the first row
    string error = sqlite3_errmsg(this->db);
    if(error != "not an error"){
        ErrorExecuteQueryDBMessage(query, error);
        vector<string> values;
        values.push_back(to_string(SQLITE_ERROR));
        results.push_back(values);
    }
    
    return results;  
}

string Database::getSubjectName(int subjectID){
    string name = (this->query("SELECT name FROM subjects WHERE id=" + to_string(subjectID) + ";")[0])[0];
    return name;
//    return name == to_string(SQLITE_ERROR) ? NULL : name;
}

int Database::getSubjectID(string subjectName){
    string name = (this->query("SELECT id FROM subjects WHERE name=\"" + subjectName + "\";")[0])[0];
    return name == to_string(SQLITE_ERROR) ? NULL : stoi(name);
}

void Database::close(){
    sqlite3_close(this->db);
}

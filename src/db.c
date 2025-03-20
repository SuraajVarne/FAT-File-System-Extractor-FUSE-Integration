#include <stdio.h>
#include <sqlite3.h>
#include "db.h"
#include "fat_defs.h"

int db_init(const char* db_path) {
    sqlite3* db;
    int rc = sqlite3_open(db_path, &db);
    if (rc) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        return -1;
    }
    const char* sql = "CREATE TABLE IF NOT EXISTS metadata ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "vol_label TEXT,"
        "bytes_per_sector INTEGER,"
        "sectors_per_cluster INTEGER"
        ");";
    char* err_msg = NULL;
    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
        sqlite3_close(db);
        return -1;
    }
    sqlite3_close(db);
    return 0;
}

int db_insert_metadata(const char* db_path, FAT32_BootSector* bs) {
    sqlite3* db;
    int rc = sqlite3_open(db_path, &db);
    if (rc) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        return -1;
    }
    char sql[512];
    snprintf(sql, sizeof(sql),
        "INSERT INTO metadata (vol_label, bytes_per_sector, sectors_per_cluster) VALUES ('%.11s', %u, %u);",
        bs->BS_VolLab, bs->BPB_BytsPerSec, bs->BPB_SecPerClus);
    char* err_msg = NULL;
    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
        sqlite3_close(db);
        return -1;
    }
    sqlite3_close(db);
    return 0;
}

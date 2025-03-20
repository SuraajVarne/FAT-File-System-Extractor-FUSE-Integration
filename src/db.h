#ifndef DB_H
#define DB_H

// Initialize the SQLite database at the given path.
int db_init(const char* db_path);
// Insert FAT metadata (for example, volume label and boot sector info).
int db_insert_metadata(const char* db_path, FAT32_BootSector* bs);

#endif // DB_H
#pragma once

#ifndef ST_FILES_H
#define ST_FILES_H

/*
    Simple Think File Library
*/

typedef struct STFile
{
    void* handle;
    char* path;
    char* mode;
} STFile;

int st_file_exists(const char* path);

STFile* st_file_open(const char* path, const char* mode);
void st_file_close(STFile* file);

char* st_file_read_all(STFile* file);
int st_file_write(STFile* file, const char* text);
int st_file_append(STFile* file, const char* text);

int st_file_read_line(STFile* file, char* buffer, int buffer_size);

long st_file_size(STFile* file);

int st_file_delete(const char* path);
int st_file_rename(const char* old_path, const char* new_path);

const char* st_file_path(const STFile* file);

#endif
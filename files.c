#include "files.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

int st_file_exists(const char* path)
{
    struct stat info;

    if (path == NULL)
        return 0;

    return stat(path, &info) == 0;
}

STFile* st_file_open(const char* path, const char* mode)
{
    STFile* file;
    FILE* handle;

    if (path == NULL || mode == NULL)
        return NULL;

    handle = fopen(path, mode);

    if (handle == NULL)
        return NULL;

    file = (STFile*)malloc(sizeof(STFile));

    if (file == NULL)
    {
        fclose(handle);
        return NULL;
    }

    file->handle = handle;

    file->path = (char*)malloc(strlen(path) + 1);

    if (file->path == NULL)
    {
        fclose(handle);
        free(file);
        return NULL;
    }

    strcpy(file->path, path);

    file->mode = (char*)malloc(strlen(mode) + 1);

    if (file->mode == NULL)
    {
        fclose(handle);
        free(file->path);
        free(file);
        return NULL;
    }

    strcpy(file->mode, mode);

    return file;
}

void st_file_close(STFile* file)
{
    if (file == NULL)
        return;

    if (file->handle != NULL)
        fclose((FILE*)file->handle);

    free(file->path);
    free(file->mode);
    free(file);
}

char* st_file_read_all(STFile* file)
{
    FILE* handle;
    long size;
    long read_size;
    char* buffer;

    if (file == NULL || file->handle == NULL)
        return NULL;

    handle = (FILE*)file->handle;

    if (fseek(handle, 0, SEEK_END) != 0)
        return NULL;

    size = ftell(handle);

    if (size < 0)
        return NULL;

    rewind(handle);

    buffer = (char*)malloc((size_t)size + 1);

    if (buffer == NULL)
        return NULL;

    read_size = (long)fread(buffer, 1, (size_t)size, handle);

    buffer[read_size] = '\0';

    return buffer;
}

int st_file_write(STFile* file, const char* text)
{
    FILE* handle;

    if (file == NULL ||
        file->handle == NULL ||
        text == NULL)
    {
        return 0;
    }

    handle = (FILE*)file->handle;

    return fputs(text, handle) >= 0;
}

int st_file_append(STFile* file, const char* text)
{
    FILE* handle;

    if (file == NULL ||
        file->handle == NULL ||
        text == NULL)
    {
        return 0;
    }

    handle = (FILE*)file->handle;

    if (fseek(handle, 0, SEEK_END) != 0)
        return 0;

    return fputs(text, handle) >= 0;
}

int st_file_read_line(STFile* file, char* buffer, int buffer_size)
{
    FILE* handle;

    if (file == NULL ||
        file->handle == NULL ||
        buffer == NULL ||
        buffer_size <= 0)
    {
        return 0;
    }

    handle = (FILE*)file->handle;

    return fgets(buffer, buffer_size, handle) != NULL;
}

long st_file_size(STFile* file)
{
    FILE* handle;
    long current;
    long size;

    if (file == NULL || file->handle == NULL)
        return -1;

    handle = (FILE*)file->handle;

    current = ftell(handle);

    if (current < 0)
        return -1;

    if (fseek(handle, 0, SEEK_END) != 0)
        return -1;

    size = ftell(handle);

    fseek(handle, current, SEEK_SET);

    return size;
}

int st_file_delete(const char* path)
{
    if (path == NULL)
        return 0;

    return remove(path) == 0;
}

int st_file_rename(const char* old_path, const char* new_path)
{
    if (old_path == NULL || new_path == NULL)
        return 0;

    return rename(old_path, new_path) == 0;
}

const char* st_file_path(const STFile* file)
{
    if (file == NULL)
        return NULL;

    return file->path;
}
#include <utils.hpp>
#include <malloc.h>
#include <sys/stat.h>
#include <unistd.h>
#include "log.hpp"
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

void add_to_linked_list(linked_list** list, void* value)
{
    linked_list* next = *list;

    *list = (linked_list*)malloc(sizeof(linked_list));
    (*list)->next = next;
    (*list)->value = value;
}
void free_linked_list(linked_list** list)
{
    linked_list* current = *list;

    while (current != NULL)
    {
        linked_list* next = current->next;

        free(current);

        current = next;
    }

    *list = NULL;
}
void free_linked_list_and_its_value(linked_list** list)
{
    linked_list* current = *list;

    while (current != NULL)
    {
        linked_list* next = current->next;

        free(current->value);
        free(current);

        current = next;
    }

    *list = NULL;
}

char* read_file(const char* path)
{
    int fd = open(path, O_RDONLY | O_CLOEXEC);
    if(fd < 0)
    {
        LOGI("There was a problem when opening %s", path);
        return NULL;
    }

    struct stat file_stat;
    
    if(fstat(fd, &file_stat) != 0)
    {
        close(fd);
        LOGI("There was a problem when stating %s", path);
        return NULL;
    }

    char* buffer = (char*)malloc(file_stat.st_size + 1);
    *(buffer + file_stat.st_size) = 0;
    size_t read_position = 0;

    while (read_position < (size_t)file_stat.st_size) {
        ssize_t read_amount = read(fd, buffer + read_position, file_stat.st_size - read_position);

        if(read_amount <= 0)
        {
            break;
        }

        read_position += read_amount;
    }

    LOGI("Read %s %zu/%lld", path, read_position, file_stat.st_size);

    close(fd);

    return buffer;
}

int search_parse_config(const char* config, const char* process_name, linked_list** output_dl_to_load)
{
    const char* read_position = config;
    size_t process_name_len = strlen(process_name);

    int count = 0;

    while(true)
    {
        const char* target_process_name = strstr(read_position, " : ");
        if(target_process_name == NULL)
        {
            break;
        }

        size_t target_process_name_len = target_process_name - read_position;
        if(process_name_len != target_process_name_len || memcmp(process_name, read_position, process_name_len) != 0)
        {
            read_position = strstr(read_position, "\n");
            if(read_position == NULL)
            {
                break;
            }
            else
            {
                read_position += 1;
            }
            continue;
        }

        const char* dl_path = target_process_name + 3;
        const char* dl_path_end = strstr(dl_path, "\n");
        size_t dl_path_len = 0;

        if(dl_path_end == NULL)
        {
            dl_path_len = strlen(dl_path);
        }
        else
        {
            dl_path_len = dl_path_end - dl_path;
        }

        char* dl_path_alloc = (char*)malloc(dl_path_len + 1);
        *(dl_path_alloc + dl_path_len) = 0;

        memcpy(dl_path_alloc, dl_path, dl_path_len);
        
        add_to_linked_list(output_dl_to_load, dl_path_alloc);

        count += 1;

        read_position = dl_path_end;
        if(read_position == NULL)
        {
            break;
        }
        else
        {
            read_position += 1;
        }
    }

    return count;
}
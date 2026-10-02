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

    bool add = false;
    linked_list* current_list = NULL;

    while(true)
    {
        if(*read_position == 0)
        {
            if(current_list != NULL)
            {
                add_to_linked_list(output_dl_to_load, current_list);
                count += 1;
                current_list = NULL;
            }
            break;
        }
        if(*read_position == '\n')
        {
            read_position += 1;
            continue;
        }

        if(*read_position == '\t' && add == true)
        {
            read_position += 1;

            const char* end = strstr(read_position, "\n");
            int len = 0;

            if(end == NULL)
            {
                len = strlen(read_position);
            }
            else
            {
                len = end - read_position;
            }

            char* str = (char*)malloc(len + 1);
            *(str + len) = 0;
            memcpy(str, read_position, len);
            add_to_linked_list(&current_list, str);

            read_position += len;
            continue;
        }

        if(add == true)
        {
            add = false;

            if(current_list != NULL)
            {
                add_to_linked_list(output_dl_to_load, current_list);
                count += 1;
                current_list = NULL;
                continue;
            }
        }

        const char* target_end = strstr(read_position, "\n");
        int target_len = 0;

        if(target_end == NULL)
        {
            target_len = strlen(read_position);
        }
        else
        {
            target_len = target_end - read_position;
        }

        if(target_len != process_name_len)
        {
            read_position += target_len;
            continue;
        }

        if(memcmp(read_position, process_name, target_len) == 0)
        {
            add = true;
        }

        read_position += target_len;
    }

    return count;
}
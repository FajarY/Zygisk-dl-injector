#pragma once

struct linked_list
{
    void* value;
    linked_list* next;
};

void add_to_linked_list(linked_list** list, void* value);
void free_linked_list(linked_list** list);
void free_linked_list_and_its_value(linked_list** list);

char* read_file(const char* path);
int search_parse_config(const char* config, const char* process_name, linked_list** output_dl_to_load);
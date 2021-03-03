#pragma once

struct adg_section
{
    char* name;
};

struct adg_action;

struct adg_section**
adg_section_table_create(struct adg_action** action_table);

void
adg_section_table_destroy(struct adg_section** section_table);

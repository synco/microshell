#include "ush.h"

#include <string.h>

void ush_cmd_get_full_path(struct ush_object *self, struct ush_path_object *path_obj, const char *name, char *path, size_t max_len)
{
        ush_path_get_full_path(self, path_obj, path, max_len);
        
        if (strcmp(path, "/") == 0) {
                strcat(path, name);
        } else {
                strcat(path, "/");
                strcat(path, name);
        }
}

struct ush_cmd_descriptor const* ush_cmd_find_by_name(struct ush_object *self, const char *name)
{
        char cmd_path[USH_CONFIG_CURRENT_DIR_MAX_SIZE * 2];
        char full_path[USH_CONFIG_CURRENT_DIR_MAX_SIZE];

        struct ush_path_object *curr;
        struct ush_cmd_descriptor const *cmd;

        ush_path_get_absolute_path(self, name, full_path, sizeof(full_path));

        curr = self->path_first;        
        while (curr != NULL) {
                if (curr->mount_point != NULL) {
                        for (size_t i = 0; i < curr->cmd_list_size; i++) {
                                cmd = &curr->cmd_list[i];
                                ush_cmd_get_full_path(self, curr, cmd->name, cmd_path, sizeof(cmd_path));
                                if (strcmp(cmd_path, full_path) == 0)
                                        return cmd;
                        }
                } else {
                        for (size_t i = 0; i < curr->cmd_list_size; i++) {
                                cmd = &curr->cmd_list[i];
                                if (strcmp(cmd->name, name) == 0)
                                        return cmd;
                        }
                }

                curr = curr->next;                
        }

        return NULL;
}

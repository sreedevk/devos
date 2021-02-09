#pragma once
#include <stdint.h>
#include <stddef.h>

void initialize_storage();
void storage_write(const char *key, int32_t value);
void storage_read(const char *key, int32_t *value);
void storage_write_string(const char *key, const char *value);
void storage_read_string(const char *key, char *value, size_t *strlength);
void init_filesystem();
void unmount_filesystem();

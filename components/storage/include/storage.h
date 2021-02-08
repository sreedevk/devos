#pragma once
#include <stdint.h>

void initialize_storage();
void storage_write(const char *key, int32_t value);
void storage_read(const char *key, int32_t *value);

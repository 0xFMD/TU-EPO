#ifndef __ZVM_BLOCK_H__
#define __ZVM_BLOCK_H__

#define ZVM_BLOCK_MAX_SIZE ((1024 * 1024 * 1024) / 4)
#define zvm_block_is_valid_size(b) ((b) > 0 && ((b) <= ZVM_BLOCK_MAX_SIZE))

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define zvm_block_init(b, es, ec)                                              \
  (b)->offset = -1;                                                            \
  (b)->element_size = (es);                                                    \
  (b)->element_count = (ec);                                                   \
  memset((b)->base, 0, zvm_block_size((b)));

#define zvm_block_get(b) ((b)->base[(b)->offset])
#define zvm_block_set(b, v) ((b)->base[(b)->offset]) = (v)

#define zvm_block_size(b) ((b)->element_size * (b)->element_count)
#define zvm_block_elements_count(b) ((b)->element_count)

#define zvm_block_is_offset(b)                                                 \
  (((b)->offset < zvm_block_size((b))) && ((b)->offset >= 0))
#define zvm_block_is_valid_offset(b, p) ((p) < zvm_block_size((b)) && (p) >= 0)

#define zvm_block_is_full(b) ((b)->offset == zvm_block_size((b)) - 1)
#define zvm_block_is_empty(b) ((b)->offset == -1)

typedef struct zvm_block_t zvm_block_t;
typedef int32_t zvm_block_offset_t;
typedef uint16_t zvm_block_count_t;

struct zvm_block_t {
  uint8_t *base;
  zvm_block_count_t element_count;
  uint8_t element_size;
  zvm_block_offset_t offset;
};

zvm_block_t *zvm_block_new(zvm_block_count_t size, uint8_t element_size);
bool zvm_block_delete(zvm_block_t *block);
bool zvm_block_read_next(zvm_block_t *block, uint8_t *val);
bool zvm_block_read_prev(zvm_block_t *block, uint8_t *val);
bool zvm_block_rewind(zvm_block_t *block);
bool zvm_block_seek(zvm_block_t *block, const zvm_block_offset_t pos);
bool zvm_block_peek(const zvm_block_t *block, uint8_t *out);
bool zvm_block_write_at(zvm_block_t *block, const uint8_t val,
                        const zvm_block_offset_t pos);

bool zvm_block_write_bytes(zvm_block_t *block, const uint8_t *data,
                           const size_t len);
bool zvm_block_read_bytes(zvm_block_t *block, uint8_t *out, const size_t len);
zvm_block_t *zvm_block_clone(const zvm_block_t *src);
bool zvm_block_fill(zvm_block_t *block, uint8_t byte);
bool zvm_block_copy_data(zvm_block_t *dst, const zvm_block_t *src);
bool zvm_block_compare(const zvm_block_t *a, const zvm_block_t *b);
bool zvm_block_resize(zvm_block_t *block, zvm_block_count_t new_count);

#endif
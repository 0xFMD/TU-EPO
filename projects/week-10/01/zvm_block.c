#include "zvm_block.h"

zvm_block_t *zvm_block_new(zvm_block_count_t elements_count,
                           uint8_t element_size) {
  if (!zvm_block_is_valid_size(element_size * elements_count)) {
    return NULL;
  }

  zvm_block_t *block = (zvm_block_t *)malloc(sizeof(zvm_block_t));
  if (block) {
    block->base = (uint8_t *)malloc((element_size) * (elements_count));
    if (!block->base) {
      free(block);
      return NULL;
    }
  } else
    return NULL;

  zvm_block_init(block, element_size, elements_count);
  return block;
}

bool zvm_block_delete(zvm_block_t *block) {
  if (!block)
    return false;

  if (block->base)
    free(block->base);

  free(block);

  return true;
}

bool zvm_block_read_next(zvm_block_t *block, uint8_t *out) {
  if (!block || !out)
    return false;

  if (!zvm_block_is_offset(block))
    return false;

  *out = zvm_block_get(block);
  block->offset++;

  return true;
}

bool zvm_block_read_prev(zvm_block_t *block, uint8_t *out) {
  if (!block || !out)
    return false;

  if (!zvm_block_is_valid_offset(block, block->offset - 1))
    return false;

  block->offset--;
  *out = zvm_block_get(block);

  return true;
}

bool zvm_block_rewind(zvm_block_t *block) {
  if (!block)
    return false;

  block->offset = 0;

  return true;
}

bool zvm_block_seek(zvm_block_t *block, const zvm_block_offset_t pos) {
  if (!block)
    return false;
  if (!zvm_block_is_valid_offset(block, pos))
    return false;

  block->offset = pos;
  return true;
}

bool zvm_block_write_at(zvm_block_t *block, const uint8_t val,
                        const zvm_block_offset_t pos) {
  if (!block)
    return false;
  if (!zvm_block_is_valid_offset(block, pos))
    return false;

  block->base[pos] = val;
  return true;
}

bool zvm_block_peek(const zvm_block_t *block, uint8_t *out) {
  if (!block)
    return false;
  if (!zvm_block_is_offset(block))
    return false;

  *out = zvm_block_get(block);

  return true;
}

bool zvm_block_write_bytes(zvm_block_t *block, const uint8_t *data,
                           size_t len) {
  if (!block || !data || !len)
    return false;
  if (!zvm_block_is_offset(block))
    return false;

  if (block->offset + len > zvm_block_size(block))
    return false;

  for (size_t i = 0; i < len; i++) {
    zvm_block_set(block, data[i]);
    block->offset++;
  }

  return true;
}

bool zvm_block_read_bytes(zvm_block_t *block, uint8_t *out, size_t len) {
  if (!block || !out || !len)
    return false;
  if (!zvm_block_is_offset(block))
    return false;

  if (block->offset + len > zvm_block_size(block))
    return false;

  for (size_t i = 0; i < len; i++) {
    out[i] = zvm_block_get(block);
    block->offset++;
  }

  return true;
}

zvm_block_t *zvm_block_clone(const zvm_block_t *src) {
  if (!src)
    return NULL;

  zvm_block_t *block = zvm_block_new(src->element_count, src->element_size);

  if (!block)
    return NULL;

  if (!zvm_block_seek(block, 0)) {
    zvm_block_delete(block);
    return NULL;
  }
  if (!zvm_block_write_bytes(block, src->base, zvm_block_size(src))) {
    zvm_block_delete(block);
    return NULL;
  }

  block->offset = src->offset;

  return block;
}

bool zvm_block_fill(zvm_block_t *block, uint8_t byte) {
  if (!block)
    return false;

  memset(block->base, byte, zvm_block_size(block));
  return true;
}

bool zvm_block_copy_data(zvm_block_t *dst, const zvm_block_t *src) {
  if (!dst || !src)
    return false;

  if (zvm_block_size(dst) != zvm_block_size(src))
    return false;

  memcpy(dst->base, src->base, zvm_block_size(src));

  return true;
}

bool zvm_block_compare(const zvm_block_t *block_a, const zvm_block_t *block_b) {
  if (!block_a || !block_b)
    return false;

  if (zvm_block_size(block_a) != zvm_block_size(block_b))
    return false;

  for (size_t i = 0; i < zvm_block_size(block_a); i++) {
    if (block_a->base[i] != block_b->base[i])
      return false;
  }

  return true;
}

bool zvm_block_resize(zvm_block_t *block, zvm_block_count_t new_count) {
  if (!block)
    return false;

  size_t old_size = zvm_block_size(block);
  size_t new_size = block->element_size * new_count;

  if (!zvm_block_is_valid_size(new_size))
    return false;

  uint8_t *new_base = malloc(new_size);
  if (!new_base)
    return false;

  memset(new_base, 0, new_size);
  memcpy(new_base, block->base, old_size < new_size ? old_size : new_size);

  free(block->base);

  block->base = new_base;
  block->element_count = new_count;
  block->offset = -1;

  return true;
}

int main(void) {
  zvm_block_t *blk = zvm_block_new(8, 1);
  if (!blk) {
    printf("blk allocation failed\n");
    return 1;
  }

  uint8_t data[3] = {10, 20, 30};
  zvm_block_rewind(blk);
  zvm_block_write_bytes(blk, data, 3);
  zvm_block_write_at(blk, 99, 5);

  uint8_t val;
  printf("blk content: ");
  zvm_block_rewind(blk);

  while (zvm_block_read_next(blk, &val))
    printf("%d ", val);

  zvm_block_seek(blk, 2);
  zvm_block_peek(blk, &val);
  printf("\npeek index 2: %d\n", val);

  zvm_block_read_prev(blk, &val);
  printf("read previous: %d\n", val);

  zvm_block_rewind(blk);
  printf("offset after rewind: %d\n", blk->offset);

  zvm_block_t *cloned_blk = zvm_block_clone(blk);
  printf("compare cloned: %d\n", zvm_block_compare(blk, cloned_blk));

  zvm_block_t *copied_blk = zvm_block_new(8, 1);
  zvm_block_copy_data(copied_blk, blk);
  printf("compare copied data: %d\n", zvm_block_compare(blk, copied_blk));

  zvm_block_fill(blk, 0);
  printf("compare after fill: %d\n", zvm_block_compare(blk, cloned_blk));

  zvm_block_resize(cloned_blk, 20);
  printf("resize cloned_blk to: %d\n", zvm_block_size(blk));

  zvm_block_rewind(cloned_blk);
  while (zvm_block_read_next(cloned_blk, &val))
    printf("%d ", val);

  zvm_block_delete(blk);
  zvm_block_delete(cloned_blk);
  zvm_block_delete(copied_blk);
  return 0;
}
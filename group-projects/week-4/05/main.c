#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define HASH_LEN 32

unsigned char seed[HASH_LEN] = {0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
                                0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f,
                                0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
                                0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f};

void xor_hash(unsigned char *data, int file_size, unsigned char *hash) {
  for (int i = 0; i < HASH_LEN; i++)
    hash[i] = seed[i];

  for (int i = 0; i < file_size; i++)
    hash[i % HASH_LEN] ^= data[i];
}

void xor_data(unsigned char *data, int size, unsigned char *hash) {
  for (int i = 0; i < size; i++)
    data[i] ^= hash[i % HASH_LEN];
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("usage:\n");
    printf("%s <file>              \n", argv[0]);
    printf("%s <hash>              \n", argv[0]);
    printf("%s -f <hash> <output>  \n", argv[0]);
    return 1;
  }

  unsigned char hash[HASH_LEN];
  char root_folder[128] = "data";
  char folder_name[128];
  char folder_path[128];
  char full_path[128];
  char gen_file_name[128];
  struct stat st;
  int file_size;

  unsigned char *data;

  if (strchr(argv[1], '.')) { // has extension = file
    FILE *fh = fopen(argv[1], "rb");

    fstat(fileno(fh), &st);
    file_size = st.st_size;

    data = malloc(file_size);
    fread(data, 1, file_size, fh);
    fclose(fh);

    xor_hash(data, file_size, hash);

    printf("hash: ");
    for (int i = 0; i < HASH_LEN; i++)
      printf("%02x", hash[i]);

    snprintf(folder_name, sizeof(folder_name), "%s/%02x", root_folder, hash[0]);
    mkdir(root_folder, 0755);
    mkdir(folder_name, 0755);

    char *ptr = gen_file_name;
    for (int i = 1; i < HASH_LEN; i++)
      ptr += sprintf(ptr, "%02x", hash[i]);

    sprintf(ptr, ".gen");
    printf("\nfile name: %s\n", gen_file_name);

    xor_data(data, file_size, hash);

    sprintf(full_path, "%s/%s", folder_name, gen_file_name);
    FILE *generated_fh = fopen(full_path, "wb");
    fwrite(data, 1, file_size, generated_fh);
    fclose(generated_fh);

    free(data);

  } else {

    int hash_input_index = argc == 4 ? 2 : 1;
    const char *hex_input = argv[hash_input_index];

    for (int i = 0; i < HASH_LEN; i++)
      sscanf(hex_input + 2 * i, "%2hhx", &hash[i]);

    snprintf(folder_name, sizeof(folder_name), "%.2s", hex_input);
    snprintf(folder_path, sizeof(folder_path), "%s/%s", root_folder,
             folder_name);

    if (stat(folder_path, &st) != 0) {
      printf("folder not found - %s\n", folder_path);
      return 1;
    }

    const char *file_name = hex_input + 2;
    snprintf(full_path, sizeof(full_path), "%s/%s/%s.gen", root_folder,
             folder_name, file_name);

    FILE *fh = fopen(full_path, "rb");

    if (!fh) {
      printf("file not found\n");
      return 1;
    }

    fstat(fileno(fh), &st);
    file_size = st.st_size;
    unsigned char *modified_data = malloc(file_size);
    fread(modified_data, 1, file_size, fh);
    fclose(fh);

    xor_data(modified_data, file_size, hash);

    if (argv[1][1] == 'f' && argc == 4) {
      FILE *output_fh = fopen(argv[3], "wb");
      fwrite(modified_data, 1, file_size, output_fh);
      fclose(output_fh);
    } else {
      printf("\n\ndata: \n\n\n");
      for (int i = 0; i < file_size; i++)
        printf("%c ", modified_data[i]);
    }

    free(modified_data);
  }
  return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#define mb_1 1024 * 1024

int main(int argc, char **argv) {

  if (argc < 3) {
    printf("Usage: %s <filename> <number of partitions>\n", argv[0]);
    return 1;
  }

  char *fileName = argv[1];
  int numPartitions = atoi(argv[2]);

  if (numPartitions < 4 || numPartitions > 16) {
    printf("partitions must be between 4 and 16\n");
    return 1;
  }

  FILE *originalFile = fopen(fileName, "rb");
  if (!originalFile)
    return 1;

  struct stat st;
  fstat(fileno(originalFile), &st);
  size_t size = st.st_size;

  if (size > mb_1 || size == 0) {
    printf("file size must be less than 1mb and greater than 0\n");
    return 1;
  }

  if (size < numPartitions) {
    printf("file size must be greater than number of partitions\n");
    return 1;
  }

  int blockSize = (size + numPartitions - 1) / numPartitions;

  printf("Original file size: %d bytes\n", size);
  printf("Block size: %d bytes\n\n", blockSize);

  unsigned char *pBuffer = (unsigned char *)malloc(blockSize);

  for (int i = 0; i < numPartitions; i++) {
    size_t sizeBytesRead = fread(pBuffer, 1, blockSize, originalFile);

    char partitionFilename[100];
    sprintf(partitionFilename, "%s.%d", fileName, i + 1);
    printf("%s: %d bytes\n", partitionFilename, sizeBytesRead);

    FILE *pPartitionFile = fopen(partitionFilename, "wb");
    fwrite(pBuffer, 1, sizeBytesRead, pPartitionFile);

    fclose(pPartitionFile);
  }

  free(pBuffer);
  fclose(originalFile);
  return 0;
}
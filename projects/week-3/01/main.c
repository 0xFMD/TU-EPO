#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define mb_1 1024 * 1024

typedef struct PMD {
  char fileName[128];
  int fileSize;
  int numPartitions;
  int blockSize;
  int lastBlockSize;
} PMD;

size_t getFileSize(FILE *fh) {
  struct stat st;
  fstat(fileno(fh), &st);
  size_t size = st.st_size;

  if (size > mb_1 || size == 0) {
    printf("file size must be less than 1mb and greater than 0\n");
    exit(1);
  }

  return size;
};

PMD splitFile(FILE *originalFile, char *fileName, int numPartitions,
              PMD fileMetaData) {
  if (!originalFile)
    return fileMetaData;
  size_t size = getFileSize(originalFile);

  if (size < numPartitions) {
    printf("file size must be greater than number of partitions\n");
    exit(1);
  }

  int blockSize = (size % numPartitions == 0) ? size / numPartitions
                                              : size / numPartitions + 1;

  strcpy(fileMetaData.fileName, fileName);
  fileMetaData.numPartitions = numPartitions;
  fileMetaData.fileSize = size;
  fileMetaData.blockSize = blockSize;

  printf("Original file size: %d bytes\n", size);
  printf("Block size: %d bytes\n\n", blockSize);

  unsigned char *pBuffer = (unsigned char *)malloc(blockSize);

  for (int i = 0; i < numPartitions; i++) {
    size_t totalBytesRead = fread(pBuffer, 1, blockSize, originalFile);
    if (i == numPartitions - 1)
      fileMetaData.lastBlockSize = totalBytesRead;

    char partitionFilename[128];
    sprintf(partitionFilename, "%s.%d", fileName, i + 1);
    printf("%s: %d bytes\n", partitionFilename, totalBytesRead);

    FILE *pPartitionFile = fopen(partitionFilename, "wb");
    fwrite(pBuffer, 1, totalBytesRead, pPartitionFile);

    fclose(pPartitionFile);
  }

  printf("\n\nfileName:%s\npartition:%d\nfileSize:%d\nblockSize:%"
         "d\nlastBlockSize: %d\n",
         fileMetaData.fileName, fileMetaData.numPartitions,
         fileMetaData.fileSize, fileMetaData.blockSize,
         fileMetaData.lastBlockSize);
  free(pBuffer);

  return fileMetaData;
}

void validateMetaData(PMD fileMetaData, char *fileName, char operationType) {
  char originalFileName[128];
  char partitionFilename[128];

  strncpy(originalFileName, fileName, strlen(fileName) - 4);
  originalFileName[strlen(fileName) - 4] = '\0';

  if (strcmp(fileMetaData.fileName, originalFileName) != 0) {
    printf("file name does not match");
    exit(1);
  }

  printf("\n\nValidating files\n", operationType);

  for (int i = 0; i < fileMetaData.numPartitions; i++) {
    sprintf(partitionFilename, "%s.%d", fileMetaData.fileName, i + 1);

    FILE *fh = fopen(partitionFilename, "rb");
    size_t currentFileSize = getFileSize(fh);

    int blockSize = i == (fileMetaData.numPartitions - 1)
                        ? fileMetaData.lastBlockSize
                        : fileMetaData.blockSize;

    if (blockSize != currentFileSize) {
      printf("error %s - size: %d , expected: %d", partitionFilename,
             currentFileSize, blockSize);
      exit(1);
    }
    printf("\n%s: Done!", partitionFilename);
  }
}

void mergeFiles(PMD fileMetaData, int numRotations) {
  printf("\n\nMerging files with %d rotations\n", numRotations);

  char mergedFileName[128];
  char partitionFilename[128];
  unsigned char *baseOffsets[fileMetaData.numPartitions];

  sprintf(mergedFileName, !numRotations ? "merged.%s" : "rotation_merged.%s",
          fileMetaData.fileName);

  FILE *mergedFile = fopen(mergedFileName, "wb");

  unsigned char *bufferFile = (unsigned char *)malloc(fileMetaData.fileSize);

  for (int i = 0; i < fileMetaData.numPartitions; i++) {
    sprintf(partitionFilename, "%s.%d", fileMetaData.fileName, i + 1);

    FILE *fh = fopen(partitionFilename, "rb");
    int blockSize = (i == fileMetaData.numPartitions - 1)
                        ? fileMetaData.lastBlockSize
                        : fileMetaData.blockSize;

    if (numRotations) {
      baseOffsets[i] =
          bufferFile + (fileMetaData.blockSize *
                        ((i + numRotations) % fileMetaData.numPartitions));
    } else {
      baseOffsets[i] = bufferFile + (fileMetaData.blockSize * i);
    }
    fread(baseOffsets[i], 1, blockSize, fh);
    fclose(fh);
  }

  fwrite(bufferFile, fileMetaData.fileSize, 1, mergedFile);
  free(bufferFile);
  fclose(mergedFile);
}

int main(int argc, char **argv) {

  if (argc < 3) {
    printf("Usage: %s <operation type:(-p | -m)> <filename> <number of "
           "partitions> "
           "[rotation number]\n",
           argv[0]);
    return 1;
  }

  char operationType = argv[1][1];
  char *inputFileName = argv[2];
  PMD fileMetaData;
  FILE *originalFile = fopen(inputFileName, "rb");
  if (!originalFile)
    return 1;

  switch (operationType) {
  case 'p': {
    int numPartitions = atoi(argv[3]);

    if (numPartitions < 4 || numPartitions > 16) {
      printf("partitions must be between 4 and 16\n");
      return 1;
    }

    fileMetaData =
        splitFile(originalFile, inputFileName, numPartitions, fileMetaData);

    char outMetaDataFileName[128];
    sprintf(outMetaDataFileName, "%s.pmd", inputFileName);

    FILE *fh = fopen(outMetaDataFileName, "wb");
    fwrite(&fileMetaData, sizeof(fileMetaData), 1, fh);
    fclose(fh);
    break;
  }
  case 'm': {
    if (strcmp(inputFileName + strlen(inputFileName) - 4, ".pmd") != 0) {
      printf("file must be a .pmd file");
      return 1;
    }

    int numRotations = 0;
    char secondOption = argc > 4 ? argv[3][1] : '\0';

    if (argc == 4 && secondOption != 'r') {
      printf("Usage: %s -m <filename> [-r <rotation number>]\n", argv[0]);
      return 1;
    }

    if (secondOption == 'r')
      numRotations = atoi(argv[4]);

    PMD fileMetaData;
    fread(&fileMetaData, sizeof(PMD), 1, originalFile);

    printf("\n\nReading file name: %s\nsource "
           "file: %s\npartition: %d\nfileSize: %d\nblockSize:%"
           "d\nlastBlockSize: %d\n",
           inputFileName, fileMetaData.fileName, fileMetaData.numPartitions,
           fileMetaData.fileSize, fileMetaData.blockSize,
           fileMetaData.lastBlockSize);

    validateMetaData(fileMetaData, inputFileName, secondOption);
    mergeFiles(fileMetaData, numRotations);

    break;
  }
  default: {
    printf("invalid operation");
    return 1;
  }
  }
  fclose(originalFile);
  return 0;
}
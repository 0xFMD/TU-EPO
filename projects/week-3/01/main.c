#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>

#define mb_1 1024 * 1024

typedef struct PMD {
  char fileName[128];
  int fileSize;
  int numPartitions;
  int numOfFakePartition;
  int blockSize;
  int lastBlockSize;
  unsigned short fakeFileFlags; // single bit = 1 fake else 0 real
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

void generateRandomBlock(unsigned char *buffer, int size) {
  for (int i = 0; i < size; i++)
    buffer[i] = rand() % 256;
}

PMD splitFile(FILE *originalFile, char *fileName, int numPartitions,
              int numOfFakePartition, PMD fileMetaData) {
  if (!originalFile)
    return fileMetaData;
  size_t size = getFileSize(originalFile);

  if (size < numPartitions) {
    printf("file size must be greater than number of partitions\n");
    exit(1);
  }

  int blockSize = (size % numPartitions == 0) ? size / numPartitions
                                              : size / numPartitions + 1;
  int totalPartition = numPartitions + numOfFakePartition;

  strcpy(fileMetaData.fileName, fileName);
  fileMetaData.numPartitions = numPartitions;
  fileMetaData.numOfFakePartition =
      numOfFakePartition > 0 ? numOfFakePartition : 0;
  fileMetaData.fileSize = size;
  fileMetaData.blockSize = blockSize;

  printf("Original file size: %d bytes\n", size);
  printf("Block size: %d bytes\n\n", blockSize);

  unsigned char *pBuffer = (unsigned char *)malloc(blockSize);
  for (int i = 0; i < totalPartition; i++) {
    int isFake = (fileMetaData.fakeFileFlags & (1 << i)) != 0;

    size_t remainingBytesToWrite;

    if (isFake) {
      generateRandomBlock(pBuffer, blockSize);
      remainingBytesToWrite = blockSize;
    } else {
      remainingBytesToWrite = fread(pBuffer, 1, blockSize, originalFile);
      fileMetaData.lastBlockSize = remainingBytesToWrite;
    }

    char name[128];
    sprintf(name, "%s.%d", fileName, i + 1);
    printf("%s - %d bytes%s\n", name, remainingBytesToWrite,
           isFake ? " fake" : "");

    FILE *out = fopen(name, "wb");
    fwrite(pBuffer, 1, remainingBytesToWrite, out);
    fclose(out);
  }

  printf("\nmetadata .pmd:\nfileName:%s\npartition:%d\nfileSize:%d\nblockSize:%"
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

  // remove .pmd
  strncpy(originalFileName, fileName, strlen(fileName) - 4);
  originalFileName[strlen(fileName) - 4] = '\0';

  if (strcmp(fileMetaData.fileName, originalFileName) != 0) {
    printf("file name does not match");
    exit(1);
  }

  printf("\n\nValidating files\n");

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
    fclose(fh);
  }
}

void mergeFiles(PMD fileMetaData, int numRotations, int isMergeWithFakeFiles,
                char *outFileName) {
  printf("\n\nMerging files with %d rotations\n", numRotations);

  char mergedFileName[128];
  char outputFileName[128];
  char partitionFilename[128];

  sprintf(mergedFileName, !numRotations ? "merged.%s" : "rotation_merged.%s",
          fileMetaData.fileName);

  sprintf(outputFileName, "%s.out", outFileName ? outFileName : "");

  FILE *mergedFile = fopen(mergedFileName, "wb");
  FILE *outputFile = isMergeWithFakeFiles ? fopen(outputFileName, "wb") : NULL;

  int totalPartition =
      fileMetaData.numPartitions + fileMetaData.numOfFakePartition;

  /* to map blocks with their sizes */
  unsigned char *blockPtrs[totalPartition];
  int blockSizes[totalPartition];
  int bufferSize = fileMetaData.blockSize * totalPartition;

  unsigned char *bufferFile = (unsigned char *)malloc(bufferSize);

  for (int i = 0; i < totalPartition; i++) {

    sprintf(partitionFilename, "%s.%d", fileMetaData.fileName, i + 1);

    FILE *fh = fopen(partitionFilename, "rb");
    blockSizes[i] = (i == fileMetaData.numPartitions - 1)
                        ? fileMetaData.lastBlockSize
                        : fileMetaData.blockSize;

    blockPtrs[i] = bufferFile + (fileMetaData.blockSize * i);

    fread(blockPtrs[i], 1, blockSizes[i], fh);

    fclose(fh);
  }

  // merge with fake files
  if (isMergeWithFakeFiles) {
    for (int i = 0; i < totalPartition; i++) {
      int blockIndex = (i + numRotations) % totalPartition;
      fwrite(blockPtrs[blockIndex], 1, blockSizes[blockIndex], outputFile);
    }
  } else {
    // real files only
    for (int i = 0; i < totalPartition; i++) {
      int blockIndex = (i + numRotations) % totalPartition;
      int isFake = (fileMetaData.fakeFileFlags & (1 << i)) != 0;

      if (isFake)
        continue;

      fwrite(blockPtrs[blockIndex], 1, blockSizes[blockIndex], mergedFile);
    }
  }

  free(bufferFile);
  fclose(mergedFile);
  if (isMergeWithFakeFiles)
    fclose(outputFile);
}
int main(int argc, char **argv) {

  srand(time(NULL));
  if (argc < 3) {
    printf("Usage: %s <operation type:(-p | -m)> <filename> <number of "
           "partitions> "
           "[rotation number]\n",
           argv[0]);
    return 1;
  }

  char operationType = argv[1][1];
  char *inputFileName = argv[2];
  char *outFileName = NULL;

  PMD fileMetaData;
  FILE *originalFile = fopen(inputFileName, "rb");
  if (!originalFile)
    return 1;

  switch (operationType) {
  case 'p': {
    char RandomOption = argc > 4 ? argv[4][1] : '\0';

    if (argc != 6 && argc != 4) {
      printf("Usage: %s -p <filename> <number of partitions>\n", argv[0]);
      return 1;
    }

    int numPartitions = atoi(argv[3]);

    if (numPartitions < 2 || numPartitions > 8) {
      printf("partitions must be between 2 and 8\n");
      return 1;
    }

    int numOfFakeFile = 0;
    fileMetaData.fakeFileFlags = 0;

    if (RandomOption == 'R') {
      if (argc != 6) {
        printf("Usage: %s -p <filename> <number of partitions> -R <random "
               "number>\n",
               argv[0]);
        return 1;
      }

      numOfFakeFile = atoi(argv[5]);
      int totalPartitions = numPartitions + numOfFakeFile;

      int currentfakeFilePos = 0;
      while (currentfakeFilePos < numOfFakeFile) {
        int pos = rand() % totalPartitions;
        int bitMask = 1 << pos;
        if (!(fileMetaData.fakeFileFlags & bitMask)) {
          fileMetaData.fakeFileFlags = fileMetaData.fakeFileFlags | bitMask;
          currentfakeFilePos++;
        }
      }
    }

    fileMetaData = splitFile(originalFile, inputFileName, numPartitions,
                             numOfFakeFile, fileMetaData);

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
    char RandomOption = argc > 4 ? argv[3][1] : '\0';

    if ((argc == 4 && (secondOption != 'r' || RandomOption != 'R'))) {
      printf("Usage: %s -m <filename> [-r <rotation number>]\n", argv[0]);
      return 1;
    }

    if (secondOption == 'r')
      numRotations = atoi(argv[4]);

    int isMergeWithFakeFiles = RandomOption == 'R';

    if (isMergeWithFakeFiles) {
      if (argc != 5) {
        printf("Usage: %s -m <filename> [-r <rotation number>] -R  <output "
               "file name>\n",
               argv[0]);
        return 1;
      }
      outFileName = argv[4];
    }

    PMD fileMetaData;
    fread(&fileMetaData, sizeof(PMD), 1, originalFile);

    printf("\n\nReading file name: %s\nsource "
           "file: %s\npartition: %d\nfakePartitions: %d\nfileSize: "
           "%d\nblockSize:%"
           "d\nlastBlockSize: %d\n",
           inputFileName, fileMetaData.fileName, fileMetaData.numPartitions,
           fileMetaData.numOfFakePartition, fileMetaData.fileSize,
           fileMetaData.blockSize, fileMetaData.lastBlockSize);

    if (!isMergeWithFakeFiles)
      validateMetaData(fileMetaData, inputFileName, secondOption);

    mergeFiles(fileMetaData, numRotations, isMergeWithFakeFiles, outFileName);

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
/* References:
https://en.wikipedia.org/wiki/Extended_Display_Identification_Data
*/

#include <stdio.h>
#include <stdlib.h>

#define EDID_SIGNATURE 0x00FFFFFFFFFFFF00
#define LAYOUT_SIZE 128

#define BASE_MANUFACTURE_YEAR 1990
#define BASE_GAMMA 100
#define BASE_ASPECT_RATIO 99

#define DEREF(type, data) *((type *)(data))

#define MASK_LETTER_TO_ASCII 0x40
#define MASK_VENDOR_LETTER1 0x7C00
#define MASK_VENDOR_LETTER2 0x03E0
#define MASK_VENDOR_LETTER3 0x001F
#define MASK_VIDEO_INPUT_BIT_DEPTH 0x70
#define MASK_VIDEO_INPUT_INTERFACE 0x0F
#define MASK_VIDEO_INPUT_IS_DIGITAL 0x80

#define IS_GAMMA_DEFINED_BY_DIEXT 0xFF
#define IS_MODEL_YEAR_ONLY 0XFF

typedef unsigned char byte;

typedef struct MonitorInfo {
  // header
  unsigned long signature;
  unsigned int vendor_id;
  unsigned int serial_number;
  unsigned short product_code;

  byte origin_week;
  byte origin_year;
  byte EDID_version;
  byte EDID_revision;
  byte video_input;
  byte h_size_cm;
  byte v_size_cm;
  byte raw_gamma;
} MonitorInfo;

const char *BIT_DEPTH[] = {
    "undefined",         "6 bits per color",  "8 bits per color",
    "10 bits per color", "12 bits per color", "14 bits per color",
    "16 bits per color", "reserved",
};

const char *VIDEO_INTERFACE[] = {"undefined", "DVI",  "HDMIa",
                                 "HDMIb",     "MDDI", "DisplayPort"};

void unpackData(MonitorInfo *pMonitorInfo, char *filename);
unsigned int decodeLetters(unsigned short value);
void printMonitorInfo(MonitorInfo *pMonitorInfo);

int main(int argc, char **argv) {
  if (argc != 2) {
    printf("Usage: %s <EDID_file>\n", argv[0]);
    return 1;
  }

  MonitorInfo *pMonitorInfo = (MonitorInfo *)malloc(sizeof(MonitorInfo));

  unpackData(pMonitorInfo, argv[1]);
  printMonitorInfo(pMonitorInfo);

  free(pMonitorInfo);

  return 0;
}

void unpackData(MonitorInfo *pMonitorInfo, char *filename) {
  FILE *fh = fopen(filename, "rb");
  if (!fh)
    return;

  void *pReadData = malloc(LAYOUT_SIZE);

  if (!pReadData) {
    fclose(fh);
    return;
  }

  fread(pReadData, LAYOUT_SIZE, 1, fh);
  fclose(fh);

  pMonitorInfo->signature = DEREF(unsigned long, pReadData);
  pMonitorInfo->vendor_id = decodeLetters(DEREF(
      unsigned short, pReadData + 8)); // unpack 3 letters from 2 bytes and map
                                       // them to their ASCII values
  pMonitorInfo->product_code = DEREF(unsigned short, pReadData + 10);
  pMonitorInfo->serial_number = DEREF(unsigned int, pReadData + 12);
  pMonitorInfo->origin_week = DEREF(byte, pReadData + 16);
  pMonitorInfo->origin_year = DEREF(byte, pReadData + 17);
  pMonitorInfo->EDID_version = DEREF(byte, pReadData + 18);
  pMonitorInfo->EDID_revision = DEREF(byte, pReadData + 19);
  pMonitorInfo->video_input = DEREF(byte, pReadData + 20);
  pMonitorInfo->h_size_cm = DEREF(byte, pReadData + 21);
  pMonitorInfo->v_size_cm = DEREF(byte, pReadData + 22);
  pMonitorInfo->raw_gamma = DEREF(byte, pReadData + 23);

  free(pReadData);
}

void printMonitorInfo(MonitorInfo *pMonitorInfo) {
  if (pMonitorInfo->signature != EDID_SIGNATURE) {
    printf("error: not EDID binary file\n");
    return;
  }

  printf("File signature: 0x%.16lX\n", pMonitorInfo->signature);
  printf("EDID version: %d.%d\n", pMonitorInfo->EDID_version,
         pMonitorInfo->EDID_revision);
  printf("Vendor: %s\n", (char *)&pMonitorInfo->vendor_id);
  printf("Product code: %d\n", pMonitorInfo->product_code);
  printf("Serial Number: 0x%.8X\n", pMonitorInfo->serial_number);

  if (pMonitorInfo->origin_week == IS_MODEL_YEAR_ONLY)
    printf("Model year: %d\n",
           pMonitorInfo->origin_year + BASE_MANUFACTURE_YEAR);
  else
    printf("Made in: week %d of %d\n", pMonitorInfo->origin_week,
           pMonitorInfo->origin_year + BASE_MANUFACTURE_YEAR);

  if (pMonitorInfo->video_input & MASK_VIDEO_INPUT_IS_DIGITAL) {
    printf("Input type:     Digital\n"); // if MSB is 1, it's digital
    printf("Interface: %s\n", VIDEO_INTERFACE[pMonitorInfo->video_input &
                                              MASK_VIDEO_INPUT_INTERFACE]);
    printf("Bit depth: %s\n",
           BIT_DEPTH[(pMonitorInfo->video_input & MASK_VIDEO_INPUT_BIT_DEPTH) >>
                     4]);
  }

  // rawSize = (AR×100) − 99
  // AR = (rawSize+99) / 100

  // scale

  if (pMonitorInfo->h_size_cm == 0 && pMonitorInfo->v_size_cm == 0)
    printf("undefined\n");
  else if (pMonitorInfo->v_size_cm == 0)
    printf("Aspect ratio: %.2f\n",
           (pMonitorInfo->h_size_cm + BASE_ASPECT_RATIO) / 100.0f);
  // datavalue = (100/AR) − 99
  // AR = 100 / (datavalue + 99)
  else if (pMonitorInfo->h_size_cm == 0)
    printf("Aspect ratio: %.2f\n",
           100.0f / (pMonitorInfo->v_size_cm + BASE_ASPECT_RATIO));
  else
    printf("Size: %d x %d cm\n", pMonitorInfo->h_size_cm,
           pMonitorInfo->v_size_cm);

  // rawGamma = (gamma×100) − 100
  // gamma =  (rawGamma+100) / 100

  if (pMonitorInfo->raw_gamma == IS_GAMMA_DEFINED_BY_DIEXT)
    printf("Gamma is defined by DI-EXT block\n");
  else
    printf("Gamma: %.2f\n", (pMonitorInfo->raw_gamma + BASE_GAMMA) / 100.0f);
}

unsigned int decodeLetters(unsigned short value) {

  byte hb = value >> 8;
  byte lb = value;

  // convert to big endian
  value = (lb << 8) | hb;

  // each 5-bit is a letter
  //   L1    L2     L3
  // 0|000 00|00 000|0 0000
  byte l1 = ((value & MASK_VENDOR_LETTER1) >> 10) ^
            MASK_LETTER_TO_ASCII; // 0x40 is to make the counting starting from
                                  // 64, which is mapping 1 to 'A' on ASCII

  byte l2 = ((value & MASK_VENDOR_LETTER2) >> 5) ^ MASK_LETTER_TO_ASCII;
  byte l3 = (value & MASK_VENDOR_LETTER3) ^ MASK_LETTER_TO_ASCII;

  return (l3 << 16) | (l2 << 8) | l1;
}
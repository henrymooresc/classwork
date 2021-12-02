#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>

// Various sizes in bytes
#define SECTOR_SIZE 512
#define FAT_SIZE 4608
#define ROOT_SIZE 7168
#define DATA_SIZE 1457664
#define MAX_BYTES 1474560
#define ENTRY_SIZE 32
#define MAX_PATH 256
#define MAX_NAME 8
#define MAX_EXT 3
#define MAX_ENTRIES 16

// Sector starting bytes
#define DATA_START 16896
#define FAT2_START 5120
#define ROOT_START 9728

// Num of entries possible in FAT
#define FAT_ENTRIES 3072

// Common FAT entries
#define FREE 0x000
#define USE_MIN 0x002
#define USE_MAX 0xfef
#define RESERVED_MIN 0xff0
#define RESERVED_MAX 0xff6
#define BAD 0xff7

// Struct used to store two 12 bit FAT entries
typedef struct FatEntries{
    uint16_t one;
    uint16_t two;
} FatEntries;

// Struct used to store file entry information
typedef struct File {
    unsigned char *name;
    unsigned char *ext;
    unsigned char *attributes;
    unsigned char *reserved;
    unsigned char *creation_time;
    unsigned char *creation_date;
    unsigned char *last_access;
    unsigned char *last_mod_time;
    unsigned char *last_mod_date;
    unsigned char *first_cluster; 
    unsigned char *size;
    char *path;
    int path_len;
    int not_deleted;
} File;

FatEntries calcEntries(unsigned char *one, unsigned char *two, unsigned char *three);
File *fileCreate(unsigned char *raw, char *path);
void fileExtract(File *file);
void filePrint(File *file);
uint32_t convertHex(unsigned char *hex, int bytes_num);
void parseRoot();
void parseSubDir(File *file);
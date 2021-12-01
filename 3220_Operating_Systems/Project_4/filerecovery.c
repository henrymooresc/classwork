#include "filerecovery.h"

/* Global Variables */
char boot[SECTOR_SIZE];
uint16_t *fat1[FAT_ENTRIES];
uint16_t *fat2[FAT_ENTRIES];
unsigned char root[ROOT_SIZE];
unsigned char data[DATA_SIZE];

char *dest_path;
int file_count = 0;

//Purpose: Calculates the entry values from three bytes (specific to FAT12)
FatEntries calcEntries(unsigned char *one, unsigned char *two, unsigned char *three)
{
	FatEntries e;
	// Split the second byte up
	uint16_t temp = (uint16_t) *two;
	uint16_t nib_1 = (temp >> 4); //Dest: Second
	uint16_t nib_2 = ((temp & 0x0f) << 8); //Dest: First

	//Place the nibs into their respective entries
	uint16_t first = (uint16_t) *one;
	e.one = (nib_2 | first);
	uint16_t third = (uint16_t) *three;
	e.two = (nib_1 | (third << 4));

	return e;
}

File *fileCreate(unsigned char *raw, char *path)
{
	File *file = malloc(sizeof(File));
	int byte_num = 0;

	//Check if the file is deleted
	if(0xE5 != raw[byte_num])
	{
		file->not_deleted = 1;
	}
	else
	{
		file->not_deleted = 0;
	}

	//Read in name-------------------------------------------------
	file->name = malloc(8 * sizeof(char));
	for(int i = 0; i < 8; i++)
	{
		file->name[i] = raw[byte_num+i];
	}
	byte_num += 8;

	//Read in extension-------------------------------------------------
	file->ext = malloc(3 * sizeof(char));
	for(int i = 0; i < 3; i++)
	{
		file->ext[i] = raw[byte_num+i];
	}
	byte_num += 3;

	//Read in attributes------------------------------------------------
	file->attributes = malloc(sizeof(char));
	file->attributes[0] = raw[byte_num];
	byte_num++;

	//Read in reserved--------------------------------------------------
	file->reserved = malloc(2 * sizeof(char));
	for(int i = 0; i < 2; i++)
	{
		file->reserved[i] = raw[byte_num+i];
	}
	byte_num += 2;

	//Read in creation_time-----------------------------------------------
	file->creation_time = malloc(2 * sizeof(char));
	for(int i = 0; i < 2; i++)
	{
		file->creation_time[i] = raw[byte_num+i];
	}
	byte_num += 2;

	//Read in creation_date-----------------------------------------------
	file->creation_date = malloc(2 * sizeof(char));
	for(int i = 0; i < 2; i++)
	{
		file->creation_date[i] = raw[byte_num+i];
	}
	byte_num += 2;

	//Read in last_access info------------------------------------------
	file->last_access = malloc(2 * sizeof(char));
	for(int i = 0; i < 2; i++)
	{
		file->last_access[i] = raw[byte_num+i];
	}
	byte_num += 4;

	//Read in lastModified Time------------------------------------------
	file->last_mod_time = malloc(2 * sizeof(char));
	for(int i = 0; i < 2; i++)
	{
		file->last_mod_time[i] = raw[byte_num+i];
	}
	byte_num += 2;

	//Read in lastModified Date-------------------------------------------
	file->last_mod_date = malloc(2 * sizeof(char));
	for(int i = 0; i < 2; i++)
	{
		file->last_mod_date[i] = raw[byte_num+i];
	}
	byte_num += 2;

	//Read in first logical cluster---------------------------------------
	file->first_cluster = malloc(2 * sizeof(char));
	for(int i = 0; i < 2; i++)
	{
		file->first_cluster[i] = raw[byte_num+i];
	}
	byte_num += 2;

	//Read in file size---------------------------------------------------
	file->size = malloc(4 * sizeof(char));
	for(int i = 0; i < 4; i++)
	{
		file->size[i] = raw[byte_num+i];
	}
	byte_num += 4;

	file->path_len = strlen(path);

	file->path = (char *) malloc(file->path_len);
	for(int i = 0; i < file->path_len; i++)
	{
		file->path[i] = path[i];
	}

	return file;
}

//Purpose: Finds the file's contents and saves it to the user's specified directory
void fileExtract(File *file)
{
	// Declare variables for storing name information, indices, and flags
	char file_name[MAX_PATH];
	char temp[8];
	char extension[4];
	unsigned int last_cluster = 0;  // Flag for indicating if we are in the last sector of the file, set to 1 for yes
	unsigned int write_count = 0;

	// Give path for file_name, and extract extension
	strcpy(file_name, dest_path);
	sprintf(temp, "%d.", file_count);
	memcpy(&file_name[strlen(dest_path)], "/file", 5);
	memcpy(&file_name[strlen(dest_path)+5], temp, strlen(temp));

	int i = 0;
	while(file->ext[i] != '.' && i < 3)
	{
		extension[i] = file->ext[i];
		i++;
	}
	extension[i] = '\0';
	memcpy(&file_name[strlen(dest_path)+strlen(temp)+5], extension, 4);

	// Open a new file for writing with the extracted file_name
	FILE *out_file = fopen(file_name, "wb");

	if (out_file == NULL)
	{
		perror("Output file could not be opened");
	}

	// Move to proper index of data array to begin writing bytes to output file and extract to buffer
	unsigned int sector_num = (u_int16_t)convertHex(file->first_cluster, 2);

	if ((sector_num >= 0xFF0 && sector_num <= 0xFF7) || sector_num == 0)
	{
		perror("Issue with first cluster");
	}

	unsigned int file_size = (u_int16_t)convertHex(file->size, 4);
	unsigned int byte_num = (sector_num - 2) * SECTOR_SIZE;

	do
	{
		i = 0;
		while (i < SECTOR_SIZE && write_count < file_size)
		{
			fprintf(out_file, "%c", data[byte_num + i]);
			i++; write_count++;
		}

		// Find the next sector from the FAT
		if (*fat1[sector_num] >= 0xFF0 || *fat1[sector_num] == 0x0)
		{
			last_cluster = 1;
		}

		sector_num = *fat1[sector_num];
		byte_num = (sector_num - 2) * SECTOR_SIZE;

	} while (last_cluster == 0);
	
	fclose(out_file);
	file_count++;
}

/* Function: filePrint     */
//Purpose: Print formatted file information to stdout
void filePrint(File *file)
{
	printf("FILE\t");

	if(file->not_deleted)
	{
		printf("NORMAL\t");
	}
	else
	{
		printf("DELETED\t");
	}

	for(int i = 0; i < file->path_len; i++)
	{
		if(file->path[i] != ' ' && file->path[i] != '.')
		{
			printf("%c", file->path[i]);
		}
	}

	int name_index = 0;

	if(!file->not_deleted)
	{
		printf("_");
		name_index = 1;
	}

	while(file->name[name_index] != ' ' && file->name[name_index] != '.' && name_index < MAX_NAME)
	{
		printf("%c", file->name[name_index]);
		name_index++;
	}
	
	printf(".");

	for(int i = 0; i < MAX_EXT; i++)
	{
		printf("%c", file->ext[i]);
	}

	printf("\t%d\n", convertHex(file->size, 4));
}

//Purpose: Converts a unsigned char *hex in little endian to uint32_t in big endian
uint32_t convertHex(unsigned char *hex, int bytes_num)
{
	uint32_t result = 0;

	for(int i = 0; i < bytes_num; i++)
	{
		uint32_t temp = ((uint32_t) hex[i]) << (unsigned) (8 * i);
		result = result | temp;
	}

	return result;
}

void parseData(unsigned char *disk)
{
	//Read the contents of the disk into the appropriate data structures
	unsigned char fat1_raw[FAT_SIZE];
	unsigned char fat2_raw[FAT_SIZE];

	for(int count = 0; count < MAX_BYTES; count++)
	{
		unsigned char curr_byte = disk[count];

		if(count < SECTOR_SIZE)
		{
			boot[count] = curr_byte;
		}
		else if(count < FAT2_START)
		{
			fat1_raw[count-SECTOR_SIZE] = curr_byte;
		}
		else if(count < ROOT_START)
		{
			fat2_raw[count-FAT2_START] = curr_byte;
		}
		else if(count < DATA_START)
		{
			root[count-ROOT_START] = curr_byte;
		}
		else
		{
			data[count-DATA_START] = curr_byte;
		}
	}

	//Format the FATs correctly
	unsigned char *one_f1 = malloc(sizeof(unsigned char));
	unsigned char *two_f1 = malloc(sizeof(unsigned char));
	unsigned char *three_f1 = malloc(sizeof(unsigned char));
	unsigned char *one_f2 = malloc(sizeof(unsigned char));
	unsigned char *two_f2 = malloc(sizeof(unsigned char));
	unsigned char *three_f2 = malloc(sizeof(unsigned char));

	int entry_num = 0;
	for(int i = 0; i < FAT_SIZE; i += 3)
	{
		*one_f1 = fat1_raw[i];
		*two_f1 = fat1_raw[i+1];
		*three_f1 = fat1_raw[i+2];

		*one_f2 = fat2_raw[i];
		*two_f2 = fat2_raw[i+1];
		*three_f2 = fat2_raw[i+2];

		FatEntries e_f1 = calcEntries(one_f1, two_f1, three_f1);
		fat1[entry_num] = malloc(sizeof(uint16_t));
		fat1[entry_num+1] = malloc(sizeof(uint16_t));
		*fat1[entry_num] = e_f1.one;
		*fat1[entry_num+1] = e_f1.two;

		FatEntries e_f2 = calcEntries(one_f2, two_f2, three_f2);
		fat2[entry_num] = malloc(sizeof(uint16_t));
		fat2[entry_num+1] = malloc(sizeof(uint16_t));
		*fat2[entry_num] = e_f2.one;
		*fat2[entry_num+1] = e_f2.two;

		entry_num += 2;
	}
}

/* Function: parseRoot  */
void parseRoot()
{
	int count = 0;
	while((count + ENTRY_SIZE) <= ROOT_SIZE)
	{
		if(root[count] != FREE)
		{
			unsigned char *raw_bytes = malloc(ENTRY_SIZE);

			for(int i = 0; i <  ENTRY_SIZE; i++)
			{
				raw_bytes[i] = root[count];
				count++;
			}

			char *file_path = malloc(sizeof(char));
			file_path = "/";

			File *file = fileCreate(raw_bytes, file_path);

			//If file isn't a subdirectory, print and explore it
			//else explore subdirectory
			if((convertHex(file->size, 4)) != 0)
			{
				filePrint(file);
				fileExtract(file);
			}
			else
			{
				parseSubDir(file);
			}
		}
		else
		{
			count += 32;
		}
	}
}

//Purpose: Explores a subdirectory for files contained within
void parseSubDir(File *file)
{
	uint16_t clusters[MAX_ENTRIES];
	clusters[0] = (uint16_t)(convertHex(file->first_cluster, 2));

	int cluster_num = 0;
	while(cluster_num < MAX_ENTRIES)
	{
		if(*(fat1[clusters[cluster_num]]) == 0x00)
		{
			break;
		}
		else if(*(fat1[clusters[cluster_num]]) >= LAST_MIN || *(fat1[clusters[cluster_num]]) <= LAST_MAX)
		{
			cluster_num++;
			break;
		}
		else if(*fat1[clusters[cluster_num]] >= USE_MIN && *fat1[clusters[cluster_num]] <= USE_MAX)
		{
			clusters[cluster_num+1] = *(fat1[clusters[cluster_num]]);
			cluster_num++;
		}
	}

	int name_len = 0;
	while(name_len < MAX_NAME)
	{
		if(file->name[name_len] == ' ')
		{
			break;
		}
		name_len++;
	}

	char dir_name[name_len];
	for(int i = 0; i < name_len; i++)
	{
		dir_name[i] = file->name[i];
	}
	dir_name[name_len] = '\0';

	char curr_path[file->path_len];
	for(int i = 0; i < file->path_len; i++)
	{
		curr_path[i] = file->path[i];
	}

	//Get new file path
	char *new_path = malloc(file->path_len + name_len + 1);
	strcpy(new_path, curr_path);
	strcat(new_path, dir_name);
	strcat(new_path, "/");

	for(int count = 0; count < cluster_num; count++)
	{
		int sector_start = ((clusters[count]-2)* SECTOR_SIZE);
		int entry_start = 64;
		
		while((entry_start + ENTRY_SIZE) <= SECTOR_SIZE)
		{
			if(data[sector_start + entry_start] != FREE)
			{
				unsigned char *raw_bytes = malloc(ENTRY_SIZE);

				for(int x = 0; x < ENTRY_SIZE; x++)
				{
					raw_bytes[x] = data[sector_start + entry_start];
					entry_start++;
				}

				File *new_file = fileCreate(raw_bytes, new_path);
				
				free(raw_bytes);

				if((convertHex(new_file->size, 4)) != 0)
				{
					filePrint(new_file);
					fileExtract(new_file);
				}
				else
				{
					parseSubDir(new_file);
				}
			}
			else
			{
				entry_start += 32;
			}
		}
	}
}

int main(int argc, char *argv[])
{
	if(argc != 3)
	{
		perror("Incorrect number of inputs given");
	}

	//Program Input Parameters
	char *image_path = argv[1];			//Path of disk image
	dest_path = malloc(strlen(argv[2])); //Directory path for file output
	dest_path = argv[2];
	mkdir(dest_path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

	//Get Data
	unsigned char *data;
	unsigned int length;
	struct stat stat_buffer;

	int fp = open(image_path, O_RDONLY);
	fstat(fp, &stat_buffer);

	length = (unsigned int) stat_buffer.st_size;
	data = (uint8_t *) mmap(0, length, PROT_READ, MAP_FILE|MAP_PRIVATE, fp, 0);

	parseData(data);

	parseRoot();

	return 0;
}
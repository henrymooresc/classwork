#include "filerecovery.h"

// Globals for data storage and counting
char *dest_path;
int file_count = 0;
char boot[SECTOR_SIZE];
uint16_t *fat1[FAT_ENTRIES];
uint16_t *fat2[FAT_ENTRIES];
unsigned char root[ROOT_SIZE];
unsigned char data[DATA_SIZE];

// Calculates the FAT entry based on given three bytes
FatEntries calcEntries(unsigned char *one, unsigned char *two, unsigned char *three)
{
	// Divide the second byte into two nibbles
	uint16_t nib_1 = (((uint16_t) *two) & 0x0f) << 8;
	uint16_t nib_2 = ((uint16_t) *two) >> 4;

	// Combine nibbles with first and third byte to get entries
	FatEntries e;
	e.one = nib_1 | ((uint16_t) *one);
	e.two = nib_2 | (((uint16_t) *three) << 4);

	return e;
}

// Assigns file data to File struct
File *fileCreate(unsigned char *raw, char *path)
{
	int byte_num = 0;
	File *file = malloc(sizeof(File));

	// Check if the file was deleted or not
	if(0xE5 != raw[byte_num])
	{
		file->not_deleted = 1;
	}
	else
	{
		file->not_deleted = 0;
	}

	// Assign first 8 bytes to name
	file->name = malloc(8);
	for(int i = 0; i < 8; i++)
	{
		file->name[i] = raw[byte_num+i];
	}
	byte_num += 8;

	// Assign next 3 bytes to ext
	file->ext = malloc(3);
	for(int i = 0; i < 3; i++)
	{
		file->ext[i] = raw[byte_num+i];
	}
	byte_num += 3;

	// Assign next byte to attributes
	file->attributes = malloc(1);
	file->attributes[0] = raw[byte_num];
	byte_num++;

	// Assign next 2 bytes to reserved
	file->reserved = malloc(2);
	for(int i = 0; i < 2; i++)
	{
		file->reserved[i] = raw[byte_num+i];
	}
	byte_num += 2;

	// Assign next 2 bytes to creation_time
	file->creation_time = malloc(2);
	for(int i = 0; i < 2; i++)
	{
		file->creation_time[i] = raw[byte_num+i];
	}
	byte_num += 2;

	// Assign next 2 bytes to creation_date
	file->creation_date = malloc(2);
	for(int i = 0; i < 2; i++)
	{
		file->creation_date[i] = raw[byte_num+i];
	}
	byte_num += 2;

	// Assign next 2 bytes to last_access
	file->last_access = malloc(2);
	for(int i = 0; i < 2; i++)
	{
		file->last_access[i] = raw[byte_num+i];
	}
	byte_num += 4;

	// Assign next 2 bytes to last_mod_time
	file->last_mod_time = malloc(2);
	for(int i = 0; i < 2; i++)
	{
		file->last_mod_time[i] = raw[byte_num+i];
	}
	byte_num += 2;

	// Assign next 2 bytes to last_mod_date
	file->last_mod_date = malloc(2);
	for(int i = 0; i < 2; i++)
	{
		file->last_mod_date[i] = raw[byte_num+i];
	}
	byte_num += 2;

	// Assign next 2 bytes to first_cluster
	file->first_cluster = malloc(2);
	for(int i = 0; i < 2; i++)
	{
		file->first_cluster[i] = raw[byte_num+i];
	}
	byte_num += 2;

	// Assign next 4 bytes to size
	file->size = malloc(4);
	for(int i = 0; i < 4; i++)
	{
		file->size[i] = raw[byte_num+i];
	}
	byte_num += 4;

	// Assign length of given path to path_len
	file->path_len = strlen(path);

	// Assign given path to File based on calculated length
	file->path = malloc(file->path_len);
	for(int i = 0; i < file->path_len; i++)
	{
		file->path[i] = path[i];
	}

	return file;
}

// Extracts the file data and saves it to given destination
void fileExtract(File *file)
{
	// Variables for building output file path
	char file_name[strlen(dest_path) + 10];
	char temp[MAX_NAME];
	char ext[MAX_EXT + 1];

	// Build output file path from given destination, file name, and extension
	strcpy(file_name, dest_path);
	sprintf(temp, "/file%d.", file_count);
	strcat(file_name, temp);

	for (int i = 0; i < 3; i++)
	{
		ext[i] = file->ext[i];
	}
	ext[4] = '\0';
	strcat(file_name, ext);

	// Create output file
	FILE *out_file = fopen(file_name, "wb");

	if (out_file == NULL)
	{
		perror("Output file could not be opened");
	}

	// Get data sector for the first logical cluster of the file
	unsigned int sector_num = (u_int16_t)convertHex(file->first_cluster, 2);

	if ((sector_num >= 0xFF0 && sector_num <= 0xFF7) || sector_num == 0)
	{
		perror("Issue with first cluster");
	}

	// Get the file size and calculate starting byte number
	unsigned int file_size = (u_int16_t)convertHex(file->size, 4);
	unsigned int byte_num = (sector_num - 2) * SECTOR_SIZE;

	unsigned int last_cluster = 0;
	unsigned int write_count = 0;

	// Write data from first cluster and loop for any others
	do
	{
		int index = 0;
		while (index < SECTOR_SIZE && write_count < file_size)
		{
			fprintf(out_file, "%c", data[byte_num + index]);
			index++; write_count++;
		}

		// Check if the cluster is the last one in the file
		if (*fat1[sector_num] >= 0xFF0 || *fat1[sector_num] == 0x0)
		{
			last_cluster = 1;
		}

		sector_num = *fat1[sector_num];
		byte_num = (sector_num - 2) * SECTOR_SIZE;

	} while (last_cluster == 0);
	
	// Close output file
	fclose(out_file);

	// Add to file_count global
	file_count++;
}

// Prints the file info as required for the assignment
void filePrint(File *file)
{
	printf("FILE\t");

	// Checks if it was a deleted file or not
	if(file->not_deleted)
	{
		printf("NORMAL\t");
	}
	else
	{
		printf("DELETED\t");
	}

	// Prints the file path
	for(int i = 0; i < file->path_len; i++)
	{
		printf("%c", file->path[i]);
	}

	// Sets first char of name to _ if file was deleted
	int name_index = 0;
	if(!file->not_deleted)
	{
		printf("_");
		name_index = 1;
	}

	// Prints file name without spaces
	while(file->name[name_index] != ' ' && name_index < MAX_NAME)
	{
		printf("%c", file->name[name_index]);
		name_index++;
	}
	
	// Prints file extension
	printf(".");
	for(int i = 0; i < MAX_EXT; i++)
	{
		printf("%c", file->ext[i]);
	}

	// Prints the file size
	printf("\t%d\n", convertHex(file->size, 4));
}

// Helper function to convert the given bytes to uint32_t big endian
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

// Helper function to separate bytes into the correct globals
void parseData(unsigned char *disk)
{
	unsigned char fat1_raw[FAT_SIZE];
	unsigned char fat2_raw[FAT_SIZE];

	// Loops through bytes from image
	// Checks byte count against location constants to assign properly
	for(int count = 0; count < MAX_BYTES; count++)
	{
		if(count < SECTOR_SIZE)
		{
			boot[count] = disk[count];
		}
		else if(count < FAT2_START)
		{
			fat1_raw[count-SECTOR_SIZE] = disk[count];
		}
		else if(count < ROOT_START)
		{
			fat2_raw[count-FAT2_START] = disk[count];
		}
		else if(count < DATA_START)
		{
			root[count-ROOT_START] = disk[count];
		}
		else
		{
			data[count-DATA_START] = disk[count];
		}
	}

	// Vars for calculating FAT entries
	unsigned char *one_f1 = malloc(sizeof(unsigned char));
	unsigned char *two_f1 = malloc(sizeof(unsigned char));
	unsigned char *three_f1 = malloc(sizeof(unsigned char));
	unsigned char *one_f2 = malloc(sizeof(unsigned char));
	unsigned char *two_f2 = malloc(sizeof(unsigned char));
	unsigned char *three_f2 = malloc(sizeof(unsigned char));

	// Loops through FAT tables to calculate entries and assign to fat globals
	int entry_num = 0;
	for(int i = 0; i < FAT_SIZE; i += 3)
	{
		// Gets next three bytes
		*one_f1 = fat1_raw[i];
		*two_f1 = fat1_raw[i+1];
		*three_f1 = fat1_raw[i+2];

		*one_f2 = fat2_raw[i];
		*two_f2 = fat2_raw[i+1];
		*three_f2 = fat2_raw[i+2];

		// Calculate entry values using helper function
		// Assign to fat entry globals
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

// Parses through the ROOT data to begin image analysis
void parseRoot()
{
	// Loops through the entries in the ROOT
	int count = 0;
	while((count + ENTRY_SIZE) <= ROOT_SIZE)
	{
		// Checks if there are other entries
		if(root[count] != FREE)
		{
			unsigned char *raw_bytes = malloc(ENTRY_SIZE);

			// Gets bytes for directory entry
			for(int i = 0; i <  ENTRY_SIZE; i++)
			{
				raw_bytes[i] = root[count];
				count++;
			}

			// Creates File object from directory entry
			char *file_path = malloc(sizeof(char));
			file_path = "/";

			File *file = fileCreate(raw_bytes, file_path);

			free(raw_bytes);

			// Checks if the directory entry is a file or subdirectory
			if(convertHex(file->size, 4) == 0)
			{
				parseSubDir(file);
			}
			else
			{
				filePrint(file);
				fileExtract(file);
			}
		}
		else
		{
			break;
		}
	}
}

// Parses a subdirectory and its entries
void parseSubDir(File *file)
{
	// Gets first logical cluster for subdirectory
	uint16_t clusters[MAX_ENTRIES];
	clusters[0] = (uint16_t)convertHex(file->first_cluster, 2);

	// Gets clusters associated with subdirectory
	int cluster_num = 0;
	while(cluster_num < MAX_ENTRIES)
	{
		if(*(fat1[clusters[cluster_num]]) == 0x00)
		{
			break;
		}
		else if(*fat1[clusters[cluster_num]] >= USE_MIN && *fat1[clusters[cluster_num]] <= USE_MAX)
		{
			clusters[cluster_num+1] = *(fat1[clusters[cluster_num]]);
			cluster_num++;
		}
		else
		{
			cluster_num++;
			break;
		}
	}

	// Calculates name length without spaces
	int name_len = 0;
	while(name_len < MAX_NAME)
	{
		if(file->name[name_len] == ' ')
		{
			break;
		}
		name_len++;
	}

	// Creates directory name for path building
	char dir_name[name_len];
	for(int i = 0; i < name_len; i++)
	{
		dir_name[i] = file->name[i];
	}
	dir_name[name_len] = '\0';

	// Gets current path of subdirectory for path building
	char curr_path[file->path_len];
	for(int i = 0; i < file->path_len; i++)
	{
		curr_path[i] = file->path[i];
	}

	// Combines current path and directory name to form new path
	char *new_path = malloc(file->path_len + name_len + 1);
	strcpy(new_path, curr_path);
	strcat(new_path, dir_name);
	strcat(new_path, "/");

	// Loops through subdirectory clusters
	for(int count = 0; count < cluster_num; count++)
	{
		int sector_start = ((clusters[count]-2)* SECTOR_SIZE);
		int entry_start = 64;
		
		// Loops through entries in cluster, skipping . and ..
		while(entry_start + ENTRY_SIZE <= SECTOR_SIZE)
		{
			// Checks if there are more entries
			if(data[sector_start + entry_start] != FREE)
			{
				// Gets bytes from data global at calculated sector
				unsigned char *raw_bytes = malloc(ENTRY_SIZE);
				for(int x = 0; x < ENTRY_SIZE; x++)
				{
					raw_bytes[x] = data[sector_start + entry_start];
					entry_start++;
				}

				// Creates new file from found entry
				File *new_file = fileCreate(raw_bytes, new_path);
				
				free(raw_bytes);

				// Checks if new file is a subdirectory or file
				if(convertHex(new_file->size, 4) == 0)
				{
					parseSubDir(new_file);
				}
				else
				{
					filePrint(new_file);
					fileExtract(new_file);
				}
			}
			else
			{
				break;
			}
		}
	}
}

// Main function
int main(int argc, char *argv[])
{
	// Checks for correct argument number
	if(argc != 3)
	{
		perror("Incorrect number of inputs given");
	}

	// Gets image file from input
	char *image_path = argv[1];
	int fp = open(image_path, O_RDONLY);

	// Assign virtual memory for image using mmap
	unsigned char *data;
	unsigned int length;
	struct stat stat_buffer;

	fstat(fp, &stat_buffer);
	length = (unsigned int)stat_buffer.st_size;
	data = (uint8_t *)mmap(0, length, PROT_READ, MAP_FILE|MAP_PRIVATE, fp, 0);

	// Get output destination from input and create if needed
	dest_path = malloc(strlen(argv[2]));
	dest_path = argv[2];
	mkdir(dest_path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

	// Call helper function to seperate data into correct globals
	parseData(data);

	// Start parsing the ROOT
	parseRoot();

	return 0;
}
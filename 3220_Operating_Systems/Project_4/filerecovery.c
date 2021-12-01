#include "filerecovery.h"

/* Global Variables */
char boot[SECTOR_SIZE];
uint16_t *fat1[FAT_ENTRIES];
uint16_t *fat2[FAT_ENTRIES];
unsigned char root[ROOT_SIZE];
unsigned char data[DATA_SIZE];

char *dest_path;
int fileNumber = 0;


/* Helper Functions======================================================================*/

/* Function: getSector      */
// uint16_t getSectorIndex(uint16_t fatEntry){
// 	return (uint16_t)(fatEntry - 2);
// }

/* Function: convertHex */
//Purpose: Converts a unsigned char *hex in little endian to uint32_t in big endian
uint32_t convertHex(unsigned char *hex, int numBytes){
	uint32_t result = 0;
	for(int i = 0; i < numBytes; i++){
		uint32_t temp = ((uint32_t) hex[i]) << (unsigned) (8 * i);
		result = result | temp;
	}
	return result;
}

/* Function: filePrint     */
//Purpose: Print formatted file information to stdout
void filePrint(File *file){
	assert(file != NULL);
	printf("FILE\t");

	assert(file->name != NULL);
	if(file->not_deleted)
	{
		printf("NORMAL\t");
	}
	else
	{
		printf("DELETED\t");
	}

	assert(file->path != NULL);
	int i = 0;
	while(i < file->path_len)
	{
		if(file->path[i] != ' ' && file->path[i] != '.') printf("%c", file->path[i]);
		i++;
	}

	if(!file->not_deleted)
	{
		printf("_");
		i = 1;
	}
	else
	{
		i = 0;
	}

	while(file->name[i] != ' ' && file->name[i] != '.' && i < 8)
	{
		printf("%c", file->name[i]);
		i++;
	}
	
	printf(".");

	assert(file->ext != NULL);
	i = 0;
	while(file->ext[i] != '.' && i < 3)
	{
		printf("%c", file->ext[i]);
		i++;
	}

	printf("\t");
	assert(file->size != NULL);
	printf("%d", convertHex(file->size, 4));

	printf("\n");
}

/*	Exploration Functions==================================================================*/
	/* Function: fileCreate    */
		File *fileCreate(unsigned char *raw, char *filepath){
			File *nextFile = malloc(sizeof(File));
			int currentInd = 0;

			//Check if the file is valid
			if(0xE5 != raw[currentInd]){
				nextFile->not_deleted = 1;
			} else {
				nextFile->not_deleted = 0;
			}

			//Read in name-------------------------------------------------
			assert(currentInd == 0);
			nextFile->name = (unsigned char *)malloc(8 * sizeof(char));
			for(int i = 0; i < 8; i++){
				nextFile->name[i] = raw[currentInd+i];
			}
			currentInd += 8;

			//Read in extension-------------------------------------------------
			assert(currentInd == 8);
			nextFile->ext = (unsigned char *)malloc(3 * sizeof(char));
			for(int i = 0; i < 3; i++){
				nextFile->ext[i] = raw[currentInd+i];
			}
			currentInd += 3;

			//Read in attributes------------------------------------------------
			assert(currentInd == 11);
			nextFile->attributes = (unsigned char *)malloc(1 * sizeof(char));
			nextFile->attributes[0] = raw[currentInd];
			currentInd++;

			//Read in reserved--------------------------------------------------
			assert(currentInd == 12);
			nextFile->reserved = (unsigned char *)malloc(2 * sizeof(char));
			for(int i = 0; i < 2; i++){
				nextFile->reserved[i] = raw[currentInd+i];
			}
			currentInd += 2;

			//Read in creation_time-----------------------------------------------
			assert(currentInd  == 14);
			nextFile->creation_time = (unsigned char *)malloc(2 * sizeof(char));
			for(int i = 0; i < 2; i++){
				nextFile->creation_time[i] = raw[currentInd+i];
			}
			currentInd += 2;

			//Read in creation_date-----------------------------------------------
			assert(currentInd == 16);
			nextFile->creation_date = (unsigned char *)malloc(2 * sizeof(char));
			for(int i = 0; i < 2; i++){
				nextFile->creation_date[i] = raw[currentInd+i];
			}
			currentInd += 2;

			//Read in last_access info------------------------------------------
			assert(currentInd == 18);
			nextFile->last_access = (unsigned char *)malloc(2 * sizeof(char));
			for(int i = 0; i < 2; i++){
				nextFile->last_access[i] = raw[currentInd+i];
			}
			currentInd += 4;

			//Read in lastModified Time------------------------------------------
			assert(currentInd == 22);
			nextFile->last_mod_time = (unsigned char *)malloc(2 * sizeof(char));
			for(int i = 0; i < 2; i++){
				nextFile->last_mod_time[i] = raw[currentInd+i];
			}
			currentInd += 2;

			//Read in lastModified Date-------------------------------------------
			assert(currentInd  == 24);
			nextFile->last_mod_date = (unsigned char *)malloc(2 * sizeof(char));
			for(int i = 0; i < 2; i++){
				nextFile->last_mod_date[i] = raw[currentInd+i];
			}
			currentInd += 2;

			//Read in first logical cluster---------------------------------------
			assert(currentInd == 26);
			nextFile->first_cluster = (unsigned char *)malloc(2 * sizeof(char));
			for(int i = 0; i < 2; i++){
				nextFile->first_cluster[i] = raw[currentInd+i];
			}
			currentInd += 2;

			//Read in file size---------------------------------------------------
			assert(currentInd == 28);
			nextFile->size = (unsigned char *)malloc(4 * sizeof(char));
			for(int i = 0; i < 4; i++){
				nextFile->size[i] = raw[currentInd+i];
			}
			currentInd += 4;

			nextFile->path_len = strlen(filepath);

			nextFile->path = (char *) malloc(nextFile->path_len);
			for(int i = 0; i < nextFile->path_len; i++){
				nextFile->path[i] = filepath[i];
			}

			return nextFile;
		}

/* Function: fileExtract   */
//Purpose: Finds the file's contents and saves it to the user's specified directory
void fileExtract(File *file)
{
	// Declare variables for storing name information, indices, and flags
	char file_name[MAX_PATH];
	char temp[8];
	char extension[4];
	unsigned int lastSectorFlag = 0;  // Flag for indicating if we are in the last sector of the file, set to 1 for yes
	unsigned int sectorIndex, bytesWritten = 0;

	// Give path for file_name, and extract extension
	strcpy(file_name, dest_path);
	sprintf(temp, "%d.", fileNumber);
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
	FILE * outputFile = fopen(file_name, "wb");
	assert(outputFile != NULL);

	union {
		char tempNum[16];
			unsigned int ui;
	} u;

	union {
		char tempNum2[16];
			unsigned int filesize;
	} u2;

	strncpy(u.tempNum, (char *)file->first_cluster, 2);
	for (i=2; i<=16; i++)
	{
		u.tempNum[i] = '\0';
	}
	
	strncpy(u2.tempNum2, (char *)file->size, 4);
	for (i=4; i<=16; i++)
	{
		u2.tempNum2[i] = '\0';
	}

	// Move to proper index of data array to begin writing bytes to output file and extract to buffer
	sectorIndex = u.ui;
	unsigned int dataIndex = (sectorIndex - 2) * SECTOR_SIZE;
	if ((sectorIndex >= 0xFF0 && sectorIndex <= 0xFF7) || sectorIndex == 0)
	{
		goto loopskip;
	}

	do {
		i = 0;
		while (i < SECTOR_SIZE && bytesWritten < u2.filesize)
		{
			fprintf(outputFile, "%c", data[dataIndex + i]);
			i++; bytesWritten++;
		}

		// Find the next sector from the FAT
		if (*fat1[sectorIndex] >= 0xFF0 || *fat1[sectorIndex] == 0x0)
		{
			lastSectorFlag = 1;
		}

		sectorIndex = *fat1[sectorIndex];
		dataIndex = (sectorIndex - 2) * SECTOR_SIZE;

	} while (lastSectorFlag == 0);

	loopskip: // Use this for special case of bad initial logical cluster
	
	fclose(outputFile); /* This is causing abortion in random image due to an invalid size */
	fileNumber++;
}

/* Function: parseDirectory */
//Purpose: Explores a subdirectory for files contained within
void parseDirectory(File *file)
{
	uint16_t clusters[16];
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
		else if(*fat1[clusters[cluster_num]] <= USE_MAX && *fat1[clusters[cluster_num]] >= USE_MIN )
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
	for(int k = 0; k < name_len; k++)
	{
		dir_name[k] = file->name[k];
	}
	dir_name[name_len] = '\0';

	char curr_path[file->path_len];
	for(int k = 0; k < file->path_len; k++)
	{
		curr_path[k] = file->path[k];
	}

	//Get new filepath
	int new_size = file->path_len + name_len + 1;
	char *new_path = malloc(new_size);

	strcpy(new_path, curr_path);
	strcat(new_path, dir_name);
	strcat(new_path, "/");

	int count = 0;
	while(count < cluster_num)
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
				assert(new_file != NULL);
				free(raw_bytes);

				if((convertHex(new_file->size, 4)) != 0)
				{
					filePrint(new_file);
					fileExtract(new_file);
				}
				else
				{
					parseDirectory(new_file);
				}
			}
			else
			{
				entry_start += 32;
			}
		}
		count++;
	}
}



/* Function: parseRoot  */
void parseRoot()
{
	assert(root != NULL);

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
			assert(file != NULL);

			//If file isn't a subdirectory, print and explore it
				//else explore subdirectory
			if((convertHex(file->size, 4)) != 0)
			{
				filePrint(file);
				fileExtract(file);
			}
			else
			{
				parseDirectory(file);
			}
		}
		else
		{
			count += 32;
		}
	}
}

/* Function: calcEntries     */
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

/* Disk Utility Functions==============================================================*/

/* Function: parseData  */
void parseData(unsigned char *disk)
{
	//Read the contents of the disk into the appropriate data structures
	unsigned char fat1_raw[FAT_SIZE];
	unsigned char fat2_raw[FAT_SIZE];

	int count = 0;
	while(count < MAX_BYTES){
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
		count++;
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

/* Function: getDisk        */
unsigned char *getData(char *image_path)
{
	unsigned char *data;
	unsigned int length;
	struct stat stat_buffer;

	int fp = open(image_path, O_RDONLY);
	//Assert file was read properly
	assert(fp >= 0);
	assert(fstat(fp, &stat_buffer) >= 0);

	length = (unsigned int) stat_buffer.st_size;
	data = (uint8_t *) mmap(0, length, PROT_READ, MAP_FILE|MAP_PRIVATE, fp, 0);
	assert(data != 0);

	return data;
}


/* Main */

int main(int argc, char *argv[])
{
	assert(argc == 3);

	//Program Input Parameters
	char *image_path = argv[1];			//Path of disk image
	dest_path = malloc(strlen(argv[2])); //Directory path for file output
	dest_path = argv[2];
	mkdir(dest_path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

	//Get Data
	unsigned char *data = getData(image_path);
	parseData(data);

	parseRoot();


	return 0;
}

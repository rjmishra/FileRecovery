/*--------------------------------------------------------*/
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <ext2fs/ext2_fs.h>
#include <ext2fs/ext2_types.h>	
#include <regex.h>
#include <sys/mount.h>
#include <string.h>
#include <time.h>
/*
#include <linux/fs.h>
#include <dirent.h>
*/
//defining the global variables
static unsigned long block_size = 0;  /* block size (to be calculated) */
static unsigned int inodePerGroup = 0;




/*--------------------------------------------------------*/

#define BASE_OFFSET 1024   
#define BLOCK_SIZES 4096	 //need to capture from superblock, to remove hardcoded value
#define BLOCK_OFFSET(block) ((block)*BLOCK_SIZES)
#define USB_DEV "/dev/sdb"
#define MOUNT_LOC "./pd"
#define EXT2_S_IFDIR 0x4000
#define INODE_LEN 4
#define REC_LEN 2
#define NAME_LEN 2
#define MAX_DIR_LEN 255
#define INPUT_DIR "/fresh/start/with/"
					


typedef struct node{
	unsigned long inodeNumber;
	unsigned long mergeLocation;						// location of previous record to which deleted is merged.
	unsigned long mergedLength;							// total length after records got merged.
	unsigned long previousRecordLength;					// previous records total length
	unsigned long blockNumber;
	char fileName[255];
}recoveryInode;

/*------------------------------------------------------------------------------------------


	Functions necesary for reading file system structure and finding out the inodes
	


-------------------------------------------------------------------------------------------*/




int printSuperBlock(int);
int printBlockGroupDesc(int);

unsigned long calculateInode(unsigned char *, unsigned);
unsigned long calculateRecordLen(unsigned char *, unsigned);

unsigned getDeletedInode(char *, unsigned, unsigned, FILE *);

void getAllDir(char *);
int readInodeTable(int, unsigned long);

void recoverDirEntry(char *);

void getAllEntries (char *, int );


unsigned long getLastInode (char *, int );

unsigned long getChildInodeNumber (int , unsigned long , char *);


unsigned long getInodeNumber (char *, char *);

unsigned *getDataBlocks (int fd);

struct ext2_group_desc getGroupDescriptor (int , unsigned );


/*------------------------------------------------------------------------------------------


	Functions necesary for recovery of the file 	


-------------------------------------------------------------------------------------------*/

void showDeletedFiles(char *path, int fd);

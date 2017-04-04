/*--------------------------------------------------------*/

//including necessary header files
#include "functions.h"

/*--------------------------------------------------------*/

//main function
int main(int argc, char *argv[])
{	
	int fd;	//file descriptor 
	int inodeLocation;
	long unsigned location;
	
	fd = open(USB_DEV, O_RDONLY);
	if(fd == -1)
	{
		perror("\nNot able to open the USB device!");
		exit(-1);
	}
	
	block_size = printSuperBlock(fd);
	
	/*printf("Size of the block: %lu\n", block_size);

	//inodeLocation = printBlockGroupDesc(fd);
	
	location = inodeLocation;
	printf("Inodes table block: %u\n", inodeLocation);
	
	location = location << 12;
	location += 256;
	printf("root directory location: %lu\n", location);
	
	//readInodeTable(fd, location);

	//getAllEntries(INPUT_DIR, fd);

	
	
	//printf("look into \"output\" directory for more output information files...\n");
	
	//mountPD();
	//getDirInodeNS();
	//umountPD(); */

	showDeletedFiles(INPUT_DIR, fd);
	
	close(fd); 

	return 0;
}

/*--------------------------------------------------------*/
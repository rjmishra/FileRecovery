#include "functions.h"

/*--------------------------------------------------------*/

//main function
int main(int argc, char *argv[])
{	
	int fd, i = 0;	//file descriptor 
	int inodeLocation;
	long unsigned location;
	char data[10];
	fd = open(USB_DEV, O_RDONLY);
	if(fd == -1)
	{
		perror("\nNot able to open the USB device!");
		exit(-1);
	}

	printSuperBlock(fd);
	struct ext2_group_desc group;

	group = getGroupDescriptor(fd, 0);

	readInodeTable(fd, BLOCK_OFFSET(group.bg_inode_table) + sizeof(struct ext2_inode)*2);


	for(i = 10; i < 20; i++){
		
		readInodeTable(fd, BLOCK_OFFSET(group.bg_inode_table) + sizeof(struct ext2_inode)*i);

	}
	//readInodeTable(fd, BLOCK_OFFSET(group.bg_inode_table) + sizeof(struct ext2_inode)*19);

	close(fd);
	return 0;

}

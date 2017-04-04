/*--------------------------------------------------------*/

//including necessary header files
#include "functions.h"

/*--------------------------------------------------------*/


recoveryInode getDeletedNode(char *data, unsigned start, unsigned end){
	unsigned len = 0, record = 0, nameLength = 0;
	unsigned long inode = 0;
	unsigned char inodeData[INODE_LEN], recData[REC_LEN], dir_name[NAME_LEN]; 

	recoveryInode recoverInode;

	recoverInode.inodeNumber = 0;
	
	unsigned i = start;
	
	/*FILE *fp;
	fp = fopen("./output/directory_root", "w+");
	if (fp == NULL)
	{
		perror("\nNot able to open \"./output/direcory_root\" directory..!");
		exit(-1);
	}*/


	while (data[i] == '\0' && i <= end)
		i++;
	
	/*if(i == end)
	{
		fprintf(fp, "\nNo deleted inode found");
		return 0;
	}*/



	while (data[i] != '\0' && len <= INODE_LEN)
	{
		inodeData[len] = data[i];
		i++; len++;
	}
	
	inode = calculateInode(inodeData, len);

	recoverInode.inodeNumber =  inode;

	i = i + (INODE_LEN - len);
	len = 0;

	while (data[i] != '\0' && len <= REC_LEN)
	{
		recData[len] = data[i];
		i++; len++;
	}
	
	record = calculateRecordLen(recData, len);
	i = i + (REC_LEN - len);

	nameLength = data[i++];
	i++;

	for (len = 0; len < nameLength; len++)
	{
			dir_name[len] = data[i];
			i++;
	}  
	dir_name[len] = '\0';

	strcpy(recoverInode.fileName, dir_name);

	return recoverInode;
}



recoveryInode *getAllDeletedFilesinBlock(char *data){

	unsigned i = 0, j, k, temp; 
	
	unsigned totalRecordLength = 0;
	unsigned char inode[INODE_LEN];
	unsigned char rec_len[REC_LEN];
	char dir_name[MAX_DIR_LEN];

	unsigned long inodeNumber;
	unsigned recordLength;
	unsigned nameLength;
	unsigned check = 0;

	int numOfDeletedFiles = 0;

	unsigned MAX_POSSIBLE_DIR = 340;

	recoveryInode *allDeletedFilesInThisBlock;

	allDeletedFilesInThisBlock = (recoveryInode *) malloc(sizeof(recoveryInode)*340);

	int numOfDeletedFiles = 0;

	while (totalRecordLength < 4096)
	{
		j = 0;
		temp = i;

		// for inode
		while (data[i] != '\0' && j < INODE_LEN)
		{						
			inode[j] = data[i];
			j++; i++; 
		}

		inodeNumber = calculateInode(inode, j);

		i = i + (INODE_LEN - j);
		j = 0;

		while (data[i] != '\0' && j < REC_LEN)
		{
			rec_len[j] = data[i];
			j++; i++;
		}

		recordLength = calculateRecordLen(rec_len, j);
		
		totalRecordLength += recordLength;
		i = i + (REC_LEN - j);
		j = 0;

		nameLength = data[i++];
		i++;

		for (k = 0; k < nameLength; k++)
		{
			dir_name[k] = data[i];
			i++;
		}  
		dir_name[k] = '\0';

		check = 8 + nameLength + nameLength % 4;

		if (recordLength >= check + 12){

			allDeletedFilesInThisBlock[numOfDeletedFiles] = getDeletedNode(data, i, temp+recordLength, fp);

			allDeletedFilesInThisBlock[numOfDeletedFiles].mergeLocation = temp;
			allDeletedFilesInThisBlock[numOfDeletedFiles].mergedLength recordLength;
			allDeletedFilesInThisBlock[numOfDeletedFiles].previousRecordLength = check;
			numOfDeletedFiles++;
		}

		i = temp + recordLength;
	}	

	allDeletedFilesInThisBlock[numOfDeletedFiles].mergedLength = 0;

	return allDeletedFilesInThisBlock;
}


recoveryInode *getAllDeletedFiles(unsigned long *dataBlocks, int fd){


	recoveryInode *deletedFiles;
	//for(i = 0; i < 15; i++){
		//printf("datablock i: %lu\n", dataBlocks[i]);
		if(dataBlocks[0] != 0){
		//		printf("\nLooking in data blocks to find the directories");
			lseek(fd, BLOCK_OFFSET(dataBlocks[0]), SEEK_SET);
			read(fd, &data, sizeof(data));
			//getAllDir(data);

			deletedFiles = getAllDeletedFilesinBlock(data);


			while(deletedFiles[i].deletedFiles != 0){
				deletedFiles[i].blockNumber = dataBlocks[0];
				i++;
			}

			return deletedFiles;

		}

		return NULL;
	}



recoveryInode getDeletedFileToRecover(char *path, int fd){
	unsigned long inodeNumber;
	unsigned long long location;
	unsigned groupNumber, i = 0;
	struct ext2_group_desc groupDesc;
	unsigned *dataBlocks;
	char data[4096];
	recoveryInode *deletedFiles;

	//block_size = printSuperBlock(fd);
	inodeNumber = getLastInode(path, fd);



	groupNumber = inodeNumber/7712;

	printf("\ninode number: %lu", inodeNumber);

	groupDesc = getGroupDescriptor(fd, groupNumber);
	
	lseek(fd, BLOCK_OFFSET(groupDesc.bg_inode_table)+((inodeNumber - 1) % 7712)*sizeof(struct ext2_inode), SEEK_SET);
	
	
	location = BLOCK_OFFSET(groupDesc.bg_inode_table)+((inodeNumber - 1) % 7712)*sizeof(struct ext2_inode);
	dataBlocks = getDataBlocks(fd);


	deletedFiles = getAllDeletedFiles(dataBlocks, fd);


	printf("\nPlease select file to recover: ");

	while(deletedFiles[i].mergedLength != 0){
		printf("SN: %u Inode Number: %lu  file Name: %s", i, deletedFiles[i].inodeNumber, deletedFiles[i].fileName);
	}

	printf("\nPlease enter your choice(only integer):  ");
	scanf("%u", &i);

	return deletedFiles[i];

}



int writeRecordLength(unsigned long recordLength, unsigned long location, int fd){

	char x[2];

	x[0] = recordLength % 256;
	if(recordLength/256 != 0)										//need to remove hardcoded value		
		x[1] = recordLength/256;				
	else
		//x[0] = recordLength % 256;
		x[1] = NULL;


	x[0] = recordLength % 256;

	lseek(fd, location, SEEK_SET);
	if(write(fd, x, sizeof(x)) != -1){
		return 1;
	}

	return 0;

}



int recoverDirectoryEntry(recoveryInode recoverInode, int fd){
	unsigned long location;

	int result = 0, result2 = 0;

	location = BLOCK_OFFSET(recoverInode.blockNumber);

	location += recoverInode.mergeLocation + REC_LEN;

	result = writeRecordLength(recoverInode.previousRecordLength, location, fd );

	location += previousRecordLength;

	result2 = writeRecordLength(recoverInode.mergedLength - recoverInode.previousRecordLength, location, fd );	

	return result2*result;

}


int recoverBlockBitmap(char *bitmapBlock, unsigned long *blocks){
	int i = 0;										// indirect blocks to be included, single, double as well triple.
	unsigned byteNumber;							// only direct for now
	unsigned bitNumber;

	for(i = 0; i <= 12; i++){
		if(blocks[i] == 0){
			return i+1;
		}

		byteNumber = blocks[i]/8;
		bitNumber = block[i] % 8;

		bitmapBlock[byteNumber] |= 1 << bitNumber;
	}

	return 0;
}


int recoverInodeTable(unsigned long inodeNumber, int fd){
	unsigned groupNumber;
	struct ext2_group_desc groupDesc;
	struct ext2_inode inode;

	char blockBitmap[4096];
	char inodeBitmap[4096];

	unsigned groupNumber = inodeNumber/inodePerGroup;
	groupDesc = getGroupDescriptor(fd, groupNumber);

	if(groupNumber == 0)
		lseek(fd, BLOCK_OFFSET(groupDesc.bg_inode_table)+(inodeNumber % inodePerGroup)*sizeof(struct ext2_inode), SEEK_SET);
		
	else
		lseek(fd, BLOCK_OFFSET(groupDesc.bg_inode_table)+((inodeNumber - 1) %inodePerGroup)*sizeof(struct ext2_inode), SEEK_SET);
	

	read(fd, &inode, sizeof(inode));

	inode.i_dtime = 0;
	inode.i_links_count += 1

	lseek(fd, sizeof(struct ext2_inode)*(-1), SEEK_CUR);

	if(write(fd, &inode, sizeof(inode)) != -1){
		
		lseek(fd, BLOCK_OFFSET(groupDesc.bg_block_bitmap), SEEK_SET);

		if(read(fd, &blockBitmap, sizeof(blockBitmap)) != -1){
			
			if(recoverBlockBitmap(blockBitmap, inode.i_blocks)){
			
				lseek(fd, sizeof(blockBitmap) * (-1), SEEK_CUR);
				if( write(fd, &blockBitmap, sizeof(blockBitmap)) != -1)
					return 1;
			}
		}
		return 0;
	}


	return 0;
}



int recoverInodeBitmap(unsigned long inodeNumber, int fd){
	struct ext2_group_desc groupDesc;
	char data[4096];
	unsigned int byteNumber = 0;
	unsigned bitNumber = 0;
	//int writeStatus = 0;
	groupDesc = getGroupDescriptor(fd, groupNumber);
	lseek(fd, BLOCK_OFFSET(groupDesc.bg_inode_bitmap), SEEK_SET);

	if(read(fd, &data, sizeof(data)) != -1){
		inodeNumber = inodeNumber % 7712;					//hardcoded value needs to be changed
		byteNumber = inodeNumber/8;
		bitNumber = (char) (inodeNumber % 8);

		data[byteNumber] |= 1 << bitNumber;

		lseek(fd, sizeof(data) * (-1), SEEK_CUR);
		if (write(fd, &data, sizeof(data)) != -1)
			return 1;
	}

	reurn 0;	
	
}

int is_power(unsigned x, unsigned y){
	if (x > 1)
		while (x < y && y % x == 0)
			y /= x;
	return x == y || (x != 0 && y == 1);
}


int isSuperBlockGroup(unsigned groupNumber){
	if(groupNumber == 0 || groupNumber == 1 || is_power(3, groupNumber) || is_power(5, groupNumber) || is_power(7, groupNumber))
		return 1;

	return 0;
}


int recoverGroupDesc(int fd, unsigned groupNumber, unsigned numberOfBlocks){
	unsigned i = 0, j = 0;
	struct ext2_group_desc groupDesc;


	for(i = 0; i < 16; i++){							// need to remove hardcoded value

		j = (isSuperBlockGroup(groupNumber));
		
		lseek(fd, 32768 * block_size * i + groupNumber + 4096 * j, SEEK_SET);
		
		if( read(fd, &groupDesc, sizeof(struct ext2_group_desc)) ! = -1){
			groupDesc.bg_free_blocks_count -= numberOfBlocks;
			groupDesc.bg_free_inodes_count -= 1;
			lseek(fd, sizeof(struct ext2_group_desc) * (-1), SEEK_CUR);

			if(write(fd, &groupDesc, sizeof(struct ext2_group_desc)) == 0){
				return 0;
			}
				//bg_used_dirs_count can be added for the case of deleted directory.
		}
	}

	return 1;
}


int recoverSuperBlock(int fd, unsigned numberOfBlocks){
	unsigned i = 0, j = 0;
	struct ext2_super_block superBlock;
	unsigned long time;

	timeCurrent = (unsigned long) time(NULL);
	lseek(fd, 1024 , SEEK_SET);
	if(read(fd, &superBlock, sizeof(ext2_super_block)) != -1){
		superBlock.s_free_blocks_count -=  numberOfBlocks;
		superBlock.s_free_inodes_count -= 1;
		superBlock.s_wtime = timeCurrent;
		lseek(fd, 1024 , SEEK_SET);
		if(write(fd, &superBlock, sizeof(struct ext2_super_block)) == 0){
			return 0;
		}
	}

	for(i = 1; i < 16; i++){	
											// need to remove hardcoded value
		if((isSuperBlockGroup(i)){
			lseek(fd, i*32768 , SEEK_SET);
			if(read(fd, &superBlock, sizeof(ext2_super_block)) != -1){
				superBlock.s_free_blocks_count -=  numberOfBlocks;
				superBlock.s_free_inodes_count -= 1;
				superBlock.s_wtime = timeCurrent;
				lseek(fd, sizeof(ext2_super_block)*(-1) , SEEK_CUR);
				if(write(fd, &superBlock, sizeof(struct ext2_super_block)) == 0){
					return 0;
				}
			}

		}

	}




	return 1;
}
/*--------------------------------------------------------*/

//including necessary header files
#include "functions.h"

/*--------------------------------------------------------*/

int printSuperBlock (int fd)
{
	struct ext2_super_block super;
	FILE *fp;
	
	lseek(fd, BASE_OFFSET, SEEK_SET); 

	fp = fopen("./output/superblock", "w+");
	if (fp == NULL)
	{
		perror("\nNot able to open \"./output/superblock\" directory..!");
		exit(-1);
	}

	if (read(fd, &super, sizeof(struct ext2_super_block)) !=-1)
	{
		fprintf(fp, "\nSize of superblock:%lu\n\n", sizeof(struct ext2_super_block));
  		
 		fprintf(fp, "Inodes count: %u\n", super.s_inodes_count);
  		fprintf(fp, "Blocks count: %u\n", super.s_blocks_count);
  		fprintf(fp, "Reserved blocks count: %u\n", super.s_r_blocks_count);
		fprintf(fp, "Free blocks count: %d\n", super.s_free_blocks_count);
  		fprintf(fp, "Free inodes count: %u\n", super.s_free_inodes_count);
		fprintf(fp, "First Data Block: %u\n", super.s_first_data_block);
  		fprintf(fp, "Blocks size: %u\n", super.s_log_block_size);  	
  		fprintf(fp, "Allocation cluster size: %u\n", super.s_log_cluster_size);
  		
  		fprintf(fp, "Blocks per group: %u\n", super.s_blocks_per_group);
  		fprintf(fp, "Fragments per group: %u\n", super.s_clusters_per_group);
  		fprintf(fp, "Inodes per group: %u\n", super.s_inodes_per_group);
  		
  		fprintf(fp, "Mount time: %u\n", super.s_mtime);
  		fprintf(fp, "Write time: %u\n", super.s_wtime);
  		fprintf(fp, "Mount count: %hu\n", super.s_mnt_count);
  		fprintf(fp, "Maximal mount count: %hu\n", super.s_max_mnt_count);
  		fprintf(fp, "Magic signature: %hu\n", super.s_magic);
  		fprintf(fp, "File system state: %hu\n", super.s_state);
  		fprintf(fp, "Behaviour when detecting errors: %hu\n", super.s_errors);
  		fprintf(fp, "minor revision level: %hu\n", super.s_minor_rev_level);
  		
  		fprintf(fp, "time of last check: %hu\n", super.s_lastcheck);
  		fprintf(fp, "max. time between checks: %u\n", super.s_checkinterval);
  		fprintf(fp, "OS: %hu\n", super.s_creator_os);
  		fprintf(fp, "Revision level: %hu\n", super.s_rev_level);
  		fprintf(fp, "Default uid for reserved blocks: %u\n", super.s_def_resuid);
  		fprintf(fp, "Default gid for reserved blocks: %u\n", super.s_def_resgid);
  		
  		fprintf(fp, "First non-reserved inode: %u\n", super.s_first_ino);
  		fprintf(fp, "size of inode structure: %hu\n", super.s_inode_size);
  		fprintf(fp, "block group # of this superblock: %hu\n", super.s_block_group_nr);
  		fprintf(fp, "volume name: %s\n", super.s_volume_name);
  		fprintf(fp, "When the filesystem was created: %u\n", super.s_mkfs_time);	
  		fprintf(fp, "\nthese are important variables in superblock..others you can see in /usr/include/ext2_fs.h\n");
		
  		block_size = 1024 << super.s_log_block_size;
  		inodePerGroup = super.s_inodes_per_group;

  		fclose(fp);
  		return block_size;
	}
	
		perror("\nUnable to read super block!");
		fclose(fp);
		return -1;
}

/*--------------------------------------------------------*/

int printBlockGroupDesc (int fd)
{
	struct ext2_group_desc blockDesc;
	FILE *fp;


	lseek(fd, BLOCK_SIZES, SEEK_SET); 			//remove BLOCK_SIZE with block_size

	fp = fopen("./output/block_group_desc", "w+");
	if (fp == NULL)
	{
		perror("\nNot able to open \"./output/block_group_desc\" directory..!");
		exit(-1);
	}

	if (read(fd, &blockDesc, sizeof(struct ext2_group_desc)) != -1)
	{
		 fprintf(fp, "\nReading first group-descriptor from device:\n"
         "Blocks bitmap block: %u\n"
         "Inodes bitmap block: %u\n"
         "Inodes table block : %u\n"
         "Free blocks count  : %u\n"
         "Free inodes count  : %u\n"
         "Directories count  : %u\n",
         blockDesc.bg_block_bitmap,
         blockDesc.bg_inode_bitmap,
         blockDesc.bg_inode_table,
         blockDesc.bg_free_blocks_count,
         blockDesc.bg_free_inodes_count,
         blockDesc.bg_used_dirs_count);

		 printf("size of inode(large): %lu\n", sizeof(struct ext2_inode_large));
		 printf("size of inode: %lu\n", sizeof(struct ext2_inode));
		 
		 fclose(fp);
		 return blockDesc.bg_inode_table;
	}

	perror("\nUnbale to read block group descriptor!");
	fclose(fp);
	return -1;
}

/*--------------------------------------------------------*/

int readInodeTable (int fd, unsigned long location)
{
	struct ext2_inode inode;
	FILE *fp;
	int i;
	char data[4096];

	lseek(fd, location , SEEK_SET); 
	
	fp = fopen("./output/inode_details", "a");
	if (fp == NULL)
	{
		perror("\nNot able to open \"./output/inode_root\" directory..!");
		exit(-1);
	}

	if (read(fd, &inode, sizeof(struct ext2_inode)) != -1)
	{
		fprintf(fp, "\nFile mode: %u\n", inode.i_mode);	
		fprintf(fp, "Low 16 bits of Owner Uid: %u\n", inode.i_uid);	
		fprintf(fp, "Size in bytes: %u\n", inode.i_size);	
		fprintf(fp, "Access time: %u\n", inode.i_atime);	
		fprintf(fp, "Inode change time: %u\n", inode.i_ctime);	
		fprintf(fp, "Modification time: %u\n", inode.i_mtime);	
		fprintf(fp, "Deletion Time: %u\n", inode.i_dtime);	
		fprintf(fp, "Low 16 bits of Group Id: %u\n", inode.i_gid);
		fprintf(fp, "Links count: %u\n", inode.i_links_count);	
		fprintf(fp, "Blocks count: %u\n", inode.i_blocks);	
		fprintf(fp, "File flags: %u\n", inode.i_flags);
	}
	else
	{
		perror("\nUnbale to read inode Table!");
		fclose(fp);
		return -1;
	}

	//if (inode.i_mode & EXT2_S_IFDIR)
	//{
		fprintf(fp, "\nIts data blocks assigned are:\n");

		for (i = 0; i < EXT2_N_BLOCKS; i++)
		{
			fprintf(fp, "(%d)%u  ", i + 1, inode.i_block[i]);
		}
		fprintf(fp, "\n");
	//}
	
	//lseek(fd, BLOCK_OFFSET(inode.i_block[0]), SEEK_SET); 

	/*if (read(fd, &data, sizeof(data)) != -1)
	{	
		getAllDir(data);
		//recoverDirEntry(data);
	}
	else
	{
		perror("\nUnbale to read data block!");
		fclose(fp);
		return -1;
	}*/

	fclose(fp);
	return 0;
}

/*--------------------------------------------------------*/

void getAllDir (char *data)
{
	FILE *fp;
	unsigned i = 0, j, k, temp; 
	
	unsigned totalRecordLength = 0;
	unsigned char inode[INODE_LEN];
	unsigned char rec_len[REC_LEN];
	char dir_name[MAX_DIR_LEN];

	unsigned long inodeNumber;
	unsigned recordLength;
	unsigned nameLength;
	unsigned check = 0;

	fp = fopen("./output/directory_info", "w+");
	if (fp == NULL)
	{
		perror("\nNot able to open \"./output/direcory_root\" directory..!");
		exit(-1);
	}

	fprintf(fp, "\nDetails of deleted files in this directory:");

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

		fprintf(fp, "\ninode: %6lu\t\t rec_len : %6hu\t\t name_len: %4hu\t\t name: %s",
				inodeNumber, recordLength, nameLength, dir_name );

		check = 8 + nameLength + nameLength % 4;

		if (recordLength >= check + 12)
			getDeletedInode(data, i, temp+recordLength, fp);

		i = temp + recordLength;
	}	
	fprintf(fp, "\n");
	fclose(fp);
}

/*--------------------------------------------------------*/

unsigned long calculateInode (unsigned char *x, unsigned len)
{
	if (len == 1)
		return x[0];
	
	else if (len == 2)
		return x[0] + 256 * x[1];
	
	else if (len == 3)
		return x[0] + 256 * x[1] + 256 * 256 * x[2];
	
	else
		return x[0] + 256 * x[1] + 256 * 256 * x[2] + 256 * 256 * 256 * x[3]; 
}

/*-----------------------------------------------------------*/

unsigned long calculateRecordLen (unsigned char *x, unsigned len)
{
	if (len == 1)
		return x[0];
	
	else
		return x[1] + 256 * x[0];
}

/*--------------------------------------------------------*/

unsigned getDeletedInode (char *data, unsigned start, unsigned end, FILE *fp)
{
	unsigned len = 0, record = 0, nameLength = 0;
	unsigned long inode = 0;
	unsigned char inodeData[INODE_LEN], recData[REC_LEN], dir_name[NAME_LEN]; 
	
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

	if(nameLength > 0)
		printf("\ninode: %5lu*\t\t rec_len : %6hu\t\t name_len: %4hu\t\t name: %s",
			inode, record, nameLength, dir_name );

	return 0;
}

/*--------------------------------------------------------*/

/* Function to get the group descriptor of trhe given group */
/* This function will need group number as input and return group descritor */

 struct ext2_group_desc getGroupDescriptor (int fd, unsigned groupNumber)
 {
	struct ext2_group_desc groupDesc;
	
	lseek(fd, BLOCK_SIZES + groupNumber * sizeof(struct ext2_group_desc), SEEK_SET);   //remove BLOCK_SIZE with block_size
	//groupDesc = (ext2_group_desc*) malloc(sizeof(ext2_group_desc));
	
	if (read(fd, &groupDesc, sizeof(struct ext2_group_desc)) != -1)
		return groupDesc;
	
	return groupDesc;
 }

/*--------------------------------------------------------*/

 unsigned *getDataBlocks (int fd)
 {
	struct ext2_inode inode;
	FILE *fp;
	int i;
	unsigned *dataBlocks;

	dataBlocks = (unsigned *) malloc(sizeof(unsigned)*15);

	fp = fopen("./output/inode_details_all", "a");
	if (fp == NULL)
	{
		perror("\nNot able to open \"./output/inode_root\" directory..!");
		exit(-1);
	}

	//lseek(fd, location , SEEK_SET);
	if (read(fd, &inode, sizeof(struct ext2_inode)) != -1)
	{
		//if (inode.i_mode & EXT2_S_IFDIR)
		//{

		fprintf(fp, "\nFile mode: %u\n", inode.i_mode);	
		fprintf(fp, "Low 16 bits of Owner Uid: %u\n", inode.i_uid);	
		fprintf(fp, "Size in bytes: %u\n", inode.i_size);	
		fprintf(fp, "Access time: %u\n", inode.i_atime);	
		fprintf(fp, "Inode change time: %u\n", inode.i_ctime);	
		fprintf(fp, "Modification time: %u\n", inode.i_mtime);	
		fprintf(fp, "Deletion Time: %u\n", inode.i_dtime);	
		fprintf(fp, "Low 16 bits of Group Id: %u\n", inode.i_gid);
		fprintf(fp, "Links count: %u\n", inode.i_links_count);	
		fprintf(fp, "Blocks count: %u\n", inode.i_blocks);	
		fprintf(fp, "File flags: %u\n", inode.i_flags);

		fprintf(fp, "\nIts directory data blocks assigned are:\n");

		for (i = 0; i < EXT2_N_BLOCKS; i++)
		{
			fprintf(fp, "(%d)%u  ", i + 1, inode.i_block[i]);
		}
		fprintf(fp, "\n");

			//printf("inode number is: lu\n");
			dataBlocks = inode.i_block;

			return dataBlocks;
		//}

		//printf("\nError reading data block from memory!");
	}
	
	fclose(fp);

	printf("\n Returning dummy!");
	return dataBlocks;			//dummy need to recorrect
}
		
/*--------------------------------------------------------*/

unsigned long getInodeNumber (char *data, char *childName)
{
	FILE *fp;
	unsigned i = 0, j, k, temp; 
	
	unsigned totalRecordLength = 0;
	unsigned char inode[INODE_LEN];
	unsigned char rec_len[REC_LEN];
	char dir_name[MAX_DIR_LEN];

	unsigned long inodeNumber;
	unsigned recordLength;
	unsigned nameLength;
	unsigned check = 0;

	while (totalRecordLength < BLOCK_SIZES){	//replace BLOCK_SIZE with block_size

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
		dir_name[k] = '\0';					//need to recheck about assigning the it to same pointer 

		//fprintf(fp, "\ninode: %6lu\t\t rec_len : %6hu\t\t name_len: %4hu\t\t name: %s",
		//		inodeNumber, recordLength, nameLength, dir_name );

		check = INODE_LEN + REC_LEN + NAME_LEN + nameLength + nameLength % 4;

		if (strcmp(childName, dir_name) == 0)
		{
			printf("\n");
			return inodeNumber;
		}

		i = temp + recordLength;
	}	

	return 0;
}

/*--------------------------------------------------------*/
/*
void recoverDirEntry (char *data)
{

	char dirName[255];
	char parentDir[255];

	if(data[0] != '/'){
		printf("Not valid locations!");
		return;
	}

	else{
		strcpy(parentDir, "/");
		while(data)
	}
}

/*--------------------------------------------------------*/

/* This  function will take two arguments, first is inode number of parent and second
  the name of child to be searched, it will return inode of the child if found else, will retorn 0 */

unsigned long getChildInodeNumber (int fd, unsigned long parentInode, char *childName)
{
	int i = 0;
	struct ext2_group_desc groupDesc;
	unsigned *dataBlocks, inodeNumber;
	unsigned groupNumber= parentInode/inodePerGroup;
	char data[4096];
	
	groupDesc = getGroupDescriptor(fd, groupNumber);
	if(groupNumber == 0)
		lseek(fd, BLOCK_OFFSET(groupDesc.bg_inode_table)+(parentInode%inodePerGroup)*sizeof(struct ext2_inode), SEEK_SET);
		
	else
		lseek(fd, BLOCK_OFFSET(groupDesc.bg_inode_table)+((parentInode - 1) %inodePerGroup)*sizeof(struct ext2_inode), SEEK_SET);
	

	printf("\nOffset : %lu , count in group : %lu,  inode number: %lu and group : %lu inodetable loc: %lu inodePerGroup : %lu ", BLOCK_OFFSET(groupDesc.bg_inode_table),  (parentInode - 1) %inodePerGroup , parentInode, groupNumber,  groupDesc.bg_inode_table, inodePerGroup );

	
	dataBlocks = getDataBlocks(fd);

	//printf("\nPresently child is : %s and parent's inode is: %lu\n", childName, parentInode);

	while (dataBlocks[i] != 0)
	{
		printf("\n the data block is : %u\n", dataBlocks[i]);
		lseek(fd, BLOCK_OFFSET(dataBlocks[i]), SEEK_SET);
		read(fd, &data, sizeof(data));
		inodeNumber = getInodeNumber(data, childName);
		i++;
		if(inodeNumber != 0)
			return inodeNumber;
	}

	//free(dataBlocks);

	return 0;
}

/*--------------------------------------------------------*/

unsigned long getLastInode (char *path, int fd)
{
	char parent[255];
	char child[255];
	int i = 0, j = 0;

	unsigned inodeNumberParent = 2;
	unsigned inodeNumberChild = 0;

	if(path[0] != '/'){
		printf("Not valid locations!");
		return 0;
	}

		strcpy(parent, "/");
		while(*path != '\0'){
			path++;
			while(*path != '/' && *path != '\0'){
				child[i] = *path;
				i++;
				path++;
			}
			child[i] = '\0';

			inodeNumberChild = getChildInodeNumber(fd, inodeNumberParent, child);
			inodeNumberParent = inodeNumberChild;

			for(j = 0; j < i; j++){
				child[j] = '\0';
			}

			i = 0;
		}
	return inodeNumberChild;
}

/*--------------------------------------------------------*/

void getAllEntries (char *path, int fd)
{

	unsigned long location;
	unsigned long inodeNumber;
	struct ext2_group_desc groupDesc;
	unsigned groupNumber;
	inodeNumber = getLastInode(path, fd);
	groupNumber= inodeNumber/inodePerGroup;
	groupDesc = getGroupDescriptor(fd, groupNumber);
	location =  BLOCK_OFFSET(groupDesc.bg_inode_table)+(inodeNumber%inodePerGroup)*sizeof(struct ext2_inode);
	//printf("inode: %lu and group: %lu and location: %lu start of inode: %u inode per grp: %lu", inodeNumber, groupNumber, location, groupDesc.bg_inode_table, inodePerGroup);
	//readInodeTable(fd, location);

	printf("\nThe last inode is: %lu\n",inodeNumber);

}

/*--------------------------------------------------------*/
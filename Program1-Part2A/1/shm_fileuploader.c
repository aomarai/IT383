#include <unistd.h>
#include <sys/mman.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#define MAX_FILESIZE 52428800
struct filesharing_struct
{
    char flag;
    int fileSize;
    char fileName[50];
    char buffer[MAX_FILESIZE];
};


int main(int argc, char *argv[])
{
    int fd;
    struct filesharing_struct *structptr;
    FILE *sourceFile;
    char *sourceFileName;
    size_t bytes;
    char readBuffer[1024];

    //Create shared memory object
    fd = shm_open("/aomarai_filesharing", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    if (fd == 1)
    {
        fprintf(stderr, "Problem opening shared memory.");
        return(1);
    }

    //Set the size of the object
    if (ftruncate(fd, sizeof(struct filesharing_struct)) == -1)
    {
        fprintf(stderr, "Problem setting shared memory size.");
        return(2);
    }

    //Map the memory area
    if ((structptr = mmap(NULL, sizeof(struct filesharing_struct), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)) == MAP_FAILED)
    {
        fprintf(stderr, "Failed to map memory region.");
        return(3);
    }
    
    printf("Enter file location: ");
    scanf("%s", sourceFileName);

    //Write filename to the struct in shared memory
    strncpy(structptr->fileName, sourceFileName, sizeof(sourceFileName));

    //Read from file into struct
    sourceFile = fopen(sourceFileName, "r");
    if (sourceFile == NULL)
    {
        fprintf(stderr, "Failed to open source file for reading.");
        return(4);
    }

    //Read from source file while making sure it returns more than 0
    while (0 < (bytes = fread(readBuffer, 1, sizeof(readBuffer), sourceFile)))
    {
        //Copy data from the source file buffer into the struct so it reads 1024 bytes at a time
        memcpy(structptr->buffer, readBuffer, sizeof(readBuffer));
    }

    //Set the size of the file that was created
    structptr->fileSize = sizeof(structptr->buffer);

    //Flag the file as uploaded
    structptr->flag = 'u';

    fclose(sourceFile);

    return 0;
}   
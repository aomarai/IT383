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
    FILE *outputFile;
    size_t bytes;

    //Open shared memory
    fd = shm_open("/aomarai_filesharing", O_RDONLY, 0666);
    if (fd == 1)
    {
        fprintf(stderr, "Problem opening shared memory.");
        return(1);
    }

    //Map the object
    if ((structptr = mmap(NULL, sizeof(struct filesharing_struct), PROT_READ, MAP_SHARED, fd, 0)) == MAP_FAILED)
    {
        fprintf(stderr, "Failed to map memory region.");
        return(2);
    }

    //See if there's any data in the memory region
    if (structptr->flag != 'u')
    {
        printf("File not found.");
        return(3);
    }

    //Open output file
    outputFile = fopen("sharedFile", "w");
    if (outputFile == NULL)
    {
        fprintf(stderr, "File sharedFile could not be opened");
        return(4);
    }

    //Read from source file while making sure return is more than 0
    while (0 < (bytes = fread(structptr->buffer, 1, sizeof(structptr->buffer), outputFile)))
    {
        fwrite(structptr->buffer, 1, bytes, outputFile);
    }

    //Set file as downloaded
    structptr->flag = 'd';

    //Close files
    fclose(outputFile);

    return 0;
}
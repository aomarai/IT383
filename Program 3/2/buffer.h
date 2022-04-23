typedef int bufferitem;
#define BUFFER_SIZE 5

//Inserts an item into the buffer. Returns 0 if successful, -1 otherwise.
int insert_item(bufferitem item);

//Removes an item from the buffer. Returns 0 if successful, -1 otherwise.
int remove_item(bufferitem *item);
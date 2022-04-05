all: shm_fileuploader shm_filedownloader

shm_fileuploader: shm_fileuploader.c
	gcc -o uploader shm_fileuploader.c -lrt

shm_filedownloader: shm_filedownloader.c
	gcc -o downloader shm_filedownloader.c -lrt
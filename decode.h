#ifndef DECODE_H
#define DECODE_H

#include "types.h" // Contains user defined types

#define MAX_SECRET_BUF_SIZE 1  
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8) 
#define MAX_FILE_SUFFIX 4  

typedef struct _DecodeInfo
{
    /* Input Image info */
    char *src_image_fname;  //store .bmp file
    FILE *fptr_src_image;  //open src file and store in fptr
    // char image_data[MAX_IMAGE_BUF_SIZE]; // char arr of 8 byte to store the data

    /* Secret File Info */
    char secret_fname[50];  // store s.txt file
    FILE *fptr_secret; // store scrt file  
    char extn_secret_file[MAX_FILE_SUFFIX];  //in here file extn is .txt(max 4 bytes) but any type of file can be passed
    //char secret_data[MAX_SECRET_BUF_SIZE]; 
    long size_secret_file;
} DecodeInfo;


/* Encoding function prototype */

/* Read and validate Encode args from argv */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);

/* Perform the encoding */
Status do_decoding(DecodeInfo *decInfo);

/* skip bmp image header*/
Status Skip_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image);

/* Store Magic String */
Status decode_magic_string(const char *magic_string, DecodeInfo *decInfo);

/* Encode secret file extenstion size */
Status decode_secret_file_extn_size(uint size, DecodeInfo *decInfo);

/* Encode secret file extenstion */
Status decode_secret_file_extn(const char *file_extn, DecodeInfo *decInfo);

/* Encode secret file size */
Status decode_secret_file_size(long file_size, DecodeInfo *decInfo);

/* Encode secret file data*/
Status decode_secret_file_data(DecodeInfo *decInfo);

/* Encode function, which does the real encoding */
Status decode_data_from_image(char *data, int size, FILE *fptr_src_image, FILE *fptr_stego_image);

/* Encode a byte into LSB of image data array */
Status decode_byte_from_lsb(char data, char *image_buffer);

/* Encode a 4 byte into LSB of image data array */
Status decode_size_from_lsb(uint size, char *image_buffer);

#endif

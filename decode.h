#ifndef DECODE_H
#define DECODE_H

#include "types.h" // Contains user defined types

#define MAX_SECRET_BUF_SIZE 1  
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8) 
#define MAX_FILE_SUFFIX 4  

typedef struct _DecodeInfo
{
    /* Input Image info */
    char *src_image_fname; 
    FILE *fptr_src_image;  

    /* Secret File Info */
    char secret_fname[50];  
    FILE *fptr_secret;   
    char extn_secret_file[MAX_FILE_SUFFIX]; 
    long size_secret_file;
} DecodeInfo;


/* Encoding function prototype */

/* Read and validate Decode args from argv */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);

/* Perform the decoding */
Status do_decoding(DecodeInfo *decInfo);

/* skip bmp image header*/
Status Skip_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image);

/* Store Magic String */
Status decode_magic_string(const char *magic_string, DecodeInfo *decInfo);

/* Decode secret file extenstion size */
Status decode_secret_file_extn_size(uint size, DecodeInfo *decInfo);

/* Decode secret file extenstion */
Status decode_secret_file_extn(const char *file_extn, DecodeInfo *decInfo);

/* Decode secret file size */
Status decode_secret_file_size(long file_size, DecodeInfo *decInfo);

/* Decode secret file data*/
Status decode_secret_file_data(DecodeInfo *decInfo);

/* Decode function, which does the real decoding */
Status decode_data_from_image(char *data, int size, FILE *fptr_src_image, FILE *fptr_stego_image);

/* Decode a byte from LSB of image data array */
Status decode_byte_from_lsb(char data, char *image_buffer);

/* Decode a 4 byte from LSB of image data array */
Status decode_size_from_lsb(uint size, char *image_buffer);

#endif

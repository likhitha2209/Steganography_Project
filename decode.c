#include <stdio.h>
#include "decode.h"
#include "types.h"
#include "common.h"
#include <string.h>

Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)
{
    /* validate the input file name */
    printf("validating the decoding arguments!!!\n");

    
    if(argv[2] == NULL)
    {
        printf("Error -> Soure file name not found.\n");
        return e_failure;
    }
    else if(strstr(argv[2], ".bmp") == NULL)
    {
        printf("Error -> Source image must have a .bmp extension.\n");
        return e_failure;
    }
    else if(strlen(argv[2]) < 4 || strcmp(argv[2] + strlen(argv[2]) - 4, ".bmp") != 0)
    {
        printf("Error -> Invalid source image name.\n");
        return e_failure;
    }
    
    strcpy(decInfo -> steg_img_fname , argv[2]);

    decInfo -> fptr_steg_img = fopen(decInfo ->steg_img_fname , "rb");
    if(decInfo ->fptr_steg_img == NULL)
    {
        printf("Error -> Unable to open input image.\n");
        return e_failure;
    }


    /*validate output file name */
    if(argv[3] != NULL)
    {
        //strcpy(decInfo ->output_img_fname , argv[3]);
        char *token = strtok(argv[3], ".");
        strcpy(decInfo->output_fname, token);
    }
    else
    {
        printf("output file name not found. so it's assigned as 'default'\n");
        strcpy(decInfo ->output_fname , "default");
    }

    printf("Arguments validation completed!!!");

    return e_success;
}

Status do_decoding(char *argv[],DecodeInfo *decInfo)
{

    printf("DECODING STARTED...\n");

    /* skip bmp header*/
    if(Skip_bmp_header(decInfo ->fptr_steg_img) != e_success)
    {
        printf("Error -> Failed to skip the 54 bytes of bmp header...\n");
        return e_failure;
    }

    /* decode magic string*/
    if(decode_magic_string(MAGIC_STRING,decInfo) == e_failure)
    {
        printf("Error -> Failed to decode magic string...\n");
        return e_failure;
    }

    /* decode secret file extension size */
    if(decode_secret_file_extn_size(decInfo) == e_failure)
    {
        return e_failure;
    }

    /* decode secret file extension*/
    if(decode_secret_file_extn(decInfo) == e_failure)
    {
        return e_failure;
    }

    /* decode secret file size */
    if(decode_secret_file_size(decInfo) == e_failure)
    {
        return e_failure;
    }

    /* create output filename*/
    strcat(decInfo->output_fname , decInfo->extn_output_file);

    /* open output file */
    decInfo->fptr_output = fopen(decInfo->output_fname, "wb");
    if(decInfo->fptr_output ==NULL)
    {
        printf("Error -> Unable to create output file.\n");
        return e_failure;
    }

    /* decode secret file data*/
    if(decode_secret_file_data(decInfo) == e_failure)
    {
        fclose(decInfo->fptr_output);
        return e_failure;
    }

    fclose(decInfo->fptr_output);
    fclose(decInfo->fptr_steg_img);

    return e_success;

}

Status Skip_bmp_header(FILE *fptr_src_image)
{
    fseek(fptr_src_image,54,SEEK_SET);
    printf("skipped %ld bytes of bmp header!!!...\n",ftell(fptr_src_image));
    return e_success;
}

Status decode_magic_string(const char *magic_string, DecodeInfo *decInfo)
{

    printf("Fetching the magic string...\n");

    char buff[strlen(magic_string) + 1];    

    decode_data_from_image(buff , strlen(magic_string) , decInfo->fptr_steg_img);
    
    buff[strlen(magic_string)] = '\0';

    if(strcmp(buff, magic_string) == 0)
    {
        printf("Magic string decoded successfully!\n");
        return e_success;
    }
    else
    {
        printf("Magic string doesn't match!\n");
        return e_failure;
    }

}

Status decode_byte_from_lsb(char *data, char *image_buffer)
{
    char buff =0;
    int i;

    for(i = 0; i < 8 ; i++)
    {
        buff = buff << 1;
        buff = buff | (image_buffer[i] & 1);
    }
    *data = buff;

    return e_success;
}

Status decode_secret_file_extn_size(DecodeInfo *decInfo)
{
    printf("Fetching secret file extension size...\n");

    char buff[32];

    /* read 32 bytes from steg image*/
    fread(buff, 1, 32, decInfo->fptr_steg_img);

    /* Decode 4-byte size from LSB */
    decInfo ->size_output_file = decode_size_from_lsb(buff);

    printf("Decoded extension size = %ld\n", decInfo ->size_output_file);

    return e_success;
}

Status decode_secret_file_extn(DecodeInfo *decInfo)
{
    printf("Fetching secret file extension...\n");

    char buff[8];
    char data;

    int i;
    for(i=0;i<decInfo->size_output_file;i++)
    {
        fread(buff,1,8,decInfo->fptr_steg_img);
        decode_byte_from_lsb(&data,buff);
        decInfo ->extn_output_file[i] = data;
    }
    decInfo ->extn_output_file[i] = '\0';

    printf("Secret file extension decoded : %s\n",decInfo->extn_output_file);

    return e_success;
}

Status decode_secret_file_size(DecodeInfo *decInfo)
{
    printf("Fetching secret file size...\n");

    char buff[32];

    fread(buff,1,32,decInfo->fptr_steg_img);
    decInfo->size_output_file = decode_size_from_lsb(buff);

    printf("Secret file size = %ld bytes\n", decInfo->size_output_file);

    return e_success;
}

Status decode_secret_file_data(DecodeInfo *decInfo)
{

    printf("Fetching secret file data...\n");

    char buff[8];
    char data;
    int i;

    for(i=0;i<decInfo->size_output_file;i++)
    {
        fread(buff,1,8,decInfo->fptr_steg_img);
        decode_byte_from_lsb(&data,buff);
        fwrite(&data,1,1,decInfo->fptr_output);
    }

    printf("Secret file data decoded successfully!!!...\n");

    return e_success;
}

Status decode_data_from_image(char *data, int size,FILE *fptr_steg_img)
{
    char buff[8];
    int i;

    for(i = 0 ; i < size; i++)
    {
        fread(buff,1,8,fptr_steg_img);
        decode_byte_from_lsb(&data[i],buff);
    }

    return e_success;
}


uint decode_size_from_lsb(char *image_buffer)
{
    /* loop for 32 times in seq manner */
    /* extract LSB of image_buffer[] */
    /* store the extracted bit in size */
    uint size=0;

    int i;
    for(i=0;i<32;i++)
    {
        size = size<<1;
        size = size| (image_buffer[i] & 1);
    }

  
    return size;
}
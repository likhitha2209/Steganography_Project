#include <stdio.h>
#include "encode.h"
#include "types.h"
#include "common.h"

/* Function Definitions */

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);
    printf("width = %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
    printf("height = %u\n", height);

    // Return image capacity
    return width * height * 3;
}

/* 
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */


Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)
{
    /*  check the agrv[2] must be having ".bmp" extn and last 4 char should be ".bmp"
        if yes,store file name into str mem (encInfo->source_image_fname = argv[2])
        if not, return e_failure
    */
    if(strstr(argv[2], ".bmp") == NULL)
    {
        printf("Invalid!! The 2nd argument should consists of '.bmp'\n");
        return e_failure;
    }
    else if(strlen(argv[2]) < 4 || strcmp(argv[2] + strlen(argv[2]) - 4, ".bmp") != 0)
    {
        printf("Invalid!! The 2nd argument should end with '.bmp'\n");
        return e_failure;
    }
    else
    {
        encInfo->src_image_fname = argv[2];
        return e_success;
    }

    /*  check if secret file the file is existing or not (open file in read mode argv[2])
        encInfo->fptr_src_image=fopen()
        if file not present return null
    */
    encInfo->fptr_src_image=fopen(argv[2],"r");
    if(encInfo -> fptr_src_image == NULL)
    {
        printf("Invalid!! Source file not present... ");
        return e_failure;
    }

    /*
        check the argv[3] is a valid file (is the file present or not)
        encInfo->secret_fname=argv[3]
        if present, store in file pointer 
            encInfo->fptr_secret = fopen()
        if not return e_failure
    */
    encInfo -> secret_fname = argv[3];
    encInfo ->fptr_secret = fopen(argv[3],"r");
    if(encInfo -> fptr_secret == NULL)
    {
        fclose(encInfo -> fptr_src_image);
        encInfo -> fptr_src_image = NULL;
        printf("Invalid!! Secret file not present... ");
        return e_failure;
    }

    /*
        check argv[4] is null or not
        if not NULL,  
            encInfo->stego_image_fname=argv[4]
        if NULL, encInfo->stego_image_fname="default.bmp"
    */
    if(argv[4] == NULL)
    {
        encInfo->stego_image_fname="default.bmp";
        return e_success;
    }
    else
    {
    char *dot = strstr(argv[4], ".bmp");
    if(dot == NULL)
    {
        printf("Invalid!! The 4nd argument should consists of '.bmp'\n");
        return e_failure;
    }
    else if(strlen(argv[4]) < 4 || strcmp(argv[4] + strlen(argv[4]) - 4, ".bmp") != 0)
    {
        printf("Invalid!! The 4nd argument should end with '.bmp'\n");
        return e_failure;
    }
        encInfo->stego_image_fname=argv[4];
        return e_success;
    }

    /*
        check if the file extensiton is of .bmp 
        if not return e_failure
        open the file in 'w' mode, and store the fptr to th str mem. 
    */
    encInfo -> fptr_stego_image = fopen(encInfo -> stego_image_fname,"w");
    if(encInfo -> fptr_stego_image == NULL)
    {
        fclose(encInfo -> fptr_src_image);
        fclose(encInfo -> fptr_secret);

        encInfo ->fptr_src_image = NULL;
        encInfo -> fptr_secret = NULL;

        return e_failure;
    }
    return e_success;
}

Status do_encoding(EncodeInfo *encInfo)
{
    /*
        check_capacity(encInfo) == e_failure
        print error msg and retun e_failure
    */
    if(check_capacity(encInfo) == e_failure)
    {
        printf("");
        return e_failure;
    }

    /*
        copy_bmp_header(ecoInfo -> fptr_src_image,ecoInfo -> fptr_stego_image) == failure
        printf error messsage and return e failure
    */
    if(copy_bmp_header(encInfo -> fptr_src_image , encInfo -> fptr_stego_image) == e_failure)
    {
        printf("");
        return e_failure;
    }

    /*
        encode_magic_string(MAGIC_STRING,encInfo)== e_failure
        printf error messsage and return e failure
    */
    //char *str = MAGIC_STRING;
    if(encode_magic_string(MAGIC_STRING,encInfo) == e_failure)
    {
        printf("");
        return e_failure;
    }

    /*
        char *dot = strchr(argv[3],'.')
        strcpy(encInfo->extn_secret_file,dot)
        encode_secret_file_extn_size(strlen(encInfo->extn_secret_file), encInfo)== e_failure
        printf error messsage and return e failure
    */

    char *dot = strchr(argv[3],'.');
    strcpy(encInfo->extn_secret_file,dot);
    if(encode_secret_file_extn_size(strlen(encInfo->extn_secret_file), encInfo) == e_failure)
    {
        printf("");
        return e_failure;
    }

    /*
        encode_secret_file_extn( encInfo->extn_secret_file,encInfo) == e_failure
        printf error messsage and return e failure
    */
    if(encode_secret_file_extn( encInfo->extn_secret_file,encInfo) == e_failure)
    {
        printf("");
        return e_failure;
    }

    /*
        encode_secret_file_extn( encInfo->extn_secret_file,encInfo) == e_failure
        printf error messsage and return e failure
    */
    if(encode_secret_file_extn( encInfo->extn_secret_file,encInfo) == e_failure)
    {
        printf("");
        return e_failure;
    }

    /*
        encode_secret_file_extn( encInfo->extn_secret_file,encInfo) == e_failure
        printf error messsage and return e failure
    */
    if(encode_secret_file_extn( encInfo->extn_secret_file,encInfo) == e_failure)
    {
        printf("");
        return e_failure;
    }

    /*
        encode_secret_file_size(encInfo->size_secret_file,encInfo)== e_failure
        printf error messsage and return e failure
    */
    if(encode_secret_file_size(encInfo->size_secret_file,encInfo)== e_failure)
    {
        printf("");
        return e_failure;
    }

    /*
        encode_secret_file_data(encInfo)== e_failure
        printf error messsage and return e failure
    */
    if(encode_secret_file_data(encInfo)== e_failure)
    {
        printf("");
        return e_failure;
    }

    /*
        copy_remaining_img_data(ecoInfo -> fptr_src_image,ecoInfo -> fptr_stego_image)== e_failure
        printf error messsage and return e failure
    */
    if(copy_remaining_img_data(encInfo -> fptr_src_image, encInfo -> fptr_stego_image)== e_failure)
    {
        printf("");
        return e_failure;
    }

}

Status check_capacity(EncodeInfo *encInfo)
{
    /*

    */
}

uint get_file_size(FILE *fptr)
{
    /*
        
    */
}

Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{
    /*
        rewind(),src imae and sct image
        declare a buff of 54 bytes, char buff[54]
        read 54bytes from src image to buff(use fread)
        write 54 bytes from buff to dest image(use fwrite)
    */
}

Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo)
{
    /*
        declare buff of 8 bytes

        to perfrom 2 bytes loop for length of magic string[index] times
        read 8 bytes from src_image into buff
        encode_byte_to_lsb(magic_string[index],buff)
        write 8 bytes of encodded data from buff to dest_image

        return e_success
    */
}

Status encode_byte_to_lsb(char data, char *image_buffer)
{
    /*
        loop for 8 times in seq manner
        check nth bit is set or not 
        if set, 
            set lsb of image_buffer[]
        if clear,
            clear lsb of image_buffer[]
        return e_success

        
    */
}

Status encode_secret_file_extn_size(uint size, EncodeInfo *encInfo)
{
    /*
        declar buff of 32 bytes


        read 32 bytes from src_image
        encode_size_to_lsb(size,buff)
        write buff to dest image
        return e_success
    */
}

Status encode_size_to_lsb(uint size, char *image_buffer)
{
    /*
        loop for 32 times in seq manner
        check nth bit is set or not 
        if set, 
            set lsb of image_buffer[]
        if clear,
            clear lsb of image_buffer[]
        return e_success

        
    */
}

Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo)
{
    /*
        declare buff of 8 bytes

        to perfrom 2 bytes loop for length of file_extn[index] times
        read 8 bytes from src_image into buff
        encode_byte_to_lsb(file_extn[index],buff)
        write 8 bytes of encodded data from buff to dest_image

        return e_success
    */
}

Status encode_secret_file_size(long file_size, EncodeInfo *encInfo)
{
    /*
        declar buff of 32 bytes


        read 32 bytes from src_image
        encode_size_to_lsb(size,buff)
        write buff to dest image
        return e_success
    */
}

Status encode_secret_file_data(EncodeInfo *encInfo)
{
    /*
        declare buff of 8 bytes char buff[8], data;

        to perfrom 2 bytes loop for length of secret_file_size[index] times
        read 8 bytes from secret into buff
        read char from secret_file data
        encode_byte_to_lsb(data,buff)
        write 8 bytes of encodded data from buff to dest_image

        return e_success
    */
}

Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)
{
    /*
        run loop for till EOF
        get/read 1 byte from src img and write to dset image
    */
}
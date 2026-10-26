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
    if(argv[2] == NULL)
    {
        printf("Error: Soure file name not found.\n");
        return e_failure;
    }
    else if(strstr(argv[2], ".bmp") == NULL)
    {
        printf("Error: Source image must have a .bmp extension.\n");
        return e_failure;
    }
    else if(strlen(argv[2]) < 4 || strcmp(argv[2] + strlen(argv[2]) - 4, ".bmp") != 0)
    {
        printf("Error: Invalid source image name.\n");
        return e_failure;
    }
    else
    {
        encInfo->src_image_fname = argv[2];
    }

    /*  check if source file is existing or not (open file in read mode argv[2])
        encInfo->fptr_src_image=fopen()
        if file not present return null
    */
    encInfo->fptr_src_image=fopen(argv[2],"rb");
    if(encInfo -> fptr_src_image == NULL)
    {
        printf("Error: Unable to open Source image.\n");
        return e_failure;
    }

    /*
        check the argv[3] is a valid file (is the file present or not)
        encInfo->secret_fname=argv[3]
        if present, store in file pointer 
            encInfo->fptr_secret = fopen()
        if not return e_failure
    */
    if (argv[3] == NULL)
    {
        printf("Error: Secret file name not found.\n");
        fclose(encInfo->fptr_src_image);
        encInfo->fptr_src_image = NULL;
        return e_failure;
    }

    encInfo -> secret_fname = argv[3];
    encInfo ->fptr_secret = fopen(argv[3],"rb");
    if(encInfo -> fptr_secret == NULL)
    {
        fclose(encInfo -> fptr_src_image);
        encInfo -> fptr_src_image = NULL;
        printf("Error: Secret file not present.\n");
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
    }
    else if(strstr(argv[4], ".bmp") == NULL)    
    {
        printf("Error: Output image must have a .bmp extension.\n");
        fclose(encInfo->fptr_src_image);
        fclose(encInfo->fptr_secret);

        encInfo->fptr_src_image = NULL;
        encInfo->fptr_secret = NULL;
        return e_failure;
    }
    else if(strlen(argv[4]) < 4 || strcmp(argv[4] + strlen(argv[4]) - 4, ".bmp") != 0)
    {
        printf("Error: Invalid output image name.\n");
        fclose(encInfo->fptr_src_image);
        fclose(encInfo->fptr_secret);

        encInfo->fptr_src_image = NULL;
        encInfo->fptr_secret = NULL;
        return e_failure;
    }
    else
    {
        encInfo->stego_image_fname=argv[4];
    }

    /*
        check if the file extensiton is of .bmp 
        if not return e_failure
        open the file in 'w' mode, and store the fptr to th str mem. 
    */
    encInfo -> fptr_stego_image = fopen(encInfo -> stego_image_fname,"wb");
    if(encInfo -> fptr_stego_image == NULL)
    {
        fclose(encInfo -> fptr_src_image);
        fclose(encInfo -> fptr_secret);

        encInfo ->fptr_src_image = NULL;
        encInfo -> fptr_secret = NULL;
        printf("Error: Unable to create output image file.\n");
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
        printf("Error: Insufficient capacity in source image to encode secret file.\n");
        return e_failure;
    }

    /*
        copy_bmp_header(ecoInfo -> fptr_src_image,ecoInfo -> fptr_stego_image) == failure
        printf error messsage and return e failure
    */
    if(copy_bmp_header(encInfo -> fptr_src_image , encInfo -> fptr_stego_image) == e_failure)
    {
        printf("Error: Failed to copy BMP header to output image.\n");
        return e_failure;
    }

    /*
        encode_magic_string(MAGIC_STRING,encInfo)== e_failure
        printf error messsage and return e failure
    */
    //char *str = MAGIC_STRING;
    if(encode_magic_string(MAGIC_STRING,encInfo) == e_failure)
    {
        printf("Error: Failed to encode magic string.\n");
        return e_failure;
    }

    /*
        char *dot = strchr(argv[3],'.')
        strcpy(encInfo->extn_secret_file,dot)
        encode_secret_file_extn_size(strlen(encInfo->extn_secret_file), encInfo)== e_failure
        printf error messsage and return e failure
    */

    char *dot = strrchr(encInfo -> secret_fname,'.');
    if (dot == NULL)
    {
        printf("Error: Secret file extension not found.\n");
        return e_failure;
    }
    strcpy(encInfo->extn_secret_file,dot);

    if(encode_secret_file_extn_size(strlen(encInfo->extn_secret_file), encInfo) == e_failure)
    {
        printf("Error: Failed to encode secret file extension size.\n");
        return e_failure;
    }

    /*
        encode_secret_file_extn( encInfo->extn_secret_file,encInfo) == e_failure
        printf error messsage and return e failure
    */
    if(encode_secret_file_extn( encInfo->extn_secret_file,encInfo) == e_failure)
    {
        printf("Error: Failed to encode secret file extension.\n");
        return e_failure;
    }
    /*
        encode_secret_file_size(encInfo->size_secret_file,encInfo)== e_failure
        printf error messsage and return e failure
    */
    if(encode_secret_file_size(encInfo->size_secret_file,encInfo)== e_failure)
    {
        printf("Error: Failed to encode secret file size.\n");
        return e_failure;
    }

    /*
        encode_secret_file_data(encInfo)== e_failure
        printf error messsage and return e failure
    */
    if(encode_secret_file_data(encInfo)== e_failure)
    {
        printf("Error: Failed to encode secret file data.\n");
        return e_failure;
    }

    /*
        copy_remaining_img_data(ecoInfo -> fptr_src_image,ecoInfo -> fptr_stego_image)== e_failure
        printf error messsage and return e failure
    */
    if(copy_remaining_img_data(encInfo -> fptr_src_image, encInfo -> fptr_stego_image)== e_failure)
    {
        printf("Error: Failed to copy remaining image data to output image.\n");
        return e_failure;
    }
    return e_success;

}

Status check_capacity(EncodeInfo *encInfo)
{
    /*
        encoIncFo ->size_secret_file = get_file_size(encoInfo -> fptr_secret)
        get_image_size_for_bmp(encoInfo -> fptr_src_image) > (14*8) 
            return e_success
        return e_failure
    */
    encInfo -> size_secret_file = get_file_size(encInfo -> fptr_secret);
    if(get_image_size_for_bmp(encInfo -> fptr_src_image) > (get_file_size(encInfo -> fptr_secret) +14)*8 )
    {
        return e_success;
    }
    else
    {
        return e_failure;
    }

}

uint get_file_size(FILE *fptr)
{
    /*
        fseek() to SEEK_END
        return ftell()
    */
    fseek(fptr,0,SEEK_END);
    return ftell(fptr);
}

Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{
   /* rewind(),src imae and sct image */
    rewind(fptr_src_image);
    rewind(fptr_dest_image);

    /* declare a buff of 54 bytes, char buff[54] */
    char buff[54];

    /* read 54bytes from src image to buff(use fread) */
    fread(buff,54,1,fptr_src_image);

    /* write 54 bytes from buff to dest image(use fwrite) */
    fwrite(buff,54,1,fptr_dest_image);

    return e_success;
}

Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo)
{
    /* declare buff of 8 bytes */
    char buff[8];

    /* loop for length of magic string[index] times */
    /* read 8 bytes from src_image into buff */
    /* encode_byte_to_lsb(magic_string[index],buff) */
    /* write 8 bytes of encodded data from buff to dest_image */
    int i;
    for(i = 0; i < strlen(magic_string); i++)
    {
        fread(buff,8,1,encInfo->src_image_fname);
        encode_byte_to_lsb(magic_string[i],buff);
        fwrite(buff,8,1,encInfo->fptr_stego_image);
    }

    /* return e_success*/
    return e_success;
}

Status encode_byte_to_lsb(char data, char *image_buffer)
{
    /* loop for 8 times in seq manner */
    /* check nth bit is set or not */
    /* if set, set lsb of image_buffer[] */
    /* if clear, clear lsb of image_buffer[] */
    int i, mask=1;
    for(i = 0; i < 8; i++)
    {
        mask=1 << (7 - i);
        if(data & mask)
        {
            image_buffer[i] = image_buffer[i] | 1;
        }
        else
        {
            image_buffer[i] = image_buffer[i] & (~1);
        }

    }

    /* return e_success*/
    return e_success;
}

Status encode_secret_file_extn_size(uint size, EncodeInfo *encInfo)
{
    /* declar buff of 32 bytes */
    char buff[32];

    /* read 32 bytes from src_image */
    fread(buff,32,1,encInfo->fptr_src_image);

    /* encode_size_to_lsb(size,buff) */
    encode_size_to_lsb(size,buff);

    /* write buff to dest image */
    fwrite(buff,32,1,encInfo->fptr_stego_image);

    /* return e_success*/
    return e_success;
}

Status encode_size_to_lsb(uint size, char *image_buffer)
{
    /* loop for 32 times in seq manner */
    /* check nth bit is set or not */
    /* if set, set lsb of image_buffer[] */
    /* if clear, clear lsb of image_buffer[] */
    int i,mask=1;
    for(i=0;i<32;i++)
    {
        mask = 1<<(31 - i);
        if(size & mask)
        {
            image_buffer[i] = image_buffer[i] | 1;
        }
        else
        {
            image_buffer[i] = image_buffer[i] & (~1);
        }
    }

    /* return e_success*/
    return e_success;    
}

Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo)
{
    /* declare buff of 8 bytes */
    char buff[8];

    /* loop for length of file_extn[index] times */
    /* read 8 bytes from src_image into buff */
    /* encode_byte_to_lsb(file_extn[index],buff) */
    /* write 8 bytes of encodded data from buff to dest_image */
    int i;
    for(i=0;i< strlen(file_extn);i++)
    {
        fread(buff,8,1,encInfo ->fptr_src_image);
        encode_byte_to_lsb(file_extn[i],buff);
        fwrite(buff,8,1,encInfo->fptr_stego_image);

    }

    /* return e_success*/
    return e_success;
}

Status encode_secret_file_size(long file_size, EncodeInfo *encInfo)
{
    /* declar buff of 32 bytes */
    char buff[32];

    /* read 32 bytes from src_image */
    fread(buff,32,1,encInfo -> fptr_src_image);

    /* encode_size_to_lsb(size,buff) */
    encode_size_to_lsb(file_size,buff);

    /* write buff to dest image */
    fwrite(buff,32,1,encInfo ->fptr_stego_image);
    
    /* return e_success*/
    return e_success;
}

Status encode_secret_file_data(EncodeInfo *encInfo)
{
    /* declare buff of 8 bytes char buff[8], data */
    char buff[8];
    char data;

    /* loop for length of secret_file_size[index] times */
    /* read 8 bytes from secret into buff */
    /* read char from secret_file data */
    /* encode_byte_to_lsb(data,buff) */
    /* write 8 bytes of encodded data from buff to dest_image */
    int i;
    if(data == EOF)
    {
        return e_failure;
    }
    for(i=0;i<encInfo->size_secret_file;i++)
    {
        fread(buff,8,1,encInfo->fptr_secret);
        data = fgetc(encInfo->fptr_secret);
        encode_byte_to_lsb(data,buff);
        fwrite(buff,8,1,encInfo->fptr_stego_image);
    }

    /* return e_success*/
    return e_success;
}

Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)
{
    /*
        run loop for till EOF
        get/read 1 byte from src img and write to dset image
    */
    char ch;
    while ((ch = fgetc(fptr_src)) != EOF)
    {
        if (fputc(ch, fptr_dest) == EOF)
        {
            return e_failure;
        }
    }
    if (ferror(fptr_src))
    {
        return e_failure;
    }

    /* return e_success*/
    return e_success;
}
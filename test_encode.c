#include <stdio.h>
#include "encode.h"
#include "types.h"
#include "decode.h"

int main(int argc, char *argv[])
{
    EncodeInfo encInfo;
    DecodeInfo decInfo;
    /* call check_operation_type(argv[1][1]== e_encode)
        //start operation
        read_and_validate_encode_args(argv, &encInfo)
    */
   if(check_operation_type(argv[1][1]) == e_encode)
   {
        if (argc < 4 || argc>5)
        {
            printf("Invalid Input!!!\n");
            printf("Use: ./a.out -e source.bmp secret.txt steg.bmp\n");
            return 0;
        }
        if(read_and_validate_encode_args(argv, &encInfo) == e_success)
        {
            if (do_encoding(&encInfo) == e_success)
            {
                printf("Encoding completed successfully!!!\n");
            }
            else
            {
                printf("Invalid!!! Encoding failed!!!\n");
            }
        }
        else
        {
            printf("Invalid encoding arguments.\n");
        }
   }
   else if(check_operation_type(argv[1][1]) == e_decode)
   {
        if(argc < 3 || argc > 4)
        {
            printf("Invalid input!!!\n");
            printf("Use: ./a.out -e steg.bmp secret.txt\n");
            return 0;
        }
        if(read_and_validate_decode_args(argv, &decInfo) == e_success)
        {
            if (do_decoding(&decInfo) == e_success)
            {
                printf("Decoding completed successfully!!!\n");
            }
            else
            {
                printf("Invalid!!! Decoding failed!!!\n");
            }
        }
        else
        {
            printf("Invalid decoding arguments.\n");
        }
   }
   else
   {
        printf("Invalid input!!!... \nEntered operation type is invalid.\n");
   }
    return 0;
}

OperationType check_operation_type(char opt)
{
    /*
    if opt is e
        return e_encode
    else if opt is d
        return e_decode
    else
        return e_unsupported
    */
   if(opt == 'e')
   {
        printf("---------------------- ENCODING STARTED ----------------------\n");
        return e_encode;
   }
   else if(opt == 'd')
   {
        printf("---------------------- DECODING STARTED ----------------------\n");
        return e_decode;
   }

   else
   {
        printf("Invalid Input!!!\n");

        return e_unsupported;
   }
}

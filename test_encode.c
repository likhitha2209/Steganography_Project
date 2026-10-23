#include <stdio.h>
#include "encode.h"
#include "types.h"

int main(int argc, char *argv[])
{
    EncodeInfo encInfo;
    
    /* call check_operation_type(argv[1][1]== e_encode)
        //start operation
        read_and_validate_encode_args(argv, &encInfo)
    */
   if(check_operation_type(argv[1][1]) == e_encode);
   {
        read_and_validate_encode_args(argv, &encInfo);
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
        return e_encode;
   }
   else if(opt == 'd')
   {
        return e_decode;
   }

   else
   {
        return e_unsupported;
   }
}

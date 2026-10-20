#include <stdio.h>
#include "encode.h"
#include "types.h"

int main()
{
    EncodeInfo encInfo;
    
    /* call check_operation_type(arg[1][1]== e_encode)
        //start operation
        read_and_validate_encode_args(argv, &encInfo)
    */
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
}

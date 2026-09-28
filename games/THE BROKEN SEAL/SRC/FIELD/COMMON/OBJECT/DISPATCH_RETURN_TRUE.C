#include "TYPES.H"

/* A routine that only reports success, after the object dispatcher;
   nothing in the image calls it by name. */
s32 ObjectDispatch_ReturnTrue(void)
{
    return 1;
}

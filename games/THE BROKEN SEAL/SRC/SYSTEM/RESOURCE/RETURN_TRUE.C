#include "TYPES.H"

/* A routine that only reports success, after the fixed resource block
   loader; nothing in the image calls it by name. */
s32 Resource_ReturnTrue(void)
{
    return 1;
}

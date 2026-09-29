#include "TYPES.H"

/* A routine that only reports success; the window far-call table reaches
   it through its stub. */
s32 Item_ReturnTrue(void)
{
    return 1;
}

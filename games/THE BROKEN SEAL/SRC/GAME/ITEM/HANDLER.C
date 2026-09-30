#include "TYPES.H"

#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

s32 PartyInventory_RemoveFar(s32);

s32 Item_CallHandler48(s32 arg0, s32 arg1)
{
    PartyInventory_RemoveFar(arg1);
    return 0;
}

/* A routine that only reports success; the window far-call table reaches
   it through its stub. */
s32 Item_ReturnTrue(void)
{
    return 1;
}

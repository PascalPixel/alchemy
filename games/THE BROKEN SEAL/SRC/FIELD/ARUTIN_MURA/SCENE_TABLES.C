#include "TYPES.H"

/* The table accessors between the scene-id selectors at the head of the
   overlay. */

s32 SceneData_ReturnZero(void)
{
    return 0;
}

/*
 * The owner at 0x02000074 is eight bytes and includes its one pool word at
 * 0x02000078: the pc-relative load reads that word, so the word belongs to
 * this owner. The word is an address returned without being dereferenced.
 * Many getters share this body, but each returns a different address.
 */
u8 *SceneData_GetTable9390(void)
{
    return (u8 *)0x02009390;
}

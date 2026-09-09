/* Contiguous unnamed leaf-owner run for resource_3b7. */

#include "types.h"

/*
 * resource_3b7 owner at 0x0200003c: a leaf that loads its literal pool word
 * and returns it. The eight-byte owner includes that pool word at
 * 0x02000040, holding the address 0x02009ac8 -- image offset 0x1ac8 --
 * returned without being dereferenced. Many rows across the tree share this
 * body but each returns a different address.
 */

u8 *SceneData_GetTable9ac8(void)
{
    return (u8 *)0x02009ac8;
}

#include "types.h"

extern s16 Data_02000240[];
extern u8 Value_000000bd;
extern u8 Data_02009aec[];
extern u8 Data_02009cfc[];

s32 SceneData_SelectTable9cfcByState(void)
{
    if (Data_02000240[224] == (s32)&Value_000000bd) {
        return (s32)Data_02009aec;
    }
    return (s32)Data_02009cfc;
}

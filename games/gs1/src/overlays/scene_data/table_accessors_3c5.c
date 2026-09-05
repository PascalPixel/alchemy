/* Contiguous unnamed leaf-owner run for resource_3c5. */

#include "types.h"

extern s16 Data_02000240[];
extern u8 Value_000000b0;
extern u8 Data_0200b5b8[];

s32 SceneData_SelectTableB5b8ByState(void) {
    if (Data_02000240[224] == (s32)&Value_000000b0) {
        return (s32)Data_0200b5b8;
    }
    return 0;
}

/* Return this overlay's state block. */
#include "types.h"

u8 *SceneData_GetTableb5f8(void) { return (u8 *)0x0200b5f8; }

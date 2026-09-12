#include "fixed_math.h"
#include "types.h"
#include "scene.h"
#include "object_lookup.h"

/* battle/effects/objects/set_random_table_value.c */
extern s8 gRom[];

u32 Random16(void);
/* LCG: seed = seed * 0x41c64e6d + 0x3039, returns bits 8-23. */
#define Rand Random16

void BattleFx_SetRandomTableValueOnObject(s32 arg0)
{
    s8 *table = gRom;
    s32 index = Rand();
    Battle_Apply(arg0, table[(u32)(index * 8) >> 16]);
}

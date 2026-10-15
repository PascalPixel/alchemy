#include "types.h"

struct OwnerGrowthRecord {
    u8 unknown_00[0x50];
    s16 hp[6];
    s16 pp[6];
    u16 atk[6];
    u16 def[6];
    u16 agi[6];
    u8 luck[6];
    u8 unknown_92[0x22];
};

extern struct OwnerGrowthRecord Owner_GrowthRecords[8];

struct OwnerGrowthRecord *Character_GetDefinition(s32 index)
{
    s32 stride = sizeof(struct OwnerGrowthRecord);
    u8 *base;

    /* FAKEMATCH: construct the row width before the native table-base pool load. */
    asm("" : : "r"(stride));
    base = (u8 *)Owner_GrowthRecords;
    /* FAKEMATCH: keep both operands live before multiplying the row offset. */
    asm("" : "+r"(stride), "+r"(base));
    /* FAKEMATCH: the integer address sum retains the native multiply/add lifetimes; pointer addition changes six bytes. */
    return (struct OwnerGrowthRecord *)(index * stride + (u32)base);
}

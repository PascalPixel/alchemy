/* Draft only, 2026-10-01: level-helper6-growth-pointer-add.
 * Ordinary TLA compiler emits 16 bytes; 6 byte positions differ
 * from the complete English 16-byte owner, including alignment and pool.
 * Reference owner: games/THE LOST AGE/SRC/GAME/CHARACTER/GROWTH.C.
 * No adoption; the complete native owner still differs.
 * Both supporting data tables remain uncredited raw scaffold.
 */
#include "TYPES.H"

/* Character growth at levels 0, 20, 40, 60, 80 and 100. */
struct OwnerGrowthRecord {
    u8 unknown_00[0x50];
    s16 hp[6];
    s16 pp[6];
    u16 attack[6];
    u16 defense[6];
    u16 agility[6];
    u8 luck[6];
    u8 unknown_92[0x22];
};

extern struct OwnerGrowthRecord Owner_GrowthRecords[8];

struct OwnerGrowthRecord *Owner_GetRecordStride180(s32 index)
{
    /* FAKEMATCH: keep the row width in the native multiply operand register. */
    register s32 stride asm("r3") = sizeof(struct OwnerGrowthRecord);
    /* FAKEMATCH: keep the table base in r2 while the row offset remains in r0. */
    register u8 *base asm("r2");

    /* FAKEMATCH: construct the row width before the native table-base pool load. */
    asm("" : : "r"(stride));
    base = (u8 *)Owner_GrowthRecords;
    /* FAKEMATCH: keep both operands live before multiplying the row offset. */
    asm("" : "+r"(stride), "+r"(base));
    return (struct OwnerGrowthRecord *)(base + index * stride);
}

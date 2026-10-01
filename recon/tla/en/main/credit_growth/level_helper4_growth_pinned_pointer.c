/* Draft only, 2026-10-01: level-helper4-growth-pinned-pointer.
 * Ordinary TLA compiler emits 16 bytes; 10 byte positions differ
 * from the complete English 16-byte owner, including alignment and pool.
 * Reference owner: games/THE LOST AGE/SRC/GAME/CHARACTER/GROWTH.C.
 * No adoption; the complete native owner still differs.
 * Both supporting data tables remain uncredited raw scaffold.
 */
#include "TYPES.H"
struct OwnerGrowthRecord { u8 bytes[180]; };
extern struct OwnerGrowthRecord Owner_GrowthRecords[8];
struct OwnerGrowthRecord *Owner_GetRecordStride180(s32 owner)
{
    /* FAKEMATCH: retain native row-width and table-base operand registers around the lifetime boundary. */
    register s32 stride asm("r3") = sizeof(struct OwnerGrowthRecord);
    /* FAKEMATCH: retain the native base in r2 while the owner offset stays in r0. */
    register u8 *base asm("r2") = (u8 *)Owner_GrowthRecords;
    /* FAKEMATCH: the native row-width move and table-base pool load both precede the stride multiply. */
    asm("" : "+r"(stride), "+r"(base));
    return (struct OwnerGrowthRecord *)(base + owner * stride);
}

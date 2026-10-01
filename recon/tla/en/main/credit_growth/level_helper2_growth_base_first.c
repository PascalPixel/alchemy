/* Draft only, 2026-10-01: level-helper2-growth-base-first.
 * Ordinary TLA compiler emits 16 bytes; 4 byte positions differ
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
    u8 *base = (u8 *)Owner_GrowthRecords;
    owner *= 180;
    return (struct OwnerGrowthRecord *)(base + owner);
}

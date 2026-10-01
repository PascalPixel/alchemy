/* Draft only, 2026-10-01: level-helper2-growth-integer.
 * Ordinary TLA compiler emits 16 bytes; 5 byte positions differ
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
    return (struct OwnerGrowthRecord *)(owner * 180 + (u32)Owner_GrowthRecords);
}

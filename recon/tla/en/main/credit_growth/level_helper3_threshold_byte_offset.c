/* Draft only, 2026-10-01: level-helper3-threshold-byte-offset.
 * Ordinary TLA compiler emits 72 bytes; 41 byte positions differ
 * from the complete English 76-byte owner, including alignment and pool.
 * Reference owner: recon/tla/raw/080af8d0.s.
 * No adoption; the complete native owner still differs.
 * Both supporting data tables remain uncredited raw scaffold.
 */
#include "TYPES.H"
struct OwnerCurveState {
    u8 unknown_000[0x129];
    u8 kind;
    u8 unknown_12a[0x20];
    u16 curve;
};
struct OwnerCurveState *Owner_GetState(s32 owner);
extern const u32 Owner_ExperienceThresholds[8 * 99];
u32 Owner_GetLevelThreshold(s32 owner, s32 level)
{
    struct OwnerCurveState *state = Owner_GetState(owner);
    u32 curve;
    if (state->kind) {
        if (level <= 0)
            return 0;
        if (level <= 99) {
            curve = state->curve;
            if (curve <= 7)
                return *(const u32 *)((const u8 *)Owner_ExperienceThresholds + (curve * 99 + level) * 4 - 4);
        }
    }
    return (u32)-1;
}

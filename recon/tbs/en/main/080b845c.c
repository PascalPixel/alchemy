/* DRAFT audit, 2026-10-03: BattleMotion_ProjectScaledPosition.
 * The complete owned-English-ROM extent is 100 bytes at 080b845c..080b84c0,
 * including the four-byte IWRAM multiplier literal. The raw listing reproduces
 * all 100 bytes; the prior MOTION3.C function scored 0 with its unused frame.
 * Regenerate the reference from roms/tbs-en.gba with a bounded Thumb decode
 * of that extent, continuing past the two fixed-entry calls that return via
 * ip. Name the five BL targets from the current linker map, and name the
 * literal IwramMulQ16ReturnIp. Assemble with the normal scaffold command,
 * resolve against the current build solely for comparison, and compare the
 * complete extent with the owned ROM. The verified demotion now transfers
 * this function to recon/raw in English and to scaffold in the other editions.
 *
 * The former production position[3] declaration was explicitly unused. Native code
 * subtracts/adds 12 from SP, but never addresses those slots or passes their
 * address. Render_ProjectPoint receives object->x and the caller's output.
 * Its IwramTransformVector reads three source words and writes three result
 * words through those pointers; neither it nor IwramMulQ16ReturnIp consumes
 * an implicit caller-frame object.
 *
 * Real consumers pass an EffectPosition: EffectPosition_ApplyStepAndYOffset
 * adjusts its y, and BattleFx_PlayUnitElementEffect consumes its x and y.
 * The adjacent BattleMotion_ProjectPosition really fills a three-word local
 * input before projection. That lifetime is absent from this function.
 *
 * Ordinary trials, all with the approved TBS compiler/options:
 * - Remove the unused declaration (retained): 100 bytes, score 300:
 *   one inserted alignment instruction and two deleted SP adjustments.
 * - Copy object x/y/z into a consumed local input vector: score 1040
 *   (2 operand, 8 inserted, 2 deleted); actual loads/stores are not native.
 * - Project into an EffectPosition and copy all three fields to the caller:
 *   score 1670 (9 register, 2 operand, 3 reordered, 12 inserted, 2 deleted).
 * - Copy the complete projected EffectPosition, adjust y, and assign it back:
 *   score 1285 (9 register, 3 operand, 6 reordered, 8 inserted).
 *
 * No real consumed local explains the reserved frame in these trials.
 * The unused declaration changes instruction presence, not just instruction
 * order or register choice. This audit does not establish an S2 justification.
 * The complete 100-byte function is uncredited in all six TBS editions.
 * The other MOTION3.C functions stay source; its two trailing helpers share
 * one named text section solely to preserve the demotion boundary.
 */
#include "TYPES.H"
#include "MOTION_OBJECT.H"
#include "IWRAM_CALL.H"
#include "ANIMSPR.H"

s32 Camera_ApplyTransformByFlag(void);
s32 Render_ProjectPoint(const s32 *, s32 *);
u32 Battle_GetObjectTableValue(s32);

s32 BattleMotion_ProjectScaledPosition(s32 id, s32 *projected)
{
    struct MotionObject *object = GetBattleObjectSlot(id)->object;
    struct AnimationObject *record = GetMotionRecord(object, 0);
    s32 scaled;
    s32 factor;

    Camera_ApplyTransformByFlag();
    scaled = Render_ProjectPoint(&object->x, projected);
    factor = Iwram_MulQ16(scaled, record->scale);
    scaled = Iwram_MulQ16(factor, (s32)Battle_GetObjectTableValue(id) >> 16);
    projected[1] -= scaled;
    return 0;
}

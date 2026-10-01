/* NONMATCHING: 2026-10-01 brief Wave2 CopyWords plain-source attempt.
 * Removing this one source device changes BattleFx_StepPaletteToResource.
 * Remaining difference: a direct call changes BattleFx_StepPaletteToResource from mov r6, r8 to mov r6, r9 (111/114 assembly lines).
 * Measured with the existing TBS agscc option set, EN edition; no option changes.
 * Production retains the measured helper with a body-local FAKEMATCH reason.
 */
#include "TYPES.H"
#include "IWRAM_CALL.H"
#include "RESOURCE.H"


/* Step every background colour one unit per channel towards the palette
   of resource_id; colour 0 is forced to black. */
void BattleFx_StepPaletteToResource(s32 resource_id)
{
    u16 target[64];
    u16 *color = (u16 *)0x05000000;
    s32 i;

    Iwram_CopyWords(target, Resource_GetTableEntry(resource_id), 128);
    target[0] = 0;
    for (i = 0; i != 64; i++, color++) {
        s32 red = *color & 0x1f;
        s32 green = (*color >> 5) & 0x1f;
        s32 blue = (*color >> 10) & 0x1f;
        s32 target_red = target[i] & 0x1f;
        s32 target_green = (target[i] >> 5) & 0x1f;
        s32 target_blue = (target[i] >> 10) & 0x1f;

        if (red < target_red)
            red++;
        else if (red > target_red)
            red--;
        if (green < target_green)
            green++;
        else if (green > target_green)
            green--;
        if (blue < target_blue)
            blue++;
        else if (blue > target_blue)
            blue--;
        target[i] = (blue << 10) | (green << 5) | red;
    }
    Iwram_CopyWords((u16 *)0x05000000, target, 128);
}

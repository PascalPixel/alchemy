/* Near miss, differ 2 of 62 (2026-10-12), from ☀️ BattleFx_SpawnItemBreakMode3.
 * One pair is left at the loop's bottom test: the listing loads the scale
 * between the two halves of 0xff00 (movs r2, #255; ldr r3, [r5, #24];
 * lsls r2, r2, #8), this draft after both. The two barriers below fix the
 * other two pairs; a while loop gives the same bottom test. */
#include "types.h"
#include "fixed_math.h"
#include "system.h"

void Audio_PlayCue(s32);
void *Object_Spawn(s32, s32, s32, s32);
void Object_SetMode(void *, s32);

void *Func_080ddfd4(s32 x, s32 y, s32 z, s32 angle)
{
    u8 *obj;
    s32 v;

    Audio_PlayCue(138);
    obj = Object_Spawn(278, x, y, z);
    if (obj != NULL) {
        *(s32 *)(obj + 0x18) = *(s32 *)(obj + 0x1C) = 0x4000;
        *(s32 *)(obj + 0x34) = *(s32 *)(obj + 0x30) = 0x30000;
        /* FAKEMATCH: ⚓️ stores both speeds before it loads the sprite; without the barrier the load is scheduled between them. */
        __asm__ volatile("");
        *(s8 *)(*(u8 **)(obj + 0x50) + 9) &= ~0xC;
        Object_SetMode(obj, 3);
        v = *(s32 *)(obj + 0x18);
        /* FAKEMATCH: ⚓️ loads the scale before it builds the limit; the plain test starts the constant first (one pair of instructions swapped). */
        __asm__ volatile("");
        if (v < 0x10000) {
            do {
                v += 0x800;
                *(s32 *)(obj + 0x1C) = v;
                *(s32 *)(obj + 0x18) = v;
                *(u16 *)(obj + 6) += 0x2000;
                WaitFrames(1);
                v = *(s32 *)(obj + 0x18);
            } while (v <= 0xFFFF);
        }
        *(u16 *)(obj + 6) = (u16)angle;
    }
    return obj;
}

#include "TYPES.H"
#include "HEYA.H"

/*
 * Fune room: a one-shot cue timer. While the timer word is nonzero it counts
 * down, and exactly at 70 remaining it fires the cue. Once it hits zero, when
 * the scene's timer condition is met the cue plays and the timer is rearmed
 * to 80.
 */

extern void Engine_WorkSetValuesIfNonNegative();
extern s32 Engine_RandomNext();
extern void Engine_AudioPlayCue();
extern s32 FuneHeya_CueTimer;

/* Same argument forwarding used by the byte-exact timer sibling. */

void Scene_UpdateCueTimer(s32 a0, s32 a1, s32 a2)
{
    s32 value;
    s32 *timer = &FuneHeya_CueTimer;
    s32 remaining;
    if (*timer != 0) {
        remaining = *timer - 1;
        *timer = *timer - 1;
        if (remaining == 70)
            Call3(Engine_WorkSetValuesIfNonNegative, -1, -1, 0xe666);
    } else {
        value = Engine_RandomNext();
        if (((u32)(((value << 4) - value) << 3) >> 16) == 0) {
            Engine_AudioPlayCue(181);
            Call3(Engine_WorkSetValuesIfNonNegative, 0x20000, 0x20000, 0x10000);
            *timer = 80;
        }
    }
}

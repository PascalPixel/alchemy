#include "YAMA.H"

/*
 * Stamp a fixed value into the caller's record at +102, then play sound cue
 * 288. The owner at 0x02003724 in resource_3a4 is 20 bytes with no literal
 * pool.
 */
void SceneState_SetRecordWord102AndPlayCue288(u16 *record)
{
    record = (u16 *)((char *)record + 102);
    {
        s32 value = 0x21;

        *record = value;
    }
    Audio_PlayCue(288);
}

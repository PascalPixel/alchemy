#include "types.h"

/*
 * Stamp a fixed value into the caller's record at +102, then play sound cue
 * 288. The owner at 0x02003724 in resource_3a4 is 20 bytes with no literal
 * pool.
 */

/* Plays a sound cue; the name is this site's own call word, not a runtime
 * address. */
void Func_02007452();

void SceneState_SetRecordWord102AndPlayCue288(u16 *record)
{
    record = (u16 *)((char *)record + 102);
    {
        s32 value = 0x21;

        *record = value;
    }
    Func_02007452(288);
}

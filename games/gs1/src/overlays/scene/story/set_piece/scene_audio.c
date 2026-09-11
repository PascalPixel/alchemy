#include "types.h"

#define SceneAudio_PlayCue183EverySixtyTicks Func_02002eec
#define SceneState_SetRecordWord102AndPlayCue288 Func_02003724
extern s32 Data_0200d1b0;   /* In-image writable data at image offset 0x51b0. */

void Func_02006c1e();
void Func_02007452();

/*
 * Overlay resource_3a4. Per-frame tick that counts to sixty, fires one
 * audio cue and rewinds itself. The address of this routine is stored in
 * a record as a plain word, so it runs as a published callback.
 */

/* Plays a sound cue; the name is this site's own call word, not a runtime
 * address. */

/*
 * The owner spans the code, one alignment halfword and its one pool word,
 * thirty-six bytes in all. Func_02006c1e names the loader-relocated call
 * word for the cue call, not a runtime address. The limit of sixty reads
 * as one second of frames, but nothing here fixes a frame rate.
 */
void SceneAudio_PlayCue183EverySixtyTicks(void)
{
    Data_0200d1b0 = Data_0200d1b0 + 1;
    if (Data_0200d1b0 == 60) {
        Func_02006c1e(183);
        Data_0200d1b0 = 0;
    }
}

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
    Func_02007452(288);
}

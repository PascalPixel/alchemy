#include "types.h"

/*
 * Overlay resource_3a4. Per-frame tick that counts to sixty, fires one
 * audio cue and rewinds itself. The address of this routine is stored in
 * a record as a plain word, so it runs as a published callback.
 */

extern s32 Data_0200d1b0;   /* In-image writable data at image offset 0x51b0. */

extern void Func_02006c1e();

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

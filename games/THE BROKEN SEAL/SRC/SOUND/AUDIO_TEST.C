/* Sound test: three cue numbers, one per row (a background track, a jingle
   and a song). Up and down pick the row; right and left step its cue by one
   and R and L by ten; A plays it, B plays cue 19 and START fades the music
   out. SELECT cycles the mixer preset and fades the second player. A clip in
   the mixer restarts the twenty-frame indicator timer. Never returns. */
#include "TYPES.H"
#include "SYSTEM.H"

extern volatile u32 gKeysRepeat;
extern s32 gSoundClipFlag;
extern s32 Data_03007804;

s32 Sound_LoadPresetParameters(s32 preset);
void AudioCommand_Play(s32 id);

void AudioTest_RunParameterController(void)
{
    s32 cues[3] = {0, 80, 100};
    s32 preset;
    s32 row;
    s32 timer;

    row = 0;
    preset = 2;
    do {
        Data_03007804 = 0;
    } while (0);
    timer = 20;

    for (;;) {
        if (timer != 0)
            timer--;

        if (gSoundClipFlag != 0) {
            gSoundClipFlag = 0;
            timer = 20;
        }

        if (gKeysRepeat & 4) {
            preset = (preset + 1) % 5;
            Sound_LoadPresetParameters(preset);
        }
        if (gKeysRepeat & 0x100)
            cues[row] += 10;
        if (gKeysRepeat & 0x200)
            cues[row] -= 10;
        if (gKeysRepeat & 0x10)
            cues[row] += 1;
        if (gKeysRepeat & 0x20)
            cues[row] -= 1;
        if (gKeysRepeat & 0x40) {
            if (row > 0)
                row--;
        }
        if (gKeysRepeat & 0x80) {
            if (row <= 1)
                row++;
        }
        if (gKeysRepeat & 1)
            AudioCommand_Play(cues[row]);
        if (gKeysRepeat & 2)
            AudioCommand_Play(19);
        if (gKeysRepeat & 8)
            AudioCommand_Play(17);
        if (gKeysRepeat & 4)
            AudioCommand_Play(0x121);

        WaitFrames(1);
    }
}

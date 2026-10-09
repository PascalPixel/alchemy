#include "types.h"
#include "io_reg.h"
#include "system.h"

extern volatile u32 gKeysRepeat;
extern s32 gSoundClipFlag;
extern s32 IwramHi_Data1;

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
        IwramHi_Data1 = 0;
    } while (0);
    timer = 20;

    for (;;) {
        if (timer != 0)
            timer--;

        if (gSoundClipFlag != 0) {
            gSoundClipFlag = 0;
            timer = 20;
        }

        if (gKeysRepeat & KEY_SELECT) {
            preset = (preset + 1) % 5;
            Sound_LoadPresetParameters(preset);
        }
        if (gKeysRepeat & KEY_R)
            cues[row] += 10;
        if (gKeysRepeat & KEY_L)
            cues[row] -= 10;
        if (gKeysRepeat & KEY_RIGHT)
            cues[row] += 1;
        if (gKeysRepeat & KEY_LEFT)
            cues[row] -= 1;
        if (gKeysRepeat & KEY_UP) {
            if (row > 0)
                row--;
        }
        if (gKeysRepeat & KEY_DOWN) {
            if (row <= 1)
                row++;
        }
        if (gKeysRepeat & KEY_A)
            AudioCommand_Play(cues[row]);
        if (gKeysRepeat & KEY_B)
            AudioCommand_Play(19);
        if (gKeysRepeat & KEY_START)
            AudioCommand_Play(17);
        if (gKeysRepeat & KEY_SELECT)
            AudioCommand_Play(0x121);

        WaitFrames(1);
    }
}

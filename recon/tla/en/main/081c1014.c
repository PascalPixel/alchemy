/* Near miss: differ 2 of 145, The Broken Seal's AudioTest_RunParameterController with ⚓️'s preset 3, the frame state's byte 2 set, a clip timer of 2 and the commands 0, 78 and 390. One swap remains at the start: the native loads the cue initializer's address before it copies sp for the array (mov r9, sp). The two one-pass blocks come from a greedy device search (differ 14 without them); a struct copy, declaration order and three permuter minutes found nothing better. Adopting also places this object's .rodata (the initializer {0, 80, 100}) at 0x081c3430, where the scaffolds hold it after Audio_EmptyCallback. */
#include "types.h"
#include "io_reg.h"
#include "input.h"

struct SoundFrameState
{
    u8 unk00[2];
    u8 unk02;
    u8 unk03;
};

extern volatile struct SoundFrameState gData_03001138;
extern s32 Data_03000ee4;
extern s32 gIwramHi_Data1;

void Sound_LoadPresetParameters(s32 preset);
void AudioCommand_Play(s32 id);
void WaitFrames(s32 frames);

void AudioTest_RunParameterController(void)
{
    s32 cues[3] = {0, 80, 100};
    s32 preset;
    s32 row;
    s32 timer;

    row = 0;
    preset = 3;
    do {
        gIwramHi_Data1 = 0;
    } while (0);
    gData_03001138.unk02 = 1;
    timer = 20;

    for (;;)
    {
        if (timer != 0)
            timer--;

        if (Data_03000ee4 != 0)
        {
            do {
                Data_03000ee4 = 0;
            } while (0);
            timer = 2;
        }

        if (gInput.repeat & KEY_SELECT)
        {
            preset = (preset + 1) % 5;
            Sound_LoadPresetParameters(preset);
        }
        if (gInput.repeat & KEY_R)
            cues[row] += 10;
        if (gInput.repeat & KEY_L)
            cues[row] -= 10;
        if (gInput.repeat & KEY_RIGHT)
            cues[row] += 1;
        if (gInput.repeat & KEY_LEFT)
            cues[row] -= 1;
        if (gInput.repeat & KEY_UP)
        {
            if (row > 0)
                row--;
        }
        if (gInput.repeat & KEY_DOWN)
        {
            if (row <= 1)
                row++;
        }
        if (gInput.repeat & KEY_A)
            AudioCommand_Play(cues[row]);
        if (gInput.repeat & KEY_B)
            AudioCommand_Play(0);
        if (gInput.repeat & KEY_START)
            AudioCommand_Play(78);
        if (gInput.repeat & KEY_SELECT)
            AudioCommand_Play(390);

        WaitFrames(1);
    }
}

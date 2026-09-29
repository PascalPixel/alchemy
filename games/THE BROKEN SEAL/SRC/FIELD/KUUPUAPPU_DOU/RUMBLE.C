/* The cave rumbles: the map shakes for 480 frames, drifting its scroll by
   a random amount each frame, then the second layer fades out, the opened
   passage is copied into the map and the chime plays. */
#include "TYPES.H"
#include "FIELD_EVENT.H"

struct MapShake {
    u8 unknown_00[0x18];
    s32 direction_x;
    s32 direction_z;
    u8 unknown_20[4];
    s32 scroll;
};

extern u8 *gMapWork;
extern u32 gFrameCount;

void QueueIoWriteDelay2(u32 address, s32 value);
s32 Engine_MathRemainder(s32 value, s32 divisor);
void Engine_MapRenderWaitForValues(void);

void KuupuappuDou_RunRumble(void)
{
    struct MapShake *shake = (struct MapShake *)(gMapWork + 0x164);
    s32 frames;
    s32 eva;
    s32 evb;
    s32 top;

    Event_Begin();
    if (gFrameCount & 1) {
        shake->direction_x = 1;
        shake->direction_z = 1;
    } else {
        shake->direction_x = -1;
        shake->direction_z = -1;
    }
    Work_SetValuesIfNonNegative(0x30000, 0x30000, 0x10000);
    Work_SetValuesIfNonNegative(-1, -1, 0xe666);
    Audio_PlayCue(163);
    for (frames = 0x1df; frames >= 0; frames--) {
        u32 drift = (u32)(Random_Next() << 11) >> 16;

        shake->scroll = (double)shake->scroll - (4718.592 - (double)drift);
        Event_Wait(1);
    }
    eva = 6;
    evb = 6;
    frames = 0;
    top = eva << 10;
    do {
        QueueIoWriteDelay2(0x4000052, top | (eva << 5) | evb);
        Event_Wait(1);
        if (Engine_MathRemainder(frames, 20) == 0) {
            evb--;
            eva--;
        }
        frames++;
    } while (frames <= 69);
    Map_CopyCells(19, 83, 15, 8, 19, 91);
    Audio_PlayCue(0x120);
    Map_Redraw();
    Engine_MapRenderWaitForValues();
    Event_End();
}

#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "KANPAN.H"

union Slot {
    s32 w;
    s16 h[2];
};

extern u8 LinkedMessage_TheresNothingWeCanDo[];

s32 BuildMotionCountdown(s32, s16);

void FieldScene_RunScene3af_02000bb8(void)
{
    u32 i;
    s32 record;

    if (GameFlag_IsSet(0x271) == 0) {
        Audio_PlayCue(158);
        Map_CopyCellsTo(30, 94, 13, 94, 1, 3);
        GameFlag_Set(0x271);
    }
}

void FieldScene_RunScene3af_02000bf0(void)
{
    u32 i;
    s32 record;

    if (GameFlag_IsSet(0x272) == 0) {
        Audio_PlayCue(158);
        Map_CopyCellsTo(30, 108, 13, 108, 1, 2);
        GameFlag_Set(0x272);
    }
}

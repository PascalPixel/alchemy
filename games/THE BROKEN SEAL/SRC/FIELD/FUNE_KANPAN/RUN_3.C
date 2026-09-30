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
s32 Object_GetById();

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

void FieldScene_RunScene3af_02004218(void)
{
    s32 record;

    Camera_MoveTo(0xe80000, -1, 0x2a40000, 0);
    Map_Redraw();
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0xe80000, 0x2a40000);
    record = Object_GetById(0);
    {
        s32 shown = 0x4000;

        *(u16 *)(record + 6) = shown;
    }
    Task_Wait(1);
}

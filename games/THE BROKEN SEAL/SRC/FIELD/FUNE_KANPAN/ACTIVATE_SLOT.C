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
u8 *Object_GetById(s32);

void OverlayObject_ActivateSlotWithMode3(s32 a)
{
    u8 *p = Object_GetById(a);

    if (p != 0) {
        Actor_SetSpritePriority(a, 3);
        Actor_SetSpriteFlags(p, 0);
        p[89] = 0;
        {
            s32 c;
            c = 2 | p[35];
            p[35] = c;
        }
    }
}

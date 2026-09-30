/* Draft: FacingObject_TurnPairToFaceEachOther (0x080d3600), ☀️'s
   FIELD/COMMON/OBJECT/LINK_PAIR.C. Score 60: the ROM sets turning (movs r1,
   #2) right after loading first->facing; this source sets it after the
   sign extension. Links as recon/tla/raw/080d3600.s. */
#include "TYPES.H"

struct FacingObject {
    u8 unknown_00[6];
    u16 facing;
    s32 position_x;
    u8 unknown_0c[4];
    s32 position_z;
};

s32 ArcTan2(s32, s32);
s32 WaitFrames(s32 frames);

void FacingObject_TurnPairToFaceEachOther(struct FacingObject *first, struct FacingObject *second)
{
    s32 toward;
    s32 away;
    s32 frame;
    s32 turning;
    s32 delta;

    if (first == NULL || second == NULL)
        return;
    toward = (u16)ArcTan2(second->position_z - first->position_z,
        second->position_x - first->position_x);
    away = toward + 0x8000;
    for (frame = 0; frame < 60; frame++) {
        delta = (s16)(toward - first->facing);
        turning = 2;
        if (delta != 0) {
            if (delta > 0x1000)
                delta = 0x1000;
            if (delta < -0x1000)
                delta = -0x1000;
            first->facing += delta;
        } else {
            turning = 1;
        }
        delta = (s16)(away - second->facing);
        if (delta != 0) {
            if (delta > 0x1000)
                delta = 0x1000;
            if (delta < -0x1000)
                delta = -0x1000;
            second->facing += delta;
        } else {
            turning--;
        }
        if (turning == 0)
            break;
        WaitFrames(1);
    }
}

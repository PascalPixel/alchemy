#include "RAMAKAN.H"

void FieldScene_RunScene3a5_02001874(void)
{
    u32 i;
    s32 record;

    Actor_SetSpeed(8, 0x8000, 0x4000);
    Actor_SetAnimation(8, 1);
    Actor_WalkToAndWait(8, 168, 96);
    Actor_SetAnimation(8, 2);
}


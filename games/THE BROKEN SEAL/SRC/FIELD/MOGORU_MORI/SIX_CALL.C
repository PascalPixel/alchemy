#include "MORI.H"

void FieldScene_RunSixCallSetupSequence(s32 no, s32 val)
{
    s32 v0 = 0x20000;
    s32 v1 = 0x4000;

    Camera_SetSpeed(v0, v1);
    Camera_MoveToActor(no, 1);
    Camera_WaitForMove();
    Battle_WaitMode0(30);
    MogoruMori_SpawnPuffRing(no);
    Actor_SetChildValue(no, val);
}


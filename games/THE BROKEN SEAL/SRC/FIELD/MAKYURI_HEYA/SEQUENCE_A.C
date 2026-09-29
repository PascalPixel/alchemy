#include "PROBE.H"

extern const u16 MakyuriHeya_FloorSwitchCloseCells[];

void FieldScene_RunScene39cSequenceA(void)
{
    struct FieldActor *actor;

    if (GameFlag_IsSet(0x256) != 0) {
        Event_Begin();
        GameFlag_Clear(0x256);
        Actor_Get(0)->y.fixed += 0x20000;
        actor = Actor_Get(0);
        FIELD_AT_OFFSET(actor, s32, 0x3c) = Actor_Get(0)->y.fixed;
        Event_Wait(5);
        Map_CopyCellsTo(8, 29, 10, 23, 1, 1);
        Audio_PlayCue(217);
        Map_AnimateCells(MakyuriHeya_FloorSwitchCloseCells, 10, 18);
        Event_End();
    }
}

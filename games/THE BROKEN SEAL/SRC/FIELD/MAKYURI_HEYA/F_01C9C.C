#include "TYPES.H"
#include "FIELD_EVENT.H"

extern u16 Data_0200e064[];

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

/* Once, when the leader stands in the cell block at x 164-171 and z
 * 372-379, lifts him by two pixels, opens the passage and animates it. */
void MakyuriHeya_TriggerFloorSwitch(void)
{
    s32 x;
    s32 z;

    if (Value1((s32 (*)())Engine_GameFlagIsSet, 0x256) != 0)
        return;
    x = Engine_ActorGet(0)->x.part.pixel;
    z = Engine_ActorGet(0)->z.part.pixel;
    if (x < 164 || x > 171)
        return;
    if (z < 372)
        return;
    if (z >= 380)
        return;
    Engine_EventBegin();
    Engine_GameFlagSet(0x256);
    Engine_EventWait(5);
    Engine_ActorGet(0)->y.fixed -= 0x20000;
    Engine_ActorGet(0)->target_y = Engine_ActorGet(0)->y.fixed;
    Engine_MapCopyCellsTo(6, 29, 10, 23, 1, 1);
    Engine_AudioPlayCue(217);
    Engine_MapAnimateCells(Data_0200e064, 10, 18);
    Engine_EventEnd();
}

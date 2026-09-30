#include "TYPES.H"
#include "MAKYURI.H"
#include "CALL.H"

extern u16 MakyuriHeya_FloorSwitchCells[];

/* Once, when the leader stands in the cell block at x 164-171 and z
 * 372-379, lifts him by two pixels, opens the passage and animates it. */
void MakyuriHeya_TriggerFloorSwitch(void)
{
    s32 x;
    s32 z;

    if (Value1((s32 (*)())Engine_GameFlagIsSet, 0x256) != 0)
        return;
    x = Object_GetById(0)->x.part.pixel;
    z = Object_GetById(0)->z.part.pixel;
    if (x < 164 || x > 171)
        return;
    if (z < 372)
        return;
    if (z >= 380)
        return;
    Engine_EventBegin();
    Engine_GameFlagSet(0x256);
    Battle_WaitMode0(5);
    Object_GetById(0)->y.fixed -= 0x20000;
    Object_GetById(0)->target_y = Object_GetById(0)->y.fixed;
    Engine_MapCopyCellsTo(6, 29, 10, 23, 1, 1);
    Audio_PlayCue(217);
    Engine_MapAnimateCells(MakyuriHeya_FloorSwitchCells, 10, 18);
    Engine_EventEnd();
}

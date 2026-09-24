#include "TYPES.H"
#include "FIELD_EVENT.H"

/* Floor switch under actor 0 (pixel x 676 to 683, z 788 to 795): stepping on it sets flag 0x201, redraws the switch cell and the 3x5 cells at (41, 117), plays cue 161 and lowers the actor two pixels; standing off it clears the flag and restores them. */
void VinasuHeya_UpdateFloorSwitch(void)
{
    struct FieldActor *leader = Engine_ActorGet(0);
    s32 x = leader->x.part.pixel;
    s32 z = leader->z.part.pixel;

    if ((u32)(x - 676) > 7 || z < 788 || z >= 796) {
        Map_CopyCellsTo(53, 50, 42, 49, 1, 1);
        Map_CopyCellsTo(55, 117, 41, 117, 3, 5);
        GameFlag_Clear(0x201);
        leader->motion_flags |= 1;
        *(s32 *)leader->unknown_14 = 0;
        leader->y.fixed = 0;
    } else if (!GameFlag_IsSet(0x201)) {
        Engine_EventBegin();
        Engine_EventWait(5);
        Map_CopyCellsTo(52, 50, 42, 49, 1, 1);
        Map_CopyCellsTo(52, 117, 41, 117, 3, 5);
        GameFlag_Set(0x201);
        Engine_AudioPlayCue(161);
        leader->motion_flags &= ~1;
        *(s32 *)leader->unknown_14 = -0x20000;
        leader->y.fixed = -0x20000;
        Engine_EventEnd();
    }
}

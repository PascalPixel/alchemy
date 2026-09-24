#include "TYPES.H"
#include "FIELD_EVENT.H"

s32 FuneHeya_FindFirstSetFlag(u32 group, s32 alternate);
void OverlayObject_SetPositionAndHeading(s32 actor, s32 x, s32 z, s32 heading);

/* Ship cabin: for each of four groups whose flag is set (all when not gated), place its first actor at the group's spot and hide the stand-in actor 10 to 13. */
void FuneHeya_PlaceFoundActors(s32 gated)
{
    s32 actor;

    if (gated && !Engine_GameFlagIsSet(0x929)) {
        return;
    }
    if ((actor = FuneHeya_FindFirstSetFlag(0, 0)) != 0) {
        OverlayObject_SetPositionAndHeading(actor, 410, 172, 0xd000);
        Engine_ActorSetPosition(10, 0, 0);
    }
    if (gated && !Engine_GameFlagIsSet(0x92a)) {
        return;
    }
    if ((actor = FuneHeya_FindFirstSetFlag(1, 0)) != 0) {
        OverlayObject_SetPositionAndHeading(actor, 470, 172, 0xb000);
        Engine_ActorSetPosition(11, 0, 0);
    }
    if (gated && !Engine_GameFlagIsSet(0x92b)) {
        return;
    }
    if ((actor = FuneHeya_FindFirstSetFlag(2, 0)) != 0) {
        OverlayObject_SetPositionAndHeading(actor, 410, 204, 0xd000);
        Engine_ActorSetPosition(12, 0, 0);
    }
    if (gated) {
        return;
    }
    if ((actor = FuneHeya_FindFirstSetFlag(3, 0)) != 0) {
        OverlayObject_SetPositionAndHeading(actor, 470, 204, 0xb000);
        Engine_ActorSetPosition(13, 0, 0);
    }
}

#include "TYPES.H"

struct PendingEvent;

s32 BattleFx_FindMatchingEvent(s32, s32, void *);
s32 Func_080ce31c(s32 kind);
struct PendingEvent *Func_080ce458(u32 filter, s32 kind, s32 selector);

s32 BattleFx_HasMatchingEvent5(s32 effectId);
void ObjectMotion_SnapToTerrain(void *object);

/* Whether a kind 5 event answers the effect, as in ☀️; ⚓️ passes the
   event lookup the effect's selector. */
s32 BattleFx_HasMatchingEvent5(s32 effectId)
{
    u16 kind = effectId;
    s32 result = (s32)Func_080ce458(0xf0000005, kind, Func_080ce31c(kind));

    return (u32)((-result) | result) >> 31;
}

u32 BattleFx_HasTrigger(u16 effectId)
{
    s32 result;

    result = Event_FindFacingTrigger(effectId);
    return (u32)((0 - result) | result) >> 0x1F;
}

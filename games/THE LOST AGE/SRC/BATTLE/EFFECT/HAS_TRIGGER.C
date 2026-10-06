#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "LIB.H"
#include "MENU.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "TYPES.H"

struct PendingEvent;



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

#include "TYPES.H"
#include "FIELD_EVENT.H"

static __inline__ s32 Value1(s32 (*f)(s32), s32 a0)
{
    return f(a0);
}

static __inline__ void Call1(s32 (*f)(s32), s32 a0)
{
    f(a0);
}

/* Once the leader stands in cells x 21-23, z 10-11 while not cloaked (and the
 * game-state halfword at +0x24a is not 8), sets flag 0x220 and raises
 * trigger 91; flag 0x220 keeps it from firing again. */
void TakaraAshiba_Func02001308(void)
{
    struct FieldActor *leader = Engine_ActorGet(0);
    s32 x = leader->x.fixed / 0x100000;
    s32 z = leader->z.fixed / 0x100000;
    struct EventWork *event = gEventWork;

    if (!Value1(Engine_GameFlagIsSet, 0x220) && gGameState.cloaked == 0
        && *(s16 *)((u8 *)&gGameState + 0x24a) != 8
        && (u32)(x - 21) <= 2 && z >= 10 && z <= 11) {
        Call1(Engine_GameFlagSet, 0x220);
        event->raised_trigger = 91;
    }
}

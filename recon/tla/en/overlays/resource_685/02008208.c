/* differ 2: the native loads the first warp target into r6 two instructions after the owner address (between the halves of the constant 424); here that load has the lowest priority before the first call and comes last. */
#include "../../../../../games/tla/src/scene/champa_cliff_caves/scene.h"

extern const struct WarpTarget Data_02003d18;

void ObjectMotion_CommitPositionAndActivate(s32 actor, s32 dx, s32 dz);
void Func_020034d4(s32 actor, s32 x, s32 z);

static __inline__ void ObjectMotion_CommitPositionAndActivate_(s32 actor, s32 dx, s32 dz)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    ObjectMotion_CommitPositionAndActivate(actor, dx, dz);
}

static __inline__ void ObjectMotion_SetSpeedParameters_(s32 actor, s32 speed, s32 acceleration)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    ObjectMotion_SetSpeedParameters(actor, speed, acceleration);
}

static __inline__ void Func_020034d4_(s32 actor, s32 x, s32 z)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_020034d4(actor, x, z);
}

void Func_02000208(void)
{
    const struct WarpTarget *warp = &Data_02003d10;
    s32 msg;

    ObjectMotion_SetPositionAndReset(gPartyState.current_owner, 168, 424);
    Func_02003534(gPartyState.current_owner, 0);
    Func_0200362c(158);
    Func_02003414(warp->scene, warp->x, warp->z);
    Object_GetById(8)->motion_flags = 0;
    {
        s32 x = 184 << 16;

        Func_020034d4_(8, x, 408 << 16);
    }
    ObjectMotion_SetSpeedParameters_(8, 0x1cccc, 0xe666);
    ObjectMotion_CommitPositionAndActivate(8, 0, 16);
    Func_02003534(8, 0x8000);
    msg = (s32)MsgHasMyGrandson;
    Func_0200350c(msg);
    UiText_OpenMessageAtObject(8, 0);
    if (Inventory_PromptAndSetObjectMode(Func_020035f4(), 0) == 0) {
        Battle_WaitMode0(10);
        Func_0200350c(msg + 1);
    } else {
        Battle_WaitMode0(20);
        Func_0200350c(msg + 2);
    }
    Func_02003524(8, 0);
    ObjectMotion_SetSpeedParameters_(8, 0xcccc, 0x6666);
    ObjectMotion_CommitPositionAndActivate_(8, 0, -14);
    Func_020034d4(8, 0, 0);
    warp = &Data_02003d18;
    Func_02003414(warp->scene, warp->x, warp->z);
    Func_0200362c(158);
}

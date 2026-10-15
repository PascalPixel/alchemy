/* Draft of overlay 685's Func_02000208, differ 2 of 95: the room's rest offer, for the head of
 * events.c (its linker neighbour Func_02000304 asks the same question). Left: the native loads the
 * warp pointer (ldr r6) right after it starts 424, before the owner load; this draft loads it
 * last before the call. The three forwarding inlines are needed (differ 10 as direct calls);
 * Func_02003534_ and an inline for the first move change nothing, nor does assigning the warp
 * pointer as a statement. */
#include "../../../../games/tla/src/scene/champa_cliff_caves/scene.h"
#include "scene_ids.h"

void Func_020034d4(s32 actor, s32 x, s32 z);
void ObjectMotion_CommitPositionAndActivate(s32 actor, s32 dx, s32 dz);

extern const struct WarpTarget Data_02003d18;

static __inline__ void ObjectMotion_CommitPositionAndActivate_(s32 actor, s32 dx, s32 dz)
{
    ObjectMotion_CommitPositionAndActivate(actor, dx, dz);
}

static __inline__ void ObjectMotion_SetSpeedParameters_(s32 actor, s32 speed, s32 acceleration)
{
    ObjectMotion_SetSpeedParameters(actor, speed, acceleration);
}

static __inline__ void Func_020034d4_(s32 actor, s32 x, s32 z)
{
    Func_020034d4(actor, x, z);
}

void Func_02000208(void)
{
    const struct WarpTarget *warp;
    s32 msg;

    warp = &Data_02003d10;
    ObjectMotion_SetPositionAndReset(gPartyState.current_owner, 168, 424);
    Func_02003534(gPartyState.current_owner, 0);
    Func_0200362c(158);
    Func_02003414(warp->scene, warp->x, warp->z);
    Object_GetById(8)->motion_flags = 0;
    Func_020034d4_(8, 184 << 16, 204 << 17);
    ObjectMotion_SetSpeedParameters_(8, 0x1cccc, 0xe666);
    ObjectMotion_CommitPositionAndActivate_(8, 0, 16);
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
    Func_020034d4_(8, 0, 0);
    warp = &Data_02003d18;
    Func_02003414(warp->scene, warp->x, warp->z);
    Func_0200362c(158);
}


#include "jupiter_room_m.h"
#include "scene_ids.h"

extern const u8 MsgIMSorry[];

void Func_020047d4(void);
void Func_0200494c(s32 a);
void Func_0200484c(s32 id, s32 x, s32 z, s32 angle);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_0200488c(const u8 *message);
void Func_0200489c(s32 actor, s32 a, s32 b);
struct ObjectRuntime *Func_020048cc(s32 actor, s32 emote, s32 frames);
void Motion_CamBounds(s32 x, s32 y, s32 z, s32 d);
void Func_020048f4(void);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object, s32 flags);
void Func_020048b4(s32 actor, s32 a);
void ObjectMotion_OffsetPositionAndReset(u32 objectId, s32 x_offset, s32 z_offset);
void Party_SetFields1eeAnd1f0(s32 scene, s32 entrance);
void Party_SetFields1f2And1f4(s32 scene, s32 entrance);
void Party_RemoveActiveOwner(s32 owner);
s32 Party_AddActiveOwner(s32 owner);
s32 Owner_AdjustFirstValue(s32 owner, s32 delta);
void UnitCalc(s32 owner);
s32 UnitPpAdd(s32 owner, s32 delta);
void Func_02004904(s32 a, s32 b);

static __inline__ void JupiterRoomM_AdjustFirstValue(s32 owner, s32 delta)
{
    /* FAKEMATCH: forwarding through the inline reloads -10000 from the pool at each call, as the native does (direct: the four uses share it in r5, differ 23 of 160). */
    Owner_AdjustFirstValue(owner, delta);
}

static __inline__ void JupiterRoomM_PpAdd(s32 owner, s32 delta)
{
    /* FAKEMATCH: forwarding through the inline reloads -10000 from the pool at each call, as the native does (direct: the four uses share it in r5, differ 23 of 160). */
    UnitPpAdd(owner, delta);
}

static __inline__ void JupiterRoomM_Func_020048b4(s32 actor, s32 a)
{
    /* FAKEMATCH: forwarding through the inline builds the second argument around the first, as the native does (direct: differ 2 of 160). */
    Func_020048b4(actor, a);
}

static __inline__ void ObjPlace(s32 id, s32 x, s32 z, s32 angle)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does (direct: differ 35 of 160). */
    Func_0200484c(id, x, z, angle);
}

/* FAKEMATCH: the angle passes through a block temporary, so it is built before x and z, as the native does (a plain argument: differ 12 of 160). */
#define PLACE(id, x, z, angle) \
    { \
        s32 angle_ = (angle); \
        ObjPlace(id, (x) << 16, (z) << 16, angle_); \
    }

// Places actors 0, 2, 27 and 26 and shows "I'm sorry"; then the party is
// left with owners 2 and 0, whose first values and PP drop by 10000.
void Func_02001c7c(void)
{
    Func_020047d4();
    Func_0200494c(0);
    PLACE(0, 776, 202, 0x8000);
    PLACE(2, 731, 232, 0xa000);
    PLACE(27, 731, 213, 0x4000);
    PLACE(26, 737, 202, 0);
    Object_SetModeById(0, 1);
    Object_SetModeById(2, 1);
    Object_SetModeById(26, 1);
    Object_SetModeById(27, 1);
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    Func_0200488c(MsgIMSorry);
    Func_0200489c(2, 0, 5);
    Func_020048cc(4, 0x100, 30);
    Motion_CamBounds(705 << 16, 0, 177 << 16, 1);
    Func_020048f4();
    Battle_WaitMode0(30);
    ObjectDispatch_SetSingleChildFlags(Object_GetById(2), 0);
    Object_SetModeById(2, 32);
    Battle_WaitMode0(10);
    JupiterRoomM_Func_020048b4(0, 0x6000);
    Func_0200489c(0, 0, 5);
    Func_020048cc(26, 0x107, 30);
    Func_0200489c(26, 0, 5);
    ObjectMotion_OffsetPositionAndReset(26, 24, 0);
    Battle_WaitMode0(20);
    do {
        /* FAKEMATCH: the one-pass block ends CSE's block, so the byte store's dead zero is not kept in r8 for the store of the owner below (without: differ 31 of 160). */
        gPartyState.battleRule24b = 6;
    } while (0);
    Party_SetFields1eeAnd1f0((s32)&SceneId_Ov6a3, 77);
    Party_SetFields1f2And1f4((s32)&SceneId_Ov6a3, 77);
    Party_RemoveActiveOwner(4);
    Party_RemoveActiveOwner(7);
    Party_RemoveActiveOwner(5);
    Party_RemoveActiveOwner(6);
    Party_RemoveActiveOwner(2);
    Party_RemoveActiveOwner(0);
    Party_AddActiveOwner(2);
    Party_AddActiveOwner(0);
    Party_RemoveActiveOwner(3);
    Party_RemoveActiveOwner(1);
    JupiterRoomM_AdjustFirstValue(2, -10000);
    JupiterRoomM_AdjustFirstValue(0, -10000);
    UnitCalc(0);
    UnitCalc(2);
    JupiterRoomM_AdjustFirstValue(0, 173);
    JupiterRoomM_PpAdd(2, -10000);
    JupiterRoomM_PpAdd(0, -10000);
    gPartyState.currentOwner = 0;
    Func_02004904(102, 4);
}

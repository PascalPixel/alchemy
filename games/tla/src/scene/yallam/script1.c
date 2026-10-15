#include "types.h"
#include "object_runtime.h"
#include "party_state.h"

s32 GameFlag_Test(s32 flag);
struct ObjectRuntime *Object_GetById(s32 id);
void ObjectMotion_OffsetPositionAndResetMotion(s32 id, s32 x, s32 z);
void ObjectMotion_CommitCurrentPositionAndActivate(s32 id);
void Func_02002488(void);
void Func_02002490(void);
void Func_020024f8(s32 id, s32 x, s32 z);
void Func_02002550(s32 id, s32 a);
void Func_02002590(s32 value);
void Func_02002598(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6);

static inline void Yallam_02002598(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Func_02002598(a0, a1, a2, a3, a4, a5, a6);
}

static inline void Yallam_020024f8(s32 id, s32 x, s32 z)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Func_020024f8(id, x, z);
}

static inline void Yallam_Offset(s32 id, s32 x, s32 z)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    ObjectMotion_OffsetPositionAndResetMotion(id, x, z);
}

void Func_0200038c(void)
{
    struct ObjectRuntime *leader = Object_GetById(gPartyState.current_owner);

    Yallam_02002598(26, 95, 2, 1, 26, 76, 2);
    if (GameFlag_Test(0xf62) == 0) {
        Yallam_020024f8(66, 0x1a80000, 0xe80000);
        if (leader->x >> 20 == 26 && leader->z >> 20 == 14) {
            Func_02002488();
            Func_02002590(0);
            Func_02002550(gPartyState.current_owner, 0x102);
            Yallam_Offset(gPartyState.current_owner, 16, 0);
            leader->velocity_y = 0x60000;
            ObjectMotion_CommitCurrentPositionAndActivate(gPartyState.current_owner);
            Func_02002490();
        }
    }
}

void Func_0200041c(void)
{
    Yallam_02002598(67, 25, 3, 3, 106, 20, 1);
}

void Func_0200043c(void)
{
    struct ObjectRuntime *leader = Object_GetById(gPartyState.current_owner);

    Yallam_02002598(67, 25, 3, 3, 106, 20, 1);
    if (GameFlag_Test(0xf60) == 0) {
        Yallam_020024f8(64, 0x2a80000, 0x1580000);
        if (leader->x >> 20 == 42 && leader->z >> 20 == 21) {
            Func_02002488();
            Func_02002590(0);
            Func_02002550(gPartyState.current_owner, 0x102);
            Yallam_Offset(gPartyState.current_owner, 0, -16);
            leader->velocity_y = 0x60000;
            ObjectMotion_CommitCurrentPositionAndActivate(gPartyState.current_owner);
            Func_02002490();
        }
    }
}

#include "TYPES.H"



#include "PARTY_STATE.H"

struct SceneEventWork {
    u8 unknown_000[0x1c4];
    u16 branch_counter;
};

struct SceneIwram {
    u8 unknown_00[0x6c];
    struct SceneEventWork *events;
};

#define SCENE_IWRAM ((struct SceneIwram *)0x03000000)

extern const u8 gActorExitScript[];
void Func_0200b138();
void Func_0200b158();
void Func_0200b1b0();
void Func_0200b1b8();
void Func_0200b1c0();
s32 Func_0200b1d0();
void ObjectMotion_SetSpeedParameters();
void ObjectMotion_EnableActionAndSetCallback();
void Object_RefreshSelectorById();
void ObjectMotion_EnableActionAndResetMotion();
void Object_SetActionCallbackAndRefreshById();
void ObjectMotion_ResetAndSetPositionInMode2();
void ObjectMotion_SetPositionAndReset();
void Func_0200b220();
void ObjectMotion_CopyPosition();
void Object_SetModeById();
void Motion_SetModeAndWaitAnimation();
void ObjectMotion_Launch();
void Func_0200b288();
s32 Func_0200b290();
void Func_0200b2a0();
s32 Func_0200b2a8();
void Func_0200b2b0();
s32 Func_0200b2c0();
void Func_0200b2c8();
void Func_0200b2d0();
void Func_0200b2d8();
void Func_0200b310();
void Object_LinkObjectAndSetCallback();
void Func_0200b320();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

void Func_02000d14(void)
{
    const u8 *script;

    Func_0200b1b8();
    Func_0200b310(0);
    Call2(Func_0200b2d0, 0x9999, 0x1333);
    Call4(Func_0200b2d8, 0x6c0000, -1, 0x2d00000, 1);
    Call3(ObjectMotion_SetSpeedParameters, gPartyState.current_owner, 0xcccc, 0x6666);
    Call3(ObjectMotion_SetPositionAndReset, gPartyState.current_owner, 86, 0x2c8);
    ObjectMotion_CopyPosition(8, gPartyState.current_owner);
    Func_0200b138(1);
    Call3(ObjectMotion_SetSpeedParameters, 8, 0xcccc, 0x6666);
    Call3(ObjectMotion_SetPositionAndReset, 8, 104, 0x2c8);
    Func_0200b2b0(gPartyState.current_owner, 0, 0);
    Call3(Func_0200b2b0, 8, 0x8000, 20);
    Call1(Func_0200b288, 0x16de);
    Call2(Func_0200b2a0, 0x2008, 0);
    Func_0200b2b0(8, 0, 40);
    Call3(Func_0200b2b0, 8, 0x5000, 20);
    Func_0200b2b0(8, 0, 0);
    Call3(ObjectMotion_SetPositionAndReset, gPartyState.current_owner, 104, 0x2d8);
    Func_0200b2b0(gPartyState.current_owner, 0, 20);
    Call3(Func_0200b2b0, gPartyState.current_owner, 0x2000, 20);
    Func_0200b2b0(gPartyState.current_owner, 0, 0);
    Func_0200b2a0(8, 0);
    Call3(Func_0200b2b0, 8, 0x3000, 0);
    Call3(Func_0200b2b0, gPartyState.current_owner, 0xc000, 20);
    Object_SetModeById(8, 3);
    Value2(Func_0200b2a8, 8, 0);
    Value3(Func_0200b2c0, 8, 0x106, 40);
    Func_0200b2a0(8, 0);
    Call2(ObjectMotion_CopyPosition, 5, gPartyState.current_owner);
    ObjectMotion_CopyPosition(6, gPartyState.current_owner);
    Func_0200b138(1);
    Call3(ObjectMotion_SetSpeedParameters, 6, 0xcccc, 0x6666);
    Call3(ObjectMotion_SetSpeedParameters, 5, 0xcccc, 0x6666);
    Call3(ObjectMotion_ResetAndSetPositionInMode2, 5, 86, 0x2c8);
    Call3(ObjectMotion_SetPositionAndReset, 6, 86, 0x2d8);
    Call3(Func_0200b2b0, 6, 0xe000, 0);
    Func_0200b220(5);
    Object_SetModeById(5, 1);
    Call3(Func_0200b2b0, 5, 0x4000, 0);
    Func_0200b2a0(5, 0);
    Call3(Func_0200b2b0, gPartyState.current_owner, 0x8000, 0);
    Func_0200b2b0(6, 0, 40);
    Call3(Func_0200b2b0, 6, 0xe000, 0);
    Call3(Func_0200b2b0, gPartyState.current_owner, 0xc000, 0);
    Object_SetModeById(8, 4);
    Value2(Func_0200b290, 0x2008, 0);
    Func_0200b2b0(6, 0, 0);
    Call3(Func_0200b2b0, 5, 0x2000, 0);
    if (Value2(Func_0200b1d0, gPartyState.current_owner, 0) == 0) {
        Motion_SetModeAndWaitAnimation(8, 4);
        Call2(Func_0200b2a0, 0x2008, 0);
        SCENE_IWRAM->events->branch_counter += 1;
    } else {
        SCENE_IWRAM->events->branch_counter += 1;
        Motion_SetModeAndWaitAnimation(8, 3);
        Call2(Func_0200b2a0, 0x2008, 0);
    }
    Func_0200b2b0(5, 0, 0);
    Func_0200b2a0(5, 0);
    Value3(Func_0200b2c0, 6, 0x101, 0);
    Call3(Func_0200b2b0, 6, 0xc000, 0);
    Func_0200b2a0(6, 0);
    Call3(Func_0200b2b0, 8, 0x5000, 0);
    Call2(Func_0200b2a0, 0x2008, 0);
    Call3(Func_0200b2c0, gPartyState.current_owner, 0x101, 40);
    Call3(Func_0200b2b0, 8, 0x3000, 0);
    Object_SetModeById(8, 4);
    Call2(Func_0200b2a0, 0x2008, 0);
    Value3(Func_0200b2c0, 5, 0x100, 20);
    Func_0200b2a0(5, 0);
    Call3(Func_0200b2b0, 8, 0x8000, 20);
    Motion_SetModeAndWaitAnimation(8, 3);
    Call2(Func_0200b2a0, 0x2008, 0);
    Func_0200b2b0(6, 0, 20);
    Func_0200b2a0(6, 0);
    Call3(Func_0200b2b0, 5, 0x2000, 0);
    Value2(Func_0200b290, 5, 0);
    if (Value2(Func_0200b1d0, 8, 0) == 0) {
        Call3(Func_0200b2b0, 8, 0x3000, 20);
        Motion_SetModeAndWaitAnimation(8, 3);
        Call2(Func_0200b2a0, 0x2008, 0);
        SCENE_IWRAM->events->branch_counter += 1;
    } else {
        SCENE_IWRAM->events->branch_counter += 1;
        Value3(Func_0200b2c0, 8, 0x103, 0);
        Call3(Func_0200b2b0, 8, 0x4000, 20);
        Call2(Func_0200b2a0, 0x2008, 0);
        Func_0200b320();
    }
    Value3(Func_0200b2c0, 5, 0x100, 0);
    Func_0200b2a0(5, 0);
    Value3(Func_0200b2c0, 8, 0x100, 40);
    Call3(Func_0200b2b0, 8, 0x5000, 0);
    Call2(Func_0200b2a0, 0x2008, 0);
    Value3(Func_0200b2c0, 6, 0x101, 0);
    Call3(Func_0200b2b0, 6, 0xe000, 40);
    Func_0200b2a0(6, 0);
    Call3(Func_0200b2b0, 5, 0x4000, 20);
    Func_0200b2a0(5, 0);
    Call3(Func_0200b2b0, 8, 0x3000, 0);
    Call3(Func_0200b2b0, gPartyState.current_owner, 0xc000, 20);
    Value2(Func_0200b290, 0x2008, 0);
    if (Value2(Func_0200b1d0, gPartyState.current_owner, 0) == 0) {
        Value3(Func_0200b2c0, 6, 0x103, 0);
        Func_0200b2b0(6, 0, 20);
        Func_0200b2a0(6, 0);
        SCENE_IWRAM->events->branch_counter += 1;
    } else {
        SCENE_IWRAM->events->branch_counter += 1;
        Value3(Func_0200b2c0, 6, 0x108, 0);
        Func_0200b2b0(6, 0, 20);
        Func_0200b2a0(6, 0);
    }
    Value3(Func_0200b2c0, 8, 0x100, 0);
    Call2(Func_0200b2a0, 0x2008, 0);
    Call3(Func_0200b2b0, 6, 0xe000, 0);
    Motion_SetModeAndWaitAnimation(6, 3);
    Func_0200b2a0(6, 0);
    Value3(Func_0200b2c0, 5, 0x101, 40);
    Func_0200b2a0(5, 0);
    Object_LinkObjectAndSetCallback(gPartyState.current_owner, 6);
    Object_LinkObjectAndSetCallback(8, 6);
    Object_LinkObjectAndSetCallback(5, 6);
    Call3(ObjectMotion_SetSpeedParameters, 6, 0x6666, 0x3333);
    Call3(ObjectMotion_SetPositionAndReset, 6, 86, 0x2ec);
    Func_0200b1b0(40);
    Value3(Func_0200b2c0, 6, 0x109, 80);
    Call2(Func_0200b2a0, 0x2008, 0);
    ObjectMotion_EnableActionAndResetMotion(gPartyState.current_owner);
    ObjectMotion_EnableActionAndResetMotion(8);
    ObjectMotion_EnableActionAndResetMotion(5);
    Motion_SetModeAndWaitAnimation(6, 3);
    Func_0200b1b0(40);
    Object_SetModeById(5, 4);
    Func_0200b2a0(5, 0);
    Call3(Func_0200b2b0, 6, 0xc000, 0);
    Object_RefreshSelectorById(6);
    Object_SetModeById(6, 4);
    Func_0200b2a0(6, 0);
    Call3(Func_0200b2b0, 8, 0x3000, 0);
    Call3(Func_0200b2b0, gPartyState.current_owner, 0xc000, 40);
    Call3(Func_0200b2b0, 8, 0x5000, 0);
    Call3(Func_0200b2b0, gPartyState.current_owner, 0x6000, 20);
    Call2(Func_0200b2a0, 0x2008, 0);
    Motion_SetModeAndWaitAnimation(6, 3);
    Func_0200b2a0(6, 0);
    Object_SetModeById(8, 3);
    Object_SetModeById(5, 3);
    Motion_SetModeAndWaitAnimation(gPartyState.current_owner, 3);
    Call3(Func_0200b2b0, 6, 0xe000, 0);
    Func_0200b2a0(6, 0);
    Call3(Func_0200b2b0, 8, 0x3000, 0);
    Value2(Func_0200b2a8, 0x2008, 0);
    Call3(Func_0200b2b0, 6, 0xa000, 40);
    Func_0200b2a0(6, 0);
    Func_0200b2b0(5, 0, 0);
    Call3(Func_0200b2b0, 8, 0x8000, 20);
    Object_SetModeById(6, 4);
    Func_0200b2a0(6, 0);
    Call3(Func_0200b2b0, 5, 0x4000, 0);
    Call3(Func_0200b2b0, 8, 0x5000, 0);
    Value3(Func_0200b2c0, 8, 0x105, 0);
    Value3(Func_0200b2c0, 5, 0x105, 40);
    Func_0200b2a0(5, 0);
    Call3(Func_0200b2b0, 6, 0xc000, 0);
    Func_0200b2a0(6, 0);
    Call2(Func_0200b2c8, 8, 0x102);
    Call2(Func_0200b2a0, 0x2008, 0);
    Value3(Func_0200b2c0, 6, 0x108, 20);
    Call3(Func_0200b2b0, 6, 0xe000, 20);
    Func_0200b2a0(6, 0);
    Motion_SetModeAndWaitAnimation(8, 3);
    Func_0200b2a0(5, 0);
    Call3(Func_0200b2b0, 5, 0x2000, 0);
    Value2(Func_0200b290, 5, 0);
    if (Value2(Func_0200b1d0, gPartyState.current_owner, 0) == 0) {
        Object_SetModeById(5, 3);
        Func_0200b2a0(5, 0);
        SCENE_IWRAM->events->branch_counter += 1;
    } else {
        SCENE_IWRAM->events->branch_counter += 1;
        Object_SetModeById(5, 4);
        Func_0200b2a0(5, 0);
    }
    Value3(Func_0200b2c0, 8, 0x109, 40);
    Motion_SetModeAndWaitAnimation(6, 4);
    Func_0200b1b0(20);
    Call2(Func_0200b2a0, 0x2008, 0);
    Motion_SetModeAndWaitAnimation(6, 3);
    Call3(Func_0200b2b0, 8, 0x8000, 20);
    Call2(Func_0200b2a0, 0x2008, 0);
    ObjectMotion_Launch(8, 4, 20);
    Call2(Func_0200b2a0, 0x2008, 0);
    Call3(ObjectMotion_SetSpeedParameters, 6, 0xcccc, 0x6666);
    script = gActorExitScript;
    ObjectMotion_EnableActionAndSetCallback(8, script);
    Call2(ObjectMotion_EnableActionAndSetCallback, 5, script);
    Object_SetActionCallbackAndRefreshById(6, script);
    Call1(Func_0200b158, 0x867);
    Func_0200b1c0();
}

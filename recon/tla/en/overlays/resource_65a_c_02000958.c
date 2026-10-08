#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "party_state.h"

struct EventWork154 {
    u8 unknown_000[0x154];
    s16 return_entrance;
};

struct EventWork1ac {
    u8 unknown_000[0x1ac];
    s32 flag;
};

struct AnimationRecord18 {
    u8 unknown_00[0x12];
    u16 frame;
};

extern u8 MsgFieldEyesOfTruth;
extern const u8 Data_020013c0[];
extern const u8 Data_020013c4[];
extern const u8 Data_020013ca[];
extern const u8 Data_0200121c[];

struct ObjectRuntime *Object_GetById(s32 id);
s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);
void WaitFrames(s32 frames);
void UiText_ShowPositionedMessageAndWait(s32 message, s32 mode);
void Battle_WaitMode0(s32 frames);
void Motion_CamBounds(s32 a, s32 b, s32 c, s32 d);
void ObjectMotion_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
void ObjectMotion_EnableActionAndSetCallback(s32 id, const void *script);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object, s32 flags);
void Object_SetPartAttribute(struct ObjectRuntime *object, s32 value);
void Event_StartScreenTransition(void);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_0200110c(void);
void Func_020011ac(s32 skip);
void Func_02001114(void);
void Func_02001214(s32 cue);
void Func_020010bc(struct ObjectRuntime *object, s32 x, s32 y, s32 z);
void Func_020010c4(struct ObjectRuntime *object);
void Func_020010cc(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_020010dc(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_02001134(s32 id, s32 x, s32 z);
void Func_0200113c(s32 id, s32 value);
void Func_0200115c(s32 id);
void Func_0200117c(s32 entrance);
s32 Func_02001184(s32 owner);
void Func_020011b4(void);
void Func_020011bc(s32 a, s32 b, s32 c, s32 d);
void Func_020011c4(const void *data);
void Func_020011dc(const void *data);
s32 Func_020011fc(void);
void Func_02001204(void);
void Func_02000038(const void *data);
void Func_0200014c(const void *data);
void Func_02000270(struct ObjectRuntime *object);
void Func_020008d0(struct ObjectRuntime *object);
void Func_02000958(void);
void Func_02000a28(void);
void Func_02000c94(void);
void Func_02000cf4(void);

static __inline__ s32 Flag_Test(s32 flag)
{
    return GameFlag_Test(flag);
}

static __inline__ void Flag_Set(s32 flag)
{
    GameFlag_SetBit(flag);
}

static __inline__ void Flag_Clear(s32 flag)
{
    GameFlag_ClearBit(flag);
}

static __inline__ void Map_CopyCells(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f)
{
    Func_020010cc(a, b, c, d, e, f);
}

static __inline__ void Map_CopyAttributes(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f)
{
    Func_020010dc(a, b, c, d, e, f);
}

static __inline__ void Actor_Place(s32 id, s32 x, s32 z)
{
    Func_02001134(id, x, z);
}

static __inline__ void Wait(s32 frames)
{
    WaitFrames(frames);
}

static __inline__ void Cue(s32 cue)
{
    Func_02001214(cue);
}

static __inline__ void Camera_SetBounds(s32 a, s32 b, s32 c, s32 d)
{
    Motion_CamBounds(a, b, c, d);
}

void Func_02000958(void)
{
    u32 i;

    Actor_Place(14, 136 << 16, 258 << 16);
    Actor_Place(19, 184 << 16, 258 << 16);
    Actor_Place(13, 120 << 16, 226 << 16);
    Actor_Place(18, 200 << 16, 226 << 16);
    Actor_Place(12, 120 << 16, 194 << 16);
    Actor_Place(17, 200 << 16, 194 << 16);
    Actor_Place(11, 136 << 16, 162 << 16);
    Actor_Place(16, 184 << 16, 162 << 16);
    Actor_Place(10, 136 << 16, 138 << 16);
    Actor_Place(15, 184 << 16, 138 << 16);
    for (i = 0; i <= 9; i++) {
        Object_SetModeById(i + 20, 0);
        Func_0200113c(i + 20, i + 10);
    }
    Map_CopyAttributes(0, 0, 2, 2, 9, 22);
    Wait(1);
}

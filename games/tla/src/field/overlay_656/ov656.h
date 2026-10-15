#ifndef TLA_OVERLAY_656_H
#define TLA_OVERLAY_656_H

#include "types.h"
#include "party_state.h"
#include "ram_buffer.h"
#include "scene_ids.h"
#include "text/msg_ids.h"
#include "field_sprite.h"

struct Ov656Object {
    u8 unknown_00[6];
    u16 facing;
    s32 x;
    s32 y;
    s32 z;
    s32 height;
    s32 scale_x;
    s32 scale_y;
    u8 unknown_20[2];
    u8 field_22;
    u8 priority_flags;
    s32 velocity_x;
    s32 velocity_y;
    u8 unknown_2c[0x18];
    s32 field_44;
    s32 field_48;
    u8 unknown_4c[4];
    struct FieldSprite *sprite;
    u8 unknown_54;
    u8 motion_flags;
    u8 unknown_56[3];
    u8 collision_flags;
    u8 unknown_5a[2];
    u8 field_5c;
    u8 unknown_5d[5];
    u8 timer;
    u8 unknown_63;
    s16 field_64;
    s16 field_66;
    s32 field_68;
    void *update;
};

struct Ov656EventWork {
    u8 unknown_000[0x154];
    s16 exit;
    u8 unknown_156[4];
    s16 field_15a;
    u8 unknown_15c[2];
    s16 field_15e;
    u8 unknown_160[6];
    s16 field_166;
    u8 unknown_168[2];
    u16 field_16a;
    u8 unknown_16c[0x40];
    s32 screen_effect;
    u8 unknown_1b0[4];
    s32 field_1b4;
    u8 unknown_1b8[0x0c];
    u16 field_1c4;
};

extern s32 gFrameCount;

u32 Random16Far(void);
void WaitFrames(s32 frames);
s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);
void Battle_WaitMode0(s32 frames);
struct Ov656Object *Object_GetById(s32 id);
void Object_SetPositionAndResetMotion(struct Ov656Object *object, s32 x, s32 y, s32 z);
void Object_SetPartAttribute(struct Ov656Object *object, s32 value);
void ObjectMotion_SetAngleToward(s32 actor, s32 target, s32 frames);
void ObjectMotion_ArmCallback(s32 actor, s32 angle, s32 frames);
void ObjectMotion_SetActionVariant(s32 actor, s32 variant);

void Func_02002c30(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_02002c38(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_02002c68(void);
void Func_02002c70(void);
void Func_02002d18(s32 message);
void Func_02002d20(s32 speaker, s32 flags);
void Func_02002d30(s32 actor, s32 angle);
void Func_02002da8(s32 skip);
void Func_02002dc0(const void *script);
void Func_02002de0(s32 cue);
void Func_02002cd0(s32 actor, s32 x, s32 z);
struct Ov656Object *Func_02002c00(s32 type, s32 x, s32 y, s32 z);
void Func_02002bf0(struct Ov656Object *object, s32 mode);
void ObjectDispatch_SetSingleChildFlags(struct Ov656Object *object, u32 value);
void Animation_SetStateFlags(struct Ov656Object *object, s32 flags);
void Object_SetModeById(s32 actor, s32 mode);
void Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 priority);
void Scheduler_RemoveCallbackFar(void (*callback)(void));
void Field_DispatchTypeHandler(s32 type);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_02002d70(s32 countdown);

void Func_02002db8(const void *script);
void Func_02002c18(struct Ov656Object *object, s32 x, s32 y, s32 z);
void Func_02002c20(struct Ov656Object *object);
void Event_StartScreenTransition(void);

static inline void Ov656_ObjTurn(s32 actor, s32 angle)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Func_02002d30(actor, angle);
}

static inline void Ov656_BlendStart(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f)
{
    /* FAKEMATCH: the blend start reaches the engine through one inline layer. */
    Func_02002c38(a, b, c, d, e, f);
}

static inline s32 Ov656_FlagChk(s32 flag)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    return GameFlag_Test(flag);
}

static inline void Ov656_FlagSet(s32 flag)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    GameFlag_SetBit(flag);
}

static inline void Ov656_ObjArm(s32 actor, s32 angle, s32 frames)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    ObjectMotion_ArmCallback(actor, angle, frames);
}

static inline void Ov656_ObjSetPos(struct Ov656Object *object, s32 x, s32 y, s32 z)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Object_SetPositionAndResetMotion(object, x, y, z);
}

void Motion_CamBounds(s32 x, s32 y, s32 z, s32 mode);
void Func_02002d58(s32 speed, s32 acceleration);
void ObjectMotion_SetSpeedParameters(s32 actor, s32 speed, s32 acceleration);
void ObjectMotion_SetPositionAndReset(s32 actor, s32 x, s32 z);
void Func_02002cd8(s32 actor, s32 target);
void ObjectMotion_EnableActionAndSetCallback(s32 actor, const void *callback);
void Object_SetActionCallbackAndRefreshById(s32 actor, const void *callback);
s32 UiText_OpenMessageAtObject(s32 speaker, s32 flags);
s32 Inventory_PromptAndSetObjectMode(s32 actor, s32 flags);
void Func_02002d40(s32 actor, s32 emote, s32 frames);
void Func_02002d48(s32 actor, s32 emote);
void ObjectMotion_Launch(s32 actor, s32 speed, s32 frames);
void Motion_SetModeAndWaitAnimation(s32 actor, s32 mode);
void ObjectMotion_ResetAndSetPosition(s32 actor, s32 x, s32 z);
void ObjectMotion_CommitCurrentPositionAndActivate(s32 actor);
void Event_PrepareObjectAndApplyValue(s32 actor, s32 value);

static inline void Ov656_CamMove(s32 x, s32 y, s32 z, s32 mode)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Motion_CamBounds(x, y, z, mode);
}

static inline void Ov656_ObjSpeed(s32 actor, s32 speed, s32 acceleration)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    ObjectMotion_SetSpeedParameters(actor, speed, acceleration);
}

static inline void Ov656_ObjWalk(s32 actor, s32 x, s32 z)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    ObjectMotion_SetPositionAndReset(actor, x, z);
}

static inline void Ov656_MapCopy(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Func_02002c30(a, b, c, d, e, f);
}

static inline void Ov656_Wait(s32 frames)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Battle_WaitMode0(frames);
}

static inline void Ov656_SePlay(s32 cue)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Func_02002de0(cue);
}

static inline void Ov656_WaitFrames(s32 frames)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    WaitFrames(frames);
}

#define OV656_TASK_ADD(callback, priority) \
    { \
        /* FAKEMATCH: a temporary keeps the priority constant ahead of the callback load. */ \
        s32 priority_ = (priority); \
        Scheduler_AddOrUpdateCallback((callback), priority_); \
    }

static inline void Ov656_ObjEmoteWait(s32 actor, s32 emote)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Func_02002d48(actor, emote);
}

static inline void Ov656_TaskAdd(void (*callback)(void), s32 priority)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Scheduler_AddOrUpdateCallback(callback, priority);
}

static inline void Ov656_MesPut(s32 speaker, s32 flags)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Func_02002d20(speaker, flags);
}

static inline void Ov656_MesOpen(s32 speaker, s32 flags)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    UiText_OpenMessageAtObject(speaker, flags);
}

void UiText_ShowPositionedMessageAndWait(s32 message, s32 position);
void ObjectMotion_EnableActionAndResetMotion(s32 actor);
void Object_AttachWorkTargetToObject(s32 actor, s32 value);
void Motion_SetVarCbAndRefresh(s32 actor, s32 value);
void ObjectMotion_SetVariantCallback(s32 actor, s32 value);
void ObjectMotion_SetPositionAndCommit(s32 actor, s32 x, s32 z);
void Party_SetFields1eeAnd1f0(s32 scene, s32 entrance);
void Func_02002c10(void);
void Func_02002c48(s32 a, s32 b, s32 c);
void Func_02002d68(s32 a, s32 b, s32 c, s32 d);
void Func_02002d78(s32 a, s32 b);
void Func_02002d88(s32 a, s32 b);
void Func_02002dd0(const void *script);
void Func_02002dd8(void);

static inline void Ov656_ObjCommit(s32 actor, s32 x, s32 z)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    ObjectMotion_SetPositionAndCommit(actor, x, z);
}

static inline void Ov656_CamSpeed(s32 speed, s32 acceleration)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Func_02002d58(speed, acceleration);
}

static inline void Ov656_ObjPlace(s32 actor, s32 x, s32 z)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Func_02002cd0(actor, x, z);
}

static inline void Ov656_CamSet(s32 a, s32 b, s32 c, s32 d)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Func_02002d68(a, b, c, d);
}

// ---- this overlay

void Func_02000d10(s32 id);
void Func_02000e2c(void);
void Func_02001380(s32 id);
void Func_020013b8(s32 id);

extern const u8 Data_02002e30[];
extern const u8 Data_02002e6c[];
extern const u8 Data_02002e9c[];
extern const u8 Data_02003010[];
extern const u8 Data_02003014[];
extern const u8 Data_0200301e[];
extern const u8 Data_02003034[];
extern const u8 Data_0200304e[];

void Func_02001a04(void);
void Func_02001650(void);
void Func_02002bf8(struct Ov656Object *object, const void *script);
void Func_02002dc8(const void *script);
void Vector_AddPolarOffsetFar(s32 radius, s32 angle, s32 *vector);
extern const u8 Data_02003ef8[];

#endif

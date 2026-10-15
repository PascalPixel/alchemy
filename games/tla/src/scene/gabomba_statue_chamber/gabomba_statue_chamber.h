#ifndef TLA_GABOMBA_STATUE_CHAMBER_H
#define TLA_GABOMBA_STATUE_CHAMBER_H

#include "types.h"
#include "party_state.h"
#include "ram_buffer.h"
#include "scene_ids.h"
#include "text/msg_ids.h"
#include "field_sprite.h"

struct GabombaStatueChamberObject {
    u8 unk00[6];
    u16 facing;
    s32 x;
    s32 y;
    s32 z;
    s32 height;
    s32 scaleX;
    s32 scaleY;
    u8 unk20[3];
    u8 priorityFlags;
    s32 velocityX;
    s32 velocityY;
    u8 unk2c[0x18];
    s32 field44;
    s32 field48;
    u8 unk4c[4];
    struct FieldSprite *sprite;
    u8 unk54;
    u8 motionFlags;
    u8 unk56[3];
    u8 collisionFlags;
    u8 unk5a[2];
    u8 field5c;
    u8 unk5d[5];
    u8 timer;
    u8 unk63;
    s16 field64;
    s16 field66;
    s32 field68;
    void *update;
};

struct GabombaStatueChamberEventWork {
    u8 unk000[0x154];
    s16 exit;
    u8 unk156[4];
    s16 field15a;
    u8 unk15c[2];
    s16 field15e;
    u8 unk160[6];
    s16 field166;
    u8 unk168[0x44];
    s32 screenEffect;
    u8 unk1b0[4];
    s32 field1b4;
    u8 unk1b8[0x0c];
    u16 field1c4;
};

extern s32 gFrameCount;

u32 Random16Far(void);
void WaitFrames(s32 frames);
s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);
void Battle_WaitMode0(s32 frames);
struct GabombaStatueChamberObject *Object_GetById(s32 id);
void Object_SetPositionAndResetMotion(struct GabombaStatueChamberObject *object, s32 x, s32 y, s32 z);
void Object_SetPartAttribute(struct GabombaStatueChamberObject *object, s32 value);
void ObjectMotion_SetAngleToward(s32 actor, s32 target, s32 frames);
void ObjectMotion_ArmCallback(s32 actor, s32 angle, s32 frames);
void ObjectMotion_SetActionVariant(s32 actor, s32 variant);

void Func_02001d58(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_02001d68(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_02001da0(void);
void Func_02001da8(void);
void Func_02001e48(s32 message);
void Func_02001e58(s32 actor, s32 a, s32 b);
void Func_02001e60(s32 speaker, s32 flags);
void Func_02001e70(s32 actor, s32 angle);
void Func_02001e80(s32 actor);
void Func_02001ee8(s32 skip);
void Func_02001f10(s32 cue);
void Func_02001e10(s32 actor, s32 x, s32 z);
struct GabombaStatueChamberObject *Func_02001d28(s32 type, s32 x, s32 y, s32 z);
void Func_02001d30(struct GabombaStatueChamberObject *object);
void ObjectDispatch_SetSingleChildFlags(struct GabombaStatueChamberObject *object, u32 value);
void Animation_SetStateFlags(struct GabombaStatueChamberObject *object, s32 flags);
void Object_SetModeById(s32 actor, s32 mode);
void Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 priority);
void Scheduler_RemoveCallbackFar(void (*callback)(void));
void Field_DispatchTypeHandler(s32 type);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_02001ea8(void);
void Func_02001eb0(s32 countdown);

void Func_02001d48(struct GabombaStatueChamberObject *object, s32 x, s32 y, s32 z);
void Func_02001d50(struct GabombaStatueChamberObject *object);
void Event_StartScreenTransition(void);

static inline void GabombaStatueChamber_ObjTurn(s32 actor, s32 angle)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Func_02001e70(actor, angle);
}

static inline void GabombaStatueChamber_BlendStart(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f)
{
    /* FAKEMATCH: the blend start reaches the engine through one inline layer. */
    Func_02001d68(a, b, c, d, e, f);
}

static inline s32 GabombaStatueChamber_FlagCheck(s32 flag)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    return GameFlag_Test(flag);
}

static inline void GabombaStatueChamber_FlagSet(s32 flag)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    GameFlag_SetBit(flag);
}

static inline void GabombaStatueChamber_ObjArm(s32 actor, s32 angle, s32 frames)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    ObjectMotion_ArmCallback(actor, angle, frames);
}

static inline void GabombaStatueChamber_ObjSetPos(struct GabombaStatueChamberObject *object, s32 x, s32 y, s32 z)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Object_SetPositionAndResetMotion(object, x, y, z);
}

void Motion_CamBounds(s32 x, s32 y, s32 z, s32 mode);
void Func_02001e98(s32 speed, s32 acceleration);
void ObjectMotion_SetSpeedParameters(s32 actor, s32 speed, s32 acceleration);
void ObjectMotion_SetPositionAndReset(s32 actor, s32 x, s32 z);
void Func_02001e20(s32 actor, s32 target);
void ObjectMotion_EnableActionAndSetCallback(s32 actor, const void *callback);
void Object_SetActionCallbackAndRefreshById(s32 actor, const void *callback);
s32 UiText_OpenMessageAtObject(s32 speaker, s32 flags);
s32 Inventory_PromptAndSetObjectMode(s32 actor, s32 flags);
void Func_02001e88(s32 actor, s32 emote, s32 frames);
void Func_02001e90(s32 actor, s32 emote);
void ObjectMotion_Launch(s32 actor, s32 speed, s32 frames);
void Motion_SetModeAndWaitAnimation(s32 actor, s32 mode);
void ObjectMotion_ResetAndSetPosition(s32 actor, s32 x, s32 z);
void ObjectMotion_CommitCurrentPositionAndActivate(s32 actor);
void Event_PrepareObjectAndApplyValue(s32 actor, s32 value);

static inline void GabombaStatueChamber_CamMove(s32 x, s32 y, s32 z, s32 mode)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Motion_CamBounds(x, y, z, mode);
}

static inline void GabombaStatueChamber_ObjSpeed(s32 actor, s32 speed, s32 acceleration)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    ObjectMotion_SetSpeedParameters(actor, speed, acceleration);
}

static inline void GabombaStatueChamber_ObjWalk(s32 actor, s32 x, s32 z)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    ObjectMotion_SetPositionAndReset(actor, x, z);
}

static inline void GabombaStatueChamber_ObjPosSet(s32 actor, s32 x, s32 z)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Func_02001e10(actor, x, z);
}

static inline void GabombaStatueChamber_MapCopy(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Func_02001d58(a, b, c, d, e, f);
}

static inline void GabombaStatueChamber_Wait(s32 frames)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Battle_WaitMode0(frames);
}

static inline void GabombaStatueChamber_SePlay(s32 cue)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Func_02001f10(cue);
}

static inline void GabombaStatueChamber_WaitFrames(s32 frames)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    WaitFrames(frames);
}

#define GABOMBA_STATUE_CHAMBER_TASK_ADD(callback, priority) \
    { \
        /* FAKEMATCH: a temporary keeps the priority constant ahead of the callback load. */ \
        s32 priority_ = (priority); \
        Scheduler_AddOrUpdateCallback((callback), priority_); \
    }

static inline void GabombaStatueChamber_ObjEmoteWait(s32 actor, s32 emote)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Func_02001e90(actor, emote);
}

static inline void GabombaStatueChamber_TaskAdd(void (*callback)(void), s32 priority)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Scheduler_AddOrUpdateCallback(callback, priority);
}

static inline void GabombaStatueChamber_MessagePut(s32 speaker, s32 flags)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Func_02001e60(speaker, flags);
}

static inline void GabombaStatueChamber_MessageOpen(s32 speaker, s32 flags)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    UiText_OpenMessageAtObject(speaker, flags);
}

// ---- this overlay

void Func_02000a98(s32 x, s32 z);

extern u8 Data_020020f8[];
extern u8 Data_020021e8[];
extern u8 Data_02002458[];
extern u8 Data_02002470[];
extern u8 Data_020024a0[];
extern u8 Data_020024dc[];
extern u8 Data_02002530[];
extern u8 Data_0200a08c[];

#endif

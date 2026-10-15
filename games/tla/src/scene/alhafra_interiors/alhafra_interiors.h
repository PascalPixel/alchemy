#ifndef TLA_ALHAFRA_INTERIORS_H
#define TLA_ALHAFRA_INTERIORS_H

#include "types.h"
#include "party_state.h"
#include "ram_buffer.h"
#include "scene_ids.h"
#include "text/msg_ids.h"
#include "field_sprite.h"

struct AlhafraInteriorsObject {
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

struct AlhafraInteriorsEventWork {
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
struct AlhafraInteriorsObject *Object_GetById(s32 id);
void Object_SetPositionAndResetMotion(struct AlhafraInteriorsObject *object, s32 x, s32 y, s32 z);
void Object_SetPartAttribute(struct AlhafraInteriorsObject *object, s32 value);
void ObjectMotion_SetAngleToward(s32 actor, s32 target, s32 frames);
void ObjectMotion_ArmCallback(s32 actor, s32 angle, s32 frames);
void ObjectMotion_SetActionVariant(s32 actor, s32 variant);

void Func_02000a94(void);
void Func_02000a9c(void);
void Func_02000b24(s32 message);
void Func_02000b34(s32 actor, s32 a, s32 b);
void Func_02000b3c(s32 speaker, s32 flags);
void Func_02000b4c(s32 actor, s32 angle);
void Func_02000b8c(s32 skip);
void Func_02000bc4(s32 shop, s32 keeper);
void Func_02000bcc(s32 keeper);
void Func_02000bd4(s32 shop, s32 keeper);
void Func_02000bdc(s32 cue);
void Func_02000aec(s32 actor, s32 x, s32 z);
void ObjectDispatch_SetSingleChildFlags(struct AlhafraInteriorsObject *object, u32 value);
void Animation_SetStateFlags(struct AlhafraInteriorsObject *object, s32 flags);
void Object_SetModeById(s32 actor, s32 mode);
void Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 priority);
void Scheduler_RemoveCallbackFar(void (*callback)(void));
void Field_DispatchTypeHandler(s32 type);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_02000b7c(void);

void Event_StartScreenTransition(void);

static inline void AlhafraInteriors_ObjTurn(s32 actor, s32 angle)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Func_02000b4c(actor, angle);
}

static inline s32 AlhafraInteriors_FlagCheck(s32 flag)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    return GameFlag_Test(flag);
}

static inline void AlhafraInteriors_FlagSet(s32 flag)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    GameFlag_SetBit(flag);
}

static inline void AlhafraInteriors_ObjArm(s32 actor, s32 angle, s32 frames)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    ObjectMotion_ArmCallback(actor, angle, frames);
}

static inline void AlhafraInteriors_ObjSetPos(struct AlhafraInteriorsObject *object, s32 x, s32 y, s32 z)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Object_SetPositionAndResetMotion(object, x, y, z);
}

void Motion_CamBounds(s32 x, s32 y, s32 z, s32 mode);
void ObjectMotion_SetSpeedParameters(s32 actor, s32 speed, s32 acceleration);
void ObjectMotion_SetPositionAndReset(s32 actor, s32 x, s32 z);
void ObjectMotion_EnableActionAndSetCallback(s32 actor, const void *callback);
void Object_SetActionCallbackAndRefreshById(s32 actor, const void *callback);
s32 UiText_OpenMessageAtObject(s32 speaker, s32 flags);
s32 Inventory_PromptAndSetObjectMode(s32 actor, s32 flags);
void Func_02000b5c(s32 actor, s32 emote, s32 frames);
void Func_02000b64(s32 actor, s32 emote);
void ObjectMotion_Launch(s32 actor, s32 speed, s32 frames);
void Motion_SetModeAndWaitAnimation(s32 actor, s32 mode);
void ObjectMotion_ResetAndSetPosition(s32 actor, s32 x, s32 z);
void ObjectMotion_CommitCurrentPositionAndActivate(s32 actor);
void Event_PrepareObjectAndApplyValue(s32 actor, s32 value);

static inline void AlhafraInteriors_CamMove(s32 x, s32 y, s32 z, s32 mode)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Motion_CamBounds(x, y, z, mode);
}

static inline void AlhafraInteriors_ObjSpeed(s32 actor, s32 speed, s32 acceleration)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    ObjectMotion_SetSpeedParameters(actor, speed, acceleration);
}

static inline void AlhafraInteriors_ObjWalk(s32 actor, s32 x, s32 z)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    ObjectMotion_SetPositionAndReset(actor, x, z);
}

static inline void AlhafraInteriors_Wait(s32 frames)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Battle_WaitMode0(frames);
}

static inline void AlhafraInteriors_SePlay(s32 cue)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Func_02000bdc(cue);
}

static inline void AlhafraInteriors_WaitFrames(s32 frames)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    WaitFrames(frames);
}

#define ALHAFRA_INTERIORS_TASK_ADD(callback, priority) \
    { \
        /* FAKEMATCH: a temporary keeps the priority constant ahead of the callback load. */ \
        s32 priority_ = (priority); \
        Scheduler_AddOrUpdateCallback((callback), priority_); \
    }

static inline void AlhafraInteriors_ObjEmoteWait(s32 actor, s32 emote)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Func_02000b64(actor, emote);
}

static inline void AlhafraInteriors_TaskAdd(void (*callback)(void), s32 priority)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Scheduler_AddOrUpdateCallback(callback, priority);
}

static inline void AlhafraInteriors_MessagePut(s32 speaker, s32 flags)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Func_02000b3c(speaker, flags);
}

static inline void AlhafraInteriors_MessageOpen(s32 speaker, s32 flags)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    UiText_OpenMessageAtObject(speaker, flags);
}

s32 Func_02000ba4(void);

// ---- this overlay

extern u8 gData_02000cbc[];
extern u8 gData_02000f14[];
extern u8 gData_0200116c[];
extern u8 gData_020013dc[];
extern u8 gData_02001730[];
extern u8 gData_020019dc[];

#endif

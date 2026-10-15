#ifndef TLA_OVERLAY_670_H
#define TLA_OVERLAY_670_H

#include "types.h"
#include "party_state.h"
#include "ram_buffer.h"
#include "scene_ids.h"
#include "text/msg_ids.h"
#include "field_sprite.h"

struct Ov670Object {
    u8 unknown_00[6];
    u16 facing;
    s32 x;
    s32 y;
    s32 z;
    s32 height;
    s32 scale_x;
    s32 scale_y;
    u8 unknown_20[3];
    u8 priority_flags;
    s32 velocity_x;
    s32 velocity_y;
    u8 unknown_2c[0x0c];
    s32 target_x;
    s32 target_y;
    s32 target_z;
    s32 field_44;
    s32 field_48;
    s32 field_4c;
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

struct Ov670EventWork {
    u8 unknown_000[0x154];
    s16 exit;
    u8 unknown_156[4];
    s16 field_15a;
    u8 unknown_15c[2];
    s16 field_15e;
    u8 unknown_160[6];
    s16 field_166;
    u8 unknown_168[0x44];
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
struct Ov670Object *Object_GetById(s32 id);
void Object_SetPositionAndResetMotion(struct Ov670Object *object, s32 x, s32 y, s32 z);
void Object_SetPartAttribute(struct Ov670Object *object, s32 value);
void ObjectMotion_SetAngleToward(s32 actor, s32 target, s32 frames);
void ObjectMotion_ArmCallback(s32 actor, s32 angle, s32 frames);
void ObjectMotion_SetActionVariant(s32 actor, s32 variant);

void Func_02005a64(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_02005a6c(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_02005a9c(void);
void Func_02005aa4(void);
void Func_02005b4c(s32 message);
void Func_02005b5c(s32 actor, s32 a, s32 b);
void Func_02005b64(s32 speaker, s32 flags);
s32 Func_02005b6c(s32 speaker, s32 flags);
void Func_02005b7c(s32 actor, s32 angle);
void Func_02005b8c(s32 actor);
void Func_02005c3c(s32 skip);
void Func_02005c6c(const void *script);
void Func_02005c84(s32 shop, s32 keeper);
void Func_02005c8c(s32 keeper);
void Func_02005c94(s32 shop, s32 keeper);
void Func_02005c9c(s32 cue);
void Func_02005b0c(s32 actor, s32 x, s32 z);
struct Ov670Object *Func_02005a34(s32 type, s32 x, s32 y, s32 z);
void Func_02005a24(struct Ov670Object *object, s32 mode);
void Func_02005a3c(struct Ov670Object *object);
void ObjectDispatch_SetSingleChildFlags(struct Ov670Object *object, u32 value);
void Animation_SetStateFlags(struct Ov670Object *object, s32 flags);
void Object_SetModeById(s32 actor, s32 mode);
void Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 priority);
void Scheduler_RemoveCallbackFar(void (*callback)(void));
void Field_DispatchTypeHandler(s32 type);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_02005bb4(void);
void Func_02005bc4(s32 countdown);
void Func_02005c14(s32 a, s32 b);
s32 Func_02005c1c(s32 a, s32 b);
void Func_02005c24(void);
void Func_02005c2c(void);
void Func_02005c34(void);

void Func_02005bcc(void);
void Func_02005c04(s32 a);
void Func_02005c54(void);
void Func_02005c5c(s32 a, s32 b, s32 c, s32 d);
void Func_02005c64(const void *script);
void Func_02005a54(struct Ov670Object *object, s32 x, s32 y, s32 z);
void Func_02005a5c(struct Ov670Object *object);
void Event_StartScreenTransition(void);

static inline void Ov670_ObjTurn(s32 actor, s32 angle)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Func_02005b7c(actor, angle);
}

static inline void Ov670_BlendStart(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f)
{
    /* FAKEMATCH: the blend start reaches the engine through one inline layer. */
    Func_02005a6c(a, b, c, d, e, f);
}

static inline s32 Ov670_FlagCheck(s32 flag)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    return GameFlag_Test(flag);
}

static inline void Ov670_FlagSet(s32 flag)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    GameFlag_SetBit(flag);
}

static inline void Ov670_ObjArm(s32 actor, s32 angle, s32 frames)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    ObjectMotion_ArmCallback(actor, angle, frames);
}

static inline void Ov670_ObjSetPos(struct Ov670Object *object, s32 x, s32 y, s32 z)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Object_SetPositionAndResetMotion(object, x, y, z);
}

void Motion_CamBounds(s32 x, s32 y, s32 z, s32 mode);
struct Ov670Object *Func_02005bbc(void);
void Func_02005ba4(s32 speed, s32 acceleration);
void ObjectMotion_SetSpeedParameters(s32 actor, s32 speed, s32 acceleration);
void ObjectMotion_SetPositionAndReset(s32 actor, s32 x, s32 z);
void Func_02005b1c(s32 actor, s32 target);
void ObjectMotion_EnableActionAndSetCallback(s32 actor, const void *callback);
void Object_SetActionCallbackAndRefreshById(s32 actor, const void *callback);
s32 UiText_OpenMessageAtObject(s32 speaker, s32 flags);
s32 Inventory_PromptAndSetObjectMode(s32 actor, s32 flags);
struct Ov670Object *Func_02005b94(s32 actor, s32 emote, s32 frames);
void Func_02005b9c(s32 actor, s32 emote);
void ObjectMotion_Launch(s32 actor, s32 speed, s32 frames);
void Motion_SetModeAndWaitAnimation(s32 actor, s32 mode);
void ObjectMotion_ResetAndSetPosition(s32 actor, s32 x, s32 z);
void ObjectMotion_CommitCurrentPositionAndActivate(s32 actor);
void Event_PrepareObjectAndApplyValue(s32 actor, s32 value);

static inline void Ov670_CamMove(s32 x, s32 y, s32 z, s32 mode)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Motion_CamBounds(x, y, z, mode);
}

static inline void Ov670_CamSpeed(s32 speed, s32 acceleration)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Func_02005ba4(speed, acceleration);
}

static inline void Ov670_ObjSpeed(s32 actor, s32 speed, s32 acceleration)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    ObjectMotion_SetSpeedParameters(actor, speed, acceleration);
}

static inline void Ov670_ObjWalk(s32 actor, s32 x, s32 z)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    ObjectMotion_SetPositionAndReset(actor, x, z);
}

void Func_02005b14(s32 actor, s32 x, s32 z, s32 angle);

static inline void Ov670_ObjPlaceFacing(s32 actor, s32 x, s32 z, s32 angle)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Func_02005b14(actor, x, z, angle);
}

void ObjectMotion_ResetAndSetPositionInMode2(s32 actor, s32 x, s32 z);

static inline void Ov670_ObjPlace(s32 actor, s32 x, s32 z)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    ObjectMotion_ResetAndSetPositionInMode2(actor, x, z);
}

static inline void Ov670_MapCopy(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Func_02005a64(a, b, c, d, e, f);
}

static inline void Ov670_Wait(s32 frames)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Battle_WaitMode0(frames);
}

static inline void Ov670_SePlay(s32 cue)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Func_02005c9c(cue);
}

void Func_02005bd4(s32 a, s32 b);

static inline void Ov670_FadeTo(s32 a, s32 b)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Func_02005bd4(a, b);
}

static inline void Ov670_WaitFrames(s32 frames)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    WaitFrames(frames);
}

#define OV670_TASK_ADD(callback, priority) \
    { \
        /* FAKEMATCH: a temporary keeps the priority constant ahead of the callback load. */ \
        s32 priority_ = (priority); \
        Scheduler_AddOrUpdateCallback((callback), priority_); \
    }

static inline void Ov670_ObjEmote(s32 actor, s32 emote, s32 frames)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Func_02005b94(actor, emote, frames);
}

static inline void Ov670_ObjEmoteWait(s32 actor, s32 emote)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Func_02005b9c(actor, emote);
}

static inline void Ov670_TaskAdd(void (*callback)(void), s32 priority)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Scheduler_AddOrUpdateCallback(callback, priority);
}

static inline void Ov670_MessagePut(s32 speaker, s32 flags)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Func_02005b64(speaker, flags);
}

static inline void Ov670_MessageOpen(s32 speaker, s32 flags)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    UiText_OpenMessageAtObject(speaker, flags);
}

// ---- this overlay

void Func_020050ac(void);
s32 Func_02002b08(s32 value);
void Func_020000a0(struct Ov670Object *object);
void Func_02000524(s32 show);
void Func_0200057c(void);
void Func_02000590(s32 show, s32 first, u32 count);
void Func_020005e0(void);
void Func_02000624(void);
void Func_02000668(void);
void Func_020006ac(void);
void Func_020006f0(void);
void Func_02000978(void);
void Func_02000b2c(void);
void Func_02000d74(void);
void Func_02001318(void);
void Func_02002b18(s32 index, s32 entry, s32 delta);
void Func_02002ba4(s32 index, s32 entry, s32 delta);
void Func_02002c34(void);
void Func_02002ccc(void);
void Func_02002d64(void);
void Func_02002f34(void);
void Func_02003030(void);
void Func_02003184(void);
void Func_020031f0(s32 id);
void Func_02003220(void);
void Func_02003504(void);
void Func_02003640(s32 open, s32 hide);
void Func_020036e0(s32 open, s32 mode);
void Func_02003898(void);
void Func_020038ec(void);
void Func_02003a08(s32 x, s32 z);
void Func_02004524(void);
void Func_020045d0(void);
void Func_02005114(void);
void Func_0200516c(void);
void Func_020054e0(void);
void Func_02005594(void);
void Func_020057e0(void);
void Func_02005898(void);

extern const u8 Data_02005cd0[];
extern const u8 Data_02005d18[];
extern const u8 Data_02005d5c[];
extern const u8 Data_02005da0[];
extern const u8 Data_02005e54[];
extern const u8 Data_0200613c[];
extern const u8 Data_0200614c[];
extern const u8 Data_02006324[];
extern s16 Data_02007948[];
extern struct Ov670Object *Data_02007950;

#endif

#ifndef TLA_OVERLAY_654_KABE_H
#define TLA_OVERLAY_654_KABE_H

#include "types.h"
#include "party_state.h"
#include "ram_buffer.h"

struct KabeObject {
    u8 unknown_00[6];
    u16 facing;
    s32 x;
    s32 y;
    s32 z;
    s32 height;
    s32 field_18;
    s32 field_1c;
    u8 unknown_20[2];
    u8 layer;
    u8 priority_flags;
    u8 unknown_24[4];
    s32 speed;
    u8 unknown_2c[0x24];
    void *sprite;
    u8 unknown_54;
    u8 motion_flags;
    u8 unknown_56[3];
    u8 collision_flags;
    u8 unknown_5a[8];
    u8 state;
    u8 unknown_63[9];
    void (*update)(struct KabeObject *object);
};

struct KabeSprite {
    u8 unknown_00[0x12];
    u16 rotation;
};

struct KabeEventWork {
    u8 unknown_000[0x154];
    s16 exit;
    u8 unknown_156[4];
    u16 unknown_15a;
    u8 unknown_15c[0x50];
    s32 screen_effect;
};

struct KabeMapLayer {
    u8 unknown_00[8];
    s32 x;
    s32 y;
    u8 unknown_10[0x28];
};

struct KabeMapWork {
    u8 unknown_000[0x17];
    u8 low_17 : 2;
    u8 mode_17 : 2;
    u8 high_17 : 4;
    u8 unknown_018[0xf0];
    struct KabeMapLayer layers[3];
};

struct KabeObject *Object_GetById(s32 id);
void WaitFrames(s32 frames);
void Battle_WaitMode0(s32 frames);
s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);
void UiText_ShowPositionedMessageAndWait(s32 message, s32 position);
void ObjectMotion_SetActionVariant(s32 actor, s32 variant);
void ObjectMotion_SetSpeedParameters(s32 actor, s32 speed, s32 acceleration);
void ObjectMotion_SetPositionAndReset(s32 actor, s32 x, s32 z);
void ObjectMotion_ArmCallback(s32 actor, s32 angle, s32 frames);
void ObjectMotion_Launch(s32 actor, s32 speed, s32 frames);
void Object_SetPositionAndResetMotion(struct KabeObject *object, s32 x, s32 y, s32 z);
void Object_SetModeById(s32 actor, s32 mode);
void Func_020039d8(void);
void Func_020039e0(void);
void Func_02003b08(s32 skip);
void Func_02003aa0(s32 actor);
void Func_02003998(s32 src_x, s32 src_y, s32 width, s32 height, s32 dest_x, s32 dest_y);
void Func_020039a8(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_02003ae0(s32 actor, s32 value);
void Func_02003b28(void);
void Func_02003b38(const void *script);
void Func_02003b40(void);
void Func_02003b48(s32 cue);
void Func_02001200(s32 show);
void Func_0200142c(struct KabeObject *object);

static inline void Kabe_CopyRect(s32 sx, s32 sy, s32 w, s32 h, s32 dx, s32 dy)
{
    /* FAKEMATCH: the rectangle copy reaches the engine through one inline layer. */
    Func_02003998(sx, sy, w, h, dx, dy);
}

static inline void Kabe_BlendStart(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f)
{
    /* FAKEMATCH: the blend start reaches the engine through one inline layer. */
    Func_020039a8(a, b, c, d, e, f);
}

void Func_02003a30(s32 actor, s32 x, s32 z);
void Func_02003a38(s32 actor, s32 x, s32 z, s32 angle);
void Func_02003a70(s32 message);
void Func_02003a78(s32 actor, s32 a, s32 b);
void Func_02003a80(s32 speaker, s32 flags);
void Func_02003aa8(s32 actor, s32 height, s32 frames);
void Func_02003ab0(s32 actor, s32 emote);
void Func_02003ab8(s32 speed, s32 acceleration);
void Func_02003ac8(void);
struct KabeObject *Func_02003ad0(void);
void ObjectMotion_ResetAndSetPosition(s32 actor, s32 x, s32 z);
void ObjectMotion_CommitCurrentPositionAndActivate(s32 actor);
void ObjectMotion_EnableActionAndResetMotion(s32 actor);
void ObjectMotion_EnableActionAndSetCallback(s32 actor, const void *callback);
void ObjectMotion_SetAngleToward(s32 actor, s32 target, s32 frames);
void Object_LinkObjectAndSetCallback(s32 actor, s32 target);
void Motion_CamBounds(s32 x, s32 y, s32 z, s32 mode);
void Motion_SetModeAndWaitAnimation(s32 actor, s32 mode);
void Motion_SetVarCbAndRefresh(s32 actor, s32 value);
void ObjectMotion_SetPositionAndCommit(s32 actor, s32 x, s32 z);
void ObjectDispatch_SetSingleChildFlags(struct KabeObject *object, u32 value);

static inline void Kabe_Jump(s32 actor, s32 height, s32 frames)
{
    /* FAKEMATCH: the native loads the actor after the height and frames. */
    register s32 h __asm__("r1") = height;
    /* FAKEMATCH: the frames stay fixed in r2 for the same reason. */
    register s32 f __asm__("r2") = frames;

    Func_02003aa8(actor, h, f);
}

static inline void Kabe_ObjSpeed(s32 actor, s32 speed, s32 acceleration)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    ObjectMotion_SetSpeedParameters(actor, speed, acceleration);
}

static inline void Kabe_ObjWalk(s32 actor, s32 x, s32 z)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    ObjectMotion_SetPositionAndReset(actor, x, z);
}

static inline void Kabe_ObjCommit(s32 actor, s32 x, s32 z)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    ObjectMotion_SetPositionAndCommit(actor, x, z);
}

static inline void Kabe_ObjTurn(s32 actor, s32 angle, s32 frames)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    ObjectMotion_ArmCallback(actor, angle, frames);
}

static inline void Kabe_ObjPlace(s32 actor, s32 x, s32 z)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Func_02003a30(actor, x, z);
}

static inline void Kabe_ObjSetPos(struct KabeObject *object, s32 x, s32 y, s32 z)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Object_SetPositionAndResetMotion(object, x, y, z);
}

static inline void Kabe_CamSpeed(s32 speed, s32 acceleration)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Func_02003ab8(speed, acceleration);
}

static inline void Kabe_CamMove(s32 x, s32 y, s32 z, s32 mode)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Motion_CamBounds(x, y, z, mode);
}

static inline void Kabe_ObjLaunch(s32 actor, s32 speed, s32 frames)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    ObjectMotion_Launch(actor, speed, frames);
}

static inline void Kabe_MesPut(s32 speaker, s32 flags)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Func_02003a80(speaker, flags);
}

static inline void Kabe_ObjModeWait(s32 actor, s32 mode)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Motion_SetModeAndWaitAnimation(actor, mode);
}

static inline void Kabe_Wait(s32 frames)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Battle_WaitMode0(frames);
}

static inline void Kabe_ObjVar(s32 actor, s32 value)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Motion_SetVarCbAndRefresh(actor, value);
}

static inline void Kabe_ObjMode(s32 actor, s32 mode)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Object_SetModeById(actor, mode);
}

static inline void Kabe_ObjEmote(s32 actor, s32 emote)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Func_02003ab0(actor, emote);
}

void Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 priority);

static inline void Kabe_TaskAdd(void (*callback)(void), s32 priority)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    s32 p = priority;

    Scheduler_AddOrUpdateCallback(callback, p);
}

#define KABE_TASK_ADD_LATE(callback, priority) \
    do { \
        /* FAKEMATCH: the native builds the priority before it loads the callback. */ \
        register s32 priority_ __asm__("r1"); \
        priority_ = (priority); \
        /* FAKEMATCH: a barrier keeps the callback load after the priority. */ \
        __asm__ volatile("" : : "r"(priority_)); \
        Scheduler_AddOrUpdateCallback((callback), priority_); \
    } while (0)

static inline s32 Kabe_FlagChk(s32 flag)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    return GameFlag_Test(flag);
}

static inline void Kabe_FlagSet(s32 flag)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    GameFlag_SetBit(flag);
}

static inline void Kabe_ObjPlaceAt(s32 actor, s32 x, s32 z, s32 angle)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Func_02003a38(actor, x, z, angle);
}

static inline void Kabe_ObjPlaceTurn(s32 actor, s32 x, s32 z, s32 angle)
{
    /* FAKEMATCH: the native loads the angle before the position. */
    register s32 g __asm__("r3") = angle;

    Func_02003a38(actor, x, z, g);
}

#endif

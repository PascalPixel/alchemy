#ifndef TLA_OVERLAY_686_OV686_H
#define TLA_OVERLAY_686_OV686_H

#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "party_state.h"
#include "field_scene.h"
#include "scene_ids.h"
#include "field_sprite.h"

s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);
void WaitFrames(s32 frames);
struct ObjectRuntime *Object_GetById(s32 id);
void ObjectMotion_SetActionVariant(u32 id, s32 variant);
s32 Random16Far(void);


extern s16 Data_02003780;
extern s16 Data_02003782;
extern s16 Data_02003784;
extern s32 Data_02003788;
extern s32 Data_0200378c;

s32 Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 priority);
void Battle_WaitMode0(s32 frames);
void Motion_CamBounds(s32 x, s32 y, s32 z, s32 mode);
void ObjectMotion_OffsetPositionAndResetMotion(s32 actor, s32 x, s32 z);
void ObjectMotion_CommitCurrentPositionAndActivate(s32 actor);
void Func_02000b30(s32 id, s32 cue);
void Func_0200160c(void);
void Func_020019a0(void);
void Func_02001f7c(void);
s32 Func_02002dcc(void (*callback)(void));
void Func_02002df4(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_02002dfc(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_02002e24(s32 a, s32 b, s32 c);
void Func_02002e44(void);
void Func_02002e4c(void);
void Func_02002e94(s32 actor, s32 a, s32 b);
void Func_02002e9c(s32 actor, s32 a);
void Func_02002edc(s32 skip);
void Func_02002eec(s32 cue);

static inline s32 Ov686_FlagChk(s32 flag)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    return GameFlag_Test(flag);
}

static inline void Ov686_FlagSet(s32 flag)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    GameFlag_SetBit(flag);
}

static inline void Ov686_Call2e24(s32 a, s32 b, s32 c)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Func_02002e24(a, b, c);
}

static inline void Ov686_CamBounds(s32 x, s32 y, s32 z, s32 mode)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Motion_CamBounds(x, y, z, mode);
}

static inline void Ov686_Call2df4(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Func_02002df4(a, b, c, d, e, f);
}

static inline void Ov686_Call2dfc(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Func_02002dfc(a, b, c, d, e, f);
}

static inline void Ov686_Call2e94(s32 actor, s32 a, s32 b)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Func_02002e94(actor, a, b);
}

static inline void Ov686_ObjOffset(s32 actor, s32 x, s32 z)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    ObjectMotion_OffsetPositionAndResetMotion(actor, x, z);
}

void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object, s32 flags);
void Object_AttachWorkTargetToObject(s32 actor, s32 target);
void Func_02002e34(struct ObjectRuntime *object, s32 a);
void Func_02002e6c(s32 id, s32 a, s32 b);
void Func_02002eb4(void);

#endif

#ifndef TLA_MADRA_INTERIORS_H
#define TLA_MADRA_INTERIORS_H

#include "types.h"
#include "object_runtime.h"
#include "party_state.h"
#include "ram_buffer.h"
#include "scene_ids.h"
#include "text/msg_ids.h"

#define FIELD_AT(base, type, off) \
    (((struct {                   \
        u8   pad[off];            \
        type v;                   \
    } *)(base))                   \
            ->v)

extern const u8 gData_02002240[];
extern const u8 gData_02002480[];
extern const u8 gData_02002750[];
extern const u8 gData_02002828[];
extern const u8 gData_020028a0[];
extern const u8 gData_02002bb8[];
extern const u8 gData_02002c9c[];
extern const u8 gData_02002da4[];
extern const u8 gData_0200308c[];

struct ObjectRuntime *Object_GetById(s32 id);
s32                   GameFlag_Test(s32 flag);
void                  GameFlag_SetBit(s32 flag);
void                  Battle_WaitMode0(s32 frames);
void                  Func_02001d50(void);
void                  Func_02001d58(void);
void                  Func_02001eb8(s32 skip);
void                  Func_02001e18(s32 message);
void                  Func_02001e30(s32 speaker, s32 flags);
void                  Func_02001e48(s32 actor, s32 animation, s32 frames);
s32                   Func_02001ee8(void);
void                  Func_02001f00(s32 actor);
void                  Func_02001f08(s32 slot, s32 actor);
void                  Func_02001d38(struct ObjectRuntime *object, s32 value);
void                  Func_02001db8(s32 actor, s32 x, s32 z);
void                  Func_02001dc0(s32 actor, s32 x, s32 z, s32 angle);
void                  Func_020007dc(void);
void                  ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object, s32 flags);
void                  ObjectMotion_SetAngleToward(s32 actor, s32 target, s32 frames);
void                  ObjectMotion_Launch(s32 actor, s32 speed, s32 frames);
void                  ObjectMotion_ArmCallback(s32 actor, s32 angle, s32 frames);
void                  ObjectMotion_CommitPositionAndActivate(s32 actor, s32 x, s32 z);
void                  Motion_SetVarCbAndRefresh(s32 actor, s32 variant);
void                  UiText_OpenMessageAtObject(s32 speaker, s32 flags);
s32                   Inventory_PromptAndSetObjectMode(s32 actor, s32 flags);

static inline s32 FlagCheck(s32 flag)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    return GameFlag_Test(flag);
}

static inline void FlagSet(s32 flag)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    GameFlag_SetBit(flag);
}

static inline void ActorTurn(s32 actor, s32 angle, s32 frames)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    ObjectMotion_ArmCallback(actor, angle, frames);
}

static inline void ActorPlace(s32 actor, s32 x, s32 z)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Func_02001db8(actor, x, z);
}

static inline void ActorPlaceFacing(s32 actor, s32 x, s32 z, s32 angle)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Func_02001dc0(actor, x, z, angle);
}

#endif

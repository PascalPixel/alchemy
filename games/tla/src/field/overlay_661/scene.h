#ifndef TLA_OVERLAY_661_SCENE_H
#define TLA_OVERLAY_661_SCENE_H

#include "types.h"
#include "party_state.h"
#include "ram_buffer.h"
#include "field_event.h"
#include "scene_ids.h"

struct SceneEventWork {
    u8 unknown_000[0x16a];
    u16 unknown_16a;
    u8 unknown_16c[0x40];
    s32 screen_effect;
};

extern const u8 Data_02004768[];
extern const u8 Data_0200478e[];
extern const u8 Data_020047d2[];
extern const u8 Data_020047ec[];
extern const u8 Data_02004800[];
extern s32 Data_02004808;

struct FieldActor *Object_GetById(s32 id);
void WaitFrames(s32 frames);
s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);
void UiText_ShowPositionedMessageAndWait(s32 message, s32 position);
void Object_SetPartAttribute(struct FieldActor *object, s32 value);
void Object_SetWideSprite(s32 actor, s32 value);
s32 Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 priority);
void Scheduler_RemoveCallbackFar(void (*callback)(void));
void Field_DispatchTypeHandler(s32 type);

void Func_020006ac(void);
void Func_02001594(void);
void Func_020015ec(s32 show);
void Func_0200163c(void);
void Func_02001650(s32 show, s32 first, u32 count);
void Func_02001e58(void);
void Func_02002010(void);
void Func_02002c6c(void);
void Func_02002ce0(s32 id);
void Func_02002d10(void);
void Func_02002d54(void);
void Func_02002d6c(struct FieldActor *object, s32 x, s32 z, s32 mode);
void Func_020032c4(void);
void Func_02003820(const void *script);
void Func_0200388c(const void *script, s32 a, s32 b);
void Func_02003ff0(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_02004000(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_02004040(void);
void Func_02004048(void);
void Func_02004090(s32 actor, s32 x, s32 z);
void Func_020040c8(s32 message);
s32 Func_020040d8(s32 speaker, s32 flags);
void Func_020040f8(s32 actor);
void Func_02004148(s32 a, s32 b);
void Func_02004178(s32 a, s32 b, s32 c);
void Func_02004180(s32 value);
void Func_02004190(s32 a, s32 b);
void Func_02004198(s32 a, s32 b);
void Func_020041a0(void);
void Func_020041a8(void);
void Func_020041b0(void);
void Func_020041b8(s32 skip);
void Func_020041c8(s32 cue);
s32 Func_020041e0(struct FieldActor *object);
void Func_020040d0(s32 actor, s32 flags);
void Func_020004e0(void);
void Func_02000918(u16 *list);
void Func_0200095c(u16 *list);
void ObjectMotion_SetAngleToward(s32 id, s32 target, s32 flags);
void ObjectMotion_ArmCallback(s32 id, s32 angle, s32 flags);
void Object_AttachWorkTargetToObject(s32 id, s32 mode);
void Object_LinkObjectAndSetCallback(s32 id, s32 target);
void Event_StartScreenTransition(void);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Battle_WaitMode0(s32 frames);
void PartyInventory_GiveItem(s32 item, s32 flags);
struct FieldActor *Func_02001b70(void);
void Func_02003fc8(struct FieldActor *object);
void Func_020041f8(s32 cue);
extern struct FieldActor *Data_0200480c;
extern s32 gInput;
extern u32 gFrameCount;
void ObjectMotion_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
void ObjectMotion_SetPositionAndReset(s32 actor, s32 x, s32 z);
void ObjectDispatch_SetSingleChildFlags(struct FieldActor *object, s32 flags);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
void Func_02003fe0(struct FieldActor *object, s32 x, s32 y, s32 z);
void Func_02003fe8(struct FieldActor *object);
void Func_020040e8(s32 actor, s32 angle);
void Func_02004138(s32 mode);
void Func_02004118(s32 a, s32 b);
void Motion_CamBounds(s32 x, s32 a, s32 z, s32 b);
void ObjectMotion_SetPositionAndCommit(s32 id, s32 x, s32 z);
void ObjectMotion_EnableActionAndResetMotion(s32 id);
void Func_020021d4(void);
void Func_02002180(void);
void Func_02002318(void);
void Func_02003338(s32 id);
void Func_020020a8(void);
void Func_02003fa8(struct FieldActor *object, s32 mode);
void Func_02003910(s32 id, u32 kind, s32 flag, s32 layer);

static inline void Scene_TaskAdd(void (*callback)(void), s32 priority)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Scheduler_AddOrUpdateCallback(callback, priority);
}

static inline s32 Scene_FlagTest(s32 flag)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    return GameFlag_Test(flag);
}

static inline void Scene_FlagSet(s32 flag)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    GameFlag_SetBit(flag);
}

static inline void Scene_FlagClear(s32 flag)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    GameFlag_ClearBit(flag);
}

static inline void Scene_ArmCallback(s32 id, s32 angle, s32 flags)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    ObjectMotion_ArmCallback(id, angle, flags);
}

static inline void Scene_Call4090(s32 actor, s32 x, s32 z)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Func_02004090(actor, x, z);
}

static inline void Scene_SetSpeed(s32 id, s32 speed, s32 acceleration)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    ObjectMotion_SetSpeedParameters(id, speed, acceleration);
}

static inline void Scene_CamBounds(s32 x, s32 a, s32 z, s32 b)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Motion_CamBounds(x, a, z, b);
}

static inline void Scene_SetPositionAndCommit(s32 id, s32 x, s32 z)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    ObjectMotion_SetPositionAndCommit(id, x, z);
}

static inline void Scene_Call4178(s32 a, s32 b, s32 c)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Func_02004178(a, b, c);
}

static inline void Scene_SetPositionAndReset(s32 id, s32 x, s32 z)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    ObjectMotion_SetPositionAndReset(id, x, z);
}

#endif

#ifndef ALCHEMY_FIELD_EVENT_H
#define ALCHEMY_FIELD_EVENT_H

/// @file
/// The scene script layer: the engine services a scene's events call, declared
/// once (Engine_EventBegin, Engine_ActorJump, Engine_EventSetMessage and the
/// rest), with the constants and small inlines scenes share.
///
/// It is a habit of the whole game written once, as declarations and macros: a
/// layer, not a compiler-steering device (AGENTS.md S2). The event.h commands
/// and field_service.h are written over it. A scene's event reads `EV_BEGIN();
/// Engine_ActorStop(9); OBJ_FACE(9, ACTOR_PARTY_LEADER, 0); EV_END();`.

#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "mapcopy.h"

#include "static_assert.h"

#include "game_state.h"

// A scene stores into its own const tables: an overlay's data is in RAM.
// The games' compiler takes the store as written; a current GCC refuses it,
// so the modern build names the place without its const.
#ifdef MODERN
/// The comma expression removes qualifiers without promoting narrow integers.
#define STORE_CONST(place) (*(__typeof__(((void)0, (place))) *)&(place))
#else
#define STORE_CONST(place) (place)
#endif

extern u8 gSceneState[];

extern u8 *gColossoWork;

#include "frame.h"

#include "field_obj.h"

enum { TASK_PRIORITY_SCENE = 3200 };

enum ActorAnimation {
  ANIM_STAND = 1,
  ANIM_WALK = 2,
  ANIM_NOD = 3,
  ANIM_SHAKE_HEAD = 4
};

#define EMOTE_IN_FRONT 0x100

// Speaker bits above the actor id in a message command, as
// UiText_OpenMessageAtObject reads them: the portrait left or right of the
// speaker, and the message window above it or never above it.
#define SPEAKER_PORTRAIT_LEFT 0x1000
#define SPEAKER_PORTRAIT_RIGHT 0x2000
#define SPEAKER_WINDOW_ABOVE 0x4000
#define SPEAKER_WINDOW_BELOW 0x8000

#define MAP_CELL_STEP(srcX, srcY, width, height, frames)                       \
  (srcX), (srcY), (width), (height), (frames)
#define MAP_CELL_STEPS_END 0xffff

/// Cell replacement: procedure and destination cells
struct MapCellAnim {
  const u16 *steps;
  u16 x;
  u16 y;
};

// Imports of main-image routines. EventRequestExit, ActorSetPosition,
// ActorSetAnimation and RandomNext have differing argument counts at their
// call sites, as measured in the bytes, so retain old-style declarations.
void Engine_EventBegin(void);
void Engine_EventEnd(void);
void Engine_EventWait(s32 frames);
void Engine_TaskWait(s32 frames);
void Engine_EventSetMessage(s32 message);
void Engine_EventShowMessage(s32 speaker, s32 flags);
s32 Engine_EventOpenMessage(s32 speaker, s32 flags);
s32 Engine_EventAskYesNo(s32 speaker, s32 flags);
s32 Engine_EventChooseYesNo(s32 actor, s32 flags);
void Engine_MessageShowCentered(s32 message, s32 flags);
void Engine_EventRequestExit();
void Engine_EventOpenScreen(void);
void Engine_EventCloseScreen(void);
void Engine_EventWaitForScreen(void);
void Engine_BlendSetDarkenTarget16(s32 target);
u8 *Engine_ResourceGetTableEntry(s32 resource);
void Engine_ActorSetPosition();
void Engine_ActorSetSpeed(s32 actor, s32 speed, s32 acceleration);
void Engine_ActorSetDestination(s32 actor, s32 x, s32 z);
void Engine_ActorSetDestinationOffset(s32 actor, s32 dx, s32 dz);
void Engine_ActorWalkTo(s32 actor, s32 x, s32 z);
void Engine_ActorWalkBy(s32 actor, s32 dx, s32 dz);
void Engine_ActorWaitForMove(s32 actor);
void Engine_ActorFaceDirection(s32 actor, s32 facing, s32 frames);
void Engine_ActorFaceActor(s32 actor, s32 target, s32 frames);
void Engine_ActorFaceEachOther(s32 actor, s32 other, s32 frames);
void Engine_ActorSetAnimation();
void Engine_ActorSetAnimationAndWait(s32 actor, s32 animation);
void Engine_ActorStartRepeatedMotion(s32 actor, s32 repeats);
void Engine_ActorRunRepeatedMotion(s32 actor, s32 repeats);
void Engine_ActorShowEmote(s32 actor, s32 emote, s32 frames);
void Engine_ActorSetAttachedEffect(s32 actor, s32 effect);
void Engine_ActorSetSpritePriority(s32 actor, s32 priority);
void Engine_ActorSetSpriteFlags(struct FieldActor *actor, s32 flags);
void Engine_CameraFollowActor(s32 actor, s32 keep_position);
void Engine_CameraSetSpeed(s32 speed, s32 acceleration);
void Engine_CameraMoveTo(s32 fixed_x, s32 fixed_y, s32 fixed_z, s32 pan);
void Engine_CameraMoveToActor(s32 actor, s32 pan);
void Engine_CameraWaitForMove(void);
void Engine_MapCopyCells(s32 srcX, s32 srcY, s32 width, s32 height, s32 destX,
                         s32 destY);
void Engine_MapCopyCellAttributes(s32 srcX, s32 srcY, s32 width, s32 height,
                                  s32 destX, s32 destY);
void Engine_MapRedraw(void);
void Engine_WorkSetValuesIfNonNegative(s32 first, s32 second, s32 third);
s32 Engine_GameFlagIsSet(s32 flag);
void Engine_GameFlagSet(s32 flag);
void Engine_GameFlagClear(s32 flag);
void Engine_AudioPlayCue(s32 cue);
void Engine_ShopOpen(s32 shop, s32 keeper);
void Engine_InnOpen(s32 inn, s32 keeper);
void Engine_SanctumOpen(s32 priest);
s32 Engine_TaskRemoveCallback(void (*callback)(void));
s32 Engine_RandomNext();
s32 Engine_MathSin(s32 angle);
s32 Engine_MathCos(s32 angle);
s32 __divsi3(s32 dividend, s32 divisor);
void *Engine_HeapAllocate(s32 slot, s32 size);
void Engine_HeapRelease(s32 slot);
s32 Engine_VramLoad(s32 block, s32 size, const void *data);
struct FieldActor *Engine_ObjectCreate(s32 type, s32 fixed_x, s32 fixed_y,
                                       s32 fixed_z);
void Object_SetMode(struct FieldActor *object, s32 animation);
void Engine_ObjectSetScript(struct FieldActor *object, const s32 *script);
void Engine_ObjectSetBlendMode(struct FieldActor *object, s32 mode);
void Engine_ObjectSetPartPalettes(struct FieldActor *object, s32 palette);
void Engine_ObjectSetPosition(struct FieldActor *object, s32 fixed_x,
                              s32 fixed_y, s32 fixed_z);
void Engine_MapAnimateCells(const u16 *steps, s32 destX, s32 destY);
void Engine_MapObjectSetPosition(s32 object, s32 fixed_x, s32 fixed_z);
void Engine_ItemLoadIcon(s32 item);
void Engine_ItemShowFound(s32 item, s32 height);
s32 Engine_PartyGiveItem(s32 item, s32 flags);
void Engine_ActorStop(s32 actor);
void Engine_ActorCenterAndWalk(s32 actor, s32 priority, s32 dz);
void Engine_ActorWalkByAndWait(s32 actor, s32 dx, s32 dz);
s32 Engine_LeaderCheckAhead(void);
void Engine_PsynergyBegin(s32 ability, s32 flags);
void Engine_PsynergySetTarget(s32 caster, s32 target);
void Engine_PsynergyRaiseHands(void);
void Engine_PsynergyPlayEffect(s32 effect);
void Engine_PsynergyLowerHands(void);
void Engine_PsynergyCancel(void);
void Engine_EventShowMessageAndWait(s32 speaker, s32 flags, s32 frames);
struct FieldActor *Engine_EventGetViewCenter(void);
struct FieldActor *Engine_ActorLookup(s32 actor);
void Engine_ActorDestroy(s32 actor);
void Engine_ActorSetChildValue(s32 actor, s32 value);
void Engine_ActorEnableActionCallback(s32 actor, s32 action);
/// The engine encodes either a built-in action or an action-script address.
static inline void Actor_SetActionScript(s32 actor, const s32 *script) {
  Engine_ActorEnableActionCallback(actor, (s32)script);
}

/// Runs a script, waiting at most 300 frames for its final command.
static inline void Actor_RunActionScriptAndWait(s32 actor, const s32 *script) {
  Object_SetActionCallbackAndRefreshById(actor, (s32)script);
}

static inline void Actor_SetBuiltinAction(s32 actor, s32 action) {
  Engine_ActorEnableActionCallback(actor, action);
}

void Engine_ActorSetActionCallback(struct FieldActor *actor, s32 value);
void Engine_ActorsRefresh(void);
void Engine_MapCopyCellsTo(s32 srcX, s32 srcY, s32 destX, s32 destY, s32 width,
                           s32 height);
void Engine_MapCopyCellsLayered(s32 srcX, s32 srcY, s32 destX, s32 destY,
                                s32 width, s32 height);
void Engine_ColorBufferApplySource(s32 value, s32 mode);
void Engine_ColorBufferApplyTarget(s32 value, s32 mode);
void Engine_ColorBufferInterpolate(s32 frames);
void Engine_ActorMoveToAndWait(s32 actor, s32 x, s32 z);
void Engine_ActorWalkToAndWait(s32 actor, s32 x, s32 z);
void Engine_ActorJump(s32 actor, s32 height, s32 frames);
void Engine_EventShowTwoMessagesAndWait(s32 speaker, s32 x, s32 y, s32 arg,
                                        s32 extra, s32 other_speaker,
                                        s32 otherX, s32 other_y, s32 other_arg,
                                        s32 other_extra, s32 flags);
void Engine_PartyAddMembers(s32 first, s32 second);
void Engine_MapRenderWaitForValues(void);

void Engine_ObjectMotionSetPositionAndCommit(s32 actor, s32 x, s32 z);
void Engine_ObjectDispatchRelease(struct FieldActor *object);
void Engine_MapWaitWorkValuesBelow256(void);
void Engine_RunRisingObjectSequence(struct FieldActor *object, s32 mode);

#ifdef FIELD_STAGED_ACTOR_IMPORTS
void Audio_PlayCue(s32 cue);
#endif

static inline void Event_SkipMessages(s32 count) {
  u16 *message = &gEventWork->message;

  *message = *message + count;
}

static inline void Event_ShowMessage(s32 speaker, s32 flags) {
  Engine_EventShowMessage(speaker, flags);
}

static inline s32 Event_OpenMessage(s32 speaker, s32 flags) {
  return Engine_EventOpenMessage(speaker, flags);
}

static inline s32 Event_AskYesNo(s32 speaker, s32 flags) {
  return Engine_EventAskYesNo(speaker, flags);
}

static inline struct FieldActor *Actor_Get(s32 actor) {
  return Object_GetById(actor);
}

static inline void Actor_SetPosition(s32 actor, s32 fixed_x, s32 fixed_z) {
  Engine_ActorSetPosition(actor, fixed_x, fixed_z);
}

static inline void Actor_SetSpeed(s32 actor, s32 speed, s32 acceleration) {
  Engine_ActorSetSpeed(actor, speed, acceleration);
}

static inline void Actor_SetDestination(s32 actor, s32 x, s32 z) {
  Engine_ActorSetDestination(actor, x, z);
}

#ifndef FIELD_STAGED_ACTOR_IMPORTS
static inline void Actor_SetDestinationOffset(s32 actor, s32 dx, s32 dz) {
  Engine_ActorSetDestinationOffset(actor, dx, dz);
}
#endif

static inline void Actor_WalkTo(s32 actor, s32 x, s32 z) {
  Engine_ActorWalkTo(actor, x, z);
}

static inline void Actor_WalkBy(s32 actor, s32 dx, s32 dz) {
  Engine_ActorWalkBy(actor, dx, dz);
}

static inline void Actor_FaceDirection(s32 actor, s32 facing, s32 frames) {
  Engine_ActorFaceDirection(actor, facing, frames);
}

static inline void Actor_FaceActor(s32 actor, s32 target, s32 frames) {
  Engine_ActorFaceActor(actor, target, frames);
}

static inline void Actor_ShowEmote(s32 actor, s32 emote, s32 frames) {
  Engine_ActorShowEmote(actor, emote, frames);
}

static inline void Actor_SetAttachedEffect(s32 actor, s32 effect) {
  Engine_ActorSetAttachedEffect(actor, effect);
}

static inline void Camera_SetSpeed(s32 speed, s32 acceleration) {
  Engine_CameraSetSpeed(speed, acceleration);
}

static inline void Camera_MoveTo(s32 fixed_x, s32 fixed_y, s32 fixed_z,
                                 s32 pan) {
  Engine_CameraMoveTo(fixed_x, fixed_y, fixed_z, pan);
}

static inline void Map_CopyCells(s32 srcX, s32 srcY, s32 width, s32 height,
                                 s32 destX, s32 destY) {
  Engine_MapCopyCells(srcX, srcY, width, height, destX, destY);
}

static inline void Map_CopyCellAttributes(s32 srcX, s32 srcY, s32 width,
                                          s32 height, s32 destX, s32 destY) {
  Engine_MapCopyCellAttributes(srcX, srcY, width, height, destX, destY);
}

/// Sets shake amplitudes in fixed-point pixels and the per-frame decay factor.
/// A negative argument keeps that component unchanged; Q16_ONE keeps the shake.
static inline void Map_SetShake(s32 horizontal, s32 vertical, s32 decay) {
  Engine_WorkSetValuesIfNonNegative(horizontal, vertical, decay);
}

// Keep the scene wrappers separate from the main-image declarations.
#define GameFlag_Set FieldEvent_GameFlagSet
#define GameFlag_IsSet FieldEvent_GameFlagIsSet
#define GameFlag_Clear FieldEvent_GameFlagClear
static inline s32 FieldEvent_GameFlagIsSet(s32 flag) {
  return Engine_GameFlagIsSet(flag);
}

static inline void FieldEvent_GameFlagSet(s32 flag) {
  Engine_GameFlagSet(flag);
}

static inline void FieldEvent_GameFlagClear(s32 flag) {
  Engine_GameFlagClear(flag);
}

#ifndef FIELD_STAGED_ACTOR_IMPORTS
static inline void Event_PlayCue(s32 cue) { Engine_AudioPlayCue(cue); }
#endif

static inline s32 Random_Next(void) { return Engine_RandomNext(); }

static inline s32 Math_Divide(s32 dividend, s32 divisor) {
  return dividend / divisor;
}

static inline struct FieldActor *Object_Create(s32 type, s32 fixed_x,
                                               s32 fixed_y, s32 fixed_z) {
  return Engine_ObjectCreate(type, fixed_x, fixed_y, fixed_z);
}

static inline void Object_SetScript(struct FieldActor *object,
                                    const s32 *script) {
  Engine_ObjectSetScript(object, script);
}

enum ObjectBlendMode { OBJECT_BLEND_NORMAL = 0, OBJECT_BLEND_TRANSLUCENT = 1 };

static inline void Map_AnimateCells(const u16 *steps, s32 destX, s32 destY) {
  Engine_MapAnimateCells(steps, destX, destY);
}

static inline void MapObject_SetPosition(s32 object, s32 fixed_x, s32 fixed_z) {
  Engine_MapObjectSetPosition(object, fixed_x, fixed_z);
}

static inline void Actor_CenterAndWalk(s32 actor, s32 priority, s32 dz) {
  Engine_ActorCenterAndWalk(actor, priority, dz);
}

static inline void Actor_WalkByAndWait(s32 actor, s32 dx, s32 dz) {
  Engine_ActorWalkByAndWait(actor, dx, dz);
}

static inline void Event_ShowMessageAndWait(s32 speaker, s32 flags,
                                            s32 frames) {
  Engine_EventShowMessageAndWait(speaker, flags, frames);
}

static inline void Actor_SetChildValue(s32 actor, s32 value) {
  Engine_ActorSetChildValue(actor, value);
}

#define ACTION_TABLE_STOP ((const u8 *)1)

static inline void Map_CopyCellsTo(s32 srcX, s32 srcY, s32 destX, s32 destY,
                                   s32 width, s32 height) {
  Engine_MapCopyCellsTo(srcX, srcY, destX, destY, width, height);
}

static inline void Map_CopyCellsLayered(s32 srcX, s32 srcY, s32 destX,
                                        s32 destY, s32 width, s32 height) {
  Engine_MapCopyCellsLayered(srcX, srcY, destX, destY, width, height);
}

static inline void ColorBuffer_ApplySource(s32 value, s32 mode) {
  Engine_ColorBufferApplySource(value, mode);
}

static inline void ColorBuffer_ApplyTarget(s32 value, s32 mode) {
  Engine_ColorBufferApplyTarget(value, mode);
}

static inline void Actor_MoveToAndWait(s32 actor, s32 x, s32 z) {
  Engine_ActorMoveToAndWait(actor, x, z);
}

static inline void Actor_WalkToAndWait(s32 actor, s32 x, s32 z) {
  Engine_ActorWalkToAndWait(actor, x, z);
}

static inline void Event_ShowTwoMessagesAndWait(s32 speaker, s32 x, s32 y,
                                                s32 arg, s32 extra,
                                                s32 otherSpeaker, s32 otherX,
                                                s32 otherY, s32 otherArg,
                                                s32 otherExtra, s32 flags) {
  Engine_EventShowTwoMessagesAndWait(speaker, x, y, arg, extra, otherSpeaker,
                                     otherX, otherY, otherArg, otherExtra,
                                     flags);
}

#endif

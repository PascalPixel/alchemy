#ifndef ALCHEMY_FIELD_EVENT_H
#define ALCHEMY_FIELD_EVENT_H

#include "unit/object/object.h"
#include "unit/object/sparkle.h"
#include "unit/object/evactor.h"
#include "unit/object/evcamera.h"
#include "unit/object/djinnev.h"
#include "unit/object/psydisp.h"
#include "unit/object/psyfx.h"
#include "mapcopy.h"

#include "layout_guard.h"

#include "game_state.h"

// A scene stores into its own const tables: an overlay's data is in RAM.
// The games' compiler takes the store as written; a current GCC refuses it,
// so the modern build names the place without its const.
#ifdef MODERN
#define STORE_CONST(place) (*(__typeof_unqual__(place) *)&(place))
#else
#define STORE_CONST(place) (place)
#endif

extern u8 gSceneState[];

extern u8 *gKorosseoWork;

// volatile: gFrameCount carries its tag where frame.h declares it
extern volatile u32 gFrameCount;

#include "fieldobj.h"

enum
{
	TASK_PRIORITY_SCENE = 3200
};

enum ActorAnimation
{
	ANIM_STAND = 1,
	ANIM_WALK = 2,
	ANIM_NOD = 3,
	ANIM_SHAKE_HEAD = 4
};

#define EMOTE_IN_FRONT 0x100

#define SPEAKER_WINDOW_ABOVE 0x4000

#define MAP_CELL_STEP(src_x, src_y, width, height, frames) \
	(src_x), (src_y), (width), (height), (frames)
#define MAP_CELL_STEPS_END 0xffff

// セル書き換え: 手順と書き込み先のセル
struct MapCellAnim
{
	const u16 *steps;
	u16		   x;
	u16		   y;
};

void			   Engine_EventBegin();
void			   Engine_EventEnd();
void			   Engine_EventWait();
void			   Engine_TaskWait();
void			   Engine_EventSetMessage();
void			   Engine_EventShowMessage();
s32				   Engine_EventOpenMessage();
s32				   Engine_EventAskYesNo();
s32				   Engine_EventChooseYesNo();
void			   Engine_MessageShowCentered();
void			   Engine_EventRequestExit();
void			   Engine_EventOpenScreen();
void			   Engine_EventCloseScreen();
void			   Engine_EventWaitForScreen();
void			   Engine_BlendSetDarkenTarget16();
u8				  *Engine_ResourceGetTableEntry();
void			   Engine_ActorSetPosition();
void			   Engine_ActorSetSpeed();
void			   Engine_ActorSetDestination();
void			   Engine_ActorSetDestinationOffset();
void			   Engine_ActorWalkTo();
void			   Engine_ActorWalkBy();
void			   Engine_ActorWaitForMove();
void			   Engine_ActorFaceDirection();
void			   Engine_ActorFaceActor();
void			   Engine_ActorFaceEachOther();
void			   Engine_ActorSetAnimation();
void			   Engine_ActorSetAnimationAndWait();
void			   Engine_ActorStartRepeatedMotion();
void			   Engine_ActorRunRepeatedMotion();
void			   Engine_ActorShowEmote();
void			   Engine_ActorSetAttachedEffect();
void			   Engine_ActorSetSpritePriority();
void			   Engine_ActorSetSpriteFlags();
void			   Engine_CameraFollowActor();
void			   Engine_CameraSetSpeed();
void			   Engine_CameraMoveTo();
void			   Engine_CameraMoveToActor();
void			   Engine_CameraWaitForMove();
void			   Engine_MapCopyCells();
void			   Engine_MapCopyCellAttributes();
void			   Engine_MapRedraw();
void			   Engine_WorkSetValuesIfNonNegative();
s32				   Engine_GameFlagIsSet();
s32				   Engine_GameFlagSet();
void			   Engine_GameFlagClear();
void			   Engine_AudioPlayCue();
void			   Engine_ShopOpen();
void			   Engine_InnOpen();
void			   Engine_SanctumOpen();
s32				   Engine_TaskAddCallback();
s32				   Engine_TaskRemoveCallback();
s32				   Engine_RandomNext();
s32				   Engine_MathSin();
s32				   Engine_MathCos();
s32				   __divsi3(s32 dividend, s32 divisor);
void			  *Engine_HeapAllocate();
void			   Engine_HeapRelease();
s32				   Engine_VramLoad();
struct FieldActor *Engine_ObjectCreate();
void			   Object_SetMode(struct FieldActor *object, s32 animation);
void			   Engine_ObjectSetScript();
void			   Engine_ObjectSetBlendMode();
void			   Engine_ObjectSetPartPalettes();
void			   Engine_ObjectSetPosition();
void			   Engine_ObjectCommitPosition();
void			   Engine_MapAnimateCells();
void			   Engine_MapObjectSetPosition();
void			   Engine_ItemLoadIcon();
void			   Engine_ItemShowFound();
s32				   Engine_PartyGiveItem();
void			   Engine_ActorStop();
void			   Engine_ActorCenterAndWalk();
void			   Engine_ActorWalkByAndWait();
s32				   Engine_LeaderCheckAhead();
void			   Engine_PsynergyBegin();
void			   Engine_PsynergySetTarget();
void			   Engine_PsynergyRaiseHands();
void			   Engine_PsynergyPlayEffect();
void			   Engine_PsynergyLowerHands();
void			   Engine_PsynergyCancel();
void			   Engine_EventShowMessageAndWait();
struct FieldActor *Engine_EventGetViewCenter();
struct FieldActor *Engine_ActorLookup();
void			   Engine_ActorDestroy();
void			   Engine_ActorSetChildValue();
void			   Engine_ActorEnableActionCallback();
void			   Engine_ActorSetActionCallback();
void			   Engine_ActorsRefresh();
void			   Engine_MapCopyCellsTo();
void			   Engine_MapCopyCellsLayered();
void			   Engine_ColorBufferApplySource();
void			   Engine_ColorBufferApplyTarget();
void			   Engine_ColorBufferInterpolate();
void			   Engine_ActorMoveToAndWait();
void			   Engine_ActorWalkToAndWait();
void			   Engine_ActorJump();
void			   Engine_EventShowTwoMessagesAndWait();
void			   Engine_PartyAddMembers();
void			   Engine_MapRenderWaitForValues();

void Engine_ObjectMotionSetPositionAndCommit();
void Engine_ObjectDispatchRelease();
void Engine_MapWaitWorkValuesBelow256();
void Engine_RunRisingObjectSequence();

#ifdef FIELD_STAGED_ACTOR_IMPORTS
void Audio_PlayCue(s32 cue);
#endif

static inline void Event_SkipMessages(s32 count)
{
	u16 *message = &gEventWork->message;

	*message = *message + count;
}

static inline void Event_ShowMessage(s32 speaker, s32 flags)
{
	Engine_EventShowMessage(speaker, flags);
}

static inline s32 Event_OpenMessage(s32 speaker, s32 flags)
{
	return Engine_EventOpenMessage(speaker, flags);
}

static inline s32 Event_AskYesNo(s32 speaker, s32 flags)
{
	return Engine_EventAskYesNo(speaker, flags);
}

static inline struct FieldActor *Actor_Get(s32 actor)
{
	return Object_GetById(actor);
}

static inline void Actor_SetPosition(s32 actor, s32 fixed_x, s32 fixed_z)
{
	Engine_ActorSetPosition(actor, fixed_x, fixed_z);
}

#ifndef FIELD_STAGED_ACTOR_IMPORTS
static inline void Actor_SetSpeed(s32 actor, s32 speed, s32 acceleration)
{
	Engine_ActorSetSpeed(actor, speed, acceleration);
}
#else
static inline void Actor_SetSpeed(s32 actor, s32 speed, s32 acceleration)
{
	Engine_ActorSetSpeed(actor, speed, acceleration);
}
#endif

static inline void Actor_SetDestination(s32 actor, s32 x, s32 z)
{
	Engine_ActorSetDestination(actor, x, z);
}

#ifndef FIELD_STAGED_ACTOR_IMPORTS
static inline void Actor_SetDestinationOffset(s32 actor, s32 dx, s32 dz)
{
	Engine_ActorSetDestinationOffset(actor, dx, dz);
}
#endif

static inline void Actor_WalkTo(s32 actor, s32 x, s32 z)
{
	Engine_ActorWalkTo(actor, x, z);
}

static inline void Actor_WalkBy(s32 actor, s32 dx, s32 dz)
{
	Engine_ActorWalkBy(actor, dx, dz);
}

static inline void Actor_FaceDirection(s32 actor, s32 facing, s32 frames)
{
	Engine_ActorFaceDirection(actor, facing, frames);
}

static inline void Actor_FaceActor(s32 actor, s32 target, s32 frames)
{
	Engine_ActorFaceActor(actor, target, frames);
}

static inline void Actor_ShowEmote(s32 actor, s32 emote, s32 frames)
{
	Engine_ActorShowEmote(actor, emote, frames);
}

static inline void Actor_SetAttachedEffect(s32 actor, s32 effect)
{
	Engine_ActorSetAttachedEffect(actor, effect);
}

static inline void Camera_SetSpeed(s32 speed, s32 acceleration)
{
	Engine_CameraSetSpeed(speed, acceleration);
}

static inline void Camera_MoveTo(s32 fixed_x, s32 fixed_y, s32 fixed_z, s32 pan)
{
	Engine_CameraMoveTo(fixed_x, fixed_y, fixed_z, pan);
}

static inline void Map_CopyCells(
	s32 src_x, s32 src_y, s32 width, s32 height, s32 dest_x, s32 dest_y)
{
	Engine_MapCopyCells(src_x, src_y, width, height, dest_x, dest_y);
}

#ifndef FIELD_STAGED_ACTOR_IMPORTS
static inline void Map_CopyCellAttributes(
	s32 src_x, s32 src_y, s32 width, s32 height, s32 dest_x, s32 dest_y)
{
	Engine_MapCopyCellAttributes(src_x, src_y, width, height, dest_x, dest_y);
}
#else
static inline void Map_CopyCellAttributes(
	s32 src_x, s32 src_y, s32 width, s32 height, s32 dest_x, s32 dest_y)
{
	Engine_MapCopyCellAttributes(src_x, src_y, width, height, dest_x, dest_y);
}
#endif

static inline void Work_SetValuesIfNonNegative(s32 first, s32 second, s32 third)
{
	Engine_WorkSetValuesIfNonNegative(first, second, third);
}

enum
{
	FLAG_ARRIVAL_EVENT_PENDING = 0x12f
};

#ifdef MODERN
// field.h and game_flags.h declare the engine's GameFlag_IsSet, _Set and
// _Clear; modern GCC refuses these static ones under the same names, so the
// scene layer's take their own
#define GameFlag_IsSet FieldEvent_GameFlagIsSet
#define GameFlag_Set FieldEvent_GameFlagSet
#define GameFlag_Clear FieldEvent_GameFlagClear
#endif
static inline s32 GameFlag_IsSet(s32 flag)
{
	return Engine_GameFlagIsSet(flag);
}

static inline s32 GameFlag_Set(s32 flag)
{
	return Engine_GameFlagSet(flag);
}

static inline void GameFlag_Clear(s32 flag)
{
	Engine_GameFlagClear(flag);
}

#ifndef FIELD_STAGED_ACTOR_IMPORTS
static inline void Event_PlayCue(s32 cue)
{
	Engine_AudioPlayCue(cue);
}
#endif

static inline s32 Random_Next(void)
{
	return Engine_RandomNext();
}

static inline s32 Math_Divide(s32 dividend, s32 divisor)
{
	return dividend / divisor;
}

static inline struct FieldActor *Object_Create(s32 type, s32 fixed_x, s32 fixed_y, s32 fixed_z)
{
	return Engine_ObjectCreate(type, fixed_x, fixed_y, fixed_z);
}

static inline void Object_SetScript(struct FieldActor *object, const s32 *script)
{
	Engine_ObjectSetScript(object, script);
}

enum ObjectBlendMode
{
	OBJECT_BLEND_NORMAL = 0,
	OBJECT_BLEND_TRANSLUCENT = 1
};

static inline void Map_AnimateCells(const u16 *steps, s32 dest_x, s32 dest_y)
{
	Engine_MapAnimateCells(steps, dest_x, dest_y);
}

static inline void MapObject_SetPosition(s32 object, s32 fixed_x, s32 fixed_z)
{
	Engine_MapObjectSetPosition(object, fixed_x, fixed_z);
}

static inline void Actor_CenterAndWalk(s32 actor, s32 priority, s32 dz)
{
	Engine_ActorCenterAndWalk(actor, priority, dz);
}

static inline void Actor_WalkByAndWait(s32 actor, s32 dx, s32 dz)
{
	Engine_ActorWalkByAndWait(actor, dx, dz);
}

static inline void Event_ShowMessageAndWait(s32 speaker, s32 flags, s32 frames)
{
	Engine_EventShowMessageAndWait(speaker, flags, frames);
}

static inline void Actor_SetChildValue(s32 actor, s32 value)
{
	Engine_ActorSetChildValue(actor, value);
}

#define ACTION_TABLE_STOP ((const u8 *)1)

static inline void Map_CopyCellsTo(
	s32 src_x, s32 src_y, s32 dest_x, s32 dest_y, s32 width, s32 height)
{
	Engine_MapCopyCellsTo(src_x, src_y, dest_x, dest_y, width, height);
}

static inline void Map_CopyCellsLayered(
	s32 src_x, s32 src_y, s32 dest_x, s32 dest_y, s32 width, s32 height)
{
	Engine_MapCopyCellsLayered(src_x, src_y, dest_x, dest_y, width, height);
}

static inline void MapRender_SetValues(s32 value0, s32 value1, s32 value2)
{
	Engine_WorkSetValuesIfNonNegative(value0, value1, value2);
}

static inline void ColorBuffer_ApplySource(s32 value, s32 mode)
{
	Engine_ColorBufferApplySource(value, mode);
}

static inline void ColorBuffer_ApplyTarget(s32 value, s32 mode)
{
	Engine_ColorBufferApplyTarget(value, mode);
}

static inline void Actor_MoveToAndWait(s32 actor, s32 x, s32 z)
{
	Engine_ActorMoveToAndWait(actor, x, z);
}

static inline void Actor_WalkToAndWait(s32 actor, s32 x, s32 z)
{
	Engine_ActorWalkToAndWait(actor, x, z);
}

static inline void Event_ShowTwoMessagesAndWait(s32 speaker, s32 x, s32 y, s32 arg, s32 extra,
	s32 other_speaker, s32 other_x, s32 other_y, s32 other_arg, s32 other_extra, s32 flags)
{
	Engine_EventShowTwoMessagesAndWait(
		speaker, x, y, arg, extra, other_speaker, other_x, other_y, other_arg, other_extra, flags);
}

#endif

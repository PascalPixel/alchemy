#ifndef ALCHEMY_FIELD_EVENT_H
#define ALCHEMY_FIELD_EVENT_H

#include "MAPCOPY.H"

#include "LAYOUT_GUARD.H"

/*
 * What a field scene's event scripts ask of the engine: holding the party
 * still while an event plays, dialogue, actor motion, the camera, the map,
 * flags, sound, exits and the shop, inn and sanctum services.
 */

#include "GAME_STATE.H"

/* Saved words the current scene keeps for itself: a puzzle's board, a
   recorded course, the coins counted before paying. */
extern u8 gSceneState[];

/* The Korosseo stage's work, which its scenes share. */
extern u8 *gKorosseoWork;

/* Counts the frames the game has drawn. */
extern u32 gFrameCount;

#include "FIELDOBJ.H"

/* The task priority scenes run their per-frame callbacks at. */
enum {
    TASK_PRIORITY_SCENE = 3200
};

/*
 * Animations a person's sprite plays. The party leader nods when the player
 * answers yes to a question and shakes its head at no.
 */
enum ActorAnimation {
    ANIM_STAND = 1,
    ANIM_WALK = 2,
    ANIM_NOD = 3,
    ANIM_SHAKE_HEAD = 4
};

/* Emote and attached-effect sprites draw over the scenery with this bit. */
#define EMOTE_IN_FRONT 0x100

/* A speaker id with this bit opens the message window above the speaker. */
#define SPEAKER_WINDOW_ABOVE 0x4000

/*
 * A map cell animation is a list of steps of five words: the column and row
 * of a block of source cells, its width and height, and the frames the
 * block holds once copied into place. MAP_CELL_STEPS_END ends the list.
 */
#define MAP_CELL_STEP(src_x, src_y, width, height, frames) \
    (src_x), (src_y), (width), (height), (frames)
#define MAP_CELL_STEPS_END 0xffff

/*
 * The engine entry points a scene overlay imports. Scripts call plain imports
 * directly; the tagged inline services below preserve measured compiler order.
 */
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
void Engine_EventRequestExit(s32 exit);
void Engine_EventOpenScreen(void);
void Engine_EventCloseScreen(void);
void Engine_EventWaitForScreen(void);
void Engine_BlendSetDarkenTarget16(s32 target);
u8 *Engine_ResourceGetTableEntry(s32 resource);
void Engine_ResourceDecodeType01(const u8 *source, void *destination);
struct FieldActor *Object_GetById(s32 actor);
void Engine_ActorSetPosition(s32 actor, s32 fixed_x, s32 fixed_z);
void Engine_ActorSetSpeed(s32 actor, s32 speed, s32 acceleration);
void Engine_ActorSetDestination(s32 actor, s32 x, s32 z);
void Engine_ActorSetDestinationOffset(s32 actor, s32 dx, s32 dz);
void Engine_ActorWalkTo(s32 actor, s32 x, s32 z);
void Engine_ActorWalkBy(s32 actor, s32 dx, s32 dz);
void Engine_ActorWaitForMove(s32 actor);
void Engine_ActorFaceDirection(s32 actor, s32 facing, s32 frames);
void Engine_ActorTurnToAngle(s32 actor, s32 angle, s32 frames);
void Engine_ActorFaceActor(s32 actor, s32 target, s32 frames);
void Engine_ActorFaceEachOther(s32 actor, s32 other, s32 frames);
void Engine_ActorSetAnimation(s32 actor, s32 animation);
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
void Engine_MapCopyCells(s32 src_x, s32 src_y, s32 width, s32 height, s32 dest_x, s32 dest_y);
void Engine_MapCopyCellAttributes(s32 src_x, s32 src_y, s32 width, s32 height, s32 dest_x,
                                  s32 dest_y);
void Engine_MapRedraw(void);
void Engine_WorkSetValuesIfNonNegative(s32 first, s32 second, s32 third);
s32 Engine_GameFlagIsSet(s32 flag);
s32 Engine_GameFlagSet(s32 flag);
void Engine_GameFlagClear(s32 flag);
void Engine_AudioPlayCue(s32 cue);
void Engine_ShopOpen(s32 shop, s32 keeper);
void Engine_InnOpen(s32 inn, s32 keeper);
void Engine_SanctumOpen(s32 priest);
s32 Engine_TaskAddCallback(void (*callback)(void), s32 priority);
s32 Engine_TaskRemoveCallback(void (*callback)(void));
s32 Engine_RandomNext(void);
s32 Engine_MathSin(s32 angle);
s32 Engine_MathCos(s32 angle);
s32 __divsi3(s32 dividend, s32 divisor);
void *Engine_HeapAllocate(s32 slot, s32 size);
void Engine_HeapRelease(s32 slot);
s32 Engine_VramLoad(s32 block, s32 size, const void *data);
struct FieldActor *Engine_ObjectCreate(s32 type, s32 fixed_x, s32 fixed_y, s32 fixed_z);
void Object_SetMode(struct FieldActor *object, s32 animation);
void Engine_ObjectSetScript(struct FieldActor *object, const s32 *script);
void Engine_ObjectSetBlendMode(struct FieldActor *object, s32 mode);
void ObjectGroup_SetChildValue(struct FieldActor *object, s32 palette);
void Engine_ObjectSetPartPalettes(struct FieldActor *object, s32 palette);
void Engine_ObjectSetPosition(struct FieldActor *object, s32 fixed_x, s32 fixed_y, s32 fixed_z);
void Engine_ObjectCommitPosition(struct FieldActor *object);
void Engine_MapAnimateCells(const u16 *steps, s32 dest_x, s32 dest_y);
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
void Engine_ActorEnableActionCallback(s32 actor, const u8 *table);
void Engine_ActorSetActionCallback(struct FieldActor *actor, s32 value);
void Engine_ActorsRefresh(void);
void Engine_MapCopyCellsTo(s32 src_x, s32 src_y, s32 dest_x, s32 dest_y, s32 width, s32 height);
void Engine_MapRenderSetValues(s32 value0, s32 value1, s32 value2);
void Engine_ColorBufferApplySource(s32 value, s32 mode);
void Engine_ColorBufferApplyTarget(s32 value, s32 mode);
void Engine_ColorBufferInterpolate(s32 frames);
void Engine_ActorMoveToAndWait(s32 actor, s32 x, s32 z);
void Engine_ActorWalkToAndWait(s32 actor, s32 x, s32 z);
void Engine_ActorJump(s32 actor, s32 height, s32 frames);
void Engine_EventShowTwoMessagesAndWait(s32 speaker, s32 x, s32 y, s32 arg, s32 extra,
                                        s32 other_speaker, s32 other_x, s32 other_y,
                                        s32 other_arg, s32 other_extra, s32 flags);
void Engine_PartyAddMembers(s32 first, s32 second);
void Engine_MapRenderWaitForValues(void);

/* Loader-imported object-motion services shared by field-scene overlays. */
s32 Engine_UiWorkWaitThenFinalizeCapacity(s32 first, s32 second);
void Engine_ObjectMotionArmCallback(s32 actor, s32 angle, s32 frames);
void Engine_ObjectMotionSetPositionAndCommit(s32 actor, s32 x, s32 z);
void Engine_ObjectMotionSetPositionAndReset(s32 actor, s32 x, s32 z);
/* Throws the actor upward at the given speed, then waits the given frames. */
void Engine_ObjectMotionLaunch(s32 actor, s32 speed, s32 frames);
void Engine_ObjectDispatchRelease(struct FieldActor *object);
void Engine_MapWaitWorkValuesBelow256(void);
void Engine_RunRisingObjectSequence(struct FieldActor *object, s32 mode);

#ifdef FIELD_STAGED_ACTOR_IMPORTS
/*
 * An overlay that links the staged-actor code (FIELD/COMMON/OBJECT/
 * STAGED_ACTOR.C) names these imports as that code does, by the main image's
 * names, so each of its import veneers carries one name. Its scenes define
 * FIELD_STAGED_ACTOR_IMPORTS and reach the same imports by those names.
 */
void Battle_WaitMode0(s32 frames);
void WaitFrames(s32 frames);
void ObjectMotion_SetSpeedParameters(s32 actor, s32 speed, s32 acceleration);
void ObjectMotion_CommitCurrentPositionAndActivate(s32 actor);
void ObjectMotion_OffsetPositionAndResetMotion(s32 actor, s32 dx, s32 dz);
void Object_SetModeById(s32 actor, s32 animation);
void Audio_PlayCue(s32 cue);
#endif

/*
 * Dialogue: choose the message, then let an actor say it. Each message shown
 * advances to the next one. An opened message stays on screen for the
 * question that follows it, and choosing yes returns 0.
 */

static inline void Event_ShowMessage(s32 speaker, s32 flags)
{
    /* FAKEMATCH: Direct calls change FieldScene_RunFourActorEncounter (EN): mov	r0, #3 becomes mov	r7, r8. */
    Engine_EventShowMessage(speaker, flags);
}

static inline s32 Event_OpenMessage(s32 speaker, s32 flags)
{
    /* FAKEMATCH: Direct calls change FieldScene_RunFourActorEncounter (EN): mov	r0, #3 becomes mov	r7, r8. */
    return Engine_EventOpenMessage(speaker, flags);
}

static inline s32 Event_AskYesNo(s32 speaker, s32 flags)
{
    /* FAKEMATCH: Direct calls change FieldScene_RunTwoActorCutsceneSequence (JA): mov	r1, #192 becomes ldr	r5, .L0+24. */
    return Engine_EventAskYesNo(speaker, flags);
}

/*
 * Actors. Positions set directly are fixed-point; destinations and offsets
 * are whole pixels. Walking plays the walk animation on the way.
 */
static inline struct FieldActor *Actor_Get(s32 actor)
{
    /* FAKEMATCH: Direct calls change HaidiaArashi_RunRiverSearch (EN): add	r5, r5, #100 becomes mov	r3, r5. */
    return Object_GetById(actor);
}

static inline void Actor_SetPosition(s32 actor, s32 fixed_x, s32 fixed_z)
{
    /* FAKEMATCH: Direct calls change RunEventScript01 (EN): mov	r0, #0 becomes lsl	r1, r1, #15. */
    Engine_ActorSetPosition(actor, fixed_x, fixed_z);
}

#ifndef FIELD_STAGED_ACTOR_IMPORTS
static inline void Actor_SetSpeed(s32 actor, s32 speed, s32 acceleration)
{
    /* FAKEMATCH: Direct calls change FieldScene_RunMiddleSequence (EN): ldr	r0, .L2+20 becomes mov	r5, #128. */
    Engine_ActorSetSpeed(actor, speed, acceleration);
}
#else
static inline void Actor_SetSpeed(s32 actor, s32 speed, s32 acceleration)
{
    /* FAKEMATCH: Direct calls change FieldScene_RunScene39f_02000d90 (EN): mov	r0, r6 becomes lsl	r1, r1, #10. */
    ObjectMotion_SetSpeedParameters(actor, speed, acceleration);
}
#endif

static inline void Actor_SetDestination(s32 actor, s32 x, s32 z)
{
    /* FAKEMATCH: Direct calls change RunEventScript01 (EN): mov	r0, #0 becomes mov	r5, #160. */
    Engine_ActorSetDestination(actor, x, z);
}

#ifndef FIELD_STAGED_ACTOR_IMPORTS
static inline void Actor_SetDestinationOffset(s32 actor, s32 dx, s32 dz)
{
    /* FAKEMATCH: Direct calls change Kyuden_RunKolimaRequest (EN): mov	r0, #1 becomes neg	r1, r1. */
    Engine_ActorSetDestinationOffset(actor, dx, dz);
}
#endif

static inline void Actor_WalkTo(s32 actor, s32 x, s32 z)
{
    /* FAKEMATCH: Direct calls change RunEventScript01 (EN): mov	r0, #188 becomes mov	r5, #195. */
    Engine_ActorWalkTo(actor, x, z);
}

static inline void Actor_WalkBy(s32 actor, s32 dx, s32 dz)
{
    /* FAKEMATCH: Direct calls change FieldScene_RunScene3aeSequenceB (JA): mov	r5, r8 becomes mov	r5, r9. */
    Engine_ActorWalkBy(actor, dx, dz);
}

/* Waits until the actor reaches its destination, then stands it still. */
static inline void Actor_FaceDirection(s32 actor, s32 facing, s32 frames)
{
    /* FAKEMATCH: Direct calls change FieldScene_RunMiddleSequence (EN): mov	r0, sl becomes lsl	r1, r1, #8. */
    Engine_ActorFaceDirection(actor, facing, frames);
}

static inline void Actor_FaceActor(s32 actor, s32 target, s32 frames)
{
    /* FAKEMATCH: Direct calls change FieldScene_RunPairedEntranceWalk (JA): push	{r5, lr} becomes push	{r5, r6, lr}. */
    Engine_ActorFaceActor(actor, target, frames);
}

/* Shows an emote over the actor and waits the given frames. */
static inline void Actor_ShowEmote(s32 actor, s32 emote, s32 frames)
{
    /* FAKEMATCH: Direct calls change RunEventScript01 (JA): mov	r0, #9 becomes mov	r5, #129. */
    Engine_ActorShowEmote(actor, emote, frames);
}

/* Attaches a lasting effect sprite to the actor; effect 0 removes it. */
static inline void Actor_SetAttachedEffect(s32 actor, s32 effect)
{
    /* FAKEMATCH: Direct calls change RunEventScript01 (EN): mov	r0, #12 becomes mov	r5, #129. */
    Engine_ActorSetAttachedEffect(actor, effect);
}

/* The camera. A pan of 0 moves it at once; -1 keeps a coordinate. */

static inline void Camera_SetSpeed(s32 speed, s32 acceleration)
{
    /* FAKEMATCH: Direct calls change RunEventScript01 (EN): push	{r5, lr} becomes push	{r5, r6, lr}. */
    Engine_CameraSetSpeed(speed, acceleration);
}

static inline void Camera_MoveTo(s32 fixed_x, s32 fixed_y, s32 fixed_z, s32 pan)
{
    /* FAKEMATCH: Direct calls change RunEventScript01 (EN): push	{r5, lr} becomes push	{r5, r6, lr}. */
    Engine_CameraMoveTo(fixed_x, fixed_y, fixed_z, pan);
}

/*
 * The map, in cells. Copying cells redraws them; copying attributes keeps
 * each destination cell's tile and takes the rest from the source.
 */
static inline void Map_CopyCells(s32 src_x, s32 src_y, s32 width, s32 height, s32 dest_x,
                                 s32 dest_y)
{
    /* FAKEMATCH: Direct calls change Gate_DrawPropped (EN): mov	r2, #62 becomes str	r3, [sp]. */
    Engine_MapCopyCells(src_x, src_y, width, height, dest_x, dest_y);
}

#ifndef FIELD_STAGED_ACTOR_IMPORTS
static inline void Map_CopyCellAttributes(s32 src_x, s32 src_y, s32 width, s32 height,
                                          s32 dest_x, s32 dest_y)
{
    /* FAKEMATCH: Direct calls change FieldScene_RunScene398SequenceA (JA): mov	r2, #14 becomes str	r3, [sp]. */
    Engine_MapCopyCellAttributes(src_x, src_y, width, height, dest_x, dest_y);
}
#else
static inline void Map_CopyCellAttributes(s32 src_x, s32 src_y, s32 width, s32 height,
                                          s32 dest_x, s32 dest_y)
{
    /* FAKEMATCH: Direct calls change MogoruMori_RunProbedLandingScene (EN): mov	r2, #68 becomes asr	r3, r3, #20. */
    Map_CopyCellAttributeRect(src_x, src_y, width, height, dest_x, dest_y);
}
#endif

static inline void Work_SetValuesIfNonNegative(s32 first, s32 second, s32 third)
{
    /* FAKEMATCH: Direct calls change RunEventScript01 (JA): push	{r5, lr} becomes push	{r5, r6, r7, lr}. */
    Engine_WorkSetValuesIfNonNegative(first, second, third);
}

/*
 * Story scenes all over the world clear this flag as they finish, and an
 * entry script sets it for an entrance whose arrival plays an event.
 */
enum {
    FLAG_ARRIVAL_EVENT_PENDING = 0x12f
};

static inline s32 GameFlag_IsSet(s32 flag)
{
    /* FAKEMATCH: Direct calls change FieldScene_RunScene398SequenceB (EN): ldr	r0, .L1+4 becomes ldr	r6, .L1+4. */
    return Engine_GameFlagIsSet(flag);
}

static inline s32 GameFlag_Set(s32 flag)
{
    /* FAKEMATCH: Direct calls change HaidiaArashi_FlashLightning (EN): push	{lr} becomes push	{r5, lr}. */
    return Engine_GameFlagSet(flag);
}

static inline void GameFlag_Clear(s32 flag)
{
    /* FAKEMATCH: Direct calls change FieldScene_RunScene371_02001c08 (EN): push	{lr} becomes push	{r5, r6, lr}. */
    Engine_GameFlagClear(flag);
}

#ifndef FIELD_STAGED_ACTOR_IMPORTS
static inline void Audio_PlayCue(s32 cue)
{
    /* FAKEMATCH: Direct calls change FieldScene_RunScene371_02002274 (EN): ldr	r2, .L0+48 becomes ldr	r5, .L0+48. */
    Engine_AudioPlayCue(cue);
}
#endif

static inline s32 Random_Next(void)
{
    /* FAKEMATCH: Direct calls change FieldScene_RunScene3a8SequenceA (EN): mov	r2, r0 becomes ldr	r3, .L0. */
    return Engine_RandomNext();
}

static inline s32 Math_Divide(s32 dividend, s32 divisor)
{
    return dividend / divisor;
}

static inline struct FieldActor *Object_Create(s32 type, s32 fixed_x, s32 fixed_y, s32 fixed_z)
{
    /* FAKEMATCH: Direct calls change VinasuChojo_RunTransitionStep (DE): mov	r6, r0 becomes mov	sl, r0. */
    return Engine_ObjectCreate(type, fixed_x, fixed_y, fixed_z);
}

static inline void Object_SetScript(struct FieldActor *object, const s32 *script)
{
    /* FAKEMATCH: Direct calls change Effect_Spawn (EN): sub	sp, sp, #8 becomes sub	sp, sp, #4. */
    Engine_ObjectSetScript(object, script);
}

/* The GBA sprite modes an object's sprite can take. */
enum ObjectBlendMode {
    OBJECT_BLEND_NORMAL = 0,
    OBJECT_BLEND_TRANSLUCENT = 1
};

static inline void Map_AnimateCells(const u16 *steps, s32 dest_x, s32 dest_y)
{
    /* FAKEMATCH: Direct calls change Door_Enter (EN): sub	r5, r5, #4 becomes ldr	r2, .L0+4. */
    Engine_MapAnimateCells(steps, dest_x, dest_y);
}

static inline void MapObject_SetPosition(s32 object, s32 fixed_x, s32 fixed_z)
{
    /* FAKEMATCH: Direct calls change FieldScene_RunScene3c0SequenceA (EN): mov	r1, #1 becomes mov	r2, #1. */
    Engine_MapObjectSetPosition(object, fixed_x, fixed_z);
}

static inline void Actor_CenterAndWalk(s32 actor, s32 priority, s32 dz)
{
    /* FAKEMATCH: Direct calls change Scene_RunScene3c3SequenceC (EN): mov	r1, #3 becomes neg	r2, r2. */
    Engine_ActorCenterAndWalk(actor, priority, dz);
}

static inline void Actor_WalkByAndWait(s32 actor, s32 dx, s32 dz)
{
    /* FAKEMATCH: Direct calls change FieldScene_RunPrimaryScript (EN): push	{lr} becomes push	{r5, lr}. */
    Engine_ActorWalkByAndWait(actor, dx, dz);
}

/* Shows the next message for a speaker, then waits the given frames. */
static inline void Event_ShowMessageAndWait(s32 speaker, s32 flags, s32 frames)
{
    /* FAKEMATCH: Direct calls change FieldScene_RunActorTransferSequence (EN): mov	r0, #20 becomes ldr	r5, .L0+92. */
    Engine_EventShowMessageAndWait(speaker, flags, frames);
}

/* Gives NULL for an actor the scene does not hold. */

/* Removes an actor from the scene and frees its slot. */

static inline void Actor_SetChildValue(s32 actor, s32 value)
{
    /* FAKEMATCH: Direct calls change FieldScene_RunScene381_02000e30 (EN): mov	r0, sl becomes lsl	r1, r1, #1. */
    Engine_ActorSetChildValue(actor, value);
}

/*
 * Starts an actor's action table. Passing ACTION_TABLE_STOP in place of a
 * table stops the action that is playing.
 */
#define ACTION_TABLE_STOP ((const u8 *)1)

/* Walks the scene's actor list after the map or camera changes. */

/*
 * Copies a block of map cells; unlike Map_CopyCells the destination comes
 * before the size.
 */
static inline void Map_CopyCellsTo(s32 src_x, s32 src_y, s32 dest_x, s32 dest_y, s32 width,
                                   s32 height)
{
    /* FAKEMATCH: Direct calls change UpdateStatueLight1 (JA): mov	r2, #5 becomes str	r3, [sp]. */
    Engine_MapCopyCellsTo(src_x, src_y, dest_x, dest_y, width, height);
}

/* Stores three map render values; a negative value leaves its slot unchanged. */
static inline void MapRender_SetValues(s32 value0, s32 value1, s32 value2)
{
    /* FAKEMATCH: Direct calls change FieldScene_RunComplexActorSequence (JA): mov	r0, #17 becomes ldr	r3, [r3]. */
    Engine_MapRenderSetValues(value0, value1, value2);
}

/* The colour buffers a scene blends the palette through. */
static inline void ColorBuffer_ApplySource(s32 value, s32 mode)
{
    /* FAKEMATCH: Direct calls change FieldScene_RunFourActorEncounter (EN): mov	r0, #3 becomes mov	r7, r8. */
    Engine_ColorBufferApplySource(value, mode);
}

static inline void ColorBuffer_ApplyTarget(s32 value, s32 mode)
{
    /* FAKEMATCH: Direct calls change FieldScene_PrepareStatueTransition (DE): mov	r2, #0 becomes ldr	r5, .L0+4. */
    Engine_ColorBufferApplyTarget(value, mode);
}

/*
 * Moves an actor to a whole-pixel position and waits until it arrives.
 * Walking plays the walk animation on the way and stands the actor still.
 */
static inline void Actor_MoveToAndWait(s32 actor, s32 x, s32 z)
{
    /* FAKEMATCH: Direct calls change RunEventScript01 (EN): mov	r2, #160 becomes mov	r5, #160. */
    Engine_ActorMoveToAndWait(actor, x, z);
}

static inline void Actor_WalkToAndWait(s32 actor, s32 x, s32 z)
{
    /* FAKEMATCH: Direct calls change RunEventScript01 (JA): mov	r0, #11 becomes ldr	r1, .L0. */
    Engine_ActorWalkToAndWait(actor, x, z);
}

/* Throws the actor upward, plays the jump sound and waits the given frames. */

/* Two speakers say their next messages at once; waits until both close. */
static inline void Event_ShowTwoMessagesAndWait(s32 speaker, s32 x, s32 y, s32 arg, s32 extra,
                                                s32 other_speaker, s32 other_x, s32 other_y,
                                                s32 other_arg, s32 other_extra, s32 flags)
{
    /* FAKEMATCH: Direct calls change HouseScene_RunRepairMorning (EN): mov	r3, #14 becomes mov	r3, #2. */
    Engine_EventShowTwoMessagesAndWait(speaker, x, y, arg, extra, other_speaker, other_x,
                                       other_y, other_arg, other_extra, flags);
}

/* Adds two characters to the party. */

/* Waits until the first two map render values settle, then clears the third. */

#endif

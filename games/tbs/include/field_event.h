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

/// 0x78 bytes of EWRAM where scene overlays keep state of their own, such as
/// Altmiller Cave's rock puzzle. It lies outside the heap, so a scene change
/// leaves it alone; GameState_InitDefaults clears it for a new game.
extern u8 gSceneState[];

/// The Colosso race work, heap slot HEAP_SLOT_COLOSSO_RACE under its own name.
extern u8 *gColossoWork;

#include "frame.h"

#include "field_obj.h"

/// The main loop pass's task key, as SCHEDULER_KEY_MAIN_LOOP: a scene's
/// per-frame callbacks run once a frame with the field's own.
enum { TASK_PRIORITY_SCENE = 3200 };

/// Animation numbers of an actor's sprite, for Engine_ActorSetAnimation.
enum ActorAnimation {
  ANIM_STAND = 1,
  ANIM_WALK = 2,
  ANIM_NOD = 3,
  ANIM_SHAKE_HEAD = 4,
  /// Leaning back for a moment before a jump. This and the leader's other
  /// poses below are as Isaac's sprite (spr_000.png) draws them.
  ANIM_LEADER_SPRING = 6,
  ANIM_LEADER_LEAP = 7, ///< in mid-stride, leaning forward
  /// The leader leaning into what it pushes, as scenes play it while a statue
  /// or stone pillar slides ahead of it.
  ANIM_PUSH = 8,
  /// ANIM_STAND's idle with more glances to each side, as when a whirlwind
  /// catches the leader.
  ANIM_LEADER_LOOK_AROUND = 9,
  ANIM_LEADER_CLIMB = 10,      ///< climbing a wall, seen from behind
  ANIM_LEADER_CLING = 12,      ///< still on a wall, seen from behind
  ANIM_LEADER_CLIMB_EAST = 14, ///< reaching along a wall as he climbs east
  ANIM_LEADER_CLIMB_WEST = 15, ///< reaching along a wall as he climbs west
  /// Held in ANIM_LEADER_LEAP's mid-stride lean, as he lunges into a fight.
  ANIM_LEADER_LUNGE = 16,
  ANIM_LEADER_LEAN_BACK = 22,   ///< leaning back, his head thrown back
  ANIM_LEADER_ARMS_RAISED = 28, ///< both arms raised over his head
  /// Held in ANIM_LEADER_SPRING's crouch, knees bent, as after a lost match.
  ANIM_LEADER_CROUCH = 35
};

// Speaker bits, above the actor id in the low 12 bits of a message command's
// speaker, as UiText_OpenMessageAtObject reads them.
#define SPEAKER_PORTRAIT_LEFT 0x1000  ///< the portrait left of the speaker
#define SPEAKER_PORTRAIT_RIGHT 0x2000 ///< the portrait right of the speaker
#define SPEAKER_WINDOW_ABOVE 0x4000   ///< the message window above the speaker
#define SPEAKER_WINDOW_BELOW 0x8000   ///< the message window never above it

/// One step of a cell animation: copy the width by height cells at srcX, srcY
/// over the destination, then wait frames.
#define MAP_CELL_STEP(srcX, srcY, width, height, frames)                       \
  (srcX), (srcY), (width), (height), (frames)
#define MAP_CELL_STEPS_END 0xffff ///< a source column that ends the steps

/// A cell animation, such as a door opening: its steps and the cell they are
/// copied to (Map_PlayMetatileCopySequence, which blocks until the last).
struct MapCellAnim {
  /// MAP_CELL_STEP records, ended by MAP_CELL_STEPS_END, for
  /// Map_PlayMetatileCopySequence.
  const u16 *steps;
  u16 x; ///< destination cell column
  u16 y; ///< destination cell row
};

// Imports of main-image routines. EventRequestExit, ActorSetPosition,
// ActorSetAnimation and RandomNext have differing argument counts at their
// call sites, as measured in the bytes, so retain old-style declarations.
/// Opens an event (FieldEvent_Begin): closes any timed notice, stops the
/// leader and makes it the speaker.
void Engine_EventBegin(void);
/// Closes an event (FieldEvent_End): the camera follows the leader again and
/// FLAG_LURE_CAP is refreshed.
void Engine_EventEnd(void);
/// Waits the given frames unless debug fast-forward is on
/// (FieldEvent_WaitFrames).
void Engine_EventWait(s32 frames);
/// Runs the frame loop for the given frames (WaitFrames); debug fast-forward
/// doesn't shorten it.
void Engine_TaskWait(s32 frames);
/// Sets the number of the event's next message in the field work
/// (Event_SetMessageId).
void Engine_EventSetMessage(s32 message);
/// Opens the current message at speaker and waits until it has been read and
/// its window has closed (FieldEvent_ShowMessageAndWait). The speaker may carry
/// SPEAKER_ bits.
void Engine_EventShowMessage(s32 speaker, s32 flags);
/// Opens the current message beside speaker, with its portrait, and moves the
/// event on to the next (UiText_OpenMessageAtObject). Returns the message
/// window once its text is typed out, without waiting for it to be read.
s32 Engine_EventOpenMessage(s32 speaker, s32 flags);
/// Asks the current message at speaker, then shows the message after it for
/// yes or the one after that for no (FieldEvent_AskYesNoAndShowResponse).
/// Returns 0 for yes, 1 for no; either way the event goes on past both.
s32 Engine_EventAskYesNo(s32 speaker, s32 flags);
/// Offers yes or no once the message on screen is read, the speaker nodding at
/// yes and shaking its head at no (Event_AskYesNoWithActorAnimation). Returns 0
/// for yes, 1 for no; flags puts the choice below the message.
s32 Engine_EventChooseYesNo(s32 actor, s32 flags);
/// Shows message in a window centred on the screen, waits until it is typed out
/// and closes it (UiText_ShowPositionedMessageAndWait). The bits of flags zoom
/// the window, lower it or leave it open.
void Engine_MessageShowCentered(s32 message, s32 flags);
/// Sets the field's exit request (Event_SetExitRequest); 999 leaves for
/// gGameState's scene and entrance.
void Engine_EventRequestExit();
/// Starts the event's screen transition (Event_StartScreenTransition).
void Engine_EventOpenScreen(void);
/// Finishes the event's screen transition (Event_FinishScreenTransition).
void Engine_EventCloseScreen(void);
/// Waits as long as the event's screen transition runs
/// (Event_WaitTransitionFrames).
void Engine_EventWaitForScreen(void);
/// Fades the game's layers to black (Fade_ToBlack). The argument is the fade's
/// length in frames: the cleared-game menu passes 0 to black the screen at
/// once.
void Engine_BlendSetDarkenTarget16(s32 target);
/// Returns the ROM address of a resource, from the resource directory
/// (Resource_GetTableEntry).
u8 *Engine_ResourceGetTableEntry(s32 resource);
/// Puts an actor at rest at (x, z), in 16.16 pixels; one that follows the
/// ground keeps its height above the terrain
/// (ObjectMotion_SetHorizontalPositionWithTerrain).
void Engine_ActorSetPosition();
/// Sets an actor's top speed and acceleration, in 16.16 pixels per frame
/// (ObjectMotion_SetSpeedParameters).
void Engine_ActorSetSpeed(s32 actor, s32 speed, s32 acceleration);
/// Sends an actor toward (x, z), in pixels, in whatever animation it has
/// (ObjectMotion_ResetAndSetPosition).
void Engine_ActorSetDestination(s32 actor, s32 x, s32 z);
/// Sends an actor (dx, dz) pixels from where it stands
/// (ObjectMotion_OffsetPositionAndResetMotion).
void Engine_ActorSetDestinationOffset(s32 actor, s32 dx, s32 dz);
/// Walks an actor toward (x, z), in pixels, in ANIM_WALK
/// (ObjectMotion_ResetAndSetPositionInMode2).
void Engine_ActorWalkTo(s32 actor, s32 x, s32 z);
/// Walks an actor (dx, dz) pixels from where it stands, in ANIM_WALK
/// (ObjectMotion_OffsetPositionAndReset).
void Engine_ActorWalkBy(s32 actor, s32 dx, s32 dz);
/// Waits for an actor to arrive, 600 frames at most, and stands it still
/// (ObjectMotion_CommitCurrentPositionAndActivate).
void Engine_ActorWaitForMove(s32 actor);
/// Turns an actor toward facing, an angle where 0x10000 is a full turn, then
/// waits frames (ObjectMotion_ArmCallback).
void Engine_ActorFaceDirection(s32 actor, s32 facing, s32 frames);
/// Turns actor at once to face target, then waits frames
/// (ObjectMotion_SetAngleToward).
void Engine_ActorFaceActor(s32 actor, s32 target, s32 frames);
/// Turns two actors toward each other, then waits frames (Object_LinkPair).
void Engine_ActorFaceEachOther(s32 actor, s32 other, s32 frames);
/// Switches an actor's sprite to an ActorAnimation (Object_SetModeById).
void Engine_ActorSetAnimation();
/// Plays an ActorAnimation and waits for it to end
/// (Motion_SetModeAndWaitAnimation).
void Engine_ActorSetAnimationAndWait(s32 actor, s32 animation);
/// Starts an actor squashing and springing back repeats times, 1 to 3
/// (ObjectMotion_SetVariantCallback).
void Engine_ActorStartRepeatedMotion(s32 actor, s32 repeats);
/// The same squash and spring as Engine_ActorStartRepeatedMotion, waiting for
/// it to end (Motion_SetVarCbAndRefresh).
void Engine_ActorRunRepeatedMotion(s32 actor, s32 repeats);
/// Shows an emote bubble over an actor, which frees itself after 46 frames,
/// then waits frames (FieldFx_SpawnLinked). See EMOTE_IN_FRONT.
void Engine_ActorShowEmote(s32 actor, s32 emote, s32 frames);
/// Attaches, removes or plays once the effect object that rides an actor, by
/// enum AttachedEffectFlag (FieldFx_ConfigureLinkedObject).
void Engine_ActorSetAttachedEffect(s32 actor, s32 effect);
/// Fixes an actor's sprite priority, which then no longer follows its depth
/// (ObjectMotion_SetActionVariant).
void Engine_ActorSetSpritePriority(s32 actor, s32 priority);
/// Sets the flags of a one-sprite object's sprite, bit 0 drawing its shadow
/// (ObjectDispatch_SetSingleChildFlags).
void Engine_ActorSetSpriteFlags(struct FieldActor *actor, s32 flags);
/// Makes the camera follow actor (Camera_FollowActor). Unless keep_position is
/// set it jumps there at once and, off the world map, the map is redrawn.
void Engine_CameraFollowActor(s32 actor, s32 keep_position);
/// Sets the camera's top speed and acceleration, in 16.16 pixels per frame
/// (Camera_SetSpeedParameters).
void Engine_CameraSetSpeed(s32 speed, s32 acceleration);
/// Sends the camera to a point in 16.16 pixels, held far enough inside the map
/// that the screen never shows past its edge (Camera_MoveWithinMapBounds). -1
/// keeps an axis; pan 0 jumps there at once.
void Engine_CameraMoveTo(s32 fixed_x, s32 fixed_y, s32 fixed_z, s32 pan);
/// Sends the camera to an actor's x and z, inside the map's edges
/// (Object_PlaceCurrentWithinCameraBounds).
void Engine_CameraMoveToActor(s32 actor, s32 pan);
/// Waits, 600 frames at most, for the camera to arrive, then two frames more
/// (Camera_WaitForArrival).
void Engine_CameraWaitForMove(void);
/// Copies a rectangle of cells whole, block and attributes, from (srcX, srcY)
/// to (destX, destY), and redraws those on screen (Map_CopyMetatileCellsRect).
void Engine_MapCopyCells(s32 srcX, s32 srcY, s32 width, s32 height, s32 destX,
                         s32 destY);
/// Copies only the attributes of a rectangle of cells (flags, collision code
/// and shape), keeping each destination's block, so nothing on screen changes
/// (Map_CopyCellAttributeRect).
void Engine_MapCopyCellAttributes(s32 srcX, s32 srcY, s32 width, s32 height,
                                  s32 destX, s32 destY);
/// Centres the camera on the point the map follows and scrolls the layers there
/// (Map_CenterOnFollowedPoint).
void Engine_MapRedraw(void);
/// Sets the screen shake's amplitudes and decay, in 16.16, leaving alone the
/// part whose argument is negative (Map_SetScreenShake).
void Engine_WorkSetValuesIfNonNegative(s32 first, s32 second, s32 third);
/// Returns 1 when a game flag is set (GameFlag_Test).
s32 Engine_GameFlagIsSet(s32 flag);
/// Sets a game flag (GameFlag_SetBit).
void Engine_GameFlagSet(s32 flag);
/// Clears a game flag (GameFlag_ClearBit).
void Engine_GameFlagClear(s32 flag);
/// Plays a sound effect, fanfare or music cue, or a fade control
/// (AudioCommand_Play); the low 12 bits are the cue.
void Engine_AudioPlayCue(s32 cue);
/// Runs a shop's counter until the player leaves (Shop_Run): shop is the row of
/// its stock table and keeper the object whose portrait the messages show.
void Engine_ShopOpen(s32 shop, s32 keeper);
/// Runs an inn's counter (Inn_CheckIn): inn is the row of the price table and
/// keeper the innkeeper's object.
void Engine_InnOpen(s32 inn, s32 keeper);
/// Runs a sanctum's welcome, services and farewell (Shop_ConfirmAct); priest
/// is the keeper's object.
void Engine_SanctumOpen(s32 priest);
/// Unschedules the task running callback (Scheduler_RemoveCallback). Returns
/// its old index, or -1.
s32 Engine_TaskRemoveCallback(void (*callback)(void));
/// Advances the game's random number generator and returns 0 to 0xffff
/// (Random16).
s32 Engine_RandomNext();
/// Returns the sine of a 16-bit angle, where 0x10000 is a full turn, in 16.16
/// (Trig_Sin).
s32 Engine_MathSin(s32 angle);
/// Returns the cosine of a 16-bit angle in 16.16 (Trig_Cos, which adds a
/// quarter turn and goes on into Trig_Sin).
s32 Engine_MathCos(s32 angle);
/// The remainder of an unsigned division, from the ARM routine in IWRAM that
/// also serves as __umodsi3 (IwramUnsignedRemainder).
u32 Math_RemainderUnsigned(u32 dividend, u32 divisor);
/// Signed division, the compiler's helper for `/`, imported from the ARM
/// routine in IWRAM (IwramSignedDivide).
s32 __divsi3(s32 dividend, s32 divisor);
/// Returns heap slot's block, first allocating size bytes for it when it has
/// none (Runtime_AllocateHeapBlock). Returns NULL when no pool has room.
void *Engine_HeapAllocate(s32 slot, s32 size);
/// Frees heap slot's block, and with it every later block of the same pool
/// (Runtime_ReleaseHeapBlock).
void Engine_HeapRelease(s32 slot);
/// Gives cache entry block size bytes of sprite VRAM and copies data there
/// (VramCache_Load). Returns the first sprite tile number, or 0 when there was
/// no room.
s32 Engine_VramLoad(s32 block, s32 size, const void *data);
/// Creates a field object at (x, y, z) in 16.16 pixels, or returns NULL when
/// none is free (FieldObject_Create). type / 4096 is its enum ObjectCreateKind
/// and the low 12 bits the sprite's animation id.
struct FieldActor *Engine_ObjectCreate(s32 type, s32 fixed_x, s32 fixed_y,
                                       s32 fixed_z);
/// Switches every sprite of an object to an animation, the scene's import name
/// for ObjectDispatch_SelectAnimation.
void Object_SetMode(struct FieldActor *object, s32 animation);
/// Gives an object a sprite script and starts it from the top
/// (ObjectDispatch_Initialize).
void Engine_ObjectSetScript(struct FieldActor *object, const s32 *script);
/// Sets a one-sprite object's OAM blend mode, an ObjectBlendMode
/// (ObjectDispatch_SetChildBlendMode).
void Engine_ObjectSetBlendMode(struct FieldActor *object, s32 mode);
/// Sets the colour mode (SpriteColorMode) of a one-sprite object's layers,
/// except those drawn hidden (Animation_ApplyChildValues).
void Engine_ObjectSetPartPalettes(struct FieldActor *object, s32 palette);
/// Sends an object toward a point in 16.16 pixels, coasting to a stop there
/// (Object_SetMoveTarget).
void Engine_ObjectSetPosition(struct FieldActor *object, s32 fixed_x,
                              s32 fixed_y, s32 fixed_z);
/// Plays a cell animation's MAP_CELL_STEP records onto the cell at (destX,
/// destY), returning only after the last (Map_PlayMetatileCopySequence).
void Engine_MapAnimateCells(const u16 *steps, s32 destX, s32 destY);
/// Moves the actor of a marker to (x, z) on the ground, in 16.16 pixels; -1
/// keeps the centre of its cell on that axis (Marker_SetActorPosition).
void Engine_MapObjectSetPosition(s32 object, s32 fixed_x, s32 fixed_z);
/// Draws an item's icon into the glyph work buffer, with no frame or marks
/// (Ui_PrepareTransferForItem).
void Engine_ItemLoadIcon(s32 item);
/// Shows an item's picture above the leader for 80 frames
/// (FieldFx_StartEffectObject22). Bit 0 of height adds sparkles and bit 1
/// raises it as a found item.
void Engine_ItemShowFound(s32 item, s32 height);
/// Gives an item to the first party member with room and says so, letting the
/// player drop something when every bag is full (PartyInventory_GiveItem).
/// Returns the member who got it, or -1 when it was left behind.
s32 Engine_PartyGiveItem(s32 item, s32 flags);
/// Stops an actor's action and puts it back under the event's control, on the
/// idle script (ObjectMotion_EnableActionAndResetMotion).
void Engine_ActorStop(s32 actor);
/// Walks an actor to the middle of its 16-pixel column, fixes its sprite
/// priority, then sends it dz pixels along z (ObjectMotion_CenterOnColumn).
void Engine_ActorCenterAndWalk(s32 actor, s32 priority, s32 dz);
/// Walks an actor (dx, dz) pixels, waits for it to arrive and stands it still
/// (ObjectMotion_CommitPositionAndActivate).
void Engine_ActorWalkByAndWait(s32 actor, s32 dx, s32 dz);
/// Makes the leader jump two tiles forward where the map has a jump spot
/// (Field_TryJumpForward). Returns 0 after a jump and -1 when it can't jump.
s32 Engine_LeaderCheckAhead(void);
/// Prepares the work and spark tiles of a field Psynergy effect for ability
/// (FieldFx_LoadActionEffectResources).
void Engine_PsynergyBegin(s32 ability, s32 flags);
/// Records an effect's caster and target; with no target the effect plays 16
/// pixels in front of the caster (FieldFx_SetupObjectPair).
void Engine_PsynergySetTarget(s32 caster, s32 target);
/// Puts the caster in its casting pose and, unless the effect is silent or
/// quiet, flashes it and shows the ability's name (EventObject_Initialize).
void Engine_PsynergyRaiseHands(void);
/// Runs one step of the current field effect, an enum PsynergyEffectStep
/// (Field_DispatchTypeHandler).
void Engine_PsynergyPlayEffect(s32 effect);
/// Stops the caster's per-frame hook, restores its colours and waits a frame
/// (FieldFx_StopCasterHook).
void Engine_PsynergyLowerHands(void);
/// Stops the wave effect: the caster's colours and the palettes come back and
/// its work is released (FieldFx_FinishSceneAndReleaseHeapBlock).
void Engine_PsynergyCancel(void);
/// Shows the current message as Engine_EventShowMessage does, then waits frames
/// (FieldEvent_ShowMessageThenWait).
void Engine_EventShowMessageAndWait(s32 speaker, s32 flags, s32 frames);
/// Returns the camera object an event moves (Camera_GetEventObject).
struct FieldActor *Engine_EventGetViewCenter(void);
/// Returns the actor in a slot, or NULL (ObjectTable_Get).
struct FieldActor *Engine_ActorLookup(s32 actor);
/// Releases an actor and empties its place in the actor table
/// (ObjectTable_DestroyById).
void Engine_ActorDestroy(s32 actor);
/// Gives every sprite part of an actor a sprite param; with bit 0x100 the actor
/// blinks instead (ObjectGroup_ConfigureChildValue).
void Engine_ActorSetChildValue(s32 actor, s32 value);
/// Starts an actor's action, an enum ActorBehavior or a script's address, and
/// makes it face where it moves (ObjectMotion_EnableActionAndSetCallback).
void Engine_ActorEnableActionCallback(s32 actor, s32 action);
/// The engine encodes either a built-in action or an action-script address.
static inline void Actor_SetActionScript(s32 actor, const s32 *script) {
  Engine_ActorEnableActionCallback(actor, (s32)script);
}

/// Runs a script, waiting at most 300 frames for its final command.
static inline void Actor_RunActionScriptAndWait(s32 actor, const s32 *script) {
  Object_SetActionCallbackAndRefreshById(actor, (s32)script);
}

/// Starts one of the engine's own actions on an actor, an enum ActorBehavior,
/// where Actor_SetActionScript gives it a scene's script.
static inline void Actor_SetBuiltinAction(s32 actor, s32 action) {
  Engine_ActorEnableActionCallback(actor, action);
}

/// Starts an actor's action, an enum ActorBehavior or a script to run, leaving
/// its facing flag alone (ObjectMotion_SetActionCallback).
void Engine_ActorSetActionCallback(struct FieldActor *actor, s32 value);
/// Puts back the actors ObjectTable_Snapshot saved and moves the camera to the
/// leader's height (ObjectTable_Restore).
void Engine_ActorsRefresh(void);
/// Copies the block numbers of a rectangle of cells, keeping each destination's
/// attributes, and redraws the copied cells on screen
/// (Map_CopyMetatileIndicesRect). It takes the destination before the size.
void Engine_MapCopyCellsTo(s32 srcX, s32 srcY, s32 destX, s32 destY, s32 width,
                           s32 height);
/// Copies the block numbers of a rectangle of cells, keeping each destination's
/// attributes (Map_CopyCellIndexRect). XXX: the routine and its callers
/// (lalivero.c) pass the size before the destination, as the parameter names
/// here do not.
void Engine_MapCopyCellsLayered(s32 srcX, s32 srcY, s32 destX, s32 destY,
                                s32 width, s32 height);
/// Sets the colours the colour buffer shows now (ColorBuffer_SetSource): value
/// is one colour, a PALETTE_TRANSFORM_ value or a ready buffer, and mode picks
/// all palettes (0), the BG's (1) or the OBJ's (2).
void Engine_ColorBufferApplySource(s32 value, s32 mode);
/// Sets the colours the colour buffer moves toward, value and mode as for
/// Engine_ColorBufferApplySource (ColorBuffer_SetTarget).
void Engine_ColorBufferApplyTarget(s32 value, s32 mode);
/// Starts moving the shown colours to the target colours over frames
/// (ColorBuffer_StartInterpolation).
void Engine_ColorBufferInterpolate(s32 frames);
/// Sends an actor toward (x, z) as Engine_ActorSetDestination does, then waits
/// for it, giving up after 600 frames (ObjectMotion_SetPositionAndCommit).
void Engine_ActorMoveToAndWait(s32 actor, s32 x, s32 z);
/// Walks an actor to (x, z), in pixels, waits for it to arrive and stands it in
/// ANIM_STAND (ObjectMotion_SetPositionAndReset).
void Engine_ActorWalkToAndWait(s32 actor, s32 x, s32 z);
/// Throws an actor upward at height pixels per frame with a jump sound, then
/// waits frames while gravity brings it down (ObjectMotion_Launch).
void Engine_ActorJump(s32 actor, s32 height, s32 frames);
/// Two actors speak at once, each message and portrait at its own tile
/// position, until A, B, L or R closes both
/// (FieldEvent_ShowTwoMessagesAndWait). The routine takes ten arguments and
/// never reads flags.
void Engine_EventShowTwoMessagesAndWait(s32 speaker, s32 x, s32 y, s32 arg,
                                        s32 extra, s32 other_speaker,
                                        s32 otherX, s32 other_y, s32 other_arg,
                                        s32 other_extra, s32 flags);
/// Adds two units to the party and announces them together
/// (Event_PrepareTwoObjectsAndApply); Vale's scene calls it twice.
void Engine_PartyAddMembers(s32 first, s32 second);
/// Waits until both shake amplitudes are below 1/256 pixel, 300 frames at
/// most, then stops the shake (Map_WaitForShakeToSettle).
void Engine_MapRenderWaitForValues(void);

/// The same move and wait as Engine_ActorMoveToAndWait, under a second import
/// name (ObjectMotion_SetPositionAndCommit).
void Engine_ObjectMotionSetPositionAndCommit(s32 actor, s32 x, s32 z);
/// Releases an object's sprites and zeroes it, so it can be handed out again
/// (ObjectDispatch_Release).
void Engine_ObjectDispatchRelease(struct FieldActor *object);
/// The shake wait of Engine_MapRenderWaitForValues under the name Goma Cave
/// Entrance imports it by (Map_WaitForShakeToSettle).
void Engine_MapWaitWorkValuesBelow256(void);
/// Shows a found item's picture object; bit 0 of mode adds sparkles and bit 1
/// raises it above the leader (ItemDisplay_ShowPicture).
void Engine_ItemShowPicture(struct FieldActor *object, s32 mode);

#ifdef FIELD_STAGED_ACTOR_IMPORTS
/// Declared for the scenes that define FIELD_STAGED_ACTOR_IMPORTS in place of
/// Event_PlayCue. Nothing calls it and no import resolves it: those scenes call
/// Engine_AudioPlayCue.
void Audio_PlayCue(s32 cue);
#endif

/// Moves the event's message number count messages on, past lines the scene
/// doesn't show.
static inline void Event_SkipMessages(s32 count) {
  u16 *message = &gEventWork->message;

  *message = *message + count;
}

/// Shows the scene's next line of dialogue and waits for the player to read it.
static inline void Event_ShowMessage(s32 speaker, s32 flags) {
  Engine_EventShowMessage(speaker, flags);
}

/// Opens the scene's next line and returns its window once the text is typed
/// out, so the scene can act while it is still on screen.
static inline s32 Event_OpenMessage(s32 speaker, s32 flags) {
  return Engine_EventOpenMessage(speaker, flags);
}

/// Puts the current line to the player as a question and shows the line that
/// answers it; returns 0 for yes.
static inline s32 Event_AskYesNo(s32 speaker, s32 flags) {
  return Engine_EventAskYesNo(speaker, flags);
}

/// Looks up an actor for a scene, NULL when it doesn't exist (Object_GetById).
static inline struct FieldActor *Actor_Get(s32 actor) {
  return Object_GetById(actor);
}

/// Places an actor at a point at once, without walking it there.
static inline void Actor_SetPosition(s32 actor, s32 fixed_x, s32 fixed_z) {
  Engine_ActorSetPosition(actor, fixed_x, fixed_z);
}

/// Sets how fast an actor moves, and how quickly it gets up to speed, for the
/// scene's next moves.
static inline void Actor_SetSpeed(s32 actor, s32 speed, s32 acceleration) {
  Engine_ActorSetSpeed(actor, speed, acceleration);
}

/// Moves an actor to a point in the pose it has, without walking; the scene
/// carries on meanwhile.
static inline void Actor_SetDestination(s32 actor, s32 x, s32 z) {
  Engine_ActorSetDestination(actor, x, z);
}

#ifndef FIELD_STAGED_ACTOR_IMPORTS
/// Moves an actor some distance from where it stands, without waiting for it.
static inline void Actor_SetDestinationOffset(s32 actor, s32 dx, s32 dz) {
  Engine_ActorSetDestinationOffset(actor, dx, dz);
}
#endif

/// Walks an actor to a point; the scene goes on while it walks.
static inline void Actor_WalkTo(s32 actor, s32 x, s32 z) {
  Engine_ActorWalkTo(actor, x, z);
}

/// Walks an actor some distance from where it stands, without waiting for it
/// to get there.
static inline void Actor_WalkBy(s32 actor, s32 dx, s32 dz) {
  Engine_ActorWalkBy(actor, dx, dz);
}

/// Turns an actor to a direction and holds the scene for a few frames.
static inline void Actor_FaceDirection(s32 actor, s32 facing, s32 frames) {
  Engine_ActorFaceDirection(actor, facing, frames);
}

/// Has one actor look at another at once, then pauses the scene.
static inline void Actor_FaceActor(s32 actor, s32 target, s32 frames) {
  Engine_ActorFaceActor(actor, target, frames);
}

/// Pops an emote bubble up over an actor, then lets frames pass before the
/// scene goes on.
static inline void Actor_ShowEmote(s32 actor, s32 emote, s32 frames) {
  Engine_ActorShowEmote(actor, emote, frames);
}

/// Attaches an effect object to an actor, takes it off, or plays one over it
/// once.
static inline void Actor_SetAttachedEffect(s32 actor, s32 effect) {
  Engine_ActorSetAttachedEffect(actor, effect);
}

/// Sets how fast the camera pans for the scene's next camera moves.
static inline void Camera_SetSpeed(s32 speed, s32 acceleration) {
  Engine_CameraSetSpeed(speed, acceleration);
}

/// Pans the camera to a point in the room, or cuts there when pan is 0.
static inline void Camera_MoveTo(s32 fixed_x, s32 fixed_y, s32 fixed_z,
                                 s32 pan) {
  Engine_CameraMoveTo(fixed_x, fixed_y, fixed_z, pan);
}

/// Changes part of the room in place, copying whole cells, picture and
/// collision alike, from another part of the map.
static inline void Map_CopyCells(s32 srcX, s32 srcY, s32 width, s32 height,
                                 s32 destX, s32 destY) {
  Engine_MapCopyCells(srcX, srcY, width, height, destX, destY);
}

/// Changes where the party can walk in part of the room, copying only the
/// cells' collision and flags.
static inline void Map_CopyCellAttributes(s32 srcX, s32 srcY, s32 width,
                                          s32 height, s32 destX, s32 destY) {
  Engine_MapCopyCellAttributes(srcX, srcY, width, height, destX, destY);
}

/// Sets shake amplitudes in fixed-point pixels and the per-frame decay factor.
/// A negative argument keeps that component unchanged; Q16_ONE keeps the shake.
static inline void Map_SetShake(s32 horizontal, s32 vertical, s32 decay) {
  Engine_WorkSetValuesIfNonNegative(horizontal, vertical, decay);
}

/// A scene's GameFlag_Set is FieldEvent_GameFlagSet, which keeps it apart from
/// the main image's declaration in game_flags.h.
#define GameFlag_Set FieldEvent_GameFlagSet
/// FieldEvent_GameFlagIsSet, apart from field.h's GameFlag_IsSet.
#define GameFlag_IsSet FieldEvent_GameFlagIsSet
/// FieldEvent_GameFlagClear, apart from game_flags.h's GameFlag_Clear.
#define GameFlag_Clear FieldEvent_GameFlagClear
/// Lets a scene branch on what the player has done: 1 when the FLAG_ is set.
static inline s32 FieldEvent_GameFlagIsSet(s32 flag) {
  return Engine_GameFlagIsSet(flag);
}

/// Records something for later scenes to test, setting its FLAG_.
static inline void FieldEvent_GameFlagSet(s32 flag) {
  Engine_GameFlagSet(flag);
}

/// Clears a FLAG_ a scene tracks, such as a FLAG_SCENE_TEMP_ one.
static inline void FieldEvent_GameFlagClear(s32 flag) {
  Engine_GameFlagClear(flag);
}

#ifndef FIELD_STAGED_ACTOR_IMPORTS
/// Plays a sound effect or music from a scene: the SE command of event.h.
static inline void Event_PlayCue(s32 cue) { Engine_AudioPlayCue(cue); }
#endif

/// A random number from 0 to 0xffff for a scene.
static inline s32 Random_Next(void) { return Engine_RandomNext(); }

/// Signed division written as a call; the `/` compiles to __divsi3.
static inline s32 Math_Divide(s32 dividend, s32 divisor) {
  return dividend / divisor;
}

/// Creates a free object in the room, outside the actor table, or returns NULL
/// when every object is taken.
static inline struct FieldActor *Object_Create(s32 type, s32 fixed_x,
                                               s32 fixed_y, s32 fixed_z) {
  return Engine_ObjectCreate(type, fixed_x, fixed_y, fixed_z);
}

/// Starts an object on a sprite script from its first command.
static inline void Object_SetScript(struct FieldActor *object,
                                    const s32 *script) {
  Engine_ObjectSetScript(object, script);
}

/// OAM blend modes for Engine_ObjectSetBlendMode: OBJECT_BLEND_TRANSLUCENT
/// alpha-blends the sprite with what lies behind it.
enum ObjectBlendMode { OBJECT_BLEND_NORMAL = 0, OBJECT_BLEND_TRANSLUCENT = 1 };

/// Plays a cell animation, such as a door opening, and holds the scene until
/// its last step.
static inline void Map_AnimateCells(const u16 *steps, s32 destX, s32 destY) {
  Engine_MapAnimateCells(steps, destX, destY);
}

/// Moves a marker's actor to a point on the ground.
static inline void MapObject_SetPosition(s32 object, s32 fixed_x, s32 fixed_z) {
  Engine_MapObjectSetPosition(object, fixed_x, fixed_z);
}

/// Lines an actor up with the middle of its column, then walks it straight
/// along z.
static inline void Actor_CenterAndWalk(s32 actor, s32 priority, s32 dz) {
  Engine_ActorCenterAndWalk(actor, priority, dz);
}

/// Walks an actor some distance and waits until it is there.
static inline void Actor_WalkByAndWait(s32 actor, s32 dx, s32 dz) {
  Engine_ActorWalkByAndWait(actor, dx, dz);
}

/// Shows a line of dialogue, then pauses before the scene goes on.
static inline void Event_ShowMessageAndWait(s32 speaker, s32 flags,
                                            s32 frames) {
  Engine_EventShowMessageAndWait(speaker, flags, frames);
}

/// Sets a sprite param on every part of an actor's sprite, or makes it blink.
static inline void Actor_SetChildValue(s32 actor, s32 value) {
  Engine_ActorSetChildValue(actor, value);
}

/// Not used in TBS.
#define ACTION_TABLE_STOP ((const u8 *)1)

/// Changes what part of the room shows, copying the cells' pictures but not
/// their collision.
static inline void Map_CopyCellsTo(s32 srcX, s32 srcY, s32 destX, s32 destY,
                                   s32 width, s32 height) {
  Engine_MapCopyCellsTo(srcX, srcY, destX, destY, width, height);
}

/// Changes the pictures of part of the room as Map_CopyCellsTo does, through
/// Map_CopyCellIndexRect, which takes the size first.
static inline void Map_CopyCellsLayered(s32 srcX, s32 srcY, s32 destX,
                                        s32 destY, s32 width, s32 height) {
  Engine_MapCopyCellsLayered(srcX, srcY, destX, destY, width, height);
}

/// Sets the colours the screen shows now, where a scene's colour fade starts.
static inline void ColorBuffer_ApplySource(s32 value, s32 mode) {
  Engine_ColorBufferApplySource(value, mode);
}

/// Sets the colours a scene's colour fade ends on.
static inline void ColorBuffer_ApplyTarget(s32 value, s32 mode) {
  Engine_ColorBufferApplyTarget(value, mode);
}

/// Moves an actor to a point in the pose it has and waits for it.
static inline void Actor_MoveToAndWait(s32 actor, s32 x, s32 z) {
  Engine_ActorMoveToAndWait(actor, x, z);
}

/// Walks an actor to a point and waits for it, then stands it still.
static inline void Actor_WalkToAndWait(s32 actor, s32 x, s32 z) {
  Engine_ActorWalkToAndWait(actor, x, z);
}

/// Has two actors speak at once, each window placed by the scene.
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

#ifndef ALCHEMY_SCRIPT_LAYER_H
#define ALCHEMY_SCRIPT_LAYER_H

/// @file
/// The script layer: the engine services a map's scripts call, declared
/// once (Engine_ScriptStart, Engine_ObjectEventJump, Engine_ScriptSetMessage
/// and the rest), with the constants and small inlines maps share.
///
/// It is a habit of the whole game written once, as declarations and macros: a
/// layer, not a compiler-steering device (AGENTS.md S2). The script.h commands
/// and field_service.h are written over it. A map's script reads
/// `SCRIPT_START(); Engine_ObjectEventStop(9); OBJECT_FACE(9,
/// LOCALID_PARTY_LEADER, 0); SCRIPT_END();`.

#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "map_grid_copy.h"

#include "static_assert.h"

#include "game_state.h"

// A map stores into its own const tables: an overlay's data is in RAM.
// The games' compiler takes the store as written; a current GCC refuses it,
// so the modern build names the place without its const.
#ifdef MODERN
/// The comma expression removes qualifiers without promoting narrow integers.
#define STORE_CONST(place) (*(__typeof__(((void)0, (place))) *)&(place))
#else
#define STORE_CONST(place) (place)
#endif

/// 0x78 bytes of EWRAM where map overlays keep state of their own, such as
/// Altmiller Cave's rock puzzle. It lies outside the heap, so a map change
/// leaves it alone; GameState_InitializeDefaults clears it for a new game.
extern u8 gMapLocalState[];

/// The Colosso race work, heap slot HEAP_SLOT_COLOSSO_RACE under its own name.
extern u8 *gColossoWork;

#include "frame.h"

#include "object_event.h"

/// The main loop pass's task key, as TASK_PRIORITY_MAIN_LOOP: a map's
/// per-frame callbacks run once a frame with the field's own.
enum { TASK_PRIORITY_MAP = 3200 };

/// Animation numbers of an object event's sprite, for
/// Engine_ObjectEventSetAnimation.
enum ObjectEventAnimation {
  /// Standing still: nearly every sprite's first frame held, without the
  /// blinks and small gestures of ANIMATION_STAND.
  ANIMATION_STILL = 0,
  ANIMATION_STAND = 1,
  ANIMATION_WALK = 2,
  ANIMATION_NOD = 3,
  ANIMATION_SHAKE_HEAD = 4,
  /// Running, leaning forward, four frames a step to the walk's six. This and
  /// the leader's other poses below are as Isaac's sprite (spr_000.png) draws
  /// them.
  ANIMATION_LEADER_RUN = 5,
  ANIMATION_LEADER_SPRING = 6, ///< leaning back for a moment before a jump
  ANIMATION_LEADER_LEAP = 7,   ///< in mid-stride, leaning forward
  /// The leader leaning into what it pushes, as maps play it while a statue
  /// or stone pillar slides ahead of it.
  ANIMATION_PUSH = 8,
  /// ANIMATION_STAND's idle with more glances to each side, as when a whirlwind
  /// catches the leader.
  ANIMATION_LEADER_LOOK_AROUND = 9,
  ANIMATION_LEADER_CLIMB = 10,      ///< climbing a wall, seen from behind
  ANIMATION_LEADER_CLING = 12,      ///< still on a wall, seen from behind
  ANIMATION_LEADER_CLIMB_EAST = 14, ///< reaching along a wall as he climbs east
  ANIMATION_LEADER_CLIMB_WEST = 15, ///< reaching along a wall as he climbs west
  /// One leg forward, braced, held briefly: the frame ANIMATION_LEADER_LEAP
  /// holds
  /// and the run starts on, as when he lunges into a fight or pulls someone
  /// from a river.
  ANIMATION_LEADER_STRIDE = 16,
  /// Holding out a glowing hand, as the party's sprites cast Psynergy
  /// (spr_000.png, spr_001.png).
  ANIMATION_CAST_PSYNERGY = 17,
  /// Sitting waist-deep in water and splashing, as in a Lamakan Desert oasis.
  ANIMATION_LEADER_BATHE = 18,
  /// Lying collapsed on the ground; every party member's sheet maps this
  /// number to such a pose (spr_000.png to spr_003.png).
  ANIMATION_COLLAPSE = 19,
  ANIMATION_LEADER_LEAN_BACK = 22, ///< leaning back, his head thrown back
  /// The arrivals and leavings of the animated warps (enum WarpStyle), each
  /// four frames of the leader turning: an arrival grows from a small figure
  /// seen side-on to full size facing the player, a leaving turns from his
  /// back to side-on as he shrinks and is then hidden. The alternate pair has
  /// its own drawings of the same turns.
  ///@{
  ANIMATION_LEADER_ARRIVE_ALTERNATE = 23,
  ANIMATION_LEADER_ARRIVE = 24,
  ANIMATION_LEADER_LEAVE_ALTERNATE = 25,
  ANIMATION_LEADER_LEAVE = 26,
  ///@}
  /// Spinning round once, then drawn blank (ANIMCMD_BLANK).
  ANIMATION_LEADER_SPIN_AWAY = 27,
  ANIMATION_LEADER_ARMS_RAISED = 28, ///< both arms raised over his head
  /// Held in ANIMATION_LEADER_SPRING's crouch, knees bent, as after a lost
  /// match.
  ANIMATION_LEADER_CROUCH = 35
};

// Speaker bits, above the local id in the low 12 bits of a message command's
// speaker, as FieldMessage_ShowAtObject reads them.
#define SPEAKER_PORTRAIT_LEFT 0x1000  ///< the portrait left of the speaker
#define SPEAKER_PORTRAIT_RIGHT 0x2000 ///< the portrait right of the speaker
#define SPEAKER_WINDOW_ABOVE 0x4000   ///< the message window above the speaker
#define SPEAKER_WINDOW_BELOW 0x8000   ///< the message window never above it

/// One step of a cell animation: copy the width by height cells at sourceX,
/// sourceY over the destination, then wait frames.
#define MAPGRID_ANIMATION_STEP(sourceX, sourceY, width, height, frames)        \
  (sourceX), (sourceY), (width), (height), (frames)
#define MAPGRID_ANIMATION_STEPS_END                                            \
  0xffff ///< a source column that ends the steps

/// A cell animation, such as a door opening: its steps and the cell they are
/// copied to (MapGrid_PlayMetatileCopySequence, which blocks until the last).
struct MapGridCellAnimation {
  /// MAPGRID_ANIMATION_STEP records, ended by MAPGRID_ANIMATION_STEPS_END, for
  /// MapGrid_PlayMetatileCopySequence.
  const u16 *steps;
  u16 x; ///< destination cell column
  u16 y; ///< destination cell row
};

// Imports of main-image routines. ScriptRequestWarp, ObjectEventSetPosition,
// ObjectEventSetAnimation and RandomNext have differing argument counts at
// their call sites, as measured in the bytes, so retain old-style declarations.
/// Opens a script (FieldScript_Start): closes any timed notice, stops the
/// leader and makes it the speaker.
void Engine_ScriptStart(void);
/// Closes a script (FieldScript_End): the camera follows the leader again and
/// FLAG_LURE_CAP is refreshed.
void Engine_ScriptEnd(void);
/// Waits the given frames unless debug fast-forward is on
/// (FieldScript_WaitFrames).
void Engine_ScriptWait(s32 frames);
/// Runs the frame loop for the given frames (WaitFrames); debug fast-forward
/// doesn't shorten it.
void Engine_TaskWait(s32 frames);
/// Sets the number of the script's next message in the field work
/// (FieldScript_SetMessageId).
void Engine_ScriptSetMessage(s32 message);
/// Opens the current message at speaker and waits until it has been read and
/// its window has closed (FieldScript_ShowMessageAndWait). The speaker may
/// carry SPEAKER_ bits.
void Engine_ScriptShowMessage(s32 speaker, s32 flags);
/// Opens the current message beside speaker, with its portrait, and moves the
/// script on to the next (FieldMessage_ShowAtObject). Returns the message
/// window once its text is typed out, without waiting for it to be read.
s32 Engine_ScriptOpenMessage(s32 speaker, s32 flags);
/// Asks the current message at speaker, then shows the message after it for
/// yes or the one after that for no (FieldScript_AskYesNoAndShowResponse).
/// Returns 0 for yes, 1 for no; either way the script goes on past both.
s32 Engine_ScriptAskYesNo(s32 speaker, s32 flags);
/// Offers yes or no once the message on screen is read, the speaker nodding at
/// yes and shaking its head at no
/// (FieldScript_AskYesNoWithObjectEventAnimation). Returns 0 for yes, 1 for no;
/// flags puts the choice below the message.
s32 Engine_ScriptChooseYesNo(s32 localId, s32 flags);
/// Shows message in a window centred on the screen, waits until it is typed out
/// and closes it (FieldMessage_Show). The bits of flags zoom
/// the window, lower it or leave it open.
void Engine_MessageShowCentered(s32 message, s32 flags);
/// Sets the field's warp request (FieldScript_SetWarpRequest); 999 leaves for
/// gGameState's map and warp destination.
void Engine_ScriptRequestWarp();
/// Starts the script's screen transition (FieldScript_StartScreenTransition).
void Engine_ScriptOpenScreen(void);
/// Finishes the script's screen transition
/// (FieldScript_FinishScreenTransition).
void Engine_ScriptCloseScreen(void);
/// Waits as long as the script's screen transition runs
/// (FieldScript_WaitTransitionFrames).
void Engine_ScriptWaitForScreen(void);
/// Fades the game's layers to black (ScreenFade_ToBlack). The argument is the
/// fade's length in frames: the cleared-game menu passes 0 to black the screen
/// at once.
void Engine_BlendSetDarkenTarget16(s32 target);
/// Returns the ROM address of a resource, from the resource directory
/// (Resource_GetTableEntry).
u8 *Engine_ResourceGetTableEntry(s32 resource);
/// Puts an object event at rest at (x, z), in 16.16 pixels; one that follows
/// the ground keeps its height above the terrain
/// (ObjectMotion_SetHorizontalPositionWithTerrain).
void Engine_ObjectEventSetPosition();
/// Sets an object event's top speed and acceleration, in 16.16 pixels per frame
/// (ObjectMotion_SetSpeedParameters).
void Engine_ObjectEventSetSpeed(s32 localId, s32 speed, s32 acceleration);
/// Sends an object event toward (x, z), in pixels, in whatever animation it has
/// (ObjectMotion_ResetAndSetPosition).
void Engine_ObjectEventSetDestination(s32 localId, s32 x, s32 z);
/// Sends an object event (dx, dz) pixels from where it stands
/// (ObjectMotion_OffsetPositionAndResetMotion).
void Engine_ObjectEventSetDestinationOffset(s32 localId, s32 dx, s32 dz);
/// Walks an object event toward (x, z), in pixels, in ANIMATION_WALK
/// (ObjectMotion_ResetAndSetPositionInMode2).
void Engine_ObjectEventWalkTo(s32 localId, s32 x, s32 z);
/// Walks an object event (dx, dz) pixels from where it stands, in
/// ANIMATION_WALK (ObjectMotion_OffsetPositionAndReset).
void Engine_ObjectEventWalkBy(s32 localId, s32 dx, s32 dz);
/// Waits for an object event to arrive, 600 frames at most, and stands it still
/// (ObjectMotion_CommitCurrentPositionAndActivate).
void Engine_ObjectEventWaitForMove(s32 localId);
/// Turns an object event toward facing, an angle where 0x10000 is a full turn,
/// then waits frames (ObjectMotion_ArmCallback).
void Engine_ObjectEventFaceDirection(s32 localId, s32 facing, s32 frames);
/// Turns object event at once to face target, then waits frames
/// (ObjectMotion_SetAngleToward).
void Engine_ObjectEventFaceObjectEvent(s32 localId, s32 target, s32 frames);
/// Turns two object events toward each other, then waits frames
/// (ObjectEvent_LinkPair).
void Engine_ObjectEventFaceEachOther(s32 localId, s32 other, s32 frames);
/// Switches an object event's sprite to an ObjectEventAnimation
/// (ObjectEvent_SetModeById).
void Engine_ObjectEventSetAnimation();
/// Plays an ObjectEventAnimation and waits for it to end
/// (ObjectMotion_SetAnimationAndWait).
void Engine_ObjectEventSetAnimationAndWait(s32 localId, s32 animation);
/// Starts an object event squashing and springing back repeats times, 1 to 3
/// (ObjectMotion_SetVariantCallback).
void Engine_ObjectEventStartRepeatedMotion(s32 localId, s32 repeats);
/// The same squash and spring as Engine_ObjectEventStartRepeatedMotion, waiting
/// for it to end (ObjectMotion_SetVarCbAndRefresh).
void Engine_ObjectEventRunRepeatedMotion(s32 localId, s32 repeats);
/// Shows an emote bubble over an object event, which frees itself after 46
/// frames, then waits frames (FieldEffect_SpawnLinked). See EMOTE_IN_FRONT.
void Engine_ObjectEventShowEmote(s32 localId, s32 emote, s32 frames);
/// Attaches, removes or plays once the effect object that rides an object
/// event, by enum AttachedEffectFlag (FieldEffect_ConfigureLinkedObject).
void Engine_ObjectEventSetAttachedEffect(s32 localId, s32 effect);
/// Fixes an object event's sprite priority, which then no longer follows its
/// depth (ObjectMotion_SetActionVariant).
void Engine_ObjectEventSetSpritePriority(s32 localId, s32 priority);
/// Sets the flags of a one-sprite object's sprite, bit 0 drawing its shadow
/// (ObjectDispatch_SetSingleChildFlags).
void Engine_ObjectEventSetSpriteFlags(struct ObjectEvent *objectEvent,
                                      s32 flags);
/// Makes the camera follow object event (FieldCamera_FollowObjectEvent). Unless
/// keep_position is set it jumps there at once and, off the world map, the map
/// is redrawn.
void Engine_CameraFollowObjectEvent(s32 localId, s32 keep_position);
/// Sets the camera's top speed and acceleration, in 16.16 pixels per frame
/// (FieldCamera_SetSpeedParameters).
void Engine_CameraSetSpeed(s32 speed, s32 acceleration);
/// Sends the camera to a point in 16.16 pixels, held far enough inside the map
/// that the screen never shows past its edge (FieldCamera_MoveWithinMapBounds).
/// -1 keeps an axis; pan 0 jumps there at once.
void Engine_CameraMoveTo(s32 fixed_x, s32 fixed_y, s32 fixed_z, s32 pan);
/// Sends the camera to an object event's x and z, inside the map's edges
/// (ObjectEvent_PlaceCurrentWithinCameraBounds).
void Engine_CameraMoveToObjectEvent(s32 localId, s32 pan);
/// Waits, 600 frames at most, for the camera to arrive, then two frames more
/// (FieldCamera_WaitForArrival).
void Engine_CameraWaitForMove(void);
/// Copies a rectangle of cells whole, metatile and attributes, from (sourceX,
/// sourceY) to (destinationX, destinationY), and redraws those on screen
/// (MapGrid_CopyMetatileCellsRect).
void Engine_MapGridCopyCells(s32 sourceX, s32 sourceY, s32 width, s32 height,
                             s32 destinationX, s32 destinationY);
/// Copies only the attributes of a rectangle of cells (flags, metatile behavior
/// and shape), keeping each destination's metatile, so nothing on screen
/// changes (MapGrid_CopyCellAttributeRect).
void Engine_MapGridCopyCellAttributes(s32 sourceX, s32 sourceY, s32 width,
                                      s32 height, s32 destinationX,
                                      s32 destinationY);
/// Centres the camera on the point the map follows and scrolls the layers there
/// (MapGrid_CenterOnFollowedPoint).
void Engine_MapGridRedraw(void);
/// Sets the screen shake's amplitudes and decay, in 16.16, leaving alone the
/// part whose argument is negative (MapGrid_SetScreenShake).
void Engine_WorkSetValuesIfNonNegative(s32 first, s32 second, s32 third);
/// Returns 1 when a game flag is set (GameFlag_Test).
s32 Engine_GameFlagIsSet(s32 flag);
/// Sets a game flag (GameFlag_SetBit).
void Engine_GameFlagSet(s32 flag);
/// Clears a game flag (GameFlag_ClearBit).
void Engine_GameFlagClear(s32 flag);
/// Plays a sound effect, fanfare or music cue, or a fade control
/// (Sound_PlaySong); the low 12 bits are the cue.
void Engine_AudioPlayCue(s32 cue);
/// Runs a shop's counter until the player leaves (Shop_Run): shop is the row of
/// its stock table and keeper the object whose portrait the messages show.
void Engine_ShopOpen(s32 shop, s32 keeper);
/// Runs an inn's counter (Inn_CheckIn): inn is the row of the price table and
/// keeper the innkeeper's object.
void Engine_InnOpen(s32 inn, s32 keeper);
/// Runs a sanctum's welcome, services and farewell (Sanctum_Run); priest
/// is the keeper's object.
void Engine_SanctumOpen(s32 priest);
/// Unschedules the task running callback (Task_Remove). Returns
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
/// none (AllocHeapSlot). Returns NULL when no pool has room.
void *Engine_HeapAllocate(s32 slot, s32 size);
/// Frees heap slot's block, and with it every later block of the same pool
/// (FreeHeapSlot).
void Engine_HeapRelease(s32 slot);
/// Gives cache entry block size bytes of sprite VRAM and copies data there
/// (VramCache_Load). Returns the first sprite tile number, or 0 when there was
/// no room.
s32 Engine_VramLoad(s32 block, s32 size, const void *data);
/// Creates a field object at (x, y, z) in 16.16 pixels, or returns NULL when
/// none is free (Object_CreateField). type / 4096 is its enum ObjectCreateKind
/// and the low 12 bits the sprite's animation id.
struct ObjectEvent *Engine_ObjectCreate(s32 type, s32 fixed_x, s32 fixed_y,
                                        s32 fixed_z);
/// Switches every sprite of an object to an animation, the map's import name
/// for ObjectDispatch_SelectAnimation.
void ObjectEvent_SetMode(struct ObjectEvent *objectEvent, s32 animation);
/// Gives an object a sprite script and starts it from the top
/// (ObjectDispatch_Initialize).
void Engine_ObjectSetScript(struct ObjectEvent *objectEvent, const s32 *script);
/// Sets a one-sprite object's OAM blend mode, an ObjectBlendMode
/// (ObjectDispatch_SetChildBlendMode).
void Engine_ObjectSetBlendMode(struct ObjectEvent *objectEvent, s32 mode);
/// Sets the colour mode (SpriteColorMode) of a one-sprite object's layers,
/// except those drawn hidden (Animation_ApplyChildValues).
void Engine_ObjectSetPartPalettes(struct ObjectEvent *objectEvent, s32 palette);
/// Sends an object toward a point in 16.16 pixels, coasting to a stop there
/// (Object_SetMoveTarget).
void Engine_ObjectSetPosition(struct ObjectEvent *objectEvent, s32 fixed_x,
                              s32 fixed_y, s32 fixed_z);
/// Plays a cell animation's MAPGRID_ANIMATION_STEP records onto the cell at
/// (destinationX, destinationY), returning only after the last
/// (MapGrid_PlayMetatileCopySequence).
void Engine_MapGridAnimateCells(const u16 *steps, s32 destinationX,
                                s32 destinationY);
/// Moves the object event of a marker to (x, z) on the ground, in 16.16 pixels;
/// -1 keeps the centre of its cell on that axis
/// (TreasureSpot_SetObjectEventPosition).
void Engine_MarkerSetPosition(s32 object, s32 fixed_x, s32 fixed_z);
/// Draws an item's icon into the glyph work buffer, with no frame or marks
/// (Ui_PrepareTransferForItem).
void Engine_ItemLoadIcon(s32 item);
/// Shows an item's picture above the leader for 80 frames
/// (FieldEffect_StartEffectObject22). Bit 0 of height adds sparkles and bit 1
/// raises it as a found item.
void Engine_ItemShowFound(s32 item, s32 height);
/// Gives an item to the first party member with room and says so, letting the
/// player drop something when every bag is full (PartyInventory_GiveItem).
/// Returns the member who got it, or -1 when it was left behind.
s32 Engine_PartyGiveItem(s32 item, s32 flags);
/// Stops an object event's action and puts it back under the script's control,
/// on the idle script (ObjectMotion_EnableActionAndResetMotion).
void Engine_ObjectEventStop(s32 localId);
/// Walks an object event to the middle of its 16-pixel column, fixes its sprite
/// priority, then sends it dz pixels along z (ObjectMotion_CenterOnColumn).
void Engine_ObjectEventCenterAndWalk(s32 localId, s32 priority, s32 dz);
/// Walks an object event (dx, dz) pixels, waits for it to arrive and stands it
/// still (ObjectMotion_CommitPositionAndActivate).
void Engine_ObjectEventWalkByAndWait(s32 localId, s32 dx, s32 dz);
/// Makes the leader jump two tiles forward where the map has a jump spot
/// (Field_TryJumpForward). Returns 0 after a jump and -1 when it can't jump.
s32 Engine_LeaderCheckAhead(void);
/// Prepares the work and spark tiles of a field Psynergy effect for ability
/// (FieldEffect_LoadActionEffectResources).
void Engine_PsynergyStart(s32 ability, s32 flags);
/// Records an effect's caster and target; with no target the effect plays 16
/// pixels in front of the caster (FieldEffect_SetupObjectPair).
void Engine_PsynergySetTarget(s32 caster, s32 target);
/// Puts the caster in its casting pose and, unless the effect is silent or
/// quiet, flashes it and shows the ability's name (FieldEffect_StartCast).
void Engine_PsynergyRaiseHands(void);
/// Runs one step of the current field effect, an enum PsynergyEffectStep
/// (Field_DispatchTypeHandler).
void Engine_PsynergyPlayEffect(s32 effect);
/// Stops the caster's per-frame hook, restores its colours and waits a frame
/// (FieldEffect_StopCasterHook).
void Engine_PsynergyLowerHands(void);
/// Stops the wave effect: the caster's colours and the palettes come back and
/// its work is released (FieldEffect_FinishWaveAndReleaseHeapBlock).
void Engine_PsynergyCancel(void);
/// Shows the current message as Engine_ScriptShowMessage does, then waits
/// frames (FieldScript_ShowMessageThenWait).
void Engine_ScriptShowMessageAndWait(s32 speaker, s32 flags, s32 frames);
/// Returns the camera object a script moves (FieldCamera_GetScriptObject).
struct ObjectEvent *Engine_ScriptGetViewCenter(void);
/// Returns the object event in a slot, or NULL (ObjectEvent_GetByLocalId).
struct ObjectEvent *Engine_ObjectEventLookup(s32 localId);
/// Releases an object event and empties its place in the object event table
/// (ObjectEvent_DestroyById).
void Engine_ObjectEventDestroy(s32 localId);
/// Gives every sprite part of an object event a sprite param; with bit 0x100
/// the object event blinks instead (ObjectGroup_ConfigureChildValue).
void Engine_ObjectEventSetChildValue(s32 localId, s32 value);
/// Starts an object event's action, an enum MovementType or a script's address,
/// and makes it face where it moves (ObjectMotion_EnableActionAndSetCallback).
void Engine_ObjectEventEnableActionCallback(s32 localId, s32 action);
/// The engine encodes either a built-in action or an action-script address.
static inline void ObjectEvent_SetActionScript(s32 localId, const s32 *script) {
  Engine_ObjectEventEnableActionCallback(localId, (s32)script);
}

/// Runs a script, waiting at most 300 frames for its final command.
static inline void ObjectEvent_RunActionScriptAndWait(s32 localId,
                                                      const s32 *script) {
  ObjectEvent_SetActionCallbackAndRefreshById(localId, (s32)script);
}

/// Starts one of the engine's own actions on an object event, an enum
/// MovementType, where ObjectEvent_SetActionScript gives it a map's script.
static inline void ObjectEvent_SetBuiltinAction(s32 localId, s32 action) {
  Engine_ObjectEventEnableActionCallback(localId, action);
}

/// Starts an object event's action, an enum MovementType or a script to run,
/// leaving its facing flag alone (ObjectEvent_SetMovementType).
void Engine_ObjectEventSetActionCallback(struct ObjectEvent *objectEvent,
                                         s32 value);
/// Puts back the object events ObjectEvent_Snapshot saved and moves the camera
/// to the leader's height (ObjectEvent_Restore).
void Engine_ObjectEventsRefresh(void);
/// Copies the metatile numbers of a rectangle of cells, keeping each
/// destination's attributes, and redraws the copied cells on screen
/// (MapGrid_CopyMetatileIndicesRect). It takes the destination before the size.
void Engine_MapGridCopyCellsTo(s32 sourceX, s32 sourceY, s32 destinationX,
                               s32 destinationY, s32 width, s32 height);
/// Copies the metatile numbers of a rectangle of cells, keeping each
/// destination's attributes (MapGrid_CopyCellIndexRect). XXX: the routine and
/// its callers (lalivero.c) pass the size before the destination, as the
/// parameter names here do not.
void Engine_MapGridCopyCellsLayered(s32 sourceX, s32 sourceY, s32 destinationX,
                                    s32 destinationY, s32 width, s32 height);
/// Sets the colours the colour buffer shows now (PaletteFade_SetSource): value
/// is one colour, a PALETTE_TINT_ value or a ready buffer, and mode picks
/// all palettes (0), the BG's (1) or the OBJ's (2).
void Engine_ColorBufferApplySource(s32 value, s32 mode);
/// Sets the colours the colour buffer moves toward, value and mode as for
/// Engine_ColorBufferApplySource (PaletteFade_SetTarget).
void Engine_ColorBufferApplyTarget(s32 value, s32 mode);
/// Starts moving the shown colours to the target colours over frames
/// (PaletteFade_StartInterpolation).
void Engine_ColorBufferInterpolate(s32 frames);
/// Sends an object event toward (x, z) as Engine_ObjectEventSetDestination
/// does, then waits for it, giving up after 600 frames
/// (ObjectMotion_SetPositionAndCommit).
void Engine_ObjectEventMoveToAndWait(s32 localId, s32 x, s32 z);
/// Walks an object event to (x, z), in pixels, waits for it to arrive and
/// stands it in ANIMATION_STAND (ObjectMotion_SetPositionAndReset).
void Engine_ObjectEventWalkToAndWait(s32 localId, s32 x, s32 z);
/// Throws an object event upward at height pixels per frame with a jump sound,
/// then waits frames while gravity brings it down (ObjectMotion_Launch).
void Engine_ObjectEventJump(s32 localId, s32 height, s32 frames);
/// Two object events speak at once, each message and portrait at its own tile
/// position, until A, B, L or R closes both
/// (FieldScript_ShowTwoMessagesAndWait). The routine takes ten arguments and
/// never reads flags.
void Engine_ScriptShowTwoMessagesAndWait(s32 speaker, s32 x, s32 y, s32 arg,
                                         s32 extra, s32 other_speaker,
                                         s32 otherX, s32 other_y, s32 other_arg,
                                         s32 other_extra, s32 flags);
/// Adds two units to the party and announces them together
/// (FieldScript_PrepareTwoObjectsAndApply); Vale's map calls it twice.
void Engine_PartyAddMembers(s32 first, s32 second);
/// Waits until both shake amplitudes are below 1/256 pixel, 300 frames at
/// most, then stops the shake (MapGrid_WaitForShakeToSettle).
void Engine_MapGridRenderWaitForValues(void);

/// The same move and wait as Engine_ObjectEventMoveToAndWait, under a second
/// import name (ObjectMotion_SetPositionAndCommit).
void Engine_ObjectMotionSetPositionAndCommit(s32 localId, s32 x, s32 z);
/// Releases an object's sprites and zeroes it, so it can be handed out again
/// (ObjectDispatch_Release).
void Engine_ObjectDispatchRelease(struct ObjectEvent *objectEvent);
/// The shake wait of Engine_MapGridRenderWaitForValues under the import name
/// five maps call it by (MapGrid_WaitForShakeToSettle).
void Engine_MapGridWaitForShakeToSettle(void);
/// Shows a found item's picture object; bit 0 of mode adds sparkles and bit 1
/// raises it above the leader (ItemDisplay_ShowPicture).
void Engine_ItemShowPicture(struct ObjectEvent *objectEvent, s32 mode);

#ifdef FIELD_STAGED_OBJ_EVENT_IMPORTS
/// Declared for the maps that define FIELD_STAGED_OBJ_EVENT_IMPORTS in place
/// of Script_PlaySong. Nothing calls it and no import resolves it: those maps
/// call Engine_AudioPlayCue.
void Audio_PlayCue(s32 cue);
#endif

/// Moves the script's message number count messages on, past lines the script
/// doesn't show.
static inline void Script_SkipMessages(s32 count) {
  u16 *message = &gFieldWork->message;

  *message = *message + count;
}

/// Shows the script's next line of dialogue and waits for the player to read
/// it.
static inline void Script_ShowMessage(s32 speaker, s32 flags) {
  Engine_ScriptShowMessage(speaker, flags);
}

/// Opens the script's next line and returns its window once the text is typed
/// out, so the script can act while it is still on screen.
static inline s32 Script_OpenMessage(s32 speaker, s32 flags) {
  return Engine_ScriptOpenMessage(speaker, flags);
}

/// Puts the current line to the player as a question and shows the line that
/// answers it; returns 0 for yes.
static inline s32 Script_AskYesNo(s32 speaker, s32 flags) {
  return Engine_ScriptAskYesNo(speaker, flags);
}

/// Looks up an object event for a script, NULL when it doesn't exist
/// (ObjectEvent_GetById).
static inline struct ObjectEvent *ObjectEvent_Get(s32 localId) {
  return ObjectEvent_GetById(localId);
}

/// Places an object event at a point at once, without walking it there.
static inline void ObjectEvent_SetPosition(s32 localId, s32 fixed_x,
                                           s32 fixed_z) {
  Engine_ObjectEventSetPosition(localId, fixed_x, fixed_z);
}

/// Sets how fast an object event moves, and how quickly it gets up to speed,
/// for the script's next moves.
static inline void ObjectEvent_SetSpeed(s32 localId, s32 speed,
                                        s32 acceleration) {
  Engine_ObjectEventSetSpeed(localId, speed, acceleration);
}

/// Moves an object event to a point in the pose it has, without walking; the
/// script carries on meanwhile.
static inline void ObjectEvent_SetDestination(s32 localId, s32 x, s32 z) {
  Engine_ObjectEventSetDestination(localId, x, z);
}

#ifndef FIELD_STAGED_OBJ_EVENT_IMPORTS
/// Moves an object event some distance from where it stands, without waiting
/// for it.
static inline void ObjectEvent_SetDestinationOffset(s32 localId, s32 dx,
                                                    s32 dz) {
  Engine_ObjectEventSetDestinationOffset(localId, dx, dz);
}
#endif

/// Walks an object event to a point; the script goes on while it walks.
static inline void ObjectEvent_WalkTo(s32 localId, s32 x, s32 z) {
  Engine_ObjectEventWalkTo(localId, x, z);
}

/// Walks an object event some distance from where it stands, without waiting
/// for it to get there.
static inline void ObjectEvent_WalkBy(s32 localId, s32 dx, s32 dz) {
  Engine_ObjectEventWalkBy(localId, dx, dz);
}

/// Turns an object event to a direction and holds the script for a few frames.
static inline void ObjectEvent_FaceDirection(s32 localId, s32 facing,
                                             s32 frames) {
  Engine_ObjectEventFaceDirection(localId, facing, frames);
}

/// Has one object event look at another at once, then pauses the script.
static inline void ObjectEvent_FaceObjectEvent(s32 localId, s32 target,
                                               s32 frames) {
  Engine_ObjectEventFaceObjectEvent(localId, target, frames);
}

/// Pops an emote bubble up over an object event, then lets frames pass before
/// the script goes on.
static inline void ObjectEvent_ShowEmote(s32 localId, s32 emote, s32 frames) {
  Engine_ObjectEventShowEmote(localId, emote, frames);
}

/// Attaches an effect object to an object event, takes it off, or plays one
/// over it once.
static inline void ObjectEvent_SetAttachedEffect(s32 localId, s32 effect) {
  Engine_ObjectEventSetAttachedEffect(localId, effect);
}

/// Sets how fast the camera pans for the script's next camera moves.
static inline void FieldCamera_SetSpeed(s32 speed, s32 acceleration) {
  Engine_CameraSetSpeed(speed, acceleration);
}

/// Pans the camera to a point in the room, or cuts there when pan is 0.
static inline void FieldCamera_MoveTo(s32 fixed_x, s32 fixed_y, s32 fixed_z,
                                      s32 pan) {
  Engine_CameraMoveTo(fixed_x, fixed_y, fixed_z, pan);
}

/// Changes part of the room in place, copying whole cells, picture and
/// metatile behavior alike, from another part of the map.
static inline void MapGrid_CopyCells(s32 sourceX, s32 sourceY, s32 width,
                                     s32 height, s32 destinationX,
                                     s32 destinationY) {
  Engine_MapGridCopyCells(sourceX, sourceY, width, height, destinationX,
                          destinationY);
}

/// Changes where the party can walk in part of the room, copying only the
/// cells' metatile behaviors and flags.
static inline void MapGrid_CopyCellAttributes(s32 sourceX, s32 sourceY,
                                              s32 width, s32 height,
                                              s32 destinationX,
                                              s32 destinationY) {
  Engine_MapGridCopyCellAttributes(sourceX, sourceY, width, height,
                                   destinationX, destinationY);
}

/// Sets shake amplitudes in fixed-point pixels and the per-frame decay factor.
/// A negative argument keeps that component unchanged; Q16_ONE keeps the shake.
static inline void MapGrid_SetShake(s32 horizontal, s32 vertical, s32 decay) {
  Engine_WorkSetValuesIfNonNegative(horizontal, vertical, decay);
}

/// A map's GameFlag_Set is FieldScript_GameFlagSet, which keeps it apart from
/// the main image's declaration in game_flags.h.
#define GameFlag_Set FieldScript_GameFlagSet
/// FieldScript_GameFlagIsSet, apart from field.h's GameFlag_IsSet.
#define GameFlag_IsSet FieldScript_GameFlagIsSet
/// FieldScript_GameFlagClear, apart from game_flags.h's GameFlag_Clear.
#define GameFlag_Clear FieldScript_GameFlagClear
/// Lets a script branch on what the player has done: 1 when the FLAG_ is set.
static inline s32 FieldScript_GameFlagIsSet(s32 flag) {
  return Engine_GameFlagIsSet(flag);
}

/// Records something for later scripts to test, setting its FLAG_.
static inline void FieldScript_GameFlagSet(s32 flag) {
  Engine_GameFlagSet(flag);
}

/// Clears a FLAG_ a map tracks, such as a FLAG_MAP_TEMP_ one.
static inline void FieldScript_GameFlagClear(s32 flag) {
  Engine_GameFlagClear(flag);
}

#ifndef FIELD_STAGED_OBJ_EVENT_IMPORTS
/// Plays a sound effect or music from a script: the SE command of script.h.
static inline void Script_PlaySong(s32 cue) { Engine_AudioPlayCue(cue); }
#endif

/// A random number from 0 to 0xffff for a script.
static inline s32 Random_Next(void) { return Engine_RandomNext(); }

/// Signed division written as a call; the `/` compiles to __divsi3.
static inline s32 Math_Divide(s32 dividend, s32 divisor) {
  return dividend / divisor;
}

/// Creates a free object in the room, outside the object event table, or
/// returns NULL when every object is taken.
static inline struct ObjectEvent *ObjectEvent_Create(s32 type, s32 fixed_x,
                                                     s32 fixed_y, s32 fixed_z) {
  return Engine_ObjectCreate(type, fixed_x, fixed_y, fixed_z);
}

/// Starts an object on a sprite script from its first command.
static inline void ObjectEvent_SetScript(struct ObjectEvent *objectEvent,
                                         const s32 *script) {
  Engine_ObjectSetScript(objectEvent, script);
}

/// OAM blend modes for Engine_ObjectSetBlendMode: OBJECT_BLEND_TRANSLUCENT
/// alpha-blends the sprite with what lies behind it.
enum ObjectBlendMode { OBJECT_BLEND_NORMAL = 0, OBJECT_BLEND_TRANSLUCENT = 1 };

/// Plays a cell animation, such as a door opening, and holds the script until
/// its last step.
static inline void MapGrid_AnimateCells(const u16 *steps, s32 destinationX,
                                        s32 destinationY) {
  Engine_MapGridAnimateCells(steps, destinationX, destinationY);
}

/// Moves a marker's object event to a point on the ground.
static inline void TreasureSpot_SetPosition(s32 object, s32 fixed_x,
                                            s32 fixed_z) {
  Engine_MarkerSetPosition(object, fixed_x, fixed_z);
}

/// Lines an object event up with the middle of its column, then walks it
/// straight along z.
static inline void ObjectEvent_CenterAndWalk(s32 localId, s32 priority,
                                             s32 dz) {
  Engine_ObjectEventCenterAndWalk(localId, priority, dz);
}

/// Walks an object event some distance and waits until it is there.
static inline void ObjectEvent_WalkByAndWait(s32 localId, s32 dx, s32 dz) {
  Engine_ObjectEventWalkByAndWait(localId, dx, dz);
}

/// Shows a line of dialogue, then pauses before the script goes on.
static inline void Script_ShowMessageAndWait(s32 speaker, s32 flags,
                                             s32 frames) {
  Engine_ScriptShowMessageAndWait(speaker, flags, frames);
}

/// Sets a sprite param on every part of an object event's sprite, or makes it
/// blink.
static inline void ObjectEvent_SetChildValue(s32 localId, s32 value) {
  Engine_ObjectEventSetChildValue(localId, value);
}

/// Not used in TBS.
#define ACTION_TABLE_STOP ((const u8 *)1)

/// Changes what part of the room shows, copying the cells' pictures but not
/// their metatile behaviors.
static inline void MapGrid_CopyCellsTo(s32 sourceX, s32 sourceY,
                                       s32 destinationX, s32 destinationY,
                                       s32 width, s32 height) {
  Engine_MapGridCopyCellsTo(sourceX, sourceY, destinationX, destinationY, width,
                            height);
}

/// Changes the pictures of part of the room as MapGrid_CopyCellsTo does,
/// through MapGrid_CopyCellIndexRect, which takes the size first.
static inline void MapGrid_CopyCellsLayered(s32 sourceX, s32 sourceY,
                                            s32 destinationX, s32 destinationY,
                                            s32 width, s32 height) {
  Engine_MapGridCopyCellsLayered(sourceX, sourceY, destinationX, destinationY,
                                 width, height);
}

/// Sets the colours the screen shows now, where a script's colour fade starts.
static inline void PaletteFade_ApplySource(s32 value, s32 mode) {
  Engine_ColorBufferApplySource(value, mode);
}

/// Sets the colours a script's colour fade ends on.
static inline void PaletteFade_ApplyTarget(s32 value, s32 mode) {
  Engine_ColorBufferApplyTarget(value, mode);
}

/// Moves an object event to a point in the pose it has and waits for it.
static inline void ObjectEvent_MoveToAndWait(s32 localId, s32 x, s32 z) {
  Engine_ObjectEventMoveToAndWait(localId, x, z);
}

/// Walks an object event to a point and waits for it, then stands it still.
static inline void ObjectEvent_WalkToAndWait(s32 localId, s32 x, s32 z) {
  Engine_ObjectEventWalkToAndWait(localId, x, z);
}

/// Has two object events speak at once, each window placed by the script.
static inline void Script_ShowTwoMessagesAndWait(s32 speaker, s32 x, s32 y,
                                                 s32 arg, s32 extra,
                                                 s32 otherSpeaker, s32 otherX,
                                                 s32 otherY, s32 otherArg,
                                                 s32 otherExtra, s32 flags) {
  Engine_ScriptShowTwoMessagesAndWait(speaker, x, y, arg, extra, otherSpeaker,
                                      otherX, otherY, otherArg, otherExtra,
                                      flags);
}

#endif

/// @file
/// The ship at Alhafra's eastern harbour. The party boards as the mayor of
/// Madra arrives (EasternAlhafraShip_MayorArrives), and Briggs is caught after
/// the battle (EasternAlhafraShip_BriggsIsCaught). Later the baby Eoleo lifts
/// things with Psynergy (EasternAlhafraShip_EoleoLifts), Tremor shakes Large
/// Bread down from a crate (EasternAlhafraShip_ShakeDownTheBread) and Move
/// pushes a crate across the deck.

#include "shared/tables.h"
#include "battle/battle.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "types.h"
#include "item_ids.h"
#include "flags.h"
#include "field/scene_tables.h"
#include "field/actor.h"
#include "system/main_routine_work.h"
#include "system/ram_buffer.h"
#include "field/field_sequence_event.h"
#include "scene/scene.h"
#include "field/event_commands.h"
#include "field/event_inline.h"
#include "field/map_grid.h"
#include "scene_ids.h"
#include "text/msg_ids.h"
#include "field/actor_script.h"
#include "fixed_math.h"

/// The crate pushed onto cell (24, 40), out of the way.
#define FLAG_CRATE_MOVED 0x900
/// Tremor has shaken the Large Bread down from where it sat.
#define FLAG_BREAD_SHAKEN_DOWN 0x901
/// The Large Bread has been picked up.
#define FLAG_BREAD_TAKEN 0x902
/// Set the first time the baby Eoleo lifts it with Psynergy; his mother
/// remarks on it only then.
#define FLAG_EOLEO_LIFTED 0x8ae

/// Where the party comes back on deck after the battle with Briggs's crew,
/// as The Broken Seal's Altin Peak names its own.
enum { ENTRANCE_AFTER_BATTLE = 99 };

/// The people and things aboard, numbering the rows of
/// gEasternAlhafraShip_ActorPlacements whose sprites are not the party's.
enum EasternAlhafraShipActorId {
  ACTOR_BRIGGS = ACTOR_FIRST_PLACED,
  /// Briggs's mate in the cabin: "He knew we needed a ship, and he knew we
  /// went east."
  ACTOR_BRIGGS_MATE,
  ACTOR_CHAUCHA,
  ACTOR_EOLEO,
  /// The vase of flowers Eoleo lifts with Psynergy.
  ACTOR_VASE,
  /// The crate Move pushes out of the way (FLAG_CRATE_MOVED).
  ACTOR_CRATE,
  /// The stacked crates Tremor shakes the Large Bread down from.
  ACTOR_STACKED_CRATES,
  /// The Large Bread, an item icon.
  ACTOR_LARGE_BREAD,
  /// Those who come aboard after the battle, waiting off the map until then.
  ACTOR_ALHAFRAS_MAYOR,
  ACTOR_MADRAS_MAYOR,
  ACTOR_MADRAS_ELDER,
  ACTOR_MADRAN_SOLDIER,
  /// Alhafra's soldiers.
  ///@{
  ACTOR_ALHAFRAN_SOLDIER,
  ACTOR_SECOND_ALHAFRAN_SOLDIER,
  ///@}
  /// The rest of Briggs's crew, who come aboard with him after the battle.
  ///@{
  ACTOR_FIRST_PIRATE,
  ACTOR_SECOND_PIRATE,
  ACTOR_THIRD_PIRATE,
  ///@}
  ACTOR_KRADEN
};

extern struct ActorPlacement gEasternAlhafraShip_ActorPlacements[];
void EasternAlhafraShip_BriggsIsCaught(void);
extern u32 gEasternAlhafraShip_BriggsRunsScript[];
extern s32 gEasternAlhafraShip_MateRunsScript[];

void EasternAlhafraShip_ShakeDownTheBread(void);

void Event_PickUpItem();

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

s32 Scene_GetActorPlacements(void) {
  return (s32)gEasternAlhafraShip_ActorPlacements;
}

void EasternAlhafraShip_StartMovingCrate(void) {}

void EasternAlhafraShip_FinishMovingCrate(s32 unused, s32 id) {
  struct Actor *object = Actor_GetById(id);
  s32 x = FIELD_CELL(object->x.fixed);
  s32 z = FIELD_CELL(object->z.fixed);

  if (x == 24 && z == 40)
    GameFlag_Set(FLAG_CRATE_MOVED);
  if (x == 23 && z == 40)
    GameFlag_Clear(FLAG_CRATE_MOVED);
}

void EasternAlhafraShip_ShakeDownTheBread(void) {
  GameFlag_Set(FLAG_BREAD_SHAKEN_DOWN);
  Actor_GetById(gMainRoutineWork.leaderActorId)->velocityY = px(6);
  Actor_SetSpeed(gMainRoutineWork.leaderActorId, 2);
  Actor_WalkTo(gMainRoutineWork.leaderActorId, 136, 488);
  Event_Wait(10);
  Actor_SetDestination(ACTOR_LARGE_BREAD, 120, 472);
  Actor_MoveToAndWaitById(ACTOR_STACKED_CRATES, 136, 475);
  Actor_GetById(ACTOR_LARGE_BREAD)->velocityY = px(6);
  Sound_Play(SE_POUND);
  Actor_WaitForMoveById(gMainRoutineWork.leaderActorId);
  Actor_SetAnimationById(gMainRoutineWork.leaderActorId, 1);
  Actor_FaceDirectionById(gMainRoutineWork.leaderActorId, FACING_NORTH, 0);
  Event_Wait(20);
  Actor_WaitForMoveById(ACTOR_LARGE_BREAD);
  Sound_Play(SE_DOOR_CLOSE);
  return;
}

void EasternAlhafraShip_TakeTheBread(s32 id) {
  Event_PickUpItem(id, ITEM_LARGE_BREAD, FLAG_BREAD_TAKEN);
  return;
}

void EasternAlhafraShip_EoleoLifts(s32 id) {
  struct Actor *object = Actor_GetById(ACTOR_VASE);
  u8 *flags;
  s32 i;

  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgGooGooGa);
  // "Goo goo ga ga!"
  Event_ShowMessageAndWait(ACTOR_EOLEO, 0);
  Field_StartColorTransition(8, 0x1210);
  BattleEffect_StartArcEffect();
  BattleEffect_SetObjectUpdateMode((s32)Actor_GetById(id), 2);
  Sound_Play(SE_ASCEND);
  Event_Wait(30);
  flags = &object->motionFlags;
  *flags = 4;
  for (i = 39; i >= 0; i--) {
    object->y.fixed += px(1) / 5;
    Event_Wait(1);
  }
  Event_Wait(70);
  BattleEffect_SetObjectUpdateMode((s32)Actor_GetById(id), 0);
  BattleEffect_WaitForArcGroups();
  BattleEffect_EndArcEffect();
  Field_StartPaletteTransition(8);
  *flags = 3;
  object->gravity = px(1) / 2;
  Event_Wait(5);
  Sound_Play(SE_WATER);
  if (Event_GetFlag(FLAG_EOLEO_LIFTED) == 0) {
    GameFlag_Set(FLAG_EOLEO_LIFTED);
    Event_Wait(30);
    Actor_FaceDirectionById(ACTOR_CHAUCHA, FACING_SOUTH_SOUTHEAST, 0);
    Event_Wait(30);
    Actor_RunRepeatedMotion(ACTOR_CHAUCHA, 2);
    Event_Wait(10);
    Event_ShowMessageThenWait(ACTOR_CHAUCHA, 0, 10);
    Actor_FaceDirection(ACTOR_CHAUCHA, 0xafff, 0);
  }
  Event_End();
}

const struct Trigger *Scene_GetTriggers(void) { return gSceneEvents; }

s32 Scene_Initialize(void) {
  struct Actor *object;

  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
  if (gMainRoutineWork.entrance == ENTRANCE_AFTER_BATTLE) {
    GameFlag_Set(FLAG_MAYOR_ARRIVED_AT_THE_SHIP);
    GameFlag_Set(FLAG_BRIGGS_CAUGHT);
    gMainRoutineWork.returnEntrance = 3;
    gMainRoutineWork.entrance = 3;
    EasternAlhafraShip_BriggsIsCaught();
  }
  if (GameFlag_Get(FLAG_ALHAFRA_MAST_CLEARED)) {
    Actor_SetPosition(ACTOR_CHAUCHA, 0, 0);
    Actor_SetPosition(ACTOR_EOLEO, 0, 0);
    Actor_SetPosition(ACTOR_BRIGGS, 0, 0);
    Actor_SetPosition(ACTOR_BRIGGS_MATE, 0, 0);
  } else if (GameFlag_Get(FLAG_BRIGGS_CAUGHT)) {
    Actor_SetPosition(ACTOR_BRIGGS, 0, 0);
    Actor_SetPosition(ACTOR_BRIGGS_MATE, 0, 0);
    if (!GameFlag_Get(FLAG_FAMILY_VISITED_BRIGGS)) {
      Actor_SetPosition(ACTOR_CHAUCHA, 0, 0);
      Actor_SetPosition(ACTOR_EOLEO, 0, 0);
    }
  } else {
    Actor_SetPosition(ACTOR_CHAUCHA, 0, 0);
    Actor_SetPosition(ACTOR_EOLEO, 0, 0);
  }
  if (GameFlag_Get(FLAG_CRATE_MOVED))
    Actor_SetPosition(ACTOR_CRATE, 392, 648);
  Actor_GetById(ACTOR_CRATE)->priorityFlags |= ACTOR_PRIORITY_IGNORES_RISES;
  Actor_ShowItemIconById(ACTOR_LARGE_BREAD, ITEM_LARGE_BREAD);
  if (!GameFlag_Get(FLAG_BREAD_SHAKEN_DOWN)) {
    if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
      Actor_GetById(ACTOR_STACKED_CRATES)->priorityFlags |= 128;
      object = Actor_GetById(ACTOR_LARGE_BREAD);
      object->priorityFlags |= 64;
      object->y.fixed += px(16);
      object->groundHeight += px(16);
    }
  } else {
    Actor_SetPosition(ACTOR_STACKED_CRATES, 136, 475);
    if (!GameFlag_Get(FLAG_BREAD_TAKEN))
      Actor_SetPosition(ACTOR_LARGE_BREAD, 120, 472);
    else
      Actor_SetPosition(ACTOR_LARGE_BREAD, 0, 0);
  }
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

extern u32 gEasternAlhafraShip_NoteScript[];

void EasternAlhafraShip_SpawnNote(x, y, z, value, timer) s32 x, y, z;
s16 value;
s32 timer;
{
  struct Actor *object;
  struct Sprite *sprite;

  object = Import_ActorCreate(SPRITE_EMOTE, x, y, z);
  if (object != NULL) {
    sprite = object->sprite;
    Actor_SelectAnimation(object, 1);
    Actor_StartScript(object, gEasternAlhafraShip_NoteScript);
    object->motionFlags = 0;
    object->wait = timer;
    sprite->flags = 0;
    sprite->rotation = value;
  }
}

void Actor_EmitNotes(s32 id) {
  struct Actor *object = Actor_GetById(id);

  EasternAlhafraShip_SpawnNote(object->x.fixed + px(8),
                               object->y.fixed + px(28), object->z.fixed,
                               0x1000, 15);
  Sound_Play(SE_LIGHT_APPEAR);
  Event_Wait(15);
  EasternAlhafraShip_SpawnNote(object->x.fixed - px(8),
                               object->y.fixed + px(28), object->z.fixed,
                               0xf000, 15);
  Sound_Play(SE_LIGHT_APPEAR);
  Event_Wait(15);
  EasternAlhafraShip_SpawnNote(object->x.fixed + px(8),
                               object->y.fixed + px(28), object->z.fixed,
                               0x1000, 15);
  Sound_Play(SE_LIGHT_APPEAR);
  Event_Wait(15);
  EasternAlhafraShip_SpawnNote(object->x.fixed - px(8),
                               object->y.fixed + px(28), object->z.fixed,
                               0xf000, 15);
  Sound_Play(SE_LIGHT_APPEAR);
  Event_Wait(15);
}

extern u32 gEasternAlhafraShip_LeaveTheDeckScript[];

void EasternAlhafraShip_MayorArrives(void) {
  struct MapWork *map;
  s32 result;

  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgTheMayorOf);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  map = Ram_HeapSlots->mapGrid;
  map->minX = 0;
  map->maxX = px(1024);
  map->minZ = 0;
  map->maxZ = px(1024);
  // "The mayor of Madra? Here?"
  Event_ShowMessageThenWait(ACTOR_BRIGGS, 0, 10);
  Actor_WalkToAndWait(ACTOR_FELIX, 456, 408);
  FieldActor_LaunchFromLeader(ACTOR_KRADEN, 0, -16, FACING_SOUTH);
  FieldActor_LaunchFromLeader(ACTOR_JENNA, 16, 0, FACING_SOUTH);
  FieldActor_LaunchFromLeader(ACTOR_SHEBA, 16, -16, FACING_SOUTH);
  Actor_WaitForMoveById(ACTOR_SHEBA);
  Actor_FaceDirection(ACTOR_FELIX, FACING_SOUTH, 0);
  Event_Wait(20);
  Actor_FaceDirectionById(ACTOR_KRADEN, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_WEST, 0);
  Actor_FaceDirectionById(ACTOR_FELIX, FACING_EAST, 0);
  Event_Wait(40);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTHWEST, 0);
  Actor_FaceDirectionById(ACTOR_FELIX, FACING_SOUTHWEST, 0);
  Event_Wait(20);
  FieldCamera_MoveTo(376, CAMERA_KEEP, 416, 1);
  FieldCamera_WaitForArrival();
  Event_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_BRIGGS, 2);
  Event_Wait(10);
  // "You're certain?"
  Event_ShowMessageThenWait(ACTOR_BRIGGS, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_BRIGGS_MATE, 3);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_BRIGGS, FACING_NORTH_NORTHWEST, 0);
  Event_Wait(30);
  // "How did he know we came to Alhafra?"
  Event_ShowMessageThenWait(ACTOR_BRIGGS, 0, 10);
  Actor_Jump(ACTOR_BRIGGS_MATE, 6, 15);
  Actor_Jump(ACTOR_BRIGGS_MATE, 6, 23);
  // "He knew we needed a ship, and he knew we went east. It seems pretty..."
  Event_ShowMessageThenWait(ACTOR_BRIGGS_MATE, 0, 10);
  Actor_ShowEmote(ACTOR_BRIGGS, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Actor_FaceDirection(ACTOR_BRIGGS, FACING_SOUTH, 0);
  Event_Wait(20);
  // "Arr... Well, yeah... When you put it that way, it does make sense..."
  Event_ShowMessageThenWait(ACTOR_BRIGGS, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_BRIGGS_MATE, 3);
  Event_Wait(10);
  // "He was looking to find the mayor of Alhafra..."
  Event_ShowMessageThenWait(ACTOR_BRIGGS_MATE, 0, 10);
  Actor_ShowEmote(ACTOR_BRIGGS, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  // "He must be hoping the mayor will tell him where we are..."
  Event_ShowMessageThenWait(ACTOR_BRIGGS, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_BRIGGS_MATE, 2);
  Event_Wait(10);
  // "He'll probably tell the Alhafrans we're pirates, too, huh?"
  Event_ShowMessageThenWait(ACTOR_BRIGGS_MATE, 0, 10);
  Actor_FaceDirection(ACTOR_BRIGGS, FACING_NORTH_NORTHWEST, 0);
  Event_Wait(30);
  // "Pirates!? That seems a little unfair!"
  Event_ShowMessageThenWait(ACTOR_BRIGGS, 0, 10);
  Actor_ShowEmote(ACTOR_BRIGGS_MATE, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  // "What do you mean? We...are pirates, aren't we? I mean, we've taken..."
  Event_ShowMessageThenWait(ACTOR_BRIGGS_MATE, 0, 10);
  Actor_FaceDirection(ACTOR_BRIGGS, FACING_SOUTH, 0);
  Event_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_BRIGGS, 2);
  Event_Wait(10);
  // "That's only because everyone in Champa would starve otherwise! We..."
  Event_ShowMessageThenWait(ACTOR_BRIGGS, 0, 10);
  Actor_ShowEmote(ACTOR_BRIGGS_MATE, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  // "Ah..."
  Event_ShowMessageThenWait(ACTOR_BRIGGS_MATE, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_BRIGGS, 3);
  Event_Wait(10);
  // "And even then, it's never a lot."
  Event_ShowMessageThenWait(ACTOR_BRIGGS, 0, 10);
  Actor_ShowEmote(ACTOR_BRIGGS_MATE, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
  // "Wha-What's not?"
  Event_ShowMessageThenWait(ACTOR_BRIGGS_MATE, 0, 10);
  // "The stuff we take. We never take too much from any one town."
  Event_ShowMessageThenWait(ACTOR_BRIGGS, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_BRIGGS_MATE, 3);
  Event_Wait(10);
  // "Yeah, I guess not..."
  Event_ShowMessageThenWait(ACTOR_BRIGGS_MATE, 0, 10);
  Actor_ShowEmote(ACTOR_BRIGGS, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  // "And why do you think that is?"
  Event_ShowMessageThenWait(ACTOR_BRIGGS, 0, 10);
  Actor_StartRepeatedMotion(ACTOR_BRIGGS_MATE, 3);
  Actor_SetAttachedEffect(ACTOR_BRIGGS_MATE,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(40);
  Actor_FaceDirectionById(ACTOR_BRIGGS_MATE, FACING_SOUTH_SOUTHEAST, 0);
  Event_Wait(30);
  Actor_EmitNotes(9);
  Sound_Play(SE_LIGHT_APPEAR);
  Actor_ShowEmote(ACTOR_BRIGGS_MATE, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Actor_FaceDirectionById(ACTOR_BRIGGS_MATE, FACING_NORTH_NORTHEAST, 0);
  Event_Wait(30);
  // "Er... Why?"
  Event_ShowMessageThenWait(ACTOR_BRIGGS_MATE, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_BRIGGS, 3);
  Event_Wait(10);
  // "Because you never can tell how these wealthy towns will react. It's..."
  Event_ShowMessageThenWait(ACTOR_BRIGGS, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_BRIGGS_MATE, 2);
  Event_Wait(10);
  // "Hey, yeah! Which is why we didn't steal anything from Alhafra..."
  Event_ShowMessageThenWait(ACTOR_BRIGGS_MATE, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_BRIGGS, 3);
  Event_Wait(10);
  // "Right! That's why we avoid the risky towns and just take what we..."
  Event_ShowMessageThenWait(ACTOR_BRIGGS, 0, 10);
  Actor_ShowEmote(ACTOR_BRIGGS_MATE, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  // "See? That's why you're our captain, Briggs! You've got brains coming..."
  Event_ShowMessageThenWait(ACTOR_BRIGGS_MATE, 0, 10);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 30);
  // "Hmph! There's nothing brainy about that!"
  Event_ShowMessageThenWait(ACTOR_JENNA, 0, 10);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTHEAST, 0);
  Actor_FaceDirectionById(ACTOR_FELIX, FACING_EAST, 0);
  Event_Wait(20);
  Actor_StartRepeatedMotion(ACTOR_KRADEN, 3);
  Actor_SetAttachedEffect(ACTOR_KRADEN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_StartRepeatedMotion(ACTOR_FELIX, 3);
  Actor_SetAttachedEffect(ACTOR_FELIX,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(30);
  Actor_FaceDirectionById(ACTOR_BRIGGS, FACING_EAST, 0);
  Actor_FaceDirectionById(ACTOR_BRIGGS_MATE, FACING_EAST, 0);
  Actor_ShowEmote(ACTOR_BRIGGS, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmoteById(ACTOR_BRIGGS_MATE, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                      30);
  Event_Wait(20);
  // "Did you hear that? It sounded like a girl! Who's out there!?"
  Event_ShowMessageThenWait(ACTOR_BRIGGS, 0, 10);
  Actor_SetSpeed(ACTOR_BRIGGS, 1.5);
  Actor_SetSpeed(ACTOR_BRIGGS_MATE, 1.5);
  Actor_SetMovementTypeById(ACTOR_BRIGGS_MATE,
                            (s32)gEasternAlhafraShip_MateRunsScript);
  Event_Wait(5);
  Actor_SetMovementTypeById(ACTOR_BRIGGS,
                            (s32)gEasternAlhafraShip_BriggsRunsScript);
  Actor_WaitForScriptEndById(ACTOR_BRIGGS);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_SOUTH, 0);
  FieldCamera_MoveTo(448, CAMERA_KEEP, 416, 1);
  FieldCamera_WaitForArrival();
  Event_Wait(20);
  Actor_Jump(ACTOR_BRIGGS_MATE, 6, 15);
  Actor_Jump(ACTOR_BRIGGS_MATE, 6, 23);
  // "What do you think you're doing?"
  Event_ShowMessageThenWait(ACTOR_BRIGGS_MATE, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_SHEBA, ANIM_NOD);
  Event_Wait(20);
  // "Don't you know that someone has been falsely imprisoned for your crimes?"
  Event_ShowMessageThenWait(ACTOR_SHEBA, 0, 10);
  Actor_ShowEmote(ACTOR_BRIGGS, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  // "Er... What's your point?"
  Event_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_BRIGGS, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_JENNA, 2);
  Event_Wait(10);
  // "You have to come with us and attest to that man's innocence!"
  Event_ShowMessageThenWait(ACTOR_JENNA, 0, 10);
  Actor_FaceDirectionById(ACTOR_BRIGGS, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_BRIGGS_MATE, FACING_WEST, 0);
  Event_Wait(45);
  Actor_FaceDirection(ACTOR_BRIGGS, FACING_NORTH, 0);
  Actor_FaceDirectionById(ACTOR_BRIGGS_MATE, FACING_NORTH_NORTHWEST, 0);
  Event_Wait(20);
  // "And...why would I want to do something like that?"
  Event_ShowMessageThenWait(ACTOR_BRIGGS, 0, 10);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_WEST, 0);
  Event_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_SHEBA, ANIM_NOD);
  Event_Wait(10);
  Actor_FaceDirectionById(ACTOR_KRADEN, FACING_EAST, 0);
  Event_Wait(20);
  Actor_StartRepeatedMotion(ACTOR_KRADEN, 3);
  Actor_SetAttachedEffect(ACTOR_KRADEN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(40);
  Actor_FaceDirectionById(ACTOR_JENNA, FACING_NORTHWEST, 0);
  Event_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
  Event_Wait(10);
  Actor_FaceDirectionById(ACTOR_KRADEN, FACING_SOUTHEAST, 0);
  Event_Wait(20);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  // "Oh-ho! I was hoping we could avoid any ugliness, but you leave us no..."
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0, 10);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
  Actor_FaceDirectionById(ACTOR_SHEBA, FACING_SOUTH, 0);
  Event_Wait(20);
  Actor_Jump(ACTOR_KRADEN, 6, 15);
  Actor_Jump(ACTOR_KRADEN, 6, 23);
  // "We'll bring you in against your will if we must, but either way..."
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0, 10);
  Actor_ShowEmoteById(ACTOR_BRIGGS, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 50);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 50);
  // "Isn't that right, Felix?"
  Event_OpenMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0);
  result = FieldEvent_AskYesNoWithActorAnimation(4, 0);
  if (result == 0) {
    Event_Wait(20);
    Actor_FaceDirectionById(ACTOR_JENNA, FACING_WEST, 0);
    Event_Wait(20);
    Event_ShowMessageThenWait(ACTOR_JENNA, 0, 10);
    gFieldWork->message++;
  } else {
    EasternAlhafraShip_CountAnswer();
    Event_Wait(20);
    Actor_FaceDirectionById(ACTOR_JENNA, FACING_WEST, 0);
    Event_Wait(20);
    gFieldWork->message++;
    Event_ShowMessageThenWait(ACTOR_JENNA, 0, 10);
  }
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
  Actor_Jump(ACTOR_BRIGGS_MATE, 6, 15);
  Actor_Jump(ACTOR_BRIGGS_MATE, 6, 23);
  Event_ShowMessageThenWait(ACTOR_BRIGGS_MATE, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_BRIGGS, 2);
  Event_Wait(10);
  Event_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_BRIGGS, 0, 10);
  Actor_WalkBy(ACTOR_BRIGGS, 0, -16);
  Actor_WalkByById(ACTOR_BRIGGS_MATE, 0, -16);
  Event_Wait(10);
  Event_SetAfterBattleDestination(SCENE_EASTERN_ALHAFRA_SHIP,
                                  ENTRANCE_AFTER_BATTLE);

  gMainRoutineWork.battleType = BATTLE_TYPE_NO_ESCAPE;
  Field_RequestBattleFromEncounterTable(10, 1);
  Event_End();
}

void EasternAlhafraShip_BriggsIsCaught(void) {
  struct Actor *emote;
  struct Actor *briggs;
  struct Actor *leader;

  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgWellThatTakes);
  Actor_SetPosition(ACTOR_BRIGGS, 436, 416);
  FieldActor_SetSpriteRotationById(ACTOR_BRIGGS, TRIG_TURN / 4);
  Actor_SetSpriteFlags((s32)Actor_GetById(ACTOR_BRIGGS), 0);
  Actor_SetPosition(ACTOR_BRIGGS_MATE, 456, 432);
  FieldActor_SetSpriteRotationById(ACTOR_BRIGGS_MATE, TRIG_TURN / 2);
  Actor_SetSpriteFlags((s32)Actor_GetById(ACTOR_BRIGGS_MATE), 0);
  Actor_GetById(ACTOR_BRIGGS_MATE)->facing = FACING_SOUTH;
  Actor_SetPosition(ACTOR_FIRST_PIRATE, 456, 460);
  FieldActor_SetSpriteRotationById(ACTOR_FIRST_PIRATE, TRIG_TURN / 2);
  Actor_SetSpriteFlags((s32)Actor_GetById(ACTOR_FIRST_PIRATE), 0);
  Actor_SetPosition(ACTOR_SECOND_PIRATE, 420, 424);
  Actor_SetSpriteRotationById(ACTOR_SECOND_PIRATE, TRIG_TURN * 3 / 4);
  Actor_SetSpriteFlags((s32)Actor_GetById(ACTOR_SECOND_PIRATE), 0);
  Actor_SetPosition(ACTOR_THIRD_PIRATE, 388, 440);
  Actor_SetSpriteRotationById(ACTOR_THIRD_PIRATE, TRIG_TURN / 4);
  Actor_SetSpriteFlags((s32)Actor_GetById(ACTOR_THIRD_PIRATE), 0);
  Actor_SetPosition(ACTOR_KRADEN, 456, 376);
  Actor_SetPosition(ACTOR_JENNA, 472, 392);
  Actor_SetPosition(ACTOR_SHEBA, 472, 376);
  Actor_SetPosition(ACTOR_CHAUCHA, 456, 552);
  Actor_SetPosition(ACTOR_SECOND_ALHAFRAN_SOLDIER, 472, 280);
  Actor_SetPosition(ACTOR_ALHAFRAN_SOLDIER, 440, 280);
  Actor_SetPosition(ACTOR_MADRAN_SOLDIER, 456, 296);
  Actor_SetPosition(ACTOR_MADRAS_MAYOR, 472, 296);
  Actor_SetPosition(ACTOR_MADRAS_ELDER, 440, 296);
  Actor_SetPosition(ACTOR_ALHAFRAS_MAYOR, 456, 304);
  Actor_SetSpeed(ACTOR_SECOND_ALHAFRAN_SOLDIER, 1);
  Actor_SetSpeed(ACTOR_ALHAFRAN_SOLDIER, 1);
  Actor_SetSpeed(ACTOR_MADRAN_SOLDIER, 1);
  Actor_SetSpeed(ACTOR_MADRAS_MAYOR, 1);
  Actor_SetSpeed(ACTOR_MADRAS_ELDER, 1);
  Actor_SetSpeed(ACTOR_ALHAFRAS_MAYOR, 1);
  Actor_SetSpeed(ACTOR_FELIX, 1);
  Actor_SetSpeed(ACTOR_KRADEN, 1);
  Actor_SetSpeed(ACTOR_JENNA, 1);
  Actor_SetSpeed(ACTOR_SHEBA, 1);
  Actor_SetSpeed(ACTOR_CHAUCHA, 1);
  Actor_SetSpeed(ACTOR_BRIGGS, 1);
  Actor_SetSpeed(ACTOR_BRIGGS_MATE, 1);
  Actor_SetSpeed(ACTOR_FIRST_PIRATE, 1);
  Actor_SetSpeed(ACTOR_SECOND_PIRATE, 1);
  Actor_SetSpeed(ACTOR_THIRD_PIRATE, 1);
  FieldCamera_MoveTo(464, CAMERA_KEEP, 408, 0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
  Event_Wait(10);
  // "Well, that takes care of that!"
  Event_ShowMessageThenWait(ACTOR_JENNA, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_SHEBA, ANIM_NOD);
  Event_Wait(10);
  // "And I'm willing to bet everyone Briggs has been harassing will be..."
  Event_ShowMessageThenWait(ACTOR_SHEBA, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, 3);
  Event_Wait(10);
  // "This should set everyone in Indra at ease... They all seemed to fear..."
  Event_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_BRIGGS, 2);
  Event_Wait(10);
  // "We lost..."
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_BRIGGS, 0, 10);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTH - 1, 0);
  Actor_FaceDirectionById(ACTOR_JENNA, FACING_NORTH + 1, 0);
  Event_Wait(50);
  Actor_FaceDirectionById(ACTOR_FELIX, FACING_SOUTH, 0);
  Actor_FaceDirectionById(ACTOR_JENNA, FACING_SOUTH, 0);
  Event_Wait(40);
  Actor_SetAnimationAndWait(ACTOR_BRIGGS, 4);
  // "Even pirates have honor... I'll go quietly. I won't make a scene..."
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_BRIGGS, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_BRIGGS, 2);
  Event_Wait(10);
  // "Go on... Hand us over to Madra or Alhafra or whoever..."
  Event_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_BRIGGS, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_JENNA, 2);
  Event_Wait(10);
  // "Good... First off, you can tell the Madrans that the man called..."
  Event_ShowMessageThenWait(ACTOR_JENNA, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_BRIGGS, 3);
  Event_Wait(10);
  // "Fine. So, what, I just have to say that Piers isn't one of my men?"
  Event_OpenMessage(8, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(4, 0) == 0) {
    Event_Wait(30);
    Actor_SetAnimationAndWait(ACTOR_SHEBA, ANIM_NOD);
    Event_Wait(10);
    Event_ShowMessageThenWait(ACTOR_SHEBA, 0, 10);
    gFieldWork->message += 1;
  } else {
    Event_Wait(40);
    Actor_RunRepeatedMotion(ACTOR_SHEBA, 2);
    Event_Wait(10);
    gFieldWork->message += 1;
    Event_ShowMessageThenWait(ACTOR_SHEBA, 0, 10);
  }
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
  Event_Wait(10);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0, 10);
  Event_ShowMessageThenWait(ACTOR_CHAUCHA, 0, 10);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  FieldCamera_FollowActor(ACTOR_CHAUCHA, 1);
  FieldCamera_WaitForArrival();
  Actor_WalkToAndWait(ACTOR_CHAUCHA, 456, 504);
  Actor_WalkByAndWaitById(ACTOR_CHAUCHA, 16, 0);
  Actor_WalkToAndWaitById(ACTOR_CHAUCHA, 472, 424);
  Event_Wait(10);
  FieldCamera_MoveTo(464, CAMERA_KEEP, 408, 1);
  FieldCamera_WaitForArrival();
  Event_Wait(10);
  Event_ShowMessageThenWait(ACTOR_CHAUCHA, 0, 10);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST, 0);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0, 10);
  Actor_StartRepeatedMotion(ACTOR_CHAUCHA, 3);
  Actor_SetAttachedEffect(ACTOR_CHAUCHA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(40);
  Event_ShowMessageThenWait(ACTOR_CHAUCHA, 0, 10);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  Event_ShowMessageThenWait(ACTOR_JENNA, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_BRIGGS, 2);
  Event_Wait(10);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_BRIGGS, 0, 10);
  Actor_ShowEmote(ACTOR_CHAUCHA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
  Event_ShowMessageThenWait(ACTOR_CHAUCHA, 0, 10);
  Actor_SetAnimationById(ACTOR_KRADEN, 3);
  Actor_SetAnimationById(ACTOR_JENNA, ANIM_NOD);
  Actor_SetAnimationById(ACTOR_SHEBA, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
  Event_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_CHAUCHA, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(ACTOR_CHAUCHA, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_SHEBA, 2);
  Event_Wait(10);
  Event_ShowMessageThenWait(ACTOR_SHEBA, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_CHAUCHA, 4);
  Event_ShowMessageThenWait(ACTOR_CHAUCHA, 0, 15);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTH - 1, 0);
  Actor_FaceDirectionById(ACTOR_JENNA, FACING_NORTH + 1, 0);
  Event_Wait(50);
  Actor_FaceDirection(ACTOR_FELIX, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
  Event_Wait(40);
  Actor_RunRepeatedMotion(ACTOR_CHAUCHA, 2);
  Event_Wait(10);
  Event_ShowMessageThenWait(ACTOR_CHAUCHA, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_CHAUCHA, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(ACTOR_CHAUCHA, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_BRIGGS, 2);
  Event_Wait(10);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_BRIGGS, 0, 10);
  Actor_SetAnimationById(ACTOR_JENNA, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_SHEBA, ANIM_NOD);
  Event_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_CHAUCHA, 4);
  Event_ShowMessageThenWait(ACTOR_CHAUCHA, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
  Event_Wait(10);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0, 10);
  emote = (struct Actor *)Actor_ShowEmote(ACTOR_BRIGGS,
                                          EMOTE_IN_FRONT | EMOTE_SCOWL, 0);
  emote->update = 0;
  Actor_SetPositionAndResetMotion(emote, (emote->x.fixed + px(22)),
                                  (emote->y.fixed + px(8)), emote->z.fixed);
  Event_Wait(30);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_BRIGGS, 0, 10);
  Actor_FaceDirection(ACTOR_CHAUCHA, FACING_WEST, 0);
  Event_Wait(20);
  Event_ShowMessageThenWait(ACTOR_CHAUCHA, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_JENNA, 2);
  Event_Wait(10);
  Event_ShowMessageThenWait(ACTOR_JENNA, 0, 10);
  Actor_FaceDirection(ACTOR_CHAUCHA, FACING_NORTH_NORTHWEST, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  Event_ShowMessageThenWait(ACTOR_SHEBA, 0, 10);
  Actor_FaceDirectionById(ACTOR_KRADEN, FACING_EAST, 0);
  Event_Wait(20);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0, 10);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTH_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_WEST, 0);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTHWEST, 0);
  Event_Wait(20);
  Event_ShowMessageThenWait(ACTOR_JENNA, 0, 10);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTHEAST, 0);
  Event_Wait(20);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0, 10);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  Event_ShowMessageThenWait(ACTOR_JENNA, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_BRIGGS, 2);
  Event_Wait(10);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_BRIGGS, 0, 10);
  Actor_FaceDirection(ACTOR_FELIX, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST, 0);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_CHAUCHA, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(ACTOR_CHAUCHA, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_BRIGGS, 4);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_BRIGGS, 0, 20);
  Actor_WalkByById(ACTOR_ALHAFRAS_MAYOR, 0, 8);
  Event_ShowMessageThenWait(ACTOR_MADRAS_MAYOR, 0, 10);
  emote = (struct Actor *)Actor_ShowEmote(
      ACTOR_BRIGGS, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  emote->update = 0;
  Actor_SetPositionAndResetMotion(emote, (emote->x.fixed + px(22)),
                                  (emote->y.fixed + px(8)), emote->z.fixed);
  Actor_ShowEmote(ACTOR_CHAUCHA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 50);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH, 0);
  Event_Wait(20);
  FieldCamera_MoveTo(464, CAMERA_KEEP, 304, 1);
  FieldCamera_WaitForArrival();
  Event_Wait(30);
  Actor_FaceDirection(ACTOR_ALHAFRAS_MAYOR, FACING_NORTHWEST, 0);
  Event_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_ALHAFRAS_MAYOR, 2);
  Event_Wait(10);
  Event_ShowMessageThenWait(ACTOR_ALHAFRAS_MAYOR, 0, 10);
  Actor_ShowEmote(ACTOR_MADRAS_ELDER, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  Actor_FaceDirection(ACTOR_MADRAS_ELDER, FACING_SOUTHEAST, 0);
  Event_Wait(20);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_MADRAS_ELDER, 0,
                                 10);
  Actor_ShowEmote(ACTOR_ALHAFRAS_MAYOR, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  Event_ShowMessageThenWait(ACTOR_ALHAFRAS_MAYOR, 0, 10);
  Actor_ShowEmote(ACTOR_MADRAS_MAYOR, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
  Event_ShowMessageThenWait(ACTOR_MADRAS_MAYOR, 0, 10);
  Actor_ShowEmote(ACTOR_MADRAN_SOLDIER, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  Event_ShowMessageThenWait(ACTOR_MADRAN_SOLDIER, 0, 10);
  Actor_FaceDirection(ACTOR_ALHAFRAS_MAYOR, FACING_NORTH, 0);
  Event_Wait(10);
  Event_ShowMessageThenWait(ACTOR_ALHAFRAS_MAYOR, 0, 10);
  Actor_ShowEmote(ACTOR_MADRAN_SOLDIER, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  Event_ShowMessageThenWait(ACTOR_MADRAN_SOLDIER, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_ALHAFRAS_MAYOR, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(ACTOR_ALHAFRAS_MAYOR, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_MADRAN_SOLDIER, 2);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_ALHAFRAS_MAYOR, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(ACTOR_ALHAFRAS_MAYOR, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_MADRAN_SOLDIER, 4);
  Actor_RunRepeatedMotion(ACTOR_MADRAS_ELDER, 2);
  Event_Wait(10);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_MADRAS_ELDER, 0,
                                 10);
  Actor_FaceDirection(ACTOR_ALHAFRAS_MAYOR, FACING_NORTHWEST, 0);
  Event_Wait(20);
  Actor_ShowEmote(ACTOR_ALHAFRAS_MAYOR, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  Event_ShowMessageThenWait(ACTOR_ALHAFRAS_MAYOR, 0, 10);
  Actor_ShowEmote(ACTOR_MADRAS_ELDER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_MADRAN_SOLDIER, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Actor_ShowEmote(ACTOR_MADRAS_MAYOR, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 30);
  Event_ShowMessageThenWait(ACTOR_MADRAS_MAYOR, 0, 10);
  Actor_FaceDirection(ACTOR_ALHAFRAS_MAYOR, FACING_NORTHEAST, 0);
  Event_Wait(20);
  Event_ShowMessageThenWait(ACTOR_ALHAFRAS_MAYOR, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_MADRAS_ELDER, 2);
  Event_Wait(10);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_MADRAS_ELDER, 0,
                                 10);
  Actor_FaceDirection(ACTOR_ALHAFRAS_MAYOR, FACING_NORTHWEST, 0);
  Event_Wait(20);
  Event_ShowMessageThenWait(ACTOR_ALHAFRAS_MAYOR, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_MADRAN_SOLDIER, 2);
  Event_Wait(10);
  Event_ShowMessageThenWait(ACTOR_MADRAN_SOLDIER, 0, 10);
  Actor_FaceDirection(ACTOR_ALHAFRAS_MAYOR, FACING_NORTH, 0);
  Event_Wait(20);
  Actor_ShowEmote(ACTOR_ALHAFRAS_MAYOR, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 30);
  Event_ShowMessageThenWait(ACTOR_ALHAFRAS_MAYOR, 0, 10);
  Actor_StartRepeatedMotion(ACTOR_MADRAS_ELDER, 3);
  Actor_SetAttachedEffect(ACTOR_MADRAS_ELDER,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(40);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0, 10);
  Actor_ShowEmote(ACTOR_ALHAFRAS_MAYOR, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_MADRAS_MAYOR, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_MADRAS_ELDER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_MADRAN_SOLDIER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  Actor_FaceDirection(ACTOR_ALHAFRAS_MAYOR, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_MADRAS_MAYOR, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_MADRAS_ELDER, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_MADRAN_SOLDIER, FACING_SOUTH, 0);
  Event_Wait(20);
  FieldCamera_MoveWithinMapBounds(px(464), -1, px(344), 1);
  FieldCamera_WaitForArrival();
  Event_Wait(30);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
  Event_Wait(10);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0, 10);
  Actor_WalkByAndWaitById(ACTOR_MADRAS_MAYOR, 0, 16);
  Event_Wait(10);
  Event_ShowMessageThenWait(ACTOR_MADRAS_MAYOR, 0, 10);
  Actor_WalkByById(ACTOR_ALHAFRAS_MAYOR, 0, 16);
  Actor_WalkByAndWaitById(ACTOR_MADRAS_ELDER, 0, 16);
  Actor_SetAnimationById(ACTOR_ALHAFRAS_MAYOR, 1);
  Event_Wait(10);
  FieldEvent_ShowTwoMessagesAndWait(ACTOR_MADRAS_ELDER, 13, 1, 13, 4,
                                    ACTOR_ALHAFRAS_MAYOR, 17, 11, 22, 6, 0);
  Actor_StartRepeatedMotion(ACTOR_ALHAFRAS_MAYOR, 2);
  Actor_RunRepeatedMotion(ACTOR_MADRAS_ELDER, 2);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_MADRAS_ELDER, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_ALHAFRAS_MAYOR, FACING_NORTHWEST, 0);
  Event_Wait(30);
  Actor_ShowEmote(ACTOR_MADRAS_MAYOR, EMOTE_IN_FRONT | EMOTE_WINCE, 50);
  Actor_ShowEmote(ACTOR_ALHAFRAS_MAYOR, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 60);
  Actor_ShowEmote(ACTOR_MADRAS_ELDER, EMOTE_IN_FRONT | EMOTE_SMILE, 50);
  Actor_SetAnimationAndWait(ACTOR_ALHAFRAS_MAYOR, 4);
  Actor_ShowEmote(ACTOR_ALHAFRAS_MAYOR, EMOTE_IN_FRONT | EMOTE_SMILE, 50);
  Actor_ShowEmote(ACTOR_MADRAS_ELDER, EMOTE_IN_FRONT | EMOTE_SCOWL, 70);
  Actor_ShowEmote(ACTOR_ALHAFRAS_MAYOR, EMOTE_IN_FRONT | EMOTE_WINCE, 50);
  Actor_SetAnimationAndWait(ACTOR_ALHAFRAS_MAYOR, 3);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_MADRAS_ELDER, 3);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_ALHAFRAS_MAYOR, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_MADRAS_ELDER, FACING_SOUTH_SOUTHEAST, 0);
  Event_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_ALHAFRAS_MAYOR, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(ACTOR_ALHAFRAS_MAYOR, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_MADRAS_ELDER, 3);
  Event_Wait(10);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_MADRAS_ELDER, 0,
                                 10);
  Actor_RunRepeatedMotion(ACTOR_MADRAN_SOLDIER, 2);
  Event_Wait(10);
  Event_ShowMessageThenWait(ACTOR_MADRAN_SOLDIER, 0, 10);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_SOUTH - 1, 0);
  Event_Wait(60);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTH, 0);
  Event_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, 3);
  Event_Wait(10);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0, 10);
  Actor_FaceDirectionById(ACTOR_KRADEN, FACING_SOUTH - 1, 0);
  Event_Wait(20);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
  Event_Wait(20);
  Actor_FaceDirectionById(ACTOR_KRADEN, FACING_NORTH, 0);
  Event_Wait(40);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
  Event_Wait(10);
  Event_ShowMessageThenWait(ACTOR_JENNA, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_SHEBA, ANIM_NOD);
  Event_Wait(10);
  Event_ShowMessageThenWait(ACTOR_SHEBA, 0, 10);
  Actor_SetAnimationById(ACTOR_ALHAFRAS_MAYOR, 3);
  Actor_SetAnimationById(ACTOR_MADRAS_MAYOR, 3);
  Actor_SetAnimationById(ACTOR_MADRAS_ELDER, 3);
  Actor_SetAnimationAndWait(ACTOR_MADRAN_SOLDIER, 3);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0, 10);
  Actor_StartRepeatedMotion(ACTOR_ALHAFRAS_MAYOR, 2);
  Actor_RunRepeatedMotion(ACTOR_MADRAS_MAYOR, 2);
  Event_Wait(10);
  FieldEvent_ShowTwoMessagesAndWait(ACTOR_MADRAS_MAYOR, 12, 1, 25, 4,
                                    ACTOR_ALHAFRAS_MAYOR, 9, 11, 14, 6, 0);
  Actor_StartRepeatedMotion(ACTOR_MADRAS_MAYOR, 3);
  Actor_SetAttachedEffect(ACTOR_MADRAS_MAYOR,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_StartRepeatedMotion(ACTOR_ALHAFRAS_MAYOR, 3);
  FieldEffect_SetAttached(ACTOR_ALHAFRAS_MAYOR,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(40);
  Actor_FaceDirection(ACTOR_ALHAFRAS_MAYOR, FACING_NORTHEAST, 0);
  Actor_FaceDirectionById(ACTOR_MADRAS_MAYOR, FACING_SOUTH_SOUTHWEST, 0);
  Event_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_ALHAFRAS_MAYOR, 3);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_MADRAS_MAYOR, 3);
  Event_Wait(20);
  Actor_FaceDirectionById(ACTOR_ALHAFRAS_MAYOR, FACING_SOUTH, 0);
  Event_Wait(30);
  Actor_RunRepeatedMotion(ACTOR_MADRAS_MAYOR, 2);
  Event_Wait(10);
  Event_ShowMessageThenWait(ACTOR_MADRAS_MAYOR, 0, 10);
  Actor_ShowEmote(ACTOR_MADRAS_MAYOR, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
  Event_ShowMessageThenWait(ACTOR_MADRAS_MAYOR, 0, 10);
  FieldCamera_MoveTo(464, CAMERA_KEEP, 432, 1);
  FieldCamera_WaitForArrival();
  Event_Wait(20);
  Actor_ShowEmote(ACTOR_CHAUCHA, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  emote = (struct Actor *)Actor_ShowEmote(ACTOR_BRIGGS,
                                          EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  {
    s32 x = emote->x.fixed;
    s32 y = emote->y.fixed;
    emote->update = 0;
    Actor_SetPositionAndResetMotion(emote, x + px(22), y + px(8),
                                    emote->z.fixed);
  }
  Event_Wait(50);
  FieldCamera_MoveTo(464, CAMERA_KEEP, 344, 1);
  FieldCamera_WaitForArrival();
  Event_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_ALHAFRAS_MAYOR, 2);
  Event_Wait(10);
  Event_ShowMessageThenWait(ACTOR_ALHAFRAS_MAYOR, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_ALHAFRAS_MAYOR, 4);
  Event_ShowMessageThenWait(ACTOR_ALHAFRAS_MAYOR, 0, 10);
  Actor_Jump(ACTOR_MADRAS_ELDER, 6, 15);
  Actor_Jump(ACTOR_MADRAS_ELDER, 6, 23);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_MADRAS_ELDER, 0,
                                 10);
  FieldCamera_MoveTo(464, CAMERA_KEEP, 424, 1);
  FieldCamera_WaitForArrival();
  Event_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_BRIGGS, 2);
  Event_Wait(10);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_BRIGGS, 0, 10);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_SOUTH, 0);
  Event_Wait(30);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_BRIGGS, 0, 10);
  briggs = Actor_GetById(ACTOR_BRIGGS);
  Actor_SetPosition(ACTOR_BRIGGS, briggs->x.part[0].pixel + 20,
                    briggs->z.part[0].pixel + 8);
  Actor_SetSpriteRotationById(ACTOR_BRIGGS, 0);
  Actor_SetSpriteFlags((s32)Actor_GetById(ACTOR_BRIGGS), 1);
  Actor_FaceDirection(ACTOR_BRIGGS, FACING_NORTH, 0);
  Actor_Jump(ACTOR_BRIGGS, 6, 40);
  Actor_SetAnimationAndWait(ACTOR_BRIGGS, 4);
  Actor_SetPosition(ACTOR_BRIGGS_MATE, 456, 440);
  Actor_SetSpriteRotationById(ACTOR_BRIGGS_MATE, 0);
  Actor_SetSpriteFlags((s32)Actor_GetById(ACTOR_BRIGGS_MATE), 1);
  Actor_SetPosition(ACTOR_FIRST_PIRATE, 456, 456);
  Actor_SetSpriteRotationById(ACTOR_FIRST_PIRATE, 0);
  Actor_SetSpriteFlags((s32)Actor_GetById(ACTOR_FIRST_PIRATE), 1);
  Actor_SetPosition(ACTOR_SECOND_PIRATE, 424, 424);
  Actor_SetSpriteRotationById(ACTOR_SECOND_PIRATE, 0);
  Actor_SetSpriteFlags((s32)Actor_GetById(ACTOR_SECOND_PIRATE), 1);
  Actor_SetPosition(ACTOR_THIRD_PIRATE, 408, 440);
  Actor_SetSpriteRotationById(ACTOR_THIRD_PIRATE, 0);
  Actor_SetSpriteFlags((s32)Actor_GetById(ACTOR_THIRD_PIRATE), 1);
  Actor_FaceDirection(ACTOR_BRIGGS_MATE, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_FIRST_PIRATE, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_SECOND_PIRATE, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_THIRD_PIRATE, FACING_NORTH, 0);
  Actor_Jump(ACTOR_FIRST_PIRATE, 6, 0);
  Actor_Jump(ACTOR_SECOND_PIRATE, 6, 0);
  Actor_Jump(ACTOR_THIRD_PIRATE, 6, 0);
  Actor_Jump(ACTOR_BRIGGS_MATE, 6, 40);
  Actor_SetAnimationById(ACTOR_FIRST_PIRATE, 4);
  Actor_SetAnimationById(ACTOR_SECOND_PIRATE, 4);
  Actor_SetAnimationById(ACTOR_THIRD_PIRATE, 4);
  Actor_SetAnimationAndWait(ACTOR_BRIGGS_MATE, 4);
  Event_Wait(10);
  FieldCamera_MoveTo(464, CAMERA_KEEP, 368, 1);
  FieldCamera_WaitForArrival();
  Event_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_MADRAS_MAYOR, 2);
  Event_Wait(10);
  Event_ShowMessageThenWait(ACTOR_MADRAS_MAYOR, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_BRIGGS, 4);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_BRIGGS, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_MADRAS_ELDER, 2);
  Event_Wait(10);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_MADRAS_ELDER, 0,
                                 10);
  FieldCamera_MoveTo(464, CAMERA_KEEP, 400, 1);
  FieldCamera_WaitForArrival();
  Event_Wait(20);
  Actor_ShowEmoteById(ACTOR_BRIGGS, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
  Actor_RunRepeatedMotion(ACTOR_CHAUCHA, 2);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_CHAUCHA, FACING_WEST, 0);
  Event_Wait(20);
  Event_ShowMessageThenWait(ACTOR_CHAUCHA, 0, 10);
  Actor_FaceDirectionById(ACTOR_BRIGGS, FACING_EAST, 0);
  Event_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_BRIGGS, 3);
  Event_Wait(10);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_BRIGGS, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_CHAUCHA, 2);
  Event_Wait(20);
  Actor_ShowEmote(ACTOR_BRIGGS, EMOTE_IN_FRONT | EMOTE_HEART, 40);
  Event_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_BRIGGS, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_CHAUCHA, 3);
  Event_Wait(30);
  Actor_FaceDirectionById(ACTOR_BRIGGS, FACING_SOUTH, 0);
  Event_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_BRIGGS, 3);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_BRIGGS_MATE, 3);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_FIRST_PIRATE, 3);
  Event_Wait(10);
  Actor_FaceDirectionById(ACTOR_BRIGGS, FACING_SOUTHWEST, 0);
  Event_Wait(15);
  Actor_SetAnimationById(ACTOR_SECOND_PIRATE, 3);
  Actor_SetAnimationAndWait(ACTOR_THIRD_PIRATE, 3);
  Event_Wait(20);
  Event_Wait(20);
  Actor_WalkByAndWait(ACTOR_BRIGGS, 16, -16);
  Actor_FaceDirection(ACTOR_CHAUCHA, FACING_NORTH_NORTHWEST, 0);
  Actor_WalkBy(ACTOR_BRIGGS_MATE, 0, -32);
  Actor_WalkBy(ACTOR_FIRST_PIRATE, 0, -32);
  Actor_WalkByById(ACTOR_SECOND_PIRATE, 16, 0);
  Actor_WalkByAndWait(ACTOR_THIRD_PIRATE, 16, -16);
  Actor_WalkBy(ACTOR_JENNA, -16, -32);
  Actor_WalkByAndWait(ACTOR_SHEBA, -16, -32);
  Actor_WaitForMoveById(ACTOR_JENNA);
  Actor_SetAnimationById(ACTOR_JENNA, ANIM_STAND);
  Actor_FaceDirectionById(ACTOR_JENNA, FACING_EAST, 0);
  Actor_FaceDirectionById(ACTOR_SHEBA, FACING_EAST, 0);
  Actor_FaceDirectionById(ACTOR_KRADEN, FACING_EAST, 0);
  Actor_FaceDirectionById(ACTOR_FELIX, FACING_EAST, 0);
  Actor_WalkByById(ACTOR_BRIGGS_MATE, 16, 0);
  Actor_WalkBy(ACTOR_FIRST_PIRATE, 0, -16);
  Actor_WalkByById(ACTOR_SECOND_PIRATE, 16, 0);
  Actor_WalkByById(ACTOR_THIRD_PIRATE, 16, 0);
  Actor_WalkByAndWait(ACTOR_BRIGGS, 0, -16);
  FieldCamera_MoveTo(464, CAMERA_KEEP, 384, 1);
  FieldCamera_WaitForArrival();
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_BRIGGS_MATE, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_SECOND_PIRATE, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_BRIGGS, FACING_WEST, 0);
  Event_Wait(20);
  Event_ShowMessageThenWait(ACTOR_BRIGGS, 0, 10);
  Actor_StartRepeatedMotion(ACTOR_FELIX, 3);
  Actor_SetAttachedEffect(ACTOR_FELIX,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(40);
  Actor_FaceDirection(ACTOR_BRIGGS, FACING_NORTH, 0);
  Event_Wait(20);
  Actor_WalkBy(ACTOR_BRIGGS_MATE, 0, -16);
  Actor_WalkByById(ACTOR_FIRST_PIRATE, 16, 0);
  Actor_WalkBy(ACTOR_SECOND_PIRATE, 0, -16);
  Actor_WalkByById(ACTOR_THIRD_PIRATE, 16, 0);
  Actor_WalkByAndWait(ACTOR_BRIGGS, 0, -16);
  Actor_WalkBy(ACTOR_BRIGGS_MATE, 0, -16);
  Actor_WalkBy(ACTOR_FIRST_PIRATE, 0, -16);
  Actor_WalkByById(ACTOR_SECOND_PIRATE, 16, 0);
  Actor_WalkBy(ACTOR_THIRD_PIRATE, 0, -16);
  Actor_WalkByAndWait(ACTOR_BRIGGS, 0, -16);
  Actor_WalkBy(ACTOR_BRIGGS_MATE, 0, -16);
  Actor_WalkBy(ACTOR_FIRST_PIRATE, 0, -16);
  Actor_WalkBy(ACTOR_SECOND_PIRATE, 0, -16);
  Actor_WalkByById(ACTOR_THIRD_PIRATE, 16, 0);
  Actor_WalkByAndWait(ACTOR_BRIGGS, 0, -16);
  FieldCamera_MoveTo(464, CAMERA_KEEP, 336, 1);
  FieldCamera_WaitForArrival();
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
  Event_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_MADRAS_MAYOR, 2);
  Event_Wait(10);
  Event_ShowMessageThenWait(ACTOR_MADRAS_MAYOR, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_BRIGGS_MATE, 2);
  Event_Wait(10);
  Event_ShowMessageThenWait(ACTOR_BRIGGS_MATE, 0, 10);
  Actor_ShowEmote(ACTOR_FIRST_PIRATE, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
  Event_ShowMessageThenWait(ACTOR_FIRST_PIRATE, 0, 10);
  Actor_FaceDirection(ACTOR_BRIGGS, FACING_SOUTH_SOUTHWEST, 0);
  Event_Wait(30);
  Actor_ShowEmote(ACTOR_BRIGGS, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 30);
  Event_ShowMessageThenWait(ACTOR_BRIGGS, 0, 10);
  Actor_ShowEmote(ACTOR_FIRST_PIRATE, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(ACTOR_SECOND_PIRATE, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(ACTOR_THIRD_PIRATE, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(ACTOR_BRIGGS_MATE, EMOTE_IN_FRONT | EMOTE_WINCE, 50);
  Actor_FaceDirection(ACTOR_BRIGGS, FACING_NORTH, 0);
  Event_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_BRIGGS, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(ACTOR_BRIGGS, 0, 10);
  Actor_FaceDirection(ACTOR_BRIGGS, FACING_SOUTH_SOUTHWEST, 0);
  Event_Wait(20);
  FieldCamera_MoveTo(464, CAMERA_KEEP, 416, 1);
  FieldCamera_WaitForArrival();
  Event_Wait(20);
  Actor_StartRepeatedMotion(ACTOR_CHAUCHA, 3);
  Actor_SetAttachedEffect(ACTOR_CHAUCHA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(50);
  FieldCamera_MoveTo(464, CAMERA_KEEP, 336, 1);
  FieldCamera_WaitForArrival();
  Event_Wait(20);
  Actor_FaceDirectionById(ACTOR_BRIGGS, FACING_NORTH_NORTHWEST, 0);
  Event_Wait(30);
  Event_ShowMessageThenWait(ACTOR_BRIGGS, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_MADRAS_MAYOR, 3);
  Event_Wait(20);
  Actor_FaceDirectionById(ACTOR_MADRAS_MAYOR, FACING_EAST, 0);
  Event_Wait(10);
  Actor_GetById(ACTOR_MADRAS_MAYOR)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkToAndWaitById(ACTOR_MADRAS_MAYOR, 456, 312);
  Event_Wait(1);
  Actor_GetById(ACTOR_MADRAS_MAYOR)->steeringFlags |= ACTOR_FACES_MOTION;
  Event_Wait(20);
  Actor_WalkBy(ACTOR_BRIGGS_MATE, 0, -16);
  Actor_WalkBy(ACTOR_FIRST_PIRATE, 0, -16);
  Actor_WalkBy(ACTOR_SECOND_PIRATE, 0, -16);
  Actor_WalkBy(ACTOR_THIRD_PIRATE, 0, -16);
  Actor_WalkByAndWait(ACTOR_BRIGGS, 0, -16);
  Actor_FaceDirection(ACTOR_BRIGGS, FACING_WEST, 0);
  Event_Wait(20);
  Actor_FaceDirectionById(ACTOR_ALHAFRAS_MAYOR, FACING_EAST, 0);
  Event_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_ALHAFRAS_MAYOR, 2);
  Event_Wait(10);
  Event_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_ALHAFRAS_MAYOR, 0,
                            10);
  Actor_SetAnimationAndWait(ACTOR_MADRAS_ELDER, 3);
  Event_Wait(10);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_MADRAS_ELDER, 0,
                                 10);
  Actor_FaceDirectionById(ACTOR_ALHAFRAS_MAYOR, FACING_NORTHWEST, 0);
  Event_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_ALHAFRAS_MAYOR, 3);
  Event_Wait(30);
  Actor_FaceDirectionById(ACTOR_ALHAFRAS_MAYOR, FACING_EAST, 0);
  Event_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_BRIGGS, 3);
  Event_Wait(20);
  Actor_WalkBy(ACTOR_BRIGGS_MATE, 0, -32);
  Actor_WalkBy(ACTOR_FIRST_PIRATE, 0, -32);
  Actor_WalkBy(ACTOR_SECOND_PIRATE, 0, -32);
  Actor_WalkBy(ACTOR_THIRD_PIRATE, 0, -32);
  Actor_WalkByAndWait(ACTOR_BRIGGS, 0, -32);
  Actor_FaceDirectionById(ACTOR_MADRAN_SOLDIER, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_ALHAFRAS_MAYOR, FACING_NORTH, 0);
  Actor_FaceDirectionById(ACTOR_BRIGGS, FACING_WEST, 0);
  Event_Wait(20);
  FieldCamera_MoveTo(464, CAMERA_KEEP, 296, 1);
  FieldCamera_WaitForArrival();
  Event_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_MADRAN_SOLDIER, 2);
  Event_Wait(10);
  Event_ShowMessageThenWait(ACTOR_MADRAN_SOLDIER, 0, 10);
  Actor_ShowEmote(ACTOR_BRIGGS, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
  Event_ShowMessageThenWait(ACTOR_BRIGGS, 0, 10);
  Actor_FaceDirectionById(ACTOR_MADRAS_ELDER, FACING_NORTHEAST, 0);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_MADRAS_ELDER, 3);
  Event_Wait(10);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_MADRAS_ELDER, 0,
                                 10);
  Actor_StartRepeatedMotion(ACTOR_MADRAN_SOLDIER, 3);
  Actor_SetAttachedEffect(ACTOR_MADRAN_SOLDIER,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(40);
  Actor_FaceDirectionById(ACTOR_MADRAN_SOLDIER, FACING_SOUTH_SOUTHWEST, 0);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_MADRAN_SOLDIER, 3);
  Event_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_MADRAS_ELDER, 2);
  Event_Wait(10);
  Event_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_MADRAS_ELDER, 0, 10);
  Actor_ShowEmote(ACTOR_MADRAN_SOLDIER, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  Event_ShowMessageThenWait(ACTOR_MADRAN_SOLDIER, 0, 10);
  Actor_FaceDirectionById(ACTOR_MADRAS_MAYOR, FACING_NORTH, 0);
  Event_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_MADRAS_MAYOR, 2);
  Event_Wait(10);
  Event_ShowMessageThenWait(ACTOR_MADRAS_MAYOR, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_MADRAN_SOLDIER, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(ACTOR_MADRAN_SOLDIER, 0, 10);
  Actor_SetAnimationById(ACTOR_MADRAS_MAYOR, 3);
  Actor_SetAnimationAndWait(ACTOR_MADRAS_ELDER, 3);
  Actor_SetSpeed(ACTOR_MADRAN_SOLDIER, 1.8);
  Actor_WalkByAndWait(ACTOR_MADRAN_SOLDIER, 0, -32);
  Actor_WalkByAndWait(ACTOR_MADRAN_SOLDIER, -16, 0);
  Actor_SetPosition(ACTOR_MADRAN_SOLDIER, 0, 0);
  Event_Wait(20);
  Actor_WalkByAndWaitById(ACTOR_ALHAFRAN_SOLDIER, 16, 16);
  Actor_FaceDirectionById(ACTOR_ALHAFRAN_SOLDIER, FACING_EAST, 0);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_ALHAFRAN_SOLDIER, 2);
  Event_Wait(10);
  Event_ShowMessageThenWait(ACTOR_ALHAFRAN_SOLDIER, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_BRIGGS, 3);
  Event_Wait(20);
  Actor_SetMovementTypeById(ACTOR_SECOND_ALHAFRAN_SOLDIER,
                            gEasternAlhafraShip_LeaveTheDeckScript);
  Event_Wait(10);
  Actor_SetMovementTypeById(ACTOR_BRIGGS,
                            gEasternAlhafraShip_LeaveTheDeckScript);
  Event_Wait(10);
  Actor_SetMovementTypeById(ACTOR_BRIGGS_MATE,
                            gEasternAlhafraShip_LeaveTheDeckScript);
  Event_Wait(10);
  Actor_SetMovementTypeById(ACTOR_FIRST_PIRATE,
                            gEasternAlhafraShip_LeaveTheDeckScript);
  Event_Wait(10);
  Actor_SetMovementTypeById(ACTOR_SECOND_PIRATE,
                            gEasternAlhafraShip_LeaveTheDeckScript);
  Event_Wait(10);
  Actor_SetMovementTypeById(ACTOR_THIRD_PIRATE,
                            gEasternAlhafraShip_LeaveTheDeckScript);
  Event_Wait(75);
  Actor_WalkByAndWaitById(ACTOR_ALHAFRAN_SOLDIER, 16, 0);
  Actor_WalkByAndWait(ACTOR_ALHAFRAN_SOLDIER, 0, -32);
  Actor_WalkByAndWait(ACTOR_ALHAFRAN_SOLDIER, -32, 0);
  Actor_SetPosition(ACTOR_ALHAFRAN_SOLDIER, 0, 0);
  Event_Wait(30);
  Actor_FaceDirection(ACTOR_MADRAS_ELDER, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_MADRAS_MAYOR, FACING_SOUTH, 0);
  Event_Wait(30);
  Actor_RunRepeatedMotion(ACTOR_ALHAFRAS_MAYOR, 2);
  Event_Wait(10);
  Event_ShowMessageThenWait(ACTOR_ALHAFRAS_MAYOR, 0, 10);
  Actor_FaceDirection(ACTOR_ALHAFRAS_MAYOR, FACING_NORTHWEST, 0);
  Event_Wait(20);
  Event_ShowMessageThenWait(ACTOR_ALHAFRAS_MAYOR, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_MADRAS_ELDER, 3);
  Event_Wait(10);
  Actor_WalkByAndWaitById(ACTOR_ALHAFRAS_MAYOR, 16, 0);
  Actor_WalkBy(ACTOR_ALHAFRAS_MAYOR, 0, -32);
  Actor_WalkByAndWait(ACTOR_MADRAS_ELDER, 0, -16);
  Actor_WalkByAndWaitById(ACTOR_MADRAS_ELDER, 16, 0);
  Actor_FaceDirection(ACTOR_MADRAS_ELDER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_MADRAS_MAYOR, FACING_NORTH, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_ALHAFRAS_MAYOR, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  Actor_FaceDirection(ACTOR_ALHAFRAS_MAYOR, FACING_SOUTH, 0);
  Event_Wait(20);
  Event_ShowMessageThenWait(ACTOR_ALHAFRAS_MAYOR, 0, 10);
  FieldCamera_MoveTo(464, CAMERA_KEEP, 320, 1);
  FieldCamera_WaitForArrival();
  Event_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_MADRAS_MAYOR, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_MADRAS_ELDER, FACING_SOUTH, 0);
  Actor_WalkByById(ACTOR_FELIX, 16, 0);
  Actor_WalkByAndWaitById(ACTOR_KRADEN, 16, 0);
  Actor_WalkBy(ACTOR_FELIX, 0, -32);
  Actor_WalkByAndWait(ACTOR_KRADEN, 0, -32);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, 3);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_ALHAFRAS_MAYOR, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(ACTOR_ALHAFRAS_MAYOR, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, 3);
  Event_Wait(20);
  Actor_ShowEmote(ACTOR_ALHAFRAS_MAYOR, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  Event_ShowMessageThenWait(ACTOR_ALHAFRAS_MAYOR, 0, 10);
  Actor_ShowEmoteById(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 50);
  Actor_SetAnimationAndWait(ACTOR_ALHAFRAS_MAYOR, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(ACTOR_ALHAFRAS_MAYOR, 0, 10);
  Actor_FaceDirectionById(ACTOR_ALHAFRAS_MAYOR, FACING_WEST, 0);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_ALHAFRAS_MAYOR, 3);
  Event_Wait(20);
  Actor_FaceDirectionById(ACTOR_MADRAS_ELDER, FACING_EAST, 0);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_MADRAS_ELDER, 3);
  Event_Wait(10);
  Actor_WalkBy(ACTOR_ALHAFRAS_MAYOR, 0, -32);
  Actor_WalkByAndWait(ACTOR_MADRAS_ELDER, 0, -32);
  Actor_WalkBy(ACTOR_ALHAFRAS_MAYOR, -32, 0);
  Actor_WalkByAndWait(ACTOR_MADRAS_ELDER, -16, 0);
  Actor_SetPosition(ACTOR_MADRAS_ELDER, 0, 0);
  Actor_WaitForMoveById(ACTOR_ALHAFRAS_MAYOR);
  Actor_SetPosition(ACTOR_ALHAFRAS_MAYOR, 0, 0);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_MADRAS_MAYOR, FACING_NORTH, 0);
  Event_Wait(30);
  Event_ShowMessageThenWait(ACTOR_MADRAS_MAYOR, 0, 10);
  Actor_FaceDirection(ACTOR_MADRAS_MAYOR, FACING_SOUTH_SOUTHEAST, 0);
  Event_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_MADRAS_MAYOR, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(ACTOR_MADRAS_MAYOR, 0, 10);
  Actor_SetAnimationById(ACTOR_KRADEN, 3);
  Actor_SetAnimationById(ACTOR_SHEBA, ANIM_NOD);
  Actor_SetAnimationById(ACTOR_JENNA, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
  Event_Wait(30);
  Actor_RunRepeatedMotion(ACTOR_MADRAS_MAYOR, 2);
  Event_Wait(10);
  Event_ShowMessageThenWait(ACTOR_MADRAS_MAYOR, 0, 10);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirectionById(ACTOR_SHEBA, FACING_SOUTH + 1, 0);
  Event_Wait(30);
  Actor_SetAnimationById(ACTOR_KRADEN, 3);
  Actor_SetAnimationById(ACTOR_SHEBA, ANIM_NOD);
  Actor_SetAnimationById(ACTOR_JENNA, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH, 0);
  Actor_FaceDirectionById(ACTOR_SHEBA, FACING_NORTH, 0);
  Event_Wait(40);
  Actor_WalkByAndWaitById(ACTOR_MADRAS_MAYOR, -32, -8);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_MADRAS_MAYOR, FACING_NORTHWEST, 0);
  Event_Wait(30);
  Actor_FaceDirectionById(ACTOR_MADRAS_MAYOR, FACING_SOUTHWEST, 0);
  Event_Wait(30);
  Actor_FaceDirection(ACTOR_MADRAS_MAYOR, FACING_NORTHWEST, 0);
  Event_Wait(30);
  Actor_WalkByAndWait(ACTOR_MADRAS_MAYOR, 24, -16);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_MADRAS_MAYOR, FACING_NORTHWEST, 0);
  Event_Wait(30);
  Actor_FaceDirectionById(ACTOR_MADRAS_MAYOR, FACING_NORTHEAST, 0);
  Event_Wait(30);
  Actor_FaceDirectionById(ACTOR_MADRAS_MAYOR, FACING_NORTHWEST, 0);
  Event_Wait(30);
  Actor_WalkByAndWaitById(ACTOR_MADRAS_MAYOR, 16, 16);
  Actor_FaceDirection(ACTOR_MADRAS_MAYOR, FACING_SOUTH, 0);
  Event_Wait(20);
  Event_ShowMessageThenWait(ACTOR_MADRAS_MAYOR, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_MADRAS_MAYOR, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(ACTOR_MADRAS_MAYOR, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_MADRAS_MAYOR, 2);
  Event_Wait(10);
  Event_ShowMessageThenWait(ACTOR_MADRAS_MAYOR, 0, 10);
  Actor_FaceDirectionById(ACTOR_KRADEN, FACING_SOUTH, 0);
  Event_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
  Event_Wait(10);
  Event_OpenMessage(ACTOR_KRADEN, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(4, 0) == 0) {
    Event_Wait(30);
    Actor_SetAnimationAndWait(ACTOR_KRADEN, 3);
    Event_Wait(10);
    Actor_FaceDirectionById(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 0);
    Event_Wait(20);
    Actor_SetAnimationAndWait(ACTOR_MADRAS_MAYOR, 4);
    Event_ShowMessageThenWait(ACTOR_MADRAS_MAYOR, 0, 10);
    gFieldWork->message += 1;
  } else {
    Event_Wait(40);
    EasternAlhafraShip_CountAnswer();
    Actor_FaceDirectionById(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 0);
    Event_Wait(20);
    Actor_SetAnimationAndWait(ACTOR_MADRAS_MAYOR, 3);
    Event_Wait(20);
    gFieldWork->message += 1;
    Event_ShowMessageThenWait(ACTOR_MADRAS_MAYOR, 0, 10);
  }
  Actor_ShowEmote(ACTOR_MADRAS_MAYOR, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
  Event_ShowMessageThenWait(ACTOR_MADRAS_MAYOR, 0, 10);
  Actor_EmitNotes(ACTOR_MADRAS_MAYOR);
  Actor_ShowEmote(ACTOR_MADRAS_MAYOR, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 40);
  Event_ShowMessageThenWait(ACTOR_MADRAS_MAYOR, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_MADRAS_MAYOR, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(ACTOR_MADRAS_MAYOR, 0, 10);
  Actor_ShowEmote(ACTOR_MADRAS_MAYOR, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  Event_ShowMessageThenWait(ACTOR_MADRAS_MAYOR, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_MADRAS_MAYOR, 3);
  Event_Wait(20);
  Actor_SetAnimationById(ACTOR_KRADEN, 3);
  Actor_SetAnimationById(ACTOR_SHEBA, ANIM_NOD);
  Actor_SetAnimationById(ACTOR_JENNA, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
  Event_Wait(20);
  Actor_WalkByAndWait(ACTOR_MADRAS_MAYOR, 0, -40);
  Actor_WalkByAndWait(ACTOR_MADRAS_MAYOR, -24, 0);
  Actor_SetPosition(ACTOR_MADRAS_MAYOR, 0, 0);
  Event_Wait(20);
  FieldCamera_MoveTo(464, CAMERA_KEEP, 400, 1);
  FieldCamera_WaitForArrival();
  Event_Wait(30);
  Actor_ShowEmote(ACTOR_CHAUCHA, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 30);
  Event_ShowMessageThenWait(ACTOR_CHAUCHA, 0, 10);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_SOUTH, 0);
  Event_Wait(30);
  Actor_ShowEmoteById(ACTOR_CHAUCHA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 50);
  Actor_StartRepeatedMotion(ACTOR_CHAUCHA, 3);
  FieldEffect_SetAttached(ACTOR_CHAUCHA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(40);
  Event_ShowMessageThenWait(ACTOR_CHAUCHA, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_CHAUCHA, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(ACTOR_CHAUCHA, 0, 10);
  Actor_SetAnimationById(ACTOR_KRADEN, 3);
  Actor_SetAnimationById(ACTOR_SHEBA, ANIM_NOD);
  Actor_SetAnimationById(ACTOR_JENNA, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
  Event_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_CHAUCHA, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(ACTOR_CHAUCHA, 0, 10);
  Actor_WalkToAndWait(ACTOR_CHAUCHA, 472, 376);
  FieldCamera_FollowActor(ACTOR_CHAUCHA, 1);
  Actor_FaceDirectionById(ACTOR_SHEBA, FACING_EAST, 0);
  Actor_FaceDirectionById(ACTOR_JENNA, FACING_EAST, 0);
  Actor_WalkBy(ACTOR_KRADEN, -16, -32);
  Actor_WalkByAndWait(ACTOR_FELIX, -16, -32);
  Actor_SetAnimationById(ACTOR_KRADEN, 1);
  Actor_FaceDirectionById(ACTOR_KRADEN, FACING_EAST, 0);
  Actor_FaceDirectionById(ACTOR_FELIX, FACING_EAST, 0);
  Actor_WalkToAndWait(ACTOR_CHAUCHA, 472, 320);
  Actor_WalkBy(ACTOR_JENNA, 16, -32);
  Actor_WalkBy(ACTOR_SHEBA, 16, -32);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
  Actor_WalkToAndWait(ACTOR_CHAUCHA, 472, 264);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTH_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTH_NORTHWEST, 0);
  Actor_WalkToAndWait(ACTOR_CHAUCHA, 440, 264);
  Actor_SetPosition(ACTOR_CHAUCHA, 0, 0);
  Event_Wait(30);
  Event_ShowMessageThenWait(ACTOR_SHEBA, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_JENNA, 2);
  Event_Wait(10);
  Event_ShowMessageThenWait(ACTOR_JENNA, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_SHEBA, 2);
  Event_Wait(25);
  Actor_SetAnimationAndWait(ACTOR_SHEBA, ANIM_NOD);
  Event_Wait(10);
  Actor_FaceDirectionById(ACTOR_KRADEN, FACING_SOUTH, 0);
  Event_Wait(20);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0, 10);
  Event_Wait(20);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 30);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0, 10);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_WEST, 0);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_SHEBA, ANIM_NOD);
  Event_Wait(10);
  Event_ShowMessageThenWait(ACTOR_SHEBA, 0, 10);
  Actor_FaceDirectionById(ACTOR_JENNA, FACING_WEST, 0);
  Event_Wait(20);
  Event_ShowMessageThenWait(ACTOR_JENNA, 0, 10);
  Actor_FaceDirectionById(ACTOR_FELIX, FACING_EAST, 0);
  Actor_FaceDirectionById(ACTOR_KRADEN, FACING_SOUTHEAST, 0);
  Event_Wait(20);
  Event_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0, 10);
  Actor_FaceDirectionById(ACTOR_SHEBA, FACING_SOUTHWEST, 0);
  Event_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_SHEBA, 2);
  Event_Wait(10);
  Event_OpenMessage(6, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTHEAST, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(4, 0) == 0) {
    Event_Wait(30);
    Actor_SetAnimationAndWait(ACTOR_SHEBA, ANIM_NOD);
    Event_Wait(20);
    Event_ShowMessageThenWait(ACTOR_JENNA, 0, 10);
    gFieldWork->message += 1;
  } else {
    Event_Wait(40);
    Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_SHAKE_HEAD);
    gFieldWork->message += 1;
    Event_ShowMessageThenWait(ACTOR_JENNA, 0, 10);
  }
  Actor_SetAnimationById(ACTOR_KRADEN, 3);
  Actor_SetAnimationById(ACTOR_SHEBA, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
  Event_Wait(30);
  Actor_SetSpeed(ACTOR_JENNA, 1.2);
  Actor_SetAnimationById(ACTOR_JENNA, ANIM_WALK);
  leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  if (leader != 0) {
    Actor_SetDestinationById(ACTOR_JENNA, leader->x.part[0].pixel,
                             leader->z.part[0].pixel);
  }
  Actor_WaitForMoveById(ACTOR_JENNA);
  Actor_SetPosition(ACTOR_JENNA, 0, 0);
  Actor_SetSpeed(ACTOR_SHEBA, 1.2);
  Actor_SetAnimationById(ACTOR_SHEBA, ANIM_WALK);
  leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  if (leader != 0) {
    Actor_SetDestinationById(ACTOR_SHEBA, leader->x.part[0].pixel,
                             leader->z.part[0].pixel);
  }
  Actor_WaitForMoveById(ACTOR_SHEBA);
  Actor_SetPosition(ACTOR_SHEBA, 0, 0);
  Actor_SetSpeed(ACTOR_KRADEN, 1.2);
  Actor_SetAnimationById(ACTOR_KRADEN, 2);
  leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  if (leader != 0) {
    Actor_SetDestinationById(ACTOR_KRADEN, leader->x.part[0].pixel,
                             leader->z.part[0].pixel);
  }
  Actor_WaitForMoveById(ACTOR_KRADEN);
  Actor_SetPosition(ACTOR_KRADEN, 0, 0);
  FieldCamera_FollowActor(gMainRoutineWork.leaderActorId, 1);
  Event_End();
  return;
}

// clang-format off
u32 gEasternAlhafraShip_BriggsRunsScript[] = {
    SC_ANIMATION(2),
    SC_MOVE_BY(32, 0, 0),
    SC_SYNC,
    SC_MOVE_BY(0, 0, 28),
    SC_SYNC,
    SC_MOVE_BY(48, 0, 0),
    SC_SYNC,
    SC_MOVE_BY(12, 0, 12),
    SC_SYNC,
    SC_MOVE(456, 0, 440),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_SET(SW_FACING, FACING_NORTH),
    SC_WAIT(1),
    SC_END,
};

s32 gEasternAlhafraShip_MateRunsScript[] = {
    SC_ANIMATION(2),
    SC_MOVE_BY(32, 0, 0),
    SC_SYNC,
    SC_MOVE_BY(12, 0, -12),
    SC_SYNC,
    SC_MOVE_BY(52, 0, 0),
    SC_SYNC,
    SC_MOVE_BY(12, 0, 12),
    SC_SYNC,
    SC_MOVE(472, 0, 440),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_SET(SW_FACING, FACING_NORTH_NORTHWEST),
    SC_WAIT(1),
    SC_END,
};

u32 gEasternAlhafraShip_LeaveTheDeckScript[] = {
    SC_ANIMATION(2),
    SC_MOVE(472, 0, 264),
    SC_SYNC,
    SC_MOVE(440, 0, 264),
    SC_SYNC,
    SC_RELEASE,
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 456, 0, 392, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {                                                        
      SCENE_EXITS(EasternAlhafraShip),                                                         
      SCENE_EXIT_UNLESS(1, EasternAlhafra, 2, EXIT_FLAG_NONE),                              
      SCENE_EXIT_UNLESS(2, EasternAlhafraShip, 3, EXIT_FLAG_NONE),                              
      SCENE_EXIT_UNLESS(3, EasternAlhafraShip, 2, EXIT_FLAG_NONE),                              
      SCENE_EXIT_UNLESS(4, EasternAlhafraShip, 5, EXIT_FLAG_NONE),                              
      SCENE_EXIT_UNLESS(5, EasternAlhafraShip, 4, EXIT_FLAG_NONE),                             
      SCENE_EXITS_END,                                                         
  };

struct ActorPlacement gEasternAlhafraShip_ActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_BRIGGS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 360, 0, 396, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_CHAMPA_CREWMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 360, 0, 440, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_CHAUCHA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 376, 0, 88, FACING_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_EOLEO, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 136, FACING_WEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FLOWER_VASE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 376, 0, 128, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(287, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 376, 0, 648, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STACKED_CRATES, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 149, 0, 475, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(489, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 149, 0, 472, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(24, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(20, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(19, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MADRAN_SOLDIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_SOLDIER_IN_SPIKED_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_SOLDIER_IN_SPIKED_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_CHAMPA_CREWMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_CHAMPA_CREWMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_CHAMPA_CREWMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SHEBA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};
// clang-format on

struct Trigger gSceneEvents[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_STEP, 20, FLAG_MAYOR_ARRIVED_AT_THE_SHIP,
     EVENT(EasternAlhafraShip_MayorArrives)},
    {TRIGGER_TALK, ACTOR_EOLEO, CONDITION_ALWAYS,
     EVENT(EasternAlhafraShip_EoleoLifts)},
    {TRIGGER_TALK, ACTOR_CHAUCHA, CONDITION_ALWAYS, MSGNO(MsgEoleoAndI)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_EOLEO, CONDITION_ALWAYS,
     MSGNO(MsgIMGonna)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_CHAUCHA, CONDITION_ALWAYS,
     MSGNO(MsgIfEoleoGets)},
    {TRIGGER_TALK, ACTOR_LARGE_BREAD, CONDITION_ALWAYS,
     EVENT(EasternAlhafraShip_TakeTheBread)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST),
     ACTOR_CRATE, CONDITION_ALWAYS, EVENT(EasternAlhafraShip_StartMovingCrate)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_CRATE, CONDITION_ALWAYS,
     EVENT(EasternAlhafraShip_FinishMovingCrate)},
    {TRIGGER_PUSH, 30, CONDITION_ALWAYS,
     EVENT(EasternAlhafraShip_StartMovingCrate)},
    {TRIGGER_PUSHED, 30, CONDITION_ALWAYS,
     EVENT(EasternAlhafraShip_FinishMovingCrate)},
    {PSYNERGY_ON_ACTOR(ABILITY_TREMOR), ACTOR_STACKED_CRATES,
     FLAG_BREAD_SHAKEN_DOWN, EVENT(EasternAlhafraShip_ShakeDownTheBread)},
    {TRIGGER_TABLE_END},
};

u32 gEasternAlhafraShip_NoteScript[] = {
    SC_RELEASE,
};

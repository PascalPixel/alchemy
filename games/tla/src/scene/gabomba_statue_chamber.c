/// @file
/// The innermost rooms of the Gabomba Statue at Kibombo, three in this
/// overlay: the small idol's chamber in the statue's head, the corridor with
/// the lift, and the final chamber where the Great Gabomba speaks.
///
/// In the small idol's chamber a groove runs from the idol at the north, down
/// the statue's tongue between two pink drums. A ladder in the west alcove
/// leads down to the room of pinions, and one in the east up to the room of
/// the arrow panels behind the eyes. Once the ceremony's charges reach the
/// statue it swallows the black orb, and the party comes in to find it gone
/// (GabombaStatueChamber_WhereIsTheOrb): they remember it rolling up the
/// groove into the hole under the idol, and Akafubu comes up the tongue behind
/// them. When the party has told him where the orb went, he sets the Dragon's
/// Eye in the idol, which slides east to open a passage north
/// (GabombaStatueChamber_AkafubuOffersToHelp). The passage leads to a long
/// corridor whose lift carries Felix down a shaft
/// (GabombaStatueChamber_RideLiftDown) to the final chamber. There Akafubu
/// waits before a seated idol with the orb in its hands
/// (GabombaStatueChamber_AkafubuReachesChamber). Examining the idol, Felix
/// hears the Great Gabomba name Akafubu witch doctor, give a message for him,
/// open a hole east of the idol and give back the orb
/// (GabombaStatueChamber_GabombaNamesWitchDoctor). On a later visit that
/// hole's ladder leads down to the Gabomba Catacombs.
#include "flags.h"
#include "item_ids.h"
#include "sprite_ids.h"

/// Set once Akafubu, in the final chamber, has said "I do not know..."
#define FLAG_AKAFUBU_REACHED_CHAMBER 0x8fe

#include "battle/battle.h"
#include "field/field_sequence.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/palette_buffers.h"
#include "graphics/glyph.h"
#include "graphics/graphics.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "types.h"
#include "field/scene_tables.h"
#include "gamepak/far_call/system.h"
#include "scene/scene.h"
#include "system/main_routine_work.h"
#include "system/ram_buffer.h"
#include "scene_ids.h"
#include "text/msg_ids.h"
#include "field/field_sprite.h"
#include "field/event_inline.h"
#include "system/frame.h"

void FieldPsynergy_RunHandler(s32 type);

#include "field/field_sequence_event.h"

void Actor_PlaceOnActor(s32 actor, s32 target);

void GabombaStatueChamber_DropIntoHole(s32 x, s32 z);

extern struct ActorPlacement gGabombaStatueChamber_IdolActorPlacements[];
extern struct ActorPlacement gGabombaStatueChamber_WitchDoctorActorPlacements[];
extern struct ActorPlacement gGabombaStatueChamber_Placements[];
extern struct Trigger gGabombaStatueChamber_IdolTriggers[];
extern struct Trigger gGabombaStatueChamber_LiftTriggers[];
extern struct Trigger gGabombaStatueChamber_WitchDoctorTriggers[];
extern struct Trigger gGabombaStatueChamber_Triggers[];
extern struct SceneRegion gGabombaStatueChamber_IdolExitRegions[];

#include "system/io_reg.h"
#include "field/actor.h"
#include "system/input.h"
#include "version.h"
#include "shared/tables.h"
#include "field/actor_script.h"
#include "fixed_math.h"
#include "window/window.h"

/// How far, in pixels, the lift goes down the shaft, a pixel a frame. The lift
/// stays on screen and the map's layers move up past it.
#define LIFT_TRAVEL 352
/// The frames over which the corridor around the lift fades into the shaft's
/// brick walls, or back, a step every four frames.
#define LIFT_BLEND_FRAMES 63
/// The lift's blend weight for its second target at the given step, rising
/// one every four steps; the first target keeps the rest of BLEND_WEIGHT_MAX
/// (BLEND_ALPHA's bottom and top).
#define LIFT_BLEND_BOTTOM(step) (((step) + 1) / 4)
/// Going down, the corridor starts fading this many pixels into the ride.
#define LIFT_DESCENT_BLEND_START 17
/// Coming back up, it has faded back in this many pixels before the top.
#define LIFT_ASCENT_BLEND_END 33
/// The frames the Black Crystal rises over the idol, half a pixel a frame, as
/// the Great Gabomba begins, and the frame Akafubu startles at it.
#define CRYSTAL_RISE_FRAMES 80
#define AKAFUBU_STARTLED_FRAME 20
/// The stone door halves in the final chamber's six alcoves, three to each,
/// along the path up to the idol.
#define STONE_DOOR_HALVES 18
/// Set once the lift has carried Felix, while he stays on it, so that it does
/// not set off again until he steps off its south edge at the top.
#define FLAG_LEADER_ON_LIFT FLAG_SCENE_TEMP_06
/// Set once Felix has ridden the lift down to the foot of the shaft.
#define FLAG_LIFT_DOWN FLAG_SCENE_TEMP_07

/// The small idol's chamber (SceneId_GabombaStatueChamberIdol): the seated
/// idol, and Kraden ("It vanished into that hole!") and Akafubu ("Who... Who
/// are you!?"); Piers's, Sheba's and Jenna's rows take their own numbers.
enum {
  ACTOR_SEATED_IDOL = ACTOR_FIRST_PLACED,
  /// The two pink drums either side of the groove where the tongue begins,
  /// which turn as the black orb rolls between them.
  ACTOR_LEFT_DRUM,
  ACTOR_RIGHT_DRUM,
  ACTOR_KRADEN,
  ACTOR_AKAFUBU,
  /// The black orb rolling up the groove into the hole, as the party
  /// remembers it, drawn as the Black Crystal's item icon.
  ACTOR_BLACK_CRYSTAL
};

/// The final chamber, where the Great Gabomba names the witch doctor: Kraden,
/// Akafubu ("I have followed Oeia's instructions and entered the final
/// chamber!"), the chest, and Akafubu again in the witch doctor's dress.
enum {
  ACTOR_CHAMBER_KRADEN = ACTOR_FIRST_PLACED + 1,
  ACTOR_CHAMBER_AKAFUBU,
  ACTOR_CHAMBER_CHEST = ACTOR_FIRST_PLACED + 4,
  /// Unseen until the Great Gabomba's flash, when it turns white over the
  /// floor where the second hole opens, hiding the change. Its "?" earlier in
  /// the scene is shown while it is still off the map, where nobody sees it.
  ACTOR_HOLE_COVER,
  ACTOR_WITCH_DOCTOR,
  ACTOR_FIRST_DOOR_HALF ///< the first of the STONE_DOOR_HALVES
};

/// The cell behaviors of the trigger tables. A hole's cells take the number
/// of the exit its ladder leaves by, and a doorway's that of its exit.
enum GabombaStatueChamberCellBehavior {
  /// The lift, at the north end of the corridor: cells (18, 11) to (22, 11).
  CELL_BEHAVIOR_LIFT = 3,
  /// The corridor just south of the lift: cells (19, 12) to (21, 12).
  CELL_BEHAVIOR_SOUTH_OF_LIFT,
  /// The final chamber's path a few steps south of where Akafubu stands:
  /// cells (23, 11) to (25, 11).
  CELL_BEHAVIOR_BELOW_AKAFUBU = 10,
  /// The final chamber's seated idol, holding the black orb: cell (24, 5).
  CELL_BEHAVIOR_IDOL
};

void GabombaStatueChamber_OpenSecondHole(void);

void GabombaStatueChamber_KeepItemWithStatue();
void GabombaStatueChamber_InitializeIdol(void);
void GabombaStatueChamber_InitializeLift(void);

void GabombaStatueChamber_InitializeWitchDoctor(void);

extern s32 gGabombaStatueChamber_MirroredScript[];

void ActorSystem_SuspendProjectionAndShowSprites(void);

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) {
  s16 scene = gMainRoutineWork.next;

  if (scene == SCENE_GABOMBA_STATUE_CHAMBER_IDOL)
    return gGabombaStatueChamber_IdolExitRegions;
  return 0;
}

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  s16 scene = gMainRoutineWork.next;

  if (scene == SCENE_GABOMBA_STATUE_CHAMBER_IDOL)
    return gGabombaStatueChamber_IdolActorPlacements;
  if (scene == SCENE_GABOMBA_STATUE_CHAMBER_WITCH_DOCTOR)
    return gGabombaStatueChamber_WitchDoctorActorPlacements;
  return gGabombaStatueChamber_Placements;
}

const struct Trigger *Scene_GetTriggers(void) {
  s16 scene = gMainRoutineWork.next;

  if (scene == SCENE_GABOMBA_STATUE_CHAMBER_IDOL)
    return gGabombaStatueChamber_IdolTriggers;
  if (scene == SCENE_GABOMBA_STATUE_CHAMBER_LIFT)
    return gGabombaStatueChamber_LiftTriggers;
  if (scene == SCENE_GABOMBA_STATUE_CHAMBER_WITCH_DOCTOR)
    return gGabombaStatueChamber_WitchDoctorTriggers;
  return gGabombaStatueChamber_Triggers;
}

/// The Great Gabomba speaks a line through the seated idol: the idol glows
/// gold, and its words appear in white across the middle of the screen, with
/// no window.
void GabombaStatueChamber_StatueSpeaks(void) {
  volatile u16 *color = (volatile u16 *)&CENTERED_TEXT_COLOR;
  struct FieldWork *work;

  Actor_SetAnimationById(ACTOR_SEATED_IDOL, 2);
  PaletteFade_ApplySource(PALETTE_TINT_NONE, 2);
  PaletteFade_SetTarget(PALETTE_TINT_WARM, 2);
  PaletteFade_StartInterpolation(20);
  wait(20);
  *color = RGB_WHITE;
  wait(1);
  work = Ram_HeapSlots->fieldWork;
  FieldMessage_ShowCentered((s16)work->message++, 0, 2);
  PaletteFade_SetTarget(PALETTE_TINT_NONE, 2);
  PaletteFade_StartInterpolation(20);
  wait(20);
  Actor_SetAnimationById(ACTOR_SEATED_IDOL, 1);
}

/// The item shown on the idol, while it is there: the Dragon's Eye in the
/// small idol's chamber, the black orb in the final chamber.
extern struct Actor *gGabombaStatueChamber_ShownItem;
extern u32 gGabombaStatueChamber_SpinScript[];
extern s32 gGabombaStatueChamber_ChestDescendsScript[];
extern u32 gGabombaStatueChamber_ChestRisesScript[];
extern u32 gGabombaStatueChamber_PartyLeavesScript[];
void ActorSystem_ResumeProjectionAndHideLayers(void);

/// Felix examines the seated idol in the final chamber (cell (24, 5)), the
/// black orb in its hands, and the Great Gabomba speaks: the room darkens,
/// the orb rises over the idol, and the voice names Akafubu witch doctor. He
/// changes into feathers and gown and hurries off south, too soon for the
/// gift, a chest, that sinks before the idol. The Gabomba gives Felix its
/// message for Akafubu, opens a hole with a ladder east of the idol in a flash
/// of white, and gives him the orb. The screen goes dark, and Felix tells
/// Akafubu the message in Kibombo by dawn.
///
/// The Black Crystal is the item the party calls the black orb. The chest is
/// the Gabomba's magic, kept back until Akafubu earns it; it rises away again
/// as the voice says it will pass the magic on to no one yet. The second hole
/// is GabombaStatueChamber_OpenSecondHole, drawn while `ACTOR_HOLE_COVER`
/// whitens its cell. Kraden's two yes-or-no questions only choose his replies.
void GabombaStatueChamber_GabombaNamesWitchDoctor(void) {
  struct Actor *actor, *leader;
  u32 i;

  Event_Start();
  Event_ResolvePendingActions(0);
  Sound_Play(MUS_JUPITER_LIGHTHOUSE);
  Sound_Play(SE_POWER_UP);
  Actor_SetAnimationById(ACTOR_SEATED_IDOL, 3);
  PaletteFade_ApplySource(PALETTE_TINT_NONE, 2);
  PaletteFade_ApplyTarget(PALETTE_TINT_NONE, 2);
  PaletteFade_SetSource(PALETTE_TINT_NONE, 1);
  PaletteFade_SetTarget(PALETTE_TINT_BLEND | RGB(8, 8, 12), 1);
  PaletteFade_StartInterpolation(40);
  wait(40);
  Sound_Play(SE_GABOMBA_VOICE);
  wait(1);
  if (gGabombaStatueChamber_ShownItem != 0) {
    for (i = 0; i < CRYSTAL_RISE_FRAMES; i++) {
      if (i == AKAFUBU_STARTLED_FRAME)
        Actor_ShowEmote(ACTOR_CHAMBER_AKAFUBU,
                        EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
      gGabombaStatueChamber_ShownItem->y.fixed += toFIXED(1) / 2;
      wait(1);
    }
    gGabombaStatueChamber_ShownItem->motionFlags = 4;
  }
  Actor_SetAnimationById(ACTOR_SEATED_IDOL, 1);
  Actor_SetSpeed(gMainRoutineWork.leaderActorId, 0.8);
  Actor_WalkToAndWaitById(gMainRoutineWork.leaderActorId, 358, 108);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_NORTHEAST, 0);
  Actor_PlaceOnActor(ACTOR_CHAMBER_KRADEN, gMainRoutineWork.leaderActorId);
  wait(1);
  Actor_SetSpeed(ACTOR_CHAMBER_KRADEN, 0.8);
  Actor_WalkToAndWait(ACTOR_CHAMBER_KRADEN, 364, 124);
  Actor_FaceDirection(ACTOR_CHAMBER_KRADEN, FACING_NORTH_NORTHEAST, 0);
  Event_SetMessageId(MsgOhYoungWitch);
  GabombaStatueChamber_StatueSpeaks();
  Actor_FaceDirectionById(ACTOR_CHAMBER_AKAFUBU, FACING_EAST, 20);
  Actor_FaceDirection(ACTOR_CHAMBER_AKAFUBU, FACING_WEST, 40);
  Actor_FaceDirectionById(ACTOR_CHAMBER_AKAFUBU, FACING_EAST, 20);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_CHAMBER_AKAFUBU, 0);
  Actor_SetAttachedEffect(ACTOR_CHAMBER_KRADEN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_ShowMessageAndWait(ACTOR_CHAMBER_KRADEN, 0);
  Actor_FaceDirectionAndWait(ACTOR_CHAMBER_AKAFUBU, FACING_NORTH_NORTHWEST);
  Actor_Jump(ACTOR_CHAMBER_AKAFUBU, 4, 20);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_CHAMBER_AKAFUBU, 0);
  GabombaStatueChamber_StatueSpeaks();
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_CHAMBER_KRADEN, FACING_NORTH_NORTHWEST, 40);
  GabombaStatueChamber_StatueSpeaks();
  Actor_ShowEmote(gMainRoutineWork.leaderActorId,
                  EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_CHAMBER_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_CHAMBER_KRADEN, FACING_SOUTH_SOUTHEAST, 0);
  Actor_SetAnimationById(ACTOR_SEATED_IDOL, 3);
  Actor_FaceDirectionById(ACTOR_CHAMBER_AKAFUBU, FACING_SOUTH_SOUTHEAST, 20);
  actor = Actor_Get(ACTOR_WITCH_DOCTOR);
  actor->facing = Actor_Get(ACTOR_CHAMBER_AKAFUBU)->facing;
  wait(1);
  Actor_PlaceOnActor(ACTOR_WITCH_DOCTOR, ACTOR_CHAMBER_AKAFUBU);
  Actor_SetPosition(ACTOR_CHAMBER_AKAFUBU, 0, 0);
  Sound_Play(SE_CHIME);
  Actor_SetAnimationById(ACTOR_WITCH_DOCTOR, 5);
  Event_Wait(60);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_WITCH_DOCTOR, FACING_NORTH_NORTHWEST, 0);
  Actor_SetAnimationById(ACTOR_SEATED_IDOL, 1);
  Event_Wait(40);
  Actor_SetAnimationById(ACTOR_SEATED_IDOL, 3);
  Sound_Play(SE_CHIME);
  Actor_SetAnimationById(ACTOR_WITCH_DOCTOR, 6);
  Event_Wait(60);
  Actor_SetAnimationById(ACTOR_WITCH_DOCTOR, ANIM_STAND);
  Event_Wait(20);
  Actor_SetAnimationById(ACTOR_SEATED_IDOL, 1);
  Event_Wait(40);
  GabombaStatueChamber_StatueSpeaks();
  Actor_Jump(ACTOR_WITCH_DOCTOR, 2, 10);
  Actor_Jump(ACTOR_WITCH_DOCTOR, 4, 10);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_WITCH_DOCTOR, 0);
  Actor_SetMovementTypeAndWaitById(ACTOR_WITCH_DOCTOR,
                                   gGabombaStatueChamber_SpinScript);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_WITCH_DOCTOR, 0);
  Actor_SetSpeed(ACTOR_WITCH_DOCTOR, 1.6);
  Actor_WalkToAndWait(ACTOR_WITCH_DOCTOR, 392, 164);
  Actor_ShowEmoteById(ACTOR_WITCH_DOCTOR, EMOTE_IN_FRONT | EMOTE_SMILE, 0);
  Actor_Jump(ACTOR_WITCH_DOCTOR, 6, 20);
  Event_ShowMessageAndWait(
      SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_ABOVE | ACTOR_WITCH_DOCTOR, 0);
  Actor_WalkToAndWait(ACTOR_WITCH_DOCTOR, 392, 220);
  Actor_SetPosition(ACTOR_WITCH_DOCTOR, 0, 0);
  Actor_FaceDirectionById(gMainRoutineWork.leaderActorId, FACING_EAST, 0);
  Actor_FaceDirectionById(ACTOR_CHAMBER_KRADEN, FACING_EAST, 0);
  Actor_Get(ACTOR_CHAMBER_CHEST)->motionFlags = 0;
  Actor_SetMovementTypeAndWaitById(ACTOR_CHAMBER_CHEST,
                                   gGabombaStatueChamber_ChestDescendsScript);
  Event_Wait(20);
  GabombaStatueChamber_StatueSpeaks();
  Actor_ShowEmote(gMainRoutineWork.leaderActorId,
                  EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_CHAMBER_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_CHAMBER_KRADEN, FACING_NORTH_NORTHEAST, 80);
  GabombaStatueChamber_StatueSpeaks();
  Event_Wait(20);
  GabombaStatueChamber_StatueSpeaks();
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_SOUTHEAST, 20);
  Actor_FaceDirectionById(gMainRoutineWork.leaderActorId, FACING_WEST, 40);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_SOUTHEAST, 20);
  Actor_SetAnimationById(ACTOR_SEATED_IDOL, 3);
  PaletteFade_ApplySource(PALETTE_TINT_NONE, 2);
  PaletteFade_SetTarget(PALETTE_TINT_WARM, 2);
  {
    volatile u16 *color = (volatile u16 *)&CENTERED_TEXT_COLOR;

    PaletteFade_StartInterpolation(10);
    wait(10);
    *color = RGB_WHITE;
    wait(1);
  }
  FieldMessage_ShowCentered((s16)(gFieldWork->message)++, 0, 2);
  PaletteFade_ApplyTarget(PALETTE_TINT_NONE, 2);
  PaletteFade_StartInterpolation(20);
  wait(20);
  Actor_SetAnimationById(ACTOR_SEATED_IDOL, 1);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_NORTHEAST, 0);
  Actor_ShowEmoteById(gMainRoutineWork.leaderActorId,
                      EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_CHAMBER_KRADEN, FACING_NORTH_NORTHWEST, 20);
  GabombaStatueChamber_StatueSpeaks();
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_CHAMBER_KRADEN, FACING_NORTH_NORTHEAST, 0);
  GabombaStatueChamber_StatueSpeaks();
  Actor_ShowEmote(gMainRoutineWork.leaderActorId,
                  EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_CHAMBER_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  GabombaStatueChamber_StatueSpeaks();
  Actor_FaceDirectionById(gMainRoutineWork.leaderActorId, FACING_EAST, 0);
  Actor_FaceDirectionById(ACTOR_CHAMBER_KRADEN, FACING_EAST, 0);
  Actor_SetMovementTypeAndWaitById(ACTOR_CHAMBER_CHEST,
                                   gGabombaStatueChamber_ChestRisesScript);
  GabombaStatueChamber_StatueSpeaks();
  Actor_SetAttachedEffect(gMainRoutineWork.leaderActorId,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(ACTOR_CHAMBER_KRADEN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_CHAMBER_KRADEN, FACING_NORTH_NORTHEAST, 20);
  GabombaStatueChamber_StatueSpeaks();
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_CHAMBER_KRADEN, FACING_NORTH_NORTHWEST, 20);
  GabombaStatueChamber_StatueSpeaks();
  Actor_FaceDirectionAndWait(ACTOR_CHAMBER_KRADEN, FACING_NORTH_NORTHEAST);
  Event_ShowMessageAndWait(ACTOR_CHAMBER_KRADEN, 0);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_NORTHEAST, 0);
  Actor_ShowEmoteById(ACTOR_HOLE_COVER, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  GabombaStatueChamber_StatueSpeaks();
  Event_ShowMessageThenWait(ACTOR_CHAMBER_KRADEN, 0, 40);
  GabombaStatueChamber_StatueSpeaks();
  Actor_Jump(ACTOR_CHAMBER_KRADEN, 4, 10);
  Event_ShowMessageAndWait(ACTOR_CHAMBER_KRADEN, 0);
  GabombaStatueChamber_StatueSpeaks();
  Actor_FaceDirectionById(gMainRoutineWork.leaderActorId, FACING_SOUTH, 0);
  Actor_FaceDirectionAndWait(ACTOR_CHAMBER_KRADEN, FACING_NORTH_NORTHWEST);
  Actor_SetAnimationAndWait(ACTOR_CHAMBER_KRADEN, ANIM_NOD);
  GabombaStatueChamber_StatueSpeaks();
  Actor_FaceDirectionById(gMainRoutineWork.leaderActorId, FACING_NORTHEAST, 0);
  Actor_FaceDirectionAndWait(ACTOR_CHAMBER_KRADEN, FACING_NORTH_NORTHEAST);
  Actor_SetAnimationById(ACTOR_CHAMBER_KRADEN, ANIM_SHAKE_HEAD);
  Event_ShowMessageAndWait(ACTOR_CHAMBER_KRADEN, 0);
  GabombaStatueChamber_StatueSpeaks();
  Sound_Play(MUS_SILENCE);
  Sound_Play(SE_SHIMMER);
  Actor_SetAnimationById(ACTOR_SEATED_IDOL, 3);
  Actor_SetSpriteColor(((s32)Actor_GetById(ACTOR_HOLE_COVER)),
                       SPRITE_COLOR_WHITE);
  Actor_SetPosition(ACTOR_HOLE_COVER, 424, 104);
  PaletteFade_SetTarget(RGB_WHITE, 0);
  PaletteFade_StartInterpolation(40);
  wait(40);
  GabombaStatueChamber_OpenSecondHole();
  Actor_SetPosition(ACTOR_HOLE_COVER, 0, 0);
  Event_Wait(1);
  PaletteFade_ApplyTarget(PALETTE_TINT_NONE, 2);
  PaletteFade_SetTarget(PALETTE_TINT_BLEND | RGB(8, 8, 12), 1);
  PaletteFade_StartInterpolation(60);
  wait(60);
  GabombaStatueChamber_StatueSpeaks();
  PaletteFade_SetTarget(PALETTE_TINT_NONE, 1);
  PaletteFade_StartInterpolation(20);
  wait(20);
  gGabombaStatueChamber_ShownItem->motionFlags = 3;
  Event_Wait(10);
  Sound_Play(SE_STAR_BOUNCE);
  Event_Wait(40);
  Sound_Play(MUS_INSIDE_THE_GABOMBA_STATUE);
  Event_Wait(40);
  Actor_FaceDirectionById(gMainRoutineWork.leaderActorId, FACING_SOUTHEAST, 0);
  Actor_FaceDirectionAndWait(ACTOR_CHAMBER_KRADEN, FACING_NORTH_NORTHWEST);
  Event_ShowMessageAndWait(ACTOR_CHAMBER_KRADEN, 0);
  Actor_SetAnimationAndWait(gMainRoutineWork.leaderActorId, ANIM_NOD);
  Actor_FaceDirection(ACTOR_CHAMBER_KRADEN, FACING_NORTH_NORTHEAST, 0);
  Actor_WalkToAndWaitById(gMainRoutineWork.leaderActorId, 392, 104);
  Actor_FaceDirectionAndRefreshById(gMainRoutineWork.leaderActorId,
                                    FACING_NORTH);
  Actor_SetAnimationById(gMainRoutineWork.leaderActorId,
                         ANIM_LEADER_ARMS_RAISED);
  if (gGabombaStatueChamber_ShownItem != 0) {
    leader = Actor_Get(gMainRoutineWork.leaderActorId);
    actor = gGabombaStatueChamber_ShownItem;
    actor->motionFlags = 0;
    Actor_SetMoveTarget((s32)actor, leader->x.fixed,
                        leader->y.fixed + toFIXED(36), leader->z.fixed);
    Actor_WaitForMove((s32)gGabombaStatueChamber_ShownItem);
  }
  PartyInventory_GiveItem(ITEM_BLACK_CRYSTAL, 0);
  if (gGabombaStatueChamber_ShownItem != 0)
    Actor_Release((s32)gGabombaStatueChamber_ShownItem);
  Actor_SetAnimationById(gMainRoutineWork.leaderActorId, ANIM_STAND);
  Actor_FaceDirectionById(gMainRoutineWork.leaderActorId, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_CHAMBER_KRADEN, FACING_NORTH_NORTHEAST, 40);
  FieldMessage_ShowAtActor(ACTOR_CHAMBER_KRADEN, 0);
  if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.leaderActorId, 0) ==
      0) {
    Actor_SetAnimationById(ACTOR_CHAMBER_KRADEN, ANIM_SHAKE_HEAD);
    Event_ShowMessageAndWait(ACTOR_CHAMBER_KRADEN, 0);
    gFieldWork->message += 1;
  } else {
    gFieldWork->message += 1;
    Actor_SetAnimationById(ACTOR_CHAMBER_KRADEN, ANIM_NOD);
    Event_ShowMessageAndWait(ACTOR_CHAMBER_KRADEN, 0);
  }
  Actor_WalkToAndWaitById(ACTOR_CHAMBER_KRADEN, 376, 124);
  Event_Wait(20);
  Event_ShowMessageAndWait(ACTOR_CHAMBER_KRADEN, 0);
  Actor_FaceDirectionAndWait(ACTOR_CHAMBER_KRADEN, FACING_NORTH_NORTHEAST);
  FieldMessage_ShowAtActor(ACTOR_CHAMBER_KRADEN, 0);
  if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.leaderActorId, 0) ==
      0) {
    Event_ShowMessageAndWait(ACTOR_CHAMBER_KRADEN, 0);
    gFieldWork->message += 1;
  } else {
    gFieldWork->message += 1;
    Event_ShowMessageAndWait(ACTOR_CHAMBER_KRADEN, 0);
  }
  Actor_SetAnimationById(ACTOR_CHAMBER_KRADEN, ANIM_SHAKE_HEAD);
  Event_ShowMessageAndWait(ACTOR_CHAMBER_KRADEN, 0);
  Actor_SetAnimationAndWait(gMainRoutineWork.leaderActorId, ANIM_NOD);
  Event_ShowMessageAndWait(ACTOR_CHAMBER_KRADEN, 0);
  Actor_SetAnimationAndWait(ACTOR_CHAMBER_KRADEN, ANIM_NOD);
  Event_ShowMessageAndWait(ACTOR_CHAMBER_KRADEN, 0);
  Event_ShowMessageAndWait(ACTOR_CHAMBER_KRADEN, 0);
  Actor_SetAnimationAndWait(gMainRoutineWork.leaderActorId, ANIM_NOD);
  Actor_SetMovementTypeById(ACTOR_CHAMBER_KRADEN,
                            gGabombaStatueChamber_PartyLeavesScript);
  Event_Wait(20);
  Actor_SetMovementTypeById(gMainRoutineWork.leaderActorId,
                            gGabombaStatueChamber_PartyLeavesScript);
  Event_Wait(40);
  PaletteFade_SetTarget(0, 0);
  PaletteFade_StartInterpolation(60);
  wait(64);
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 1);
  gFieldWork->transitionFrames = 10;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  PaletteFade_SetTarget(PALETTE_TINT_NONE, 0);
  gFieldWork->transitionFrames = 1;
  PaletteFade_StartInterpolation(1);
  ActorSystem_SuspendProjectionAndShowSprites();
  FieldMessage_ShowCentered(MsgCarriedTheMessage, 0, 3);
  ActorSystem_ResumeProjectionAndHideLayers();
  GameFlag_Set(FLAG_GABOMBA_MESSAGE_CARRIED);
  GameFlag_Clear(FLAG_KIBOMBO_AT_CEREMONY);
  GameFlag_Clear(FLAG_GABOMBA_CEREMONY_UNDER_WAY);
  gMainRoutineWork.retreatScene = SCENE_KIBOMBO;
  gMainRoutineWork.retreatEntrance = 1;
  gMainRoutineWork.restartScene = SCENE_KIBOMBO_HOUSES;
  gMainRoutineWork.restartEntrance = 6;
  Import_EventSetExitRequest(
      GABOMBA_STATUE_CHAMBER_WITCH_DOCTOR_TO_KIBOMBO_HOUSES);
}

/// Felix walks to the hole at (x, z), in pixels, turns north and climbs down
/// its ladder out of sight, and the screen goes dark. He leaves by the exit
/// whose number the hole's cells hold.
void GabombaStatueChamber_DropIntoHole(s32 x, s32 z) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  struct Actor *actor = Actor_GetById(gMainRoutineWork.leaderActorId);

  Event_Start();
  Event_ResolvePendingActions(0);
  actor->motionFlags = 0;
  Actor_SetSpeed(gMainRoutineWork.leaderActorId, 1);
  Actor_WalkToAndWaitById(gMainRoutineWork.leaderActorId, x, z);
  Actor_FaceDirectionById(gMainRoutineWork.leaderActorId, FACING_NORTH, 0);
  Actor_WaitForScriptEndById(gMainRoutineWork.leaderActorId);
  Actor_SetSpriteFlags(Actor_GetById(gMainRoutineWork.leaderActorId), 0);
  Actor_SetAnimationById(gMainRoutineWork.leaderActorId,
                         ANIM_LEADER_CLIMB_DOWN_FROM_TOP);
  FieldActor_SetMoveTarget(actor, x, -12, z);
  Actor_WaitForMove(actor);
  Actor_SetAnimationById(gMainRoutineWork.leaderActorId, ANIM_LEADER_CLIMB);
  Sound_Play(SE_MAP_EXIT);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(work->stepTrigger);
}

/// Felix walks north into the hole in the small idol's chamber, in the narrow
/// alcove west of the idol (cell (19, 10)), and climbs down its ladder to the
/// room of pinions below.
void GabombaStatueChamber_StepIntoFirstHole(void) {
  GabombaStatueChamber_DropIntoHole(312, 144);
}

/// Felix walks into the hole the Great Gabomba opened east of the final
/// chamber's idol (cell (26, 6)) and climbs down its ladder to the Gabomba
/// Catacombs. The four cells beside the hole answer only while he presses
/// toward it, so that he can walk round it without falling in.
void GabombaStatueChamber_StepIntoSecondHole(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  s32 x = FIELD_CELL(leader->x.fixed);
  s32 z = FIELD_CELL(leader->z.fixed);

  if (x == 26) {
    if (z == 5 && (gInput.held & KEY_DOWN) == 0)
      return;
    if (z == 7 && (gInput.held & KEY_UP) == 0)
      return;
  } else if (z == 6) {
    if (x == 25 && (gInput.held & KEY_RIGHT) == 0)
      return;
    if (x == 27 && (gInput.held & KEY_LEFT) == 0)
      return;
  }
  GabombaStatueChamber_DropIntoHole(424, 100);
}

void BlendScript_Suspend(void);

/// Coming back from the final chamber, Felix arrives at the foot of the lift
/// shaft, steps south onto the lift, and it carries him up to the top, the
/// brick walls sliding down past it until the corridor fades back in around
/// him. It stops at the north end of the corridor (cell (20, 11)) that leads
/// south to the small idol's chamber.
///
/// The ride is the descent played backwards: the map's layers start
/// `LIFT_TRAVEL` pixels on and come back a pixel a frame.
void GabombaStatueChamber_RideLiftBackUp(void) {
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  struct MapLayer *lower = &map->layers[0];
  struct MapLayer *upper = &map->layers[2];
  s32 i, step;

  Event_Start();
  Event_ResolvePendingActions(0);
  BlendScript_Suspend();
  step = 1;
  for (i = 0; i < LIFT_TRAVEL; i++) {
    if (i >= LIFT_DESCENT_BLEND_START &&
        i < LIFT_DESCENT_BLEND_START + LIFT_BLEND_FRAMES) {
      s32 blend = LIFT_BLEND_BOTTOM(step) << 8;

      blend |= BLEND_WEIGHT_MAX - LIFT_BLEND_BOTTOM(step);
      REG_BLDALPHA = blend;
      step++;
    }
    lower->offsetY += toFIXED(1);
    upper->offsetY += toFIXED(1);
  }
  MapGrid_CenterOnFollowedPoint();
  wait(1);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Actor_SetSpeed(gMainRoutineWork.leaderActorId, 0.8);
  Actor_WalkToAndWaitById(gMainRoutineWork.leaderActorId, 328, 184);
  Event_Wait(20);
  Sound_Play(SE_MATERIALIZE);
  Actor_SetSpritePriority(gMainRoutineWork.leaderActorId, 2);
  for (; i != 0; i--) {
    if (i >= LIFT_ASCENT_BLEND_END &&
        i < LIFT_ASCENT_BLEND_END + LIFT_BLEND_FRAMES) {
      s32 blend = LIFT_BLEND_BOTTOM(step) << 8;

      blend |= BLEND_WEIGHT_MAX - LIFT_BLEND_BOTTOM(step);
      REG_BLDALPHA = blend;
      step--;
    }
    lower->offsetY += -toFIXED(1);
    upper->offsetY += -toFIXED(1);
    wait(1);
  }
  Actor_SetSpritePriority(gMainRoutineWork.leaderActorId, 1);
  Sound_Play(SE_STOP_EXIT_SE);
  wait(1);
  Sound_Play(SE_CHEST_CLOSE);
  Event_Wait(20);
  GameFlag_Set(FLAG_LEADER_ON_LIFT);
  Event_End();
}

/// Felix steps onto the lift at the north end of the corridor from the small
/// idol's chamber (cells (18, 11) to (22, 11)), turns to face south, and it
/// carries him down the shaft: the corridor fades into brick walls that slide
/// up past him until the lift stops at the foot, where the way on leads north
/// to the final chamber.
///
/// The lift never moves on screen; both map layers scroll `LIFT_TRAVEL`
/// pixels, and Felix is put on the lower floor's layer at the end.
void GabombaStatueChamber_RideLiftDown(void) {
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  struct MapLayer *lower = &map->layers[0];
  struct MapLayer *upper = &map->layers[2];
  s32 i, step;

  Event_Start();
  Event_ResolvePendingActions(0);
  BlendScript_Suspend();
  Actor_SetSpeed(gMainRoutineWork.leaderActorId, 0.8);
  Actor_SetDestinationOffsetById(gMainRoutineWork.leaderActorId, 0, -8);
  Actor_WaitForMoveById(gMainRoutineWork.leaderActorId);
  Actor_FaceDirectionAndRefreshById(gMainRoutineWork.leaderActorId,
                                    FACING_SOUTH);
  Sound_Play(SE_MATERIALIZE);
  step = 1;
  Actor_SetSpritePriority(gMainRoutineWork.leaderActorId, 2);
  for (i = 0; i <= LIFT_TRAVEL - 1; i++) {
    if ((u32)(i - LIFT_DESCENT_BLEND_START) <= LIFT_BLEND_FRAMES - 1) {
      s32 blend = LIFT_BLEND_BOTTOM(step) << 8;

      blend |= BLEND_WEIGHT_MAX - LIFT_BLEND_BOTTOM(step);
      REG_BLDALPHA = blend;
      step++;
    }
    lower->offsetY += toFIXED(1);
    upper->offsetY += toFIXED(1);
    wait(1);
  }
  Actor_SetSpritePriority(gMainRoutineWork.leaderActorId, 1);
  Sound_Play(SE_STOP_EXIT_SE);
  wait(1);
  Sound_Play(SE_CHEST_CLOSE);
  Event_Wait(20);
  Actor_GetById(gMainRoutineWork.leaderActorId)->layer = 1;
  GameFlag_Set(FLAG_LEADER_ON_LIFT);
  GameFlag_Set(FLAG_LIFT_DOWN);
  Event_End();
}

/// Nothing visible happens when Felix steps off the lift at the top onto the
/// corridor south of it (cells (19, 12) to (21, 12)). The game notes that he
/// has left it, so that stepping back on carries him down again.
void GabombaStatueChamber_StepOffLift(void) {
  GameFlag_Clear(FLAG_LEADER_ON_LIFT);
}

/// As Felix walks up the path of the final chamber towards the idol (cells
/// (23, 11) to (25, 11)), the view moves up to Akafubu standing before it,
/// who says he has followed Oeia's instructions to this chamber but does not
/// know what he must do here to become witch doctor. It plays once.
void GabombaStatueChamber_AkafubuReachesChamber(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  FieldCamera_MoveTo(392, CAMERA_KEEP, 120, 1);
  FieldCamera_WaitForArrival();
  Event_Wait(20);
  Event_SetMessageId((s32)MsgIHaveFollowed);
  Actor_SetAttachedEffect(ACTOR_CHAMBER_AKAFUBU,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  // "I have followed Oeia's instructions and entered the final chamber!"
  Event_ShowMessageAndWait(ACTOR_CHAMBER_AKAFUBU, 0);
  Actor_FaceDirectionAndWait(ACTOR_CHAMBER_AKAFUBU, FACING_WEST);
  // "He told me that if I reached this chamber, I would become Kibombo's..."
  Event_ShowMessageAndWait(ACTOR_CHAMBER_AKAFUBU, 0);
  Actor_FaceDirectionAndRefreshById(ACTOR_CHAMBER_AKAFUBU, FACING_EAST);
  // "But what must I do here to become a witch doctor?"
  Event_ShowMessageAndWait(ACTOR_CHAMBER_AKAFUBU, 0);
  Actor_FaceDirectionAndWait(ACTOR_CHAMBER_AKAFUBU, FACING_NORTH_NORTHEAST);
  Actor_SetAnimationAndWait(ACTOR_CHAMBER_AKAFUBU, ANIM_SHAKE_HEAD);
  // "I do not know..."
  Event_ShowMessageAndWait(ACTOR_CHAMBER_AKAFUBU, 0);
  GameFlag_Set(FLAG_AKAFUBU_REACHED_CHAMBER);
  Event_End();
}

/// Each frame, keeps the item shown on the idol with the idol, so that the
/// Dragon's Eye slides aside with it.
void GabombaStatueChamber_KeepItemWithStatue(struct Actor *actor) {
  actor->x.fixed = Actor_GetById(ACTOR_SEATED_IDOL)->x.fixed;
}

/// Shows the Dragon's Eye, its red item icon, set in the front of the small
/// idol, where Akafubu places it.
void GabombaStatueChamber_ShowDragonsEye(void) {
  struct Actor *statue = Actor_GetById(ACTOR_SEATED_IDOL);
  s32 x = statue->x.fixed;
  s32 z = statue->z.fixed;
  struct Actor *item;
  struct Sprite *sprite;
  u8 *icon;
  u8 clear = 0;

  gGabombaStatueChamber_ShownItem = item =
      Import_ActorCreate(SPRITE_UNSEEN_MARKER, x, toFIXED(18), z);
  if (item != NULL) {
    sprite = item->sprite;
    sprite->image.fullColor = 0;
    sprite->image.palette = 0;
    sprite->image.priority = 2;
    item->motionFlags = clear;
    item->retainSpriteCache = 1;
    item->update = GabombaStatueChamber_KeepItemWithStatue;
    sprite->flags = clear;
    sprite->count = clear;
    icon = HeapSlot_Alloc(HEAP_SLOT_GLYPH_TRANSFER, sizeof(GlyphTransfer));
    Icon_DrawItem(ITEM_DRAGONS_EYE);
    VramCache_Load(sprite->slot, ICON_TILE_BYTES,
                   ((GlyphTransfer *)icon)->tiles);
    HeapSlot_Release(HEAP_SLOT_GLYPH_TRANSFER);
  }
}

/// Shows the black orb, the Black Crystal's item icon, resting in the hands of
/// the final chamber's idol.
void GabombaStatueChamber_ShowBlackCrystal(void) {
  struct Actor *statue = Actor_GetById(ACTOR_SEATED_IDOL);
  s32 x = statue->x.fixed;
  s32 z = statue->z.fixed;
  struct Actor *item;
  struct Sprite *sprite;
  u8 *icon;
  u8 clear = 0;

  gGabombaStatueChamber_ShownItem = item =
      Import_ActorCreate(SPRITE_UNSEEN_MARKER, x, toFIXED(18), z);
  if (item != NULL) {
    sprite = item->sprite;
    sprite->image.fullColor = 0;
    sprite->image.palette = 0;
    sprite->image.priority = 2;
    item->motionFlags = clear;
    item->retainSpriteCache = 1;
    item->update = GabombaStatueChamber_KeepItemWithStatue;
    sprite->flags = clear;
    sprite->count = clear;
    icon = HeapSlot_Alloc(HEAP_SLOT_GLYPH_TRANSFER, sizeof(GlyphTransfer));
    Icon_DrawItem(ITEM_BLACK_CRYSTAL);
    VramCache_Load(sprite->slot, ICON_TILE_BYTES,
                   ((GlyphTransfer *)icon)->tiles);
    HeapSlot_Release(HEAP_SLOT_GLYPH_TRANSFER);
  }
}

/// Unrolls the statue's tongue in the map: the orange roll between the two
/// pink drums becomes a tongue running south out of the small idol's chamber,
/// its end (cells (23, 21) to (25, 21)) the way out to the statue grounds.
void GabombaStatueChamber_UnrollTongue(void) {
  MapGrid_CopyCellPictureIndicesRect(85, 78, 22, 79, 5, 11);
  MapGrid_CopyCellAttributes(85, 78, 5, 11, 22, 15);
  MapGrid_CopyCellsTo(86, 30, 86, 20, 5, 2);
  MapGrid_CopyCellPictureIndicesRect(86, 30, 84, 22, 9, 2);
}

/// Moves the small idol's place in the map with it as it slides east: its old
/// cell (24, 9), in front of the passage north, is freed and cell (26, 9)
/// blocked.
void GabombaStatueChamber_UncoverPassage(void) {
  Map_CopyCellAttributeRect(23, 1, 3, 1, 23, 9);
  Map_CopyCellAttributeRect(23, 0, 3, 1, 25, 9);
}

/// Opens the hole east of the final chamber's idol in the map, its ladder the
/// way down to the Gabomba Catacombs: the hole at cell (26, 6) and the four
/// cells around it that lead in.
void GabombaStatueChamber_OpenSecondHole(void) {
  MapGrid_CopyCellPictureIndicesRect(0, 1, 26, 6, 1, 1);
  MapGrid_CopyCellPictureIndicesRect(0, 65, 26, 70, 1, 1);
  MapGrid_CopyCellAttributes(0, 2, 3, 3, 25, 5);
}

extern u32 gGabombaStatueChamber_WalkOffScript[];

/// The party comes into the small idol's chamber after the statue has
/// swallowed the black orb, and Piers asks where it is. In sepia, the view
/// follows the orb as they remember it rolling up the groove between the pink
/// drums and vanishing into the hole under the idol. Sheba sees light beyond
/// the hole, Akafubu comes up the tongue demanding who they are, and Jenna
/// leads him off east with the party to the room of the arrow panels.
///
/// It plays on arriving from the ceremony by
/// `GABOMBA_STATUE_CHAMBER_IDOL_FROM_KIBOMBO_STATUE_GROUNDS_CEREMONY_NORTH`.
/// The question "Are you Kibombo!?" only chooses Akafubu's reply.
void GabombaStatueChamber_WhereIsTheOrb(void) {
  struct Actor *crystal =
      (struct Actor *)((s32)Actor_GetById(ACTOR_BLACK_CRYSTAL));
  struct FieldSprite *sprite;
  u8 *block;
  s32 turnA, turnB;
  s32 leader;

  Event_Start();
  Event_ResolvePendingActions(0);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  FieldActor_SetPositionAndResetMotion(
      ((s32)Actor_GetById(ACTOR_BLACK_CRYSTAL)), toFIXED(392), 0, toFIXED(268));
  wait(1);
  leader = gMainRoutineWork.leaderActorId;
  Actor_SetPositionAndFacing(leader, toFIXED(360), toFIXED(184),
                             turnA = FACING_NORTHEAST);
  Actor_SetPositionAndFacing(ACTOR_PIERS, toFIXED(376), toFIXED(184), turnA);
  Actor_SetPositionAndFacing(ACTOR_KRADEN, toFIXED(360), toFIXED(168), turnA);
  Actor_SetPositionAndFacing(ACTOR_SHEBA, toFIXED(423), toFIXED(168),
                             turnB = FACING_NORTHWEST);
  Actor_SetPositionAndFacing(ACTOR_JENNA, toFIXED(408), toFIXED(184), turnB);
  if (crystal != 0) {
    sprite = crystal->sprite;
    sprite->flags = 0;
    sprite->countAndShadow[0] = 0;
    sprite->fullColor = 0;
    sprite->palette = 0;
    sprite->priority = 1;
    crystal->retainSpriteCache = 1;
    block =
        (u8 *)HeapSlot_Alloc(HEAP_SLOT_GLYPH_TRANSFER, sizeof(GlyphTransfer));
    Icon_DrawItem(ITEM_BLACK_CRYSTAL);
    VramCache_Load(sprite->slot, ICON_TILE_BYTES,
                   (s32)((GlyphTransfer *)block)->tiles);
    HeapSlot_Release(HEAP_SLOT_GLYPH_TRANSFER);
  }
  GabombaStatueChamber_UnrollTongue();
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(40);
  Actor_Jump(ACTOR_PIERS, 4, 10);
  Event_SetMessageId(MsgTheBlackOrb);
  // "The black orb... Where is it?"
  Event_ShowMessageAndWait(ACTOR_PIERS, 0);
  Actor_SetAttachedEffect(ACTOR_SHEBA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_FaceDirectionAndWait(ACTOR_SHEBA, FACING_WEST);
  // "I saw it roll in there..."
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_SHEBA, 0);
  Actor_ShowEmoteById(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 20);
  // "It came in through there and rolled along this groove in the floor..."
  Event_ShowMessageAndWait(ACTOR_JENNA, 0);
  FieldActor_SetPositionAndResetMotion(
      ((s32)Actor_GetById(ACTOR_BLACK_CRYSTAL)), toFIXED(392), 0, toFIXED(340));
  Sound_Play(SOUND_FADE_OUT_BGM);
  PaletteFade_SetTarget(PALETTE_TINT_SEPIA, 0);
  PaletteFade_StartInterpolation(40);
  wait(40);
  Actor_SetAnimationById(ACTOR_LEFT_DRUM, 2);
  Actor_SetAnimationById(ACTOR_RIGHT_DRUM, 2);
  Actor_WatchAndSetScript(gMainRoutineWork.leaderActorId, ACTOR_BLACK_CRYSTAL);
  Actor_WatchAndSetScript(ACTOR_JENNA, ACTOR_BLACK_CRYSTAL);
  Actor_WatchAndSetScript(ACTOR_SHEBA, ACTOR_BLACK_CRYSTAL);
  Actor_WatchAndSetScript(ACTOR_PIERS, ACTOR_BLACK_CRYSTAL);
  Actor_WatchAndSetScript(ACTOR_KRADEN, ACTOR_BLACK_CRYSTAL);
  Event_Wait(10);
  FieldCamera_SetSpeed(0.8);
  FieldCamera_MoveTo(392, CAMERA_KEEP, 246, 1);
  Actor_SetSpeed(ACTOR_BLACK_CRYSTAL, 0.8);
  Actor_MoveToAndWait(ACTOR_BLACK_CRYSTAL, 392, 246);
  Actor_SetAnimationById(ACTOR_LEFT_DRUM, 1);
  Actor_SetAnimationById(ACTOR_RIGHT_DRUM, 1);
  crystal->velocityY = toFIXED(3);
  crystal->gravity = toFIXED(1) / 2;
  Actor_SetSpeed(ACTOR_BLACK_CRYSTAL, 1.6);
  Actor_MoveToAndWait(ACTOR_BLACK_CRYSTAL, 392, 234);
  Event_Wait(10);
  FieldCamera_MoveTo(392, CAMERA_KEEP, 168, 1);
  Actor_SetSpeed(ACTOR_BLACK_CRYSTAL, 0.8);
  Actor_MoveToAndWaitById(ACTOR_BLACK_CRYSTAL, 392, 160);
  Actor_Stop(gMainRoutineWork.leaderActorId);
  Actor_Stop(ACTOR_JENNA);
  Actor_Stop(ACTOR_SHEBA);
  Actor_Stop(ACTOR_PIERS);
  Actor_Stop(ACTOR_KRADEN);
  wait(1);
  Actor_SetPosition(ACTOR_BLACK_CRYSTAL, 0, 0);
  Event_Wait(20);
  PaletteFade_SetTarget(PALETTE_TINT_NONE, 0);
  PaletteFade_StartInterpolation(40);
  wait(40);
  Scene_PlayMusic();
  Actor_ShowEmoteById(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  // "It vanished into that hole!"
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_KRADEN, 0);
  Actor_ShowEmoteById(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 20);
  // "It looks pretty deep..."
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_SHEBA, 0);
  Actor_FaceDirectionAndRefreshById(ACTOR_JENNA, turnA);
  // "Can you see it? Do you think you can reach it?"
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | ACTOR_JENNA, 0);
  Actor_FaceDirectionById(ACTOR_JENNA, FACING_NORTHWEST, 0);
  Actor_SetSpeed(ACTOR_SHEBA, 0.8);
  Actor_WalkToAndWait(ACTOR_SHEBA, 400, 172);
  Actor_SetAnimationById(ACTOR_SHEBA, ANIM_LEADER_LOOK_AROUND);
  Event_Wait(20);
  // "Hm... No, I can't even see it."
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_SHEBA, 0);
  Actor_SetAnimationById(ACTOR_PIERS, ANIM_SHAKE_HEAD);
  // "It's no use... Now, I'll never see Lemuria again... I am lost..."
  Event_ShowMessageAndWait(ACTOR_PIERS, 0);
  // "Wait, hold on... I can see light on the other side of this hole..."
  Event_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | ACTOR_SHEBA, 0);
  Actor_ShowEmoteById(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  // "A light?"
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_KRADEN, 0);
  Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST);
  // "There must be a room on the other side of this statue..."
  Event_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | ACTOR_KRADEN, 0);
  Actor_SetAnimationAndWait(gMainRoutineWork.leaderActorId, ANIM_NOD);
  Sound_Play(MUS_SILENCE);
  {
    s32 turn = FACING_NORTH_NORTHEAST;
    Actor_SetHorizontalPositionAndAngleWithTerrain(ACTOR_AKAFUBU, toFIXED(400),
                                                   toFIXED(320), turn);
  }
  wait(1);
  // "Who... Who are you!?"
  Event_ShowMessageAndWait(ACTOR_AKAFUBU, 0);
  Actor_SetAnimationById(ACTOR_SHEBA, ANIM_STAND);
  wait(1);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST, 0);
  FieldCamera_SetSpeed(1.6);
  FieldCamera_MoveTo(392, CAMERA_KEEP, 200, 1);
  Actor_SetSpeed(ACTOR_AKAFUBU, 1.2);
  Actor_WalkToAndWait(ACTOR_AKAFUBU, 404, 220);
  Actor_Jump(ACTOR_AKAFUBU, 4, 10);
  // "What are you doing here? Are you Kibombo!?" Yes: "What? Do you take me for
  // a naive fool!?" No: "Then what are you doing inside the Great..."
  FieldMessage_ShowAtActor(ACTOR_AKAFUBU, 0);
  if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.leaderActorId, 0) ==
      0) {
    FieldEffect_SetAttached(ACTOR_AKAFUBU,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Event_Wait(20);
    // "What? Do you take me for a naive fool!?"
    Event_ShowMessageAndWait(ACTOR_AKAFUBU, 0);
    gFieldWork->message += 1;
  } else {
    gFieldWork->message += 1;
    Actor_ShowEmote(ACTOR_AKAFUBU, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 20);
    // "Then what are you doing inside the Great Gabomba!?"
    Event_ShowMessageAndWait(ACTOR_AKAFUBU, 0);
  }
  Actor_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_SCOWL, 20);
  // "We've come to reclaim the black orb you stole!"
  Event_ShowMessageAndWait(ACTOR_PIERS, 0);
  Actor_ShowEmoteById(ACTOR_AKAFUBU, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  // "Do you really believe I would permit you to take it!?"
  Event_ShowMessageAndWait(ACTOR_AKAFUBU, 0);
  Actor_ShowEmoteById(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_SCOWL, 20);
  // "Hey, watch it! You wouldn't even be in here if it weren't for us!"
  Event_ShowMessageAndWait(ACTOR_SHEBA, 0);
  FieldEffect_SetAttached(ACTOR_AKAFUBU,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(20);
  // "What is that supposed to mean?"
  Event_ShowMessageAndWait(ACTOR_AKAFUBU, 0);
  Actor_SetAnimationById(gMainRoutineWork.leaderActorId, ANIM_NOD);
  Actor_SetAnimationById(ACTOR_JENNA, ANIM_NOD);
  Actor_SetAnimationById(ACTOR_SHEBA, ANIM_NOD);
  Actor_SetAnimationById(ACTOR_PIERS, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  Actor_SetAnimationById(ACTOR_AKAFUBU, ANIM_SHAKE_HEAD);
  // "Why should I believe you? I am Akafubu, the chosen witch doctor of..."
  Event_ShowMessageAndWait(ACTOR_AKAFUBU, 0);
  Actor_ShowEmoteById(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 20);
  Actor_SetSpeed(ACTOR_JENNA, 0.8);
  Actor_WalkToAndWait(ACTOR_JENNA, 420, 228);
  Actor_FaceDirectionAndRefreshById(ACTOR_JENNA, FACING_NORTHWEST);
  Event_Wait(10);
  Actor_SetSpeed(ACTOR_JENNA, 1.6);
  Actor_WalkToAndWait(ACTOR_JENNA, 412, 222);
  Actor_SetSpeed(ACTOR_JENNA, 1);
  Actor_SetSpeed(ACTOR_AKAFUBU, 1);
  Actor_SetMovementTypeById(ACTOR_JENNA, gGabombaStatueChamber_WalkOffScript);
  Event_Wait(10);
  Actor_SetMovementTypeById(ACTOR_AKAFUBU, gGabombaStatueChamber_WalkOffScript);
  Event_Wait(60);
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  GameFlag_Set(FLAG_KEEP_PARTY_POSITION);
  Import_EventSetExitRequest(
      GABOMBA_STATUE_CHAMBER_IDOL_TO_GABOMBA_STATUE_ARROWS_EAST);
}

extern u32 gGabombaStatueChamber_AkafubuPath[];

/// Back in the small idol's chamber with the party, Akafubu walks up to the
/// idol and sets the Dragon's Eye in it, and the idol slides east, opening a
/// passage north behind it. He says he might be able to retrieve the orb and
/// goes in, and the party talks over whether to trust him before following.
///
/// It plays on arriving from the room of the arrow panels by
/// `GABOMBA_STATUE_CHAMBER_IDOL_FROM_GABOMBA_STATUE_ARROWS_WEST`. Answering No
/// to Jenna's question is counted by GabombaStatueChamber_CountAnswer.
void GabombaStatueChamber_AkafubuOffersToHelp(void) {
  struct HeapSlots *heap;
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  System_Wait(1);
  GabombaStatueChamber_UnrollTongue();
  MapGrid_CenterOnFollowedPoint();
  System_Wait(1);
  Actor_SetPositionAndFacing(gMainRoutineWork.leaderActorId, toFIXED(360),
                             toFIXED(216), FACING_NORTHEAST);
  Actor_SetPositionAndFacing(ACTOR_PIERS, toFIXED(376), toFIXED(216),
                             FACING_NORTHEAST);
  Actor_SetPositionAndFacing(ACTOR_KRADEN, toFIXED(360), toFIXED(200), 0);
  Actor_SetPositionAndFacing(ACTOR_JENNA, toFIXED(408), toFIXED(216),
                             FACING_NORTHWEST);
  Actor_SetPositionAndFacing(ACTOR_SHEBA, toFIXED(424), toFIXED(200),
                             FACING_WEST);
  Actor_SetPositionAndFacing(ACTOR_AKAFUBU, toFIXED(392), toFIXED(200),
                             FACING_NORTH_NORTHWEST);
  heap = Ram_HeapSlots;
  System_Wait(1);
  ((struct FieldWork *)heap->fieldWork)->transition =
      SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_Wait(40);
  Actor_FaceDirection(ACTOR_AKAFUBU, FACING_NORTH_NORTHEAST, 20);
  Actor_FaceDirectionAndWait(ACTOR_AKAFUBU, FACING_SOUTH_SOUTHWEST);
  Actor_ShowEmoteById(ACTOR_AKAFUBU, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 40);
  Actor_FaceDirection(ACTOR_PIERS, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTHWEST, 0);
  Actor_SetSpeed(ACTOR_AKAFUBU, 0.8);
  Actor_WalkToAndWait(ACTOR_AKAFUBU, 392, 168);
  Event_PlaySound(SE_HIDDEN_PASSAGE_OPEN);
  GabombaStatueChamber_ShowDragonsEye();
  FieldEvent_Wait(40);
  Event_PlaySound(SE_PUSH);
  Actor_GetById(ACTOR_SEATED_IDOL)->motionFlags = 0;
  GabombaStatueChamber_UncoverPassage();
  Actor_SetSpeed(ACTOR_SEATED_IDOL, 0.4);
  Actor_MoveToAndWait(ACTOR_SEATED_IDOL, 424, 152);
  Actor_FaceDirectionAndWait(ACTOR_AKAFUBU, FACING_SOUTH_SOUTHWEST);
  FieldEvent_SetMessageId((s32)MsgIMightBe);
  // "I might be able to retrieve the orb."
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_AKAFUBU, 0);
  Actor_SetAnimationAndWait(ACTOR_AKAFUBU, ANIM_NOD);
  Actor_WalkToAndWait(ACTOR_AKAFUBU, 392, 140);
  System_Wait(1);
  Actor_SetPosition(ACTOR_AKAFUBU, 0, 0);
  System_Wait(1);
  Actor_ShowEmoteById(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  Actor_FaceDirection(ACTOR_JENNA, FACING_WEST, 0);
  // "Do you think Akafubu will really return the orb to us?" Yes: "Maybe we had
  // him all wrong..." No: "I think Jenna is right. It sounds like he's..."
  FieldMessage_ShowAtActor(SPEAKER_WINDOW_ABOVE | ACTOR_JENNA, 0);
  if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.leaderActorId, 0) ==
      0) {
    Actor_FaceDirectionAndWait(ACTOR_PIERS, FACING_EAST);
    Actor_SetAnimation(ACTOR_PIERS, ANIM_NOD);
    // "Maybe we had him all wrong..."
    Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_PIERS, 0);
    ((struct FieldWork *)heap->fieldWork)->message++;
  } else {
    ((struct FieldWork *)heap->fieldWork)->message++;
    Actor_FaceDirectionAndWait(ACTOR_PIERS, FACING_WEST);
    // "I think Jenna is right. It sounds like he's going to return what he..."
    Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_PIERS, 0);
    GabombaStatueChamber_CountAnswer();
  }
  Actor_FaceDirection(ACTOR_SHEBA, FACING_WEST, 0);
  Actor_SetAttachedEffect(ACTOR_SHEBA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(20);
  // "I thought Akafubu was an evil man, but I guess we were wrong..."
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_SHEBA, 0);
  Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_EAST);
  Actor_SetAnimation(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
  // "You never can tell, Sheba..."
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_KRADEN, 0);
  Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_NORTH_NORTHEAST);
  // "We have little choice but to trust that he will return it. Come on!"
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_KRADEN, 0);
  Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  // "Let's go, Felix!"
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_KRADEN, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_WEST, 20);
  Actor_SetAnimation(gMainRoutineWork.leaderActorId, ANIM_NOD);
  Actor_SetAnimation(ACTOR_PIERS, ANIM_NOD);
  Actor_SetAnimation(ACTOR_JENNA, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_SHEBA, ANIM_NOD);
  Actor_SetSpeed(ACTOR_KRADEN, 0.8);
  Actor_SetSpeed(ACTOR_PIERS, 0.8);
  Actor_SetSpeed(ACTOR_JENNA, 1);
  Actor_SetSpeed(ACTOR_SHEBA, 1.2);
  {
    s32 path = (s32)gGabombaStatueChamber_AkafubuPath;

    Actor_SetScript(ACTOR_KRADEN, path);
    Actor_SetScript(ACTOR_PIERS, path);
    Actor_SetScript(ACTOR_JENNA, path);
    Actor_RunScriptAndWait(ACTOR_SHEBA, path);
  }
  Event_SetFlag(FLAG_AKAFUBU_OFFERED_HELP);
  Event_End();
}

s32 Scene_Initialize(void) {
  struct FieldWork *work;
  s16 scene;

  work = Ram_HeapSlots->fieldWork;
  work->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
  ((struct Actor *)Actor_GetById(Party_GetLeaderActor()))->priorityFlags |=
      ACTOR_PRIORITY_IGNORES_RISES;
  scene = gMainRoutineWork.next;
  if (scene == SCENE_GABOMBA_STATUE_CHAMBER_IDOL)
    GabombaStatueChamber_InitializeIdol();
  else if (scene == SCENE_GABOMBA_STATUE_CHAMBER_LIFT)
    GabombaStatueChamber_InitializeLift();
  else if (scene == SCENE_GABOMBA_STATUE_CHAMBER_WITCH_DOCTOR)
    GabombaStatueChamber_InitializeWitchDoctor();
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

/// Sets up the small idol's chamber. Once Akafubu has offered his help, the
/// tongue is out and the idol stands aside from the passage, the Dragon's Eye
/// in it; before that, arriving from the ceremony or from the arrow panels
/// plays his two scenes. Coming in by the tongue makes Retreat in the final
/// chamber lead back to it.
void GabombaStatueChamber_InitializeIdol(void) {
  Actor_DisableById(ACTOR_SEATED_IDOL);
  Actor_DisableById(ACTOR_LEFT_DRUM);
  Actor_DisableById(ACTOR_RIGHT_DRUM);
  wait(1);
  if (GameFlag_Get(FLAG_AKAFUBU_OFFERED_HELP)) {
    GabombaStatueChamber_UnrollTongue();
    GabombaStatueChamber_UncoverPassage();
    Actor_SetPosition(ACTOR_SEATED_IDOL, 424, 152);
    wait(1);
    GabombaStatueChamber_ShowDragonsEye();
  } else {
    s32 entrance;

    entrance = gMainRoutineWork.entrance;
    if (entrance ==
        GABOMBA_STATUE_CHAMBER_IDOL_FROM_KIBOMBO_STATUE_GROUNDS_CEREMONY_NORTH) {
      GabombaStatueChamber_WhereIsTheOrb();
      return;
    }
    if (entrance ==
        GABOMBA_STATUE_CHAMBER_IDOL_FROM_GABOMBA_STATUE_ARROWS_WEST) {
      GabombaStatueChamber_AkafubuOffersToHelp();
      return;
    }
  }
  {
    struct MainRoutineWork *party = &gMainRoutineWork;

    if (party->entrance ==
        GABOMBA_STATUE_CHAMBER_IDOL_FROM_KIBOMBO_STATUE_GROUNDS) {
      GameFlag_Clear(FLAG_GABOMBA_RETREAT_TO_PINION);
      GameFlag_Set(FLAG_GABOMBA_RETREAT_TO_IDOL);
    }
  }
}

/// Sets up the corridor with the lift: with the lift at the foot of the shaft
/// if the party left it there, or riding back up if the party arrives from the
/// final chamber.
void GabombaStatueChamber_InitializeLift(void) {
  if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
    if (GameFlag_Get(FLAG_LIFT_DOWN)) {
      struct MapWork *map = Ram_HeapSlots->mapGrid;
      struct MapLayer *front = &map->layers[0];
      struct MapLayer *back = &map->layers[2];
      s32 blend = 1;
      s32 i;

      BlendScript_Suspend();
      for (i = 0; i < LIFT_TRAVEL; i++) {
        if (i >= LIFT_DESCENT_BLEND_START &&
            i <= LIFT_DESCENT_BLEND_START + LIFT_BLEND_FRAMES - 1) {
          u32 alpha = LIFT_BLEND_BOTTOM(blend) << 8;

          alpha |= BLEND_WEIGHT_MAX - LIFT_BLEND_BOTTOM(blend);
          REG_BLDALPHA = alpha;
          blend++;
        }
        front->offsetY += toFIXED(1);
        back->offsetY += toFIXED(1);
      }
      MapGrid_CenterOnFollowedPoint();
      wait(1);
    }
  } else if (
      gMainRoutineWork.entrance ==
      GABOMBA_STATUE_CHAMBER_LIFT_FROM_GABOMBA_STATUE_CHAMBER_WITCH_DOCTOR) {
    GabombaStatueChamber_RideLiftBackUp();
  }
}

/// Sets up the final chamber: once the Gabomba's message is carried, its hole
/// is open and Akafubu gone; before that, the black orb rests in the idol's
/// hands. Climbing up from the Gabomba Catacombs, Retreat leads back to where
/// the party came into the statue: the hall of pinions, or the tongue.
void GabombaStatueChamber_InitializeWitchDoctor(void) {
  u32 i;

  for (i = 0; i <= STONE_DOOR_HALVES - 1; i++)
    Actor_SetSpritePriority(i + ACTOR_FIRST_DOOR_HALF, 2);
  Actor_DisableById(ACTOR_SEATED_IDOL);
  Actor_GetById(ACTOR_SEATED_IDOL)->y.fixed = 0;
  if (GameFlag_Get(FLAG_GABOMBA_MESSAGE_CARRIED)) {
    GabombaStatueChamber_OpenSecondHole();
    Actor_SetPosition(ACTOR_CHAMBER_AKAFUBU, 0, 0);
  } else {
    GabombaStatueChamber_ShowBlackCrystal();
  }
#if TLA_PATCH_1 /* The Japanese edition does not set the retreat. */
  if (gMainRoutineWork.entrance ==
      GABOMBA_STATUE_CHAMBER_WITCH_DOCTOR_FROM_GABOMBA_CATACOMBS_TELEPORTERS) {
    if (GameFlag_Get(FLAG_GABOMBA_RETREAT_TO_PINION)) {
      gMainRoutineWork.retreatScene = SCENE_GABOMBA_STATUE_PINION;
      gMainRoutineWork.retreatEntrance = 1;
    } else if (GameFlag_Get(FLAG_GABOMBA_RETREAT_TO_IDOL)) {
      gMainRoutineWork.retreatScene = SCENE_GABOMBA_STATUE_CHAMBER_IDOL;
      gMainRoutineWork.retreatEntrance =
          GABOMBA_STATUE_CHAMBER_IDOL_FROM_KIBOMBO_STATUE_GROUNDS;
    }
  }
#endif
}

#include "text/msg_ids.h"

// clang-format off
u32 gGabombaStatueChamber_WalkOffScript[] = {
    SC_ANIMATION(2),
    SC_MOVE(428, 0, 234),
    SC_SYNC,
    SC_MOVE(488, 0, 234),
    SC_SYNC,
    SC_END,
};

u32 gGabombaStatueChamber_AkafubuPath[] = {
    SC_ANIMATION(2),
    SC_MOVE(360, 0, 216),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_POSITION_ORIGIN,
    SC_END,
};

/// The new witch doctor spins round once, a 32nd of a turn a frame.
u32 gGabombaStatueChamber_SpinScript[] = {
    SC_ADD(SW_FACING, TRIG_TURN / 32),
    SC_WAIT(1),
    SC_LOOP(32, SC_TOP),
    SC_END,
};

/// The chest appears 64 pixels above the floor before the idol and sinks to
/// it, a pixel a frame.
s32 gGabombaStatueChamber_ChestDescendsScript[] = {
    SC_POSITION(392, 64, 124),
    SC_WAIT(1),
    SC_LABEL(1),
    SC_ADD(SW_Y, toFIXED(-1)),
    SC_WAIT(1),
    SC_LOOP(64, SC_TO(1)),
    SC_WAIT(1),
    SC_END,
};

/// The chest rises away, two pixels a frame for 80 frames, and is put away.
u32 gGabombaStatueChamber_ChestRisesScript[] = {
    SC_ADD(SW_Y, toFIXED(2)),
    SC_WAIT(1),
    SC_LOOP(80, SC_TOP),
    SC_POSITION_ORIGIN,
    SC_END,
};

u32 gGabombaStatueChamber_PartyLeavesScript[] = {
    SC_ANIMATION(2), SC_MOVE(392, 0, 148),
    SC_SYNC,   SC_MOVE(392, 0, 236),
    SC_SYNC,   SC_ANIMATION(1),
    SC_END,
};

s32 gGabombaStatueChamber_MirroredScript[] = {
    SC_SET(SW_SCALE_X, -FIXED_ONE),
    SC_END,
};

struct Actor *gGabombaStatueChamber_ShownItem = NULL;

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 392, 0, 152, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

struct SceneRegion gGabombaStatueChamber_IdolExitRegions[] = {
    {464, 16, 144, 480, 32, 160, CONDITION_ALWAYS, 2},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(GabombaStatueChamberIdol),
    SCENE_EXIT_UNLESS(GABOMBA_STATUE_CHAMBER_IDOL_TO_GABOMBA_STATUE_COGS, GabombaStatueCogs, GABOMBA_STATUE_COGS_FROM_GABOMBA_STATUE_CHAMBER_IDOL, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(GABOMBA_STATUE_CHAMBER_IDOL_TO_GABOMBA_STATUE_ARROWS_EAST, GabombaStatueArrows, GABOMBA_STATUE_ARROWS_FROM_GABOMBA_STATUE_CHAMBER_IDOL_EAST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(GABOMBA_STATUE_CHAMBER_IDOL_TO_KIBOMBO_STATUE_GROUNDS, KibomboStatueGrounds, KIBOMBO_STATUE_GROUNDS_FROM_GABOMBA_STATUE_CHAMBER_IDOL, FLAG_KIBOMBO_AT_CEREMONY),
    SCENE_EXIT_UNLESS(GABOMBA_STATUE_CHAMBER_IDOL_TO_KIBOMBO_STATUE_GROUNDS, KibomboStatueGroundsCeremony, KIBOMBO_STATUE_GROUNDS_CEREMONY_FROM_GABOMBA_STATUE_CHAMBER_IDOL_SOUTH, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(GABOMBA_STATUE_CHAMBER_IDOL_TO_GABOMBA_STATUE_CHAMBER_LIFT, GabombaStatueChamberLift, GABOMBA_STATUE_CHAMBER_LIFT_FROM_GABOMBA_STATUE_CHAMBER_IDOL, EXIT_FLAG_NONE),
    SCENE_EXITS(GabombaStatueChamberLift),
    SCENE_EXIT(GABOMBA_STATUE_CHAMBER_LIFT_TO_GABOMBA_STATUE_CHAMBER_IDOL, GabombaStatueChamberIdol, GABOMBA_STATUE_CHAMBER_IDOL_FROM_GABOMBA_STATUE_CHAMBER_LIFT),
    SCENE_EXIT(GABOMBA_STATUE_CHAMBER_LIFT_TO_GABOMBA_STATUE_CHAMBER_WITCH_DOCTOR, GabombaStatueChamberWitchDoctor, GABOMBA_STATUE_CHAMBER_WITCH_DOCTOR_FROM_GABOMBA_STATUE_CHAMBER_LIFT),
    SCENE_EXITS(GabombaStatueChamberWitchDoctor),
    SCENE_EXIT(GABOMBA_STATUE_CHAMBER_WITCH_DOCTOR_TO_GABOMBA_STATUE_CHAMBER_LIFT, GabombaStatueChamberLift, GABOMBA_STATUE_CHAMBER_LIFT_FROM_GABOMBA_STATUE_CHAMBER_WITCH_DOCTOR),
    SCENE_EXIT(GABOMBA_STATUE_CHAMBER_WITCH_DOCTOR_TO_GABOMBA_CATACOMBS_TELEPORTERS, GabombaCatacombsTeleporters, GABOMBA_CATACOMBS_TELEPORTERS_FROM_GABOMBA_STATUE_CHAMBER_WITCH_DOCTOR),
    SCENE_EXIT(GABOMBA_STATUE_CHAMBER_WITCH_DOCTOR_TO_KIBOMBO_HOUSES, KibomboHouses, KIBOMBO_HOUSES_FROM_GABOMBA_STATUE_CHAMBER_WITCH_DOCTOR),
    SCENE_EXITS_END,
};

struct ActorPlacement gGabombaStatueChamber_IdolActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_SEATED_IDOL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 152, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PINK_DRUM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 256, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PINK_DRUM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 456, 0, 256, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_PIERS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SHEBA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_AKAFUBU, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gGabombaStatueChamber_WitchDoctorActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_SEATED_IDOL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 88, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_AKAFUBU, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 136, FACING_NORTH_NORTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 392, 0, 84, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_CHEST, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_AKAFUBU_WITCH_DOCTOR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_NORTH_NORTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_DOOR_HALF, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 336, 0, 264, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_DOOR_HALF, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 352, 0, 264, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_STONE_DOOR_HALF, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 368, 0, 264, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_STONE_DOOR_HALF, CONDITION_ALWAYS, (s32)gGabombaStatueChamber_MirroredScript, 416, 0, 264, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_STONE_DOOR_HALF, CONDITION_ALWAYS, (s32)gGabombaStatueChamber_MirroredScript, 432, 0, 264, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_STONE_DOOR_HALF, CONDITION_ALWAYS, (s32)gGabombaStatueChamber_MirroredScript, 448, 0, 264, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_STONE_DOOR_HALF, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 336, 0, 328, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_STONE_DOOR_HALF, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 352, 0, 328, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_STONE_DOOR_HALF, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 368, 0, 328, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_STONE_DOOR_HALF, CONDITION_ALWAYS, (s32)gGabombaStatueChamber_MirroredScript, 416, 0, 328, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_STONE_DOOR_HALF, CONDITION_ALWAYS, (s32)gGabombaStatueChamber_MirroredScript, 432, 0, 328, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_STONE_DOOR_HALF, CONDITION_ALWAYS, (s32)gGabombaStatueChamber_MirroredScript, 448, 0, 328, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_STONE_DOOR_HALF, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 336, 0, 392, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_STONE_DOOR_HALF, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 352, 0, 392, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_STONE_DOOR_HALF, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 368, 0, 392, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_STONE_DOOR_HALF, CONDITION_ALWAYS, (s32)gGabombaStatueChamber_MirroredScript, 416, 0, 392, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_STONE_DOOR_HALF, CONDITION_ALWAYS, (s32)gGabombaStatueChamber_MirroredScript, 432, 0, 392, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_STONE_DOOR_HALF, CONDITION_ALWAYS, (s32)gGabombaStatueChamber_MirroredScript, 448, 0, 392, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    {SCENE_TABLE_END},
};

struct ActorPlacement gGabombaStatueChamber_Placements[] = {
    {SCENE_TABLE_END},
};
// clang-format on

struct Trigger gGabombaStatueChamber_IdolTriggers[] = {
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH),
     GABOMBA_STATUE_CHAMBER_IDOL_TO_GABOMBA_STATUE_COGS, CONDITION_ALWAYS,
     EVENT(GabombaStatueChamber_StepIntoFirstHole)},
    {TRIGGER_EXIT, GABOMBA_STATUE_CHAMBER_IDOL_TO_KIBOMBO_STATUE_GROUNDS,
     CONDITION_ALWAYS, GABOMBA_STATUE_CHAMBER_IDOL_TO_KIBOMBO_STATUE_GROUNDS},
    {TRIGGER_EXIT, GABOMBA_STATUE_CHAMBER_IDOL_TO_GABOMBA_STATUE_CHAMBER_LIFT,
     CONDITION_ALWAYS,
     GABOMBA_STATUE_CHAMBER_IDOL_TO_GABOMBA_STATUE_CHAMBER_LIFT},
    {TRIGGER_TABLE_END},
};

struct Trigger gGabombaStatueChamber_LiftTriggers[] = {
    {TRIGGER_EXIT, GABOMBA_STATUE_CHAMBER_LIFT_TO_GABOMBA_STATUE_CHAMBER_IDOL,
     CONDITION_ALWAYS,
     GABOMBA_STATUE_CHAMBER_LIFT_TO_GABOMBA_STATUE_CHAMBER_IDOL},
    {TRIGGER_EXIT,
     GABOMBA_STATUE_CHAMBER_LIFT_TO_GABOMBA_STATUE_CHAMBER_WITCH_DOCTOR,
     CONDITION_ALWAYS,
     GABOMBA_STATUE_CHAMBER_LIFT_TO_GABOMBA_STATUE_CHAMBER_WITCH_DOCTOR},
    {TRIGGER_STEP, CELL_BEHAVIOR_LIFT, FLAG_LEADER_ON_LIFT,
     EVENT(GabombaStatueChamber_RideLiftDown)},
    {TRIGGER_STEP, CELL_BEHAVIOR_SOUTH_OF_LIFT, CONDITION_ALWAYS,
     EVENT(GabombaStatueChamber_StepOffLift)},
    {TRIGGER_TABLE_END},
};

struct Trigger gGabombaStatueChamber_WitchDoctorTriggers[] = {
    {TRIGGER_EXIT,
     GABOMBA_STATUE_CHAMBER_WITCH_DOCTOR_TO_GABOMBA_STATUE_CHAMBER_LIFT,
     CONDITION_ALWAYS,
     GABOMBA_STATUE_CHAMBER_WITCH_DOCTOR_TO_GABOMBA_STATUE_CHAMBER_LIFT},
    {STEP_TRIGGER_WALKING,
     GABOMBA_STATUE_CHAMBER_WITCH_DOCTOR_TO_GABOMBA_CATACOMBS_TELEPORTERS,
     CONDITION_ALWAYS, EVENT(GabombaStatueChamber_StepIntoSecondHole)},
    {TRIGGER_STEP, CELL_BEHAVIOR_BELOW_AKAFUBU, FLAG_AKAFUBU_REACHED_CHAMBER,
     EVENT(GabombaStatueChamber_AkafubuReachesChamber)},
    {TRIGGER_TALK, ACTOR_CHAMBER_AKAFUBU, CONDITION_ALWAYS,
     MSGNO(MsgYourBlackOrb)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_CHAMBER_AKAFUBU,
     CONDITION_ALWAYS, MSGNO(MsgOeiaWhatAm)},
    {EXAMINE_TRIGGER_FACING(FACING_NORTH), CELL_BEHAVIOR_IDOL,
     FLAG_GABOMBA_MESSAGE_CARRIED,
     EVENT(GabombaStatueChamber_GabombaNamesWitchDoctor)},
    {TRIGGER_TABLE_END},
};

struct Trigger gGabombaStatueChamber_Triggers[] = {
    {TRIGGER_TABLE_END},
};

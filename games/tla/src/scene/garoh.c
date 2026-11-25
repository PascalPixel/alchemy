/// @file
/// Garoh, the village of werewolves that guards Air's Rock, in five scenes that
/// share this overlay: the way up from the world map (SceneId_GarohWayUp),
/// the path up to the village (SceneId_GarohPath, whose chest
/// holds the Hypnos Sword), the village itself (SceneId_Garoh), the tunnels
/// under its houses (SceneId_GarohTunnels) and the cave behind the rock by the
/// sanctum (SceneId_GarohWerewolfCave).
///
/// The party first comes on a night of the full moon. Climbing the way up,
/// the light turns to evening and then to night (Garoh_CrossLowerTwilightLine,
/// Garoh_CrossUpperTwilightLine); on the path a werewolf howls and runs past
/// into the village (Garoh_WerewolfRunsPast). The hooded villagers say there
/// are no werewolves, and by the sanctum an old werewolf peers out of the big
/// grey rock and hides again (Garoh_WerewolfPeersFromCave). With Reveal from
/// Air's Rock the party opens the rock, follows the little werewolf into the
/// cave (Garoh_FollowWerewolfIntoCave) and is surrounded by the werewolves,
/// whose elder Maha tells them to come back later
/// (Garoh_WerewolvesRevealThemselves). By day the villagers show their faces,
/// and Maha tells the werewolves' legend and gives the party Ether
/// (Garoh_MahaTellsTheLegend).
#include "../field/actor_set_sprite_priority.c"

#include "../field/field_effect_move.c"
#include "field/actor_view.h"
#include "field/field.h"
#define EFFECT_SET_MODE Actor_SelectAnimation
#define EFFECT_SET_SCRIPT Actor_StartScript
#include "../field/field_effect_spawn.c"

#include "system/task.h"
#include "types.h"
#include "macros.h"
#include "system/ram_buffer.h"
#include "field/actor.h"

#ifdef MODERN
void Rope_Lash(s32 a, s32 b, s32 c);
#endif
void Actor_PlaceBlocks(const void *data);
void Actor_LiftTargetBlock(const void *data);
void Actor_PlaceMovedBlock(void);

void Garoh_SinkPillar(s32 id, struct Actor *actor);
void Garoh_SinkFirstPillarOnCell(void);
void Garoh_SinkSecondPillarOnCell(void);
void Garoh_CheckThirdPillar(void);
void Garoh_SinkFourthPillarOnCell(void);
void Garoh_ScatterDustRing(s32 id);
#include "field/event_inline.h"
#include "field/map_grid.h"
#include "field/field.h"
#include "field/event_commands.h"
#include "scene/scene.h"
#include "scene_ids.h"
#include "types.h"
#include "flags.h"
#include "djinn_ids.h"
#include "field/field_sequence_event.h"
#include "field/field_sequence.h"
#include "system/ram_buffer.h"
#include "system/dma.h"
#include "system/io_reg.h"
#include "graphics/palette_buffers.h"
#include "system/frame.h"
#include "battle/battle.h"
#include "game/game.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "field/scene_tables.h"
#include "system/main_routine_state.h"
#include "shared/tables.h"
#include "system/field_effect.h"
#include "field/actor.h"
#include "field/field_sprite.h"
#include "field/field_effect.h"
#include "field/call.h"
#include "field/actor_script.h"
#include "fixed_math.h"
#include "text/msg_ids.h"

extern struct ActorPlacement gGaroh_VillageDayActorPlacements[];
extern struct Trigger gGaroh_WayUpTriggers[];
void Garoh_WerewolvesRevealThemselves(void);
void Garoh_MahaTellsTheLegend(void);
void Garoh_InitializeVillage(void);
void Garoh_InitializeWayUp(void);
extern u32 gGaroh_WerewolfLookAroundScript[];
s32 Garoh_MetWerewolves();
void Garoh_OpenDoorAndEnter(void);
void Garoh_WerewolfPeersFromCave(void);
void Garoh_EnterOpening(void);
void Garoh_MeetInTheCave(void);
void Garoh_PlaceFirstPillar(void);
void Garoh_PlaceSecondPillar(void);
void Garoh_PlaceThirdPillar(void);
void Garoh_PlaceFourthPillar(void);
void Garoh_PlacePushedPillar(void);
void Garoh_RevealTunnelPassage(void);
void Garoh_HideCaveEntrance(void);
void Garoh_RestoreWayUpLight(void);
void Garoh_HideTunnelPassage(void);
void Garoh_HideCavePassage(void);
void Garoh_CrossLowerTwilightLine(void);
void Garoh_CrossUpperTwilightLine(void);
extern u32 gFieldEffect_ShortScript[];
extern u32 gFieldEffect_LongScript[];
extern s32 gGaroh_ShrunkSpriteScript[];
extern s16 gGaroh_DoorFrames[];
extern s16 gGaroh_WideDoorFrames[];
extern s16 gGaroh_TallDoorFrames[];
void Garoh_LashRope(void);
/// BG2's horizontal offset for each screen line while the path's layer ripples
/// by night (Garoh_WavePathLayer).
u16 gGaroh_WaveLines[DISPLAY_HEIGHT] BSS_DATA ALIGNED(16);

/// The village's people by night and by day (SceneId_Garoh), numbering the
/// rows of gGaroh_VillageActorPlacements and gGaroh_VillageDayActorPlacements:
/// the five who go hooded by night show their faces by day.
enum GarohVillageActorId {
  /// By night he asks whether the party has seen a werewolf; by day he has
  /// heard from Maha that the party is special.
  ACTOR_VILLAGER_ASKING_ABOUT_THE_WEREWOLF = ACTOR_FIRST_PLACED,
  /// "Forgive me for not taking my hood off..."
  ACTOR_HOODED_VILLAGER,
  /// "Gaaaroooooo!" ... "I was just calling my dog."
  ACTOR_HOWLING_VILLAGER,
  /// "Strangers should not go about at night..."
  ACTOR_VILLAGER_WARNING_STRANGERS,
  /// "Don't you love the sight of the full moon?"
  ACTOR_VILLAGER_ON_THE_FULL_MOON,
  /// By day only: "I was in the cave with Maha! I was that little werewolf!"
  ACTOR_LITTLE_WEREWOLF,
  /// The old werewolf who peers out of the cave's entrance by the Sanctum the
  /// first night: Maha, on his sprite in the cave.
  ACTOR_MAHA_AT_THE_CAVE_ENTRANCE = ACTOR_FIRST_PLACED + 13,
  ACTOR_CAVE_ENTRANCE
};

/// The path up to the village and the werewolves' cave
/// (SceneId_GarohPath and SceneId_GarohWerewolfCave), numbering the
/// rows of gGaroh_WerewolfCaveActorPlacements; GarohPath places the
/// werewolf and Kraden on the same rows.
enum GarohWerewolfActorId {
  ACTOR_WEREWOLF = ACTOR_FIRST_PLACED,
  ACTOR_KRADEN,
  ACTOR_MAHA,
  ACTOR_CAVE_PASSAGE,
  ACTOR_ETHER,
  /// The cave's fires, shrunk to wall lamps; the first two burn behind the
  /// party.
  ACTOR_FIRE_A = ACTOR_FIRST_PLACED + 5,
  ACTOR_FIRE_B
};

/// The tightrope posts on the path up to the village: Jenna, Sheba and Piers
/// take their own numbers, so the posts follow the werewolf and Kraden.
enum {
  ACTOR_PATH_TIGHTROPE_POST_A = ACTOR_FIRST_PLACED + 2,
  ACTOR_PATH_TIGHTROPE_POST_B
};

/// The stone pillars on the way up to the village (SceneId_GarohWayUp),
/// numbering the rows of gGaroh_WayUpActorPlacements: Move or a push takes
/// each onto the slot where it sinks.
enum GarohWayUpActorId {
  ACTOR_FIRST_PILLAR = ACTOR_FIRST_PLACED,
  ACTOR_SECOND_PILLAR,
  ACTOR_THIRD_PILLAR,
  ACTOR_FOURTH_PILLAR
};

/// The cells the step and examine triggers answer, by their behaviour in each
/// scene's map. A door's or an opening's behaviour is the exit it leaves by.
enum GarohCellBehavior {
  /// The village: east of the sanctum's door, cell (20, 16).
  CELL_BEHAVIOR_BY_SANCTUM_DOOR = 20,
  /// The village: the well, cells (19..20, 18..19).
  CELL_BEHAVIOR_GAROH_WELL = 200,
  /// The path up to the village: across it at cells (9..11, 19).
  CELL_BEHAVIOR_WEREWOLF_RUNS_PAST = 30,
  /// The way up: where the light turns to evening, cell (13, 26), and to
  /// night, cell (16, 22).
  CELL_BEHAVIOR_LOWER_TWILIGHT_LINE = 20,
  CELL_BEHAVIOR_UPPER_TWILIGHT_LINE = 21,
  /// The werewolves' cave: below the cleft, cells (17..18, 49), and below
  /// where Maha stands, cells (19..21, 15).
  CELL_BEHAVIOR_CAVE_MOUTH = 30,
  CELL_BEHAVIOR_BELOW_MAHA = 31
};

/// Story flags of the werewolves' secret, in the order the party meets them.
#define FLAG_GAROH_REACHED_VILLAGE 0x919
#define FLAG_GAROH_SAW_WEREWOLF 0x91c
#define FLAG_GAROH_FOLLOWED_WEREWOLF 0x91d
#define FLAG_GAROH_MET_WEREWOLVES 0x91e
#define FLAG_GAROH_HEARD_MAHA 0x91f
/// For this visit to the area: the old werewolf has peered from the cave,
/// the little werewolf has thanked the party, and the way up is in the
/// evening's light, between its two lines.
#define FLAG_WEREWOLF_PEERED_OUT FLAG_AREA_TEMP_20
#define FLAG_LITTLE_WEREWOLF_THANKED FLAG_AREA_TEMP_01
#define FLAG_EVENING FLAG_AREA_TEMP_18
/// By night the village's actors take the warm tint as well as its map.
/// Nothing in the code sets it, so they keep their own colours.
#define FLAG_NIGHT_TINTS_ACTORS FLAG_AREA_TEMP_11

/// The passage within the werewolves' cave behind the round grey rock, which
/// Reveal opens (Garoh_EnterOpening): from before the rock to the little room
/// beyond it and back (scene_exits.h names only the exits between scenes). An
/// exit and the entrance on its side share a number.
enum GarohWerewolfCavePassage {
  GAROH_WEREWOLF_CAVE_TO_ROOM_BEHIND_ROCK = 6,
  GAROH_WEREWOLF_CAVE_ROOM_BEHIND_ROCK_TO_CAVE,
  GAROH_WEREWOLF_CAVE_FROM_ROOM_BEHIND_ROCK = 6,
  GAROH_WEREWOLF_CAVE_ROOM_BEHIND_ROCK_FROM_CAVE
};

/// The werewolf sheet's last picture, held: sitting on the ground.
enum { ANIM_WEREWOLF_SIT = 5 };
/// The way up: the pillars sunk into their slots, and the rope Lashed.
#define FLAG_FIRST_PILLAR_SUNK FLAG_SCENE_TEMP_01
#define FLAG_SECOND_PILLAR_SUNK FLAG_SCENE_TEMP_02
#define FLAG_GAROH_ROPE_LASHED FLAG_SCENE_TEMP_03
#define FLAG_FOURTH_PILLAR_SUNK FLAG_SCENE_TEMP_04
/// The two lines on the way up where the light changes, crossed since the
/// party came in.
#define FLAG_CROSSED_LOWER_TWILIGHT_LINE FLAG_SCENE_TEMP_10
#define FLAG_CROSSED_UPPER_TWILIGHT_LINE FLAG_SCENE_TEMP_11

/// The houses' lit windows, colour 14 of BG palette 1, kept white when the
/// village darkens for the night.
#define WINDOW_LIGHT 14

/// The way into the path up to the village that no exit uses: by night it tints
/// the path purple rather than warm and dark.
#define ENTRANCE_PATH_PURPLE_NIGHT 12

/// The steps in which the Ether grows from two fifths of its size to full.
#define ETHER_GROW_STEPS 20

/// By day the path up to the village's second layer takes the first's scale and
/// wrap.
void Garoh_MatchSecondLayer(void) {
  struct MapState *map = Ram_HeapSlots->mapGrid;
  struct MapLayer *to = &map->layers[1];
  struct MapLayer *from = &map->layers[0];

  to->scaleX = from->scaleX;
  to->scaleY = from->scaleY;
  to->maskX = from->maskX;
  to->maskY = from->maskY;
}

/// By night the second map layer of the path up to the village ripples: each
/// line is offset by two sines of the frame count, handed to an HBlank DMA.
void Garoh_WavePathLayer(void) {
  s32 x;
  // The sine routine itself, called at the address Trig_Sin's far call
  // jumps to.
  s32 (*sine)(s32) = *(s32 (**)(s32))(((u32)Trig_Sin & ~3) + 4);
  struct MapLayer *layer = MapGrid_GetLayer(1);
  u16 *line = gGaroh_WaveLines;

  if (gFieldState->revealActive == 0) {
    u32 frame;
    s32 z;
    s32 i;
    volatile u16 *dma;

    layer->speedY = 0;
    frame = (u8)gFrameCount;
    i = 0;
    /* FAKEMATCH(barrier): i and line held through an empty asm; as no asm. */
    asm("" : "+r"(i), "+r"(line) : "r"(line));
    z = FIXEDtoINT(layer->y);
    x = FIXEDtoINT(layer->x);
    /* FAKEMATCH(dead): a once-only block that places the two waves' steps
     * and angles around the loop; as them declared with z. */
    {
      // The two waves: their angles at the top line, and the steps each line
      // turns them on by.
      s32 stepA = TRIG_TURN * 51 / 256;
      s32 stepB = TRIG_TURN * 153 / 2048;
      s32 a = z * stepA + frame * (TRIG_TURN / 64);
      s32 b = z * stepB - frame * (TRIG_TURN / 64);

      for (; i != DISPLAY_HEIGHT;) {
        s32 s1 = sine(a);
        s32 s2 = sine(b);

        i++;
        /* FAKEMATCH(dead): a store to z, which nothing reads again. */
        if (i == 300 || x == 300)
          z = 0;
        *line++ = FIXEDtoINT((s1 * 3 + s2 * 7) >> 3) + x;
        a += stepA;
        b += stepB;
      }
    }
    line = gGaroh_WaveLines;
    REG_BG2HOFS = line[0];
    dma = REG_DMA0;
    dma[5] &= ~DMA_START_TIMING_DRQ_REPEAT;
    dma[5] &= ~DMA_ENABLE;
    (void)dma[5];
    Dma_Set(line + 1, (void *)&REG_BG2HOFS, DMA_HBLANK_HALFWORD_PER_LINE,
            (volatile u32 *)dma);
  }
}

extern u16 gGaroh_PathDayColors[];
extern u16 gGaroh_PathNightColors[];

/// Writes count colours from first on, from a table of red, green and blue
/// halfwords, to the BG palette and the palette fade's source.
#define DEFINE_GAROH_LOAD_PALETTE(name, components, first, count)              \
  void name(void) {                                                            \
    struct PaletteFade *buffers = Ram_HeapSlots->paletteFade;                  \
    u32 i;                                                                     \
                                                                               \
    for (i = 0; i < count; i++) {                                              \
      BG_PLTT[first + i] = components[i * 3 + 2] << 10 |                       \
                           components[i * 3 + 1] << 5 | components[i * 3];     \
      buffers->palette[first + i] = BG_PLTT[first + i];                        \
    }                                                                          \
  }

/// By day the path's colours: BG palette 1's colours 8 to 15.
DEFINE_GAROH_LOAD_PALETTE(Garoh_LoadPathDayColors, gGaroh_PathDayColors, 24, 8)

/// By night the path's colours: BG palette 6's colours 1 to 9.
DEFINE_GAROH_LOAD_PALETTE(Garoh_LoadPathNightColors, gGaroh_PathNightColors, 97,
                          9)

/// A script's call: clears the actor's sprite flags.
s32 Garoh_ClearScriptedActorSpriteFlags(struct Actor *actor) {
  Actor_SetSpriteFlags(actor, 0);
  return 0;
}

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

extern struct ActorPlacement gGaroh_ActorPlacements[];
extern struct ActorPlacement gGaroh_VillageActorPlacements[];
extern struct ActorPlacement gGaroh_WayUpActorPlacements[];
extern struct ActorPlacement gGaroh_TunnelsActorPlacements[];
extern struct ActorPlacement gGaroh_PathActorPlacements[];
extern struct ActorPlacement gGaroh_WerewolfCaveActorPlacements[];

extern struct MapGridCellAnimation gGaroh_Doors[];

/// The people of the scene being entered: in the village, the hooded
/// villagers by night or their faces once Maha has shared the secret.
const void *Scene_GetActorPlacements(void) {
  s16 scene = gMainRoutineState.next;

  if (scene == SCENE_GAROH) {
    if (Event_GetFlag(FLAG_GAROH_SECRET_SHARED))
      return gGaroh_VillageDayActorPlacements;
    return gGaroh_VillageActorPlacements;
  }
  if (scene == SCENE_GAROH_PATH)
    return gGaroh_PathActorPlacements;
  if (scene == SCENE_GAROH_WAY_UP)
    return gGaroh_WayUpActorPlacements;
  if (scene == SCENE_GAROH_TUNNELS)
    return gGaroh_TunnelsActorPlacements;
  if (scene == SCENE_GAROH_WEREWOLF_CAVE)
    return gGaroh_WerewolfCaveActorPlacements;
  return gGaroh_ActorPlacements;
}

/// Walking north into the door of one of Garoh's round houses, or of the
/// sanctum, opens it and Felix walks inside, while the villagers stand still:
/// the young werewolf's family's home at cell (14, 23), the old couple's at
/// (8, 14), the weapons and armor shop's wide door at (10, 24), the inn at
/// (21, 25) and the sanctum's tall door at (14, 15).
///
/// The door's cell behaviour is the exit it leaves by, which also picks its
/// opening frames from `gGaroh_Doors`.
void Garoh_OpenDoorAndEnter(void) {
  struct FieldState *state = Ram_HeapSlots->fieldState;
  s32 index;
  s32 i;

  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  for (i = ACTOR_FIRST_PLACED; (u32)i <= ACTOR_COUNT - 1; i++) {
    {
      struct Actor *actor = Actor_Get(i);

      if (actor != NULL)
        actor->motionFlags = 0;
    }
  }
  index = state->stepTrigger;
  Event_PlaySound(SE_DOOR_OPEN);
  index--;
  MapGrid_AnimateCells(gGaroh_Doors[index].steps, gGaroh_Doors[index].x,
                       gGaroh_Doors[index].y);
  Actor_SetAnimation(gMainRoutineState.leaderActorId, ANIM_WALK);
  Actor_SetSpeed(gMainRoutineState.leaderActorId, 0.5);
  {
    s32 owner = gMainRoutineState.leaderActorId;

    // The sanctum's tall door is entered straight on, the others from the
    // centre of their cell.
    if (index == GAROH_TO_GAROH_ROOMS_SANCTUM - 1)
      Actor_SetDestinationOffset(owner, 0, -4);
    else
      Actor_CenterAndWalk(owner, 2, -4);
  }
  FieldEvent_Wait(6);
  FieldEvent_SetExitRequest(state->stepTrigger);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Event_End();
}

/// Talking at night to the hooded villager outside the young werewolf's
/// family's home: "So, uh...you say you saw a werewolf?" Yes gets "Don't be
/// ridiculous... There's no such thing as werewolves!"; No gets its own reply.
void Garoh_AskIfSawWerewolf(s32 actor) {
  s32 msg = (s32)MsgGarohSoUhYouSayYou;

  FieldEvent_SetMessageId(msg);
  Event_OpenMessage(actor, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(FieldParty_GetLeaderActor(), 0) ==
      0) {
    FieldEvent_Wait(10);
    FieldEvent_SetMessageId(msg + 1);
  } else {
    FieldEvent_Wait(20);
    FieldEvent_SetMessageId(msg + 2);
  }
  Event_ShowMessage(actor, 0);
}

/// Talking at night to the hooded villager outside the Item Shopkeeper's
/// house: "Don't you love the sight of the full moon?" Yes gets "Yes, I
/// agree... But it brings me grief."; No gets its own reply.
void Garoh_AskAboutFullMoon(s32 actor) {
  s32 msg = (s32)MsgGarohDontYouLoveTheSight;

  FieldEvent_SetMessageId(msg);
  Event_OpenMessage(actor, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(FieldParty_GetLeaderActor(), 0) ==
      0) {
    FieldEvent_Wait(10);
    FieldEvent_SetMessageId(msg + 1);
  } else {
    FieldEvent_Wait(20);
    FieldEvent_SetMessageId(msg + 2);
  }
  Event_ShowMessage(actor, 0);
}

/// Talking by day to the woman in green south-west of the well, the villager
/// who warned strangers off at night: "Maha said that you were the only
/// outsiders we could share our secret with...", and she asks whether it
/// bothers them that everyone here is a werewolf.
void Garoh_AskAboutSecret(s32 actor) {
  s32 msg = (s32)MsgGarohMahaSaidThatYouWere;

  FieldEvent_SetMessageId(msg);
  Event_OpenMessage(actor, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(FieldParty_GetLeaderActor(), 0) ==
      0) {
    FieldEvent_Wait(10);
    FieldEvent_SetMessageId(msg + 1);
  } else {
    FieldEvent_Wait(20);
    FieldEvent_SetMessageId(msg + 2);
  }
  Event_ShowMessage(actor, 0);
}

/// Talking by day to the grey-haired man west of the well, who claimed at
/// night to be calling his dog: he howls "Awoooooh!" and is glad he no longer
/// has to hide it.
void Garoh_HowlOutLoud(s32 actor) {
  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  FieldEvent_SetMessageId((s32)MsgGarohAwoooooh);
  // "Awoooooh!"
  FieldEvent_ShowMessageThenWait(actor, 0, 10);
  // "Heh heh... Sorry about that! I'm so happy I don't have to hide that..."
  FieldEvent_ShowMessageThenWait(actor, 0, 10);
  Event_End();
}

/// Talking at night to the hooded villager west of the well: he howls
/// "Gaaaroooooo!", catches himself ("Gah! Whoops...") and says he was just
/// calling his dog. His code would cut him to the howl alone once
/// FLAG_AREA_TEMP_00 is set, but nothing in Garoh, its paths, tunnels, cave or
/// rooms sets that flag, so the player always hears all three lines.
void Garoh_HowlForDog(s32 actor) {
  s32 flag;
  s32 msg;

  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  flag = Event_GetFlag(FLAG_AREA_TEMP_00);
  msg = (s32)MsgGarohGaaarooooooForDog;
  if (flag) {
    FieldEvent_SetMessageId(msg);
  } else {
    FieldEvent_SetMessageId(msg);
    // "Gaaaroooooo!"
    FieldEvent_ShowMessageThenWait(actor, 0, 10);
    // "Gah! Whoops..."
    FieldEvent_ShowMessageThenWait(actor, 0, 10);
  }
  FieldEvent_ShowMessageThenWait(actor, 0, 10);
  Event_End();
}

/// Talking by day to the fair-haired boy running about by the sanctum, or
/// reading his mind the first time: "Hey, it's you guys! Thanks for last
/// night!", and he explains he was the little werewolf in the cave. Later he
/// says he is the reason the village no longer needs its hoods.
void Garoh_LittleWerewolfThanks(void) {
  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  FieldActor_Stop(ACTOR_LITTLE_WEREWOLF);
  Actor_SetAnimation(ACTOR_LITTLE_WEREWOLF, ANIM_STAND);
  FieldEvent_Wait(10);
  Actor_FaceActor(ACTOR_LITTLE_WEREWOLF, ACTOR_FELIX, 0);
  if (Event_GetFlag(FLAG_LITTLE_WEREWOLF_THANKED)) {
    FieldEvent_SetMessageId((s32)MsgGarohImTheReasonYouCan);
  } else {
    FieldEvent_SetMessageId((s32)MsgGarohHeyItsYouGuysThanks);
    // "Hey, it's you guys! Thanks for last night!"
    FieldEvent_ShowMessageThenWait(ACTOR_LITTLE_WEREWOLF, 0, 10);
    FieldActor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_QUESTION, 15);
    FieldActor_ShowEmote(ACTOR_LITTLE_WEREWOLF,
                         EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 10);
    // "Oh, yeah... That's right... You probably don't recognize me in my..."
    FieldEvent_ShowMessageThenWait(ACTOR_LITTLE_WEREWOLF, 0, 5);
    FieldActor_ShowEmote(ACTOR_LITTLE_WEREWOLF, EMOTE_IN_FRONT | EMOTE_SMILE,
                         15);
    // "I was in the cave with Maha! I was that little werewolf!"
    FieldEvent_ShowMessageThenWait(ACTOR_LITTLE_WEREWOLF, 0, 5);
  }
  FieldEvent_ShowMessageThenWait(ACTOR_LITTLE_WEREWOLF, 0, 10);
  Event_SetFlag(FLAG_LITTLE_WEREWOLF_THANKED);
  // Back to running about the village.
  Actor_SetScript(ACTOR_LITTLE_WEREWOLF, MOVEMENT_TYPE_WANDER);
  Event_End();
}

/// Walking past the sanctum's door at night before meeting the werewolves,
/// cell (20, 16): the camera looks up at the big grey rock on the ledge above,
/// an old grey-maned werewolf opens it with Reveal and steps out, cries
/// "What!? Outsiders, here?" and slips back inside, the rock closing behind
/// him. Once a visit.
void Garoh_WerewolfPeersFromCave(void) {
  s32 flag;
  s32 facing;

  void Garoh_OpenCaveEntrance(s32 id) {
    FieldPsynergy_LoadResources(ABILITY_REVEAL, 1);
    FieldPsynergy_SetupActorPair(id, 0);
    FieldPsynergy_RunHandler(FIELD_PSYNERGY_HANDLER_ON_TARGET);
    BattleEffect_StopMainActor();
    FieldPsynergy_End();
  }

  void Garoh_CloseCaveEntrance(s32 id) {
    FieldPsynergy_LoadResources(ABILITY_REVEAL, 1);
    FieldPsynergy_SetupActorPair(id, 0);
    FieldPsynergy_RunHandler(FIELD_PSYNERGY_HANDLER_EXTRA);
    BattleEffect_StopMainActor();
    FieldPsynergy_End();
  }

  flag = Event_GetFlag(FLAG_WEREWOLF_PEERED_OUT);
  if (flag == 0) {
    Event_SetFlag(FLAG_WEREWOLF_PEERED_OUT);
    Event_Start();
    FieldEvent_EndLastingPsynergy(0);
    Actor_SetSpriteFlags(Actor_Get(ACTOR_CAVE_ENTRANCE), 0);
    facing = FACING_SOUTH;
    Actor_SetPositionAndFacing(ACTOR_CAVE_ENTRANCE, toFIXED(360), toFIXED(199),
                               facing);
    FieldCamera_MoveTo(359, CAMERA_KEEP, 220, 1);
    FieldCamera_WaitForArrival();
    Garoh_OpenCaveEntrance(ACTOR_CAVE_ENTRANCE);
    FieldEvent_Wait(30);
    Actor_SetPositionAndFacing(ACTOR_MAHA_AT_THE_CAVE_ENTRANCE, toFIXED(360),
                               toFIXED(199), facing);
    Actor_WalkByAndWait(ACTOR_MAHA_AT_THE_CAVE_ENTRANCE, 0, 16);
    Garoh_CloseCaveEntrance(ACTOR_CAVE_ENTRANCE);
    FieldEvent_SetMessageId((s32)MsgGarohWhatOutsidersHere);
    Actor_FaceDirectionAndWait(ACTOR_MAHA_AT_THE_CAVE_ENTRANCE,
                               FACING_SOUTH_SOUTHWEST);
    Actor_RunRepeatedMotion(ACTOR_MAHA_AT_THE_CAVE_ENTRANCE, 2);
    FieldEvent_Wait(20);
    Actor_FaceDirection(ACTOR_FELIX, FACING_NORTHEAST, 0);
    // "What!? Outsiders, here?"
    FieldEvent_ShowMessageThenWait(ACTOR_MAHA_AT_THE_CAVE_ENTRANCE, 0, 10);
    FieldActor_ShowEmote(ACTOR_MAHA_AT_THE_CAVE_ENTRANCE,
                         EMOTE_IN_FRONT | EMOTE_WINCE, 50);
    Actor_Get(ACTOR_MAHA_AT_THE_CAVE_ENTRANCE)->steeringFlags &=
        ~ACTOR_FACES_MOTION;
    Actor_FaceDirection(ACTOR_MAHA_AT_THE_CAVE_ENTRANCE, facing, 0);
    FieldEvent_Wait(30);
    Garoh_OpenCaveEntrance(ACTOR_CAVE_ENTRANCE);
    FieldEvent_Wait(20);
    Actor_WalkByAndWait(ACTOR_MAHA_AT_THE_CAVE_ENTRANCE, 0, -16);
    Actor_Get(ACTOR_MAHA_AT_THE_CAVE_ENTRANCE)->motionFlags = flag;
    Actor_WalkByAndWait(ACTOR_MAHA_AT_THE_CAVE_ENTRANCE, 0, -16);
    Actor_SetPosition(ACTOR_MAHA_AT_THE_CAVE_ENTRANCE, 0, 0);
    FieldEvent_Wait(10);
    Garoh_CloseCaveEntrance(ACTOR_CAVE_ENTRANCE);
    FieldEvent_Wait(60);
    Actor_SetPosition(ACTOR_CAVE_ENTRANCE, 0, 0);
    Event_End();
  }
}

extern struct Trigger gGaroh_Triggers[];
extern struct Trigger gGaroh_VillageTriggers[];
extern struct Trigger gGaroh_VillageDayTriggers[];
extern struct Trigger gGaroh_PathTriggers[];
extern struct Trigger gGaroh_TunnelsTriggers[];
extern struct Trigger gGaroh_WerewolfCaveTriggers[];

extern s32 gGaroh_HowlFrames;

/// Walking north into an opening Reveal has shown takes the party through it:
/// the cleft at the foot of the big grey rock north-east of the sanctum, cell
/// (22, 11), leads down into the werewolves' cave, and the passage behind the
/// round grey rock in the cave leads on.
void Garoh_EnterOpening(void) {
  struct FieldState *event = Ram_HeapSlots->fieldState;

  Event_PlaySound(SE_MAP_EXIT);
  gFieldState->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  Event_SetFlag(FLAG_ARRIVAL_STAIRS_DOWN);
  FieldEvent_SetExitRequest(event->stepTrigger);
}

/// Walking north to cells (19..21, 15) of the werewolves' cave, below where
/// Maha stands: the first night the werewolves surround the party, and by day
/// Maha is waiting there to tell the legend.
void Garoh_MeetInTheCave(void) {
  if (!Event_GetFlag(FLAG_GAROH_SECRET_SHARED)) {
    if (!Event_GetFlag(FLAG_GAROH_MET_WEREWOLVES))
      Garoh_WerewolvesRevealThemselves();
  } else {
    if (!Event_GetFlag(FLAG_GAROH_HEARD_MAHA))
      Garoh_MahaTellsTheLegend();
  }
}

/// The triggers of the scene being entered: in the village, the night's until
/// Maha has shared the secret and the day's after.
const void *Scene_GetTriggers(void) {
  s16 scene = gMainRoutineState.next;

  if (scene == SCENE_GAROH) {
    if (Event_GetFlag(FLAG_GAROH_SECRET_SHARED))
      return gGaroh_VillageDayTriggers;
    return gGaroh_VillageTriggers;
  }
  if (scene == SCENE_GAROH_PATH)
    return gGaroh_PathTriggers;
  if (scene == SCENE_GAROH_WAY_UP)
    return gGaroh_WayUpTriggers;
  if (scene == SCENE_GAROH_TUNNELS)
    return gGaroh_TunnelsTriggers;
  if (scene == SCENE_GAROH_WEREWOLF_CAVE)
    return gGaroh_WerewolfCaveTriggers;
  return gGaroh_Triggers;
}

/// A howl comes each time the night's frames under this mask are 0.
#define HOWL_PERIOD_MASK 0x3ff

/// A wolf howls in the distance every 1,024 frames while it is night.
void Garoh_HowlAtNight(void) {
  if (!Event_GetFlag(FLAG_GAROH_SECRET_SHARED) &&
      Event_GetFlag(FLAG_GAROH_NIGHT)) {
    gGaroh_HowlFrames++;
    if ((gGaroh_HowlFrames & HOWL_PERIOD_MASK) == 0)
      Event_PlaySound(SE_HOWL);
  }
}

void Garoh_InitializePath(void);

/// Sets up the scene being entered: the cave's lamps, and Maha once he has
/// told the legend; the village by night or day; the path up to the village;
/// or the way up, which also makes the sanctum the place a defeat returns the
/// party to once they have reached the village.
s32 Scene_Initialize(void) {
  struct FieldState *state;

  if (gMainRoutineState.next == SCENE_GAROH_TUNNELS) {
    gFieldState->transition =
        SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  } else if (gMainRoutineState.next == SCENE_GAROH_WEREWOLF_CAVE) {
    FieldActor_SetSpritePriority(ACTOR_FIRE_A, 1);
    FieldActor_SetSpritePriority(ACTOR_FIRE_B, 1);
    if (Event_GetFlag(FLAG_GAROH_HEARD_MAHA)) {
      Actor_SetPositionAndFacing(ACTOR_MAHA, toFIXED(328), toFIXED(162),
                                 FACING_SOUTH);
    }
    gFieldState->transition =
        SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
    FieldEffect_StartBlankOverlay(0);
    if (gMainRoutineState.entrance ==
        GAROH_WEREWOLF_CAVE_FROM_ROOM_BEHIND_ROCK) {
      state = Ram_HeapSlots->fieldState;
      state->transition =
          SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
      Event_ClearFlag(FLAG_ARRIVAL_STAIRS_UP);
      Event_ClearFlag(FLAG_ARRIVAL_STAIRS_DOWN);
      gFieldState->transition = SCENE_TRANSITION(
          TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
      Event_StartScreenTransition();
      Event_WaitTransitionFrames();
    }
  } else {
    gFieldState->transition = 0;
    if (gMainRoutineState.next == SCENE_GAROH) {
      Event_SetFlag(FLAG_GAROH_REACHED_VILLAGE);
      Garoh_InitializeVillage();
    } else if (gMainRoutineState.next == SCENE_GAROH_PATH) {
      Garoh_InitializePath();
    } else if (gMainRoutineState.next == SCENE_GAROH_WAY_UP) {
      Garoh_InitializeWayUp();
      if (Event_GetFlag(FLAG_GAROH_REACHED_VILLAGE)) {
        gMainRoutineState.restartScene = SCENE_GAROH_ROOMS_SANCTUM;
        gMainRoutineState.restartEntrance = GAROH_ROOMS_SANCTUM_FROM_GAROH;
      }
    }
  }
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

/// The village: by night darkened, its windows lit and the wolves howling;
/// by day the cleft in the big grey rock and the cave's other ways stand
/// open.
void Garoh_InitializeVillage(void) {
  FieldTask_AddFrame(Garoh_HowlAtNight, TASK_PASS_FRAME);
  gFieldState->transition = 0;
  if (Event_GetFlag(FLAG_GAROH_SECRET_SHARED) == 0 ||
      Event_GetFlag(FLAG_GAROH_NIGHT) != 0) {
    Event_SetFlag(FLAG_GAROH_NIGHT);
    PaletteFade_ApplyTarget(PALETTE_TINT_WARM_DARK, PALETTES_BG);
    if (Event_GetFlag(FLAG_NIGHT_TINTS_ACTORS))
      PaletteFade_ApplyTarget(PALETTE_TINT_WARM, PALETTES_OBJECTS);
    PaletteFade_Interpolate(1);
    Event_WaitTransitionFrames();
    PaletteFade_PackTarget();
    {
      struct PaletteFade *fade = Ram_HeapSlots->paletteFade;

      *(volatile u16 *)&BG_PLTT_COLOR(1, WINDOW_LIGHT) = RGB_WHITE;
      fade->palette[PALETTE_COLORS + WINDOW_LIGHT] =
          *(volatile u16 *)&BG_PLTT_COLOR(1, WINDOW_LIGHT);
    }
    PaletteFade_ApplyTarget(PALETTE_TINT_NONE, 1);
  } else {
    MapGrid_CopyCellsTo(10, 32, 10, 10, 1, 1);
    MapGrid_CopyCellsTo(12, 32, 25, 22, 1, 1);
    MapGrid_CopyCellsTo(11, 33, 8, 22, 1, 1);
    MapGrid_CopyCellsTo(74, 32, 74, 10, 1, 1);
    MapGrid_CopyCellsTo(76, 32, 89, 22, 1, 1);
    MapGrid_CopyCellsTo(75, 33, 72, 22, 1, 1);
  }
  if (gMainRoutineState.entrance == GAROH_FROM_GAROH_WEREWOLF_CAVE) {
    struct FieldState *state = Ram_HeapSlots->fieldState;

    state->transition =
        SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
    Event_ClearFlag(FLAG_ARRIVAL_STAIRS_UP);
    Event_ClearFlag(FLAG_ARRIVAL_STAIRS_DOWN);
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    gFieldState->transition = 0;
  }
}

/// The path up to the village: the tightrope strung, and by night its dark
/// colours and rippling water, by day its hidden ways open.
void Garoh_InitializePath(void) {
  Task_AddFrame(Garoh_HowlAtNight, TASK_PASS_FRAME);
  Tightrope_Initialize();
  Tightrope_Place(0, ACTOR_PATH_TIGHTROPE_POST_A, ACTOR_PATH_TIGHTROPE_POST_B);
  if (Event_GetFlag(FLAG_GAROH_NIGHT)) {
    if (gMainRoutineState.entrance == ENTRANCE_PATH_PURPLE_NIGHT) {
      PaletteFade_ApplyTarget(PALETTE_TINT_CUSTOM | RGB(9, 6, 18), 1);
    } else {
      PaletteFade_ApplyTarget(PALETTE_TINT_WARM_DARK, 1);
    }
    PaletteFade_Interpolate(1);
    Event_WaitTransitionFrames();
    PaletteFade_PackTarget();
    Garoh_LoadPathNightColors();
    PaletteFade_ApplyTarget(PALETTE_TINT_NONE, 1);
    Task_AddFrame((s32)Garoh_WavePathLayer, TASK_PASS_VBLANK);
  } else {
    MapGrid_CopyCellsTo(20, 7, 9, 7, 5, 5);
    MapGrid_CopyCellsTo(20, 71, 9, 71, 5, 5);
    MapGrid_CopyCellsTo(84, 7, 73, 7, 5, 5);
    Garoh_LoadPathDayColors();
    PaletteFade_ApplyTarget(PALETTE_TINT_NONE, 1);
    Garoh_MatchSecondLayer();
  }
}

extern s16 gGaroh_Pillars[];

/// The way up from the world map: the four stone pillars where they were
/// left, in the evening's or the night's light once the party has climbed
/// into it.
void Garoh_InitializeWayUp(void) {
  FieldTask_AddFrame(Garoh_HowlAtNight, TASK_PASS_FRAME);
  if (Event_GetFlag(FLAG_GAROH_NIGHT)) {
    if (Event_GetFlag(FLAG_EVENING))
      PaletteFade_ApplyTarget(PALETTE_TINT_WARM, 1);
    else
      PaletteFade_ApplyTarget(PALETTE_TINT_WARM_DARK, 1);
  }
  Actor_PlaceBlocks(gGaroh_Pillars);
  PaletteFade_Interpolate(1);
  Event_WaitTransitionFrames();
}

/// Nothing visible happens as Felix starts casting Move on one of the stone
/// pillars on the way up; the game lifts the pillar off the map's cells so
/// that Move can slide it.
void Garoh_LiftPillar(void) { Actor_LiftTargetBlock(gGaroh_Pillars); }

/// Nothing visible happens as Felix starts pushing one of the stone pillars on
/// the way up; as with Move, the game first lifts the pillar off the map's
/// cells so that it can slide.
void Garoh_LiftPushedPillar(void) { Actor_LiftTargetBlock(gGaroh_Pillars); }

/// A pillar set down on its slot sinks 32 pixels into the ground with a whoosh
/// and a ring of dust.
void Garoh_SinkPillar(s32 id, struct Actor *actor) {
  s32 i;
  s32 saved;

  actor->motionFlags = 0;
  saved = id;
  for (i = 16; i > 0; i--) {
    actor->y.fixed -= toFIXED(2);
    FieldEvent_Wait(1);
  }
  Event_PlaySound(SE_WHOOSH);
  Garoh_ScatterDustRing(saved);
}

/// Defines a check that sinks pillar actorId (Garoh_SinkPillar) once it stands
/// on cell (cellX, cellZ), and sets flag so it sinks once.
#define DEFINE_GAROH_TRIGGER_AT_CELL(name, actorId, flag, cellX, cellZ)        \
  void name(void) {                                                            \
    struct Actor *actor = Actor_Get(actorId);                                  \
    s32 x = FIELD_CELL(actor->x.fixed);                                        \
    s32 raw;                                                                   \
    s32 z;                                                                     \
                                                                               \
    raw = actor->z.fixed;                                                      \
    z = FIELD_CELL(raw);                                                       \
    if (!Event_GetFlag(flag) && x == cellX && z == cellZ) {                    \
      Garoh_SinkPillar(actorId, actor);                                        \
      Event_SetFlag(flag);                                                     \
    }                                                                          \
  }

/// The first pillar sinks once it stands on cell (10, 29), one west of where it
/// starts.
DEFINE_GAROH_TRIGGER_AT_CELL(Garoh_SinkFirstPillarOnCell, ACTOR_FIRST_PILLAR,
                             FLAG_FIRST_PILLAR_SUNK, 10, 29)

/// The second pillar sinks once it stands on cell (15, 28), one east of where
/// it starts.
DEFINE_GAROH_TRIGGER_AT_CELL(Garoh_SinkSecondPillarOnCell, ACTOR_SECOND_PILLAR,
                             FLAG_SECOND_PILLAR_SUNK, 15, 28)

/// The third pillar has no slot to sink into: its check reads the pillar and
/// the rope's flag and does nothing.
void Garoh_CheckThirdPillar(void) {
  Actor_Get(ACTOR_THIRD_PILLAR);
  Event_GetFlag(FLAG_GAROH_ROPE_LASHED);
}

/// The fourth pillar sinks once it stands on cell (15, 25), one south of where
/// it starts.
void Garoh_SinkFourthPillarOnCell(void) {
  struct Actor *actor = Actor_Get(ACTOR_FOURTH_PILLAR);
  s32 x = FIELD_CELL(actor->x.fixed);
  s32 z = FIELD_CELL(actor->z.fixed);

  if (!Event_GetFlag(FLAG_FOURTH_PILLAR_SUNK) && x == 15 && z == 25) {
    Garoh_SinkPillar(ACTOR_FOURTH_PILLAR, actor);
    Event_SetFlag(FLAG_FOURTH_PILLAR_SUNK);
  }
}

/// Move on the first stone pillar, on the lowest ledge of the way up at cell
/// (11, 29): slid one cell west, onto cell (10, 29), it drops off the ledge and
/// sinks into the ground with a whoosh and a ring of dust until its top is
/// level with the ledge, so that Felix can walk west off the ledge onto it.
/// From the ledge its top is a dead end: holding west or south there takes him
/// no further, and he can only step back east onto the ledge. The second
/// pillar's top is the same (Garoh_PlaceSecondPillar).
void Garoh_PlaceFirstPillar(void) {
  Actor_PlaceMovedBlock();
  Garoh_SinkFirstPillarOnCell();
}

/// Move on the second stone pillar, east of the first at cell (14, 28): slid
/// one cell east, onto cell (15, 28), it drops off its ledge onto the path
/// below and sinks into the ground with a whoosh and a ring of dust until its
/// top is level with the ledge. Felix can walk east onto its top, but holding
/// east or south there takes him no further.
void Garoh_PlaceSecondPillar(void) {
  Actor_PlaceMovedBlock();
  Garoh_SinkSecondPillarOnCell();
}

/// Move on the third stone pillar, on the ledge above at cell (10, 24): it
/// slides where Move sends it, and has no slot to sink into.
void Garoh_PlaceThirdPillar(void) {
  Actor_PlaceMovedBlock();
  Garoh_CheckThirdPillar();
}

/// Move on the fourth stone pillar, east of the third at cell (15, 24): cast
/// from the ledge north of it and slid one cell south, onto cell (15, 25), it
/// drops off the ledge onto the path below and sinks into the ground with a
/// whoosh and a ring of dust until its top is level with the ledge. Felix can
/// walk south onto its top, but like the first two it is a dead end: holding
/// south, west or east there takes him no further.
void Garoh_PlaceFourthPillar(void) {
  Actor_PlaceMovedBlock();
  Garoh_SinkFourthPillarOnCell();
}

/// Pushing a stone pillar on the way up sinks it just as Move does when it
/// comes to rest on its slot: the first on cell (10, 29), the second on
/// (15, 28), the fourth on (15, 25).
///
/// The push rows answer only while the first pillar has not sunk.
void Garoh_PlacePushedPillar(void) {
  Actor_PlaceMovedBlock();
  Garoh_SinkFirstPillarOnCell();
  Garoh_SinkSecondPillarOnCell();
  Garoh_CheckThirdPillar();
  Garoh_SinkFourthPillarOnCell();
}

/// Reveal in the village shows the cleft at the foot of the big grey rock
/// north-east of the sanctum, cell (22, 11), as a way in.
void Garoh_RevealCaveEntrance(void) {
  MapGrid_CopyCellsTo(98, 8, 86, 8, 1, 2);
  MapGrid_CopyCellAttributes(34, 9, 1, 1, 22, 11);
}

/// Reveal in the tunnels shows a hidden passage at cells (11, 33..35).
void Garoh_RevealTunnelPassage(void) {
  MapGrid_CopyCellsTo(25, 47, 11, 33, 1, 3);
  MapGrid_CopyCellAttributes(25, 47, 1, 1, 11, 33);
}

/// Reveal in the werewolves' cave shows a passage through the round grey rock
/// in its middle, cells (19..21, 5..7).
void Garoh_RevealCavePassage(void) {
  MapGrid_CopyCellsTo(44, 5, 19, 5, 3, 3);
  MapGrid_CopyCellsTo(108, 5, 83, 5, 3, 2);
  MapGrid_CopyCellAttributes(44, 5, 3, 3, 19, 5);
}

/// Reveal wearing off in the village hides the cleft in the big grey rock
/// again.
void Garoh_HideCaveEntrance(void) {
  MapGrid_CopyCellsTo(102, 8, 86, 8, 1, 2);
  MapGrid_CopyCellAttributes(22, 10, 1, 1, 22, 11);
}

/// Reveal wearing off on the path up to the village: at night the path's dark
/// colours, there before the cast, come back exactly as they were. This row
/// plays no part in it: it only checks whether it is night in Garoh, and does
/// nothing with the answer.
s32 Garoh_IsNight(void) { return Event_GetFlag(FLAG_GAROH_NIGHT); }

/// Reveal wearing off on the way up brings back the evening's or the night's
/// light.
void Garoh_RestoreWayUpLight(void) {
  if (Event_GetFlag(FLAG_GAROH_NIGHT)) {
    if (Event_GetFlag(FLAG_EVENING))
      PaletteFade_ApplyTarget(PALETTE_TINT_WARM, 1);
    else
      PaletteFade_ApplyTarget(PALETTE_TINT_WARM_DARK, 1);
    PaletteFade_Interpolate(16);
  }
}

/// Reveal wearing off in the tunnels hides their passage again.
void Garoh_HideTunnelPassage(void) {
  MapGrid_CopyCellsTo(8, 30, 11, 33, 1, 3);
  MapGrid_CopyCellAttributes(8, 30, 1, 1, 11, 33);
}

/// Reveal wearing off in the werewolves' cave closes the round grey rock
/// again.
void Garoh_HideCavePassage(void) {
  MapGrid_CopyCellsTo(39, 5, 19, 5, 3, 3);
  MapGrid_CopyCellsTo(103, 5, 83, 5, 3, 2);
  MapGrid_CopyCellAttributes(39, 5, 3, 3, 19, 5);
}

/// Climbing the way up from the world map before the werewolves' secret is
/// shared, the step onto cell (13, 26), the lower of its two lines, turns the
/// daylight to a golden evening; coming down from the path, the same step
/// brings the daylight back. Once a visit.
void Garoh_CrossLowerTwilightLine(void) {
  struct FieldState *state;

  if (!Event_GetFlag(FLAG_GAROH_SECRET_SHARED)) {
    Event_SetFlag(FLAG_CROSSED_LOWER_TWILIGHT_LINE);
    if (gMainRoutineState.entrance == GAROH_WAY_UP_FROM_WORLD_MAP) {
      PaletteFade_ApplyTarget(PALETTE_TINT_WARM, 1);
      Event_SetFlag(FLAG_GAROH_NIGHT);
      Event_SetFlag(FLAG_EVENING);
    } else {
      PaletteFade_ApplyTarget(PALETTE_TINT_NONE, 1);
      Event_ClearFlag(FLAG_GAROH_NIGHT);
      Event_ClearFlag(FLAG_EVENING);
    }
    state = Ram_HeapSlots->fieldState;
    if (state->revealActive == 0)
      PaletteFade_Interpolate(120);
  }
}

/// Climbing on up past cell (16, 22), the upper of the way up's two lines,
/// the evening darkens into night; coming down from the path, the same step
/// turns the night to evening. Once a visit, before the werewolves' secret is
/// shared.
void Garoh_CrossUpperTwilightLine(void) {
  struct FieldState *state;

  if (!Event_GetFlag(FLAG_GAROH_SECRET_SHARED)) {
    Event_SetFlag(FLAG_CROSSED_UPPER_TWILIGHT_LINE);
    if (gMainRoutineState.entrance == GAROH_WAY_UP_FROM_GAROH_PATH) {
      PaletteFade_ApplyTarget(PALETTE_TINT_WARM, 1);
      Event_SetFlag(FLAG_GAROH_NIGHT);
      Event_SetFlag(FLAG_EVENING);
    } else {
      PaletteFade_ApplyTarget(PALETTE_TINT_WARM_DARK, 1);
      Event_SetFlag(FLAG_GAROH_NIGHT);
      Event_ClearFlag(FLAG_EVENING);
    }
    state = Ram_HeapSlots->fieldState;
    if (state->revealActive == 0)
      PaletteFade_Interpolate(120);
  }
}

#include "../field/field_effect_advance_motion.c"

#undef EFFECT_SET_MODE
#undef EFFECT_SET_SCRIPT
#undef DEFINE_GAROH_LOAD_PALETTE
#undef DEFINE_GAROH_TRIGGER_AT_CELL
void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options);

/// A ring of puffs of dust spreads out from the actor, one every eighth of a
/// turn: nine, the first and last both to the east.
void Garoh_ScatterDustRing(s32 objectId) {
  struct Actor *actor;
  struct Vector3 vec;
  struct EffectOptions params;
  u32 angleStep;
  s32 angle;
  s32 x;
  FIXED z;

  actor = Actor_Get(objectId);
  params.update = FieldEffect_AdvanceMotion;
  angleStep = 0;
  do {
    angle = angleStep * (TRIG_TURN / DUST_RING_PUFFS);
    vec.x = Trig_Cos(angle);
    vec.y = 0;
    z = Math_Sin(angle);
    x = vec.x;
    vec.z = z;
    x += x / 3;
    vec.x = x;
    FieldEffect_Spawn(actor->x.fixed, actor->y.fixed, actor->z.fixed, x, vec.y,
                      z, EFFECT_USE_UPDATE | 1, &params);
    angleStep += 2;
  } while (angleStep <= DUST_RING_PUFFS);
}

extern u32 gGaroh_QuestionMarkScript[];

/// Unused: Lash ties a rope between the third and fourth stone pillars on the
/// way up; no trigger runs it.
void Garoh_LashRope(void) {
  Rope_Lash(1, ACTOR_THIRD_PILLAR, ACTOR_FOURTH_PILLAR);
  Event_SetFlag(FLAG_GAROH_ROPE_LASHED);
}

/// Until the secret is shared, whether the party has met the werewolves.
s32 Garoh_MetWerewolves(void) {
  if (!Event_GetFlag(FLAG_GAROH_SECRET_SHARED))
    return Event_GetFlag(FLAG_GAROH_MET_WEREWOLVES);
}

/// Pops a "?" bubble up at (x, y, z), tilted by rotation, for the given
/// frames.
void Garoh_PopQuestionMark(x, y, z, rotation, frames) s32 x, y, z;
s16 rotation;
s32 frames;
{
  struct Actor *object;
  struct Sprite *sprite;

  object = FieldActor_Create(SPRITE_EMOTE, x, y, z);
  if (object != NULL) {
    sprite = object->sprite;
    Actor_SelectAnimation(object, EMOTE_QUESTION);
    Actor_StartScript(object, gGaroh_QuestionMarkScript);
    object->motionFlags = 0;
    object->wait = frames;
    sprite->flags = 0;
    sprite->rotation = rotation;
  }
}

#include "text/msg_ids.h"

/// Two question marks pop up over the actor's head in turn, right and left,
/// each tilted, a third of a second apart.
void Garoh_PopQuestionMarks(s32 id) {
  struct Actor *actor = Actor_Get(id);

  Garoh_PopQuestionMark(actor->x.fixed + toFIXED(8),
                        actor->y.fixed + toFIXED(28), actor->z.fixed,
                        TRIG_TURN / 16, 20);
  Event_PlaySound(SE_LIGHT_APPEAR);
  FieldEvent_Wait(20);
  Garoh_PopQuestionMark(actor->x.fixed - toFIXED(8),
                        actor->y.fixed + toFIXED(28), actor->z.fixed,
                        TRIG_TURN * 15 / 16, 20);
  Event_PlaySound(SE_LIGHT_APPEAR);
  FieldEvent_Wait(20);
}

/// Defines Reveal's effect played on actor at the cave's passage: the effect's
/// phase, and the passage's 3 by 3 cells redrawn from column fromX.
#define DEFINE_GAROH_PLAY_EFFECT(name, phase, fromX)                           \
  void name(s32 actor) {                                                       \
    FieldPsynergy_LoadResources(ABILITY_REVEAL, 1);                            \
    FieldPsynergy_SetupActorPair(actor, 0);                                    \
    FieldPsynergy_RunHandler(phase);                                           \
    MapGrid_CopyCellsLayered(fromX, 5, 3, 3, 19, 5);                           \
    BattleEffect_StopMainActor();                                              \
    FieldPsynergy_End();                                                       \
  }

/// Reveal's effect played on the cave's passage as it opens.
DEFINE_GAROH_PLAY_EFFECT(Garoh_OpenCavePassage, 1, 44)

/// Reveal's effect played on the cave's passage as it closes.
DEFINE_GAROH_PLAY_EFFECT(Garoh_CloseCavePassage, 3, 39)

/// Sends an actor walking to where the leader stands, if there is a leader.
static inline void Scene_WalkTowardLeader(s32 actorId) {
  struct Actor *leader = Actor_Get(gMainRoutineState.leaderActorId);

  if (leader != NULL) {
    Actor_SetDestination(actorId, leader->x.part[0].integer,
                         leader->z.part[0].integer);
  }
}

/// An actor walks into the leader and is taken off the map, as the party
/// closes up into one walking sprite at the end of an event.
static inline void Scene_WalkIntoLeader(s32 actorId) {
  struct Actor *leader;

  Actor_SetAnimation(actorId, ANIM_WALK);
  leader = Actor_Get(gMainRoutineState.leaderActorId);
  if (leader != NULL) {
    Actor_SetDestination(actorId, leader->x.part[0].integer,
                         leader->z.part[0].integer);
  }
  FieldActor_WaitForMoveById(actorId);
  Actor_SetPosition(actorId, 0, 0);
}

/// The point i eighths of the way from from to to.
static inline s32 Garoh_PathStep(s32 from, s32 to, s32 i) {
  return from + ((to - from) * i) / 8;
}

/// How far, in pixels, the werewolf weaves either side of the path as it runs
/// past: across the screen, and up and down it.
enum { WEAVE_ACROSS = 12, WEAVE_DOWN = 7 };

/// Walking up the path to the village the first night, cells (9..11, 19): a
/// werewolf howls "Gaaaroooooo!" and bounds down the path past the pond and
/// off into the village, and Kraden explains to the startled party what a
/// werewolf is.
void Garoh_WerewolfRunsPast(void) {
  struct Actor *werewolf;
  s32 i;
  s32 position[3];
  s32 *pos;
  FIXED x, z;
  FIXED toX, toZ;

  Event_SetFlag(FLAG_GAROH_SAW_WEREWOLF);
  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  Actor_SetPosition(ACTOR_WEREWOLF, 96, 8);
  FieldEvent_SetMessageId((s32)MsgGarohGaaaroooooo);
  // "Gaaaroooooo!"
  FieldEvent_ShowMessageThenWait(ACTOR_WEREWOLF, 0, 0);
  Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Actor_SetSpeed(ACTOR_FELIX, 0.8);
  Actor_WalkToAndWait(ACTOR_FELIX, 152, 304);
  FieldActor_LaunchFromLeader(ACTOR_KRADEN, 32, -16, FACING_NORTH_NORTHWEST);
  FieldActor_LaunchFromLeader(ACTOR_JENNA, 0, 0, FACING_NORTH);
  FieldActor_LaunchFromLeader(ACTOR_SHEBA, 16, 0, FACING_NORTH_NORTHWEST);
  if (Event_GetFlag(FLAG_IN_PARTY(ACTOR_PIERS)))
    FieldActor_LaunchFromLeader(ACTOR_PIERS, 8, 16, FACING_NORTH);
  Actor_WalkByAndWait(ACTOR_FELIX, -16, -16);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
  FieldActor_WaitForMoveById(ACTOR_KRADEN);
  FieldEvent_Wait(20);
  // "Aow-aow-oooo!!!"
  FieldEvent_ShowMessageThenWait(ACTOR_WEREWOLF, 0, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_WEST, 0);
  FieldEvent_Wait(25);
  // "What was that!?"
  FieldEvent_ShowMessageThenWait(ACTOR_JENNA, 0, 0);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  if (Event_GetFlag(FLAG_IN_PARTY(ACTOR_PIERS)))
    Actor_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_QUESTION, 50);
  // "Awoooooo!!!"
  FieldEvent_ShowMessageThenWait(ACTOR_WEREWOLF, 0, 0);
  if (Event_GetFlag(FLAG_IN_PARTY(ACTOR_PIERS)))
    FieldActor_StartRepeatedMotion(ACTOR_PIERS, 2);
  FieldActor_StartRepeatedMotion(ACTOR_SHEBA, 2);
  FieldActor_StartRepeatedMotion(ACTOR_KRADEN, 2);
  FieldActor_StartRepeatedMotion(ACTOR_FELIX, 2);
  Actor_RunRepeatedMotion(ACTOR_JENNA, 2);
  FieldEvent_Wait(20);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 0);
  FieldEvent_Wait(20);
  FieldCamera_MoveTo(144, CAMERA_KEEP, 128, 1);
  FieldCamera_WaitForArrival();
  werewolf = Actor_Get(ACTOR_WEREWOLF);
  Actor_SetAnimation(ACTOR_WEREWOLF, ANIM_WALK);
  // The werewolf lopes down the path in eight bounds at random speeds,
  // weaving from side to side.
  for (i = 0; i <= 7; i++) {
    s32 tenths;
    s32 wave;

    wave = Math_Sin(i * (DEGtoANG(45)));
    pos = position;
    pos[0] = Garoh_PathStep(toFIXED(104), toFIXED(88), i) - wave * WEAVE_ACROSS;
    wave = Math_Sin(i * (DEGtoANG(45)));
    position[2] =
        Garoh_PathStep(toFIXED(32), toFIXED(120), i) - wave * WEAVE_DOWN;
    position[1] = 0;
    tenths = Random_Below(10) + 4;
    Actor_SetSpeedFX(ACTOR_WEREWOLF, tenths * (toFIXED(1) / 10),
                     tenths * (toFIXED(1) / 20));
    Actor_WalkToAndWait(ACTOR_WEREWOLF, position[0] / toFIXED(1),
                        position[2] / toFIXED(1));
  }
  Actor_SetAnimation(ACTOR_WEREWOLF, ANIM_STAND);
  FieldEvent_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_WEREWOLF, 2);
  Actor_SetScript(ACTOR_WEREWOLF, gGaroh_WerewolfLookAroundScript);
  gFieldState->message--;
  // "Awoooooo!!!"
  FieldEvent_ShowMessageThenWait(ACTOR_WEREWOLF, 0, 0);
  FieldActor_Stop(ACTOR_WEREWOLF);
  FieldEvent_Wait(20);
  Actor_FaceDirection(ACTOR_WEREWOLF, FACING_SOUTHEAST, 0);
  FieldEvent_Wait(20);
  EventCamera_FollowActor(ACTOR_WEREWOLF, 1);
  Actor_SetAnimation(ACTOR_WEREWOLF, ANIM_WALK);
  x = werewolf->x.fixed;
  z = werewolf->z.fixed;
  toX = toFIXED(128);
  toZ = toFIXED(184);
  for (i = 0; i <= 7; i++) {
    s32 tenths;
    FIXED wave;

    wave = Math_Sin(i * (DEGtoANG(45)));
    position[0] = Garoh_PathStep(x, toX, i) - wave * 4;
    position[2] = Garoh_PathStep(z, toZ, i);
    position[1] = 0;
    tenths = Random_Below(10) + 4;
    Actor_SetSpeedFX(ACTOR_WEREWOLF, tenths * (toFIXED(1) / 10),
                     tenths * (toFIXED(1) / 20));
    Actor_WalkToAndWait(ACTOR_WEREWOLF, position[0] / toFIXED(1),
                        position[2] / toFIXED(1));
  }
  Actor_SetAnimation(ACTOR_WEREWOLF, ANIM_STAND);
  Actor_FaceDirection(ACTOR_WEREWOLF, FACING_SOUTH_SOUTHEAST, 0);
  FieldEvent_Wait(20);
  Actor_FaceDirection(ACTOR_WEREWOLF, FACING_EAST, 0);
  FieldEvent_Wait(30);
  // "Ah-ah-awoooo!!!"
  FieldEvent_ShowMessageThenWait(ACTOR_WEREWOLF, 0, 0);
  // "Eeek!"
  FieldEvent_ShowMessageThenWait(ACTOR_SHEBA, 0, 0);
  Actor_RunRepeatedMotion(ACTOR_WEREWOLF, 2);
  FieldEvent_Wait(10);
  Actor_FaceDirection(ACTOR_WEREWOLF, FACING_SOUTH_SOUTHEAST, 0);
  FieldEvent_Wait(30);
  Actor_ShowEmote(ACTOR_WEREWOLF, EMOTE_IN_FRONT | EMOTE_WINCE, 50);
  Actor_SetSpeed(ACTOR_WEREWOLF, 0.6);
  Actor_Get(ACTOR_WEREWOLF)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkToAndWait(ACTOR_WEREWOLF, 128, 148);
  FieldEvent_Wait(1);
  Actor_Get(ACTOR_WEREWOLF)->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_SetSpeed(ACTOR_WEREWOLF, 2.5);
  Actor_WalkByAndWait(ACTOR_WEREWOLF, 0, 4);
  FieldEvent_Wait(20);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  Actor_FaceDirection(ACTOR_WEREWOLF, FACING_NORTH, 0);
  FieldEvent_Wait(20);
  FieldActor_StartRepeatedMotion(ACTOR_WEREWOLF, 3);
  Actor_SetAttachedEffect(ACTOR_WEREWOLF,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(40);
  Actor_FaceDirection(ACTOR_WEREWOLF, FACING_WEST, 0);
  FieldEvent_Wait(10);
  Actor_SetSpeed(ACTOR_WEREWOLF, 2.4);
  Actor_WalkByAndWait(ACTOR_WEREWOLF, -24, -8);
  Actor_WalkByAndWait(ACTOR_WEREWOLF, -8, -100);
  FieldEvent_Wait(30);
  Actor_SetPosition(ACTOR_WEREWOLF, 0, 0);
  FieldCamera_MoveTo(168, CAMERA_KEEP, 280, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(10);
  Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  if (Event_GetFlag(FLAG_IN_PARTY(ACTOR_PIERS)))
    Actor_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  FieldActor_Jump(ACTOR_KRADEN, 6, 15);
  FieldActor_Jump(ACTOR_KRADEN, 6, 23);
  Actor_AnimateThenWait(ACTOR_KRADEN, ANIM_NOD, 20);
  FieldEvent_Wait(10);
  // "I don't believe it! Was that a°"
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_EAST, 0);
  if (Event_GetFlag(FLAG_IN_PARTY(ACTOR_PIERS)))
    Actor_FaceDirection(ACTOR_PIERS, FACING_NORTH_NORTHEAST, 0);
  FieldEvent_Wait(25);
  Actor_RunRepeatedMotion(ACTOR_JENNA, 2);
  FieldEvent_Wait(10);
  // "A what? Kraden, do you know what that thing was?"
  FieldEvent_ShowMessageThenWait(ACTOR_JENNA, 0, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 0);
  FieldEvent_Wait(25);
  // "Well... Perhaps..."
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 0);
  Actor_RunRepeatedMotion(ACTOR_SHEBA, 2);
  FieldEvent_Wait(10);
  // "So, what are you waiting for!? Tell us!"
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_SHEBA, 0, 0);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  // "I suspect it may have been...a lycanthrope."
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 0);
  if (Event_GetFlag(FLAG_IN_PARTY(ACTOR_PIERS)))
    Actor_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_QUESTION, 50);
  Actor_RunRepeatedMotion(ACTOR_JENNA, 2);
  FieldEvent_Wait(10);
  // "A lycanthwhat?"
  FieldEvent_ShowMessageThenWait(ACTOR_JENNA, 0, 0);
  Actor_AnimateThenWait(ACTOR_KRADEN, ANIM_NOD, 20);
  FieldEvent_Wait(10);
  // "Just think of them as a race of people born with special powers..."
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 0);
  Actor_RunRepeatedMotion(ACTOR_SHEBA, 2);
  FieldEvent_Wait(10);
  // "Kind of like Adepts, you mean?"
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_SHEBA, 0, 0);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  // "Yes, that sounds right. Sheba, you've put it quite well."
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 0);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_QUESTION, 50);
  Actor_AnimateThenWait(ACTOR_KRADEN, ANIM_NOD, 20);
  FieldEvent_Wait(10);
  // "Adepts borrow their abilities from the power of the elements..."
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 0);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
  FieldEvent_Wait(10);
  // "While lycanthropes borrow theirs from the power of beasts..."
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 0);
  Actor_SetAnimation(ACTOR_JENNA, ANIM_NOD);
  Actor_SetAnimation(ACTOR_SHEBA, ANIM_NOD);
  if (Event_GetFlag(FLAG_IN_PARTY(ACTOR_PIERS)))
    Actor_SetAnimation(ACTOR_PIERS, ANIM_NOD);
  Actor_AnimateThenWait(ACTOR_FELIX, ANIM_NOD, 20);
  FieldEvent_Wait(30);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
  // "This is why they take the forms of animals..."
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 0);
  Actor_AnimateThenWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD, 33);
  // "They may look frightening... But we must try to communicate with them."
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 0);
  Actor_WalkByAndWait(ACTOR_KRADEN, -8, -8);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTH, 0);
  Actor_WalkByAndWait(ACTOR_KRADEN, -10, -6);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
  Actor_WalkByAndWait(ACTOR_KRADEN, -14, -4);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTHWEST, 0);
  if (Event_GetFlag(FLAG_IN_PARTY(ACTOR_PIERS)))
    Actor_FaceDirection(ACTOR_PIERS, FACING_NORTH, 0);
  FieldEvent_Wait(30);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH, 0);
  FieldEvent_Wait(20);
  Actor_AnimateThenWait(ACTOR_KRADEN, ANIM_NOD, 20);
  FieldEvent_Wait(10);
  // "Lycanthropes... Werewolves... A whole village in hiding..."
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTHWEST, 0);
  if (Event_GetFlag(FLAG_IN_PARTY(ACTOR_PIERS)))
    Actor_FaceDirection(ACTOR_PIERS, FACING_NORTH_NORTHWEST, 0);
  FieldEvent_Wait(20);
  // "Is it just me... Or does Kraden seem a little TOO happy to have found
  // werewolves?" Yes: "What? Me? Happy to find werewolves? You must be joking!"
  // No: "I would love to study them, but still...they do frighten me..."
  Event_OpenMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_JENNA, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_SOUTHEAST, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(ACTOR_FELIX, 0) == 0) {
    FieldEvent_Wait(30);
    Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
    FieldEvent_Wait(20);
    Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 0);
    FieldEvent_Wait(20);
    if (Event_GetFlag(FLAG_IN_PARTY(ACTOR_PIERS)))
      Actor_FaceDirection(ACTOR_PIERS, FACING_NORTH, 0);
    // "What? Me? Happy to find werewolves? You must be joking!"
    FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0, 0);
    gFieldState->message++;
  } else {
    FieldEvent_Wait(40);
    Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
    FieldEvent_Wait(20);
    Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 0);
    FieldEvent_Wait(20);
    if (Event_GetFlag(FLAG_IN_PARTY(ACTOR_PIERS)))
      Actor_FaceDirection(ACTOR_PIERS, FACING_NORTH, 0);
    gFieldState->message++;
    // "I would love to study them, but still...they do frighten me..."
    FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0, 0);
  }
  FieldActor_Jump(ACTOR_KRADEN, 6, 15);
  FieldActor_Jump(ACTOR_KRADEN, 6, 23);
  // "I guess I'm just curious about all of the secrets of these new lands..."
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0, 0);
  Actor_RunRepeatedMotion(ACTOR_JENNA, 2);
  FieldEvent_Wait(10);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
  FieldEvent_Wait(15);
  // "Any real scholar would be thrilled to explore a new land full of mystery,
  // huh?"
  FieldEvent_ShowMessageThenWait(ACTOR_JENNA, 0, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTHEAST, 0);
  FieldEvent_Wait(25);
  Actor_AnimateThenWait(ACTOR_KRADEN, ANIM_NOD, 20);
  FieldEvent_Wait(8);
  Actor_AnimateThenWait(ACTOR_KRADEN, ANIM_NOD, 20);
  FieldEvent_Wait(8);
  Actor_AnimateThenWait(ACTOR_KRADEN, ANIM_NOD, 20);
  FieldEvent_Wait(20);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  // "So, are there werewolves only on Osenia?"
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_SHEBA, 0, 0);
  FieldActor_StartRepeatedMotion(ACTOR_KRADEN, 3);
  Actor_SetAttachedEffect(ACTOR_KRADEN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(40);
  // "Ummm..."
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0, 0);
  Garoh_PopQuestionMarks(ACTOR_SHEBA);
  FieldEvent_Wait(20);
  // "If so, how could you even have known about them, Kraden?"
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_SHEBA, 0, 0);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
  FieldEvent_Wait(10);
  Actor_SetSpeed(ACTOR_KRADEN, 0.4);
  Actor_Get(ACTOR_KRADEN)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkByAndWait(ACTOR_KRADEN, 0, -16);
  FieldEvent_Wait(1);
  Actor_Get(ACTOR_KRADEN)->steeringFlags |= ACTOR_FACES_MOTION;
  FieldEvent_Wait(20);
  Actor_SetAttachedEffect(ACTOR_KRADEN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(40);
  // "Er... I, uh..."
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTHWEST, 0);
  FieldEvent_Wait(20);
  // "Yeah... That is weird."
  FieldEvent_ShowMessageThenWait(ACTOR_JENNA, 0, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_SOUTHEAST, 0);
  if (Event_GetFlag(FLAG_IN_PARTY(ACTOR_PIERS)))
    Actor_FaceDirection(ACTOR_PIERS, FACING_NORTH_NORTHWEST, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimation(ACTOR_SHEBA, ANIM_NOD);
  if (Event_GetFlag(FLAG_IN_PARTY(ACTOR_PIERS)))
    Actor_SetAnimation(ACTOR_PIERS, ANIM_NOD);
  Actor_AnimateThenWait(ACTOR_FELIX, ANIM_NOD, 20);
  FieldEvent_Wait(35);
  if (Event_GetFlag(FLAG_IN_PARTY(ACTOR_PIERS)))
    Actor_FaceDirection(ACTOR_PIERS, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTHEAST, 0);
  FieldEvent_Wait(25);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
  FieldEvent_Wait(20);
  Actor_SetSpeed(ACTOR_KRADEN, 1);
  Actor_WalkByAndWait(ACTOR_KRADEN, 0, 16);
  FieldEvent_Wait(20);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 30);
  // "All right, so I lied!"
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0, 0);
  Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  if (Event_GetFlag(FLAG_IN_PARTY(ACTOR_PIERS)))
    Actor_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 50);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
  FieldEvent_Wait(10);
  // "I'm glad we found werewolves! There! Are you happy!?"
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0, 0);
  Actor_SetSpeed(ACTOR_KRADEN, 2);
  Actor_WalkByAndWait(ACTOR_KRADEN, -16, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH, 0);
  if (Event_GetFlag(FLAG_IN_PARTY(ACTOR_PIERS)))
    Actor_FaceDirection(ACTOR_PIERS, FACING_NORTHWEST, 0);
  FieldEvent_Wait(20);
  Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Actor_AnimateThenWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD, 33);
  // "And you know what!? I even want to get a closer look if I can!"
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0, 0);
  Actor_AnimateThenWait(ACTOR_KRADEN, ANIM_NOD, 20);
  // "So let's go! Let's find us some werewolves!"
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0, 0);
  FieldActor_Jump(ACTOR_KRADEN, 6, 15);
  FieldActor_Jump(ACTOR_KRADEN, 6, 23);
  if (Event_GetFlag(FLAG_IN_PARTY(ACTOR_PIERS)))
    Actor_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_WINCE, 50);
  FieldEvent_Wait(40);
  Actor_FaceDirectionAndWait(ACTOR_FELIX, FACING_SOUTHEAST);
  FieldEvent_Wait(20);
  Actor_SetAnimation(ACTOR_SHEBA, ANIM_NOD);
  Actor_SetAnimation(ACTOR_FELIX, ANIM_NOD);
  if (Event_GetFlag(FLAG_IN_PARTY(ACTOR_PIERS)))
    Actor_SetAnimation(ACTOR_PIERS, ANIM_NOD);
  Actor_AnimateThenWait(ACTOR_JENNA, ANIM_NOD, 20);
  FieldEvent_Wait(30);
  Actor_SetSpeed(ACTOR_KRADEN, 1.2);
  Scene_WalkIntoLeader(ACTOR_KRADEN);
  Actor_SetSpeed(ACTOR_JENNA, 1.2);
  Scene_WalkIntoLeader(ACTOR_JENNA);
  Actor_SetSpeed(ACTOR_SHEBA, 1.2);
  Scene_WalkIntoLeader(ACTOR_SHEBA);
  if (Event_GetFlag(FLAG_IN_PARTY(ACTOR_PIERS))) {
    Actor_SetSpeed(ACTOR_PIERS, 1.2);
    Scene_WalkIntoLeader(ACTOR_PIERS);
  }
  EventCamera_FollowActor(gMainRoutineState.leaderActorId, 1);
  Event_End();
}

/// Coming down into the werewolves' cave through the cleft by the sanctum,
/// cells (17..18, 49): a howl echoes, the little werewolf from the path comes
/// into sight, Kraden cries "Look! It's that werewolf!", and it looks around
/// and runs off deeper into the cave; Kraden wants to follow it.
void Garoh_FollowWerewolfIntoCave(void) {
  Event_SetFlag(FLAG_GAROH_FOLLOWED_WEREWOLF);
  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  FieldEvent_SetMessageId((s32)MsgGarohHaowooo);
  Actor_SetPosition(ACTOR_WEREWOLF, 144, 752);
  Actor_WalkToAndWait(ACTOR_FELIX, 280, 776);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
  FieldEvent_Wait(10);
  // "Haowooo!!!"
  FieldEvent_ShowMessageThenWait(ACTOR_WEREWOLF, 0, 0);
  FieldActor_LaunchFromLeader(ACTOR_KRADEN, 16, 0, FACING_NORTH_NORTHWEST);
  FieldActor_WaitForMoveById(ACTOR_KRADEN);
  FieldEvent_Wait(20);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_EAST, 0);
  FieldEvent_Wait(20);
  FieldActor_StartRepeatedMotion(ACTOR_KRADEN, 3);
  Actor_SetAttachedEffect(ACTOR_KRADEN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldActor_StartRepeatedMotion(ACTOR_FELIX, 3);
  Actor_SetAttachedEffect(ACTOR_FELIX,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(45);
  // "Aow-aow-oooo!!!"
  FieldEvent_ShowMessageThenWait(ACTOR_WEREWOLF, 0, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
  FieldEvent_Wait(20);
  FieldCamera_MoveTo(280, CAMERA_KEEP, 752, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(20);
  Actor_SetPosition(ACTOR_WEREWOLF, 144, 720);
  Actor_SetSpeed(ACTOR_WEREWOLF, 0.8);
  Actor_WalkToAndWait(ACTOR_WEREWOLF, 168, 720);
  FieldEvent_Wait(10);
  Actor_SetSpeed(ACTOR_WEREWOLF, 2);
  FieldActor_Jump(ACTOR_WEREWOLF, 6, 0);
  Actor_WalkToAndWait(ACTOR_WEREWOLF, 200, 720);
  FieldEvent_Wait(10);
  Actor_SetSpeed(ACTOR_WEREWOLF, 0.8);
  Actor_WalkToAndWait(ACTOR_WEREWOLF, 216, 712);
  Actor_WalkToAndWait(ACTOR_WEREWOLF, 320, 712);
  // "Look! It's that werewolf!"
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 0);
  Actor_RunRepeatedMotion(ACTOR_WEREWOLF, 2);
  FieldEvent_Wait(10);
  Actor_SetSpeed(ACTOR_WEREWOLF, 0.6);
  Actor_Get(ACTOR_WEREWOLF)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkToAndWait(ACTOR_WEREWOLF, 296, 712);
  FieldEvent_Wait(1);
  Actor_Get(ACTOR_WEREWOLF)->steeringFlags |= ACTOR_FACES_MOTION;
  FieldEvent_Wait(15);
  Actor_FaceDirection(ACTOR_WEREWOLF, FACING_WEST_NORTHWEST, 0);
  FieldEvent_Wait(25);
  Actor_FaceDirection(ACTOR_WEREWOLF, FACING_EAST_NORTHEAST, 0);
  FieldEvent_Wait(25);
  Actor_FaceDirection(ACTOR_WEREWOLF, FACING_WEST_NORTHWEST, 0);
  FieldEvent_Wait(25);
  Actor_FaceDirection(ACTOR_WEREWOLF, FACING_SOUTH, 0);
  FieldEvent_Wait(25);
  Actor_ShowEmote(ACTOR_WEREWOLF, EMOTE_IN_FRONT | EMOTE_WINCE, 45);
  FieldActor_Jump(ACTOR_WEREWOLF, 6, 15);
  FieldActor_Jump(ACTOR_WEREWOLF, 6, 23);
  Actor_SetSpeed(ACTOR_WEREWOLF, 2.4);
  Actor_WalkToAndWait(ACTOR_WEREWOLF, 200, 712);
  FieldEvent_Wait(3);
  Actor_SetSpeed(ACTOR_WEREWOLF, 2);
  FieldActor_Jump(ACTOR_WEREWOLF, 6, 0);
  Actor_WalkToAndWait(ACTOR_WEREWOLF, 168, 712);
  FieldEvent_Wait(3);
  Actor_SetSpeed(ACTOR_WEREWOLF, 2.4);
  Actor_WalkToAndWait(ACTOR_WEREWOLF, 144, 712);
  Actor_SetPosition(ACTOR_WEREWOLF, 0, 0);
  FieldEvent_Wait(30);
  EventCamera_FollowActor(gMainRoutineState.leaderActorId, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(20);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  // "He was right there!"
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 0);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
  FieldEvent_Wait(10);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_WEST, 0);
  FieldEvent_Wait(20);
  // "Let's go find him, Felix!"
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_EAST, 0);
  FieldEvent_Wait(20);
  FieldActor_Jump(ACTOR_KRADEN, 6, 15);
  FieldActor_Jump(ACTOR_KRADEN, 6, 23);
  Actor_AnimateThenWait(ACTOR_FELIX, ANIM_NOD, 20);
  FieldEvent_Wait(10);
  Actor_SetSpeed(ACTOR_KRADEN, 1.2);
  Actor_SetAnimation(ACTOR_KRADEN, ANIM_WALK);
  Scene_WalkTowardLeader(ACTOR_KRADEN);
  FieldActor_WaitForMoveById(ACTOR_KRADEN);
  Actor_SetPosition(ACTOR_KRADEN, 0, 0);
  Event_End();
}

/// The first night in the werewolves' cave, at cells (19..21, 15): Kraden
/// wonders where the werewolf went, the werewolves surround the party, and
/// their elder Maha speaks with them and tells them to come back later.
void Garoh_WerewolvesRevealThemselves(void) {
  Event_SetFlag(FLAG_GAROH_MET_WEREWOLVES);
  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  FieldEvent_SetMessageId((s32)MsgGarohWait);
  Actor_SetPosition(ACTOR_MAHA, 328, 40);
  Actor_SetPosition(ACTOR_WEREWOLF, 328, 40);
  Actor_FaceDirection(ACTOR_MAHA, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_WEREWOLF, FACING_SOUTH, 0);
  Actor_WalkToAndWait(ACTOR_FELIX, 320, 224);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
  FieldEvent_Wait(10);
  // "Felix, wait..."
  FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_BELOW | ACTOR_KRADEN, 0, 0);
  FieldActor_LaunchFromLeader(ACTOR_KRADEN, -16, -8, FACING_NORTH);
  FieldActor_WaitForMoveById(ACTOR_KRADEN);
  FieldEvent_Wait(30);
  FieldCamera_SetSpeed(1);
  FieldCamera_MoveTo(328, CAMERA_KEEP, 152, 1);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTHWEST, 0);
  FieldEvent_Wait(25);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTHEAST, 0);
  FieldEvent_Wait(25);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTHWEST, 0);
  FieldEvent_Wait(25);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH, 0);
  FieldEvent_Wait(25);
  // "Where could that werewolf have gone?"
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTHEAST, 0);
  FieldCamera_MoveToActorById(ACTOR_FELIX, 1);
  FieldCamera_WaitForArrival();
  FieldCamera_SetSpeed(1.5);
  // "Do you think we lost him somewhere?" Yes: "That would be a shame..." No:
  // "I hope we didn't... Or rather, I'd be quite unhappy if we did..."
  Event_OpenMessage(ACTOR_KRADEN, 0);
  if (!FieldEvent_AskYesNoWithActorAnimation(ACTOR_FELIX, 0)) {
    FieldEvent_Wait(20);
    Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
    // "That would be a shame..."
    FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 0);
    gFieldState->message = (gFieldState->message + 1);
  } else {
    FieldEvent_Wait(30);
    Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
    gFieldState->message = (gFieldState->message + 1);
    // "I hope we didn't... Or rather, I'd be quite unhappy if we did..."
    FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 0);
  }
  // "Grrrr..."
  FieldEvent_ShowMessageThenWait(ACTOR_WEREWOLF, 0, 0);
  Actor_RunRepeatedMotion(ACTOR_FELIX, 2);
  FieldEvent_Wait(10);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH, 0);
  FieldEvent_Wait(20);
  FieldActor_LaunchFromLeader(ACTOR_JENNA, 16, 0, FACING_NORTH);
  FieldActor_LaunchFromLeader(ACTOR_SHEBA, 32, 0, FACING_NORTH);
  if (Event_GetFlag(FLAG_IN_PARTY(ACTOR_PIERS))) {
    FieldActor_LaunchFromLeader(ACTOR_PIERS, 48, -8, FACING_NORTHWEST);
  }
  FieldActor_WaitForMoveById(ACTOR_SHEBA);
  FieldEvent_Wait(20);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  // "What was that?"
  FieldEvent_ShowMessageThenWait(ACTOR_JENNA, 0, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTHWEST, 0);
  FieldEvent_Wait(20);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTHEAST, 0);
  FieldEvent_Wait(20);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTHWEST, 0);
  FieldEvent_Wait(20);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTH, 0);
  FieldEvent_Wait(20);
  // "It sounded like a wolf growling, didn't it?"
  FieldEvent_ShowMessageThenWait(ACTOR_SHEBA, 0, 0);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTHWEST, 0);
  FieldEvent_Wait(25);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTHEAST, 0);
  FieldEvent_Wait(25);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTHWEST, 0);
  FieldEvent_Wait(25);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTHEAST, 0);
  FieldEvent_Wait(25);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTHEAST, 0);
  FieldEvent_Wait(25);
  // "My hearing is not so very good, but... there was something."
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_EAST, 0);
  FieldEvent_Wait(40);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTHWEST, 0);
  FieldEvent_Wait(25);
  Actor_SetAnimation(ACTOR_SHEBA, ANIM_NOD);
  Actor_AnimateThenWait(ACTOR_JENNA, ANIM_NOD, 20);
  FieldEvent_Wait(10);
  // "Perhaps we should search this area more thoroughly..."
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 0);
  // "There is no need..."
  FieldEvent_ShowMessageThenWait(ACTOR_MAHA, 0, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTHEAST, 0);
  FieldCamera_MoveTo(328, CAMERA_KEEP, 144, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(20);
  Actor_SetPosition(ACTOR_CAVE_PASSAGE, 328, 136);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_CAVE_PASSAGE), 0);
  Garoh_OpenCavePassage(ACTOR_CAVE_PASSAGE);
  FieldEvent_Wait(20);
  // "Look... He's using Psynergy."
  FieldEvent_ShowMessageThenWait(ACTOR_JENNA, 0, 0);
  Actor_SetSpeed(ACTOR_WEREWOLF, 0.7);
  Actor_SetSpeed(ACTOR_MAHA, 0.7);
  Actor_SetPosition(ACTOR_MAHA, 328, 128);
  FieldEvent_Wait(10);
  Actor_WalkByAndWait(ACTOR_MAHA, 0, 16);
  Actor_SetPosition(ACTOR_WEREWOLF, 328, 128);
  FieldEvent_Wait(10);
  Actor_WalkBy(ACTOR_WEREWOLF, 0, 16);
  Actor_WalkByAndWait(ACTOR_MAHA, 0, 16);
  FieldEvent_Wait(10);
  Garoh_CloseCavePassage(ACTOR_CAVE_PASSAGE);
  Actor_SetPosition(ACTOR_CAVE_PASSAGE, 0, 0);
  EventCamera_FollowActor(ACTOR_MAHA, 1);
  FieldEvent_Wait(20);
  Actor_WalkBy(ACTOR_WEREWOLF, 0, 24);
  Actor_WalkByAndWait(ACTOR_MAHA, 0, 24);
  Actor_WalkByAndWait(ACTOR_WEREWOLF, 16, 8);
  Actor_FaceDirection(ACTOR_WEREWOLF, FACING_SOUTH, 0);
  FieldEvent_Wait(20);
  FieldCamera_MoveTo(328, CAMERA_KEEP, 208, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_WEREWOLF, 2);
  FieldEvent_Wait(10);
  // "Garrrrooow..."
  FieldEvent_ShowMessageThenWait(ACTOR_WEREWOLF, 0, 0);
  FieldActor_StartRepeatedMotion(ACTOR_KRADEN, 2);
  FieldActor_StartRepeatedMotion(ACTOR_SHEBA, 2);
  FieldActor_StartRepeatedMotion(ACTOR_JENNA, 2);
  if (Event_GetFlag(FLAG_IN_PARTY(ACTOR_PIERS))) {
    FieldActor_StartRepeatedMotion(ACTOR_PIERS, 2);
  }
  Actor_RunRepeatedMotion(ACTOR_FELIX, 2);
  FieldEvent_Wait(20);
  Actor_AnimateThenWait(ACTOR_MAHA, ANIM_SHAKE_HEAD, 33);
  // "Fear not... This child is harmless."
  FieldEvent_ShowMessageThenWait(ACTOR_MAHA, 0, 0);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  // "He does look kind of...small..."
  FieldEvent_ShowMessageThenWait(ACTOR_SHEBA, 0, 0);
  FieldActor_StartRepeatedMotion(ACTOR_WEREWOLF, 3);
  Actor_SetAttachedEffect(ACTOR_WEREWOLF,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(40);
  // "Grrrrrrr..."
  FieldEvent_ShowMessageThenWait(ACTOR_WEREWOLF, 0, 0);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
  FieldEvent_Wait(10);
  // "Tell me, who are you? How did you come to be here?"
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 0);
  FieldActor_Jump(ACTOR_WEREWOLF, 6, 15);
  FieldActor_Jump(ACTOR_WEREWOLF, 6, 23);
  Actor_FaceDirection(ACTOR_WEREWOLF, FACING_SOUTHWEST, 0);
  FieldEvent_Wait(20);
  Actor_AnimateThenWait(ACTOR_MAHA, ANIM_SHAKE_HEAD, 33);
  // "I should like to ask you the same question."
  FieldEvent_ShowMessageThenWait(ACTOR_MAHA, 0, 0);
  Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  if (Event_GetFlag(FLAG_IN_PARTY(ACTOR_PIERS))) {
    Actor_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  }
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Actor_RunRepeatedMotion(ACTOR_MAHA, 2);
  FieldEvent_Wait(10);
  // "Could it be? Do you outsiders possess the power to reveal the unseen?"
  // Yes: "Of course... I was certain that you must." No: "Impossible... Without
  // Reveal, you could not have entered this cave."
  Event_OpenMessage(ACTOR_MAHA, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(ACTOR_FELIX, 0)) {
    FieldEvent_Wait(40);
    Actor_AnimateThenWait(ACTOR_MAHA, ANIM_SHAKE_HEAD, 33);
    // "Impossible... Without Reveal, you could not have entered this cave."
    FieldEvent_ShowMessageThenWait(ACTOR_MAHA, 0, 0);
    gFieldState->message = (gFieldState->message + 1);
  } else {
    FieldEvent_Wait(30);
    Actor_RunRepeatedMotion(ACTOR_MAHA, 2);
    FieldEvent_Wait(10);
    gFieldState->message = (gFieldState->message + 1);
    // "Of course... I was certain that you must."
    FieldEvent_ShowMessageThenWait(ACTOR_MAHA, 0, 0);
  }
  Actor_RunRepeatedMotion(ACTOR_WEREWOLF, 2);
  FieldEvent_Wait(10);
  // "Grawooo..."
  FieldEvent_ShowMessageThenWait(ACTOR_WEREWOLF, 0, 0);
  Actor_FaceDirection(ACTOR_MAHA, FACING_EAST, 0);
  FieldEvent_Wait(30);
  Actor_AnimateThenWait(ACTOR_MAHA, ANIM_NOD, 20);
  // "Yes, you are correct. They must have passed the challenge of Air's Rock."
  FieldEvent_ShowMessageThenWait(ACTOR_MAHA, 0, 0);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  // "Er... How come that kid doesn't talk, like you?"
  FieldEvent_ShowMessageThenWait(ACTOR_JENNA, 0, 0);
  Actor_RunRepeatedMotion(ACTOR_MAHA, 2);
  FieldEvent_Wait(10);
  Actor_FaceDirection(ACTOR_WEREWOLF, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_MAHA, FACING_SOUTH, 0);
  FieldEvent_Wait(20);
  // "When we become werewolves, speaking the human tongue becomes more
  // difficult."
  FieldEvent_ShowMessageThenWait(ACTOR_MAHA, 0, 0);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 30);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTHWEST, 0);
  FieldEvent_Wait(20);
  // "But you can... You must be someone special, then..."
  FieldEvent_ShowMessageThenWait(ACTOR_SHEBA, 0, 0);
  Actor_AnimateThenWait(ACTOR_MAHA, ANIM_SHAKE_HEAD, 33);
  // "Hurh hurr hrrr... 'Special'? Perhaps. Why don't you
  // just call me Maha..."
  FieldEvent_ShowMessageThenWait(ACTOR_MAHA, 0, 0);
  Actor_ShowEmote(ACTOR_MAHA, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  // "So... I assume you also can use Whirlwind..."
  FieldEvent_ShowMessageThenWait(ACTOR_MAHA, 0, 0);
  Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  if (Event_GetFlag(FLAG_IN_PARTY(ACTOR_PIERS))) {
    Actor_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  }
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Actor_AnimateThenWait(ACTOR_MAHA, ANIM_NOD, 20);
  // "If you learned Reveal on Air's Rock, you must also know Whirlwind." Yes:
  // "That's right... I think we can tell Maha that we're Adepts." No: "I don't
  // think we need to hide the fact that we're Adepts from Maha..."
  FieldMessage_ShowAtActor(ACTOR_MAHA, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_WEST, 0);
  if (Event_GetFlag(FLAG_IN_PARTY(ACTOR_PIERS))) {
    Actor_FaceDirection(ACTOR_PIERS, FACING_WEST, 0);
  }
  FieldEvent_Wait(20);
  if (FieldEvent_AskYesNoWithActorAnimation(ACTOR_FELIX, 0)) {
    FieldEvent_Wait(40);
    Actor_RunRepeatedMotion(ACTOR_JENNA, 2);
    FieldEvent_Wait(20);
    // "I don't think we need to hide the fact that we're Adepts from Maha..."
    FieldEvent_ShowMessageThenWait(ACTOR_JENNA, 0, 0);
    gFieldState->message = (gFieldState->message + 1);
  } else {
    FieldEvent_Wait(30);
    Actor_AnimateThenWait(ACTOR_JENNA, ANIM_NOD, 20);
    FieldEvent_Wait(20);
    gFieldState->message = (gFieldState->message + 1);
    // "That's right... I think we can tell Maha that we're Adepts."
    FieldEvent_ShowMessageThenWait(ACTOR_JENNA, 0, 0);
  }
  Actor_FaceDirection(ACTOR_MAHA, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_WEREWOLF, FACING_SOUTHWEST, 0);
  FieldEvent_Wait(30);
  // "Awooo..."
  FieldEvent_ShowMessageThenWait(ACTOR_WEREWOLF, 0, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTH, 0);
  if (Event_GetFlag(FLAG_IN_PARTY(ACTOR_PIERS))) {
    Actor_FaceDirection(ACTOR_PIERS, FACING_NORTHWEST, 0);
  }
  FieldEvent_Wait(30);
  Actor_RunRepeatedMotion(ACTOR_SHEBA, 2);
  FieldEvent_Wait(10);
  // "What did he say? I don't understand..."
  FieldEvent_ShowMessageThenWait(ACTOR_SHEBA, 0, 0);
  Actor_FaceDirection(ACTOR_MAHA, FACING_SOUTH, 0);
  FieldEvent_Wait(30);
  // "He wanted to know what an Adept is... A question I would like answered as
  // well."
  FieldEvent_ShowMessageThenWait(ACTOR_MAHA, 0, 0);
  Actor_AnimateThenWait(ACTOR_KRADEN, ANIM_NOD, 20);
  // "Adepts tap the power of elemental energies to generate forces like
  // Whirlwind..."
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 0);
  Actor_ShowEmote(ACTOR_MAHA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  // "Elementals... You mean the four elements, the powers of earth and fire,
  // water and wind?"
  FieldEvent_ShowMessageThenWait(ACTOR_MAHA, 0, 0);
  Actor_SetAnimation(ACTOR_FELIX, ANIM_NOD);
  Actor_SetAnimation(ACTOR_JENNA, ANIM_NOD);
  FieldActor_SetAnimation(ACTOR_SHEBA, ANIM_NOD);
  if (Event_GetFlag(FLAG_IN_PARTY(ACTOR_PIERS))) {
    FieldActor_SetAnimation(ACTOR_PIERS, ANIM_NOD);
  }
  FieldActor_SetAnimation(ACTOR_KRADEN, ANIM_NOD);
  FieldEvent_Wait(20);
  FieldEvent_Wait(20);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTHEAST, 0);
  FieldEvent_Wait(20);
  // "That is correct. These elements together comprise everything in our
  // world..."
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTHEAST, 0);
  FieldEvent_Wait(30);
  Actor_AnimateThenWait(ACTOR_MAHA, ANIM_NOD, 20);
  // "Interesting. Very interesting..."
  FieldEvent_ShowMessageThenWait(ACTOR_MAHA, 0, 0);
  Actor_ShowEmote(ACTOR_WEREWOLF, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  // "Awooo..."
  FieldEvent_ShowMessageThenWait(ACTOR_WEREWOLF, 0, 0);
  Actor_FaceDirection(ACTOR_MAHA, FACING_NORTHEAST, 0);
  FieldEvent_Wait(30);
  Actor_RunRepeatedMotion(ACTOR_JENNA, 2);
  FieldEvent_Wait(10);
  // "Um... What did he say that time?"
  FieldEvent_ShowMessageThenWait(ACTOR_JENNA, 0, 0);
  Actor_FaceDirection(ACTOR_MAHA, FACING_SOUTH_SOUTHEAST, 0);
  FieldEvent_Wait(20);
  // "He asked if there were no longer any need to hide from you..."
  FieldEvent_ShowMessageThenWait(ACTOR_MAHA, 0, 0);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
  // "...Why did he have to hide in here in the first place?"
  FieldEvent_ShowMessageThenWait(ACTOR_SHEBA, 0, 0);
  Actor_FaceDirection(ACTOR_WEREWOLF, FACING_SOUTH, 0);
  FieldEvent_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_WEREWOLF, 2);
  FieldEvent_Wait(10);
  // "Aaah-Ah-Awoooh! Hawawoooh!"
  FieldEvent_ShowMessageThenWait(ACTOR_WEREWOLF, 0, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_EAST, 0);
  FieldEvent_Wait(20);
  // "No doubt, most would find the sight of a werewolf rather unnerving..."
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTHEAST, 0);
  FieldEvent_Wait(30);
  Actor_AnimateThenWait(ACTOR_MAHA, ANIM_NOD, 20);
  // "You are, sadly, correct. Many consider ours an accursed race."
  FieldEvent_ShowMessageThenWait(ACTOR_MAHA, 0, 0);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  // "And if they found you in your werewolf form?"
  FieldEvent_ShowMessageThenWait(ACTOR_JENNA, 0, 0);
  Actor_RunRepeatedMotion(ACTOR_MAHA, 2);
  FieldEvent_Wait(10);
  // "...Once, our people were burned at the stake as abominations... They
  // called this 'purification.'"
  FieldEvent_ShowMessageThenWait(ACTOR_MAHA, 0, 0);
  Actor_AnimateThenWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD, 33);
  // "Horrible..."
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 0);
  Actor_FaceDirection(ACTOR_WEREWOLF, FACING_SOUTH, 0);
  FieldEvent_Wait(20);
  Actor_WalkByAndWait(ACTOR_WEREWOLF, 0, 16);
  FieldEvent_Wait(10);
  FieldActor_Jump(ACTOR_WEREWOLF, 6, 15);
  FieldActor_Jump(ACTOR_WEREWOLF, 6, 23);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  // "But...this little one... He won't get excited and try to bite, will he?"
  FieldEvent_ShowMessageThenWait(ACTOR_SHEBA, 0, 0);
  Actor_SetSpeed(ACTOR_MAHA, 2.5);
  Actor_SetSpeed(ACTOR_WEREWOLF, 2.5);
  Actor_FaceDirection(ACTOR_MAHA, FACING_EAST, 0);
  FieldEvent_Wait(20);
  Actor_WalkByAndWait(ACTOR_MAHA, 8, 0);
  FieldActor_Jump(ACTOR_WEREWOLF, 4, 0);
  Actor_Get(ACTOR_MAHA)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkBy(ACTOR_MAHA, -8, 0);
  Actor_Get(ACTOR_WEREWOLF)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkByAndWait(ACTOR_WEREWOLF, 16, 0);
  FieldEvent_Wait(1);
  Actor_Get(ACTOR_WEREWOLF)->steeringFlags |= ACTOR_FACES_MOTION;
  FieldActor_SetAnimation(ACTOR_WEREWOLF, ANIM_WEREWOLF_SIT);
  FieldEvent_Wait(20);
  Actor_SetSpeed(ACTOR_WEREWOLF, 1.3);
  Actor_SetSpeed(ACTOR_MAHA, 0.7);
  Actor_AnimateThenWait(ACTOR_MAHA, ANIM_SHAKE_HEAD, 33);
  FieldEvent_Wait(10);
  Actor_ShowEmote(ACTOR_WEREWOLF, EMOTE_IN_FRONT | EMOTE_WINCE, 50);
  Actor_SetAnimation(ACTOR_WEREWOLF, ANIM_STAND);
  FieldActor_Jump(ACTOR_WEREWOLF, 6, 0);
  Actor_FaceDirection(ACTOR_WEREWOLF, FACING_WEST, 0);
  FieldEvent_Wait(30);
  Actor_SetAttachedEffect(ACTOR_WEREWOLF,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_AnimateThenWait(ACTOR_WEREWOLF, ANIM_NOD, 20);
  FieldEvent_Wait(15);
  Actor_SetAttachedEffect(ACTOR_WEREWOLF,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_AnimateThenWait(ACTOR_WEREWOLF, ANIM_NOD, 20);
  FieldEvent_Wait(20);
  Actor_AnimateThenWait(ACTOR_MAHA, ANIM_NOD, 20);
  FieldEvent_Wait(10);
  // "Child, return to your home."
  FieldEvent_ShowMessageThenWait(ACTOR_MAHA, 0, 0);
  Actor_ShowEmote(ACTOR_WEREWOLF, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  // "Arrooo?"
  FieldEvent_ShowMessageThenWait(ACTOR_WEREWOLF, 0, 0);
  Actor_AnimateThenWait(ACTOR_MAHA, ANIM_SHAKE_HEAD, 33);
  // "Don't worry. We can trust them... Go back home now."
  FieldEvent_ShowMessageThenWait(ACTOR_MAHA, 0, 0);
  Actor_FaceDirection(ACTOR_WEREWOLF, FACING_SOUTH_SOUTHWEST, 0);
  FieldEvent_Wait(45);
  Actor_FaceDirection(ACTOR_WEREWOLF, FACING_WEST, 0);
  FieldEvent_Wait(20);
  // "Aarrrooo?"
  FieldEvent_ShowMessageThenWait(ACTOR_WEREWOLF, 0, 0);
  Actor_AnimateThenWait(ACTOR_MAHA, ANIM_SHAKE_HEAD, 33);
  // "They are Adepts, not unlike us werewolves... They guard an ancient
  // secret."
  FieldEvent_ShowMessageThenWait(ACTOR_MAHA, 0, 0);
  Actor_FaceDirection(ACTOR_MAHA, FACING_SOUTH_SOUTHWEST, 0);
  FieldEvent_Wait(45);
  Actor_FaceDirection(ACTOR_MAHA, FACING_EAST, 0);
  FieldEvent_Wait(20);
  // "We can trust them. They will not betray us to the world."
  FieldEvent_ShowMessageThenWait(ACTOR_MAHA, 0, 0);
  Actor_AnimateThenWait(ACTOR_WEREWOLF, ANIM_NOD, 20);
  FieldEvent_Wait(20);
  Actor_FaceDirection(ACTOR_WEREWOLF, FACING_SOUTH, 0);
  FieldEvent_Wait(35);
  Actor_AnimateThenWait(ACTOR_WEREWOLF, ANIM_NOD, 20);
  FieldEvent_Wait(20);
  // "Woof."
  FieldEvent_ShowMessageThenWait(ACTOR_WEREWOLF, 0, 0);
  Actor_SetAnimation(ACTOR_FELIX, ANIM_NOD);
  Actor_SetAnimation(ACTOR_JENNA, ANIM_NOD);
  Actor_SetAnimation(ACTOR_SHEBA, ANIM_NOD);
  Actor_AnimateThenWait(ACTOR_KRADEN, ANIM_NOD, 20);
  FieldEvent_Wait(20);
  FieldActor_WatchAndSetScript(ACTOR_FELIX, ACTOR_WEREWOLF);
  FieldActor_WatchAndSetScript(ACTOR_KRADEN, ACTOR_WEREWOLF);
  FieldActor_WatchAndSetScript(ACTOR_JENNA, ACTOR_WEREWOLF);
  FieldActor_WatchAndSetScript(ACTOR_SHEBA, ACTOR_WEREWOLF);
  FieldActor_WatchAndSetScript(ACTOR_MAHA, ACTOR_WEREWOLF);
  if (Event_GetFlag(FLAG_IN_PARTY(ACTOR_PIERS))) {
    FieldActor_WatchAndSetScript(ACTOR_PIERS, ACTOR_WEREWOLF);
  }
  Actor_WalkToAndWait(ACTOR_WEREWOLF, 376, 208);
  Actor_WalkByAndWait(ACTOR_WEREWOLF, 0, 16);
  Actor_WalkByAndWait(ACTOR_WEREWOLF, -16, 24);
  Actor_WalkByAndWait(ACTOR_WEREWOLF, -32, 16);
  Actor_WalkByAndWait(ACTOR_WEREWOLF, 0, 48);
  FieldEvent_Wait(20);
  FieldActor_Stop(ACTOR_FELIX);
  FieldActor_Stop(ACTOR_KRADEN);
  FieldActor_Stop(ACTOR_JENNA);
  FieldActor_Stop(ACTOR_SHEBA);
  FieldActor_Stop(ACTOR_MAHA);
  if (Event_GetFlag(FLAG_IN_PARTY(ACTOR_PIERS))) {
    FieldActor_Stop(ACTOR_PIERS);
  }
  Actor_SetPosition(ACTOR_WEREWOLF, 0, 0);
  Actor_RunRepeatedMotion(ACTOR_MAHA, 2);
  FieldEvent_Wait(10);
  // "It has grown quite late."
  FieldEvent_ShowMessageThenWait(ACTOR_MAHA, 0, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
  if (Event_GetFlag(FLAG_IN_PARTY(ACTOR_PIERS))) {
    Actor_FaceDirection(ACTOR_PIERS, FACING_NORTHWEST, 0);
  }
  FieldEvent_Wait(30);
  Actor_AnimateThenWait(ACTOR_MAHA, ANIM_NOD, 20);
  // "Why don't you go to the inn and rest..."
  FieldEvent_ShowMessageThenWait(ACTOR_MAHA, 0, 0);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  // "But then..."
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 0);
  Actor_ShowEmote(ACTOR_MAHA, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  // "I will not leave this place."
  FieldEvent_ShowMessageThenWait(ACTOR_MAHA, 0, 0);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  // "You aren't going home?"
  FieldEvent_ShowMessageThenWait(ACTOR_SHEBA, 0, 0);
  Actor_ShowEmote(ACTOR_MAHA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
  // "When I learned Reveal, I lost my human form. The wild is my home now."
  FieldEvent_ShowMessageThenWait(ACTOR_MAHA, 0, 0);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  // "So, Master Maha, you always look this way?"
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 0);
  Actor_AnimateThenWait(ACTOR_MAHA, ANIM_NOD, 20);
  FieldEvent_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_JENNA, 2);
  FieldEvent_Wait(10);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
  FieldEvent_Wait(30);
  // "And the little one?"
  FieldEvent_ShowMessageThenWait(ACTOR_JENNA, 0, 0);
  Actor_AnimateThenWait(ACTOR_MAHA, ANIM_SHAKE_HEAD, 33);
  // "The child changed because he looked directly at the full moon."
  FieldEvent_ShowMessageThenWait(ACTOR_MAHA, 0, 0);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 50);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTH_NORTHWEST, 0);
  FieldEvent_Wait(30);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTHEAST, 0);
  FieldEvent_Wait(20);
  // "So, by tomorrow morning, he will have returned to his human form again?"
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 0);
  Actor_AnimateThenWait(ACTOR_MAHA, ANIM_NOD, 20);
  FieldEvent_Wait(10);
  // "Correct. Now, let us retire."
  FieldEvent_ShowMessageThenWait(ACTOR_MAHA, 0, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTHEAST, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimation(ACTOR_FELIX, ANIM_NOD);
  Actor_SetAnimation(ACTOR_JENNA, ANIM_NOD);
  Actor_SetAnimation(ACTOR_SHEBA, ANIM_NOD);
  Actor_AnimateThenWait(ACTOR_KRADEN, ANIM_NOD, 20);
  FieldEvent_Wait(20);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_WEST, 0);
  if (Event_GetFlag(FLAG_IN_PARTY(ACTOR_PIERS))) {
    Actor_FaceDirection(ACTOR_PIERS, FACING_WEST, 0);
  }
  FieldEvent_Wait(20);
  Actor_SetAnimation(ACTOR_JENNA, ANIM_NOD);
  FieldActor_SetAnimation(ACTOR_SHEBA, ANIM_NOD);
  if (Event_GetFlag(FLAG_IN_PARTY(ACTOR_PIERS))) {
    FieldActor_SetAnimation(ACTOR_PIERS, ANIM_NOD);
  }
  FieldActor_SetAnimation(ACTOR_KRADEN, ANIM_NOD);
  FieldEvent_Wait(20);
  FieldEvent_Wait(20);
  Actor_AnimateThenWait(ACTOR_FELIX, ANIM_NOD, 20);
  FieldEvent_Wait(10);
  Actor_SetSpeed(ACTOR_KRADEN, 1.2);
  FieldActor_SetAnimation(ACTOR_KRADEN, ANIM_WALK);
  Scene_WalkTowardLeader(ACTOR_KRADEN);
  FieldActor_WaitForMoveById(ACTOR_KRADEN);
  Actor_SetPosition(ACTOR_KRADEN, 0, 0);
  Actor_SetSpeed(ACTOR_JENNA, 1.2);
  FieldActor_SetAnimation(ACTOR_JENNA, ANIM_WALK);
  Scene_WalkTowardLeader(ACTOR_JENNA);
  FieldActor_WaitForMoveById(ACTOR_JENNA);
  Actor_SetPosition(ACTOR_JENNA, 0, 0);
  Actor_SetSpeed(ACTOR_SHEBA, 1.2);
  FieldActor_SetAnimation(ACTOR_SHEBA, ANIM_WALK);
  Scene_WalkTowardLeader(ACTOR_SHEBA);
  FieldActor_WaitForMoveById(ACTOR_SHEBA);
  Actor_SetPosition(ACTOR_SHEBA, 0, 0);
  if (Event_GetFlag(FLAG_IN_PARTY(ACTOR_PIERS))) {
    Actor_SetSpeed(ACTOR_PIERS, 1.2);
    FieldActor_SetAnimation(ACTOR_PIERS, ANIM_WALK);
    Scene_WalkTowardLeader(ACTOR_PIERS);
    FieldActor_WaitForMoveById(ACTOR_PIERS);
    Actor_SetPosition(ACTOR_PIERS, 0, 0);
  }
  EventCamera_FollowActor(gMainRoutineState.leaderActorId, 1);
  Garoh_MetWerewolves();
  FieldEvent_SetExitRequest(GAROH_WEREWOLF_CAVE_TO_GAROH_ROOMS_HOUSES);
  Event_End();
}

/// By day in the werewolves' cave, at cells (19..21, 15): "Ah, you came...
/// I've been waiting for you." Maha tells the werewolves' legend and of
/// Mt. Aleph, and gives the party the Jupiter Djinni Ether.
void Garoh_MahaTellsTheLegend(void) {
  s32 frames;
  FIXED total;
  struct Actor *ether;
  struct Sprite *sprite;

  Event_SetFlag(FLAG_GAROH_HEARD_MAHA);
  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  FieldEvent_SetMessageId((s32)MsgGarohAhYouCame);
  Actor_SetPosition(ACTOR_MAHA, 328, 40);
  Actor_FaceDirection(ACTOR_MAHA, FACING_SOUTH, 0);
  // "Ah, you came..."
  FieldEvent_ShowMessageThenWait(ACTOR_MAHA, 0, 0);
  Actor_SetSpeed(ACTOR_FELIX, 0.8);
  Actor_WalkToAndWait(ACTOR_FELIX, 320, 224);
  FieldActor_LaunchFromLeader(ACTOR_KRADEN, -16, 0, FACING_NORTH);
  FieldActor_LaunchFromLeader(ACTOR_JENNA, 16, 0, FACING_NORTH);
  FieldActor_LaunchFromLeader(ACTOR_SHEBA, 32, 0, FACING_NORTH);
  if (Event_GetFlag(FLAG_IN_PARTY(ACTOR_PIERS))) {
    FieldActor_LaunchFromLeader(ACTOR_PIERS, 48, 0, FACING_NORTH);
  }
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
  FieldActor_WaitForMoveById(ACTOR_SHEBA);
  FieldEvent_Wait(20);
  FieldCamera_MoveTo(328, CAMERA_KEEP, 144, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(20);
  Actor_SetPosition(ACTOR_CAVE_PASSAGE, 328, 136);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_CAVE_PASSAGE), 0);
  Garoh_OpenCavePassage(ACTOR_CAVE_PASSAGE);
  FieldEvent_Wait(30);
  Actor_SetSpeed(ACTOR_MAHA, 0.7);
  Actor_SetPosition(ACTOR_MAHA, 328, 128);
  FieldEvent_Wait(10);
  Actor_WalkByAndWait(ACTOR_MAHA, 0, 16);
  FieldEvent_Wait(20);
  Garoh_CloseCavePassage(ACTOR_CAVE_PASSAGE);
  Actor_SetPosition(ACTOR_CAVE_PASSAGE, 0, 0);
  EventCamera_FollowActor(ACTOR_MAHA, 1);
  FieldEvent_Wait(20);
  Actor_WalkByAndWait(ACTOR_MAHA, 0, 48);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTHWEST, 0);
  if (Event_GetFlag(FLAG_IN_PARTY(ACTOR_PIERS))) {
    Actor_FaceDirection(ACTOR_PIERS, FACING_NORTHWEST, 0);
  }
  FieldCamera_MoveTo(328, CAMERA_KEEP, 184, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(10);
  Actor_AnimateThenWait(ACTOR_MAHA, ANIM_NOD, 20);
  FieldEvent_Wait(10);
  // "I've been waiting for you."
  FieldEvent_ShowMessageThenWait(
      SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_ABOVE | ACTOR_MAHA, 0, 0);
  Actor_SetAnimation(ACTOR_FELIX, ANIM_NOD);
  Actor_SetAnimation(ACTOR_JENNA, ANIM_NOD);
  Actor_SetAnimation(ACTOR_SHEBA, ANIM_NOD);
  if (Event_GetFlag(FLAG_IN_PARTY(ACTOR_PIERS))) {
    Actor_SetAnimation(ACTOR_PIERS, ANIM_NOD);
  }
  Actor_SetAnimation(ACTOR_KRADEN, ANIM_NOD);
  FieldEvent_Wait(20);
  FieldEvent_Wait(20);
  Actor_WalkByAndWait(ACTOR_KRADEN, 0, -16);
  FieldEvent_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
  FieldEvent_Wait(10);
  // "Would you mind telling me a little more about werewolves?"
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 0);
  Actor_WalkByAndWait(ACTOR_MAHA, -8, 0);
  Actor_FaceDirection(ACTOR_MAHA, FACING_SOUTH_SOUTHWEST, 0);
  FieldEvent_Wait(20);
  // "You want to know the reason we possess the power to transform into
  // beasts?"
  FieldEvent_ShowMessageThenWait(
      SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_ABOVE | ACTOR_MAHA, 0, 0);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  // "Ah... Well, I, er... Actually, yes."
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 0);
  Actor_ShowEmote(ACTOR_MAHA, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
  // "Do you find it strange that we can change our shapes?"
  FieldEvent_ShowMessageThenWait(
      SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_ABOVE | ACTOR_MAHA, 0, 0);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
  // "Well, ummm... Yes."
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 0);
  Actor_RunRepeatedMotion(ACTOR_MAHA, 2);
  FieldEvent_Wait(10);
  // "But surely changing one's shape is not really all that unusual."
  FieldEvent_ShowMessageThenWait(
      SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_ABOVE | ACTOR_MAHA, 0, 0);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  // "What are you getting at?"
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 0);
  Actor_WalkByAndWait(ACTOR_MAHA, 16, 0);
  Actor_FaceDirection(ACTOR_MAHA, FACING_SOUTH_SOUTHWEST, 0);
  FieldEvent_Wait(20);
  // "Think about the caterpillar for a moment. When the time is right, does it
  // not transform into a beautiful butterfly?"
  FieldEvent_ShowMessageThenWait(
      SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_ABOVE | ACTOR_MAHA, 0, 0);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  // "Well, yes. It's only natural..."
  FieldEvent_ShowMessageThenWait(ACTOR_JENNA, 0, 0);
  Actor_FaceDirection(ACTOR_MAHA, FACING_SOUTHEAST, 0);
  FieldEvent_Wait(20);
  // "And the pill bug and hedgehog? Do they not change their shapes to protect
  // themselves?"
  FieldEvent_ShowMessageThenWait(
      SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_ABOVE | ACTOR_MAHA, 0, 0);
  Actor_RunRepeatedMotion(ACTOR_SHEBA, 2);
  FieldEvent_Wait(10);
  // "I have heard of these creatures, yes..."
  FieldEvent_ShowMessageThenWait(ACTOR_SHEBA, 0, 0);
  Actor_WalkByAndWait(ACTOR_MAHA, -8, 0);
  Actor_FaceDirection(ACTOR_MAHA, FACING_SOUTH_SOUTHWEST, 0);
  FieldEvent_Wait(20);
  // "And many amphibians possess the power to change color at will, correct?"
  FieldEvent_ShowMessageThenWait(
      SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_ABOVE | ACTOR_MAHA, 0, 0);
  Actor_AnimateThenWait(ACTOR_FELIX, ANIM_NOD, 20);
  FieldEvent_Wait(35);
  Actor_WalkByAndWait(ACTOR_MAHA, -8, 0);
  Actor_FaceDirection(ACTOR_MAHA, FACING_SOUTH_SOUTHWEST, 0);
  FieldEvent_Wait(25);
  Actor_AnimateThenWait(ACTOR_MAHA, ANIM_NOD, 20);
  FieldEvent_Wait(10);
  // "In light of all this, do you mean to tell me you find werewolves so
  // abnormal?"
  FieldEvent_ShowMessageThenWait(
      SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_ABOVE | ACTOR_MAHA, 0, 0);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  // "Hrmm, well... To be honest, I hadn't thought of it that way before..."
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 0);
  Actor_AnimateThenWait(ACTOR_MAHA, ANIM_SHAKE_HEAD, 33);
  // "Perhaps you simply did not give it enough thought."
  FieldEvent_ShowMessageThenWait(
      SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_ABOVE | ACTOR_MAHA, 0, 0);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 45);
  FieldActor_Jump(ACTOR_KRADEN, 6, 15);
  FieldActor_Jump(ACTOR_KRADEN, 6, 23);
  // "How insulting! I, sir... am a SCIENTIST!"
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 0);
  Actor_ShowEmote(ACTOR_MAHA, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  // "Indeed! A scientist? That is impressive."
  FieldEvent_ShowMessageThenWait(
      SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_ABOVE | ACTOR_MAHA, 0, 0);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
  FieldEvent_Wait(10);
  Actor_FaceDirection(ACTOR_MAHA, FACING_NORTH, 0);
  FieldEvent_Wait(30);
  // "I have spent a great deal of time thinking about werewolves."
  FieldEvent_ShowMessageThenWait(
      SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_ABOVE | ACTOR_MAHA, 0, 0);
  Actor_WalkByAndWait(ACTOR_MAHA, 4, -12);
  FieldEvent_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_MAHA, 2);
  FieldEvent_Wait(10);
  // "For a time, I even felt cursed to have been born a werewolf..."
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_MAHA, 0, 0);
  Actor_WalkByAndWait(ACTOR_MAHA, 4, -12);
  FieldEvent_Wait(10);
  Actor_AnimateThenWait(ACTOR_MAHA, ANIM_SHAKE_HEAD, 33);
  // "But after taking a more objective look at nature..."
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_MAHA, 0, 0);
  Actor_FaceDirection(ACTOR_MAHA, FACING_SOUTH, 0);
  FieldEvent_Wait(30);
  // "I realized that weaker species have survived by evolving...by
  // transforming. Don't you agree?"
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_MAHA, 0, 0);
  Actor_SetAnimation(ACTOR_FELIX, ANIM_NOD);
  Actor_SetAnimation(ACTOR_JENNA, ANIM_NOD);
  Actor_SetAnimation(ACTOR_SHEBA, ANIM_NOD);
  Actor_AnimateThenWait(ACTOR_KRADEN, ANIM_NOD, 20);
  FieldEvent_Wait(20);
  Actor_AnimateThenWait(ACTOR_MAHA, ANIM_NOD, 20);
  FieldEvent_Wait(10);
  // "Think about it for a moment. Without changing their bodies with weapons
  // and armor... Humans would be very weak creatures indeed."
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_MAHA, 0, 0);
  Actor_RunRepeatedMotion(ACTOR_MAHA, 2);
  FieldEvent_Wait(10);
  // "I believe that evolving into werewolves is just one way for humanity to
  // survive..."
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_MAHA, 0, 0);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
  FieldEvent_Wait(10);
  // "But that's..."
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 0);
  Actor_ShowEmote(ACTOR_MAHA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 30);
  // "Of course, there's no way I can prove this theory."
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_MAHA, 0, 0);
  Actor_RunRepeatedMotion(ACTOR_MAHA, 2);
  FieldEvent_Wait(10);
  // "But I felt that our ability to use Whirlwind was evidence of this."
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_MAHA, 0, 0);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  // "Whirlwind? Why?"
  FieldEvent_ShowMessageThenWait(ACTOR_SHEBA, 0, 0);
  Actor_FaceDirection(ACTOR_MAHA, FACING_SOUTH_SOUTHEAST, 0);
  FieldEvent_Wait(20);
  Actor_AnimateThenWait(ACTOR_MAHA, ANIM_NOD, 20);
  FieldEvent_Wait(10);
  // "I had thought only the people of Garoh could use Whirlwind."
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_MAHA, 0, 0);
  Actor_ShowEmote(ACTOR_MAHA, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  // "It was a gift known only to werewolves, to Garoh's pack..."
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_MAHA, 0, 0);
  Actor_AnimateThenWait(ACTOR_KRADEN, ANIM_NOD, 20);
  FieldEvent_Wait(10);
  // "And that only a chosen few could use Reveal..."
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 0);
  Actor_FaceDirection(ACTOR_MAHA, FACING_SOUTH_SOUTHWEST, 0);
  FieldEvent_Wait(20);
  Actor_AnimateThenWait(ACTOR_MAHA, ANIM_SHAKE_HEAD, 33);
  // "But now I find that you, too, can use these... What does it all mean?"
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_MAHA, 0, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_EAST, 0);
  if (Event_GetFlag(FLAG_IN_PARTY(ACTOR_PIERS))) {
    Actor_FaceDirection(ACTOR_PIERS, FACING_WEST, 0);
  }
  FieldEvent_Wait(40);
  Actor_RunRepeatedMotion(ACTOR_MAHA, 2);
  FieldEvent_Wait(10);
  // "Why are you able to use Whirlwind, and what is an Adept?"
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_MAHA, 0, 0);
  if (Event_GetFlag(FLAG_IN_PARTY(ACTOR_PIERS))) {
    Actor_FaceDirection(ACTOR_PIERS, FACING_NORTHWEST, 0);
  }
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTHWEST, 0);
  FieldEvent_Wait(25);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
  FieldEvent_Wait(10);
  // "Master Maha, do you know of a continent called Angara? It lies far north
  // of the Eastern Sea."
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 0);
  Actor_ShowEmote(ACTOR_MAHA, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  // "Angara?"
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_MAHA, 0, 0);
  Actor_AnimateThenWait(ACTOR_KRADEN, ANIM_NOD, 20);
  FieldEvent_Wait(10);
  // "In the middle of Angara, there is a peak called Mt. Aleph."
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 0);
  Actor_AnimateThenWait(ACTOR_MAHA, ANIM_NOD, 20);
  FieldEvent_Wait(10);
  Actor_AnimateThenWait(ACTOR_MAHA, ANIM_NOD, 20);
  FieldEvent_Wait(10);
  // "Mt. Aleph? ...Then it's true! Our legends say this peak has stood since
  // this world began!"
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_MAHA, 0, 0);
  Actor_ShowEmote(ACTOR_MAHA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  // "If Mt. Aleph does exist, then perhaps our legends are true!"
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_MAHA, 0, 0);
  Actor_AnimateThenWait(ACTOR_KRADEN, ANIM_NOD, 20);
  FieldEvent_Wait(10);
  // "It does exist..."
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTHEAST, 0);
  FieldEvent_Wait(30);
  Actor_AnimateThenWait(ACTOR_KRADEN, ANIM_NOD, 20);
  FieldEvent_Wait(10);
  // "These two hail from the village at the base of Mt. Aleph, a village called
  // Vale."
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 0);
  Actor_RunRepeatedMotion(ACTOR_MAHA, 2);
  FieldEvent_Wait(15);
  Actor_FaceDirection(ACTOR_MAHA, FACING_NORTH, 0);
  FieldEvent_Wait(30);
  Actor_AnimateThenWait(ACTOR_MAHA, ANIM_NOD, 20);
  FieldEvent_Wait(10);
  // "They come from Mt. Aleph!? Incredible!"
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_MAHA, 0, 0);
  Actor_SetAnimation(ACTOR_FELIX, ANIM_NOD);
  Actor_AnimateThenWait(ACTOR_JENNA, ANIM_NOD, 20);
  FieldEvent_Wait(30);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTHEAST, 0);
  FieldEvent_Wait(25);
  Actor_AnimateThenWait(ACTOR_KRADEN, ANIM_NOD, 20);
  FieldEvent_Wait(10);
  // "And they are both Adepts. And while their powers may vary... Everyone from
  // Vale is an Adept to a certain degree."
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 0);
  FieldActor_StartRepeatedMotion(ACTOR_MAHA, 3);
  Actor_SetAttachedEffect(ACTOR_MAHA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(40);
  Actor_FaceDirection(ACTOR_MAHA, FACING_SOUTH_SOUTHWEST, 0);
  FieldEvent_Wait(30);
  // "Then, does that not make them like us in Garoh?"
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_MAHA, 0, 0);
  Actor_AnimateThenWait(ACTOR_KRADEN, ANIM_NOD, 20);
  FieldEvent_Wait(10);
  // "In Vale, the myths say that Mt. Aleph itself gave birth to the Adepts..."
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 0);
  Actor_ShowEmote(ACTOR_MAHA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  // "This is not what the legends of Garoh tell us..."
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_MAHA, 0, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_EAST, 0);
  FieldEvent_Wait(20);
  // "As a people, they are quite gifted...but it goes far beyond that..."
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 0);
  Actor_WalkByAndWait(ACTOR_KRADEN, 16, 0);
  FieldEvent_Wait(20);
  Actor_WalkByAndWait(ACTOR_KRADEN, 16, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTHEAST, 0);
  FieldEvent_Wait(20);
  Actor_WalkByAndWait(ACTOR_KRADEN, 16, 0);
  Actor_FaceDirection(ACTOR_MAHA, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTH, 0);
  if (Event_GetFlag(FLAG_IN_PARTY(ACTOR_PIERS))) {
    Actor_FaceDirection(ACTOR_PIERS, FACING_NORTH, 0);
  }
  FieldEvent_Wait(20);
  Actor_WalkByAndWait(ACTOR_KRADEN, 16, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTHEAST, 0);
  FieldEvent_Wait(20);
  FieldCamera_MoveTo(344, CAMERA_KEEP, 208, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(20);
  // "The mystical stones on Mt. Aleph... The Psynergy Stones... They are
  // closely linked to the powers of the townsfolk."
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 0);
  Actor_SetSpeed(ACTOR_MAHA, 1.2);
  Actor_WalkByAndWait(ACTOR_MAHA, 16, 16);
  Actor_SetSpeed(ACTOR_MAHA, 0.7);
  // "'Psynergy Stones,' you say?"
  FieldEvent_ShowMessageThenWait(ACTOR_MAHA, 0, 0);
  Actor_AnimateThenWait(ACTOR_KRADEN, ANIM_NOD, 20);
  FieldEvent_Wait(10);
  if (Event_GetFlag(FLAG_IN_PARTY(ACTOR_PIERS))) {
    Actor_FaceDirection(ACTOR_PIERS, FACING_NORTHWEST, 0);
  }
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTHWEST, 0);
  FieldEvent_Wait(30);
  // "If there were similar stones in Garoh, it might explain your powers as
  // well."
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 0);
  Actor_RunRepeatedMotion(ACTOR_MAHA, 2);
  FieldEvent_Wait(10);
  // "And what manner of stones are these Psynergy Stones?"
  FieldEvent_ShowMessageThenWait(ACTOR_MAHA, 0, 0);
  Actor_RunRepeatedMotion(ACTOR_JENNA, 2);
  FieldEvent_Wait(10);
  // "You've seen them, Maha! The shining stone at the heart of Air's Rock."
  FieldEvent_ShowMessageThenWait(ACTOR_JENNA, 0, 0);
  Actor_RunRepeatedMotion(ACTOR_MAHA, 2);
  FieldEvent_Wait(10);
  Actor_FaceDirection(ACTOR_MAHA, FACING_SOUTH_SOUTHWEST, 0);
  FieldEvent_Wait(20);
  // "I know the stone you speak of, but..."
  FieldEvent_ShowMessageThenWait(ACTOR_MAHA, 0, 0);
  Actor_AnimateThenWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD, 33);
  // "Air's Rock is quite far from Garoh."
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 0);
  Actor_FaceDirection(ACTOR_MAHA, FACING_SOUTHEAST, 0);
  FieldEvent_Wait(20);
  // "I don't see how it could affect the people here."
  FieldEvent_ShowMessageThenWait(ACTOR_MAHA, 0, 0);
  Actor_AnimateThenWait(ACTOR_KRADEN, ANIM_NOD, 20);
  FieldEvent_Wait(10);
  FieldEvent_Wait(20);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 40);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_WEST, 0);
  FieldEvent_Wait(20);
  // "I wonder... Could it be the wind?" Yes: "You probably figured it out for
  // yourself already, Felix." No: "I guess I couldn't have expected you to
  // understand, Felix..."
  Event_OpenMessage(ACTOR_SHEBA, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_EAST, 0);
  if (Event_GetFlag(FLAG_IN_PARTY(ACTOR_PIERS))) {
    Actor_FaceDirection(ACTOR_PIERS, FACING_WEST, 0);
  }
  if (!FieldEvent_AskYesNoWithActorAnimation(ACTOR_FELIX, 0)) {
    FieldEvent_Wait(25);
    // "You probably figured it out for yourself already, Felix."
    FieldEvent_ShowMessageThenWait(ACTOR_SHEBA, 0, 0);
    gFieldState->message = (gFieldState->message + 1);
  } else {
    FieldEvent_Wait(40);
    gFieldState->message = (gFieldState->message + 1);
    // "I guess I couldn't have expected you to understand, Felix..."
    FieldEvent_ShowMessageThenWait(ACTOR_SHEBA, 0, 0);
  }
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  if (Event_GetFlag(FLAG_IN_PARTY(ACTOR_PIERS))) {
    Actor_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  }
  Actor_ShowEmote(ACTOR_MAHA, EMOTE_IN_FRONT | EMOTE_QUESTION, 50);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTHEAST, 0);
  FieldEvent_Wait(30);
  // "There were strong winds blowing within Air's Rock... Those winds would
  // carry far..."
  FieldEvent_ShowMessageThenWait(ACTOR_SHEBA, 0, 0);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  // "Carry? Carry what?"
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 0);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
  // "Ugh! You're so dense! The power of the Psynergy Stone!"
  FieldEvent_ShowMessageThenWait(ACTOR_SHEBA, 0, 0);
  Actor_AnimateThenWait(ACTOR_MAHA, ANIM_SHAKE_HEAD, 33);
  // "How is that possible? How could the stone's power be carried on the wind?"
  FieldEvent_ShowMessageThenWait(ACTOR_MAHA, 0, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTH, 0);
  FieldEvent_Wait(20);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 30);
  // "Do not belittle the power of the wind!"
  FieldEvent_ShowMessageThenWait(ACTOR_SHEBA, 0, 0);
  Actor_AnimateThenWait(ACTOR_SHEBA, ANIM_SHAKE_HEAD, 33);
  // "Given time, the wind can grind great mountains down to sand!"
  FieldEvent_ShowMessageThenWait(ACTOR_SHEBA, 0, 0);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Actor_ShowEmote(ACTOR_MAHA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 50);
  Actor_RunRepeatedMotion(ACTOR_SHEBA, 2);
  FieldEvent_Wait(10);
  // "I mean, the wind at Air's Rock was strong enough to blow us off the
  // ground!"
  FieldEvent_ShowMessageThenWait(ACTOR_SHEBA, 0, 0);
  Actor_SetAnimation(ACTOR_FELIX, ANIM_NOD);
  Actor_SetAnimation(ACTOR_JENNA, ANIM_NOD);
  Actor_SetAnimation(ACTOR_MAHA, ANIM_NOD);
  if (Event_GetFlag(FLAG_IN_PARTY(ACTOR_PIERS))) {
    Actor_SetAnimation(ACTOR_PIERS, ANIM_NOD);
  }
  Actor_SetAnimation(ACTOR_KRADEN, ANIM_NOD);
  FieldEvent_Wait(20);
  FieldEvent_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_SHEBA, 2);
  FieldEvent_Wait(10);
  // "So why couldn't the wind carry particles of the stone all the way to
  // Garoh?"
  FieldEvent_ShowMessageThenWait(ACTOR_SHEBA, 0, 0);
  FieldActor_StartRepeatedMotion(ACTOR_MAHA, 3);
  Actor_SetAttachedEffect(ACTOR_MAHA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldActor_StartRepeatedMotion(ACTOR_KRADEN, 3);
  Actor_SetAttachedEffect(ACTOR_KRADEN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(40);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_MAHA, FACING_SOUTHEAST, 0);
  FieldEvent_Wait(25);
  FieldActor_Jump(ACTOR_KRADEN, 6, 15);
  FieldActor_Jump(ACTOR_KRADEN, 6, 23);
  FieldEvent_Wait(10);
  Actor_SetAnimation(ACTOR_KRADEN, ANIM_NOD);
  Actor_AnimateThenWait(ACTOR_MAHA, ANIM_NOD, 20);
  FieldEvent_Wait(10);
  // "She's right!", said by both at once.
  FieldEvent_ShowTwoMessagesAndWait(ACTOR_MAHA, 9, 2, 9, 5, ACTOR_KRADEN, 17,
                                    14, 20, 9, 0);
  if (Event_GetFlag(FLAG_IN_PARTY(ACTOR_PIERS))) {
    Actor_FaceDirection(ACTOR_PIERS, FACING_NORTHWEST, 0);
  }
  FieldActor_StartRepeatedMotion(ACTOR_MAHA, 3);
  Actor_SetAttachedEffect(ACTOR_MAHA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(40);
  // "Those particles from the Psynergy Stone must be charged with its power!"
  FieldEvent_ShowMessageThenWait(ACTOR_MAHA, 0, 0);
  FieldActor_StartRepeatedMotion(ACTOR_KRADEN, 3);
  Actor_SetAttachedEffect(ACTOR_KRADEN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(40);
  // "This explains why your Psynergy is weaker than that of Vale's people...
  // You've been exposed to smaller particles."
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 0);
  Actor_ShowEmote(ACTOR_MAHA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  // "The wind patterns might also affect who among us in Garoh can use
  // Whirlwind."
  FieldEvent_ShowMessageThenWait(ACTOR_MAHA, 0, 0);
  Actor_RunRepeatedMotion(ACTOR_MAHA, 2);
  FieldEvent_Wait(10);
  // "Of course, this is all mere conjecture, but it does offer us some
  // clues..."
  FieldEvent_ShowMessageThenWait(ACTOR_MAHA, 0, 0);
  Actor_AnimateThenWait(ACTOR_MAHA, ANIM_NOD, 20);
  FieldEvent_Wait(10);
  // "Thank you, umm..."
  FieldEvent_ShowMessageThenWait(ACTOR_MAHA, 0, 0);
  Actor_AnimateThenWait(ACTOR_SHEBA, ANIM_NOD, 20);
  FieldEvent_Wait(10);
  // "It's Sheba. I'm pleased to meet you."
  FieldEvent_ShowMessageThenWait(ACTOR_SHEBA, 0, 0);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
  FieldEvent_Wait(10);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTHWEST, 0);
  FieldEvent_Wait(20);
  // "Oh! Haven't we given you our names, Master Maha?"
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTHWEST, 0);
  FieldEvent_Wait(20);
  // "I am Kraden."
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 0);
  Actor_AnimateThenWait(ACTOR_MAHA, ANIM_NOD, 20);
  FieldEvent_Wait(10);
  Actor_WalkByAndWait(ACTOR_KRADEN, -18, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 0);
  FieldEvent_Wait(20);
  // "This is Jenna..."
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH, 0);
  FieldEvent_Wait(20);
  Actor_FaceDirection(ACTOR_MAHA, FACING_SOUTH_SOUTHWEST, 0);
  FieldEvent_Wait(20);
  Actor_AnimateThenWait(ACTOR_MAHA, ANIM_NOD, 20);
  FieldEvent_Wait(20);
  Actor_AnimateThenWait(ACTOR_JENNA, ANIM_NOD, 20);
  FieldEvent_Wait(10);
  if (Event_GetFlag(FLAG_IN_PARTY(ACTOR_PIERS))) {
    Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTHEAST, 0);
    // "Piers..."
    FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0, 0);
    Actor_FaceDirection(ACTOR_MAHA, FACING_SOUTH_SOUTHWEST, 0);
    FieldEvent_Wait(20);
    Actor_AnimateThenWait(ACTOR_MAHA, ANIM_NOD, 20);
    FieldEvent_Wait(20);
    Actor_SetAnimation(ACTOR_PIERS, ANIM_NOD);
    FieldEvent_Wait(20);
    FieldEvent_Wait(10);
  } else {
    gFieldState->message = (gFieldState->message + 1);
  }
  Actor_WalkByAndWait(ACTOR_KRADEN, -42, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST, 0);
  FieldEvent_Wait(20);
  Actor_AnimateThenWait(ACTOR_KRADEN, ANIM_NOD, 20);
  FieldEvent_Wait(10);
  // "And Felix."
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTHEAST, 0);
  FieldEvent_Wait(20);
  Actor_WalkByAndWait(ACTOR_MAHA, -16, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_MAHA, FACING_SOUTH_SOUTHWEST, 0);
  FieldEvent_Wait(20);
  Actor_AnimateThenWait(ACTOR_MAHA, ANIM_NOD, 20);
  FieldEvent_Wait(30);
  Actor_AnimateThenWait(ACTOR_FELIX, ANIM_NOD, 20);
  FieldEvent_Wait(25);
  // "You must permit me to thank you for all you've done."
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_MAHA, 0, 0);
  Actor_AnimateThenWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD, 33);
  // "No, no, please... We don't need any thanks..."
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 0);
  Actor_FaceDirection(ACTOR_MAHA, FACING_NORTH, 0);
  FieldEvent_Wait(40);
  Garoh_PopQuestionMarks(ACTOR_MAHA);
  Garoh_PopQuestionMarks(ACTOR_MAHA);
  FieldEvent_Wait(20);
  Actor_WalkByAndWait(ACTOR_MAHA, 16, 0);
  FieldEvent_Wait(20);
  FieldActor_StartRepeatedMotion(ACTOR_MAHA, 3);
  Actor_SetAttachedEffect(ACTOR_MAHA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(40);
  Actor_FaceDirection(ACTOR_MAHA, FACING_WEST, 0);
  FieldEvent_Wait(20);
  Actor_WalkByAndWait(ACTOR_MAHA, -16, 0);
  FieldEvent_Wait(10);
  Actor_WalkByAndWait(ACTOR_MAHA, -16, 0);
  FieldEvent_Wait(20);
  Actor_AnimateThenWait(ACTOR_MAHA, ANIM_SHAKE_HEAD, 33);
  Actor_FaceDirection(ACTOR_MAHA, FACING_EAST, 0);
  FieldEvent_Wait(20);
  Actor_WalkByAndWait(ACTOR_MAHA, 16, 0);
  FieldEvent_Wait(20);
  Actor_FaceDirection(ACTOR_MAHA, FACING_NORTH, 0);
  FieldEvent_Wait(20);
  FieldActor_StartRepeatedMotion(ACTOR_MAHA, 3);
  Actor_SetAttachedEffect(ACTOR_MAHA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(40);
  Actor_WalkByAndWait(ACTOR_MAHA, 0, -12);
  FieldEvent_Wait(20);
  Actor_WalkByAndWait(ACTOR_MAHA, 0, -12);
  FieldEvent_Wait(20);
  FieldEvent_Wait(20);
  Actor_ShowEmote(ACTOR_MAHA, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 40);
  Actor_FaceDirection(ACTOR_MAHA, FACING_SOUTH, 0);
  FieldEvent_Wait(20);
  // "That's too bad... I was going to give you this."
  FieldEvent_ShowMessageThenWait(ACTOR_MAHA, 0, 0);
  Actor_RunRepeatedMotion(ACTOR_MAHA, 2);
  FieldEvent_Wait(10);
  Actor_AnimateThenWait(ACTOR_MAHA, ANIM_NOD, 20);
  FieldEvent_Wait(5);
  ether = Actor_Get(ACTOR_ETHER);
  sprite = ether->sprite;
  sprite->scale = FIXED_ONE * 2 / 5;
  FieldActor_SetSpritePriority(ACTOR_ETHER, 1);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_ETHER), 0);
  Actor_SetPosition(ACTOR_ETHER, 328, 160);
  FieldActor_Jump(ACTOR_ETHER, 9, 0);
  total = 0;
  for (frames = 0; frames <= ETHER_GROW_STEPS; frames++) {
    sprite->scale = Math_Divide(total, ETHER_GROW_STEPS) + FIXED_ONE * 2 / 5;
    ether->facing += FACING_STEP;
    System_Wait(1);
    total += FIXED_ONE * 3 / 5;
  }
  ether->facing = FACING_SOUTH;
  Actor_WalkByAndWait(ACTOR_ETHER, 0, 8);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_ETHER), 1);
  FieldEvent_Wait(5);
  Actor_WalkByAndWait(ACTOR_ETHER, 0, 8);
  FieldEvent_Wait(5);
  Actor_WalkByAndWait(ACTOR_ETHER, 0, 8);
  FieldEvent_Wait(5);
  Actor_WalkByAndWait(ACTOR_ETHER, 0, 8);
  FieldEvent_Wait(5);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  // "It's a Djinni!"
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_JENNA, 0, 0);
  Actor_ShowEmote(ACTOR_MAHA, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  Actor_FaceDirection(ACTOR_MAHA, FACING_SOUTH_SOUTHEAST, 0);
  FieldEvent_Wait(20);
  // "A 'Djinni'? Is that what you call them?"
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_MAHA, 0, 0);
  Actor_WalkByAndWait(ACTOR_MAHA, 0, 24);
  Actor_FaceDirection(ACTOR_MAHA, FACING_SOUTH_SOUTHEAST, 0);
  FieldEvent_Wait(10);
  // "When he is with you, he adds to your power. He is very helpful."
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_MAHA, 0, 0);
  Actor_FaceDirection(ACTOR_MAHA, FACING_SOUTH_SOUTHWEST, 0);
  FieldEvent_Wait(20);
  // "I suppose you'll be continuing your journey now?" Yes: "So you intend to
  // travel by sea? That is a long journey..." No: "So, you still can't tell
  // just what sort of journeys await you..."
  Event_OpenMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MAHA, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_WEST, 0);
  if (Event_GetFlag(FLAG_IN_PARTY(ACTOR_PIERS))) {
    Actor_FaceDirection(ACTOR_PIERS, FACING_WEST, 0);
  }
  FieldEvent_Wait(20);
  if (!FieldEvent_AskYesNoWithActorAnimation(ACTOR_FELIX, 0)) {
    FieldEvent_Wait(30);
    // "So you intend to travel by sea? That is a long journey..."
    FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_MAHA, 0, 0);
    gFieldState->message = (gFieldState->message + 1);
  } else {
    FieldEvent_Wait(40);
    gFieldState->message = (gFieldState->message + 1);
    // "So, you still can't tell just what sort of journeys await you..."
    FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_MAHA, 0, 0);
  }
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTHWEST, 0);
  if (Event_GetFlag(FLAG_IN_PARTY(ACTOR_PIERS))) {
    Actor_FaceDirection(ACTOR_PIERS, FACING_NORTHWEST, 0);
  }
  FieldEvent_Wait(30);
  Actor_RunRepeatedMotion(ACTOR_MAHA, 2);
  FieldEvent_Wait(10);
  // "In that case, I think you will get more use out of this 'Djinni' than I
  // will."
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_MAHA, 0, 0);
  Actor_SetSpeed(ACTOR_MAHA, 2.4);
  Actor_Get(ACTOR_MAHA)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_FaceDirection(ACTOR_MAHA, FACING_SOUTH_SOUTHWEST, 0);
  Actor_WalkByAndWait(ACTOR_MAHA, 0, 8);
  Actor_WalkByAndWait(ACTOR_MAHA, 0, -8);
  FieldEvent_Wait(1);
  Actor_Get(ACTOR_MAHA)->steeringFlags |= ACTOR_FACES_MOTION;
  FieldEvent_Wait(1);
  Actor_SetSpeed(ACTOR_MAHA, 0.7);
  Actor_FaceDirection(ACTOR_MAHA, FACING_SOUTH_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_WEST, 0);
  if (Event_GetFlag(FLAG_IN_PARTY(ACTOR_PIERS))) {
    Actor_FaceDirection(ACTOR_PIERS, FACING_WEST, 0);
  }
  Actor_FaceDirection(ACTOR_SHEBA, FACING_WEST, 0);
  Djinn_JoinParty(ACTOR_ETHER, DJINNI_ETHER / DJINN_PER_ELEMENT,
                  DJINNI_INDEX(DJINNI_ETHER));
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  Actor_SetAnimation(ACTOR_FELIX, ANIM_STAND);
  FieldEvent_Wait(20);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_MAHA, FACING_SOUTH, 0);
  FieldEvent_Wait(20);
  gFieldState->message = (gFieldState->message + 1);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  // "Another Djinni! Talk about lucky!"
  FieldEvent_ShowMessageThenWait(ACTOR_SHEBA, 0, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_EAST, 0);
  FieldEvent_Wait(30);
  Actor_AnimateThenWait(ACTOR_JENNA, ANIM_NOD, 20);
  FieldEvent_Wait(20);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTHEAST, 0);
  FieldEvent_Wait(20);
  // "This is a great gift. We are quite thankful."
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
  FieldEvent_Wait(20);
  Actor_AnimateThenWait(ACTOR_MAHA, ANIM_SHAKE_HEAD, 33);
  // "You don't need to thank me. Just take care on your travels."
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_MAHA, 0, 0);
  Actor_AnimateThenWait(ACTOR_KRADEN, ANIM_NOD, 20);
  FieldEvent_Wait(20);
  Actor_AnimateThenWait(ACTOR_MAHA, ANIM_NOD, 20);
  FieldEvent_Wait(10);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  // "Good luck with the... werewolf thing, Maha..."
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_JENNA, 0, 0);
  Actor_FaceDirection(ACTOR_MAHA, FACING_SOUTH_SOUTHEAST, 0);
  FieldEvent_Wait(20);
  Actor_AnimateThenWait(ACTOR_MAHA, ANIM_NOD, 20);
  FieldEvent_Wait(20);
  Actor_SetAnimation(ACTOR_FELIX, ANIM_NOD);
  Actor_SetAnimation(ACTOR_JENNA, ANIM_NOD);
  Actor_SetAnimation(ACTOR_SHEBA, ANIM_NOD);
  if (Event_GetFlag(FLAG_IN_PARTY(ACTOR_PIERS))) {
    Actor_SetAnimation(ACTOR_PIERS, ANIM_NOD);
  }
  Actor_SetAnimation(ACTOR_KRADEN, ANIM_NOD);
  FieldEvent_Wait(20);
  FieldEvent_Wait(20);
  Actor_WalkByAndWait(ACTOR_KRADEN, 12, 0);
  Actor_FaceDirection(ACTOR_MAHA, FACING_SOUTH_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH, 0);
  FieldEvent_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
  FieldEvent_Wait(10);
  // "Well, Felix, we must be going now."
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 0);
  Actor_AnimateThenWait(ACTOR_FELIX, ANIM_NOD, 20);
  FieldEvent_Wait(20);
  if (Event_GetFlag(FLAG_IN_PARTY(ACTOR_PIERS))) {
    Actor_SetAnimation(ACTOR_PIERS, ANIM_NOD);
  }
  Actor_SetAnimation(ACTOR_JENNA, ANIM_NOD);
  Actor_SetAnimation(ACTOR_SHEBA, ANIM_NOD);
  Actor_AnimateThenWait(ACTOR_FELIX, ANIM_NOD, 20);
  FieldEvent_Wait(20);
  Actor_SetSpeed(ACTOR_KRADEN, 1.2);
  Scene_WalkIntoLeader(ACTOR_KRADEN);
  Actor_SetSpeed(ACTOR_JENNA, 1.2);
  Scene_WalkIntoLeader(ACTOR_JENNA);
  Actor_SetSpeed(ACTOR_SHEBA, 1.2);
  Scene_WalkIntoLeader(ACTOR_SHEBA);
  if (Event_GetFlag(FLAG_IN_PARTY(ACTOR_PIERS))) {
    Actor_SetSpeed(ACTOR_PIERS, 1.2);
    Scene_WalkIntoLeader(ACTOR_PIERS);
  }
  EventCamera_FollowActor(gMainRoutineState.leaderActorId, 1);
  Event_End();
}

void Garoh_LittleWerewolfThanks(void);
void Garoh_LiftPillar(void);
void Garoh_LiftPushedPillar(void);
void Garoh_RevealCaveEntrance(void);
void Garoh_RevealCavePassage(void);

extern u32 gFieldEffect_MediumScript[];

// clang-format off
#include "shared/field_effect_duration_scripts.c"

/// The four pillars Move pushes, ended by -1.
s16 gGaroh_Pillars[] = {
    ACTOR_FIRST_PILLAR, ACTOR_SECOND_PILLAR, ACTOR_THIRD_PILLAR,
    ACTOR_FOURTH_PILLAR, -1, 0,
};

u32 gGaroh_WerewolfLookAroundScript[] = {
    SC_SET(SW_FACING, FACING_SOUTH_SOUTHWEST),
    SC_WAIT(12),
    SC_SET(SW_FACING, FACING_SOUTH_SOUTHEAST),
    SC_WAIT(20),
    SC_SET(SW_FACING, FACING_SOUTH_SOUTHEAST),
    SC_WAIT(7),
    SC_SET(SW_FACING, FACING_SOUTH_SOUTHWEST),
    SC_WAIT(34),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

/// The path's colours by night, BG palette 6's colours 1 to 9.
u16 gGaroh_PathNightColors[] = {
    31, 31, 31, 29, 29, 30, 27, 28,
    29, 25, 27, 28, 23, 25, 27, 21,
    24, 26, 15, 18, 19, 10, 12, 13,
    5, 6, 6,
};

/// The path's colours by day, BG palette 1's colours 8 to 15.
u16 gGaroh_PathDayColors[] = {
    24, 28, 30, 19, 24, 27, 15, 20,
    25, 11, 16, 23, 7, 12, 21, 3,
    8, 19, 27, 27, 27, 16, 24, 29,
    0,
};

const s32 *gFieldEffect_DurationScripts[] = {
    gFieldEffect_ShortScript,
    gFieldEffect_MediumScript,
    gFieldEffect_LongScript,
};

s32 gGaroh_ShrunkSpriteScript[] = {
    SC_SET(SW_MOTION, 0),
    SC_ADD(SW_Y, toFIXED(-4)),
    SC_ADD(SW_X, toFIXED(6)),
    SC_SET(SW_SCALE_X, toFIXED(0.8)),
    SC_SET(SW_SCALE_Y, toFIXED(0.8)),
    SC_CALL(Garoh_ClearScriptedActorSpriteFlags),
    SC_END,
};

#include "shared/scene_entrances_default.c"

u32 gSceneExits[] = {
    SCENE_EXITS(Garoh),
    SCENE_EXIT_UNLESS(GAROH_TO_GAROH_ROOMS_HOUSES_SOUTH, GarohRoomsHouses, GAROH_ROOMS_HOUSES_FROM_GAROH_SOUTH, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(GAROH_TO_GAROH_ROOMS_SHOP, GarohRoomsShop, GAROH_ROOMS_SHOP_FROM_GAROH, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(GAROH_TO_GAROH_ROOMS_HOUSES_NORTHWEST, GarohRoomsHouses, GAROH_ROOMS_HOUSES_FROM_GAROH_NORTHWEST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(GAROH_TO_GAROH_ROOMS_HOUSES_SOUTHWEST, GarohRoomsHouses, GAROH_ROOMS_HOUSES_FROM_GAROH_SOUTHWEST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(GAROH_TO_GAROH_ROOMS_HOUSES_SOUTHEAST, GarohRoomsHouses, GAROH_ROOMS_HOUSES_FROM_GAROH_SOUTHEAST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(GAROH_TO_GAROH_ROOMS_SANCTUM, GarohRoomsSanctum, GAROH_ROOMS_SANCTUM_FROM_GAROH, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(GAROH_TO_GAROH_TUNNELS_NORTHEAST, GarohTunnels, GAROH_TUNNELS_FROM_GAROH_NORTHEAST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(GAROH_TO_GAROH_TUNNELS_WEST, GarohTunnels, GAROH_TUNNELS_FROM_GAROH_WEST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(GAROH_TO_GAROH_TUNNELS_SOUTH, GarohTunnels, GAROH_TUNNELS_FROM_GAROH_SOUTH, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(GAROH_TO_GAROH_PATH_EAST, GarohPath, GAROH_PATH_FROM_GAROH_EAST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(GAROH_TO_GAROH_PATH_WEST, GarohPath, GAROH_PATH_FROM_GAROH_WEST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(GAROH_TO_GAROH_WEREWOLF_CAVE, GarohWerewolfCave, GAROH_WEREWOLF_CAVE_FROM_GAROH, EXIT_FLAG_NONE),
    SCENE_EXITS(GarohPath),
    SCENE_EXIT_UNLESS(GAROH_PATH_TO_GAROH_WAY_UP, GarohWayUp, GAROH_WAY_UP_FROM_GAROH_PATH, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(GAROH_PATH_TO_GAROH_EAST, Garoh, GAROH_FROM_GAROH_PATH_EAST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(GAROH_PATH_TO_GAROH_WEST, Garoh, GAROH_FROM_GAROH_PATH_WEST, EXIT_FLAG_NONE),
    SCENE_EXITS(GarohWayUp),
    SCENE_EXIT_UNLESS(GAROH_WAY_UP_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_GAROH_WAY_UP, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(GAROH_WAY_UP_TO_GAROH_PATH, GarohPath, GAROH_PATH_FROM_GAROH_WAY_UP, EXIT_FLAG_NONE),
    SCENE_EXITS(GarohTunnels),
    SCENE_EXIT_UNLESS(GAROH_TUNNELS_TO_GAROH_WEST, Garoh, GAROH_FROM_GAROH_TUNNELS_WEST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, GarohTunnels, 3, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, GarohTunnels, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(GAROH_TUNNELS_TO_GAROH_NORTHEAST, Garoh, GAROH_FROM_GAROH_TUNNELS_NORTHEAST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(GAROH_TUNNELS_TO_GAROH_ROOMS_SHOP_WEST, GarohRoomsShop, GAROH_ROOMS_SHOP_FROM_GAROH_TUNNELS_WEST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(GAROH_TUNNELS_TO_GAROH_ROOMS_SHOP_EAST, GarohRoomsShop, GAROH_ROOMS_SHOP_FROM_GAROH_TUNNELS_EAST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(GAROH_TUNNELS_TO_GAROH_ROOMS_HOUSES_EAST, GarohRoomsHouses, GAROH_ROOMS_HOUSES_FROM_GAROH_TUNNELS_EAST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(GAROH_TUNNELS_TO_GAROH_ROOMS_HOUSES_WEST, GarohRoomsHouses, GAROH_ROOMS_HOUSES_FROM_GAROH_TUNNELS_WEST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(9, GarohTunnels, 10, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(10, GarohTunnels, 9, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(GAROH_TUNNELS_TO_GAROH_SOUTH, Garoh, GAROH_FROM_GAROH_TUNNELS_SOUTH, EXIT_FLAG_NONE),
    SCENE_EXITS(GarohWerewolfCave),
    SCENE_EXIT_UNLESS(GAROH_WEREWOLF_CAVE_TO_GAROH, Garoh, GAROH_FROM_GAROH_WEREWOLF_CAVE, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, GarohWerewolfCave, 4, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, GarohWerewolfCave, 5, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(4, GarohWerewolfCave, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(5, GarohWerewolfCave, 3, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(GAROH_WEREWOLF_CAVE_TO_ROOM_BEHIND_ROCK, GarohWerewolfCave, GAROH_WEREWOLF_CAVE_ROOM_BEHIND_ROCK_FROM_CAVE, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(GAROH_WEREWOLF_CAVE_ROOM_BEHIND_ROCK_TO_CAVE, GarohWerewolfCave, GAROH_WEREWOLF_CAVE_FROM_ROOM_BEHIND_ROCK, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(GAROH_WEREWOLF_CAVE_TO_GAROH_ROOMS_HOUSES, GarohRoomsHouses, GAROH_ROOMS_HOUSES_FROM_GAROH_WEREWOLF_CAVE, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

struct ActorPlacement gGaroh_ActorPlacements[] = {
    {SCENE_TABLE_END},
};

struct ActorPlacement gGaroh_VillageActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_HOODED_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 232, 0, 400, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_HOODED_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 416, 0, 424, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_HOODED_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 192, 0, 304, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_HOODED_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 280, 0, 344, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_HOODED_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 368, 0, 312, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BRAZIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 232, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BRAZIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 408, 0, 232, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_BRAZIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 184, 0, 232, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BRAZIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 88, 0, 312, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BRAZIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 152, 0, 280, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BRAZIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 264, 0, 248, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BRAZIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 408, 0, 296, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BRAZIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 408, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_MAHA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gGaroh_VillageDayActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 232, 0, 400, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 416, 0, 424, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BURLY_MAN_WITH_GREY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 192, 0, 304, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 280, 0, 344, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 368, 0, 312, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 384, 0, 232, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BRAZIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 232, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BRAZIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 408, 0, 232, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_BRAZIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 184, 0, 232, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BRAZIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 88, 0, 312, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BRAZIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 152, 0, 280, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BRAZIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 264, 0, 248, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BRAZIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 408, 0, 296, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BRAZIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 408, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    {SCENE_TABLE_END},
};

struct ActorPlacement gGaroh_WayUpActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 184, 0, 472, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 232, 0, 456, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 168, 0, 392, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 248, 0, 392, FACING_NORTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gGaroh_TunnelsActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, (s32)gGaroh_ShrunkSpriteScript, 128, 0, 168, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, (s32)gGaroh_ShrunkSpriteScript, 208, 0, 56, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, (s32)gGaroh_ShrunkSpriteScript, 152, 0, 328, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, (s32)gGaroh_ShrunkSpriteScript, 248, 0, 328, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, (s32)gGaroh_ShrunkSpriteScript, 352, 0, 328, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, (s32)gGaroh_ShrunkSpriteScript, 216, 0, 424, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, (s32)gGaroh_ShrunkSpriteScript, 96, 0, 488, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, (s32)gGaroh_ShrunkSpriteScript, 328, 0, 440, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, (s32)gGaroh_ShrunkSpriteScript, 456, 0, 352, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, (s32)gGaroh_ShrunkSpriteScript, 136, 0, 664, FACING_NORTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gGaroh_PathActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_WEREWOLF, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SHEBA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PIERS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 72, 0, 120, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120, 0, 120, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    {SCENE_TABLE_END},
};

struct ActorPlacement gGaroh_WerewolfCaveActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_WEREWOLF, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MAHA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_JUPITER_DJINNI, FLAG_DJINNI(DJINNI_ETHER), MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SHEBA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PIERS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, (s32)gGaroh_ShrunkSpriteScript, 64, 0, 562, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, (s32)gGaroh_ShrunkSpriteScript, 370, 0, 56, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, (s32)gGaroh_ShrunkSpriteScript, 256, 0, 80, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, (s32)gGaroh_ShrunkSpriteScript, 184, 0, 352, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, (s32)gGaroh_ShrunkSpriteScript, 392, 0, 368, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, (s32)gGaroh_ShrunkSpriteScript, 280, 0, 626, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, (s32)gGaroh_ShrunkSpriteScript, 336, 0, 784, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    {SCENE_TABLE_END},
};

/// The village's doors opening, as MAPGRID_ANIMATION_STEP rows: two steps of six
/// frames each: a door, the shop's wide door, the sanctum's tall door.
s16 gGaroh_DoorFrames[] = {
    82, 32, 1, 2, 6, 80, 32, 1,
    2, 6, -1,
};
s16 gGaroh_WideDoorFrames[] = {
    82, 35, 1, 2, 6, 80, 35, 1,
    2, 6, -1,
};
s16 gGaroh_TallDoorFrames[] = {
    78, 32, 2, 2, 6, 78, 35, 2,
    2, 6, -1, 0,
};

/// The doors Felix walks into, a row for each exit from the first: the opening
/// and the cell it plays at.
struct MapGridCellAnimation gGaroh_Doors[] = {
    {gGaroh_DoorFrames, 78, 21},
    {gGaroh_DoorFrames, 0, 0},
    {gGaroh_DoorFrames, 72, 10},
    {gGaroh_WideDoorFrames, 74, 22},
    {gGaroh_DoorFrames, 85, 23},
    {gGaroh_TallDoorFrames, 78, 13},
};

struct Trigger gGaroh_Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_TABLE_END},
};
// clang-format on

struct Trigger gGaroh_VillageTriggers[] = {
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), GAROH_TO_GAROH_ROOMS_HOUSES_SOUTH,
     CONDITION_ALWAYS, EVENT(Garoh_OpenDoorAndEnter)},
    {TRIGGER_EXIT | TRIGGER_FLAG_FACING | FACING_NORTH, 2, CONDITION_ALWAYS, 2},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH),
     GAROH_TO_GAROH_ROOMS_HOUSES_NORTHWEST, CONDITION_ALWAYS,
     EVENT(Garoh_OpenDoorAndEnter)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH),
     GAROH_TO_GAROH_ROOMS_HOUSES_SOUTHWEST, CONDITION_ALWAYS,
     EVENT(Garoh_OpenDoorAndEnter)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH),
     GAROH_TO_GAROH_ROOMS_HOUSES_SOUTHEAST, CONDITION_ALWAYS,
     EVENT(Garoh_OpenDoorAndEnter)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), GAROH_TO_GAROH_ROOMS_SANCTUM,
     CONDITION_ALWAYS, EVENT(Garoh_OpenDoorAndEnter)},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT | TRIGGER_FLAG_FACING | FACING_NORTH, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {STEP_TRIGGER_FACING(FACING_NORTH), GAROH_TO_GAROH_WEREWOLF_CAVE,
     CONDITION_ALWAYS, EVENT(Garoh_EnterOpening)},
    {TRIGGER_STEP, CELL_BEHAVIOR_BY_SANCTUM_DOOR, FLAG_GAROH_MET_WEREWOLVES,
     EVENT(Garoh_WerewolfPeersFromCave)},
    {TRIGGER_TALK, ACTOR_VILLAGER_ASKING_ABOUT_THE_WEREWOLF, CONDITION_ALWAYS,
     EVENT(Garoh_AskIfSawWerewolf)},
    {TRIGGER_TALK, ACTOR_HOODED_VILLAGER, CONDITION_ALWAYS,
     MSGNO(MsgGarohGoodEveningForgiveMeFor)},
    {TRIGGER_TALK, ACTOR_HOWLING_VILLAGER, CONDITION_ALWAYS,
     EVENT(Garoh_HowlForDog)},
    {TRIGGER_TALK, ACTOR_VILLAGER_WARNING_STRANGERS, CONDITION_ALWAYS,
     MSGNO(MsgGarohYouThereStrangersShouldNot)},
    {TRIGGER_TALK, ACTOR_VILLAGER_ON_THE_FULL_MOON, CONDITION_ALWAYS,
     EVENT(Garoh_AskAboutFullMoon)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
     ACTOR_VILLAGER_ASKING_ABOUT_THE_WEREWOLF, CONDITION_ALWAYS,
     MSGNO(MsgGarohItsGrimLuckThatBrings)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HOODED_VILLAGER,
     CONDITION_ALWAYS, MSGNO(MsgGarohSuchGorgeousMoonlightIfOnly)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HOWLING_VILLAGER,
     CONDITION_ALWAYS, MSGNO(MsgGarohWhichOneOfUsWas)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_WARNING_STRANGERS,
     CONDITION_ALWAYS, MSGNO(MsgGarohForeignersOnlyComeHereEn)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_ON_THE_FULL_MOON,
     CONDITION_ALWAYS, MSGNO(MsgGarohTheNightOfTheFull)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0,
     CONDITION_ALWAYS, EVENT(Garoh_RevealCaveEntrance)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0,
     CONDITION_ALWAYS, EVENT(Garoh_HideCaveEntrance)},
    {EXAMINE_TRIGGER(EXAMINE_WELL), CELL_BEHAVIOR_GAROH_WELL, CONDITION_ALWAYS,
     FIND_MESSAGE(MSGNO(MsgGarohMehISawTheSilhouette))},
    {TRIGGER_TABLE_END},
};

struct Trigger gGaroh_VillageDayTriggers[] = {
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), GAROH_TO_GAROH_ROOMS_HOUSES_SOUTH,
     CONDITION_ALWAYS, EVENT(Garoh_OpenDoorAndEnter)},
    {TRIGGER_EXIT | TRIGGER_FLAG_FACING | FACING_NORTH, 2, CONDITION_ALWAYS, 2},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH),
     GAROH_TO_GAROH_ROOMS_HOUSES_NORTHWEST, CONDITION_ALWAYS,
     EVENT(Garoh_OpenDoorAndEnter)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH),
     GAROH_TO_GAROH_ROOMS_HOUSES_SOUTHWEST, CONDITION_ALWAYS,
     EVENT(Garoh_OpenDoorAndEnter)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH),
     GAROH_TO_GAROH_ROOMS_HOUSES_SOUTHEAST, CONDITION_ALWAYS,
     EVENT(Garoh_OpenDoorAndEnter)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), GAROH_TO_GAROH_ROOMS_SANCTUM,
     CONDITION_ALWAYS, EVENT(Garoh_OpenDoorAndEnter)},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT | TRIGGER_FLAG_FACING | FACING_NORTH, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {STEP_TRIGGER_FACING(FACING_NORTH), GAROH_TO_GAROH_WEREWOLF_CAVE,
     CONDITION_ALWAYS, EVENT(Garoh_EnterOpening)},
    {TRIGGER_TALK, ACTOR_VILLAGER_ASKING_ABOUT_THE_WEREWOLF, CONDITION_ALWAYS,
     MSGNO(MsgGarohIHearTellFromMaster)},
    {TRIGGER_TALK, ACTOR_HOODED_VILLAGER, CONDITION_ALWAYS, MSGNO(MsgGaroh16)},
    {TRIGGER_TALK, ACTOR_HOWLING_VILLAGER, CONDITION_ALWAYS,
     EVENT(Garoh_HowlOutLoud)},
    {TRIGGER_TALK, ACTOR_VILLAGER_WARNING_STRANGERS, CONDITION_ALWAYS,
     EVENT(Garoh_AskAboutSecret)},
    {TRIGGER_TALK, ACTOR_VILLAGER_ON_THE_FULL_MOON, CONDITION_ALWAYS,
     MSGNO(MsgGarohIHeardThereWereOther)},
    {TRIGGER_TALK, ACTOR_LITTLE_WEREWOLF, CONDITION_ALWAYS,
     EVENT(Garoh_LittleWerewolfThanks)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
     ACTOR_VILLAGER_ASKING_ABOUT_THE_WEREWOLF, CONDITION_ALWAYS,
     MSGNO(MsgGarohHowCouldTheseOutsidersHave)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HOODED_VILLAGER,
     CONDITION_ALWAYS, MSGNO(MsgGarohIThoughtWedNeverBe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HOWLING_VILLAGER,
     CONDITION_ALWAYS, MSGNO(MsgGarohINeverThoughtIdBe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_WARNING_STRANGERS,
     CONDITION_ALWAYS, MSGNO(MsgGarohMahaSaidTheseTravelersWere)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_ON_THE_FULL_MOON,
     CONDITION_ALWAYS, MSGNO(MsgGarohIWouldLoveToMeet)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
     ACTOR_LITTLE_WEREWOLF | TRIGGER_SUBJECT_OWN_EFFECT,
     FLAG_LITTLE_WEREWOLF_THANKED, EVENT(Garoh_LittleWerewolfThanks)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LITTLE_WEREWOLF,
     CONDITION_ALWAYS, MSGNO(MsgGarohMyDadToldMeI)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0,
     CONDITION_ALWAYS, EVENT(Garoh_RevealCaveEntrance)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0,
     CONDITION_ALWAYS, EVENT(Garoh_HideCaveEntrance)},
    {EXAMINE_TRIGGER(EXAMINE_WELL), CELL_BEHAVIOR_GAROH_WELL, CONDITION_ALWAYS,
     FIND_MESSAGE(MSGNO(MsgGarohMehISawTheSilhouette))},
    {TRIGGER_TABLE_END},
};

struct Trigger gGaroh_PathTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_STEP, CELL_BEHAVIOR_WEREWOLF_RUNS_PAST, FLAG_GAROH_SAW_WEREWOLF,
     EVENT(Garoh_WerewolfRunsPast)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0,
     CONDITION_ALWAYS, EVENT(Garoh_IsNight)},
    {TRIGGER_TABLE_END},
};

// No end row: the room reads on into the tunnels' table, whose exits and
// Reveal answer here too.
struct Trigger gGaroh_WayUpTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_STEP, CELL_BEHAVIOR_LOWER_TWILIGHT_LINE,
     FLAG_CROSSED_LOWER_TWILIGHT_LINE, EVENT(Garoh_CrossLowerTwilightLine)},
    {TRIGGER_STEP, CELL_BEHAVIOR_UPPER_TWILIGHT_LINE,
     FLAG_CROSSED_UPPER_TWILIGHT_LINE, EVENT(Garoh_CrossUpperTwilightLine)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST),
     ACTOR_FIRST_PILLAR, FLAG_FIRST_PILLAR_SUNK, EVENT(Garoh_LiftPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_PILLAR,
     FLAG_FIRST_PILLAR_SUNK, EVENT(Garoh_PlaceFirstPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST),
     ACTOR_SECOND_PILLAR, FLAG_SECOND_PILLAR_SUNK, EVENT(Garoh_LiftPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SECOND_PILLAR,
     FLAG_SECOND_PILLAR_SUNK, EVENT(Garoh_PlaceSecondPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST),
     ACTOR_THIRD_PILLAR, FLAG_GAROH_ROPE_LASHED, EVENT(Garoh_LiftPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_THIRD_PILLAR,
     FLAG_GAROH_ROPE_LASHED, EVENT(Garoh_PlaceThirdPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST),
     ACTOR_FOURTH_PILLAR, FLAG_FOURTH_PILLAR_SUNK, EVENT(Garoh_LiftPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FOURTH_PILLAR,
     FLAG_FOURTH_PILLAR_SUNK, EVENT(Garoh_PlaceFourthPillar)},
    {TRIGGER_PUSH, 0, FLAG_FIRST_PILLAR_SUNK, EVENT(Garoh_LiftPushedPillar)},
    {TRIGGER_PUSHED, 0, FLAG_FIRST_PILLAR_SUNK, EVENT(Garoh_PlacePushedPillar)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0,
     CONDITION_ALWAYS, EVENT(Garoh_RestoreWayUpLight)},
};

struct Trigger gGaroh_TunnelsTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 11, CONDITION_ALWAYS, 11},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0,
     CONDITION_ALWAYS, EVENT(Garoh_RevealTunnelPassage)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0,
     CONDITION_ALWAYS, EVENT(Garoh_HideTunnelPassage)},
    {TRIGGER_TABLE_END},
};

struct Trigger gGaroh_WerewolfCaveTriggers[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {STEP_TRIGGER_FACING(FACING_NORTH), 6, CONDITION_ALWAYS,
     EVENT(Garoh_EnterOpening)},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_STEP, CELL_BEHAVIOR_CAVE_MOUTH, FLAG_GAROH_FOLLOWED_WEREWOLF,
     EVENT(Garoh_FollowWerewolfIntoCave)},
    {TRIGGER_STEP, CELL_BEHAVIOR_BELOW_MAHA, CONDITION_ALWAYS,
     EVENT(Garoh_MeetInTheCave)},
    {TRIGGER_TALK, ACTOR_MAHA, CONDITION_ALWAYS,
     MSGNO(MsgGarohTakeCareInYourTravels)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAHA, CONDITION_ALWAYS,
     MSGNO(MsgGarohIWishThatIWere)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0,
     CONDITION_ALWAYS, EVENT(Garoh_RevealCavePassage)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0,
     CONDITION_ALWAYS, EVENT(Garoh_HideCavePassage)},
    {TRIGGER_TABLE_END},
};

/// The frames counted at night; a wolf howls every 1,024.
s32 gGaroh_HowlFrames = 0;

u32 gGaroh_QuestionMarkScript[] = {
    SC_RELEASE,
};
#undef Actor_AnimateThenWait

/// @file
/// Garoh, the village under Air's Rock whose people, its protectors, say there
/// are no werewolves while one prowls its paths at night, in five rooms and a
/// house that share this overlay (SceneId_Garoh to SceneId_GarohWerewolfCave,
/// SceneId_GarohRoomsSanctum).
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
#include "field/flag_call.h"

#ifdef MODERN
void Rope_Lash(s32 a, s32 b, s32 c);
#endif
void Actor_PlaceBlocks(const void *data);
void Actor_LiftTargetBlock(const void *data);
void Actor_PlaceMovedBlock(void);

void Garoh_RaisePillar(s32 id, struct Actor *actor);
void Garoh_RaiseFirstPillarOnCell(void);
void Garoh_RaiseSecondPillarOnCell(void);
void Garoh_CheckThirdPillar(void);
void Garoh_RaiseFourthPillarOnCell(void);
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
#include "system/ram_buffer.h"
#include "system/dma.h"
#include "system/io_reg.h"
#include "graphics/palette_buffers.h"
#include "system/frame.h"
#include "battle/battle.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "field/scene_tables.h"
#include "system/main_routine_work.h"
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
extern struct Trigger gGaroh_BlocksTriggers[];
void Garoh_WerewolvesRevealThemselves(void);
void Garoh_MahaTellsTheLegend(void);
void Garoh_InitializeVillage(void);
void Garoh_InitializeBlocks(void);
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
void Garoh_RestoreBlocksDark(void);
void Garoh_HideTunnelPassage(void);
void Garoh_HideCavePassage(void);
void Garoh_CrossWestEdge(void);
void Garoh_CrossEastEdge(void);
extern u32 gFieldEffect_ShortScript[];
extern u32 gFieldEffect_LongScript[];
extern s32 gGaroh_ShrunkSpriteScript[];
extern s16 gGaroh_DoorFrames[];
extern s16 gGaroh_WideDoorFrames[];
extern s16 gGaroh_TallDoorFrames[];
void Garoh_LashRope(void);
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

/// The werewolf's path and the cave beneath the village
/// (SceneId_GarohHypnosSwordChest and SceneId_GarohWerewolfCave), numbering the
/// rows of gGaroh_WerewolfCaveActorPlacements; GarohHypnosSwordChest places the
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

/// The werewolf's path's tightrope posts: Jenna, Sheba and Piers take their
/// own numbers, so the posts follow the werewolf and Kraden.
enum {
  ACTOR_PATH_TIGHTROPE_POST_A = ACTOR_FIRST_PLACED + 2,
  ACTOR_PATH_TIGHTROPE_POST_B
};

/// The stone pillars on the way up to the village (SceneId_GarohBlocks),
/// numbering the rows of gGaroh_BlocksActorPlacements: Move or a push takes
/// each onto the cell where it rises.
enum GarohBlocksActorId {
  ACTOR_FIRST_PILLAR = ACTOR_FIRST_PLACED,
  ACTOR_SECOND_PILLAR,
  ACTOR_THIRD_PILLAR,
  ACTOR_FOURTH_PILLAR
};

/// Story flags of the werewolves' secret, in the order the party meets them.
#define FLAG_GAROH_REACHED_VILLAGE 0x919
#define FLAG_GAROH_SAW_WEREWOLF 0x91c
#define FLAG_GAROH_FOLLOWED_WEREWOLF 0x91d
#define FLAG_GAROH_MET_WEREWOLVES 0x91e
#define FLAG_GAROH_HEARD_MAHA 0x91f
/// For this visit to the area: the old werewolf has peered from the cave,
/// the little werewolf has thanked the party, and the way up is in the
/// evening's light, between its two edges.
#define FLAG_WEREWOLF_PEERED_OUT FLAG_AREA_TEMP_20
#define FLAG_LITTLE_WEREWOLF_THANKED FLAG_AREA_TEMP_01
#define FLAG_EVENING FLAG_AREA_TEMP_18
/// The way up: the pillars raised on their cells, and the rope Lashed.
#define FLAG_FIRST_PILLAR_RAISED FLAG_SCENE_TEMP_01
#define FLAG_SECOND_PILLAR_RAISED FLAG_SCENE_TEMP_02
#define FLAG_GAROH_ROPE_LASHED FLAG_SCENE_TEMP_03
#define FLAG_FOURTH_PILLAR_RAISED FLAG_SCENE_TEMP_04
/// The edges of the way up's evening, where it begins and where the night or
/// the day does, crossed since the party came in.
#define FLAG_CROSSED_WEST_EDGE FLAG_SCENE_TEMP_10
#define FLAG_CROSSED_EAST_EDGE FLAG_SCENE_TEMP_11

/// The houses' lit windows, colour 14 of BG palette 1, kept white when the
/// village darkens for the night.
#define WINDOW_LIGHT 14

/// The werewolf's path's way in that no exit uses: by night it tints the path
/// purple rather than warm and dark.
#define ENTRANCE_PATH_PURPLE_NIGHT 12

/// The steps in which the Ether grows from two fifths of its size to full.
#define ETHER_GROW_STEPS 20

/// By day the werewolf's path's second layer takes the first's scale and
/// wrap.
void Garoh_MatchSecondLayer(void) {
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  struct MapLayer *to = &map->layers[1];
  struct MapLayer *from = &map->layers[0];

  to->scaleX = from->scaleX;
  to->scaleY = from->scaleY;
  to->maskX = from->maskX;
  to->maskY = from->maskY;
}

/// By night the werewolf's path's second layer ripples: each line is offset by
/// two sines of the frame count, handed to an HBlank DMA.
void Garoh_WaveHypnosSwordChestLayer(void) {
  s32 x;
  // The sine routine itself, called at the address Trig_Sin's far call
  // jumps to.
  s32 (*sine)(s32) = *(s32 (**)(s32))(((u32)Trig_Sin & ~3) + 4);
  struct MapLayer *layer =
      &((struct MapWork *)Ram_HeapSlots->mapGrid)->layers[1];
  u16 *line = gGaroh_WaveLines;

  if (gFieldWork->revealActive == 0) {
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

extern u16 gGaroh_HypnosSwordChestDayColors[];
extern u16 gGaroh_HypnosSwordChestNightColors[];

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
DEFINE_GAROH_LOAD_PALETTE(Garoh_LoadHypnosSwordChestDayColors,
                          gGaroh_HypnosSwordChestDayColors, 24, 8)

/// By night the path's colours: BG palette 6's colours 1 to 9.
DEFINE_GAROH_LOAD_PALETTE(Garoh_LoadHypnosSwordChestNightColors,
                          gGaroh_HypnosSwordChestNightColors, 97, 9)

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
extern struct ActorPlacement gGaroh_BlocksActorPlacements[];
extern struct ActorPlacement gGaroh_TunnelsActorPlacements[];
extern struct ActorPlacement gGaroh_HypnosSwordChestActorPlacements[];
extern struct ActorPlacement gGaroh_WerewolfCaveActorPlacements[];

extern struct MapGridCellAnimation gGaroh_Doors[];

/// Return the table for the current room.
const void *Scene_GetActorPlacements(void) {
  s16 scene = gMainRoutineWork.next;

  if (scene == SCENE_GAROH) {
    if (Flag_IsSet(FLAG_GAROH_SECRET_SHARED))
      return gGaroh_VillageDayActorPlacements;
    return gGaroh_VillageActorPlacements;
  }
  if (scene == SCENE_GAROH_HYPNOS_SWORD_CHEST)
    return gGaroh_HypnosSwordChestActorPlacements;
  if (scene == SCENE_GAROH_BLOCKS)
    return gGaroh_BlocksActorPlacements;
  if (scene == SCENE_GAROH_TUNNELS)
    return gGaroh_TunnelsActorPlacements;
  if (scene == SCENE_GAROH_WEREWOLF_CAVE)
    return gGaroh_WerewolfCaveActorPlacements;
  return gGaroh_ActorPlacements;
}

/// Stepping into a door: every placed actor stops, the door opens and the
/// leader walks through, leaving by the step trigger's exit.
void Garoh_OpenDoorAndEnter(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  s32 index;
  s32 i;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  for (i = ACTOR_FIRST_PLACED; (u32)i <= 63; i++) {
    {
      struct Actor *actor = Actor_Get(i);

      if (actor != NULL)
        actor->motionFlags = 0;
    }
  }
  index = work->stepTrigger;
  Event_PlaySound(SE_DOOR_OPEN);
  index--;
  MapGrid_AnimateCells(gGaroh_Doors[index].steps, gGaroh_Doors[index].x,
                       gGaroh_Doors[index].y);
  Actor_SetAnimation(gMainRoutineWork.leaderActorId, ANIM_WALK);
  Actor_SetSpeed(gMainRoutineWork.leaderActorId, 0.5);
  {
    s32 owner = gMainRoutineWork.leaderActorId;

    if (index == 5)
      Actor_SetDestinationOffset(owner, 0, -4);
    else
      Actor_CenterAndWalk(owner, 2, -4);
  }
  FieldEvent_Wait(6);
  FieldEvent_SetExitRequest(work->stepTrigger);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Event_End();
}

/// "So, uh...you say you saw a werewolf?"
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

/// "Don't you love the sight of the full moon?"
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

/// By day: "Maha said that you were the only outsiders we could share our
/// secret with..."
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

/// By day: "Awoooooh!" and no need to hide it any more.
void Garoh_HowlOutLoud(s32 actor) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)MsgGarohAwoooooh);
  // "Awoooooh!"
  FieldEvent_ShowMessageThenWait(actor, 0, 10);
  // "Heh heh... Sorry about that! I'm so happy I don't have to hide that..."
  FieldEvent_ShowMessageThenWait(actor, 0, 10);
  Event_End();
}

/// "Gaaaroooooo!" ... "I was just calling my dog."
void Garoh_HowlForDog(s32 actor) {
  s32 flag;
  s32 msg;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
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

/// By day: "Hey, it's you guys! Thanks for last night!" from the little
/// werewolf of the cave.
void Garoh_LittleWerewolfThanks(void) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldActor_Stop(ACTOR_LITTLE_WEREWOLF);
  Actor_SetAnimation(ACTOR_LITTLE_WEREWOLF, 1);
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
  Actor_SetScript(ACTOR_LITTLE_WEREWOLF, 2);
  Event_End();
}

/// By the Sanctum, the first night: the old werewolf opens the cave's entrance
/// by Reveal, sees the party ("What!? Outsiders, here?") and hides again.
void Garoh_WerewolfPeersFromCave(void) {
  s32 flag;
  s32 facing;

  void Garoh_OpenCaveEntrance(s32 id) {
    FieldPsynergy_LoadResources(ABILITY_REVEAL, 1);
    FieldPsynergy_SetupActorPair(id, 0);
    FieldPsynergy_RunHandler(1);
    BattleEffect_StopMainActor();
    FieldPsynergy_End();
  }

  void Garoh_CloseCaveEntrance(s32 id) {
    FieldPsynergy_LoadResources(ABILITY_REVEAL, 1);
    FieldPsynergy_SetupActorPair(id, 0);
    FieldPsynergy_RunHandler(3);
    BattleEffect_StopMainActor();
    FieldPsynergy_End();
  }

  flag = Event_GetFlag(FLAG_WEREWOLF_PEERED_OUT);
  if (flag == 0) {
    Event_SetFlag(FLAG_WEREWOLF_PEERED_OUT);
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
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
extern struct Trigger gGaroh_HypnosSwordChestTriggers[];
extern struct Trigger gGaroh_TunnelsTriggers[];
extern struct Trigger gGaroh_WerewolfCaveTriggers[];

extern s32 gGaroh_HowlFrames;

/// Stepping into an opening Reveal has shown: the cave's entrance by the
/// Sanctum leads down into it, and the passage in the cave on.
void Garoh_EnterOpening(void) {
  struct FieldWork *event = Ram_HeapSlots->fieldWork;

  Event_PlaySound(SE_MAP_EXIT);
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  Flag_Set(FLAG_ARRIVAL_STAIRS_DOWN);
  FieldEvent_SetExitRequest(event->stepTrigger);
}

/// In the cave: the werewolves gather the first night, and Maha tells the
/// legend once the secret is shared.
void Garoh_MeetInTheCave(void) {
  if (!Flag_IsSet(FLAG_GAROH_SECRET_SHARED)) {
    if (!Flag_IsSet(FLAG_GAROH_MET_WEREWOLVES))
      Garoh_WerewolvesRevealThemselves();
  } else {
    if (!Flag_IsSet(FLAG_GAROH_HEARD_MAHA))
      Garoh_MahaTellsTheLegend();
  }
}

/// Return the table for the current room.
const void *Scene_GetTriggers(void) {
  s16 scene = gMainRoutineWork.next;

  if (scene == SCENE_GAROH) {
    if (Flag_IsSet(FLAG_GAROH_SECRET_SHARED))
      return gGaroh_VillageDayTriggers;
    return gGaroh_VillageTriggers;
  }
  if (scene == SCENE_GAROH_HYPNOS_SWORD_CHEST)
    return gGaroh_HypnosSwordChestTriggers;
  if (scene == SCENE_GAROH_BLOCKS)
    return gGaroh_BlocksTriggers;
  if (scene == SCENE_GAROH_TUNNELS)
    return gGaroh_TunnelsTriggers;
  if (scene == SCENE_GAROH_WEREWOLF_CAVE)
    return gGaroh_WerewolfCaveTriggers;
  return gGaroh_Triggers;
}

/// A howl comes each time the night's frames under this mask are 0.
#define HOWL_PERIOD_MASK 0x3ff

/// A howl every 1,024 frames while it is night.
void Garoh_HowlAtNight(void) {
  if (!Flag_IsSet(FLAG_GAROH_SECRET_SHARED) && Flag_IsSet(FLAG_GAROH_NIGHT)) {
    gGaroh_HowlFrames++;
    if ((gGaroh_HowlFrames & HOWL_PERIOD_MASK) == 0)
      Event_PlaySound(SE_HOWL);
  }
}

void Garoh_InitializeHypnosSwordChest(void);

/// Sets up the room being entered: the cave's lamps and Maha once he has told
/// the legend, the village by night or day, the werewolf's path, or the way
/// up, which also makes the sanctum the place a defeat returns the party to.
s32 Scene_Initialize(void) {
  struct FieldWork *work;

  if (gMainRoutineWork.next == SCENE_GAROH_TUNNELS) {
    gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
  } else if (gMainRoutineWork.next == SCENE_GAROH_WEREWOLF_CAVE) {
    FieldActor_SetSpritePriority(ACTOR_FIRE_A, 1);
    FieldActor_SetSpritePriority(ACTOR_FIRE_B, 1);
    if (Event_GetFlag(FLAG_GAROH_HEARD_MAHA)) {
      Actor_SetPositionAndFacing(ACTOR_MAHA, toFIXED(328), toFIXED(162),
                                 FACING_SOUTH);
    }
    gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
    FieldEffect_StartBlankOverlay(0);
    if (gMainRoutineWork.entrance == 6) {
      work = Ram_HeapSlots->fieldWork;
      work->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
      Event_ClearFlag(FLAG_ARRIVAL_STAIRS_UP);
      Event_ClearFlag(FLAG_ARRIVAL_STAIRS_DOWN);
      gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
      Event_StartScreenTransition();
      Event_WaitTransitionFrames();
    }
  } else {
    gFieldWork->transition = 0;
    if (gMainRoutineWork.next == SCENE_GAROH) {
      Event_SetFlag(FLAG_GAROH_REACHED_VILLAGE);
      Garoh_InitializeVillage();
    } else if (gMainRoutineWork.next == SCENE_GAROH_HYPNOS_SWORD_CHEST) {
      Garoh_InitializeHypnosSwordChest();
    } else if (gMainRoutineWork.next == SCENE_GAROH_BLOCKS) {
      Garoh_InitializeBlocks();
      if (Event_GetFlag(FLAG_GAROH_REACHED_VILLAGE)) {
        gMainRoutineWork.restartScene = SCENE_GAROH_ROOMS_SANCTUM;
        gMainRoutineWork.restartEntrance = 2;
      }
    }
  }
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

/// The village: night colours, or by day the cells of the opened cave
/// entrance.
void Garoh_InitializeVillage(void) {
  FieldTask_AddFrame(Garoh_HowlAtNight, TASK_PASS_FRAME);
  gFieldWork->transition = 0;
  if (Event_GetFlag(FLAG_GAROH_SECRET_SHARED) == 0 ||
      Event_GetFlag(FLAG_GAROH_NIGHT) != 0) {
    Event_SetFlag(FLAG_GAROH_NIGHT);
    PaletteFade_ApplyTarget(PALETTE_TINT_WARM_DARK, 1);
    if (Event_GetFlag(FLAG_AREA_TEMP_11))
      PaletteFade_ApplyTarget(PALETTE_TINT_WARM, 2);
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
  if (gMainRoutineWork.entrance == GAROH_FROM_GAROH_WEREWOLF_CAVE) {
    struct FieldWork *work = Ram_HeapSlots->fieldWork;

    work->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
    Event_ClearFlag(FLAG_ARRIVAL_STAIRS_UP);
    Event_ClearFlag(FLAG_ARRIVAL_STAIRS_DOWN);
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    gFieldWork->transition = 0;
  }
}

/// The werewolf's path: the tightrope strung, and by night its colours and
/// the rippling layer, by day the opened cells.
void Garoh_InitializeHypnosSwordChest(void) {
  Task_AddFrame(Garoh_HowlAtNight, TASK_PASS_FRAME);
  Tightrope_Initialize();
  Tightrope_Place(0, ACTOR_PATH_TIGHTROPE_POST_A, ACTOR_PATH_TIGHTROPE_POST_B);
  if (Event_GetFlag(FLAG_GAROH_NIGHT)) {
    if (gMainRoutineWork.entrance == ENTRANCE_PATH_PURPLE_NIGHT) {
      PaletteFade_ApplyTarget(PALETTE_TINT_CUSTOM | RGB(9, 6, 18), 1);
    } else {
      PaletteFade_ApplyTarget(PALETTE_TINT_WARM_DARK, 1);
    }
    PaletteFade_Interpolate(1);
    Event_WaitTransitionFrames();
    PaletteFade_PackTarget();
    Garoh_LoadHypnosSwordChestNightColors();
    PaletteFade_ApplyTarget(PALETTE_TINT_NONE, 1);
    Task_AddFrame((s32)Garoh_WaveHypnosSwordChestLayer, TASK_PASS_VBLANK);
  } else {
    MapGrid_CopyCellsTo(20, 7, 9, 7, 5, 5);
    MapGrid_CopyCellsTo(20, 71, 9, 71, 5, 5);
    MapGrid_CopyCellsTo(84, 7, 73, 7, 5, 5);
    Garoh_LoadHypnosSwordChestDayColors();
    PaletteFade_ApplyTarget(PALETTE_TINT_NONE, 1);
    Garoh_MatchSecondLayer();
  }
}

extern s16 gGaroh_Pillars[];

/// The way up from the World Map: the pillars Move, Lash and a push place, in
/// the evening's or the night's light once the party has crossed into it.
void Garoh_InitializeBlocks(void) {
  FieldTask_AddFrame(Garoh_HowlAtNight, TASK_PASS_FRAME);
  if (Flag_IsSet(FLAG_GAROH_NIGHT)) {
    if (Flag_IsSet(FLAG_EVENING))
      PaletteFade_ApplyTarget(PALETTE_TINT_WARM, 1);
    else
      PaletteFade_ApplyTarget(PALETTE_TINT_WARM_DARK, 1);
  }
  Actor_PlaceBlocks(gGaroh_Pillars);
  PaletteFade_Interpolate(1);
  Event_WaitTransitionFrames();
}

/// Move lifts the pillar it is cast on.
void Garoh_LiftPillar(void) { Actor_LiftTargetBlock(gGaroh_Pillars); }

/// Move lifts the pillar it is cast on, from the cells a push reaches.
void Garoh_LiftPushedPillar(void) { Actor_LiftTargetBlock(gGaroh_Pillars); }

/// A block left on its cell rises with a whoosh and a ring of dust.
void Garoh_RaisePillar(s32 id, struct Actor *actor) {
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

#define DEFINE_GAROH_TRIGGER_AT_CELL(name, actorId, flag, cellX, cellZ)        \
  void name(void) {                                                            \
    struct Actor *actor = Actor_Get(actorId);                                  \
    s32 x = FIELD_CELL(actor->x.fixed);                                        \
    s32 raw;                                                                   \
    s32 z;                                                                     \
                                                                               \
    raw = actor->z.fixed;                                                      \
    z = FIELD_CELL(raw);                                                       \
    if (!Flag_IsSet(flag) && x == cellX && z == cellZ) {                       \
      Garoh_RaisePillar(actorId, actor);                                       \
      Flag_Set(flag);                                                          \
    }                                                                          \
  }

/// The first pillar rises once it stands on its cell.
DEFINE_GAROH_TRIGGER_AT_CELL(Garoh_RaiseFirstPillarOnCell, ACTOR_FIRST_PILLAR,
                             FLAG_FIRST_PILLAR_RAISED, 10, 29)

/// The second pillar rises once it stands on its cell.
DEFINE_GAROH_TRIGGER_AT_CELL(Garoh_RaiseSecondPillarOnCell, ACTOR_SECOND_PILLAR,
                             FLAG_SECOND_PILLAR_RAISED, 15, 28)

/// The third pillar has no cell to rise on: its check reads the pillar and the
/// rope's flag and does nothing.
void Garoh_CheckThirdPillar(void) {
  Actor_Get(ACTOR_THIRD_PILLAR);
  Flag_IsSet(FLAG_GAROH_ROPE_LASHED);
}

/// The fourth pillar rises once it stands on its cell.
void Garoh_RaiseFourthPillarOnCell(void) {
  struct Actor *actor = Actor_Get(ACTOR_FOURTH_PILLAR);
  s32 x = FIELD_CELL(actor->x.fixed);
  s32 z = FIELD_CELL(actor->z.fixed);

  if (!Flag_IsSet(FLAG_FOURTH_PILLAR_RAISED) && x == 15 && z == 25) {
    Garoh_RaisePillar(ACTOR_FOURTH_PILLAR, actor);
    Flag_Set(FLAG_FOURTH_PILLAR_RAISED);
  }
}

/// Move sets the first pillar down, and it rises if it is on its cell.
void Garoh_PlaceFirstPillar(void) {
  Actor_PlaceMovedBlock();
  Garoh_RaiseFirstPillarOnCell();
}

/// Move sets the second pillar down, and it rises if it is on its cell.
void Garoh_PlaceSecondPillar(void) {
  Actor_PlaceMovedBlock();
  Garoh_RaiseSecondPillarOnCell();
}

/// Move sets the third pillar down.
void Garoh_PlaceThirdPillar(void) {
  Actor_PlaceMovedBlock();
  Garoh_CheckThirdPillar();
}

/// Move sets the fourth pillar down, and it rises if it is on its cell.
void Garoh_PlaceFourthPillar(void) {
  Actor_PlaceMovedBlock();
  Garoh_RaiseFourthPillarOnCell();
}

/// A pushed pillar comes to rest: each pillar on its cell rises.
void Garoh_PlacePushedPillar(void) {
  Actor_PlaceMovedBlock();
  Garoh_RaiseFirstPillarOnCell();
  Garoh_RaiseSecondPillarOnCell();
  Garoh_CheckThirdPillar();
  Garoh_RaiseFourthPillarOnCell();
}

/// Reveal opens the cave's entrance by the Sanctum.
void Garoh_RevealCaveEntrance(void) {
  MapGrid_CopyCellsTo(98, 8, 86, 8, 1, 2);
  MapGrid_CopyCellAttributes(34, 9, 1, 1, 22, 11);
}

/// Reveal opens the passage into the tunnels.
void Garoh_RevealTunnelPassage(void) {
  MapGrid_CopyCellsTo(25, 47, 11, 33, 1, 3);
  MapGrid_CopyCellAttributes(25, 47, 1, 1, 11, 33);
}

/// Reveal opens the passage into the cave.
void Garoh_RevealCavePassage(void) {
  MapGrid_CopyCellsTo(44, 5, 19, 5, 3, 3);
  MapGrid_CopyCellsTo(108, 5, 83, 5, 3, 2);
  MapGrid_CopyCellAttributes(44, 5, 3, 3, 19, 5);
}

/// Reveal wearing off closes the cave's entrance by the Sanctum again.
void Garoh_HideCaveEntrance(void) {
  MapGrid_CopyCellsTo(102, 8, 86, 8, 1, 2);
  MapGrid_CopyCellAttributes(22, 10, 1, 1, 22, 11);
}

/// Whether it is night in Garoh.
s32 Garoh_IsNight(void) { return Flag_IsSet(FLAG_GAROH_NIGHT); }

/// Reveal wearing off in the way up: the evening's or the night's light comes
/// back.
void Garoh_RestoreBlocksDark(void) {
  if (Flag_IsSet(FLAG_GAROH_NIGHT)) {
    if (Flag_IsSet(FLAG_EVENING))
      PaletteFade_ApplyTarget(PALETTE_TINT_WARM, 1);
    else
      PaletteFade_ApplyTarget(PALETTE_TINT_WARM_DARK, 1);
    PaletteFade_Interpolate(16);
  }
}

/// Reveal wearing off closes the passage into the tunnels again.
void Garoh_HideTunnelPassage(void) {
  MapGrid_CopyCellsTo(8, 30, 11, 33, 1, 3);
  MapGrid_CopyCellAttributes(8, 30, 1, 1, 11, 33);
}

/// Reveal wearing off closes the passage into the cave again.
void Garoh_HideCavePassage(void) {
  MapGrid_CopyCellsTo(39, 5, 19, 5, 3, 3);
  MapGrid_CopyCellsTo(103, 5, 83, 5, 3, 2);
  MapGrid_CopyCellAttributes(39, 5, 3, 3, 19, 5);
}

/// Crossing the evening's west edge on the way up: from the World Map the
/// evening begins, from the path the day comes back.
void Garoh_CrossWestEdge(void) {
  struct FieldWork *work;

  if (!Flag_IsSet(FLAG_GAROH_SECRET_SHARED)) {
    Flag_Set(FLAG_CROSSED_WEST_EDGE);
    if (gMainRoutineWork.entrance == GAROH_BLOCKS_FROM_WORLD_MAP) {
      PaletteFade_ApplyTarget(PALETTE_TINT_WARM, 1);
      Flag_Set(FLAG_GAROH_NIGHT);
      Flag_Set(FLAG_EVENING);
    } else {
      PaletteFade_ApplyTarget(PALETTE_TINT_NONE, 1);
      Flag_Clear(FLAG_GAROH_NIGHT);
      Flag_Clear(FLAG_EVENING);
    }
    work = Ram_HeapSlots->fieldWork;
    if (work->revealActive == 0)
      PaletteFade_Interpolate(120);
  }
}

/// Crossing the evening's east edge on the way up: from the path the evening
/// begins, from the World Map the night falls.
void Garoh_CrossEastEdge(void) {
  struct FieldWork *work;

  if (!Flag_IsSet(FLAG_GAROH_SECRET_SHARED)) {
    Flag_Set(FLAG_CROSSED_EAST_EDGE);
    if (gMainRoutineWork.entrance ==
        GAROH_BLOCKS_FROM_GAROH_HYPNOS_SWORD_CHEST) {
      PaletteFade_ApplyTarget(PALETTE_TINT_WARM, 1);
      Flag_Set(FLAG_GAROH_NIGHT);
      Flag_Set(FLAG_EVENING);
    } else {
      PaletteFade_ApplyTarget(PALETTE_TINT_WARM_DARK, 1);
      Flag_Set(FLAG_GAROH_NIGHT);
      Flag_Clear(FLAG_EVENING);
    }
    work = Ram_HeapSlots->fieldWork;
    if (work->revealActive == 0)
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

/// Lash ties the rope between the third and fourth pillars.
void Garoh_LashRope(void) {
  Rope_Lash(1, ACTOR_THIRD_PILLAR, ACTOR_FOURTH_PILLAR);
  Flag_Set(FLAG_GAROH_ROPE_LASHED);
}

/// Until the secret is shared, whether the party has met the werewolves.
s32 Garoh_MetWerewolves(void) {
  if (!Flag_IsSet(FLAG_GAROH_SECRET_SHARED))
    return Flag_IsSet(FLAG_GAROH_MET_WEREWOLVES);
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
  struct Actor *leader = Actor_Get(gMainRoutineWork.leaderActorId);

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
  leader = Actor_Get(gMainRoutineWork.leaderActorId);
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

/// The first night: a werewolf howls ("Gaaaroooooo!") and runs along the path
/// back into the village.
void Garoh_WerewolfRunsPast(void) {
  struct Actor *werewolf;
  s32 i;
  s32 position[3];
  s32 *pos;
  FIXED x, z;
  FIXED toX, toZ;

  Event_SetFlag(FLAG_GAROH_SAW_WEREWOLF);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
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
  Actor_SetAnimation(ACTOR_WEREWOLF, 2);
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
  Actor_SetAnimation(ACTOR_WEREWOLF, 1);
  FieldEvent_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_WEREWOLF, 2);
  Actor_SetScript(ACTOR_WEREWOLF, gGaroh_WerewolfLookAroundScript);
  gFieldWork->message--;
  FieldEvent_ShowMessageThenWait(ACTOR_WEREWOLF, 0, 0);
  FieldActor_Stop(ACTOR_WEREWOLF);
  FieldEvent_Wait(20);
  Actor_FaceDirection(ACTOR_WEREWOLF, FACING_SOUTHEAST, 0);
  FieldEvent_Wait(20);
  EventCamera_FollowActor(ACTOR_WEREWOLF, 1);
  Actor_SetAnimation(ACTOR_WEREWOLF, 2);
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
  Actor_SetAnimation(ACTOR_WEREWOLF, 1);
  Actor_FaceDirection(ACTOR_WEREWOLF, FACING_SOUTH_SOUTHEAST, 0);
  FieldEvent_Wait(20);
  Actor_FaceDirection(ACTOR_WEREWOLF, FACING_EAST, 0);
  FieldEvent_Wait(30);
  FieldEvent_ShowMessageThenWait(ACTOR_WEREWOLF, 0, 0);
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
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_EAST, 0);
  if (Event_GetFlag(FLAG_IN_PARTY(ACTOR_PIERS)))
    Actor_FaceDirection(ACTOR_PIERS, FACING_NORTH_NORTHEAST, 0);
  FieldEvent_Wait(25);
  Actor_RunRepeatedMotion(ACTOR_JENNA, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(ACTOR_JENNA, 0, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 0);
  FieldEvent_Wait(25);
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 0);
  Actor_RunRepeatedMotion(ACTOR_SHEBA, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_SHEBA, 0, 0);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 0);
  if (Event_GetFlag(FLAG_IN_PARTY(ACTOR_PIERS)))
    Actor_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_QUESTION, 50);
  Actor_RunRepeatedMotion(ACTOR_JENNA, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(ACTOR_JENNA, 0, 0);
  Actor_AnimateThenWait(ACTOR_KRADEN, ANIM_NOD, 20);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 0);
  Actor_RunRepeatedMotion(ACTOR_SHEBA, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_SHEBA, 0, 0);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 0);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_QUESTION, 50);
  Actor_AnimateThenWait(ACTOR_KRADEN, ANIM_NOD, 20);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 0);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 0);
  Actor_SetAnimation(ACTOR_JENNA, ANIM_NOD);
  Actor_SetAnimation(ACTOR_SHEBA, ANIM_NOD);
  if (Event_GetFlag(FLAG_IN_PARTY(ACTOR_PIERS)))
    Actor_SetAnimation(ACTOR_PIERS, ANIM_NOD);
  Actor_AnimateThenWait(ACTOR_FELIX, ANIM_NOD, 20);
  FieldEvent_Wait(30);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 0);
  Actor_AnimateThenWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD, 33);
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
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTHWEST, 0);
  if (Event_GetFlag(FLAG_IN_PARTY(ACTOR_PIERS)))
    Actor_FaceDirection(ACTOR_PIERS, FACING_NORTH_NORTHWEST, 0);
  FieldEvent_Wait(20);
  Event_OpenMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_JENNA, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_SOUTHEAST, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(4, 0) == 0) {
    FieldEvent_Wait(30);
    Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
    FieldEvent_Wait(20);
    Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 0);
    FieldEvent_Wait(20);
    if (Event_GetFlag(FLAG_IN_PARTY(ACTOR_PIERS)))
      Actor_FaceDirection(ACTOR_PIERS, FACING_NORTH, 0);
    FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0, 0);
    gFieldWork->message++;
  } else {
    FieldEvent_Wait(40);
    Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
    FieldEvent_Wait(20);
    Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 0);
    FieldEvent_Wait(20);
    if (Event_GetFlag(FLAG_IN_PARTY(ACTOR_PIERS)))
      Actor_FaceDirection(ACTOR_PIERS, FACING_NORTH, 0);
    gFieldWork->message++;
    FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0, 0);
  }
  FieldActor_Jump(ACTOR_KRADEN, 6, 15);
  FieldActor_Jump(ACTOR_KRADEN, 6, 23);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0, 0);
  Actor_RunRepeatedMotion(ACTOR_JENNA, 2);
  FieldEvent_Wait(10);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
  FieldEvent_Wait(15);
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
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_SHEBA, 0, 0);
  FieldActor_StartRepeatedMotion(ACTOR_KRADEN, 3);
  Actor_SetAttachedEffect(ACTOR_KRADEN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(40);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0, 0);
  Garoh_PopQuestionMarks(ACTOR_SHEBA);
  FieldEvent_Wait(20);
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
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTHWEST, 0);
  FieldEvent_Wait(20);
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
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0, 0);
  Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  if (Event_GetFlag(FLAG_IN_PARTY(ACTOR_PIERS)))
    Actor_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 50);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
  FieldEvent_Wait(10);
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
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0, 0);
  Actor_AnimateThenWait(ACTOR_KRADEN, ANIM_NOD, 20);
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
  EventCamera_FollowActor(gMainRoutineWork.leaderActorId, 1);
  Event_End();
}

/// Into the cave: "Look! It's that werewolf!", the little werewolf, which
/// runs deeper in.
void Garoh_FollowWerewolfIntoCave(void) {
  Event_SetFlag(FLAG_GAROH_FOLLOWED_WEREWOLF);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
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
  EventCamera_FollowActor(gMainRoutineWork.leaderActorId, 1);
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

/// "Where could that werewolf have gone?" The werewolves surround the party and
/// Maha speaks with it.
void Garoh_WerewolvesRevealThemselves(void) {
  Event_SetFlag(FLAG_GAROH_MET_WEREWOLVES);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
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
  // "Do you think we lost him somewhere?"
  Event_OpenMessage(ACTOR_KRADEN, 0);
  if (!FieldEvent_AskYesNoWithActorAnimation(4, 0)) {
    FieldEvent_Wait(20);
    Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
    FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 0);
    gFieldWork->message = (gFieldWork->message + 1);
  } else {
    FieldEvent_Wait(30);
    Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
    gFieldWork->message = (gFieldWork->message + 1);
    FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 0);
  }
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
  FieldEvent_ShowMessageThenWait(ACTOR_JENNA, 0, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTHWEST, 0);
  FieldEvent_Wait(20);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTHEAST, 0);
  FieldEvent_Wait(20);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTHWEST, 0);
  FieldEvent_Wait(20);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTH, 0);
  FieldEvent_Wait(20);
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
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 0);
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
  FieldEvent_ShowMessageThenWait(ACTOR_MAHA, 0, 0);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  FieldEvent_ShowMessageThenWait(ACTOR_SHEBA, 0, 0);
  FieldActor_StartRepeatedMotion(ACTOR_WEREWOLF, 3);
  Actor_SetAttachedEffect(ACTOR_WEREWOLF,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(40);
  FieldEvent_ShowMessageThenWait(ACTOR_WEREWOLF, 0, 0);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 0);
  FieldActor_Jump(ACTOR_WEREWOLF, 6, 15);
  FieldActor_Jump(ACTOR_WEREWOLF, 6, 23);
  Actor_FaceDirection(ACTOR_WEREWOLF, FACING_SOUTHWEST, 0);
  FieldEvent_Wait(20);
  Actor_AnimateThenWait(ACTOR_MAHA, ANIM_SHAKE_HEAD, 33);
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
  Event_OpenMessage(ACTOR_MAHA, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(4, 0)) {
    FieldEvent_Wait(40);
    Actor_AnimateThenWait(ACTOR_MAHA, ANIM_SHAKE_HEAD, 33);
    FieldEvent_ShowMessageThenWait(ACTOR_MAHA, 0, 0);
    gFieldWork->message = (gFieldWork->message + 1);
  } else {
    FieldEvent_Wait(30);
    Actor_RunRepeatedMotion(ACTOR_MAHA, 2);
    FieldEvent_Wait(10);
    gFieldWork->message = (gFieldWork->message + 1);
    FieldEvent_ShowMessageThenWait(ACTOR_MAHA, 0, 0);
  }
  Actor_RunRepeatedMotion(ACTOR_WEREWOLF, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(ACTOR_WEREWOLF, 0, 0);
  Actor_FaceDirection(ACTOR_MAHA, FACING_EAST, 0);
  FieldEvent_Wait(30);
  Actor_AnimateThenWait(ACTOR_MAHA, ANIM_NOD, 20);
  FieldEvent_ShowMessageThenWait(ACTOR_MAHA, 0, 0);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  FieldEvent_ShowMessageThenWait(ACTOR_JENNA, 0, 0);
  Actor_RunRepeatedMotion(ACTOR_MAHA, 2);
  FieldEvent_Wait(10);
  Actor_FaceDirection(ACTOR_WEREWOLF, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_MAHA, FACING_SOUTH, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(ACTOR_MAHA, 0, 0);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 30);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTHWEST, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(ACTOR_SHEBA, 0, 0);
  Actor_AnimateThenWait(ACTOR_MAHA, ANIM_SHAKE_HEAD, 33);
  FieldEvent_ShowMessageThenWait(ACTOR_MAHA, 0, 0);
  Actor_ShowEmote(ACTOR_MAHA, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  FieldEvent_ShowMessageThenWait(ACTOR_MAHA, 0, 0);
  Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  if (Event_GetFlag(FLAG_IN_PARTY(ACTOR_PIERS))) {
    Actor_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  }
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Actor_AnimateThenWait(ACTOR_MAHA, ANIM_NOD, 20);
  FieldMessage_ShowAtActor(ACTOR_MAHA, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_WEST, 0);
  if (Event_GetFlag(FLAG_IN_PARTY(ACTOR_PIERS))) {
    Actor_FaceDirection(ACTOR_PIERS, FACING_WEST, 0);
  }
  FieldEvent_Wait(20);
  if (FieldEvent_AskYesNoWithActorAnimation(4, 0)) {
    FieldEvent_Wait(40);
    Actor_RunRepeatedMotion(ACTOR_JENNA, 2);
    FieldEvent_Wait(20);
    FieldEvent_ShowMessageThenWait(ACTOR_JENNA, 0, 0);
    gFieldWork->message = (gFieldWork->message + 1);
  } else {
    FieldEvent_Wait(30);
    Actor_AnimateThenWait(ACTOR_JENNA, ANIM_NOD, 20);
    FieldEvent_Wait(20);
    gFieldWork->message = (gFieldWork->message + 1);
    FieldEvent_ShowMessageThenWait(ACTOR_JENNA, 0, 0);
  }
  Actor_FaceDirection(ACTOR_MAHA, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_WEREWOLF, FACING_SOUTHWEST, 0);
  FieldEvent_Wait(30);
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
  FieldEvent_ShowMessageThenWait(ACTOR_SHEBA, 0, 0);
  Actor_FaceDirection(ACTOR_MAHA, FACING_SOUTH, 0);
  FieldEvent_Wait(30);
  FieldEvent_ShowMessageThenWait(ACTOR_MAHA, 0, 0);
  Actor_AnimateThenWait(ACTOR_KRADEN, ANIM_NOD, 20);
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 0);
  Actor_ShowEmote(ACTOR_MAHA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
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
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTHEAST, 0);
  FieldEvent_Wait(30);
  Actor_AnimateThenWait(ACTOR_MAHA, ANIM_NOD, 20);
  FieldEvent_ShowMessageThenWait(ACTOR_MAHA, 0, 0);
  Actor_ShowEmote(ACTOR_WEREWOLF, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  FieldEvent_ShowMessageThenWait(ACTOR_WEREWOLF, 0, 0);
  Actor_FaceDirection(ACTOR_MAHA, FACING_NORTHEAST, 0);
  FieldEvent_Wait(30);
  Actor_RunRepeatedMotion(ACTOR_JENNA, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(ACTOR_JENNA, 0, 0);
  Actor_FaceDirection(ACTOR_MAHA, FACING_SOUTH_SOUTHEAST, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(ACTOR_MAHA, 0, 0);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
  FieldEvent_ShowMessageThenWait(ACTOR_SHEBA, 0, 0);
  Actor_FaceDirection(ACTOR_WEREWOLF, FACING_SOUTH, 0);
  FieldEvent_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_WEREWOLF, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(ACTOR_WEREWOLF, 0, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_EAST, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTHEAST, 0);
  FieldEvent_Wait(30);
  Actor_AnimateThenWait(ACTOR_MAHA, ANIM_NOD, 20);
  FieldEvent_ShowMessageThenWait(ACTOR_MAHA, 0, 0);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  FieldEvent_ShowMessageThenWait(ACTOR_JENNA, 0, 0);
  Actor_RunRepeatedMotion(ACTOR_MAHA, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(ACTOR_MAHA, 0, 0);
  Actor_AnimateThenWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD, 33);
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 0);
  Actor_FaceDirection(ACTOR_WEREWOLF, FACING_SOUTH, 0);
  FieldEvent_Wait(20);
  Actor_WalkByAndWait(ACTOR_WEREWOLF, 0, 16);
  FieldEvent_Wait(10);
  FieldActor_Jump(ACTOR_WEREWOLF, 6, 15);
  FieldActor_Jump(ACTOR_WEREWOLF, 6, 23);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
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
  FieldActor_SetAnimation(ACTOR_WEREWOLF, 5);
  FieldEvent_Wait(20);
  Actor_SetSpeed(ACTOR_WEREWOLF, 1.3);
  Actor_SetSpeed(ACTOR_MAHA, 0.7);
  Actor_AnimateThenWait(ACTOR_MAHA, ANIM_SHAKE_HEAD, 33);
  FieldEvent_Wait(10);
  Actor_ShowEmote(ACTOR_WEREWOLF, EMOTE_IN_FRONT | EMOTE_WINCE, 50);
  Actor_SetAnimation(ACTOR_WEREWOLF, 1);
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
  FieldEvent_ShowMessageThenWait(ACTOR_MAHA, 0, 0);
  Actor_ShowEmote(ACTOR_WEREWOLF, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  FieldEvent_ShowMessageThenWait(ACTOR_WEREWOLF, 0, 0);
  Actor_AnimateThenWait(ACTOR_MAHA, ANIM_SHAKE_HEAD, 33);
  FieldEvent_ShowMessageThenWait(ACTOR_MAHA, 0, 0);
  Actor_FaceDirection(ACTOR_WEREWOLF, FACING_SOUTH_SOUTHWEST, 0);
  FieldEvent_Wait(45);
  Actor_FaceDirection(ACTOR_WEREWOLF, FACING_WEST, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(ACTOR_WEREWOLF, 0, 0);
  Actor_AnimateThenWait(ACTOR_MAHA, ANIM_SHAKE_HEAD, 33);
  FieldEvent_ShowMessageThenWait(ACTOR_MAHA, 0, 0);
  Actor_FaceDirection(ACTOR_MAHA, FACING_SOUTH_SOUTHWEST, 0);
  FieldEvent_Wait(45);
  Actor_FaceDirection(ACTOR_MAHA, FACING_EAST, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(ACTOR_MAHA, 0, 0);
  Actor_AnimateThenWait(ACTOR_WEREWOLF, ANIM_NOD, 20);
  FieldEvent_Wait(20);
  Actor_FaceDirection(ACTOR_WEREWOLF, FACING_SOUTH, 0);
  FieldEvent_Wait(35);
  Actor_AnimateThenWait(ACTOR_WEREWOLF, ANIM_NOD, 20);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(ACTOR_WEREWOLF, 0, 0);
  Actor_SetAnimation(ACTOR_FELIX, ANIM_NOD);
  Actor_SetAnimation(ACTOR_JENNA, ANIM_NOD);
  Actor_SetAnimation(ACTOR_SHEBA, ANIM_NOD);
  Actor_AnimateThenWait(ACTOR_KRADEN, ANIM_NOD, 20);
  FieldEvent_Wait(20);
  FieldActor_WatchAndSetScript(ACTOR_FELIX, 8);
  FieldActor_WatchAndSetScript(ACTOR_KRADEN, 8);
  FieldActor_WatchAndSetScript(ACTOR_JENNA, 8);
  FieldActor_WatchAndSetScript(ACTOR_SHEBA, 8);
  FieldActor_WatchAndSetScript(ACTOR_MAHA, 8);
  if (Event_GetFlag(FLAG_IN_PARTY(ACTOR_PIERS))) {
    FieldActor_WatchAndSetScript(ACTOR_PIERS, 8);
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
  FieldEvent_ShowMessageThenWait(ACTOR_MAHA, 0, 0);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 0);
  Actor_ShowEmote(ACTOR_MAHA, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  FieldEvent_ShowMessageThenWait(ACTOR_MAHA, 0, 0);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  FieldEvent_ShowMessageThenWait(ACTOR_SHEBA, 0, 0);
  Actor_ShowEmote(ACTOR_MAHA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
  FieldEvent_ShowMessageThenWait(ACTOR_MAHA, 0, 0);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 0);
  Actor_AnimateThenWait(ACTOR_MAHA, ANIM_NOD, 20);
  FieldEvent_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_JENNA, 2);
  FieldEvent_Wait(10);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
  FieldEvent_Wait(30);
  FieldEvent_ShowMessageThenWait(ACTOR_JENNA, 0, 0);
  Actor_AnimateThenWait(ACTOR_MAHA, ANIM_SHAKE_HEAD, 33);
  FieldEvent_ShowMessageThenWait(ACTOR_MAHA, 0, 0);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 50);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTH_NORTHWEST, 0);
  FieldEvent_Wait(30);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTHEAST, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 0);
  Actor_AnimateThenWait(ACTOR_MAHA, ANIM_NOD, 20);
  FieldEvent_Wait(10);
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
  EventCamera_FollowActor(gMainRoutineWork.leaderActorId, 1);
  Garoh_MetWerewolves();
  FieldEvent_SetExitRequest(GAROH_WEREWOLF_CAVE_TO_GAROH_ROOMS_HOUSES);
  Event_End();
}

/// By day: "Ah, you came... I've been waiting for you." Maha tells of the
/// werewolves' legend and Mt. Aleph.
void Garoh_MahaTellsTheLegend(void) {
  s32 frames;
  FIXED total;
  struct Actor *ether;
  struct Sprite *sprite;

  Event_SetFlag(FLAG_GAROH_HEARD_MAHA);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
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
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 0);
  Actor_WalkByAndWait(ACTOR_MAHA, -8, 0);
  Actor_FaceDirection(ACTOR_MAHA, FACING_SOUTH_SOUTHWEST, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(
      SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_ABOVE | ACTOR_MAHA, 0, 0);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 0);
  Actor_ShowEmote(ACTOR_MAHA, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
  FieldEvent_ShowMessageThenWait(
      SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_ABOVE | ACTOR_MAHA, 0, 0);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 0);
  Actor_RunRepeatedMotion(ACTOR_MAHA, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(
      SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_ABOVE | ACTOR_MAHA, 0, 0);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 0);
  Actor_WalkByAndWait(ACTOR_MAHA, 16, 0);
  Actor_FaceDirection(ACTOR_MAHA, FACING_SOUTH_SOUTHWEST, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(
      SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_ABOVE | ACTOR_MAHA, 0, 0);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  FieldEvent_ShowMessageThenWait(ACTOR_JENNA, 0, 0);
  Actor_FaceDirection(ACTOR_MAHA, FACING_SOUTHEAST, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(
      SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_ABOVE | ACTOR_MAHA, 0, 0);
  Actor_RunRepeatedMotion(ACTOR_SHEBA, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(ACTOR_SHEBA, 0, 0);
  Actor_WalkByAndWait(ACTOR_MAHA, -8, 0);
  Actor_FaceDirection(ACTOR_MAHA, FACING_SOUTH_SOUTHWEST, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(
      SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_ABOVE | ACTOR_MAHA, 0, 0);
  Actor_AnimateThenWait(ACTOR_FELIX, ANIM_NOD, 20);
  FieldEvent_Wait(35);
  Actor_WalkByAndWait(ACTOR_MAHA, -8, 0);
  Actor_FaceDirection(ACTOR_MAHA, FACING_SOUTH_SOUTHWEST, 0);
  FieldEvent_Wait(25);
  Actor_AnimateThenWait(ACTOR_MAHA, ANIM_NOD, 20);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(
      SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_ABOVE | ACTOR_MAHA, 0, 0);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 0);
  Actor_AnimateThenWait(ACTOR_MAHA, ANIM_SHAKE_HEAD, 33);
  FieldEvent_ShowMessageThenWait(
      SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_ABOVE | ACTOR_MAHA, 0, 0);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 45);
  FieldActor_Jump(ACTOR_KRADEN, 6, 15);
  FieldActor_Jump(ACTOR_KRADEN, 6, 23);
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 0);
  Actor_ShowEmote(ACTOR_MAHA, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  FieldEvent_ShowMessageThenWait(
      SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_ABOVE | ACTOR_MAHA, 0, 0);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
  FieldEvent_Wait(10);
  Actor_FaceDirection(ACTOR_MAHA, FACING_NORTH, 0);
  FieldEvent_Wait(30);
  FieldEvent_ShowMessageThenWait(
      SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_ABOVE | ACTOR_MAHA, 0, 0);
  Actor_WalkByAndWait(ACTOR_MAHA, 4, -12);
  FieldEvent_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_MAHA, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_MAHA, 0, 0);
  Actor_WalkByAndWait(ACTOR_MAHA, 4, -12);
  FieldEvent_Wait(10);
  Actor_AnimateThenWait(ACTOR_MAHA, ANIM_SHAKE_HEAD, 33);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_MAHA, 0, 0);
  Actor_FaceDirection(ACTOR_MAHA, FACING_SOUTH, 0);
  FieldEvent_Wait(30);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_MAHA, 0, 0);
  Actor_SetAnimation(ACTOR_FELIX, ANIM_NOD);
  Actor_SetAnimation(ACTOR_JENNA, ANIM_NOD);
  Actor_SetAnimation(ACTOR_SHEBA, ANIM_NOD);
  Actor_AnimateThenWait(ACTOR_KRADEN, ANIM_NOD, 20);
  FieldEvent_Wait(20);
  Actor_AnimateThenWait(ACTOR_MAHA, ANIM_NOD, 20);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_MAHA, 0, 0);
  Actor_RunRepeatedMotion(ACTOR_MAHA, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_MAHA, 0, 0);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 0);
  Actor_ShowEmote(ACTOR_MAHA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 30);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_MAHA, 0, 0);
  Actor_RunRepeatedMotion(ACTOR_MAHA, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_MAHA, 0, 0);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  FieldEvent_ShowMessageThenWait(ACTOR_SHEBA, 0, 0);
  Actor_FaceDirection(ACTOR_MAHA, FACING_SOUTH_SOUTHEAST, 0);
  FieldEvent_Wait(20);
  Actor_AnimateThenWait(ACTOR_MAHA, ANIM_NOD, 20);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_MAHA, 0, 0);
  Actor_ShowEmote(ACTOR_MAHA, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_MAHA, 0, 0);
  Actor_AnimateThenWait(ACTOR_KRADEN, ANIM_NOD, 20);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 0);
  Actor_FaceDirection(ACTOR_MAHA, FACING_SOUTH_SOUTHWEST, 0);
  FieldEvent_Wait(20);
  Actor_AnimateThenWait(ACTOR_MAHA, ANIM_SHAKE_HEAD, 33);
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
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 0);
  Actor_ShowEmote(ACTOR_MAHA, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_MAHA, 0, 0);
  Actor_AnimateThenWait(ACTOR_KRADEN, ANIM_NOD, 20);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 0);
  Actor_AnimateThenWait(ACTOR_MAHA, ANIM_NOD, 20);
  FieldEvent_Wait(10);
  Actor_AnimateThenWait(ACTOR_MAHA, ANIM_NOD, 20);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_MAHA, 0, 0);
  Actor_ShowEmote(ACTOR_MAHA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_MAHA, 0, 0);
  Actor_AnimateThenWait(ACTOR_KRADEN, ANIM_NOD, 20);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTHEAST, 0);
  FieldEvent_Wait(30);
  Actor_AnimateThenWait(ACTOR_KRADEN, ANIM_NOD, 20);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 0);
  Actor_RunRepeatedMotion(ACTOR_MAHA, 2);
  FieldEvent_Wait(15);
  Actor_FaceDirection(ACTOR_MAHA, FACING_NORTH, 0);
  FieldEvent_Wait(30);
  Actor_AnimateThenWait(ACTOR_MAHA, ANIM_NOD, 20);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_MAHA, 0, 0);
  Actor_SetAnimation(ACTOR_FELIX, ANIM_NOD);
  Actor_AnimateThenWait(ACTOR_JENNA, ANIM_NOD, 20);
  FieldEvent_Wait(30);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTHEAST, 0);
  FieldEvent_Wait(25);
  Actor_AnimateThenWait(ACTOR_KRADEN, ANIM_NOD, 20);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 0);
  FieldActor_StartRepeatedMotion(ACTOR_MAHA, 3);
  Actor_SetAttachedEffect(ACTOR_MAHA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(40);
  Actor_FaceDirection(ACTOR_MAHA, FACING_SOUTH_SOUTHWEST, 0);
  FieldEvent_Wait(30);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_MAHA, 0, 0);
  Actor_AnimateThenWait(ACTOR_KRADEN, ANIM_NOD, 20);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 0);
  Actor_ShowEmote(ACTOR_MAHA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_MAHA, 0, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_EAST, 0);
  FieldEvent_Wait(20);
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
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 0);
  Actor_SetSpeed(ACTOR_MAHA, 1.2);
  Actor_WalkByAndWait(ACTOR_MAHA, 16, 16);
  Actor_SetSpeed(ACTOR_MAHA, 0.7);
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
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 0);
  Actor_RunRepeatedMotion(ACTOR_MAHA, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(ACTOR_MAHA, 0, 0);
  Actor_RunRepeatedMotion(ACTOR_JENNA, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(ACTOR_JENNA, 0, 0);
  Actor_RunRepeatedMotion(ACTOR_MAHA, 2);
  FieldEvent_Wait(10);
  Actor_FaceDirection(ACTOR_MAHA, FACING_SOUTH_SOUTHWEST, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(ACTOR_MAHA, 0, 0);
  Actor_AnimateThenWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD, 33);
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 0);
  Actor_FaceDirection(ACTOR_MAHA, FACING_SOUTHEAST, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(ACTOR_MAHA, 0, 0);
  Actor_AnimateThenWait(ACTOR_KRADEN, ANIM_NOD, 20);
  FieldEvent_Wait(10);
  FieldEvent_Wait(20);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 40);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_WEST, 0);
  FieldEvent_Wait(20);
  Event_OpenMessage(ACTOR_SHEBA, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_EAST, 0);
  if (Event_GetFlag(FLAG_IN_PARTY(ACTOR_PIERS))) {
    Actor_FaceDirection(ACTOR_PIERS, FACING_WEST, 0);
  }
  if (!FieldEvent_AskYesNoWithActorAnimation(4, 0)) {
    FieldEvent_Wait(25);
    FieldEvent_ShowMessageThenWait(ACTOR_SHEBA, 0, 0);
    gFieldWork->message = (gFieldWork->message + 1);
  } else {
    FieldEvent_Wait(40);
    gFieldWork->message = (gFieldWork->message + 1);
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
  FieldEvent_ShowMessageThenWait(ACTOR_SHEBA, 0, 0);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 0);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
  FieldEvent_ShowMessageThenWait(ACTOR_SHEBA, 0, 0);
  Actor_AnimateThenWait(ACTOR_MAHA, ANIM_SHAKE_HEAD, 33);
  FieldEvent_ShowMessageThenWait(ACTOR_MAHA, 0, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTH, 0);
  FieldEvent_Wait(20);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 30);
  FieldEvent_ShowMessageThenWait(ACTOR_SHEBA, 0, 0);
  Actor_AnimateThenWait(ACTOR_SHEBA, ANIM_SHAKE_HEAD, 33);
  FieldEvent_ShowMessageThenWait(ACTOR_SHEBA, 0, 0);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Actor_ShowEmote(ACTOR_MAHA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 50);
  Actor_RunRepeatedMotion(ACTOR_SHEBA, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(ACTOR_SHEBA, 0, 0);
  Actor_SetAnimation(ACTOR_FELIX, ANIM_NOD);
  Actor_SetAnimation(ACTOR_JENNA, ANIM_NOD);
  Actor_SetAnimation(ACTOR_MAHA, 3);
  if (Event_GetFlag(FLAG_IN_PARTY(ACTOR_PIERS))) {
    Actor_SetAnimation(ACTOR_PIERS, ANIM_NOD);
  }
  Actor_SetAnimation(ACTOR_KRADEN, ANIM_NOD);
  FieldEvent_Wait(20);
  FieldEvent_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_SHEBA, 2);
  FieldEvent_Wait(10);
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
  FieldEvent_ShowMessageThenWait(ACTOR_MAHA, 0, 0);
  FieldActor_StartRepeatedMotion(ACTOR_KRADEN, 3);
  Actor_SetAttachedEffect(ACTOR_KRADEN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(40);
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 0);
  Actor_ShowEmote(ACTOR_MAHA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  FieldEvent_ShowMessageThenWait(ACTOR_MAHA, 0, 0);
  Actor_RunRepeatedMotion(ACTOR_MAHA, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(ACTOR_MAHA, 0, 0);
  Actor_AnimateThenWait(ACTOR_MAHA, ANIM_NOD, 20);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(ACTOR_MAHA, 0, 0);
  Actor_AnimateThenWait(ACTOR_SHEBA, ANIM_NOD, 20);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(ACTOR_SHEBA, 0, 0);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
  FieldEvent_Wait(10);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTHWEST, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTHWEST, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 0);
  Actor_AnimateThenWait(ACTOR_MAHA, ANIM_NOD, 20);
  FieldEvent_Wait(10);
  Actor_WalkByAndWait(ACTOR_KRADEN, -18, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 0);
  FieldEvent_Wait(20);
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
    FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0, 0);
    Actor_FaceDirection(ACTOR_MAHA, FACING_SOUTH_SOUTHWEST, 0);
    FieldEvent_Wait(20);
    Actor_AnimateThenWait(ACTOR_MAHA, ANIM_NOD, 20);
    FieldEvent_Wait(20);
    Actor_SetAnimation(ACTOR_PIERS, ANIM_NOD);
    FieldEvent_Wait(20);
    FieldEvent_Wait(10);
  } else {
    gFieldWork->message = (gFieldWork->message + 1);
  }
  Actor_WalkByAndWait(ACTOR_KRADEN, -42, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST, 0);
  FieldEvent_Wait(20);
  Actor_AnimateThenWait(ACTOR_KRADEN, ANIM_NOD, 20);
  FieldEvent_Wait(10);
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
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_MAHA, 0, 0);
  Actor_AnimateThenWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD, 33);
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
  frames = ETHER_GROW_STEPS;
  do {
    sprite->scale = Math_Divide(total, ETHER_GROW_STEPS) + FIXED_ONE * 2 / 5;
    ether->facing += FACING_STEP;
    System_Wait(1);
    frames--;
    total += FIXED_ONE * 3 / 5;
  } while (frames >= 0);
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
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_JENNA, 0, 0);
  Actor_ShowEmote(ACTOR_MAHA, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  Actor_FaceDirection(ACTOR_MAHA, FACING_SOUTH_SOUTHEAST, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_MAHA, 0, 0);
  Actor_WalkByAndWait(ACTOR_MAHA, 0, 24);
  Actor_FaceDirection(ACTOR_MAHA, FACING_SOUTH_SOUTHEAST, 0);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_MAHA, 0, 0);
  Actor_FaceDirection(ACTOR_MAHA, FACING_SOUTH_SOUTHWEST, 0);
  FieldEvent_Wait(20);
  Event_OpenMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MAHA, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_WEST, 0);
  if (Event_GetFlag(FLAG_IN_PARTY(ACTOR_PIERS))) {
    Actor_FaceDirection(ACTOR_PIERS, FACING_WEST, 0);
  }
  FieldEvent_Wait(20);
  if (!FieldEvent_AskYesNoWithActorAnimation(4, 0)) {
    FieldEvent_Wait(30);
    FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_MAHA, 0, 0);
    gFieldWork->message = (gFieldWork->message + 1);
  } else {
    FieldEvent_Wait(40);
    gFieldWork->message = (gFieldWork->message + 1);
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
  gFieldWork->message = (gFieldWork->message + 1);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  FieldEvent_ShowMessageThenWait(ACTOR_SHEBA, 0, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_EAST, 0);
  FieldEvent_Wait(30);
  Actor_AnimateThenWait(ACTOR_JENNA, ANIM_NOD, 20);
  FieldEvent_Wait(20);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTHEAST, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
  FieldEvent_Wait(20);
  Actor_AnimateThenWait(ACTOR_MAHA, ANIM_SHAKE_HEAD, 33);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_MAHA, 0, 0);
  Actor_AnimateThenWait(ACTOR_KRADEN, ANIM_NOD, 20);
  FieldEvent_Wait(20);
  Actor_AnimateThenWait(ACTOR_MAHA, ANIM_NOD, 20);
  FieldEvent_Wait(10);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
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
  EventCamera_FollowActor(gMainRoutineWork.leaderActorId, 1);
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

u16 gGaroh_HypnosSwordChestNightColors[] = {
    31, 31, 31, 29, 29, 30, 27, 28,
    29, 25, 27, 28, 23, 25, 27, 21,
    24, 26, 15, 18, 19, 10, 12, 13,
    5, 6, 6,
};

u16 gGaroh_HypnosSwordChestDayColors[] = {
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
    SCENE_EXIT_UNLESS(1, GarohRoomsHouses, 1, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, GarohRoomsShop, 4, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, GarohRoomsHouses, 3, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(4, GarohRoomsHouses, 4, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(5, GarohRoomsHouses, 5, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(6, GarohRoomsSanctum, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(7, GarohTunnels, 4, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(8, GarohTunnels, 1, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(9, GarohTunnels, 11, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(10, GarohHypnosSwordChest, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(11, GarohHypnosSwordChest, 3, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(12, GarohWerewolfCave, 1, EXIT_FLAG_NONE),
    SCENE_EXITS(GarohHypnosSwordChest),
    SCENE_EXIT_UNLESS(1, GarohBlocks, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, Garoh, 10, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, Garoh, 11, EXIT_FLAG_NONE),
    SCENE_EXITS(GarohBlocks),
    SCENE_EXIT_UNLESS(1, WorldMap, 15, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, GarohHypnosSwordChest, 1, EXIT_FLAG_NONE),
    SCENE_EXITS(GarohTunnels),
    SCENE_EXIT_UNLESS(1, Garoh, 8, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, GarohTunnels, 3, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, GarohTunnels, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(4, Garoh, 7, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(5, GarohRoomsShop, 3, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(6, GarohRoomsShop, 5, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(7, GarohRoomsHouses, 6, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(8, GarohRoomsHouses, 7, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(9, GarohTunnels, 10, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(10, GarohTunnels, 9, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(11, Garoh, 9, EXIT_FLAG_NONE),
    SCENE_EXITS(GarohWerewolfCave),
    SCENE_EXIT_UNLESS(1, Garoh, 12, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, GarohWerewolfCave, 4, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, GarohWerewolfCave, 5, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(4, GarohWerewolfCave, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(5, GarohWerewolfCave, 3, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(6, GarohWerewolfCave, 7, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(7, GarohWerewolfCave, 6, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(11, GarohRoomsHouses, 99, EXIT_FLAG_NONE),
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

struct ActorPlacement gGaroh_BlocksActorPlacements[] = {
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

struct ActorPlacement gGaroh_HypnosSwordChestActorPlacements[] = {
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
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 1, CONDITION_ALWAYS,
     EVENT(Garoh_OpenDoorAndEnter)},
    {TRIGGER_EXIT | TRIGGER_FLAG_FACING | FACING_NORTH, 2, CONDITION_ALWAYS, 2},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 3, CONDITION_ALWAYS,
     EVENT(Garoh_OpenDoorAndEnter)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 4, CONDITION_ALWAYS,
     EVENT(Garoh_OpenDoorAndEnter)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 5, CONDITION_ALWAYS,
     EVENT(Garoh_OpenDoorAndEnter)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 6, CONDITION_ALWAYS,
     EVENT(Garoh_OpenDoorAndEnter)},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT | TRIGGER_FLAG_FACING | FACING_NORTH, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {STEP_TRIGGER_FACING(FACING_NORTH), 12, CONDITION_ALWAYS,
     EVENT(Garoh_EnterOpening)},
    {TRIGGER_STEP, 20, FLAG_GAROH_MET_WEREWOLVES,
     EVENT(Garoh_WerewolfPeersFromCave)},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, EVENT(Garoh_AskIfSawWerewolf)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgGarohGoodEveningForgiveMeFor)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, EVENT(Garoh_HowlForDog)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS,
     MSGNO(MsgGarohYouThereStrangersShouldNot)},
    {TRIGGER_TALK, 12, CONDITION_ALWAYS, EVENT(Garoh_AskAboutFullMoon)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS,
     MSGNO(MsgGarohItsGrimLuckThatBrings)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS,
     MSGNO(MsgGarohSuchGorgeousMoonlightIfOnly)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS,
     MSGNO(MsgGarohWhichOneOfUsWas)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS,
     MSGNO(MsgGarohForeignersOnlyComeHereEn)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS,
     MSGNO(MsgGarohTheNightOfTheFull)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0,
     CONDITION_ALWAYS, EVENT(Garoh_RevealCaveEntrance)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0,
     CONDITION_ALWAYS, EVENT(Garoh_HideCaveEntrance)},
    {EXAMINE_TRIGGER(EXAMINE_WELL), 200, CONDITION_ALWAYS,
     FIND_MESSAGE(MSGNO(MsgGarohMehISawTheSilhouette))},
    {TRIGGER_TABLE_END},
};

struct Trigger gGaroh_VillageDayTriggers[] = {
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 1, CONDITION_ALWAYS,
     EVENT(Garoh_OpenDoorAndEnter)},
    {TRIGGER_EXIT | TRIGGER_FLAG_FACING | FACING_NORTH, 2, CONDITION_ALWAYS, 2},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 3, CONDITION_ALWAYS,
     EVENT(Garoh_OpenDoorAndEnter)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 4, CONDITION_ALWAYS,
     EVENT(Garoh_OpenDoorAndEnter)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 5, CONDITION_ALWAYS,
     EVENT(Garoh_OpenDoorAndEnter)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 6, CONDITION_ALWAYS,
     EVENT(Garoh_OpenDoorAndEnter)},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT | TRIGGER_FLAG_FACING | FACING_NORTH, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {STEP_TRIGGER_FACING(FACING_NORTH), 12, CONDITION_ALWAYS,
     EVENT(Garoh_EnterOpening)},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgGarohIHearTellFromMaster)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgGaroh16)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, EVENT(Garoh_HowlOutLoud)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS, EVENT(Garoh_AskAboutSecret)},
    {TRIGGER_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgGarohIHeardThereWereOther)},
    {TRIGGER_TALK, 13, CONDITION_ALWAYS, EVENT(Garoh_LittleWerewolfThanks)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS,
     MSGNO(MsgGarohHowCouldTheseOutsidersHave)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS,
     MSGNO(MsgGarohIThoughtWedNeverBe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS,
     MSGNO(MsgGarohINeverThoughtIdBe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS,
     MSGNO(MsgGarohMahaSaidTheseTravelersWere)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS,
     MSGNO(MsgGarohIWouldLoveToMeet)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13 | TRIGGER_SUBJECT_OWN_EFFECT,
     FLAG_LITTLE_WEREWOLF_THANKED, EVENT(Garoh_LittleWerewolfThanks)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS,
     MSGNO(MsgGarohMyDadToldMeI)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0,
     CONDITION_ALWAYS, EVENT(Garoh_RevealCaveEntrance)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0,
     CONDITION_ALWAYS, EVENT(Garoh_HideCaveEntrance)},
    {EXAMINE_TRIGGER(EXAMINE_WELL), 200, CONDITION_ALWAYS,
     FIND_MESSAGE(MSGNO(MsgGarohMehISawTheSilhouette))},
    {TRIGGER_TABLE_END},
};

struct Trigger gGaroh_HypnosSwordChestTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_STEP, 30, FLAG_GAROH_SAW_WEREWOLF, EVENT(Garoh_WerewolfRunsPast)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0,
     CONDITION_ALWAYS, EVENT(Garoh_IsNight)},
    {TRIGGER_TABLE_END},
};

// No end row: the room reads on into the tunnels' table, whose exits and
// Reveal answer here too.
struct Trigger gGaroh_BlocksTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_STEP, 20, FLAG_CROSSED_WEST_EDGE, EVENT(Garoh_CrossWestEdge)},
    {TRIGGER_STEP, 21, FLAG_CROSSED_EAST_EDGE, EVENT(Garoh_CrossEastEdge)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST),
     ACTOR_FIRST_PILLAR, FLAG_FIRST_PILLAR_RAISED, EVENT(Garoh_LiftPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_PILLAR,
     FLAG_FIRST_PILLAR_RAISED, EVENT(Garoh_PlaceFirstPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST),
     ACTOR_SECOND_PILLAR, FLAG_SECOND_PILLAR_RAISED, EVENT(Garoh_LiftPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SECOND_PILLAR,
     FLAG_SECOND_PILLAR_RAISED, EVENT(Garoh_PlaceSecondPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST),
     ACTOR_THIRD_PILLAR, FLAG_GAROH_ROPE_LASHED, EVENT(Garoh_LiftPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_THIRD_PILLAR,
     FLAG_GAROH_ROPE_LASHED, EVENT(Garoh_PlaceThirdPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST),
     ACTOR_FOURTH_PILLAR, FLAG_FOURTH_PILLAR_RAISED, EVENT(Garoh_LiftPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FOURTH_PILLAR,
     FLAG_FOURTH_PILLAR_RAISED, EVENT(Garoh_PlaceFourthPillar)},
    {TRIGGER_PUSH, 0, FLAG_FIRST_PILLAR_RAISED, EVENT(Garoh_LiftPushedPillar)},
    {TRIGGER_PUSHED, 0, FLAG_FIRST_PILLAR_RAISED,
     EVENT(Garoh_PlacePushedPillar)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0,
     CONDITION_ALWAYS, EVENT(Garoh_RestoreBlocksDark)},
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
    {TRIGGER_STEP, 30, FLAG_GAROH_FOLLOWED_WEREWOLF,
     EVENT(Garoh_FollowWerewolfIntoCave)},
    {TRIGGER_STEP, 31, CONDITION_ALWAYS, EVENT(Garoh_MeetInTheCave)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgGarohTakeCareInYourTravels)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS,
     MSGNO(MsgGarohIWishThatIWere)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0,
     CONDITION_ALWAYS, EVENT(Garoh_RevealCavePassage)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0,
     CONDITION_ALWAYS, EVENT(Garoh_HideCavePassage)},
    {TRIGGER_TABLE_END},
};

s32 gGaroh_HowlFrames = 0;

u32 gGaroh_QuestionMarkScript[] = {
    SC_RELEASE,
};
#undef Actor_AnimateThenWait

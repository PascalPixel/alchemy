/// @file
/// The Anemos Inner Sanctum's hall under Contigo, which the party reaches by
/// Teleport from the circle in the middle of the town, and in the same map
/// the Sanctum's last room (SceneId_AnemosInnerSanctumEntrance).
///
/// In the last room six grey stone blocks, each with a glowing symbol on
/// top, stand around a dark square. Felix pushes them or moves them with
/// Move until all six stand on the square; their symbols light up, and Hover
/// cast standing on them floats the party north across the gap to the
/// shadow who keeps the light. He challenges them, and the battle with the
/// Dullahan begins; once he is beaten, his light rises in his place.
#include "../field/actor_set_sprite_priority.c"
#include "field/actor_script.h"
#include "fixed_math.h"
#include "field/event_commands.h"
#include "field/field_sequence.h"

#include "../field/actor_create_silhouette.c"
#include "flags.h"
#include "types.h"
#include "system/ram_buffer.h"
#include "field/actor.h"
#include "system/main_routine_state.h"
#include "field/scene_tables.h"
#include "scene_ids.h"
#include "graphics/palette_buffers.h"

/// Set once the Dullahan, the shadow keeping the light, has been beaten; he is
/// gone.
#define FLAG_DULLAHAN_BEATEN 0xa9f

/// Set once the six blocks stand on the dark square: their symbols are lit,
/// for the rest of the visit.
#define FLAG_SYMBOLS_LIT FLAG_SCENE_TEMP_00
/// Set as the battle with the Dullahan starts, for the light to rise when the
/// party returns.
#define FLAG_DULLAHAN_FOUGHT FLAG_SCENE_TEMP_01

/// The last room's cell behaviours: its floor, where walking into a block
/// pushes it, and the cells where Hover, cast once the six blocks stand in
/// their places, floats the party across the gap.
enum {
  CELL_BEHAVIOR_ANEMOS_INNER_SANCTUM_ENTRANCE_BLOCK_FLOOR = 33,
  CELL_BEHAVIOR_ANEMOS_INNER_SANCTUM_ENTRANCE_HOVER = 77
};

/// A glow's value with this bit drawn mirrored left to right; its low three
/// bits are its frame.
#define LIGHT_SPRITE_MIRRORED (1 << 7)

/// Where a glow joins the depth list: its actor's y + z plus this, where the
/// actor's own sprite goes at + 62.
enum { GLOW_DEPTH = 58 };

void AnemosInnerSanctumEntrance_PlaceBlocks(void);
void AnemosInnerSanctumEntrance_SetLightPicture(u16 picture);
void Actor_PlaceOnActor(s32 id, s32 target);
void Field_TryJumpForward(void);
#include "field/field.h"
#include "field/event_inline.h"
#include "field/event.h"
#include "field/field_sequence_event.h"
#include "window/item_icon.h"
#include "scene/scene.h"
#include "field/scene_call.h"
#include "battle/battle.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "system/io_reg.h"
#include "version.h"
#include "field/map_grid.h"
#include "field/field_event.h"
#include "graphics/vram_tab.h"
#include "system/frame.h"
#include "system/iwram_call.h"
#include "system/task.h"
#include "macros.h"
#include "shared/tables.h"

#include "shared/scene_create_imported_actor.h"

#include "../field/field_effect_move.c"

/// The hall's and the last room's actors
/// (gAnemosInnerSanctumEntrance_Placements).
enum AnemosInnerSanctumEntranceActorId {
  /// "I am the shadow, the keeper of light."
  ACTOR_SHADOW = ACTOR_FIRST_PLACED,
  /// The six grey stone blocks, each with a glowing symbol on top, that Move
  /// sets in place before the way opens.
  ///@{
  ACTOR_SYMBOL_BLOCK_A,
  ACTOR_SYMBOL_BLOCK_B,
  ACTOR_SYMBOL_BLOCK_C,
  ACTOR_SYMBOL_BLOCK_D,
  ACTOR_SYMBOL_BLOCK_E,
  ACTOR_SYMBOL_BLOCK_F,
  ///@}
  /// The light the shadow keeps, a blue orb: once the Dullahan is beaten it
  /// flashes white, swells over him and rises.
  ACTOR_LIGHT,
  /// The fourteen coiled dragon statues, drawn without shadows by their
  /// script.
  ACTOR_FIRST_DRAGON_STATUE
};

#define EFFECT_SET_MODE Actor_SelectAnimation
#define EFFECT_SET_SCRIPT Actor_StartScript
#define EFFECT_SPRITE_PRIORITY
#include "../field/field_effect_spawn.c"

extern s32 gAnemosInnerSanctumEntrance_LightGrowsScript[];
extern s32 gAnemosInnerSanctumEntrance_LightShrinksScript[];
extern s32 gAnemosInnerSanctumEntrance_ShadowScript[];
extern s8 gAnemosInnerSanctumEntrance_BlockOrder[];
void AnemosInnerSanctumEntrance_StartLightSprites(void);
void AnemosInnerSanctumEntrance_SetLightPriority(u16 priority);
void AnemosInnerSanctumEntrance_OpenTheWay(void);

extern s32 gAnemosInnerSanctumEntrance_DragonStatueScript[];

struct Actor;

s16 gLightSpriteTile BSS_DATA;
s16 gLightSpriteCount BSS_DATA;
/// The actors that carry a glow, gLightSpriteCount of them.
struct Actor *gLightSpriteActors[16] BSS_DATA;
/// The glows' sprite attributes, built each frame by LightSprites_Draw.
struct LightOam gLightSpriteOams[16] BSS_DATA;
s16 gLightSpritesLarge BSS_DATA;
s16 gLightSpritePriority BSS_DATA;
s16 gAnemosInnerSanctumEntrance_LightPicture BSS_DATA;

#include "text/msg_ids.h"

/// The call gAnemosInnerSanctumEntrance_DragonStatueScript makes: the actor's
/// shadow is no longer drawn.
s32 AnemosInnerSanctumEntrance_RemoveShadow(struct Actor *actor) {
  Actor_SetSpriteFlags(actor, 0);
  return 0;
}

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

extern LATE_DATA struct ActorPlacement
    gAnemosInnerSanctumEntrance_EmptyPlacements[];
extern struct ActorPlacement gAnemosInnerSanctumEntrance_Placements[];

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  if (gMainRoutineState.next == (s32)&SceneId_AnemosInnerSanctumEntrance)
    return gAnemosInnerSanctumEntrance_Placements;
  return gAnemosInnerSanctumEntrance_EmptyPlacements;
}

/// As the blocks set the party down before him, the shadow at the door
/// speaks, the room's colours warm, and the battle with the Dullahan starts,
/// one the party cannot run from.
void AnemosInnerSanctumEntrance_ShadowChallenges(void) {
  Event_Start();
  Event_EndLastingPsynergy(0);
  Actor_StartRepeatedMotion(ACTOR_SHADOW, 2);
  Event_Wait(20);
  PaletteFade_SetSource(PALETTE_TINT_NONE, PALETTES_ALL);
  PaletteFade_SetTarget(PALETTE_TINT_WARM, PALETTES_BG);
  PaletteFade_StartInterpolation(60);
  Event_Wait(60);
  // "I am the shadow, the keeper of light. If you want the sun's power, show
  // me your own."
  FieldMessage_ShowCentered((s32)&MsgIAmTheShadow, 0, 6);
  gMainRoutineState.pendingDjinni = IF_FLAG_SET(FLAG_DULLAHAN_BEATEN);
  Battle_SetType(BATTLE_TYPE_NO_ESCAPE);
  Field_RequestBattleFromEncounterTable(ENCOUNTER_ZONE_HIDDEN_BOSSES,
                                        HIDDEN_BOSS_ANEMOS);
  GameFlag_Set(FLAG_DULLAHAN_FOUGHT);
}

/// Back from beating the Dullahan, the shadow and his light flash white, and
/// the light swells and rises over him while he shrinks away.
void AnemosInnerSanctumEntrance_LightRises(void) {
  struct Actor *actor = Actor_GetById(ACTOR_LIGHT);

  actor->motionFlags = 0;
  actor->scaleX = FIXED_ONE / 4;
  actor->scaleY = FIXED_ONE / 4;
  Event_Start();
  Event_EndLastingPsynergy(0);
  Sound_Play(SE_LIGHT_APPEAR);
  Actor_SetSpriteColor(Actor_GetById(ACTOR_SHADOW), SPRITE_COLOR_WHITE);
  Actor_SetSpriteColor(Actor_GetById(ACTOR_LIGHT), SPRITE_COLOR_WHITE);
  Event_Wait(20);
  Sound_Play(SE_WARP);
  Actor_PlaceOnActor(ACTOR_LIGHT, ACTOR_SHADOW);
  wait(1);
  Actor_SetMovementTypeById(ACTOR_SHADOW,
                            gAnemosInnerSanctumEntrance_ShadowScript);
  Actor_SetMovementTypeAndWaitById(
      ACTOR_LIGHT, gAnemosInnerSanctumEntrance_LightGrowsScript);
  Actor_SetMovementTypeAndWaitById(
      ACTOR_LIGHT, gAnemosInnerSanctumEntrance_LightShrinksScript);
  Event_Wait(10);
  Event_End();
}

/// Nothing visible: leaves the six blocks standing where they are, no longer
/// pushed.
void AnemosInnerSanctumEntrance_PlaceBlocks(void) {
  s32 i;

  for (i = ACTOR_SYMBOL_BLOCK_A; i <= ACTOR_SYMBOL_BLOCK_F; i++) {
    struct Actor *actor = Actor_GetById(i);

    actor->sceneCounter = 1;
    actor->update = 0;
  }
}

/// A floating block's shadow shows on the ground while the ground is near
/// under it, and not over the gap.
void AnemosInnerSanctumEntrance_ShadowByHeight(struct Actor *actor) {
  if ((actor->groundHeight = Map_GetTerrainHeight(
           0, actor->x.fixed, actor->z.fixed)) >= -toFIXED(32))
    Actor_SetSpriteFlags(actor, SPRITE_FLAG_SHADOW);
  else
    Actor_SetSpriteFlags(actor, 0);
}

extern s8 gAnemosInnerSanctumEntrance_BlockSpots[];

/// Felix moves one of the six symbol blocks in the last room, with Move or by
/// pushing it. Once all six stand together on the dark square in the middle
/// of the room (cells (11, 27) to (13, 28)), their symbols light up and
/// flicker with a rising sound, and the blocks stay put for the rest of the
/// visit.
///
/// Each block has its own spot, in gAnemosInnerSanctumEntrance_BlockSpots.
void AnemosInnerSanctumEntrance_MoveBlock(s32 unused, s32 block) {
  s32 i;

  const s8 *x = gAnemosInnerSanctumEntrance_BlockSpots;
  const s8 *z = gAnemosInnerSanctumEntrance_BlockSpots + 1;

  for (i = ACTOR_SYMBOL_BLOCK_A; i <= ACTOR_SYMBOL_BLOCK_F; i++) {
    struct Actor *actor = Actor_GetById(i);

    if (FIELD_CELL(actor->x.fixed) != *x)
      return;
    {
      s32 cellZ = *z;

      z += 2;
      x += 2;
      if (FIELD_CELL(actor->z.fixed) != cellZ)
        return;
    }
  }
  GameFlag_Set(FLAG_SYMBOLS_LIT);
  Sound_Play(SE_ASCEND);
  AnemosInnerSanctumEntrance_SetLightPicture(1);
  AnemosInnerSanctumEntrance_PlaceBlocks();
}

/// Felix walks into one of the six symbol blocks on the last room's floor and
/// pushes it a cell ahead, as AnemosInnerSanctumEntrance_MoveBlock. With no
/// block to push, he jumps two cells ahead where the ground allows
/// (Field_TryJumpForward), but only while he stands less than 28 pixels below
/// the room's ground: never on the sunken floor itself, which lies 32 pixels
/// down (cells (7, 25) to (17, 30)); only up on the blocks, which he walks on
/// as platforms, or on the step at cell (12, 25) and the landing at cell
/// (12, 9) north of the gap, each 16 pixels down.
void AnemosInnerSanctumEntrance_StepWhilePushing(s32 behavior) {
  struct MainRoutineState *party;
  s32 held = Leader_CanPushObjectAhead();

  party = &gMainRoutineState;
  if (held) {
    if (Field_FindNearbyTalkTarget(party->leaderActorId) != -1) {
      Leader_PushObjectAhead();
      AnemosInnerSanctumEntrance_MoveBlock(0, behavior);
    }
  } else {
    if (Actor_GetById(party->leaderActorId)->y.fixed > -toFIXED(28))
      Field_TryJumpForward();
  }
}

/// Felix casts Hover standing on the six blocks once their symbols are lit:
/// one after another they hop up, then float the party 19 cells north across
/// the gap (from cell (12, 28) to (12, 9)) and settle with a thump. If the
/// Dullahan is not yet beaten, the shadow then challenges the party
/// (AnemosInnerSanctumEntrance_ShadowChallenges). Cast again on the far side,
/// it floats them back south.
void AnemosInnerSanctumEntrance_OpenTheWay(void) {
  struct Actor *actor = Actor_GetById(gMainRoutineState.leaderActorId);
  s32 shift = toFIXED(304);
  s32 i;

  if (FIELD_CELL(actor->z.fixed) <= 9)
    shift = -shift;
  Event_EndLastingPsynergy(0);
  if (!GameFlag_Get(FLAG_SYMBOLS_LIT))
    return;
  if (MapGrid_GetCellBehaviorAt(actor->layer, actor->x.fixed, actor->z.fixed) !=
      CELL_BEHAVIOR_ANEMOS_INNER_SANCTUM_ENTRANCE_HOVER)
    return;
  Actor_SetAnimationById(gMainRoutineState.leaderActorId, ANIM_STAND);
#if TLA_PATCH_1
  FieldActor_SetMoveTargetFX(
      actor, (actor->x.fixed & FIELD_CELL_MASK) + toFIXED(8), actor->y.fixed,
      (actor->z.fixed & FIELD_CELL_MASK) + toFIXED(8));
#endif
  {
    s32 id = gMainRoutineState.leaderActorId;

    if (shift > 0)
      Actor_FaceDirectionById(id, FACING_NORTH, 0);
    else
      Actor_FaceDirectionById(id, FACING_SOUTH, 0);
  }
  wait(10);
  i = 0;
  // BUG: the loop runs seven times, reading one byte past the six blocks of
  // gAnemosInnerSanctumEntrance_BlockOrder; BUGFIX stops at the sixth.
  do {
    actor = Actor_GetById(gAnemosInnerSanctumEntrance_BlockOrder[i]);
    actor->motionFlags = 0;
    actor->y.fixed += toFIXED(1) / 2;
    actor->acceleration = toFIXED(1) / 16;
    actor->speedLimit = toFIXED(2);
    Actor_SetSpriteFlags(actor, SPRITE_FLAG_SHADOW);
    Actor_SetMoveTarget(actor, actor->x.fixed, -toFIXED(8), actor->z.fixed);
    actor->update = AnemosInnerSanctumEntrance_ShadowByHeight;
    Sound_Play(SE_JUMP);
    wait((Random_Below(4)) + 3);
#ifdef BUGFIX
  } while (++i < 6);
#else
  } while (++i <= 6);
#endif
  wait(40);
  Sound_Play(SE_KOLIMA_BRIDGE_POST);
  for (i = ACTOR_SYMBOL_BLOCK_A; i <= ACTOR_SYMBOL_BLOCK_F; i++) {
    actor = Actor_GetById(i);
    {
      FIXED z = actor->z.fixed - shift;

      Actor_SetMoveTarget(actor, actor->x.fixed, -toFIXED(8), z);
    }
  }
  Actor_WaitForMove(Actor_GetById(ACTOR_SYMBOL_BLOCK_A));
  wait(40);
  for (i = ACTOR_SYMBOL_BLOCK_A; i <= ACTOR_SYMBOL_BLOCK_F; i++) {
    actor = Actor_GetById(i);
    actor->z.fixed = (actor->z.fixed & FIELD_CELL_MASK) + toFIXED(8);
    actor->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
    actor->update = 0;
  }
  wait(3);
  Sound_Play(SE_THUMP);
  wait(10);
  if (!GameFlag_Get(FLAG_DULLAHAN_BEATEN) && shift > 0)
    AnemosInnerSanctumEntrance_ShadowChallenges();
}

/// Unused: does nothing.
void AnemosInnerSanctumEntrance_DoNothing(void) {}

extern struct Trigger gAnemosInnerSanctumEntrance_EmptyTriggers[];
extern struct Trigger gAnemosInnerSanctumEntrance_Triggers[];

const struct Trigger *Scene_GetTriggers(void) {
  if (gMainRoutineState.next == (s32)&SceneId_AnemosInnerSanctumEntrance)
    return gAnemosInnerSanctumEntrance_Triggers;
  return gAnemosInnerSanctumEntrance_EmptyTriggers;
}

/// Whether the game flag is set.
static __inline__ s32 Flag_Test(s32 flag) { return GameFlag_Get(flag); }

/// Opens with the window transition. In the last room, gives the six
/// blocks their symbols, lets the leader stand on them, and once they are
/// placed this visit leaves them placed and flickering. Back from the
/// Dullahan's battle, the light rises.
s32 Scene_Initialize(void) {
  gFieldState->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  if (gMainRoutineState.next == (s32)&SceneId_AnemosInnerSanctumEntrance) {
    AnemosInnerSanctumEntrance_StartLightSprites();
    AnemosInnerSanctumEntrance_SetLightPriority(2);
    // LightSprites_Add, defined below, is called with no prototype in scope.
    LightSprites_Add(ACTOR_SYMBOL_BLOCK_A, 0);
    LightSprites_Add(ACTOR_SYMBOL_BLOCK_B, 1);
    LightSprites_Add(ACTOR_SYMBOL_BLOCK_C, 2);
    LightSprites_Add(ACTOR_SYMBOL_BLOCK_D, 3);
    LightSprites_Add(ACTOR_SYMBOL_BLOCK_E, LIGHT_SPRITE_MIRRORED | 0);
    LightSprites_Add(ACTOR_SYMBOL_BLOCK_F, LIGHT_SPRITE_MIRRORED | 1);
    Actor_GetById(gMainRoutineState.leaderActorId)->priorityFlags |=
        ACTOR_PRIORITY_WALKS_ON_PLATFORMS;
    Actor_GetById(ACTOR_SYMBOL_BLOCK_A)->priorityFlags |=
        ACTOR_PRIORITY_WALKS_ON_PLATFORMS | ACTOR_PRIORITY_PLATFORM;
    Actor_GetById(ACTOR_SYMBOL_BLOCK_B)->priorityFlags |=
        ACTOR_PRIORITY_PLATFORM;
    Actor_GetById(ACTOR_SYMBOL_BLOCK_C)->priorityFlags |=
        ACTOR_PRIORITY_PLATFORM;
    Actor_GetById(ACTOR_SYMBOL_BLOCK_D)->priorityFlags |=
        ACTOR_PRIORITY_PLATFORM;
    Actor_GetById(ACTOR_SYMBOL_BLOCK_E)->priorityFlags |=
        ACTOR_PRIORITY_PLATFORM;
    Actor_GetById(ACTOR_SYMBOL_BLOCK_F)->priorityFlags |=
        ACTOR_PRIORITY_PLATFORM;
    if (GameFlag_Get(FLAG_SYMBOLS_LIT)) {
      AnemosInnerSanctumEntrance_SetLightPicture(1);
      AnemosInnerSanctumEntrance_PlaceBlocks();
    }
  }
  if (Flag_Test(FLAG_DULLAHAN_FOUGHT)) {
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    AnemosInnerSanctumEntrance_LightRises();
    GameFlag_Clear(FLAG_DULLAHAN_FOUGHT);
  }
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

/// Draws each listed actor's glow: a small 16-pixel glow on its frame, or, in
/// the large mode, a 32 by 64 one.
void LightSprites_Draw(void) {
  struct MapState *map = Ram_HeapSlots->mapGrid;
  s32 *view = &map->viewX;
  s32 viewX, viewZ, picture, tile, viewY;
  struct LightOam *oam;
  s32 i;

  viewX = view[0] & FIELD_PIXEL_MASK;
  viewZ = view[1] & FIELD_PIXEL_MASK;
  tile = gVramCacheEntries[gLightSpriteTile].offset >> 5;
  viewY = map->origin[1];
  oam = gLightSpriteOams;
  picture = gAnemosInnerSanctumEntrance_LightPicture *
            ((gFrameCount >> 2) % 5 * 32 + 32);

  for (i = 0; i < gLightSpriteCount; i++) {
    struct Actor *actor = gLightSpriteActors[i];

    if (actor != 0 && actor->x.fixed != 0) {
      s32 screenX = actor->x.fixed - viewX;
      FIXED y = actor->y.fixed - viewY + toFIXED(8);
      FIXED z = actor->z.fixed - viewZ - viewY;
      s32 screenY = z - y;
      s32 depth = FIXEDtoINT(y + z) + GLOW_DEPTH;
      s32 mode = gLightSpritesLarge;
      u32 *word;

      if (mode == 0) {
        s32 frame = actor->timer;
        s32 pixelX = FIXEDtoINT(screenX);
        s32 pixelY = FIXEDtoINT(screenY);
        screenX = pixelX - 8;
        screenY = pixelY - 16;
        if ((u32)(pixelX + 7) > 334 || screenY <= -16 || screenY > 239)
          continue;
        screenX &= OBJ_X_MASK;
        screenY &= OBJ_Y_MASK;
        word = (u32 *)oam;
        *word++ = 0;
        *word++ = screenY | OBJ_X(screenX) |
                  (frame & LIGHT_SPRITE_MIRRORED) *
                      (OBJ_HFLIP / LIGHT_SPRITE_MIRRORED) |
                  OBJ_SIZE_16X16 | OBJ_256_COLORS;
        *word = (tile + picture + (frame & 7) * 8) | OBJ_PRIORITY(2);
        if (gLightSpritePriority == -1)
          oam->priority = Actor_GetSpritePriority(actor);
        else
          oam->priority = gLightSpritePriority;
        Oam_AddSpriteAtDepthClamped(oam, depth);
        oam++;
      } else {
        s32 frame = actor->timer;
        s32 pixelX = FIXEDtoINT(screenX);
        s32 pixelY = FIXEDtoINT(screenY);
        screenX = pixelX - 8;
        screenY = pixelY - 64;
        if ((u32)(pixelX + 23) > 350 || screenY <= -64 || screenY > 175)
          continue;
        screenX &= OBJ_X_MASK;
        screenY &= OBJ_Y_MASK;
        word = (u32 *)oam;
        *word++ = 0;
        *word++ = screenY | OBJ_X(screenX) | OBJ_SIZE_32X64 | OBJ_256_COLORS;
        *word = (tile + frame * 8) | OBJ_PRIORITY(2);
        if (gLightSpritePriority == -1)
          oam->priority = Actor_GetSpritePriority(actor);
        else
          oam->priority = gLightSpritePriority;
        Oam_AddSpriteAtDepthClamped(oam, depth);
        oam++;
      }
    }
  }
}

/// Takes `size` bytes from the heap for a picture's decompression.
static inline void *BumpAlloc(s32 size) { return Heap_Alloc(size); }

#include "shared/light_sprites_init_a.h"

/// Sets up the glow sprites from the field's cached tiles D, with no picture
/// row chosen yet.
void AnemosInnerSanctumEntrance_StartLightSprites(void) {
  u8 *buffer = BumpAlloc(6144);

  Iwram_ClearWordsEntry(gLightSpriteActors, sizeof(gLightSpriteActors));
  Decompress_TaggedLz(gField_CachedTilesD, buffer);
  gLightSpriteTile = VramCache_FindFreeEntry();
  VramCache_Load(gLightSpriteTile, 6144, buffer);
  Heap_Release(buffer);
  Task_Add(LightSprites_Draw, TASK_PASS_FRAME);
  gLightSpriteCount = 0;
  gLightSpritesLarge = 0;
  gLightSpritePriority = -1;
  gAnemosInnerSanctumEntrance_LightPicture = 0;
}

/// Gives actor `id` a glow: `glow` is its frame, with LIGHT_SPRITE_MIRRORED to
/// draw it mirrored.
void LightSprites_Add(s32 id, s32 glow) {
  struct Actor *actor = Actor_GetById(id);

  if (actor != NULL) {
    actor->timer = glow;
    gLightSpriteActors[gLightSpriteCount] = actor;
    gLightSpriteCount++;
  }
}

/// Sets the glow sprites' priority; -1 takes each actor's own.
void AnemosInnerSanctumEntrance_SetLightPriority(u16 priority) {
  gLightSpritePriority = priority;
}

/// Chooses the glows' picture: 0 the still glow, 1 the five-frame flicker.
void AnemosInnerSanctumEntrance_SetLightPicture(u16 picture) {
  gAnemosInnerSanctumEntrance_LightPicture = picture;
}

// clang-format off
#include "shared/field_effect_duration_scripts.c"

/// The light's rise: it grows and climbs for 40 frames.
s32 gAnemosInnerSanctumEntrance_LightGrowsScript[] = {
    SC_ADD(SW_SCALE_X, FIXED_ONE / 64),
    SC_ADD(SW_SCALE_Y, FIXED_ONE / 64),
    SC_ADD(SW_Y, -toFIXED(1) / 4),
    SC_WAIT(1),
    SC_LOOP(40, SC_TOP),
    SC_END,
};

/// The light shrinking back as it sinks, then gone.
s32 gAnemosInnerSanctumEntrance_LightShrinksScript[] = {
    SC_ADD(SW_SCALE_X, -FIXED_ONE / 32),
    SC_ADD(SW_SCALE_Y, -FIXED_ONE / 32),
    SC_ADD(SW_Y, toFIXED(1) / 4),
    SC_WAIT(1),
    SC_LOOP(24, SC_TOP),
    SC_POSITION_ORIGIN,
    SC_WAIT(1),
    SC_END,
};

/// The shadow shrinking away for 40 frames, then gone.
s32 gAnemosInnerSanctumEntrance_ShadowScript[] = {
    SC_ADD(SW_SCALE_X, -FIXED_ONE / 64),
    SC_ADD(SW_SCALE_Y, -FIXED_ONE / 64),
    SC_WAIT(1),
    SC_LOOP(40, SC_TOP),
    SC_POSITION_ORIGIN,
    SC_WAIT(1),
    SC_END,
};
// clang-format on

/// The cells, x then z, where the six blocks must stand before Hover opens the
/// way: the dark square in the middle of the room.
s8 gAnemosInnerSanctumEntrance_BlockSpots[] = {11, 27, 11, 28, 12, 27,
                                               12, 28, 13, 27, 13, 28};

/// The order the blocks hop up in when the way opens.
s8 gAnemosInnerSanctumEntrance_BlockOrder[] = {
    ACTOR_SYMBOL_BLOCK_D, ACTOR_SYMBOL_BLOCK_F, ACTOR_SYMBOL_BLOCK_A,
    ACTOR_SYMBOL_BLOCK_C, ACTOR_SYMBOL_BLOCK_E, ACTOR_SYMBOL_BLOCK_B,
};

#undef EFFECT_SET_MODE
#undef EFFECT_SET_SCRIPT
#undef EFFECT_SPRITE_PRIORITY

LATE_DATA const s32 *gFieldEffect_DurationScripts[] = {
    gFieldEffect_ShortScript,
    gFieldEffect_MediumScript,
    gFieldEffect_LongScript,
};

/// The coiled dragon statues' script, from ACTOR_FIRST_DRAGON_STATUE on: hides
/// each one's sprite, then stops.
LATE_DATA s32 gAnemosInnerSanctumEntrance_DragonStatueScript[] = {
    SC_CALL(AnemosInnerSanctumEntrance_RemoveShadow),
    SC_END,
};

#define SCENE_ENTRANCES_SECTION LATE_DATA
#include "shared/scene_entrances_default.c"

LATE_DATA u32 gSceneExits[] = {
    SCENE_EXITS(AnemosInnerSanctumEntrance),
    SCENE_EXIT_UNLESS(
        ANEMOS_INNER_SANCTUM_ENTRANCE_TO_CONTIGO_GEOGLYPH_NORTHEAST,
        ContigoGeoglyph,
        CONTIGO_GEOGLYPH_FROM_ANEMOS_INNER_SANCTUM_ENTRANCE_NORTHEAST,
        EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(
        ANEMOS_INNER_SANCTUM_ENTRANCE_TO_ANEMOS_INNER_SANCTUM_DRAGON_SKIN_CHEST,
        AnemosInnerSanctumDragonSkinChest,
        ANEMOS_INNER_SANCTUM_DRAGON_SKIN_CHEST_FROM_ANEMOS_INNER_SANCTUM_ENTRANCE,
        EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(ANEMOS_INNER_SANCTUM_ENTRANCE_TO_CONTIGO_GEOGLYPH_NORTH,
                      ContigoGeoglyph,
                      CONTIGO_GEOGLYPH_FROM_ANEMOS_INNER_SANCTUM_ENTRANCE_NORTH,
                      EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(ANEMOS_INNER_SANCTUM_ENTRANCE_TO_CONTIGO_GEOGLYPH_SOUTH,
                      ContigoGeoglyph,
                      CONTIGO_GEOGLYPH_FROM_ANEMOS_INNER_SANCTUM_ENTRANCE_SOUTH,
                      EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(
        ANEMOS_INNER_SANCTUM_ENTRANCE_TO_ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST,
        AnemosInnerSanctumOrihalconChest,
        ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST_FROM_ANEMOS_INNER_SANCTUM_ENTRANCE,
        EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

/// The scene's other room's actors: none.
LATE_DATA struct ActorPlacement gAnemosInnerSanctumEntrance_EmptyPlacements[] =
    {
        {SCENE_TABLE_END},
};

/// The hall's and the last room's actors, in
/// AnemosInnerSanctumEntranceActorId's order.
LATE_DATA struct ActorPlacement gAnemosInnerSanctumEntrance_Placements[] = {
    ACTOR_PLACEMENT(SPRITE_DULLAHAN, FLAG_DULLAHAN_BEATEN, MOVEMENT_TYPE_STAND,
                    200, 0, 104, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_SYMBOL_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
                    232, 0, 424, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_SYMBOL_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
                    264, 0, 456, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 1),
    ACTOR_PLACEMENT(SPRITE_SYMBOL_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
                    168, 0, 472, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 1),
    ACTOR_PLACEMENT(SPRITE_SYMBOL_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
                    168, 0, 424, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 1),
    ACTOR_PLACEMENT(SPRITE_SYMBOL_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
                    136, 0, 456, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 1),
    ACTOR_PLACEMENT(SPRITE_SYMBOL_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
                    232, 0, 472, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 1),
    ACTOR_PLACEMENT(SPRITE_BLUE_ORB, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH,
                    TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_COILED_DRAGON_STATUE, CONDITION_ALWAYS,
                    (s32)gAnemosInnerSanctumEntrance_DragonStatueScript, 152, 0,
                    168, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_COILED_DRAGON_STATUE, CONDITION_ALWAYS,
                    (s32)gAnemosInnerSanctumEntrance_DragonStatueScript, 152, 0,
                    216, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 1),
    ACTOR_PLACEMENT(SPRITE_COILED_DRAGON_STATUE, CONDITION_ALWAYS,
                    (s32)gAnemosInnerSanctumEntrance_DragonStatueScript, 248, 0,
                    168, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 1),
    ACTOR_PLACEMENT(SPRITE_COILED_DRAGON_STATUE, CONDITION_ALWAYS,
                    (s32)gAnemosInnerSanctumEntrance_DragonStatueScript, 248, 0,
                    216, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 1),
    ACTOR_PLACEMENT(SPRITE_COILED_DRAGON_STATUE, CONDITION_ALWAYS,
                    (s32)gAnemosInnerSanctumEntrance_DragonStatueScript, 696, 0,
                    240, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_COILED_DRAGON_STATUE, CONDITION_ALWAYS,
                    (s32)gAnemosInnerSanctumEntrance_DragonStatueScript, 744, 0,
                    240, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_COILED_DRAGON_STATUE, CONDITION_ALWAYS,
                    (s32)gAnemosInnerSanctumEntrance_DragonStatueScript, 424, 0,
                    232, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_COILED_DRAGON_STATUE, CONDITION_ALWAYS,
                    (s32)gAnemosInnerSanctumEntrance_DragonStatueScript, 424, 0,
                    288, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_COILED_DRAGON_STATUE, CONDITION_ALWAYS,
                    (s32)gAnemosInnerSanctumEntrance_DragonStatueScript, 504, 0,
                    232, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_COILED_DRAGON_STATUE, CONDITION_ALWAYS,
                    (s32)gAnemosInnerSanctumEntrance_DragonStatueScript, 504, 0,
                    288, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_COILED_DRAGON_STATUE, CONDITION_ALWAYS,
                    (s32)gAnemosInnerSanctumEntrance_DragonStatueScript, 424, 0,
                    712, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_COILED_DRAGON_STATUE, CONDITION_ALWAYS,
                    (s32)gAnemosInnerSanctumEntrance_DragonStatueScript, 424, 0,
                    768, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_COILED_DRAGON_STATUE, CONDITION_ALWAYS,
                    (s32)gAnemosInnerSanctumEntrance_DragonStatueScript, 504, 0,
                    712, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_COILED_DRAGON_STATUE, CONDITION_ALWAYS,
                    (s32)gAnemosInnerSanctumEntrance_DragonStatueScript, 504, 0,
                    768, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    {SCENE_TABLE_END},
};

/// The scene's other room's triggers: none.
LATE_DATA struct Trigger gAnemosInnerSanctumEntrance_EmptyTriggers[] = {
    {TRIGGER_TABLE_END},
};

/// The scene's exits, the last room's floor for pushing, Move on each block and
/// Hover on the blocks' cells.
LATE_DATA struct Trigger gAnemosInnerSanctumEntrance_Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS,
     ANEMOS_INNER_SANCTUM_ENTRANCE_TO_CONTIGO_GEOGLYPH_NORTHEAST},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS,
     ANEMOS_INNER_SANCTUM_ENTRANCE_TO_ANEMOS_INNER_SANCTUM_DRAGON_SKIN_CHEST},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS,
     ANEMOS_INNER_SANCTUM_ENTRANCE_TO_CONTIGO_GEOGLYPH_NORTH},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS,
     ANEMOS_INNER_SANCTUM_ENTRANCE_TO_CONTIGO_GEOGLYPH_SOUTH},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS,
     ANEMOS_INNER_SANCTUM_ENTRANCE_TO_ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST},
    {STEP_TRIGGER_WALKING,
     CELL_BEHAVIOR_ANEMOS_INNER_SANCTUM_ENTRANCE_BLOCK_FLOOR, CONDITION_ALWAYS,
     EVENT(AnemosInnerSanctumEntrance_StepWhilePushing)},
    {STEP_TRIGGER_WALKING, CELL_BEHAVIOR_ANEMOS_INNER_SANCTUM_ENTRANCE_HOVER,
     CONDITION_ALWAYS, EVENT(AnemosInnerSanctumEntrance_StepWhilePushing)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SYMBOL_BLOCK_A, FLAG_SYMBOLS_LIT,
     EVENT(AnemosInnerSanctumEntrance_MoveBlock)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SYMBOL_BLOCK_B, FLAG_SYMBOLS_LIT,
     EVENT(AnemosInnerSanctumEntrance_MoveBlock)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SYMBOL_BLOCK_C, FLAG_SYMBOLS_LIT,
     EVENT(AnemosInnerSanctumEntrance_MoveBlock)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SYMBOL_BLOCK_D, FLAG_SYMBOLS_LIT,
     EVENT(AnemosInnerSanctumEntrance_MoveBlock)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SYMBOL_BLOCK_E, FLAG_SYMBOLS_LIT,
     EVENT(AnemosInnerSanctumEntrance_MoveBlock)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SYMBOL_BLOCK_F, FLAG_SYMBOLS_LIT,
     EVENT(AnemosInnerSanctumEntrance_MoveBlock)},
    {PSYNERGY_ANYWHERE(ABILITY_HOVER, PSYNERGY_PHASE_AFTER_CAST),
     CELL_BEHAVIOR_ANEMOS_INNER_SANCTUM_ENTRANCE_HOVER,
     IF_FLAG_SET(FLAG_SYMBOLS_LIT),
     EVENT(AnemosInnerSanctumEntrance_OpenTheWay)},
    {TRIGGER_TABLE_END},
};

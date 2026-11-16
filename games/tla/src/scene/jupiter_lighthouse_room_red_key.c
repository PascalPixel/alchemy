/// @file
/// The hall of Jupiter Lighthouse where the lighthouse is woken, between room 4
/// below and rooms 6 and M above. In its center a voice asks the holder of the
/// star to show the power of Anemos (JupiterLighthouse_HearTheVoice), and Hover
/// cast on the raised platform there activates the lighthouse
/// (JupiterLighthouse_ActivateTheLighthouse): its grey blocks rise to hover in
/// every room and a beam of light stands in the hall. Later, on the way to the
/// fight by the aerie, Alex stops the party here and heals it
/// (JupiterLighthouse_AlexHealsTheParty).
#include "flags.h"
#include "battle/battle.h"
#include "field/field_sequence.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/palette_buffers.h"
#include "graphics/graphics.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "types.h"
#include "field/scene_tables.h"
#include "field/field_event.h"
#include "field/event_inline.h"
#include "field/actor.h"
#include "system/main_routine_work.h"
#include "system/ram_buffer.h"
#include "macros.h"
#include "system/task.h"
#include "system/frame.h"
#include "field/field_sequence_event.h"
#include "window/item_icon.h"
#include "field/event_commands.h"
#include "field/cell_target.h"
#include "scene/scene.h"
#include "shared/jupiter_lighthouse_aerie.h"
#include "system/iwram_call.h"
#include "system/dma.h"
#include "system/io_reg.h"
#include "field/field_sprite.h"
#include "field/effect_sprite.h"
#include "field/call.h"
#include "system/field_effect.h"
#include "field/field_coordinate.h"
#include "gamepak/far_call/system.h"
#include "graphics/resource.h"
#include "field/map_grid.h"
#include "graphics/vram_tab.h"
#include "system/heap_release.h"
#include "system/io_write.h"
#include "shared/tables.h"
#include "field/field_effect.h"
#include "field/actor_script.h"
#include "fixed_math.h"
#include "scene/cycle_pictures.h"

/// Set while the leader stands in the center of the hall, so the voice speaks
/// once each time he steps in.
#define FLAG_VOICE_HEARD 0xa0e
/// The grey block Moved to the gap at cell (13, 34) has risen to hover.
#define FLAG_MOVED_BLOCK_RAISED 0xa09
/// Set as Alex stops the party here; he stays in the hall until Isaac and Ivan
/// have fallen.
#define FLAG_ALEX_WAITING 0xa22

enum JupiterLighthouseRoomRedKeyActorId {
  /// The three grey blocks at the west end, the middle one Moved into the gap
  /// between the others to rise with them.
  ACTOR_WEST_GREY_BLOCK = ACTOR_FIRST_PLACED + 2,
  ACTOR_MOVED_GREY_BLOCK,
  ACTOR_EAST_GREY_BLOCK,
  /// The row of four grey blocks the activation raises before the party's eyes.
  ACTOR_FIRST_ROW_BLOCK,
  ACTOR_LAST_ROW_BLOCK = ACTOR_FIRST_ROW_BLOCK + 3,
  /// The three blocks pushed or Moved about the floor.
  ACTOR_FIRST_PUSHED_BLOCK,
  ACTOR_LAST_PUSHED_BLOCK = ACTOR_FIRST_PUSHED_BLOCK + 2,
  ACTOR_ALEX = ACTOR_LAST_PUSHED_BLOCK + 3,
  /// The two halves of the beam of light the activation raises.
  ACTOR_LOWER_BEAM,
  ACTOR_UPPER_BEAM
};
#include "text/msg_ids.h"

struct Actor;
struct CellTarget;
struct CellTargetEntry;

enum {
  EFFECT_DEFAULT_TYPE = 285,
  EFFECT_SCALE_ONE = 0x10000,
  EFFECT_OPTION_MASK = 0xffff0000,
  EFFECT_SCRIPT_REPEAT_COUNT = 3
};

extern u32 gOrbScript[];

void JupiterLighthouse_StartSparkRing(const s32 *from, const s32 *to);

void JupiterLighthouse_LoadPalettes(s32 value);

void JupiterLighthouse_UpdateSparkRing(void);

void Actor_PlaceBlocks(void *data);

void JupiterLighthouse_InitSparkSprite(struct EffectSprite *sprite, s32 pivotX,
                                       s32 pivotY, u32 attributes, s32 tile);

s32 gWhirlwindRiding BSS_DATA;
u16 gPaletteCycleColors[16] BSS_DATA;
/// Palette 13's first two colours; it is copied whole, with the unlit colours
/// after them.
u16 gPaletteCycleUnlitPalette[2] BSS_DATA;
u16 gPaletteCycleUnlitColors[14] BSS_DATA;
s16 gLightSpriteTile BSS_DATA;
s16 gLightSpriteCount BSS_DATA;
struct Actor *gLightSpriteActors[16] BSS_DATA;
struct LightOam gLightSpriteOams[16] BSS_DATA;
s16 gLightSpritesLarge BSS_DATA;
s16 gLightSpritePriority BSS_DATA;
struct CellTarget gCrackedTiles BSS_DATA;

void Leader_CenterInCell(void);

void CrackedTile_Crack(struct CellTarget *table, struct CellTargetEntry *entry);

void JupiterLighthouse_HearTheVoice(void);

void JupiterLighthouse_LeaveTheCenter(void);

void JupiterLighthouse_AlexHealsTheParty(void);

extern u32 gFieldEffect_ShortScript[];
extern u32 gFieldEffect_MediumScript[];
extern u32 gFieldEffect_LongScript[];

struct FieldEffect;

void Actor_PlaceMovedBlock(void);
void MapGrid_SetAttributeBAt();
void Import_ActorSelectAnimation(struct FieldEffect *effect, s32 mode);
void JupiterLighthouse_DrawSparkSprite(struct EffectSprite *sprite);

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  return gActorPlacements;
}

extern s16 gJupiterLighthouse_Room5Blocks[];

/// As Move lifts one of the hall's blocks: lifts it off its cells.
void JupiterLighthouse_LiftPushedBlock(void) {
  Actor_LiftTargetBlock(gJupiterLighthouse_Room5Blocks);
}

void Actor_RaiseFromCell(s32 id);

/// As a block is set down: the middle grey block, once the lighthouse is
/// activated and it sits in its gap at cell (13, 34), rises 32 pixels and
/// hovers for good; any other block is set on its cells.
void JupiterLighthouse_PlacePushedBlock(s32 unused, s32 id) {
  struct Actor *object;
  s32 i;

  object = Actor_Get(id);
  if (id == ACTOR_MOVED_GREY_BLOCK) {
    if (!Event_GetFlag(FLAG_JUPITER_LIGHTHOUSE_ACTIVATED) ||
        FIELD_CELL(object->x.fixed) != 13 || FIELD_CELL(object->z.fixed) != 34)
      return;
    object->motionFlags = 0;
    for (i = 15; i >= 0; i--) {
      object->y.fixed += toFIXED(2);
      System_Wait(1);
    }
    Actor_RaiseFromCell(id);
    Event_SetFlag(FLAG_MOVED_BLOCK_RAISED);
  } else {
    Actor_PlaceMovedBlock();
  }
}

void Actor_SetSpriteColorMode();

#include "shared/actor_cycle_pictures.h"

/// An actor's update while the lighthouse wakes: every other frame its sprite
/// steps through six colour modes.
void JupiterLighthouse_ShimmerColors(struct Actor *object) {
  u32 frameCount = gFrameCount;

  if (frameCount & 1)
    Actor_SetSpriteColorMode(object, (frameCount >> 1) % 6);
}

/// A rising mote's update: it circles the beam of light.
void Orb_Update(struct Actor *self) {
  struct Actor *centre = Actor_Get(ACTOR_LOWER_BEAM);
  u16 angle = self->timer;
  FIXED sine;
  FIXED x;
  FIXED z;

  self->x.fixed = centre->x.fixed + Trig_Cos(angle) * (self->speedLimit + 3);
  sine = Math_Sin(angle);
  z = centre->z.fixed;
  x = self->x.fixed;
  z += sine * 2;
  self->z.fixed = z;
  self->targetX = x;
  self->targetZ = z;
  self->timer += -0x800;
}

void Actor_StartScript(struct Actor *object, const void *script);

/// Starts a mote circling the beam of light at a random height, every third
/// frame; every frame while FLAG_SCENE_TEMP_01 is set, which nothing in the
/// hall sets.
void Orbs_SpawnRising(void) {
  struct Actor *center;
  struct Actor *effect;
  struct FieldSprite *sprite;
  s32 angle;

  if (Event_GetFlag(FLAG_SCENE_TEMP_01) == 0 && gFrameCount % 3 != 0)
    return;
  center = Actor_Get(ACTOR_LOWER_BEAM);
  if (Event_GetFlag(FLAG_SCENE_TEMP_01) != 0) {
    u32 random;

    random = Random16();
    effect = FieldActor_Create(SPRITE_PARTICLE, center->x.fixed,
                               (s32)((u32)(random << 8) >> 16 << 16) +
                                   center->y.fixed - toFIXED(28),
                               center->z.fixed);
  } else {
    u32 random;

    random = Random16();
    effect = FieldActor_Create(SPRITE_PARTICLE, center->x.fixed,
                               (s32)((u32)(random << 6) >> 16 << 16) +
                                   center->y.fixed - toFIXED(28),
                               center->z.fixed);
  }
  if (effect != 0) {
    sprite = effect->sprite;
    Actor_StartScript(effect, gOrbScript);
    FieldActor_SetSpriteColor(effect, 1);
    effect->motionFlags = 0;
    angle = Random16() & 0xffff000;
    effect->timer = angle;
    effect->userValue = 0;
    effect->update = (void (*)(void))Orb_Update;
    effect->speedLimit = Math_Sin((u32)(Random16() * 0xffff) >> 20) * 24 >> 16;
    sprite->flags = 0;
    sprite->priority = 1;
  }
}

extern s32 gJupiterLighthouse_PulseStep;
extern s32 gJupiterLighthouse_PulseOn;

void JupiterLighthouse_AddSparks();

/// A frame task while the spark ring pulses: adds the step's sparks, with the
/// rising sound while the pulse is on.
void JupiterLighthouse_PulseAscend(void) {
  if (gJupiterLighthouse_PulseOn != 0)
    Event_PlaySound(SE_ASCEND);
  JupiterLighthouse_AddSparks(gJupiterLighthouse_PulseStep);
}

extern u32 gJupiterLighthouse_BringInScript[];
extern s32 gJupiterLighthouse_PulseScript[];

void JupiterLighthouse_EndSparkRing(void);

/// Shows one half of the beam of light, drawn underfoot at priority 2 in colour
/// 7, flat and upside down at first, and starts the script that brings it in.
static __inline__ void JupiterLighthouse_BringInBeam(s32 id) {
  struct Actor *object;

  Actor_SetSpritePriority(id, 2);
  Actor_SetSpriteColor(Actor_GetById(id), 7);
  object = Actor_GetById(id);
  object->motionFlags = 0;
  object->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
  object->scaleY = -FIXED_ONE;
  object->scaleX = FIXED_ONE / 10;
  Actor_SetMovementTypeById(id, gJupiterLighthouse_BringInScript);
}

/// Hover cast on the raised platform at cell (42, 41) wakes the lighthouse:
/// the leader faces north and shimmers, a ring of sparks pulses faster and
/// faster toward the hall's far end as the tower shakes, the beam of light
/// rises there with motes circling it, the hall's floor changes and its lights
/// cycle, and the row of four grey blocks rises to hover.
void JupiterLighthouse_ActivateTheLighthouse(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  struct Actor *a = Actor_GetById(ACTOR_FIRST_ROW_BLOCK);
  struct Actor *b = Actor_GetById(ACTOR_FIRST_ROW_BLOCK + 1);
  struct Actor *c = Actor_GetById(ACTOR_FIRST_ROW_BLOCK + 2);
  struct Actor *d = Actor_GetById(ACTOR_LAST_ROW_BLOCK);
  FIXED from[3];
  FIXED to[3];
  s32 i;

  if (FIELD_CELL(leader->x.fixed) != 42 || FIELD_CELL(leader->z.fixed) != 41)
    return;
  Event_SetFlag(FLAG_JUPITER_LIGHTHOUSE_ACTIVATED);
  FieldEvent_ResolvePendingActions(0);
  Actor_FaceDirectionAndWait(gMainRoutineWork.leaderActorId, FACING_NORTH);
  FieldEvent_Wait(30);
  leader->update = JupiterLighthouse_ShimmerColors;
  FieldEvent_Wait(30);
  Event_PlaySound(SE_SHIMMER);
  Actor_SetAnimationSpeed(leader, 64);
  FieldEvent_Wait(50);
  Event_PlaySound(SE_GATHERING_POWER);
  leader->timer = 0;
  leader->update = Actor_CyclePictures;
  FieldEvent_Wait(30);
  from[0] = leader->x.fixed;
  from[1] = leader->y.fixed + toFIXED(20);
  from[2] = leader->z.fixed;
  to[0] = toFIXED(680);
  to[1] = 0;
  to[2] = toFIXED(574);
  JupiterLighthouse_StartSparkRing(from, to);
  gJupiterLighthouse_PulseStep = 0;
  gJupiterLighthouse_PulseOn = 1;
  Task_AddFrame(JupiterLighthouse_PulseAscend, TASK_PASS_VBLANK);
  for (i = 0; i < 20; i++) {
    gJupiterLighthouse_PulseStep = 0;
    System_Wait((20 - i) / 2);
    gJupiterLighthouse_PulseStep = 1;
    System_Wait(1);
  }
  Event_PlaySound(SOUND_FADE_OUT_ACTION_SE);
  MapGrid_SetShake(1, 1, 1);
  gJupiterLighthouse_PulseStep = 1;
  FieldEvent_Wait(90);
  MapGrid_SetShake(3, 3, 1);
  gJupiterLighthouse_PulseStep = 3;
  gJupiterLighthouse_PulseOn = 0;
  FieldEvent_Wait(40);
  JupiterLighthouse_BringInBeam(ACTOR_LOWER_BEAM);
  JupiterLighthouse_BringInBeam(ACTOR_UPPER_BEAM);
  Task_AddFrame(Orbs_SpawnRising, TASK_PASS_VBLANK);
  Event_PlaySound(SE_CLATTER);
  MapGrid_SetShake(6, 6, 1);
  PaletteFade_ApplySource(PALETTE_TINT_NONE, 0);
  PaletteFade_ApplyTarget(PALETTE_TINT_BLEND | RGB(31, 31, 24), 0);
  PaletteFade_Interpolate(60);
  System_Wait(60);
  Event_PlaySound(SE_CLATTER);
  System_Wait(30);
  Event_PlaySound(SE_CLATTER);
  System_Wait(30);
  Event_PlaySound(SE_CLATTER);
  System_Wait(30);
  MapGrid_CopyCellsLayered(58, 99, 3, 3, 41, 99);
  MapGrid_CopyCellsLayered(123, 32, 1, 5, 106, 32);
  JupiterLighthouse_LoadPalettes(2);
  Event_PlaySound(SE_CLATTER);
  System_Wait(30);
  leader->timer = 3;
  System_Wait(1);
  leader->update = 0;
  Actor_SetAnimationSpeed(leader, ANIMATION_STEP_NORMAL);
  Actor_SetSpriteFlags(Actor_GetById(gMainRoutineWork.leaderActorId), 1);
  Actor_SetScript(ACTOR_LOWER_BEAM, gJupiterLighthouse_PulseScript);
  Actor_SetScript(ACTOR_UPPER_BEAM, gJupiterLighthouse_PulseScript);
  gJupiterLighthouse_PulseStep = 2;
  FieldEvent_Wait(30);
  gJupiterLighthouse_PulseStep = 1;
  FieldEvent_Wait(30);
  gJupiterLighthouse_PulseStep = 0;
  FieldActor_WaitForScriptEndById(ACTOR_LOWER_BEAM);
  FieldActor_WaitForScriptEndById(ACTOR_UPPER_BEAM);
  Actor_GetById(ACTOR_LOWER_BEAM)->scaleX = 0;
  Actor_GetById(ACTOR_UPPER_BEAM)->scaleX = 0;
  PaletteFade_ApplyTarget(PALETTE_TINT_NONE, 0);
  PaletteFade_Interpolate(30);
  MapGrid_SetShake(2, 2, 1);
  FieldEvent_Wait(30);
  MapGrid_SetShake(0, 0, 1);
  FieldTask_Remove(JupiterLighthouse_PulseAscend);
  JupiterLighthouse_EndSparkRing();
  FieldEvent_Wait(30);
  a->motionFlags = 0;
  b->motionFlags = 0;
  c->motionFlags = 0;
  d->motionFlags = 0;
  for (i = 0; i < 16; i++) {
    a->y.fixed += toFIXED(2);
    b->y.fixed += toFIXED(2);
    c->y.fixed += toFIXED(2);
    d->y.fixed += toFIXED(2);
    System_Wait(1);
  }
  Actor_RaiseFromCell(ACTOR_FIRST_ROW_BLOCK);
  Actor_RaiseFromCell(ACTOR_FIRST_ROW_BLOCK + 1);
  Actor_RaiseFromCell(ACTOR_FIRST_ROW_BLOCK + 2);
  Actor_RaiseFromCell(ACTOR_LAST_ROW_BLOCK);
  Event_PlaySound(MUS_PUZZLE_SOLVED);
  Sound_WaitForJingle();
}

extern s32 gPaletteCyclePausesInEvents;

void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options);

/// Stepping into the hall's centre: the screen warms and a voice asks for the
/// power of Anemos, once until the party leaves the centre.
void JupiterLighthouse_HearTheVoice(void) {
  if (!Event_GetFlag(FLAG_VOICE_HEARD)) {
    Event_SetFlag(FLAG_VOICE_HEARD);
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    PaletteFade_ApplySource(PALETTE_TINT_NONE, 0);
    PaletteFade_ApplyTarget(PALETTE_TINT_WARM, 1);
    PaletteFade_Interpolate(30);
    Event_WaitTransitionFrames();
    Actor_SetAnimation(ACTOR_ISAAC, ANIM_STAND);
    FieldEvent_Wait(30);
    FieldMessage_ShowCentered((s32)MsgFieldShowAnemosPower, 0, -2);
    PaletteFade_ApplyTarget(PALETTE_TINT_NONE, 0);
    PaletteFade_Interpolate(30);
    Event_End();
  }
}

/// Stepping out of the centre: the voice speaks again next time.
void JupiterLighthouse_LeaveTheCenter(void) {
  GameFlag_Clear(FLAG_VOICE_HEARD);
}

/// Alex appears out of a shower of rising sparks.
void JupiterLighthouse_AlexAppears(void) {
  struct EffectOptions params;
  struct EffectOptions *p;
  struct Actor *actor;
  u32 i;
  FIXED x;
  s32 y;
  u32 random;
  u32 randomY;
  u32 scaled;

  actor = Actor_Get(ACTOR_ALEX);
  Event_PlaySound(SE_WARP);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_ALEX), 0);
  p = &params;
  p->priority = 1;
  p->palette = 5;
  p->type = SPRITE_PARTICLE;
  p->startScaleX = FIXED_ONE * 2 / 5;
  p->startScaleY = FIXED_ONE * 3;
  for (i = 0; i <= 31; i++) {
    FieldEvent_Wait(1);
    if (!(i & 1)) {
      random = Random16();
      // random * 24, written in steps around the two barriers below, so x
      // loads between the multiply's add and its last shift.
      scaled = random << 1;
      scaled += random;
      x = actor->x.fixed;
      x += (((scaled << 3) >> 16) << 16);
      x += -toFIXED(12);
      randomY = Random16();
      y = actor->y.fixed;
      FieldEffect_Spawn(x, y + (((randomY << 5) >> 16) << 16) + toFIXED(32),
                        actor->z.fixed, 0, -toFIXED(4), 0,
                        EFFECT_USE_TYPE | EFFECT_USE_START_SCALE |
                            EFFECT_USE_PRIORITY | EFFECT_USE_PALETTE,
                        p);
    }
    if (i == 20)
      Actor_SetSpriteColorById(ACTOR_ALEX, 0x100);
  }
  Actor_SetSpriteColorById(ACTOR_ALEX, 0);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_ALEX), 1);
}

/// The healer raises arcs of light over the target, which spins and flashes
/// for two seconds to the revival jingle and then stands facing north.
void JupiterLighthouse_HealWithArc(s32 actor, s32 target) {
  s32 i;

  BattleEffect_StartArcEffect();
  BattleEffect_SetObjectUpdateMode(Actor_Get(actor), 2);
  Event_PlaySound(SE_ASCEND);
  System_Wait(50);
  Event_PlaySound(MUS_REVIVE);
  for (i = 0; i < 120; i++) {
    {
      struct Actor *object = Actor_Get(target);
      u32 facing = object->facing;

      object->facing = facing + 0x1000;
    }
    FieldActor_SetSpriteColor(Actor_Get(target), (i & 4) ? 7 : 0);
    System_Wait(1);
  }
  Actor_Get(target)->facing = FACING_NORTH;
  FieldActor_SetSpriteColor(Actor_Get(target), 0);
  BattleEffect_SetObjectUpdateMode(Actor_Get(actor), 0);
  BattleEffect_WaitForArcGroups();
  BattleEffect_EndArcEffect();
  Actor_SetAnimation(target, 1);
}

#define EVENT_WORK ((struct FieldWork *)Ram_HeapSlots->fieldWork)

/// On the way to the fight by the aerie, Alex appears behind Felix, asks
/// whether he will light the beacon, tells him to save his friends and heals
/// the party; from then on there are no encounters and Retreat is blocked.
void JupiterLighthouse_AlexHealsTheParty(void) {
  s32 angle;

  if (!Event_GetFlag(FLAG_JUPITER_LIGHTHOUSE_FIGHT_WATCHED))
    return;
  if (Event_GetFlag(FLAG_ISAAC_AND_IVAN_FELL))
    return;
  if (Event_GetFlag(FLAG_ALEX_WAITING))
    return;
  Event_SetFlag(FLAG_ALEX_WAITING);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId(MsgFieldWaitingForCompanion);
  // "Ah, Felix... I've been waiting for you."
  FieldEvent_ShowMessageThenWait(ACTOR_ALEX, 0, 5);
  Actor_WalkToAndWait(ACTOR_FELIX, 68, 280);
  Actor_WalkToAndWait(ACTOR_FELIX, 68, 272);
  Actor_FaceDirectionAndWait(ACTOR_FELIX, FACING_WEST);
  FieldEvent_Wait(20);
  Actor_FaceDirectionAndWait(ACTOR_FELIX, FACING_NORTH);
  FieldEvent_Wait(20);
  Actor_FaceDirectionAndWait(ACTOR_FELIX, FACING_EAST);
  FieldEvent_Wait(20);
  Actor_FaceDirectionAndWait(ACTOR_FELIX, FACING_NORTH);
  FieldEvent_Wait(10);
  Actor_SetSpriteFlags(((void *)Actor_Get(ACTOR_ALEX)), 0);
  FieldActor_SetSpriteColor(((void *)Actor_Get(ACTOR_ALEX)), 15);
  angle = FACING_SOUTH;
  Actor_SetPositionAndFacing(ACTOR_ALEX, toFIXED(64), toFIXED(225), angle);
  JupiterLighthouse_AlexAppears();
  FieldEvent_Wait(40);
  FieldCamera_MoveTo(68, CAMERA_KEEP, 240, 1);
  // "Have you come here to light the beacon?" Yes: "And you're sure that's all
  // you're going to..." No: "Hmph. They consider you their enemy, and..."
  Event_OpenMessage(ACTOR_ALEX, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(ACTOR_FELIX, 0) == 0) {
    Actor_SetAnimationAndWait(ACTOR_ALEX, ANIM_SHAKE_HEAD);
    // "And you're sure that's all you're going to do?"
    FieldEvent_ShowMessageThenWait(ACTOR_ALEX, 0, 5);
    EVENT_WORK->message++;
  } else {
    Actor_SetAnimationAndWait(ACTOR_ALEX, ANIM_NOD);
    FieldEvent_Wait(10);
    EVENT_WORK->message++;
    // "Hmph. They consider you their enemy, and yet you pity them."
    FieldEvent_ShowMessageThenWait(ACTOR_ALEX, 0, 5);
  }
  Actor_SetAnimationAndWait(ACTOR_ALEX, ANIM_NOD);
  FieldEvent_Wait(10);
  // "I saw you watching when Isaac's friends fell into Karst's trap."
  FieldEvent_ShowMessageThenWait(ACTOR_ALEX, 0, 5);
  Actor_ShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
  // "I know you, Felix. I know that if you leave Isaac behind, you'll..."
  FieldEvent_ShowMessageThenWait(ACTOR_ALEX, 0, 5);
  Actor_SetAnimationAndWait(ACTOR_ALEX, ANIM_SHAKE_HEAD);
  // "You're not like me. You can't simply discard someone who is no..."
  FieldEvent_ShowMessageThenWait(ACTOR_ALEX, 0, 5);
  // "Well, you'd better hurry if you still hope to save them."
  FieldEvent_ShowMessageThenWait(ACTOR_ALEX, 0, 5);
  Actor_SetAnimationAndWait(ACTOR_ALEX, ANIM_NOD);
  FieldEvent_Wait(30);
  FieldCamera_MoveTo(68, CAMERA_KEEP, 256, 1);
  Actor_WalkByAndWait(ACTOR_ALEX, 0, 16);
  FieldEvent_Wait(30);
  JupiterLighthouse_HealWithArc(ACTOR_ALEX, ACTOR_FELIX);
  Party_RestoreHpPp(1);
  Message_ShowCentered((s16)EVENT_WORK->message++, 5);
  FieldEvent_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_ALEX, ANIM_NOD);
  FieldEvent_Wait(30);
  FieldEvent_ShowMessageThenWait(ACTOR_ALEX, 0, 5);
  Event_SetFlag(FLAG_JUPITER_LIGHTHOUSE_ALEX_HEALED_THE_PARTY);
  Event_SetFlag(FLAG_NO_ENCOUNTERS_IN_AREA);
  Event_SetFlag(FLAG_RETREAT_BLOCKED);
  Event_End();
}

const struct Trigger *Scene_GetTriggers(void) { return gSceneEvents; }

/// Sets up the hall: Alex while he waits for the party, the beam of light
/// hidden until the lighthouse is activated, then shown with its motes.
s32 Scene_Initialize(void) {
  struct Actor *object;
  s32 angle;

  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  if (Event_GetFlag(FLAG_ISAAC_AND_IVAN_FELL))
    Event_ClearFlag(FLAG_ALEX_WAITING);
  if (Event_GetFlag(FLAG_ALEX_WAITING)) {
    angle = FACING_SOUTH;
    Actor_SetPositionAndFacing(ACTOR_ALEX, toFIXED(64), toFIXED(225), angle);
  }
  Actor_PlaceBlocks(gJupiterLighthouse_Room5Blocks);
  Actor_SetMovementType(Actor_Get(FIELD_TREASURE_ACTOR),
                        MOVEMENT_TYPE_TAKE_LEADER_LAYER);
  object = Actor_Get(ACTOR_LOWER_BEAM);
  object->scaleX = 0;
  object->motionFlags = 0;
  SET_ACTOR_POSITION(object, toFIXED(680), toFIXED(32), toFIXED(580));
  Actor_SetSpriteFlags(Actor_Get(ACTOR_LOWER_BEAM), 0);
  object = Actor_Get(ACTOR_UPPER_BEAM);
  object->scaleX = 0;
  object->motionFlags = 0;
  SET_ACTOR_POSITION(object, toFIXED(680), toFIXED(64), toFIXED(580));
  Actor_SetSpriteFlags(Actor_Get(ACTOR_UPPER_BEAM), 0);
  if (Event_GetFlag(FLAG_JUPITER_LIGHTHOUSE_ACTIVATED)) {
    MapGrid_CopyCellsLayered(58, 99, 3, 3, 41, 99);
    MapGrid_CopyCellsLayered(123, 32, 1, 5, 106, 32);
    Task_AddFrame(Orbs_SpawnRising, TASK_PASS_VBLANK);
  }
  return 0;
}

/// Raises the grey blocks once the lighthouse is activated, the Moved one
/// once it is in its gap.
s32 Scene_PrepareMap(void) {
  Scene_SetArrivalFlags(0);
  JupiterLighthouse_LoadPalettes(0);
  Actor_Get(gMainRoutineWork.leaderActorId)->priorityFlags |=
      ACTOR_PRIORITY_WALKS_ON_PLATFORMS;
  if (Event_GetFlag(FLAG_JUPITER_LIGHTHOUSE_ACTIVATED))
    MapGrid_SetCellBehavior(toFIXED(208), toFIXED(672), 2, CELL_BEHAVIOR_HOVER);
  if (Event_GetFlag(FLAG_JUPITER_LIGHTHOUSE_ACTIVATED)) {
    Actor_RaiseFromCell(ACTOR_WEST_GREY_BLOCK);
    Actor_RaiseFromCell(ACTOR_EAST_GREY_BLOCK);
    Actor_RaiseFromCell(ACTOR_FIRST_ROW_BLOCK);
    Actor_RaiseFromCell(ACTOR_FIRST_ROW_BLOCK + 1);
    Actor_RaiseFromCell(ACTOR_FIRST_ROW_BLOCK + 2);
    Actor_RaiseFromCell(ACTOR_LAST_ROW_BLOCK);
  }
  if (Event_GetFlag(FLAG_MOVED_BLOCK_RAISED)) {
    Actor_SetPosition(ACTOR_MOVED_GREY_BLOCK, 216, 552);
    Actor_RaiseFromCell(ACTOR_MOVED_GREY_BLOCK);
  }
  return 0;
}

#include "shared/scene_set_arrival_flags.c"
#undef EVENT_WORK

#include "shared/palette_cycle_set_pauses_in_events.h"

extern s32 gPaletteCycleFrame;

/// Sets the counter, its value built as a word.
static inline void PaletteCycle_SetFrame(s32 value) {
  gPaletteCycleFrame = value;
}

#include "shared/palette_cycle_step.h"

void MapGrid_SetCellHeight();

#include "shared/jupiter_lighthouse_load_palettes.h"

#include "shared/actor_raise_from_cell.h"

#include "shared/whirlwind_carry_leader.h"

#include "shared/position_flat_distance.h"

extern s32 gWhirlwindEndScript[];

s32 MapGrid_GetCellBehavior(s32 x, s32 z, s32 layer);

#include "shared/whirlwind_update.h"

/// Unused: raises a whirlwind 8 pixels ahead of the source, facing its way,
/// with a puff of dust; returns it.
UNUSED struct Actor *JupiterLighthouse_SpawnGust(struct Actor *source) {
  struct Vector3 position;
  struct EffectOptions options;
  struct Actor *object;

  Event_PlaySound(SE_WHIRLWIND);
  position.x =
      source->x.fixed + Math_MulFX(toFIXED(8), Trig_Cos(source->facing));
  position.z =
      source->z.fixed + Math_MulFX(toFIXED(8), Math_Sin(source->facing));
  object = FieldActor_Create(SPRITE_WHIRLWIND, position.x, source->y.fixed,
                             position.z);
  Import_ActorSelectAnimation(object, 2);
  Actor_SetSpriteFlags(object, 0);
  object->motionFlags = 0;
  object->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
  object->facing = source->facing;
  object->timer = 0;
  object->riseCounter = 0;
  object->riseEnabled = 0;
  object->update = Whirlwind_Update;
  object->speedLimit = toFIXED(3) / 2;
  options.priority = 1;
  options.palette = 7;
  FieldEffect_Spawn(position.x, source->y.fixed, position.z - toFIXED(6), 0, 0,
                    0, toFIXED(3), &options);
  return object;
}

/// Unused: every eighth frame, sends a spark up from a random spot about the
/// actor.
UNUSED s32 JupiterLighthouse_UpdateGust(struct Actor *object) {
  struct EffectOptions options;
  s32 phase;
  FIXED x, y, speed;

  phase = gFrameCount & 7;
  if (phase == 0) {
    options.priority = 3;
    options.startScaleX = FIXED_ONE * 7 / 10;
    options.startScaleY = FIXED_ONE * 7 / 10;
    options.palette = 14;
    x = object->x.fixed + INTtoFIXED((s32)(Random_Below(9)) - 4);
    y = object->y.fixed + INTtoFIXED(32 - (s32)(Random_Below(20)));
    speed = (INTtoFIXED((s32)(Random_Below(5))) + 0x50000) / 10;
    FieldEffect_Spawn(x, y, object->z.fixed, 0, speed, phase,
                      EFFECT_USE_PALETTE | EFFECT_USE_PRIORITY |
                          EFFECT_USE_START_SCALE,
                      &options);
  }
  return 0;
}

#include "shared/jupiter_lighthouse_place_raised_actors.h"

/// One spark of the ring: its sideways and upward swing, in pixels, and its
/// age along the way from one end to the other; it is drawn only while its age
/// runs from 0 to its duration.
struct Spark {
  u8 unused[12]; ///< never read or written
  s32 dx;
  s32 dy;
  s32 duration;
  s32 age;
};

/// The spark ring in heap slot 220: the orbs' pictures, 96 sparks and their
/// sprites, the ring's two ends and its VRAM entry.
struct SparkWork {
  u8 tiles[0x400];
  struct Spark sparks[96];
  struct EffectSprite sprites[96];
  s32 from[3];
  s32 to[3];
  s16 next;
  u16 tile;
  s16 vramEntry;
};

/// A frame task: moves each live spark along the way between the ring's ends,
/// swinging out and back on a half sine, and draws it.
void JupiterLighthouse_UpdateSparkRing(void) {
  struct SparkWork *work = Ram_HeapSlots->menuWork;
  struct Spark *spark = work->sparks;
  struct EffectSprite *sprite = work->sprites;
  s32 position[3];
  s32 duration;
  s32 i;

  for (i = 95; i >= 0; sprite++, i--, spark++) {
    duration = spark->duration;
    if (spark->age < 0 || spark->age > duration)
      continue;
    {
      FIXED sine = Math_Sin((s32)((spark->age << 15) / duration));
      s32 *p = position;
      p[0] = work->from[0] +
             (work->to[0] - work->from[0]) * spark->age / duration +
             sine * spark->dx;
      sine = Math_Sin((s32)((spark->age << 15) / duration));
      p[1] = work->from[1] +
             (work->to[1] - work->from[1]) * spark->age / duration +
             sine * spark->dy;
      p[2] =
          work->from[2] + (work->to[2] - work->from[2]) * spark->age / duration;
      Camera_WorldToScreen(p);
      sprite->x = p[0];
      sprite->y = p[2];
      JupiterLighthouse_DrawSparkSprite(sprite);
      spark->age++;
    }
  }
}

/// Adds count sparks to the spark ring's 96 entries in heap slot 220, each
/// with a random angle from 135 degrees across three quarters of a turn, the
/// cosine and negated sine of it at 64 pixels as its direction, a random
/// speed of 16 to 31 and an age of 0.
void JupiterLighthouse_AddSparks(s32 count) {
  struct SparkWork *work = Ram_HeapSlots->menuWork;
  struct Spark *spark;
  s32 angle;
  s32 i;
  FIXED radius;
  s32 speed;

  if (count > 0) {
    i = count;
    do {
      spark = &work->sparks[work->next];
      angle = ((Random16() * 3) >> 2) + DEGtoANG(135);
      radius = toFIXED(64);
      speed = ((Random16() << 4) >> 16) + 16;
      spark->dx = Iwram_MulFX(Trig_Cos(angle), radius) >> 16;
      spark->dy = -Iwram_MulFX(Math_Sin(angle), radius) >> 16;
      spark->duration = speed;
      spark->age = 0;
      work->next++;
      work->next %= 96;
    } while (--i != 0);
  }
}

/// Starts the spark ring between two points: loads the coloured orbs'
/// pictures, readies 96 sprites at the leader's priority and just in front of
/// it, and starts the update task.
void JupiterLighthouse_StartSparkRing(const s32 *from, const s32 *to) {
  struct SparkWork *work =
      HeapSlot_Alloc(HEAP_SLOT_MENU, sizeof(struct SparkWork));
  struct Actor *leader = Actor_Get(gMainRoutineWork.leaderActorId);
  struct EffectSprite *sprite;
  struct Spark *spark;
  s32 entry;
  s32 tile;
  s32 i;

  work->from[0] = from[0];
  work->from[1] = from[1];
  work->from[2] = from[2];
  work->to[0] = to[0];
  work->to[1] = to[1];
  work->to[2] = to[2];
  Decompress_TaggedLz(Resource_GetTableEntry(RESOURCE_COLOURED_ORBS), work);
  entry = VramCache_FindFreeEntry();
  tile = Vram_Load(entry, 0x400, work);
  work->tile = tile;
  work->vramEntry = entry;
  i = 0;
  spark = work->sparks;
  sprite = work->sprites;
  for (; i <= 95; i++, sprite++, spark++) {
    JupiterLighthouse_InitSparkSprite(sprite, 4, 4, 0, tile + (i & 15) * 2);
    sprite->oam.fullColor = 1;
    sprite->oam.palette = 0;
    sprite->oam.priority = BattleEffect_GetObjectPriority(leader);
    // Through a union, so this store may alias the age store after it.
    ((union {
       u16 v;
       s32 w;
     } *)&sprite->depth)
        ->v = BattleEffect_GetObjectDepth(leader) - 1;
    spark->age = -1;
  }
  {
    s32 zero = 0;

    work->next = zero;
  }
  Task_AddFrame(JupiterLighthouse_UpdateSparkRing, TASK_PASS_VBLANK);
}

/// Stops the spark ring and frees its pictures and work.
void JupiterLighthouse_EndSparkRing(void) {
  struct SparkWork *work = Ram_HeapSlots->menuWork;

  FieldTask_Remove(JupiterLighthouse_UpdateSparkRing);
  VramCache_FreeEntry(work->vramEntry);
  HeapSlot_Release(HEAP_SLOT_MENU);
}

/// Queues the lit actors' sprites: for each listed actor with a position,
/// projects it from the map's view, skips it off screen, and writes a 16 by 16
/// sprite, or a 32 by 64 one when the big switch is set, at its animation
/// picture and depth.
#include "shared/light_sprites_draw.h"

static inline void *BumpAlloc(s32 size) { return Heap_Alloc(size); }

#include "shared/light_sprites_init_a.h"

#include "shared/light_sprites_init_b.h"

#include "shared/light_sprites_init_c.h"

#include "shared/light_sprites_add.h"

void LightSprites_SetPriority(s32 value) { gLightSpritePriority = value; }

#include "../field/actor_set_sprite_priority.c"

#include "../field/field_effect_move.c"

#undef EVENT_WORK
extern const s32 *gFieldEffect_DurationScripts[];

void Actor_SetSpriteFlags(struct Actor *object, u32 value);

void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options) {
  struct Actor *party = Actor_GetById(gMainRoutineWork.leaderActorId);
  struct FieldEffect *effect;
  struct FieldSprite *sprite;
  const s32 *script;
  FIXED target;
  s32 count;

  if ((flags & EFFECT_USE_TYPE) != 0 && options != 0)
    effect = ((struct FieldEffect *)Import_ActorCreate(options->type, x, y, z));
  else
    effect = ((struct FieldEffect *)Import_ActorCreate(EFFECT_DEFAULT_TYPE, x,
                                                       y, z));
  if (effect == 0)
    return;

  sprite = effect->sprite;
  Import_ActorSelectAnimation(effect, (flags + 1) & EFFECT_SCRIPT_MASK);
  Actor_StartScript(effect,
                    gFieldEffect_DurationScripts[flags & EFFECT_SCRIPT_MASK]);
  effect->motionFlags = 0;
  Actor_SetSpriteFlags(effect, 0);
  effect->update = FieldEffect_UpdateMotionScaleAndSpin;
  effect->velocity.x = velocityX;
  effect->velocity.y = velocityY;
  effect->velocity.z = velocityZ;
  Scene_SetActorSpritePriority(effect,
                               ((struct FieldSprite *)party->sprite)->priority);
  effect->scaleRateX = 0;
  effect->scaleRateY = 0;
  effect->spin = 0;

  if ((flags & EFFECT_OPTION_MASK) == 0 || options == 0)
    return;
  if ((flags & EFFECT_USE_PALETTE) != 0)
    FieldActor_SetSpriteColor(effect, options->palette);
  if ((flags & EFFECT_USE_PRIORITY) != 0) {
    effect->priorityFlags &= ~1;
    Scene_SetActorSpritePriority(effect, options->priority);
  }
  if ((flags & EFFECT_USE_START_SCALE) != 0) {
    effect->scaleX = options->startScaleX;
    effect->scaleY = options->startScaleY;
  }
  if ((flags & EFFECT_SCALE_TO_TARGET) != 0) {
    script = gFieldEffect_DurationScripts[flags & EFFECT_SCRIPT_MASK];
    target = options->targetScaleX;
    count = script[EFFECT_SCRIPT_REPEAT_COUNT];
    if ((flags & EFFECT_USE_START_SCALE) != 0) {
      effect->scaleRateX = (target - effect->scaleX) / count;
      effect->scaleRateY = (options->targetScaleY - effect->scaleY) /
                           script[EFFECT_SCRIPT_REPEAT_COUNT];
    } else {
      effect->scaleRateX = (target - EFFECT_SCALE_ONE) / count;
      effect->scaleRateY = (options->targetScaleY - EFFECT_SCALE_ONE) /
                           script[EFFECT_SCRIPT_REPEAT_COUNT];
    }
  }
  if ((flags & EFFECT_USE_SCRIPT) != 0) {
    Import_ActorSelectAnimation(effect, 1);
    Actor_StartScript(effect, options->script);
  }
  if ((flags & EFFECT_USE_ROTATION) != 0)
    sprite->rotation = options->rotation;
  if ((flags & EFFECT_USE_SPIN) != 0)
    effect->spin = options->spin;
  if ((flags & EFFECT_USE_UPDATE) != 0)
    effect->update = options->update;
}

/// The cells CrackedTiles_Step watches, and the cell it copies over the one the
/// leader steps on.
#define CELL_PUSH_SLOT_ENTRY Oam_AddSpriteAtDepthClamped
#include "../field/map_cell_sprite.c"

#include "shared/cracked_tiles_find.h"

#include "shared/leader_center_in_cell.h"

// Centers the leader on its cell, plays sound 161 and copies the cell below
// the table's source cell over the leader's, keeping the source column's and
// row's 64 bit; CrackedTiles_Step calls it the first time the leader steps on
// one of the table's cells.
#include "shared/cracked_tile_crack.h"

#include "shared/cracked_tile_fall.h"

#include "../field/field_effect_advance_motion.c"

#undef CELL_PUSH_SLOT_ENTRY
void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options);

#include "shared/cracked_tile_drop_in.h"

void GameFlag_SetByte(s32 flag, s32 value);

#include "shared/cracked_tiles_step.h"

#include "shared/cracked_tile_place_landing.h"

#include "text/msg_ids.h"

void JupiterLighthouse_LiftPushedBlock(void);
void JupiterLighthouse_ActivateTheLighthouse(void);

// clang-format off
u32 gFieldEffect_ShortScript[] = {                                                               
      SC_WAIT(1), SC_LOOP(22, SC_TOP),                                     
      SC_WAIT(1), SC_POSITION(0, 0, 0),                                        
      SC_WAIT(1), SC_RELEASE,                                                     
      SC_END,                                                                  
  };

u32 gFieldEffect_MediumScript[] = {                                                               
      SC_WAIT(1), SC_LOOP(44, SC_TOP),                                     
      SC_WAIT(1), SC_POSITION(0, 0, 0),                                        
      SC_WAIT(1), SC_RELEASE,                                                     
      SC_END,                                                                  
  };

u32 gFieldEffect_LongScript[] = {                                                               
      SC_WAIT(1), SC_LOOP(126, SC_TOP),                                     
      SC_WAIT(1), SC_POSITION(0, 0, 0),                                        
      SC_WAIT(1), SC_RELEASE,                                                     
      SC_END,                                                                  
  };

u32 gJupiterLighthouse_BringInScript[] = {
    SC_ADD(SW_SCALE_X, 0x1000),
    SC_WAIT(1),
    SC_LOOP(32, SC_TOP),
    SC_END,
};

s32 gJupiterLighthouse_PulseScript[] = {                                                               
      SC_ADD(SW_SCALE_X, -960),                                                 
      SC_WAIT(1),                                                              
      SC_LOOP(140, SC_TOP),                                                     
      SC_END,                                                                  
  };

s16 gJupiterLighthouse_Room5Blocks[] = {
    17, 18, 19, -1,
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {                                                        
      SCENE_EXITS(JupiterLighthouseRoomRedKey),         SCENE_EXIT(JUPITER_LIGHTHOUSE_ROOM_RED_KEY_TO_JUPITER_LIGHTHOUSE_ROOM_LOCKED_DOORS, JupiterLighthouseRoomLockedDoors, JUPITER_LIGHTHOUSE_ROOM_LOCKED_DOORS_FROM_JUPITER_LIGHTHOUSE_ROOM_RED_KEY),    SCENE_EXIT(2, JupiterLighthouseRoomRedKey, 3), 
      SCENE_EXIT(3, JupiterLighthouseRoomRedKey, 2),  SCENE_EXIT(JUPITER_LIGHTHOUSE_ROOM_RED_KEY_TO_JUPITER_LIGHTHOUSE_ROOM_M_WEST, JupiterLighthouseRoomM, JUPITER_LIGHTHOUSE_ROOM_M_FROM_JUPITER_LIGHTHOUSE_ROOM_RED_KEY_WEST),    SCENE_EXIT(JUPITER_LIGHTHOUSE_ROOM_RED_KEY_TO_JUPITER_LIGHTHOUSE_ROOM_M_EAST, JupiterLighthouseRoomM, JUPITER_LIGHTHOUSE_ROOM_M_FROM_JUPITER_LIGHTHOUSE_ROOM_RED_KEY_EAST),  
      SCENE_EXIT(7, JupiterLighthouseRoomRedKey, 8),   SCENE_EXIT(8, JupiterLighthouseRoomRedKey, 7), SCENE_EXIT(JUPITER_LIGHTHOUSE_ROOM_RED_KEY_TO_JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_WEST, JupiterLighthouseRoomCrackedFloor, JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_FROM_JUPITER_LIGHTHOUSE_ROOM_RED_KEY_WEST), 
      SCENE_EXIT(JUPITER_LIGHTHOUSE_ROOM_RED_KEY_TO_JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_EAST, JupiterLighthouseRoomCrackedFloor, JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_FROM_JUPITER_LIGHTHOUSE_ROOM_RED_KEY_EAST), SCENE_EXITS_END,                                
  };

struct ActorPlacement gActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_WHITE_WINGS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 536, 0, 608, FACING_SOUTHWEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WHITE_WINGS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 824, 0, 608, FACING_SOUTHEAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_BALANCE_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 184, 0, 552, FACING_SOUTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BALANCE_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 216, 0, 600, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_BALANCE_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 248, 0, 552, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_BALANCE_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 616, 0, 664, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BALANCE_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 648, 0, 664, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_BALANCE_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 712, 0, 664, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_BALANCE_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 744, 0, 664, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_YELLOW_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 152, 0, 280, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_YELLOW_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 264, 0, 296, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_YELLOW_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 232, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WHITE_WINGS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 72, 0, 328, FACING_SOUTHWEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WHITE_WINGS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 488, 0, 328, FACING_SOUTHEAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_ALEX, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_PIPE_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PIPE_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_PIPE_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_PIPE_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_PIPE_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    {SCENE_TABLE_END},
};

u32 gOrbScript[] = {                                                               
      SC_SET(SW_SCALE_X, 0xcccc),                                              
      SC_SET(SW_SCALE_Y, 0xcccc),                                              
      SC_ANIMATION(1),                                                               
      SC_LABEL(1),                                                             
      SC_ADD(SW_Y, toFIXED(1)),                                                 
      SC_WAIT(1),                                                              
      SC_LOOP(16, SC_TO(1)),                                                   
      SC_POSITION(0, 0, 0),                                                    
      SC_WAIT(1),                                                              
      SC_RELEASE,                                                                 
  };

s32 gJupiterLighthouse_PulseStep = 0;

s32 gJupiterLighthouse_PulseOn = 1;

struct Trigger gSceneEvents[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 12},
    {TRIGGER_STEP, 21, FLAG_JUPITER_LIGHTHOUSE_ACTIVATED, EVENT(JupiterLighthouse_HearTheVoice)},
    {TRIGGER_STEP, 22, FLAG_JUPITER_LIGHTHOUSE_ACTIVATED, EVENT(JupiterLighthouse_LeaveTheCenter)},
    {TRIGGER_STEP, 100, CONDITION_ALWAYS, EVENT(JupiterLighthouse_AlexHealsTheParty)},
    {TRIGGER_TALK, ACTOR_ALEX, CONDITION_ALWAYS, MSGNO(MsgFieldFollowTrueFeelings)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ALEX, CONDITION_ALWAYS, MSGNO(MsgFieldPartWithAgatio)},
    {TRIGGER_PUSH, -1, CONDITION_ALWAYS, EVENT(JupiterLighthouse_LiftPushedBlock)},
    {TRIGGER_PUSHED, -1, CONDITION_ALWAYS, EVENT(JupiterLighthouse_PlacePushedBlock)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_FIRST_PUSHED_BLOCK, CONDITION_ALWAYS, EVENT(JupiterLighthouse_LiftPushedBlock)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_PUSHED_BLOCK, CONDITION_ALWAYS, EVENT(JupiterLighthouse_PlacePushedBlock)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_FIRST_PUSHED_BLOCK + 1, CONDITION_ALWAYS, EVENT(JupiterLighthouse_LiftPushedBlock)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_PUSHED_BLOCK + 1, CONDITION_ALWAYS, EVENT(JupiterLighthouse_PlacePushedBlock)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_LAST_PUSHED_BLOCK, CONDITION_ALWAYS, EVENT(JupiterLighthouse_LiftPushedBlock)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_LAST_PUSHED_BLOCK, CONDITION_ALWAYS, EVENT(JupiterLighthouse_PlacePushedBlock)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_MOVED_GREY_BLOCK, CONDITION_ALWAYS, EVENT(JupiterLighthouse_LiftPushedBlock)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_MOVED_GREY_BLOCK, CONDITION_ALWAYS, EVENT(JupiterLighthouse_PlacePushedBlock)},
    {PSYNERGY_ANYWHERE(ABILITY_HOVER, PSYNERGY_PHASE_ACTS), 0, FLAG_JUPITER_LIGHTHOUSE_ACTIVATED, EVENT(JupiterLighthouse_ActivateTheLighthouse)},
    {TRIGGER_TABLE_END},
};

s32 gPaletteCycleFrame = -1;

s32 gPaletteCyclePausesInEvents = 1;

s32 gWhirlwindEndScript[] = {
    SC_ANIMATION(5),
    SC_WAIT(12),
    SC_RELEASE,
};
// clang-format on

const s32 *gFieldEffect_DurationScripts[] = {
    gFieldEffect_ShortScript,
    gFieldEffect_MediumScript,
    gFieldEffect_LongScript,
};

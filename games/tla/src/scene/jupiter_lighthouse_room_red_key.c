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
#include "field/field.h"
#include "game/game.h"
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
#include "scene/orbit.h"

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

extern s16 gLightSpriteTile;
extern s16 gLightSpriteCount;
extern struct Actor *gLightSpriteActors[];
extern s16 gLightSpritesLarge;
extern s16 gLightSpritePriority;

void Leader_CenterInCell(void);

extern struct CellTarget gCrackedTiles;

void CrackedTile_Crack(struct CellTarget *table, struct CellTargetEntry *entry);

void JupiterLighthouse_HearTheVoice(void);

void JupiterLighthouse_LeaveTheCenter(void);

void JupiterLighthouse_AlexHealsTheParty(void);

extern u32 gFieldEffect_ShortScript[];
extern u32 gFieldEffect_MediumScript[];
extern u32 gFieldEffect_LongScript[];

struct FieldEffect;

void Actor_PlaceMovedBlock(void);
extern u16 gPaletteCycleColors[];
extern u16 gPaletteCycleUnlitPalette[];
extern u16 gPaletteCycleUnlitColors[];
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

void JupiterLighthouse_LiftPushedBlock(void) {
  Actor_LiftTargetBlock(gJupiterLighthouse_Room5Blocks);
}

void Actor_RaiseFromCell(s32 id);

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
      object->y.fixed += px(2);
      System_Wait(1);
    }
    Actor_RaiseFromCell(id);
    Event_SetFlag(FLAG_MOVED_BLOCK_RAISED);
  } else {
    Actor_PlaceMovedBlock();
  }
}

u32 __umodsi3();
void Actor_SetSpriteColorMode();

#include "shared/actor_cycle_pictures.h"

void JupiterLighthouse_ShimmerColors(struct Actor *object) {
  u32 frameCount = gFrameCount;

  if (frameCount & 1)
    Actor_SetSpriteColorMode(object, (frameCount >> 1) % 6);
}

/// A rising mote's update: it circles the beam of light.
void Orb_Update(struct Actor *self) {
  struct Actor *centre = Actor_Get(ACTOR_LOWER_BEAM);
  u16 angle = self->timer;
  s32 sine;
  s32 x;
  s32 z;

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

void Actor_StartScript(struct OrbitEffect *object, const void *script);

/// Starts a mote circling the beam of light at a random height, every third
/// frame; every frame while FLAG_SCENE_TEMP_01 is set, which nothing in the
/// hall sets.
void Orbs_SpawnRising(void) {
  struct Actor *center;
  struct OrbitEffect *effect;
  struct FieldSprite *sprite;
  s32 angle;

  if (Event_GetFlag(FLAG_SCENE_TEMP_01) == 0 && gFrameCount % 3 != 0)
    return;
  center = Actor_Get(ACTOR_LOWER_BEAM);
  if (Event_GetFlag(FLAG_SCENE_TEMP_01) != 0) {
    u32 random;

    random = Random16();
    effect = (struct OrbitEffect *)FieldActor_Create(
        0x2a0, center->x.fixed,
        (s32)((u32)(random << 8) >> 16 << 16) + center->y.fixed - px(28),
        center->z.fixed);
  } else {
    u32 random;

    random = Random16();
    effect = (struct OrbitEffect *)FieldActor_Create(
        0x2a0, center->x.fixed,
        (s32)((u32)(random << 6) >> 16 << 16) + center->y.fixed - px(28),
        center->z.fixed);
  }
  if (effect != 0) {
    sprite = effect->sprite;
    Actor_StartScript(effect, gOrbScript);
    FieldActor_SetSpriteColor(effect, 1);
    effect->motionFlags = 0;
    angle = Random16() & 0xffff000;
    effect->angle = angle;
    effect->unk66 = 0;
    effect->update = (void (*)(struct OrbitEffect *))Orb_Update;
    effect->radius = Math_Sin((u32)(Random16() * 0xffff) >> 20) * 24 >> 16;
    sprite->nextFrame = 0;
    sprite->priority = 1;
  }
}

extern s32 gJupiterLighthouse_PulseStep;
extern s32 gJupiterLighthouse_PulseOn;

void JupiterLighthouse_AddSparks();

void JupiterLighthouse_PulseAscend(void) {
  if (gJupiterLighthouse_PulseOn != 0)
    Event_PlaySound(SE_ASCEND);
  JupiterLighthouse_AddSparks(gJupiterLighthouse_PulseStep);
}

extern u32 gJupiterLighthouse_BringInScript[];
extern s32 gJupiterLighthouse_PulseScript[];

void JupiterLighthouse_EndSparkRing(void);

static __inline__ void Bring(s32 id) {
  struct Actor *object;

  Actor_SetSpritePriority(id, 2);
  Actor_SetSpriteColor(Actor_GetById(id), 7);
  object = Actor_GetById(id);
  object->motionFlags = 0;
  object->priorityFlags |= 2;
  object->scaleY = 0xffff0000;
  object->scaleX = Q16_ONE / 10;
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
  s32 from[3];
  s32 to[3];
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
  Event_PlaySound(SE_178);
  leader->timer = 0;
  leader->update = Actor_CyclePictures;
  FieldEvent_Wait(30);
  from[0] = leader->x.fixed;
  from[1] = leader->y.fixed + px(20);
  from[2] = leader->z.fixed;
  to[0] = px(680);
  to[1] = 0;
  to[2] = px(574);
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
  Bring(ACTOR_LOWER_BEAM);
  Bring(ACTOR_UPPER_BEAM);
  Task_AddFrame(Orbs_SpawnRising, TASK_PASS_VBLANK);
  Event_PlaySound(SE_CLATTER);
  MapGrid_SetShake(6, 6, 1);
  PaletteFade_ApplySource(0x10000, 0);
  PaletteFade_ApplyTarget(0x4063ff, 0);
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
  Actor_SetAnimationSpeed(leader, 16);
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
  PaletteFade_ApplyTarget(0x10000, 0);
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
    a->y.fixed += px(2);
    b->y.fixed += px(2);
    c->y.fixed += px(2);
    d->y.fixed += px(2);
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

void JupiterLighthouse_HearTheVoice(void) {
  if (!Event_GetFlag(FLAG_VOICE_HEARD)) {
    Event_SetFlag(FLAG_VOICE_HEARD);
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    PaletteFade_ApplySource(0x10000, 0);
    PaletteFade_ApplyTarget(0x10003, 1);
    PaletteFade_Interpolate(30);
    Event_WaitTransitionFrames();
    Actor_SetAnimation(ACTOR_ISAAC, ANIM_STAND);
    FieldEvent_Wait(30);
    FieldMessage_ShowCentered((s32)MsgFieldShowAnemosPower, 0, -2);
    PaletteFade_ApplyTarget(0x10000, 0);
    PaletteFade_Interpolate(30);
    Event_End();
  }
}

void JupiterLighthouse_LeaveTheCenter(void) {
  GameFlag_Clear(FLAG_VOICE_HEARD);
}

/// Alex appears out of a shower of rising sparks.
void JupiterLighthouse_AlexAppears(void) {
  struct EffectParams params;
  struct EffectParams *p;
  struct SparkActor *actor;
  u32 i;
  s32 x;
  s32 y;
  u32 random;
  u32 random2;
  u32 scaled;

  actor = ((struct SparkActor *)Actor_Get(ACTOR_ALEX));
  Event_PlaySound(SE_WARP);
  Actor_SetSpriteFlags(((struct SparkActor *)Actor_Get(ACTOR_ALEX)), 0);
  p = &params;
  p->count = 1;
  p->kind = 5;
  p->tile = 0x2a0;
  p->spread = 0x6666;
  p->rise = px(3);
  for (i = 0; i <= 31; i++) {
    FieldEvent_Wait(1);
    if (!(i & 1)) {
      random = Random16();
      // random * 24, written in steps around the two barriers below, so x
      // loads between the multiply's add and its last shift.
      scaled = random << 1;
      scaled += random;
      x = actor->x;
      x += (((scaled << 3) >> 16) << 16);
      x += -px(12);
      random2 = Random16();
      y = actor->y;
      FieldEffect_Spawn(x, y + (((random2 << 5) >> 16) << 16) + px(32),
                        actor->z, 0, -px(4), 0, px(27), p);
    }
    if (i == 20)
      Actor_SetSpriteColorById(ACTOR_ALEX, 0x100);
  }
  Actor_SetSpriteColorById(ACTOR_ALEX, 0);
  Actor_SetSpriteFlags(((struct SparkActor *)Actor_Get(ACTOR_ALEX)), 1);
}

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
  Actor_SetPositionAndFacing(ACTOR_ALEX, px(64), px(225), angle);
  JupiterLighthouse_AlexAppears();
  FieldEvent_Wait(40);
  FieldCamera_MoveTo(68, CAMERA_KEEP, 240, 1);
  // "Have you come here to light the beacon?" Yes: "And you're sure that's all
  // you're going to..." No: "Hmph. They consider you their enemy, and..."
  Event_OpenMessage(22, 0);
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
  JupiterLighthouse_HealWithArc(22, 4);
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

  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
  if (Event_GetFlag(FLAG_ISAAC_AND_IVAN_FELL))
    Event_ClearFlag(FLAG_ALEX_WAITING);
  if (Event_GetFlag(FLAG_ALEX_WAITING)) {
    angle = FACING_SOUTH;
    Actor_SetPositionAndFacing(ACTOR_ALEX, px(64), px(225), angle);
  }
  Actor_PlaceBlocks(gJupiterLighthouse_Room5Blocks);
  Actor_SetMovementType(Actor_Get(FIELD_TREASURE_ACTOR), 9);
  object = Actor_Get(ACTOR_LOWER_BEAM);
  object->scaleX = 0;
  object->motionFlags = 0;
  SET_ACTOR_POSITION(object, px(680), px(32), px(580));
  Actor_SetSpriteFlags(Actor_Get(ACTOR_LOWER_BEAM), 0);
  object = Actor_Get(ACTOR_UPPER_BEAM);
  object->scaleX = 0;
  object->motionFlags = 0;
  SET_ACTOR_POSITION(object, px(680), px(64), px(580));
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
  Actor_Get(gMainRoutineWork.leaderActorId)->priorityFlags |= 0x40;
  if (Event_GetFlag(FLAG_JUPITER_LIGHTHOUSE_ACTIVATED))
    MapGrid_SetCellBehavior(0xd00000, 0x2a00000, 2, 236);
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

extern s32 gJupiterLighthouse_OrbScript[];
extern s32 gJupiterLighthouse_OrbFrame;

s32 MapGrid_GetCellBehavior(s32 x, s32 z, s32 layer);

void JupiterLighthouse_UpdateOrb(struct Actor *self) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  struct Actor *leader = Actor_Get(gMainRoutineWork.leaderActorId);
  s32 position[3];
  s32 distance;
  s32 near;

  if (work->psynergy != 0 || work->fieldMenu != 0 || work->pauseMenu != 0 ||
      work->item != 0) {
    Actor_SetAnimationSpeed(self, 0);
    return;
  }
  Actor_SetAnimationSpeed(self, 16);
  if ((++*(u16 *)&self->timer & 31) == 31)
    Event_PlaySound(SE_WATER_LEVEL);
  {
    s8 *to = self->sprite;
    s8 *from = leader->sprite;
    u32 a = from[9] & 12;
    to[9] = (s8)((to[9] & ~12) | a);
  }
  if (MapGrid_GetCellBehavior(self->x.fixed, self->z.fixed, 2) != 255) {
    position[0] = self->x.fixed;
    position[1] = self->y.fixed;
    position[2] = self->z.fixed;
    Camera_WorldToScreen(position);
    distance = position[0];
    if (distance > px(272))
      goto stop;
    if (distance < -px(32))
      goto stop;
    {
      if (*((u8 *)self + 0x62) == 0) {
        distance = leader->y.fixed - self->y.fixed;
        if (distance < 0)
          distance = self->y.fixed - leader->y.fixed;
        near = 0;
        if (Position_FlatDistance(&leader->x.fixed, &self->x.fixed) <= 12 &&
            distance < px(12))
          near = 1;
        if (near && !Event_GetFlag(FLAG_FIELD_HANDLING_REQUESTS)) {
          leader->facing = self->facing;
          leader->watchedActor = self;
          leader->priorityFlags &= ~1;
          {
            s32 delay = 200;
            work->raisedTrigger = delay;
          }
          gMainRoutineWork.movementMode = 0;
          *((u8 *)self + 0x62) = 1;
          *((u8 *)self + 0x63) = 1;
        }
      }
      {
        s32 (*multiply)(s32, s32);
        s32 cosine = Trig_Cos(self->facing);
        s32 speed = self->speedLimit;
        /* FAKEMATCH(dead): the routine address taken inside a once-only block.
         */
        do {
          multiply = Iwram_MulQ16;
        } while (0);
        self->x.fixed += multiply(cosine, speed);
      }
      self->z.fixed += Iwram_MulQ16(Math_Sin(self->facing), self->speedLimit);
      return;
    }
  }
stop:
  {
    s32 zero = 0;
    *((u8 *)self + 0x63) = zero;
    self->update = zero;
    Actor_StartScript(self, gJupiterLighthouse_OrbScript);
    Event_PlaySound(SE_SPARKLING_PASSAGE);
    gJupiterLighthouse_OrbFrame = zero;
  }
}

struct GustObject *JupiterLighthouse_SpawnGust(struct GustObject *source) {
  struct Vector3 position;
  struct EffectOptions options;
  struct GustObject *object;

  Event_PlaySound(SE_WHIRLWIND);
  position.x = source->x + Math_MulQ16(px(8), Trig_Cos(source->angle));
  position.z = source->z + Math_MulQ16(px(8), Math_Sin(source->angle));
  object = ((struct GustObject *)FieldActor_Create(SPRITE_WHIRLWIND, position.x,
                                                   source->y, position.z));
  Import_ActorSelectAnimation(object, 2);
  Actor_SetSpriteFlags(object, 0);
  object->motionFlags = 0;
  object->priorityFlags &= ~1;
  object->angle = source->angle;
  object->unk64 = 0;
  object->unk62 = 0;
  object->unk63 = 0;
  object->update = JupiterLighthouse_UpdateOrb;
  object->speedLimit = px(3) / 2;
  options.priority = 1;
  options.palette = 7;
  FieldEffect_Spawn(position.x, source->y, position.z - px(6), 0, 0, 0, px(3),
                    &options);
  return object;
}

s32 JupiterLighthouse_UpdateGust(struct Actor *object) {
  struct EffectOptions options;
  s32 phase, x, y, speed;

  phase = gFrameCount & 7;
  if (phase != 0)
    goto done;
  options.priority = 3;
  options.startScaleX = Q16_ONE * 7 / 10;
  options.startScaleY = Q16_ONE * 7 / 10;
  options.palette = 14;
  x = object->x.fixed + px((s32)(Random_Below(9)) - 4);
  y = object->y.fixed + px(32 - (s32)(Random_Below(20)));
  speed = (px((s32)(Random_Below(5))) + 0x50000) / 10;
  FieldEffect_Spawn(x, y, object->z.fixed, 0, speed, phase,
                    EFFECT_USE_PALETTE | EFFECT_USE_PRIORITY |
                        EFFECT_USE_START_SCALE,
                    &options);
done:
  return 0;
}

#include "shared/jupiter_lighthouse_place_raised_actors.h"

struct Spark {
  u8 unk00[0x0c];
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
      s32 sine = Math_Sin((s32)((spark->age << 15) / duration));
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
/// with a random angle from 0x6000 across three quarters of a turn, the
/// cosine and negated sine of it at 64 pixels as its direction, a random
/// speed of 16 to 31 and an age of 0.
void JupiterLighthouse_AddSparks(s32 count) {
  struct SparkWork *work = Ram_HeapSlots->menuWork;
  struct Spark *spark;
  s32 angle;
  s32 i;
  s32 radius;
  s32 speed;

  if (count > 0) {
    i = count;
    do {
      spark = &work->sparks[work->next];
      angle = ((Random16() * 3) >> 2) + 0x6000;
      radius = px(64);
      speed = ((Random16() << 4) >> 16) + 16;
      spark->dx = Iwram_MulQ16(Trig_Cos(angle), radius) >> 16;
      spark->dy = -Iwram_MulQ16(Math_Sin(angle), radius) >> 16;
      spark->duration = speed;
      spark->age = 0;
      work->next++;
      work->next %= 96;
    } while (--i != 0);
  }
}

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
  Decompress_TaggedLz(Resource_GetTableEntry((s32)&ResourceId_ColouredOrbs),
                      work);
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

void JupiterLighthouse_EndSparkRing(void) {
  struct SparkWork *work = Ram_HeapSlots->menuWork;

  FieldTask_Remove(JupiterLighthouse_UpdateSparkRing);
  VramCache_FreeEntry(work->vramEntry);
  HeapSlot_Release(HEAP_SLOT_MENU);
}

extern struct LightOam gLightSpriteOams[];

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
  s32 target;
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
      SCENE_EXITS(JupiterLighthouseRoomRedKey),         SCENE_EXIT(1, JupiterLighthouseRoomLockedDoors, 1),    SCENE_EXIT(2, JupiterLighthouseRoomRedKey, 3), 
      SCENE_EXIT(3, JupiterLighthouseRoomRedKey, 2),  SCENE_EXIT(5, JupiterLighthouseRoomM, 5),    SCENE_EXIT(6, JupiterLighthouseRoomM, 6),  
      SCENE_EXIT(7, JupiterLighthouseRoomRedKey, 8),   SCENE_EXIT(8, JupiterLighthouseRoomRedKey, 7), SCENE_EXIT(9, JupiterLighthouseRoomCrackedFloor, 9), 
      SCENE_EXIT(10, JupiterLighthouseRoomCrackedFloor, 10), SCENE_EXITS_END,                                
  };

struct ActorPlacement gActorPlacements[] = {
    ACTOR_PLACEMENT(341, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 536, 0, 608, FACING_SOUTHWEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(341, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 824, 0, 608, FACING_SOUTHEAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_BALANCE_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 184, 0, 552, FACING_SOUTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BALANCE_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 216, 0, 600, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_BALANCE_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 248, 0, 552, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_BALANCE_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 616, 0, 664, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BALANCE_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 648, 0, 664, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_BALANCE_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 712, 0, 664, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_BALANCE_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 744, 0, 664, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(312, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 152, 0, 280, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(312, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 264, 0, 296, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(312, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 232, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(341, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 72, 0, 328, FACING_SOUTHWEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(341, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 488, 0, 328, FACING_SOUTHEAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(57, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
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
      SC_ADD(SW_Y, px(1)),                                                 
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

s32 gJupiterLighthouse_OrbScript[] = {
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

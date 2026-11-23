/// @file
/// The hall of Jupiter Lighthouse where the lighthouse is woken, between the
/// room of locked doors below and the cracked floor's room and Room M above.
/// Below the platform in its center a voice asks the holder of the star to
/// show the power of Anemos (JupiterLighthouse_HearTheVoice), and Hover cast at
/// the foot of the ladder up to it activates the lighthouse
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
/// The grey block Moved onto the empty socket has risen to hover over it.
#define FLAG_RED_KEY_HALL_SOCKET_BLOCK_RAISED 0xa09
/// Set as Alex stops the party here; he stays in the hall until Isaac and Ivan
/// have fallen.
#define FLAG_ALEX_WAITING 0xa22

/// The hall's actors, numbering the rows of gActorPlacements.
enum JupiterLighthouseRoomRedKeyActorId {
  /// The three grey blocks over the spikes at the west end, which hover once
  /// the lighthouse is activated; the middle one is Moved onto the empty socket
  /// between the others to rise with them.
  ACTOR_WEST_HOVERING_BLOCK = ACTOR_FIRST_PLACED + 2,
  ACTOR_SOCKET_BLOCK,
  ACTOR_EAST_HOVERING_BLOCK,
  /// The four grey blocks the activation raises before the party's eyes.
  ACTOR_FIRST_RISING_BLOCK,
  ACTOR_LAST_RISING_BLOCK = ACTOR_FIRST_RISING_BLOCK + 3,
  /// The three yellow blocks pushed or Moved about the floor.
  ACTOR_FIRST_PUSHED_BLOCK,
  ACTOR_LAST_PUSHED_BLOCK = ACTOR_FIRST_PUSHED_BLOCK + 2,
  ACTOR_ALEX = ACTOR_LAST_PUSHED_BLOCK + 3,
  /// The two halves of the beam of light the activation raises.
  ACTOR_LOWER_BEAM,
  ACTOR_UPPER_BEAM
};

/// The empty socket between the west grey blocks.
#define EMPTY_SOCKET_CELL_X 13
#define EMPTY_SOCKET_CELL_Z 34
/// The cell, just north of where the voice speaks, where Hover wakes the
/// lighthouse.
#define ACTIVATION_CELL_X 42
#define ACTIVATION_CELL_Z 41
/// The times the ring of sparks pulses, each pause shorter than the last.
#define RING_PULSES 20
/// The sparks of the activation's ring, from the leader to the beam.
#define ACTIVATION_SPARKS 96
#include "text/msg_ids.h"

struct Actor;
struct CellTarget;
struct CellTargetEntry;

extern u32 gOrb_RisingScript[];

void JupiterLighthouse_StartSparkRing(const s32 *from, const s32 *to);

void JupiterLighthouse_LoadPalettes(s32 value);

void JupiterLighthouse_UpdateSparkRing(void);

void Actor_PlaceBlocks(void *data);

/// Fills an effect sprite: its OAM attributes, its tiles and its size from its
/// shape, pivoted at (pivotX, pivotY) (EffectSprite_Init, through the overlay's
/// own import row).
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
/// Queues an effect sprite for this frame at its depth, turned and scaled about
/// its pivot (EffectSprite_Draw, through the overlay's own import row).
void JupiterLighthouse_DrawSparkSprite(struct EffectSprite *sprite);

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  return gActorPlacements;
}

extern s16 gJupiterLighthouse_PushedBlocks[];

/// As Move lifts one of the hall's blocks: lifts it off its cells.
void JupiterLighthouse_LiftPushedBlock(void) {
  Actor_LiftTargetBlock(gJupiterLighthouse_PushedBlocks);
}

void Actor_RaiseFromCell(s32 id);

/// As a block is set down: the middle grey block, once the lighthouse is
/// activated and it sits on the empty socket, rises 32 pixels and hovers for
/// good; any other block is set on its cells.
void JupiterLighthouse_PlacePushedBlock(s32 unused, s32 id) {
  struct Actor *actor;
  s32 frame;

  actor = Actor_Get(id);
  if (id == ACTOR_SOCKET_BLOCK) {
    if (!Event_GetFlag(FLAG_JUPITER_LIGHTHOUSE_ACTIVATED) ||
        FIELD_CELL(actor->x.fixed) != EMPTY_SOCKET_CELL_X ||
        FIELD_CELL(actor->z.fixed) != EMPTY_SOCKET_CELL_Z)
      return;
    actor->motionFlags = 0;
    for (frame = 0; frame < 16; frame++) {
      actor->y.fixed += toFIXED(2);
      System_Wait(1);
    }
    Actor_RaiseFromCell(id);
    Event_SetFlag(FLAG_RED_KEY_HALL_SOCKET_BLOCK_RAISED);
  } else {
    Actor_PlaceMovedBlock();
  }
}

void Actor_SetSpriteColorMode();

#include "shared/actor_cycle_pictures.h"

/// An actor's update while the lighthouse wakes: every other frame its sprite
/// steps through six colour modes.
void JupiterLighthouse_FlashColors(struct Actor *actor) {
  u32 frameCount = gFrameCount;

  if (frameCount & 1)
    Actor_SetSpriteColorMode(actor, (frameCount >> 1) % 6);
}

/// A rising particle's update: it circles the beam of light.
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
  self->timer += -TRIG_TURN / 32;
}

void Actor_StartScript(struct Actor *actor, const void *script);

/// Starts a particle circling the beam of light at a random height, every
/// third frame; every frame while FLAG_SCENE_TEMP_01 is set, which nothing in
/// the hall sets.
void Orb_SpawnRising(void) {
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
                               (s32)INTtoFIXED(FIXEDtoINT((u32)(random << 8))) +
                                   center->y.fixed - toFIXED(28),
                               center->z.fixed);
  } else {
    u32 random;

    random = Random16();
    effect = FieldActor_Create(SPRITE_PARTICLE, center->x.fixed,
                               (s32)INTtoFIXED(FIXEDtoINT((u32)(random << 6))) +
                                   center->y.fixed - toFIXED(28),
                               center->z.fixed);
  }
  if (effect != 0) {
    sprite = effect->sprite;
    Actor_StartScript(effect, gOrb_RisingScript);
    FieldActor_SetSpriteColor(effect, SPRITE_COLOR_GRB);
    effect->motionFlags = 0;
    angle = Random16() & 0xffff000;
    effect->timer = angle;
    effect->userValue = 0;
    effect->update = (ObjectUpdate)Orb_Update;
    effect->speedLimit = Math_Sin((u32)(Random16() * 65535) >> 20) * 24 >> 16;
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
  struct Actor *actor;

  Actor_SetSpritePriority(id, 2);
  Actor_SetSpriteColor(Actor_GetById(id), SPRITE_COLOR_WHITE);
  actor = Actor_GetById(id);
  actor->motionFlags = 0;
  actor->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
  actor->scaleY = -FIXED_ONE;
  actor->scaleX = FIXED_ONE / 10;
  Actor_SetMovementTypeById(id, gJupiterLighthouse_BringInScript);
}

/// Hover cast at the ladder's foot wakes the lighthouse: the leader faces
/// north and flashes in shifting colours, a ring of sparks pulses faster and
/// faster toward the hall's far end as the tower shakes, the beam of light
/// rises there with particles circling it, the hall's floor changes and its
/// lights cycle, and the four grey blocks over the spikes beside the ladder
/// rise to hover.
void JupiterLighthouse_ActivateTheLighthouse(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  struct Actor *a = Actor_GetById(ACTOR_FIRST_RISING_BLOCK);
  struct Actor *b = Actor_GetById(ACTOR_FIRST_RISING_BLOCK + 1);
  struct Actor *c = Actor_GetById(ACTOR_FIRST_RISING_BLOCK + 2);
  struct Actor *d = Actor_GetById(ACTOR_LAST_RISING_BLOCK);
  FIXED from[3];
  FIXED to[3];
  s32 i;

  if (FIELD_CELL(leader->x.fixed) != ACTIVATION_CELL_X ||
      FIELD_CELL(leader->z.fixed) != ACTIVATION_CELL_Z)
    return;
  Event_SetFlag(FLAG_JUPITER_LIGHTHOUSE_ACTIVATED);
  FieldEvent_ResolvePendingActions(0);
  Actor_FaceDirectionAndWait(gMainRoutineWork.leaderActorId, FACING_NORTH);
  FieldEvent_Wait(30);
  leader->update = JupiterLighthouse_FlashColors;
  FieldEvent_Wait(30);
  Event_PlaySound(SE_SHIMMER);
  Actor_SetAnimationSpeed(leader, ANIM_STEP_NORMAL * 4);
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
  Task_AddFrame(JupiterLighthouse_PulseAscend, TASK_PASS_FRAME);
  for (i = 0; i < RING_PULSES; i++) {
    gJupiterLighthouse_PulseStep = 0;
    System_Wait((RING_PULSES - i) / 2);
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
  Task_AddFrame(Orb_SpawnRising, TASK_PASS_FRAME);
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
  Actor_SetAnimationSpeed(leader, ANIM_STEP_NORMAL);
  Actor_SetSpriteFlags(Actor_GetById(gMainRoutineWork.leaderActorId),
                       SPRITE_FLAG_SHADOW);
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
  Actor_RaiseFromCell(ACTOR_FIRST_RISING_BLOCK);
  Actor_RaiseFromCell(ACTOR_FIRST_RISING_BLOCK + 1);
  Actor_RaiseFromCell(ACTOR_FIRST_RISING_BLOCK + 2);
  Actor_RaiseFromCell(ACTOR_LAST_RISING_BLOCK);
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
    // "Holder of the star... Show the power of Anemos!"
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
  struct Actor *actor;
  u32 frame;
  FIXED x;
  s32 y;
  u32 random;
  u32 randomY;

  actor = Actor_Get(ACTOR_ALEX);
  Event_PlaySound(SE_WARP);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_ALEX), 0);
  params.priority = 1;
  params.palette = SPRITE_COLOR_BRG;
  params.type = SPRITE_PARTICLE;
  params.startScaleX = FIXED_ONE * 2 / 5;
  params.startScaleY = FIXED_ONE * 3;
  for (frame = 0; frame < 32; frame++) {
    FieldEvent_Wait(1);
    if (!(frame & 1)) {
      // A whole pixel from 12 west of Alex to 11 east, and up to 31 over the
      // 32 above his feet.
      random = Random16();
      x = actor->x.fixed + INTtoFIXED(FIXEDtoINT(random * 24)) - toFIXED(12);
      randomY = Random16();
      y = actor->y.fixed;
      FieldEffect_Spawn(x,
                        y + INTtoFIXED(FIXEDtoINT(randomY << 5)) + toFIXED(32),
                        actor->z.fixed, 0, -toFIXED(4), 0,
                        EFFECT_USE_TYPE | EFFECT_USE_START_SCALE |
                            EFFECT_USE_PRIORITY | EFFECT_USE_PALETTE,
                        &params);
    }
    if (frame == 20)
      Actor_SetSpriteColorById(ACTOR_ALEX, SPRITE_COLOR_BLINK);
  }
  Actor_SetSpriteColorById(ACTOR_ALEX, SPRITE_COLOR_NORMAL);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_ALEX), SPRITE_FLAG_SHADOW);
}

/// The healer raises arcs of light over the target, which spins and flashes
/// for two seconds to the revival jingle and then stands facing north.
void JupiterLighthouse_HealTarget(s32 actorId, s32 target) {
  s32 frame;

  BattleEffect_StartArcEffect();
  BattleEffect_SetObjectUpdateMode(Actor_Get(actorId), 2);
  Event_PlaySound(SE_ASCEND);
  System_Wait(50);
  Event_PlaySound(MUS_RECOVERING_AFTER_COMBAT);
  for (frame = 0; frame < 120; frame++) {
    {
      struct Actor *actor = Actor_Get(target);
      u32 facing = actor->facing;

      actor->facing = facing + FACING_STEP;
    }
    FieldActor_SetSpriteColor(Actor_Get(target), (frame & 4)
                                                     ? SPRITE_COLOR_WHITE
                                                     : SPRITE_COLOR_NORMAL);
    System_Wait(1);
  }
  Actor_Get(target)->facing = FACING_NORTH;
  FieldActor_SetSpriteColor(Actor_Get(target), SPRITE_COLOR_NORMAL);
  BattleEffect_SetObjectUpdateMode(Actor_Get(actorId), 0);
  BattleEffect_WaitForArcGroups();
  BattleEffect_EndArcEffect();
  Actor_SetAnimation(target, ANIM_STAND);
}

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
  FieldActor_SetSpriteColor(((void *)Actor_Get(ACTOR_ALEX)),
                            SPRITE_COLOR_HIDDEN);
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
    gFieldWork->message++;
  } else {
    Actor_SetAnimationAndWait(ACTOR_ALEX, ANIM_NOD);
    FieldEvent_Wait(10);
    gFieldWork->message++;
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
  JupiterLighthouse_HealTarget(ACTOR_ALEX, ACTOR_FELIX);
  Party_RestoreHpPp(1);
  Message_ShowCentered((s16)gFieldWork->message++, 5);
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
/// hidden until the lighthouse is activated, then shown with its particles.
s32 Scene_Initialize(void) {
  struct Actor *actor;
  s32 angle;

  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  if (Event_GetFlag(FLAG_ISAAC_AND_IVAN_FELL))
    Event_ClearFlag(FLAG_ALEX_WAITING);
  if (Event_GetFlag(FLAG_ALEX_WAITING)) {
    angle = FACING_SOUTH;
    Actor_SetPositionAndFacing(ACTOR_ALEX, toFIXED(64), toFIXED(225), angle);
  }
  Actor_PlaceBlocks(gJupiterLighthouse_PushedBlocks);
  Actor_SetMovementType(Actor_Get(FIELD_TREASURE_ACTOR),
                        MOVEMENT_TYPE_TAKE_LEADER_LAYER);
  actor = Actor_Get(ACTOR_LOWER_BEAM);
  actor->scaleX = 0;
  actor->motionFlags = 0;
  SET_ACTOR_POSITION(actor, toFIXED(680), toFIXED(32), toFIXED(580));
  Actor_SetSpriteFlags(Actor_Get(ACTOR_LOWER_BEAM), 0);
  actor = Actor_Get(ACTOR_UPPER_BEAM);
  actor->scaleX = 0;
  actor->motionFlags = 0;
  SET_ACTOR_POSITION(actor, toFIXED(680), toFIXED(64), toFIXED(580));
  Actor_SetSpriteFlags(Actor_Get(ACTOR_UPPER_BEAM), 0);
  if (Event_GetFlag(FLAG_JUPITER_LIGHTHOUSE_ACTIVATED)) {
    MapGrid_CopyCellsLayered(58, 99, 3, 3, 41, 99);
    MapGrid_CopyCellsLayered(123, 32, 1, 5, 106, 32);
    Task_AddFrame(Orb_SpawnRising, TASK_PASS_FRAME);
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
    MapGrid_SetCellBehavior(toFIXED(208), toFIXED(672), MAPGRID_LAYER_OVERHEAD,
                            CELL_BEHAVIOR_HOVER);
  if (Event_GetFlag(FLAG_JUPITER_LIGHTHOUSE_ACTIVATED)) {
    Actor_RaiseFromCell(ACTOR_WEST_HOVERING_BLOCK);
    Actor_RaiseFromCell(ACTOR_EAST_HOVERING_BLOCK);
    Actor_RaiseFromCell(ACTOR_FIRST_RISING_BLOCK);
    Actor_RaiseFromCell(ACTOR_FIRST_RISING_BLOCK + 1);
    Actor_RaiseFromCell(ACTOR_FIRST_RISING_BLOCK + 2);
    Actor_RaiseFromCell(ACTOR_LAST_RISING_BLOCK);
  }
  if (Event_GetFlag(FLAG_RED_KEY_HALL_SOCKET_BLOCK_RAISED)) {
    Actor_SetPosition(ACTOR_SOCKET_BLOCK, 216, 552);
    Actor_RaiseFromCell(ACTOR_SOCKET_BLOCK);
  }
  return 0;
}

#include "shared/scene_set_arrival_flags.c"

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

#include "shared/whirlwind_spawn.h"

// Unused: nothing in the hall sets Actor_EmitRisingSparks as an update.
#include "shared/actor_emit_rising_sparks.h"

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

/// The spark ring in its heap slot: the orbs' pictures, the sparks and their
/// sprites, the ring's two ends and its VRAM entry.
struct SparkWork {
  u8 tiles[ORB_PICTURES * TILE_SIZE_8BPP];
  struct Spark sparks[ACTIVATION_SPARKS];
  struct EffectSprite sprites[ACTIVATION_SPARKS];
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

  for (i = 0; i < ACTIVATION_SPARKS; sprite++, i++, spark++) {
    duration = spark->duration;
    if (spark->age < 0 || spark->age > duration)
      continue;
    {
      FIXED sine = Math_Sin((s32)((spark->age * (TRIG_TURN / 2)) / duration));
      position[0] = work->from[0] +
                    (work->to[0] - work->from[0]) * spark->age / duration +
                    sine * spark->dx;
      sine = Math_Sin((s32)((spark->age * (TRIG_TURN / 2)) / duration));
      position[1] = work->from[1] +
                    (work->to[1] - work->from[1]) * spark->age / duration +
                    sine * spark->dy;
      position[2] =
          work->from[2] + (work->to[2] - work->from[2]) * spark->age / duration;
      Camera_WorldToScreen(position);
      sprite->x = position[0];
      sprite->y = position[2];
      JupiterLighthouse_DrawSparkSprite(sprite);
      spark->age++;
    }
  }
}

/// Adds count sparks to the spark ring, each with a random angle from 135
/// degrees across three quarters of a turn, the cosine and negated sine of it
/// at 64 pixels as its swing, a random duration of 16 to 31 frames and an age
/// of 0.
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
      spark->dx = FIXEDtoINT(Iwram_MulFX(Trig_Cos(angle), radius));
      spark->dy = FIXEDtoINT(-Iwram_MulFX(Math_Sin(angle), radius));
      spark->duration = speed;
      spark->age = 0;
      work->next++;
      work->next %= ACTIVATION_SPARKS;
    } while (--i != 0);
  }
}

/// Starts the spark ring between two points: loads the coloured orbs'
/// pictures, readies the sparks' sprites at the leader's priority and just in
/// front of him, and starts the update task.
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
  tile = Vram_Load(entry, sizeof(work->tiles), work);
  work->tile = tile;
  work->vramEntry = entry;
  for (i = 0, spark = work->sparks, sprite = work->sprites;
       i < ACTIVATION_SPARKS; i++, sprite++, spark++) {
    JupiterLighthouse_InitSparkSprite(sprite, 4, 4, 0,
                                      tile + (i & (ORB_PICTURES - 1)) * 2);
    sprite->oam.fullColor = 1;
    sprite->oam.palette = 0;
    sprite->oam.priority = BattleEffect_GetObjectPriority(leader);
    sprite->depth = BattleEffect_GetObjectDepth(leader) - 1;
    spark->age = -1;
  }
  /* FAKEMATCH(dead): a block that only places firstSlot's declaration at the
   * store. */
  {
    s32 firstSlot = 0;

    work->next = firstSlot;
  }
  Task_AddFrame(JupiterLighthouse_UpdateSparkRing, TASK_PASS_FRAME);
}

/// Stops the spark ring and frees its pictures and work.
void JupiterLighthouse_EndSparkRing(void) {
  struct SparkWork *work = Ram_HeapSlots->menuWork;

  FieldTask_Remove(JupiterLighthouse_UpdateSparkRing);
  VramCache_FreeEntry(work->vramEntry);
  HeapSlot_Release(HEAP_SLOT_MENU);
}

#include "shared/light_sprites_draw.h"

static inline void *BumpAlloc(s32 size) { return Heap_Alloc(size); }

#include "shared/light_sprites_init_a.h"

#include "shared/light_sprites_init_b.h"

#include "shared/light_sprites_init_c.h"

#include "shared/light_sprites_add.h"

/// Unused: sets the priority the lit actors' sprites are drawn at.
UNUSED void LightSprites_SetPriority(s32 value) {
  gLightSpritePriority = value;
}

#include "../field/actor_set_sprite_priority.c"

#include "../field/field_effect_move.c"

extern const s32 *gFieldEffect_DurationScripts[];

void Actor_SetSpriteFlags(struct Actor *actor, u32 value);

#define EFFECT_SET_MODE Actor_SelectAnimation
#define EFFECT_SET_SCRIPT Actor_StartScript
#include "../field/field_effect_spawn.c"
#undef EFFECT_SET_MODE
#undef EFFECT_SET_SCRIPT

#define CELL_PUSH_SLOT_ENTRY Oam_AddSpriteAtDepthClamped
#include "../field/map_cell_sprite.c"

#include "shared/cracked_tile_find.h"

#include "shared/leader_center_in_cell.h"

#include "shared/cracked_tile_crack.h"

#include "shared/cracked_tile_fall.h"

#include "../field/field_effect_advance_motion.c"

#undef CELL_PUSH_SLOT_ENTRY
void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options);

#include "shared/cracked_tile_drop_in.h"

void GameFlag_SetByte(s32 flag, s32 value);

#include "shared/cracked_tile_step.h"

#include "shared/cracked_tile_place_landing.h"

#include "text/msg_ids.h"

void JupiterLighthouse_LiftPushedBlock(void);
void JupiterLighthouse_ActivateTheLighthouse(void);

// clang-format off
#include "shared/field_effect_duration_scripts.c"

u32 gJupiterLighthouse_BringInScript[] = {
    SC_ADD(SW_SCALE_X, FIXED_ONE / 16),
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

s16 gJupiterLighthouse_PushedBlocks[] = {
    ACTOR_FIRST_PUSHED_BLOCK, ACTOR_FIRST_PUSHED_BLOCK + 1, ACTOR_LAST_PUSHED_BLOCK, -1,
};

#include "shared/scene_entrances_default.c"

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

#include "shared/orb_rising_script.c"

/// The spark ring's pulse: its step, and whether it still rises.
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
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_SOCKET_BLOCK, CONDITION_ALWAYS, EVENT(JupiterLighthouse_LiftPushedBlock)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SOCKET_BLOCK, CONDITION_ALWAYS, EVENT(JupiterLighthouse_PlacePushedBlock)},
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

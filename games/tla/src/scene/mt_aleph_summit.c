/// @file
/// The summit of Mt. Aleph, at the end of the game: the Golden Sun forms over
/// the mountain, Alex claims its power, and the scene's colours and dust follow
/// what happens.
#include "../field/object/set_sprite_priority.c"

#include "../field/field_effect/move.c"

#include "shared/tables.h"
#include "types.h"
#include "system/main_routine_work.h"
#include "field_sprite.h"
#include "system/field_effect.h"
#include "field/event_inline.h"
#include "battle/battle.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "lib.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "scene_tables.h"
#include "objcmd.h"
#include "system/frame.h"
#include "actor.h"
#include "system/vector3.h"
#include "system/ram_buffer.h"
#include "system/io_reg.h"
#include "system/io_write_queue.h"
#include "system/iwram_call.h"
#include "system/heap_release.h"
#include "bg_scroll.h"
#include "system/input.h"
#include "call.h"
#include "map.h"
#include "field/field_sequence_event.h"
#include "module/field/event_commands.h"
#include "module/field/field_effect.h"
#include "module/scene/scene.h"
#include "module/system/system.h"

extern const s32 *gFieldEffect_DurationScripts[];

extern u32 gFieldEffect_ShortScript[];

extern u32 gFieldEffect_MediumScript[];
extern u32 gFieldEffect_LongScript[];

extern char SceneId_Epilogue;

void FieldCamera_WaitForArrival(void);

extern void Actor_SelectAnimation(struct FieldEffect *effect, s32 mode);
extern void Actor_StartScript(struct FieldEffect *effect, const s32 *script);
void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options) {
  struct FieldEffect *party =
      ((struct FieldEffect *)Actor_GetById(gMainRoutineWork.currentOwner));
  struct FieldEffect *effect;
  struct FieldSprite *sprite;
  const s32 *script;
  u32 start;

  if ((flags & EFFECT_USE_TYPE) != 0 && options != 0)
    effect = (struct FieldEffect *)Import_ActorCreate(options->type, x, y, z);
  else
    effect = (struct FieldEffect *)Import_ActorCreate(285, x, y, z);
  if (effect == 0)
    return;

  sprite = effect->sprite;
  Actor_SelectAnimation(effect, (flags + 1) & EFFECT_SCRIPT_MASK);
  Actor_StartScript(effect,
                    gFieldEffect_DurationScripts[flags & EFFECT_SCRIPT_MASK]);
  effect->motionFlags = 0;
  Actor_SetSpriteFlags(effect, 0);
  effect->update = FieldEffect_UpdateMotionScaleAndSpin;
  effect->velocity.x = velocityX;
  effect->velocity.y = velocityY;
  effect->velocity.z = velocityZ;
  Scene_SetActorSpritePriority(effect, party->sprite->priority);
  effect->scaleRateX = 0;
  effect->scaleRateY = 0;
  effect->spin = 0;

  if ((flags & 0xffff0000) == 0 || options == 0)
    return;
  if ((flags & EFFECT_USE_PALETTE) != 0)
    FieldActor_SetSpriteColor(effect, options->palette);
  if ((flags & EFFECT_USE_PRIORITY) != 0) {
    effect->priorityFlags &= ~1;
    Scene_SetActorSpritePriority(effect, options->priority);
  }
  start = flags & EFFECT_USE_START_SCALE;
  if (start != 0) {
    effect->scaleX = options->startScaleX;
    effect->scaleY = options->startScaleY;
  }
  if ((flags & EFFECT_SCALE_TO_TARGET) != 0) {
    s32 targetX;
    s32 count;

    script = gFieldEffect_DurationScripts[flags & EFFECT_SCRIPT_MASK];
    targetX = options->targetScaleX;
    count = script[3];
    if (start != 0) {
      effect->scaleRateX = (targetX - effect->scaleX) / count;
      effect->scaleRateY = (options->targetScaleY - effect->scaleY) / script[3];
    } else {
      effect->scaleRateX = (targetX - 0x10000) / count;
      effect->scaleRateY = (options->targetScaleY - 0x10000) / script[3];
    }
  }
  if ((flags & EFFECT_USE_SCRIPT) != 0) {
    Actor_SelectAnimation(effect, 1);
    Actor_StartScript(effect, options->script);
  }
  if ((flags & EFFECT_USE_ROTATION) != 0)
    sprite->rotation = options->rotation;
  if ((flags & EFFECT_USE_SPIN) != 0)
    effect->spin = options->spin;
  if ((flags & EFFECT_USE_UPDATE) != 0)
    effect->update = options->update;
}

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetPlacements(void) {
  return gActorPlacements;
}

const struct Trigger *Scene_GetEvents(void) { return gSceneEvents; }

/// Moves each of count colours one step of red, green and blue toward its
/// target colour.
void MtAlephSummit_StepColorsToward(u16 *colors, u16 *target, s32 count) {
  s32 i = 0;

  if (count == 0)
    return;
next:
  {
    s32 to_r = *target & 31;
    s32 to_g = (*target >> 5) & 31;
    s32 to_b = (*target >> 10) & 31;
    s32 r = *colors & 31;
    s32 g = (*colors >> 5) & 31;
    s32 b = (*colors >> 10) & 31;

    if (r < to_r)
      r++;
    else if (r > to_r)
      r--;
    if (g < to_g)
      g++;
    else if (g > to_g)
      g--;
    if (b < to_b)
      b++;
    else if (b > to_b)
      b--;
    *colors = b << 10 | g << 5 | r;
  }
  i++;
  target++;
  colors++;
  if (i != count)
    goto next;
}

/// Adds delta to each of count colours' red, green and blue, kept within 0 to
/// 31.
void MtAlephSummit_ShiftColors(u16 *colors, s32 count, s32 delta) {
  s32 i;
  s32 r, g, b;

  i = 0;
  if (count == 0)
    return;
loop:
  r = (*colors & 0x1f) + delta;
  g = ((*colors >> 5) & 0x1f) + delta;
  b = ((*colors >> 10) & 0x1f) + delta;
  if (r > 31)
    r = 31;
  if (r < 0)
    r = 0;
  if (g > 31)
    g = 31;
  if (g < 0)
    g = 0;
  if (b > 31)
    b = 31;
  if (b < 0)
    b = 0;
  *colors++ = (b << 10) | (g << 5) | r;
  i++;
  if (i != count)
    goto loop;
}

/// A task: the palette at 0x05000082 pulses brighter and darker than its
/// source at 0x050000c2 with the frame count.
void MtAlephSummit_PulseColors(void) {
  u16 *src = (u16 *)&BG_PLTT_COLOR(6, 1);
  u16 *dst = (u16 *)&BG_PLTT_COLOR(4, 1);
  s32 delta = (Math_Sin((gFrameCount << 11) >> 1) * 2) >> 16;
  s32 i = 0;

next:
  {
    s32 r = (*src & 31) + delta;
    s32 g = ((*src >> 5) & 31) + delta;
    s32 b = ((*src >> 10) & 31) + delta;

    if (r > 31)
      r = 31;
    if (r < 0)
      r = 0;
    if (g > 31)
      g = 31;
    if (g < 0)
      g = 0;
    if (b > 31)
      b = 31;
    if (b < 0)
      b = 0;
    *dst = b << 10 | g << 5 | r;
  }
  i++;
  src++;
  dst++;
  if (i != 15)
    goto next;
}

// clang-format off
u32 gFieldEffect_ShortScript[] = {                                                               
      SC_WAIT(1), SC_LOOP(22, SC_TOP),                                     
      SC_WAIT(1), SC_POSITION(0, 0, 0),                                        
      SC_WAIT(1), SC_FREE,                                                     
      SC_END,                                                                  
  };

u32 gFieldEffect_MediumScript[] = {                                                               
      SC_WAIT(1), SC_LOOP(44, SC_TOP),                                     
      SC_WAIT(1), SC_POSITION(0, 0, 0),                                        
      SC_WAIT(1), SC_FREE,                                                     
      SC_END,                                                                  
  };

u32 gFieldEffect_LongScript[] = {                                                               
      SC_WAIT(1), SC_LOOP(126, SC_TOP),                                     
      SC_WAIT(1), SC_POSITION(0, 0, 0),                                        
      SC_WAIT(1), SC_FREE,                                                     
      SC_END,                                                                  
  };
// clang-format on

const s32 *gFieldEffect_DurationScripts[] = {
    gFieldEffect_ShortScript,
    gFieldEffect_MediumScript,
    gFieldEffect_LongScript,
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {1, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS_END,
};

struct ActorPlacement gActorPlacements[] = {
    ACTOR_PLACEMENT(57, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 344,
                    FACING_NORTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(311, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120, 0, 24,
                    FACING_SOUTHEAST, TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

struct Trigger gSceneEvents[] = {
    {TRIGGER_TABLE_END},
};

#include "../field/field_effect/advance_motion.c"

// The summit's events: the Golden Sun's formation and what follows it.

#include "text/msg_ids.h"

void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options);

// Scatters count dust puffs in a ring of radius around actor 8.
void MtAlephSummit_ScatterDustRing(s32 radius, u32 count) {
  struct Actor *object;
  struct EffectOptions options;
  struct Vector3 dir;
  u32 i;
  s32 angle;

  object = Actor_Get(8);
  options.update = FieldEffect_AdvanceMotion;
  options.startScaleX = 0xcccc;
  options.startScaleY = 0xcccc;
  options.priority = ((struct FieldSprite *)object->sprite)->priority;
  i = 0;
next:
  angle = i * (0x10000 / (s32)count);
  dir.x = Trig_Cos(angle) * radius / 256;
  dir.y = 0;
  dir.z = Math_Sin(angle) * radius / 256;
  dir.x += dir.x >> 1;
  dir.x += ((Random16() * 3) << 14 >> 16) - 0x6000;
  dir.z += ((Random16() * 3) << 13 >> 16) - 0x3000;
  FieldEffect_Spawn(object->x, object->y, object->z, dir.x, dir.y, dir.z,
                    EFFECT_USE_UPDATE | EFFECT_USE_START_SCALE |
                        EFFECT_USE_PRIORITY | 1,
                    &options);
  i++;
  if (i <= count)
    goto next;
}

#define ACTOR(id) (Actor_Get(id))
#define FIELD(p, off) (*(s32 *)((u8 *)(p) + (off)))

void MtAlephSummit_StepColorsToward(u16 *colors, u16 *target, s32 count);
void MtAlephSummit_ShiftColors(u16 *colors, s32 count, s32 delta);
void MtAlephSummit_PulseColors(void);
void Field_StartColorTransition(s32 a, s32 b);

s32 Scene_Initialize(void) {
  u8 *block;
  void *buffer;
  s32 mode;
  s32 i, j, scale, r, g, b, value;
  s32 c;
  u16 *color, *out;
  u8 *message;
  struct Actor *object;
  volatile u32 *held;
  struct HeapSlots *slots;

  buffer = Heap_AllocEwram(0x200);
  Window_LoadGraphics();
  WindowColor_Apply(gMainRoutineWork.paletteGlow[0],
                    gMainRoutineWork.paletteGlow[1]);
  Actor_SetPosition(4, 0, 0);
  mode = gMainRoutineWork.entrance;
  if (mode == 10) {
    Event_SetDestination((s32)&SceneId_StaffCredit, 72);
  } else if (mode == 2) {
    FieldCamera_MoveTo(256, CAMERA_KEEP, 128, 0);
    FieldCamera_SetSpeed(1);
    Actor_SetSpriteFlags((s32)(s32)Actor_Get(8), 0);
    Actor_SetAnimation(8, 12);
    Actor_SetAnimationSpeedById(8, 8);
    gFieldWork->transition = 0;
    REG_DISPCNT &= 0xfdff;
    REG_BG1CNT &= 0xfffc;
    REG_BG1CNT |= 3;
    REG_BG3CNT &= 0xfffc;
    REG_BG3CNT |= 2;
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    FieldCamera_MoveTo(256, CAMERA_KEEP, 318, 1);
    FieldCamera_WaitForArrival();
    FieldCamera_MoveTo(316, CAMERA_KEEP, 318, 1);
    FieldCamera_WaitForArrival();
    i = 0;
  L1:
    if (i == 3) {
      FieldCamera_SetSpeed(0.1);
      FieldCamera_MoveTo(316, CAMERA_KEEP, 300, 1);
    }
    ACTOR(8)->z -= px(1);
    System_Wait(30);
    i++;
    if (i != 10)
      goto L1;
    Actor_SetAnimationSpeedById(8, 0);
    Iwram_CopyWords((s32)buffer, 0x5000008, 14);
    j = 0;
  L2:
    out = (u16 *)&BG_PLTT_COLOR(0, 7);
    color = out;
    i = 0;
  L3:
    c = *color;
    r = c & 31;
    g = ((u16)c >> 5) & 31;
    b = ((u16)c >> 10) & 31;
    r += j;
    g += j / 16;
    b -= j;
    if (r > 31)
      r = 31;
    if (g > 31)
      g = 31;
    if (b < 8)
      b = 8;
    if (b > 31)
      b = 31;
    *out = (b << 10) | (g << 5) | r;
    color++;
    out++;
    i++;
    if (i != 8)
      goto L3;
    System_Wait(4);
    j++;
    if (j != 10)
      goto L2;
    Actor_ShowEmote(8, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    System_Wait(30);
    FieldEvent_SetMessageId(MsgGoldenSunFormation);
    Event_ShowMessage(SPEAKER_WINDOW_ABOVE | 8, 0);
    System_Wait(30);
    Actor_RunRepeatedMotion(8, 2);
    Event_ShowMessage(SPEAKER_WINDOW_ABOVE | 8, 0);
    Actor_SetAnimationSpeedById(8, 32);
    i = 0;
  L4:
    if (i == 16) {
      FieldCamera_MoveTo(316, CAMERA_KEEP, 128, 1);
      FieldCamera_SetSpeed(2);
    }
    if (i == 19)
      Event_FinishScreenTransition();
    ACTOR(8)->z -= px(2);
    System_Wait(8);
    i++;
    if (i != 20)
      goto L4;
    Event_SetDestination((s32)&SceneId_StaffCredit, 1);
  } else if (mode == 3) {
    slots = Ram_HeapSlots;
    block = slots->block32;
    Iwram_CopyWords(buffer, (void *)0x5000000, 0x1c0);
    FieldCamera_MoveTo(256, CAMERA_KEEP, 144, 0);
    FieldCamera_SetSpeed(1);
    Actor_SetAnimation(8, 14);
    object = ACTOR(8);
    scale = 0;
    object->facing = scale;
    ACTOR(8)->x = px(248);
    ACTOR(8)->z = px(152);
    IO_WRITE_QUEUE_PUSH((u32)&REG_BLDCNT, 0x27f, 0x20000);
    IO_WRITE_QUEUE_PUSH((u32)&REG_BLDALPHA, 0x1000, 0x20000);
    REG_DISPCNT &= 0xfdff;
    REG_BG1CNT &= 0xfffc;
    REG_BG1CNT |= 3;
    REG_BG3CNT &= 0xfffc;
    REG_BG3CNT |= 2;
    System_Wait(1);
    ((struct FieldWork *)slots->fieldWork)->transition = scale;
    REG_DISPCNT &= 0xfdff;
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    MapGrid_SetShake(4, 4, 1);
    i = 0;
  L5:
    gBgScroll[2].y += 8;
    FieldEvent_Wait(5);
    i++;
    if (i != 30)
      goto L5;
    Actor_SetAnimation(8, 15);
    System_Wait(60);
    Event_PlaySound(SE_POWER_UP);
    REG_BG1CNT &= 0xfffc;
    REG_BG1CNT |= 1;
    REG_BLDCNT = 0x3f42;
    IO_WRITE_QUEUE_PUSH((u32)&REG_BLDALPHA, 0x1002, 0x20000);
    j = 0;
  L6:
    if (j == 16) {
      Event_PlaySound(SE_SPIN);
      Actor_SetAnimation(8, 16);
    }
    if (j == 51) {
      Event_PlaySound(SE_TREMOR);
      Iwram_CopyWordsEntry(buffer, (void *)0x5000000, 0x1c0);
      Iwram_CopyWordsEntry(block + 0xf80, block + 0x1040, 96);
    }
    if (j >= 16 && j <= 28) {
      value = (j - 16) / 2 + 2;
      IO_WRITE_QUEUE_PUSH((u32)&REG_BLDALPHA, value | 0x1000, 0x20000);
    }
    if (j >= 34 && j <= 50) {
      value = (j - 34) / 2 + 8;
      IO_WRITE_QUEUE_PUSH((u32)&REG_BLDALPHA, value | 0x1000, 0x20000);
    }
    if (j >= 20 && j <= 51)
      MtAlephSummit_StepColorsToward((u16 *)&BG_PLTT_COLOR(4, 0),
                                     (u16 *)&BG_PLTT_COLOR(6, 0), 16);
    if (j > 49)
      MtAlephSummit_ShiftColors((u16 *)&BG_PLTT_COLOR(0, 0), 224, 1);
    System_Wait(4);
    j++;
    if (j != 84)
      goto L6;
    FieldActor_SetSpritePriority(8, 3);
    MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
    Event_PlaySound(SOUND_FADE_OUT_ACTION_SE);
    FieldCamera_MoveTo(256, CAMERA_KEEP, 318, 0);
    IO_WRITE_QUEUE_PUSH((u32)&REG_BG1CNT, (REG_BG1CNT & ~3) | 3, 0x20000);
    Event_PlaySound(SE_POWER_UP);
    System_Wait(1);
    Task_AddFrame(MtAlephSummit_PulseColors, 0x480);
    j = 0;
  L7:
    MtAlephSummit_StepColorsToward((u16 *)&BG_PLTT_COLOR(0, 0), buffer, 224);
    System_Wait(4);
    j++;
    if (j != 32)
      goto L7;
    FieldActor_SetSpritePriority(8, 1);
    Actor_SetAnimation(8, 1);
    FieldCamera_MoveTo(256, CAMERA_KEEP, 128, 1);
    FieldCamera_SetSpeed(1);
    FieldCamera_WaitForArrival();
    Actor_SetSpeed(8, 0.3);
    Actor_WalkToAndWait(8, 248, 128);
    Actor_RunRepeatedMotion(8, 2);
    FieldEvent_Wait(5);
    Event_PlaySound(SOUND_FADE_OUT_BGM);
    FieldEvent_SetMessageId(MsgGoldenSunEternalLifeAndPower);
    FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | 8, 0, 5);
    Actor_FaceDirection(8, FACING_SOUTH, 0);
    FieldEvent_Wait(30);
    Actor_SetAnimation(8, 14);
    FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | 8, 0, 5);
    Actor_SetAnimation(8, 15);
    FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | 8, 0, 150);
    Actor_ShowEmote(8, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
    Actor_SetAnimation(8, 1);
    Actor_FaceDirection(8, FACING_SOUTHWEST, 0);
    FieldEvent_Wait(10);
    Actor_FaceDirection(8, FACING_SOUTHEAST, 0);
    FieldEvent_Wait(10);
    Actor_FaceDirection(8, FACING_SOUTHWEST, 0);
    FieldEvent_Wait(10);
    Actor_FaceDirection(8, FACING_SOUTHEAST, 0);
    FieldEvent_Wait(10);
    Actor_FaceDirection(8, FACING_SOUTH, 0);
    FieldEvent_Wait(50);
    Actor_SetAnimation(8, 15);
    FieldEvent_Wait(50);
    FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | 8, 0, 5);
    FieldEvent_Wait(50);
    Actor_RunRepeatedMotion(8, 2);
    FieldEvent_Wait(5);
    Actor_SetSpriteFlags((s32)(s32)Actor_Get(9), 0);
    Actor_WalkToAndWait(9, 216, 96);
    Actor_FaceDirection(8, FACING_SOUTHEAST, 0);
    Actor_ShowEmote(8, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
    FieldEvent_SetMessageId((MsgGoldenSunEternalLifeAndPower + 4));
    FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | 8, 0, 5);
    Event_PlaySound(MUS_722);
    FieldEvent_ShowMessageThenWait(9, 0, 5);
    Actor_SetAnimation(8, 1);
    Actor_FaceDirection(8, FACING_NORTHWEST, 0);
    Actor_ShowEmote(8, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
    ACTOR(8)->actionFlags &= ~1;
    Actor_WalkTo(8, 256, 136);
    Actor_SetSpeed(8, 0.6);
    FieldEvent_Wait(20);
    Actor_SetAnimation(8, 1);
    FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | 8, 0, 5);
    Actor_SetAnimation(9, 4);
    FieldEvent_ShowMessageThenWait(9, 0, 5);
    Actor_RunRepeatedMotion(8, 2);
    FieldEvent_Wait(5);
    FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | 8, 0, 5);
    message = MsgGoldenSunEternalLifeAndPower;
    Actor_SetAnimation(9, 5);
    FieldEvent_ShowMessageThenWait(9, 0, 5);
    Actor_SetAttachedEffect(8, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    FieldEvent_Wait(20);
    FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | 8, 0, 5);
    Actor_SetAnimation(9, 6);
    FieldEvent_ShowMessageThenWait(9, 0, 5);
    Actor_ShowEmote(8, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
    message += 12;
    FieldEvent_SetMessageId(message);
    FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | 8, 0, 5);
    Actor_SetAnimation(8, 13);
    BattleEffect_StartArcEffect();
    BattleEffect_SetObjectUpdateMode((s32)Actor_Get(8), 2);
    Event_PlaySound(SE_178);
    Field_StartColorTransition(8, 0x4084);
    FieldEvent_Wait(20);
    Actor_SetAnimation(9, 3);
    Actor_SetSpeed(9, 0.1);
    j = 0;
  L8:
    if (j & 1) {
      Actor_WalkToAndWait(9, 216, 96);
    } else {
      ACTOR(9)->actionFlags &= ~1;
      Actor_WalkTo(9, 208, 88);
    }
    i = 0;
  L9:
    Actor_SetSpriteColorById(9, 0x100);
    FieldEvent_Wait(3);
    Actor_SetSpriteColorById(9, 0);
    FieldEvent_Wait(3);
    i++;
    if (i != 10)
      goto L9;
    j++;
    if (j != 4)
      goto L8;
    Actor_ShowEmote(8, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
    FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | 8, 0, 5);
    FieldEvent_Wait(10);
    BattleEffect_SetObjectUpdateMode((s32)Actor_Get(8), 0);
    BattleEffect_WaitForArcGroups();
    BattleEffect_EndArcEffect();
    Actor_SetAnimation(9, 6);
    BattleEffect_StartArcEffect();
    BattleEffect_SetObjectUpdateMode((s32)Actor_Get(9), 2);
    System_Wait(30);
    Event_PlaySound(SE_POWER_UP);
    MapGrid_SetShake(16, 16, 1);
    i = 0;
  L10:
    PaletteFade_ApplyTarget(0x10005, 1);
    PaletteFade_Interpolate(1);
    FieldEvent_Wait(2);
    PaletteFade_ApplyTarget(0x10000, 1);
    PaletteFade_Interpolate(1);
    FieldEvent_Wait(2);
    i++;
    if (i != 4)
      goto L10;
    MapGrid_SetShake(0, 0, 0);
    Actor_RunRepeatedMotion(8, 2);
    Field_StartPaletteTransition(1);
    Field_StartColorTransition(8, 0x1110);
    FieldEvent_Wait(20);
    Actor_SetSpeed(8, 8);
    MtAlephSummit_ScatterDustRing(128, 8);
    ACTOR(8)->actionFlags &= ~1;
    Actor_WalkTo(8, 280, 160);
    Actor_SetAnimation(8, 17);
    Event_PlaySound(SE_HEAVY_IMPACT);
    FieldEvent_Wait(2);
    MtAlephSummit_ScatterDustRing(128, 8);
    FieldEvent_Wait(2);
    MtAlephSummit_ScatterDustRing(128, 8);
    FieldEvent_Wait(56);
    Event_PlaySound(SE_178);
    i = 0;
    ACTOR(8)->motionFlags = i;
  L11:
    if (i == 16)
      Actor_SetAnimation(8, 18);
    ACTOR(8)->y += 0x2666;
    FieldEvent_Wait(1);
    i++;
    if (i != 64)
      goto L11;
    Actor_RunRepeatedMotion(8, 2);
    FieldEvent_Wait(5);
    FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | 8, 0, 5);
    i = 0;
  L12:
    ACTOR(8)->y += px(1);
    FieldEvent_Wait(1);
    i++;
    if (i != 32)
      goto L12;
    ACTOR(8)->motionFlags = 1;
    i = 0;
  L13:
    ACTOR(8)->y -= 0x851eb;
    FieldEvent_Wait(1);
    i++;
    if (i != 5)
      goto L13;
    MtAlephSummit_ScatterDustRing(256, 16);
    Actor_SetAnimation(8, 17);
    Event_PlaySound(SE_CRASH);
    MapGrid_SetShake(16, 16, 1);
    FieldEvent_Wait(10);
    MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
    BattleEffect_SetObjectUpdateMode((s32)Actor_Get(9), 0);
    BattleEffect_WaitForArcGroups();
    BattleEffect_EndArcEffect();
    Event_PlaySound(SOUND_FADE_OUT_ACTION_SE);
    Actor_SetAnimation(9, 4);
    Field_StartPaletteTransition(32);
    FieldEvent_Wait(10);
    FieldEvent_ShowMessageThenWait(9, 0, 5);
    Actor_RunRepeatedMotion(8, 2);
    FieldEvent_Wait(5);
    FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | 8, 0, 5);
    Actor_SetAnimation(9, 3);
    FieldEvent_ShowMessageThenWait(9, 0, 5);
    Actor_SetAnimation(8, 18);
    FieldEvent_Wait(10);
    FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | 8, 0, 5);
    FieldEvent_Wait(120);
    Event_PlaySound(SE_TREMOR);
    i = 0;
  L14:
    MapGrid_SetShakeFixed((i << 16) + 0x10000, (i << 16) + 0x10000, 0x10000);
    FieldEvent_Wait(8);
    i++;
    if (i != 2)
      goto L14;
    Actor_SetAnimation(9, 8);
    FieldEvent_Wait(60);
    Actor_SetAnimation(9, 7);
    FieldEvent_ShowMessageThenWait(9, 0, 5);
    Actor_ShowEmote(8, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
    FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | 8, 0, 5);
    Actor_SetAnimation(9, 8);
    FieldEvent_ShowMessageThenWait(9, 0, 5);
    for (i = 0, j = 0x20000; i != 4; i++, j += 0x10000) {
      MapGrid_SetShakeFixed(j, j, 0x10000);
      FieldEvent_Wait(8);
    }
    Actor_RunRepeatedMotion(8, 2);
    FieldEvent_Wait(5);
    FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | 8, 0, 5);
    Actor_SetAnimation(9, 3);
    FieldEvent_ShowMessageThenWait(9, 0, 5);
    Actor_RunRepeatedMotion(8, 2);
    FieldEvent_Wait(5);
    Actor_SetAnimation(9, 4);
    FieldEvent_ShowMessageThenWait(9, 0, 5);
    FieldEvent_Wait(10);
    slots = (struct HeapSlots *)Ram_HeapSlots->mapWork;
    MapGrid_SetShake(16, 16, 1);
    j = 0;
    scale = -0x1e000;
  L16:
    if (j == 20)
      Actor_SetAnimation(9, 7);
    if (j >= 30 && j <= 46) {
      ACTOR(9)->scaleX = Q16_ONE - scale;
      ACTOR(9)->scaleY = scale + 0x10000;
      ACTOR(9)->z -= px(2);
    }
    if (j == 47)
      Actor_SetPosition(9, 0, 0);
    if (j == 64) {
      PaletteFade_ApplyTarget(0x7fff, 0);
      PaletteFade_Interpolate(64);
    }
    FIELD(slots, 332) -= 0x10000;
    ACTOR(8)->z += px(1);
    System_Wait(1);
    j++;
    scale += 0x1000;
    if (j != 128)
      goto L16;
    Event_PlaySound(SOUND_FADE_OUT_BGM);
    FieldEvent_Wait(32);
    Event_PlaySound(SE_BANE_ESCAPE);
    FieldEvent_Wait(120);
    FieldTask_Remove(MtAlephSummit_PulseColors);
    PaletteFade_ApplyTarget(0x7fff, 0);
    PaletteFade_Interpolate(1);
    System_Wait(1);
    Event_PlaySound(SOUND_FADE_OUT_ACTION_SE);
    PaletteFade_ApplyTarget(0, 0);
    PaletteFade_Interpolate(120);
    System_Wait(300);
    Event_SetDestination((s32)&SceneId_Epilogue, 1);
  } else {
    gFieldWork->transition = 0;
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    for (j = 0; j != 0x140; j++) {
      held = &gInput.held;
      if (*held & 2)
        j = 0;
      if (*held & 1)
        break;
      if (j == 0) {
        Actor_SetSpriteFlags((s32)(s32)Actor_Get(8), 0);
        Actor_SetAnimation(8, 12);
      }
    }
    Event_SetDestination((s32)&SceneId_StaffCredit, 0);
  }
  Heap_Release(buffer);
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

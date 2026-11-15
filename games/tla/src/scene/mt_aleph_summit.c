/// @file
/// The summit of Mt. Aleph, at the end of the game: the Golden Sun forms over
/// the mountain, Alex claims its power, and the scene's colours and dust follow
/// what happens.
#include "../field/actor_set_sprite_priority.c"

/// The summit, by the rows of gActorPlacements.
enum MtAlephSummitActorId {
  ACTOR_ALEX = ACTOR_FIRST_PLACED,
  ACTOR_WISE_ONE ///< "I am called the Wise One..."
};

/// The summit's ways in, each a stretch of the ending; any other entrance
/// waits on the summit for A before the credits begin, and B starts the wait
/// over.
enum {
  /// Alex sees the Golden Sun begin to form and climbs toward it, between the
  /// credits' first two stretches.
  ENTRANCE_GOLDEN_SUN_FORMS = 2,
  /// Alex takes the Golden Sun's power and the Wise One shows him its limits,
  /// after the lighthouses' beacons.
  ENTRANCE_ALEX_TAKES_THE_POWER = 3,
  /// Unused: goes straight on to the credits' entrance 72.
  ENTRANCE_SKIP_TO_CREDITS = 10
};

#include "../field/field_effect_move.c"

/// The background colours the palette fade keeps, palettes 0 to 13, and the
/// fade's three channels of one 16-colour palette.
#define FADED_BG_COLORS (14 * PALETTE_COLORS)
#define PALETTE_CHANNELS (3 * PALETTE_COLORS)

#include "system/task.h"
#include "graphics/palette_buffers.h"
#include "shared/tables.h"
#include "types.h"
#include "system/main_routine_work.h"
#include "field/field_sprite.h"
#include "system/field_effect.h"
#include "field/event_inline.h"
#include "battle/battle.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "field/scene_tables.h"
#include "field/actor_script.h"
#include "system/frame.h"
#include "field/actor.h"
#include "system/vector3.h"
#include "system/ram_buffer.h"
#include "system/io_reg.h"
#include "system/io_write_queue.h"
#include "system/iwram_call.h"
#include "system/heap_release.h"
#include "system/bg_scroll.h"
#include "system/input.h"
#include "field/call.h"
#include "field/map_grid.h"
#include "field/field_sequence_event.h"
#include "field/event_commands.h"
#include "field/field_effect.h"
#include "scene/scene.h"
#include "scene/event_layer.h"

extern const s32 *gFieldEffect_DurationScripts[];

extern u32 gFieldEffect_ShortScript[];

extern u32 gFieldEffect_MediumScript[];
extern u32 gFieldEffect_LongScript[];

void FieldCamera_WaitForArrival(void);

extern void Actor_StartScript(struct FieldEffect *effect, const s32 *script);
#define EFFECT_SPAWN FieldEffect_Spawn
#define EFFECT_CREATE Import_ActorCreate
#define EFFECT_SET_MODE Actor_SelectAnimation
#define EFFECT_SET_SCRIPT Actor_StartScript
#define EFFECT_SCRIPTS gFieldEffect_DurationScripts
#include "../field/field_effect_spawn.c"
#undef EFFECT_SPAWN
#undef EFFECT_CREATE
#undef EFFECT_SET_MODE
#undef EFFECT_SET_SCRIPT
#undef EFFECT_SCRIPTS

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  return gActorPlacements;
}

const struct Trigger *Scene_GetTriggers(void) { return gSceneEvents; }

/// Moves each of count colours one step of red, green and blue toward its
/// target colour.
void MtAlephSummit_StepColorsToward(u16 *colors, u16 *target, s32 count) {
  s32 i = 0;

  if (count == 0)
    return;
next:
  {
    s32 to_r = RGB_RED(*target);
    s32 to_g = RGB_GREEN(*target);
    s32 to_b = RGB_BLUE(*target);
    s32 r = RGB_RED(*colors);
    s32 g = RGB_GREEN(*colors);
    s32 b = RGB_BLUE(*colors);

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
    *colors = RGB(r, g, b);
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
  r = RGB_RED(*colors) + delta;
  g = RGB_GREEN(*colors) + delta;
  b = RGB_BLUE(*colors) + delta;
  if (r > RGB_LEVEL_MAX)
    r = RGB_LEVEL_MAX;
  if (r < 0)
    r = 0;
  if (g > RGB_LEVEL_MAX)
    g = RGB_LEVEL_MAX;
  if (g < 0)
    g = 0;
  if (b > RGB_LEVEL_MAX)
    b = RGB_LEVEL_MAX;
  if (b < 0)
    b = 0;
  *colors++ = RGB(r, g, b);
  i++;
  if (i != count)
    goto loop;
}

/// A task: background palette 4's colours 1 to 15 pulse brighter and darker
/// than palette 6's with the frame count.
void MtAlephSummit_PulseColors(void) {
  u16 *src = (u16 *)&BG_PLTT_COLOR(6, 1);
  u16 *dst = (u16 *)&BG_PLTT_COLOR(4, 1);
  s32 delta = (Math_Sin((gFrameCount << 11) >> 1) * 2) >> 16;
  s32 i = 0;

next:
  {
    s32 r = RGB_RED(*src) + delta;
    s32 g = RGB_GREEN(*src) + delta;
    s32 b = RGB_BLUE(*src) + delta;

    if (r > RGB_LEVEL_MAX)
      r = RGB_LEVEL_MAX;
    if (r < 0)
      r = 0;
    if (g > RGB_LEVEL_MAX)
      g = RGB_LEVEL_MAX;
    if (g < 0)
      g = 0;
    if (b > RGB_LEVEL_MAX)
      b = RGB_LEVEL_MAX;
    if (b < 0)
      b = 0;
    *dst = RGB(r, g, b);
  }
  i++;
  src++;
  dst++;
  if (i != PALETTE_COLORS - 1)
    goto next;
}

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
    ACTOR_PLACEMENT(SPRITE_ALEX, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0,
                    344, FACING_NORTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WISE_ONE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120,
                    0, 24, FACING_SOUTHEAST, TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

struct Trigger gSceneEvents[] = {
    {TRIGGER_TABLE_END},
};

#include "../field/field_effect_advance_motion.c"

// The summit's events: the Golden Sun's formation and what follows it.

#include "text/msg_ids.h"

void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options);

/// Scatters count dust puffs in a ring of radius around Alex.
void MtAlephSummit_ScatterDustRing(s32 radius, u32 count) {
  struct Actor *object;
  struct EffectOptions options;
  struct Vector3 dir;
  u32 i;
  s32 angle;

  object = Actor_Get(ACTOR_ALEX);
  options.update = FieldEffect_AdvanceMotion;
  options.startScaleX = Q16_ONE * 4 / 5;
  options.startScaleY = Q16_ONE * 4 / 5;
  options.priority = ((struct FieldSprite *)object->sprite)->priority;
  i = 0;
next:
  angle = i * (TRIG_TURN / (s32)count);
  dir.x = Trig_Cos(angle) * radius / 256;
  dir.y = 0;
  dir.z = Math_Sin(angle) * radius / 256;
  dir.x += dir.x >> 1;
  dir.x += ((Random16() * 3) << 14 >> 16) - 0x6000;
  dir.z += ((Random16() * 3) << 13 >> 16) - 0x3000;
  FieldEffect_Spawn(
      object->x.fixed, object->y.fixed, object->z.fixed, dir.x, dir.y, dir.z,
      EFFECT_USE_UPDATE | EFFECT_USE_START_SCALE | EFFECT_USE_PRIORITY | 1,
      &options);
  i++;
  if (i <= count)
    goto next;
}

void MtAlephSummit_StepColorsToward(u16 *colors, u16 *target, s32 count);
void MtAlephSummit_ShiftColors(u16 *colors, s32 count, s32 delta);
void MtAlephSummit_PulseColors(void);

s32 Scene_Initialize(void) {
  struct PaletteFade *fade;
  void *buffer;
  s32 mode;
  s32 i, j, scale, r, g, b, value;
  s32 c;
  u16 *color, *out;
  u8 *message;
  struct Actor *object;
  volatile u32 *held;
  struct HeapSlots *slots;

  buffer = Heap_AllocEwram(BG_PLTT_SIZE);
  Window_LoadGraphics();
  WindowColor_Apply(gMainRoutineWork.windowHue,
                    gMainRoutineWork.windowBrightness);
  Actor_SetPosition(ACTOR_FELIX, 0, 0);
  mode = gMainRoutineWork.entrance;
  if (mode == ENTRANCE_SKIP_TO_CREDITS) {
    Event_SetDestination(SCENE_STAFF_CREDIT, 72);
  } else if (mode == ENTRANCE_GOLDEN_SUN_FORMS) {
    FieldCamera_MoveTo(256, CAMERA_KEEP, 128, 0);
    FieldCamera_SetSpeed(1);
    Actor_SetSpriteFlags(Actor_Get(ACTOR_ALEX), 0);
    Actor_SetAnimation(ACTOR_ALEX, 12);
    Actor_SetAnimationSpeedById(ACTOR_ALEX, 8);
    gFieldWork->transition = 0;
    REG_DISPCNT &= ~DISPCNT_BG1_ON;
    REG_BG1CNT &= ~BGCNT_PRIORITY(3);
    REG_BG1CNT |= BGCNT_PRIORITY(3);
    REG_BG3CNT &= ~BGCNT_PRIORITY(3);
    REG_BG3CNT |= BGCNT_PRIORITY(2);
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
    Actor_Get(ACTOR_ALEX)->z.fixed -= px(1);
    System_Wait(30);
    i++;
    if (i != 10)
      goto L1;
    Actor_SetAnimationSpeedById(ACTOR_ALEX, 0);
    Iwram_CopyWords(buffer, (void *)&BG_PLTT_COLOR(0, 4), 14);
    j = 0;
  L2:
    out = (u16 *)&BG_PLTT_COLOR(0, 7);
    color = out;
    i = 0;
  L3:
    c = *color;
    r = RGB_RED(c);
    g = RGB_GREEN((u16)c);
    b = RGB_BLUE((u16)c);
    r += j;
    g += j / 16;
    b -= j;
    if (r > RGB_LEVEL_MAX)
      r = RGB_LEVEL_MAX;
    if (g > RGB_LEVEL_MAX)
      g = RGB_LEVEL_MAX;
    if (b < 8)
      b = 8;
    if (b > RGB_LEVEL_MAX)
      b = RGB_LEVEL_MAX;
    *out = RGB(r, g, b);
    color++;
    out++;
    i++;
    if (i != 8)
      goto L3;
    System_Wait(4);
    j++;
    if (j != 10)
      goto L2;
    Actor_ShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    System_Wait(30);
    FieldEvent_SetMessageId(MsgGoldenSunFormation);
    // "The Golden Sun... The very quintessence of Alchemy's power! It's..."
    Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_ALEX, 0);
    System_Wait(30);
    Actor_RunRepeatedMotion(ACTOR_ALEX, 2);
    // "Wait for me! Please, wait! Wait until I reach the summit of Mt. Aleph!"
    Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_ALEX, 0);
    Actor_SetAnimationSpeedById(ACTOR_ALEX, 32);
    i = 0;
  L4:
    if (i == 16) {
      FieldCamera_MoveTo(316, CAMERA_KEEP, 128, 1);
      FieldCamera_SetSpeed(2);
    }
    if (i == 19)
      Event_FinishScreenTransition();
    Actor_Get(ACTOR_ALEX)->z.fixed -= px(2);
    System_Wait(8);
    i++;
    if (i != 20)
      goto L4;
    Event_SetDestination(SCENE_STAFF_CREDIT, 1);
  } else if (mode == ENTRANCE_ALEX_TAKES_THE_POWER) {
    slots = Ram_HeapSlots;
    fade = slots->paletteFade;
    Iwram_CopyWords(buffer, (void *)&BG_PLTT_COLOR(0, 0),
                    FADED_BG_COLORS * sizeof(u16));
    FieldCamera_MoveTo(256, CAMERA_KEEP, 144, 0);
    FieldCamera_SetSpeed(1);
    Actor_SetAnimation(ACTOR_ALEX, 14);
    object = Actor_Get(ACTOR_ALEX);
    scale = 0;
    object->facing = scale;
    Actor_Get(ACTOR_ALEX)->x.fixed = px(248);
    Actor_Get(ACTOR_ALEX)->z.fixed = px(152);
    IO_WRITE_QUEUE_PUSH((u32)&REG_BLDCNT,
                        BLDCNT_TGT1_ALL | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_BG1,
                        IO_WRITE_HALFWORD);
    IO_WRITE_QUEUE_PUSH((u32)&REG_BLDALPHA, BLEND_ALPHA(0, BLEND_WEIGHT_MAX),
                        IO_WRITE_HALFWORD);
    REG_DISPCNT &= ~DISPCNT_BG1_ON;
    REG_BG1CNT &= ~BGCNT_PRIORITY(3);
    REG_BG1CNT |= BGCNT_PRIORITY(3);
    REG_BG3CNT &= ~BGCNT_PRIORITY(3);
    REG_BG3CNT |= BGCNT_PRIORITY(2);
    System_Wait(1);
    ((struct FieldWork *)slots->fieldWork)->transition = scale;
    REG_DISPCNT &= ~DISPCNT_BG1_ON;
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
    Actor_SetAnimation(ACTOR_ALEX, 15);
    System_Wait(60);
    Event_PlaySound(SE_POWER_UP);
    REG_BG1CNT &= ~BGCNT_PRIORITY(3);
    REG_BG1CNT |= BGCNT_PRIORITY(1);
    REG_BLDCNT = BLDCNT_TGT1_BG1 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
    IO_WRITE_QUEUE_PUSH((u32)&REG_BLDALPHA, BLEND_ALPHA(2, BLEND_WEIGHT_MAX),
                        IO_WRITE_HALFWORD);
    j = 0;
  L6:
    if (j == 16) {
      Event_PlaySound(SE_SPIN);
      Actor_SetAnimation(ACTOR_ALEX, 16);
    }
    if (j == 51) {
      Event_PlaySound(SE_TREMOR);
      Iwram_CopyWordsEntry(buffer, (void *)&BG_PLTT_COLOR(0, 0),
                           FADED_BG_COLORS * sizeof(u16));
      Iwram_CopyWordsEntry(&fade->target[4 * PALETTE_CHANNELS],
                           &fade->target[6 * PALETTE_CHANNELS],
                           PALETTE_CHANNELS * sizeof(u16));
    }
    if (j >= 16 && j <= 28) {
      value = (j - 16) / 2 + 2;
      IO_WRITE_QUEUE_PUSH((u32)&REG_BLDALPHA,
                          BLEND_ALPHA(value, BLEND_WEIGHT_MAX),
                          IO_WRITE_HALFWORD);
    }
    if (j >= 34 && j <= 50) {
      value = (j - 34) / 2 + 8;
      IO_WRITE_QUEUE_PUSH((u32)&REG_BLDALPHA,
                          BLEND_ALPHA(value, BLEND_WEIGHT_MAX),
                          IO_WRITE_HALFWORD);
    }
    if (j >= 20 && j <= 51)
      MtAlephSummit_StepColorsToward((u16 *)&BG_PLTT_COLOR(4, 0),
                                     (u16 *)&BG_PLTT_COLOR(6, 0), 16);
    if (j > 49)
      MtAlephSummit_ShiftColors((u16 *)&BG_PLTT_COLOR(0, 0), FADED_BG_COLORS,
                                1);
    System_Wait(4);
    j++;
    if (j != 84)
      goto L6;
    FieldActor_SetSpritePriority(ACTOR_ALEX, 3);
    MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
    Event_PlaySound(SOUND_FADE_OUT_ACTION_SE);
    FieldCamera_MoveTo(256, CAMERA_KEEP, 318, 0);
    IO_WRITE_QUEUE_PUSH((u32)&REG_BG1CNT,
                        (REG_BG1CNT & ~BGCNT_PRIORITY(3)) | BGCNT_PRIORITY(3),
                        IO_WRITE_HALFWORD);
    Event_PlaySound(SE_POWER_UP);
    System_Wait(1);
    Task_AddFrame(MtAlephSummit_PulseColors, TASK_PASS_VBLANK);
    j = 0;
    do {
      MtAlephSummit_StepColorsToward((u16 *)&BG_PLTT_COLOR(0, 0), buffer,
                                     FADED_BG_COLORS);
      System_Wait(4);
      j++;
    } while (j != 32);
    FieldActor_SetSpritePriority(ACTOR_ALEX, 1);
    Actor_SetAnimation(ACTOR_ALEX, 1);
    FieldCamera_MoveTo(256, CAMERA_KEEP, 128, 1);
    FieldCamera_SetSpeed(1);
    FieldCamera_WaitForArrival();
    Actor_SetSpeed(ACTOR_ALEX, 0.3);
    Actor_WalkToAndWait(ACTOR_ALEX, 248, 128);
    Actor_RunRepeatedMotion(ACTOR_ALEX, 2);
    FieldEvent_Wait(5);
    Event_PlaySound(SOUND_FADE_OUT_BGM);
    FieldEvent_SetMessageId(MsgGoldenSunEternalLifeAndPower);
    // "At last! I have it! Eternal life...and limitless power!"
    FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0, 5);
    Actor_FaceDirection(ACTOR_ALEX, FACING_SOUTH, 0);
    FieldEvent_Wait(30);
    Actor_SetAnimation(ACTOR_ALEX, 14);
    // "At last, the power of nature is mine to control as I will!"
    FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0, 5);
    Actor_SetAnimation(ACTOR_ALEX, 15);
    // "Rise, storms! Rise up and unleash your might upon Vale and the..."
    FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0, 150);
    Actor_ShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
    Actor_SetAnimation(ACTOR_ALEX, 1);
    Actor_FaceDirection(ACTOR_ALEX, FACING_SOUTHWEST, 0);
    FieldEvent_Wait(10);
    Actor_FaceDirection(ACTOR_ALEX, FACING_SOUTHEAST, 0);
    FieldEvent_Wait(10);
    Actor_FaceDirection(ACTOR_ALEX, FACING_SOUTHWEST, 0);
    FieldEvent_Wait(10);
    Actor_FaceDirection(ACTOR_ALEX, FACING_SOUTHEAST, 0);
    FieldEvent_Wait(10);
    Actor_FaceDirection(ACTOR_ALEX, FACING_SOUTH, 0);
    FieldEvent_Wait(50);
    Actor_SetAnimation(ACTOR_ALEX, 15);
    FieldEvent_Wait(50);
    // "That's odd... I should have limitless power... So why can't I call..."
    FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0, 5);
    FieldEvent_Wait(50);
    Actor_RunRepeatedMotion(ACTOR_ALEX, 2);
    FieldEvent_Wait(5);
    Actor_SetSpriteFlags(Actor_Get(ACTOR_WISE_ONE), 0);
    Actor_WalkToAndWait(ACTOR_WISE_ONE, 216, 96);
    Actor_FaceDirection(ACTOR_ALEX, FACING_SOUTHEAST, 0);
    Actor_ShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
    FieldEvent_SetMessageId(MsgGoldenSunEternalLifeAndPower + 4);
    // "Wh-Who are you?"
    FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0, 5);
    Event_PlaySound(MUS_ELEMENTAL_STARS);
    // "I am called the Wise One..."
    FieldEvent_ShowMessageThenWait(ACTOR_WISE_ONE, 0, 5);
    Actor_SetAnimation(ACTOR_ALEX, 1);
    Actor_FaceDirection(ACTOR_ALEX, FACING_NORTHWEST, 0);
    Actor_ShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
    Actor_Get(ACTOR_ALEX)->steeringFlags &= ~ACTOR_FACES_MOTION;
    Actor_WalkTo(ACTOR_ALEX, 256, 136);
    Actor_SetSpeed(ACTOR_ALEX, 0.6);
    FieldEvent_Wait(20);
    Actor_SetAnimation(ACTOR_ALEX, 1);
    // "The Wise One? Vale's protector?"
    FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0, 5);
    Actor_SetAnimation(ACTOR_WISE_ONE, 4);
    // "You wish to have limitless power?"
    FieldEvent_ShowMessageThenWait(ACTOR_WISE_ONE, 0, 5);
    Actor_RunRepeatedMotion(ACTOR_ALEX, 2);
    FieldEvent_Wait(5);
    // "Wish to? I just got it!"
    FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0, 5);
    message = MsgGoldenSunEternalLifeAndPower;
    Actor_SetAnimation(ACTOR_WISE_ONE, 5);
    // "No. Your power is nearly limitless, but it has boundaries."
    FieldEvent_ShowMessageThenWait(ACTOR_WISE_ONE, 0, 5);
    Actor_SetAttachedEffect(ACTOR_ALEX,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    FieldEvent_Wait(20);
    // "Nearly limitless? You speak in riddles. Can't you see? The power is..."
    FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0, 5);
    Actor_SetAnimation(ACTOR_WISE_ONE, 6);
    // "Yes. You also have nearly endless life. And your Psynergy is..."
    FieldEvent_ShowMessageThenWait(ACTOR_WISE_ONE, 0, 5);
    Actor_ShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
    message += 12;
    FieldEvent_SetMessageId(message);
    // "If you are trying to anger me, have a little taste of exactly how..."
    FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0, 5);
    Actor_SetAnimation(ACTOR_ALEX, 13);
    BattleEffect_StartArcEffect();
    BattleEffect_SetObjectUpdateMode(Actor_Get(ACTOR_ALEX), 2);
    Event_PlaySound(SE_178);
    Event_StartColorTransition(8, RGB(4, 4, 16));
    FieldEvent_Wait(20);
    Actor_SetAnimation(ACTOR_WISE_ONE, 3);
    Actor_SetSpeed(ACTOR_WISE_ONE, 0.1);
    j = 0;
    do {
      if (j & 1) {
        Actor_WalkToAndWait(ACTOR_WISE_ONE, 216, 96);
      } else {
        Actor_Get(ACTOR_WISE_ONE)->steeringFlags &= ~ACTOR_FACES_MOTION;
        Actor_WalkTo(ACTOR_WISE_ONE, 208, 88);
      }
      i = 0;
      do {
        Actor_SetSpriteColorById(ACTOR_WISE_ONE, 0x100);
        FieldEvent_Wait(3);
        Actor_SetSpriteColorById(ACTOR_WISE_ONE, 0);
        FieldEvent_Wait(3);
        i++;
      } while (i != 10);
      j++;
    } while (j != 4);
    Actor_ShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
    // "Look at me! My body is brimming with power!"
    FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0, 5);
    FieldEvent_Wait(10);
    BattleEffect_SetObjectUpdateMode(Actor_Get(ACTOR_ALEX), 0);
    BattleEffect_WaitForArcGroups();
    BattleEffect_EndArcEffect();
    Actor_SetAnimation(ACTOR_WISE_ONE, 6);
    BattleEffect_StartArcEffect();
    BattleEffect_SetObjectUpdateMode(Actor_Get(ACTOR_WISE_ONE), 2);
    System_Wait(30);
    Event_PlaySound(SE_POWER_UP);
    MapGrid_SetShake(16, 16, 1);
    i = 0;
    do {
      PaletteFade_ApplyTarget(PALETTE_TINT_PALE, PALETTES_BG);
      PaletteFade_Interpolate(1);
      FieldEvent_Wait(2);
      PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_BG);
      PaletteFade_Interpolate(1);
      FieldEvent_Wait(2);
      i++;
    } while (i != 4);
    MapGrid_SetShake(0, 0, 0);
    Actor_RunRepeatedMotion(ACTOR_ALEX, 2);
    Field_StartPaletteTransition(1);
    Event_StartColorTransition(8, RGB(16, 8, 4));
    FieldEvent_Wait(20);
    Actor_SetSpeed(ACTOR_ALEX, 8);
    MtAlephSummit_ScatterDustRing(128, 8);
    Actor_Get(ACTOR_ALEX)->steeringFlags &= ~ACTOR_FACES_MOTION;
    Actor_WalkTo(ACTOR_ALEX, 280, 160);
    Actor_SetAnimation(ACTOR_ALEX, 17);
    Event_PlaySound(SE_HEAVY_IMPACT);
    FieldEvent_Wait(2);
    MtAlephSummit_ScatterDustRing(128, 8);
    FieldEvent_Wait(2);
    MtAlephSummit_ScatterDustRing(128, 8);
    FieldEvent_Wait(56);
    Event_PlaySound(SE_178);
    i = 0;
    Actor_Get(ACTOR_ALEX)->motionFlags = i;
    do {
      if (i == 16)
        Actor_SetAnimation(ACTOR_ALEX, 18);
      Actor_Get(ACTOR_ALEX)->y.fixed += px(3) / 20;
      FieldEvent_Wait(1);
      i++;
    } while (i != 64);
    Actor_RunRepeatedMotion(ACTOR_ALEX, 2);
    FieldEvent_Wait(5);
    // "How!? What's going on? I should be all-powerful! How can you defeat..."
    FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0, 5);
    i = 0;
    do {
      Actor_Get(ACTOR_ALEX)->y.fixed += px(1);
      FieldEvent_Wait(1);
      i++;
    } while (i != 32);
    Actor_Get(ACTOR_ALEX)->motionFlags = ACTOR_FOLLOWS_TERRAIN;
    i = 0;
    do {
      Actor_Get(ACTOR_ALEX)->y.fixed -= px(832) / 100;
      FieldEvent_Wait(1);
      i++;
    } while (i != 5);
    MtAlephSummit_ScatterDustRing(256, 16);
    Actor_SetAnimation(ACTOR_ALEX, 17);
    Event_PlaySound(SE_CRASH);
    MapGrid_SetShake(16, 16, 1);
    FieldEvent_Wait(10);
    MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
    BattleEffect_SetObjectUpdateMode(Actor_Get(ACTOR_WISE_ONE), 0);
    BattleEffect_WaitForArcGroups();
    BattleEffect_EndArcEffect();
    Event_PlaySound(SOUND_FADE_OUT_ACTION_SE);
    Actor_SetAnimation(ACTOR_WISE_ONE, 4);
    Field_StartPaletteTransition(32);
    FieldEvent_Wait(10);
    // "You are not all-powerful, Alex. Your power has its limits, as does..."
    FieldEvent_ShowMessageThenWait(ACTOR_WISE_ONE, 0, 5);
    Actor_RunRepeatedMotion(ACTOR_ALEX, 2);
    FieldEvent_Wait(5);
    // "That cannot be! Who is responsible for this treachery? Who has..."
    FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0, 5);
    Actor_SetAnimation(ACTOR_WISE_ONE, 3);
    // "I, the Wise One, imbued the Mars Star with some of the power of the..."
    FieldEvent_ShowMessageThenWait(ACTOR_WISE_ONE, 0, 5);
    Actor_SetAnimation(ACTOR_ALEX, 18);
    FieldEvent_Wait(10);
    // "Why?"
    FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0, 5);
    FieldEvent_Wait(120);
    Event_PlaySound(SE_TREMOR);
    i = 0;
  L14:
    MapGrid_SetShakeFixed(px(i) + px(1), px(i) + px(1), px(1));
    FieldEvent_Wait(8);
    i++;
    if (i != 2)
      goto L14;
    Actor_SetAnimation(ACTOR_WISE_ONE, 8);
    FieldEvent_Wait(60);
    Actor_SetAnimation(ACTOR_WISE_ONE, 7);
    // "The heavens and earth are changing, Alex! You must flee now!"
    FieldEvent_ShowMessageThenWait(ACTOR_WISE_ONE, 0, 5);
    Actor_ShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
    // "Wha-What!?"
    FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0, 5);
    Actor_SetAnimation(ACTOR_WISE_ONE, 8);
    // "Mt. Aleph will soon be drawn into the heart of the earth! You must..."
    FieldEvent_ShowMessageThenWait(ACTOR_WISE_ONE, 0, 5);
    for (i = 0, j = px(2); i != 4; i++, j += px(1)) {
      MapGrid_SetShakeFixed(j, j, px(1));
      FieldEvent_Wait(8);
    }
    Actor_RunRepeatedMotion(ACTOR_ALEX, 2);
    FieldEvent_Wait(5);
    // "Flee?! I can't flee! I can't even move!"
    FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0, 5);
    Actor_SetAnimation(ACTOR_WISE_ONE, 3);
    // "Ah, yes. You now see the limits of your power. If you are swallowed..."
    FieldEvent_ShowMessageThenWait(ACTOR_WISE_ONE, 0, 5);
    Actor_RunRepeatedMotion(ACTOR_ALEX, 2);
    FieldEvent_Wait(5);
    Actor_SetAnimation(ACTOR_WISE_ONE, 4);
    // "If you survive, perhaps we shall meet again someday..."
    FieldEvent_ShowMessageThenWait(ACTOR_WISE_ONE, 0, 5);
    FieldEvent_Wait(10);
    slots = (struct HeapSlots *)Ram_HeapSlots->mapGrid;
    MapGrid_SetShake(16, 16, 1);
    j = 0;
    scale = -Q16_ONE * 15 / 8;
  L16:
    if (j == 20)
      Actor_SetAnimation(ACTOR_WISE_ONE, 7);
    if (j >= 30 && j <= 46) {
      Actor_Get(ACTOR_WISE_ONE)->scaleX = Q16_ONE - scale;
      Actor_Get(ACTOR_WISE_ONE)->scaleY = scale + Q16_ONE;
      Actor_Get(ACTOR_WISE_ONE)->z.fixed -= px(2);
    }
    if (j == 47)
      Actor_SetPosition(ACTOR_WISE_ONE, 0, 0);
    if (j == 64) {
      PaletteFade_ApplyTarget(RGB(31, 31, 31), PALETTES_ALL);
      PaletteFade_Interpolate(64);
    }
    ((struct MapWork *)slots)->layers[1].offsetY -= px(1);
    Actor_Get(ACTOR_ALEX)->z.fixed += px(1);
    System_Wait(1);
    j++;
    scale += Q16_ONE / 16;
    if (j != 128)
      goto L16;
    Event_PlaySound(SOUND_FADE_OUT_BGM);
    FieldEvent_Wait(32);
    Event_PlaySound(SE_BANE_ESCAPE);
    FieldEvent_Wait(120);
    FieldTask_Remove(MtAlephSummit_PulseColors);
    PaletteFade_ApplyTarget(RGB(31, 31, 31), PALETTES_ALL);
    PaletteFade_Interpolate(1);
    System_Wait(1);
    Event_PlaySound(SOUND_FADE_OUT_ACTION_SE);
    PaletteFade_ApplyTarget(RGB(0, 0, 0), PALETTES_ALL);
    PaletteFade_Interpolate(120);
    System_Wait(300);
    Event_SetDestination(SCENE_EPILOGUE, 1);
  } else {
    gFieldWork->transition = 0;
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    for (j = 0; j != 320; j++) {
      held = &gInput.held;
      if (*held & KEY_B)
        j = 0;
      if (*held & KEY_A)
        break;
      if (j == 0) {
        Actor_SetSpriteFlags(Actor_Get(ACTOR_ALEX), 0);
        Actor_SetAnimation(ACTOR_ALEX, 12);
      }
    }
    Event_SetDestination(SCENE_STAFF_CREDIT, 0);
  }
  Heap_Release(buffer);
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

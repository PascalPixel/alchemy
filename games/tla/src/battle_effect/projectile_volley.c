#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "battle_effect_work.h"
#include "motobj.h"
#include "field_event.h"

#define Actor_SetAnimationSpeedFar                                             \
  ((s32 (*)(struct FieldActor *, s32))Actor_SetAnimationSpeedFar)

DEFINE_EFFECT_KIND(BattleEffect_RunSlash, BattleEffect_RunProjectileVolley, 0)

DEFINE_EFFECT_KIND(BattleEffect_RunShuriken, BattleEffect_RunProjectileVolley,
                   1)

DEFINE_EFFECT_KIND(BattleEffect_RunCrazyVoice, BattleEffect_RunProjectileVolley,
                   2)

DEFINE_EFFECT_KIND(BattleEffect_RunWingFlutter,
                   BattleEffect_RunProjectileVolley, 3)

DEFINE_EFFECT_KIND(BattleEffect_RunWingStroke, BattleEffect_RunProjectileVolley,
                   11)

DEFINE_EFFECT_KIND(BattleEffect_RunWingBeat, BattleEffect_RunProjectileVolley,
                   4)

void BattleEffect_RunCuttingEdge(struct BattleEffectArgument *effect) {
  struct FieldActor *object;

  object = (struct FieldActor *)GetBattleObjectSlotFar(effect->actor)->object;
  Actor_SetMode(object, 2);
  Actor_SetAnimationSpeedFar(object, 0x30);
  BattleEffect_RunProjectileVolley(effect, 5);
  Actor_SetAnimationSpeedFar(object, 0x10);
}

void BattleEffect_RunHeatWave(struct BattleEffectArgument *effect) {
  struct FieldActor *object;

  object = (struct FieldActor *)GetBattleObjectSlotFar(effect->actor)->object;
  Actor_SetMode(object, 2);
  Actor_SetAnimationSpeedFar(object, 0x30);
  BattleEffect_RunProjectileVolley(effect, 6);
  Actor_SetAnimationSpeedFar(object, 0x10);
}

DEFINE_EFFECT_KIND(BattleEffect_RunSleet, BattleEffect_RunProjectileVolley, 7)

void BattleEffect_RunBlizzard(struct BattleEffectArgument *effect) {
  struct FieldActor *object;

  object = (struct FieldActor *)GetBattleObjectSlotFar(effect->actor)->object;
  Actor_SetMode(object, 2);
  Actor_SetAnimationSpeedFar(object, 0x30);
  BattleEffect_RunProjectileVolley(effect, 9);
  Actor_SetAnimationSpeedFar(object, 0x10);
}

void BattleEffect_RunProjectileVolleyMode12WithAction(
    struct BattleEffectArgument *effect) {
  struct FieldActor *object;

  object = (struct FieldActor *)GetBattleObjectSlotFar(effect->actor)->object;
  Actor_SetMode(object, 2);
  Actor_SetAnimationSpeedFar(object, 0x30);
  BattleEffect_RunProjectileVolley(effect, 12);
  Actor_SetAnimationSpeedFar(object, 0x10);
}

void BattleEffect_RunProjectileVolleyMode13WithAction(
    struct BattleEffectArgument *effect) {
  struct FieldActor *object;

  object = (struct FieldActor *)GetBattleObjectSlotFar(effect->actor)->object;
  Actor_SetMode(object, 2);
  Actor_SetAnimationSpeedFar(object, 0x30);
  BattleEffect_RunProjectileVolley(effect, 13);
  Actor_SetAnimationSpeedFar(object, 0x10);
}

void BattleEffect_RunProjectileVolleyMode14WithAction(
    struct BattleEffectArgument *effect) {
  struct FieldActor *object;

  object = (struct FieldActor *)GetBattleObjectSlotFar(effect->actor)->object;
  Actor_SetMode(object, 2);
  Actor_SetAnimationSpeedFar(object, 0x30);
  BattleEffect_RunProjectileVolley(effect, 14);
  Actor_SetAnimationSpeedFar(object, 0x10);
}

DEFINE_EFFECT_KIND(BattleEffect_RunProjectileVolleyMode15,
                   BattleEffect_RunProjectileVolley, 15)

DEFINE_EFFECT_KIND(BattleEffect_RunProjectileVolleyMode16,
                   BattleEffect_RunProjectileVolley, 16)

DEFINE_EFFECT_KIND(BattleEffect_RunProjectileVolleyMode17,
                   BattleEffect_RunProjectileVolley, 17)

/// Kind 402: Ice Breath and Blue Dragon.
// Named, as The Broken Seal names it, for Ice Blessing.
DEFINE_EFFECT_KIND(BattleEffect_RunIceBlessing,
                   BattleEffect_RunProjectileVolley, 10)

DEFINE_EFFECT_KIND(BattleEffect_RunProjectileVolleyMode18,
                   BattleEffect_RunProjectileVolley, 18)

#include "battle_display.h"
#include "battle_effect.h"

#include "effect_step.h"
#include "io_reg.h"
#include "iwram_call.h"

#include "ram_buffer.h"
#include "task.h"

#define ABS(v) ((v) < 0 ? -(v) : (v))

// The particle buffer at the map cell buffer: flashes from 0, sparks from
// 100, smoke from 512, the falling-streak particles from 960.
#define PARTICLES ((struct EffectStep *)Ram_MapCellBuffer)
#define PTCL_SPARKS 100
#define PTCL_SMOKE 512
#define PTCL_STREAKS 960
#define PTCL_MAX 1024

// The pixel half of a 16.16 value.
#define HI(v) (((s16 *)&(v))[1])

// Directory rows, named by their assets.
extern char ResourceId_Data134; // the spark sprites
extern char ResourceId_CometSheetA;
extern char ResourceId_CometSheetB;
extern char ResourceId_CometSheetC;
extern char ResourceId_IceBlockSheet;
extern char ResourceId_WindStreakSheet;
extern char ResourceId_Data0FF;
extern char ResourceId_LimePalette;
extern char ResourceId_SmokeSheet;
extern char ResourceId_WaterSpraySheet;
extern char ResourceId_TornadoSheet;
extern char ResourceId_ShurikenSheet;
extern char ResourceId_ProjectileVolleyImage;
extern char ResourceId_YellowSparkSheet;
extern char ResourceId_FireballSheet;
extern char ResourceId_CrystalSheet;
extern char ResourceId_BlueBeamSheet;
extern char ResourceId_IceChipSheet;
extern char ResourceId_MercuryDjinnSmallSheet;

// The Broken Seal's gVolley_* tables, in effect_table.s.
extern u8 Data_08198475[];    // _CometHeights, by kind
extern u8 Data_08198478[][5]; // _Modes: five bytes for each mode
extern u8 Data_081984d7[];    // _FlashWidths
extern u8 Data_081984de[];    // _FlashHeights
extern u8 Data_081984e5[];    // _FlashSteps
extern u8 Data_081984ec[];    // _FlashRises
extern u16 Data_081984f4[];   // _FlashOffsets
extern u16 Data_08198502[];   // _PelletOffsets
extern u8 Data_0819850a[];    // the glints' palettes
// gBattleEffect_MoteCells; gBattleEffect_GlintCell* (offsets, widths,
// heights); gPrism_ShardOffsets and the shards' widths and heights.
extern u16 gData_08197410[];
// The glint's six cels: ☀️'s gBattleEffect_GlintCells, _GlintCellWidths
// and _GlintCellHeights.
extern u16 gData_08197486[];
extern u8 gData_08197492[];
extern u8 gData_08197498[];
extern s32 gData_08197834[];
extern u8 gData_0819781a[];
extern u8 gData_08197826[];

// The five bytes of a mode in Data_08198478.
#define VOLLEY_SHOTS 0   // shots at each target
#define VOLLEY_STEPS 1   // frames a shot takes to reach its target
#define VOLLEY_STAGGER 2 // frames between shots
#define VOLLEY_DELAY 3   // frames between targets
#define VOLLEY_SPARKS 4  // sparks on impact

void BattleEffect_StartCanvasLayer(s32 control);
/// Closes the canvas layer.
void BattleEffect_EndCanvasLayer(void);
// Prepares the canvas around an anchor and returns its screen point.
void Func_0815585c(struct BattleEffectArgument *effect, u32 packed, s32 *screen,
                   s32 *world);
// Builds a blitter in a heap slot: BattleEffect_BuildBlitter(slot, 7, 7,
// flags, mode), whose result it returns.
s32 BattleEffect_LoadBlitter(s32 slot, s32 modeFlags);
// BattleEffect_BuildBlitter itself.
s32 Func_08196404(s32 slot, s32 widthShift, s32 heightShift, s32 flags,
                  u32 mode);

void *Resource_GetTableEntry(s32 resourceId);
/// BattlePlayback_Start through its far call.
void Func_081180e8(s32 phase);
s32 Trig_Sin(s32 angle);
s32 Trig_Cos(s32 angle);
/// Flashes an actor in a palette for some frames.
void Func_0814cd48(s32 actor, s32 palette, s32 frames, s32 timerSlot,
                   s32 timerFrames);
/// An actor's recoil motion, through a far call.
void Func_08118088(s32 actor, s32 motion);
/// Shakes the camera while work->shakeFrames lasts.
void BattleCamera_Shake(s32 rangeX, u32 rangeY);
/// Counts down the actors' flash timers.
void BattleEffect_CountDownBattlerTints(void);
s32 BattleUnit_GetEffectHeightFar(s32 unit);
void Transform_SetLookAt(s32 *eye, s32 *target);
void EffectPosition_ProjectBattler(s32 id, struct EffectPosition *pos);
void BattleEffectKernels_IntegrateVector2(struct EffectStep *step, s32 damping,
                                          s32 gravity);

// BattleEffect_LoadBlitter(slot, mode << 4 | flags) loads a blitter into a heap
// slot.
#define BLITTER_ADDITIVE (0 << 4)
#define BLITTER_MAX (1 << 4)
#define CLIP (COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y)

// The second blitter, loaded again for each draw that needs its own.
#define DRAW_ALTERNATE ((DrawRectangle)Ram_HeapSlots->unkb8[1])

// Frees the second blitter after a draw.
#define RELEASE_DRAW_BLITTER()                                                 \
  do {                                                                         \
    HeapSlot_Release(188);                                                     \
  } while (0)

#define CUE_FIRE 136
#define CUE_IMPACT 132
#define PHASE_RESULTS 133 // Func_081180e8: the results phase begins
#define PHASE_LATE_RESULTS 134

void BattleEffect_RunProjectileVolley(struct BattleEffectArgument *effect,
                                      s32 mode) {
  struct EffectPosition screen;
  struct EffectPosition world;
  struct EffectPosition origin;
  struct EffectPosition position;
  struct BattleEffectWork *work;
  void *canvas;
  s32 i;
  s32 j;
  s32 frame;
  DrawRectangle draw[2];
  struct BattleCamera *camera;
  u8 *sheet;
  s32 seWait;
  s32 total;
  s32 kind;
  s32 shots;
  s32 steps;
  struct MotionObject *user;
  s32 k;
  // The smoke puffs placed, and later the sparks' speed shift, as the twin.
  s32 shift;
  s32 x;

  // Set up: the work, canvas, camera and sprite sheet, the canvas layer and
  // the Djinn's place when it circles first.
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  camera = Ram_HeapSlots->cameraWork;
  sheet = Ram_HeapSlots->unk64[0];
  seWait = 0;
  kind = effect->variant;
  BattleEffect_StartCanvasLayer(1);
  if (effect->prelude == 1)
    Func_0815585c(effect, 1 | effect->side << 4 | 2 << 5, &screen, &world);
  REG_BLDALPHA = 0x1010;

  // A 128x128 blitter, mirrored for the other side.
  if (mode == 5 || mode == 12 || mode == 13) {
    if (effect->side == 1)
      BattleEffect_LoadBlitter(104, BLITTER_MAX | CLIP | COMPOSITE_FLIP_X);
    else
      BattleEffect_LoadBlitter(104, BLITTER_MAX | CLIP);
  } else {
    if (effect->side == 1)
      BattleEffect_LoadBlitter(104, BLITTER_ADDITIVE | CLIP | COMPOSITE_FLIP_X);
    else
      BattleEffect_LoadBlitter(104, BLITTER_ADDITIVE | CLIP);
  }
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];

  // The shot pictures.
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, sheet, 0, 0);
  if (mode == 0 || mode == 5 || mode == 8) {
    if (mode == 5)
      kind = 2;
    if (mode == 8)
      kind = 0;
    if (kind == 0)
      Resource_LoadAndDecompress((s32)&ResourceId_CometSheetA,
                                 work->sheet + 0x1000, 1, 1);
    else if (kind == 1)
      Resource_LoadAndDecompress((s32)&ResourceId_CometSheetB,
                                 work->sheet + 0x1000, 1, 1);
    else
      Resource_LoadAndDecompress((s32)&ResourceId_CometSheetC,
                                 work->sheet + 0x1000, 1, 1);
    if (mode == 5) {
      void *palette = Resource_GetTableEntry((s32)&ResourceId_IceBlockSheet);
      s32 (*copy)(void *, const void *, s32) = Iwram_CopyWords;

      copy((void *)BG_PLTT, palette, 128);
    }
    Resource_LoadAndDecompress((s32)&ResourceId_WindStreakSheet,
                               work->sheet + 0x2000, 1, 0);
    if (mode == 5) {
      void *palette = Resource_GetTableEntry((s32)&ResourceId_IceBlockSheet);
      s32 (*copy)(void *, const void *, s32) = Iwram_CopyWords;

      copy((void *)BG_PLTT, palette, 128);
    }
    work->transferMode = 2;
    work->transferValue = 75;
  } else if (mode >= 15 && mode <= 17) {
    for (i = 0; i != 8; i++)
      work->sheet[i] = 60;
    Resource_LoadAndDecompress((s32)&ResourceId_Data0FF, work->sheet, 1, 1);
    work->transferMode = 1;
    work->transferValue = 0;
    if (mode == 15) {
      REG_BLDCNT = 0;
    } else if (mode == 16) {
      Iwram_CopyWords((void *)BG_PLTT,
                      Resource_GetTableEntry((s32)&ResourceId_LimePalette),
                      128);
      Resource_LoadAndDecompress((s32)&ResourceId_SmokeSheet,
                                 work->sheet + 0x22, 1, 0);
    } else {
      Iwram_CopyWords((void *)BG_PLTT,
                      Resource_GetTableEntry((s32)&ResourceId_WaterSpraySheet),
                      128);
      Resource_LoadAndDecompress((s32)&ResourceId_TornadoSheet,
                                 work->sheet + 0x22, 1, 0);
    }
  } else if (mode == 1) {
    Resource_LoadAndDecompress((s32)&ResourceId_ShurikenSheet, work->sheet, 1,
                               1);
    REG_BLDCNT = 0;
    work->transferMode = 1;
    work->transferValue = 0;
  } else if (mode == 2) {
    Iwram_CopyWords((void *)BG_PLTT,
                    Resource_GetTableEntry((s32)&ResourceId_CometSheetA), 128);
    Resource_LoadAndDecompress((s32)&ResourceId_ProjectileVolleyImage,
                               work->sheet, 0, 0);
    work->transferMode = 2;
    work->transferValue = 50;
  } else if (mode == 3 || mode == 4 || mode == 11) {
    Resource_LoadAndDecompress((s32)&ResourceId_YellowSparkSheet, work->sheet,
                               1, 1);
    work->transferMode = 2;
    work->transferValue = 50;
  } else if (mode == 6 || mode == 12) {
    Resource_LoadAndDecompress((s32)&ResourceId_FireballSheet, work->sheet, 1,
                               1);
    work->transferMode = 2;
    work->transferValue = 50;
  } else if (mode == 14) {
    Resource_LoadAndDecompress((s32)&ResourceId_CrystalSheet, work->sheet, 1,
                               1);
    work->transferMode = 1;
    work->transferValue = 0;
  } else if (mode == 13) {
    // The beam is unpacked once and then halved, every second byte, into
    // place at 0x1000.
    u8 *beam;

    Resource_LoadAndDecompress((s32)&ResourceId_BlueBeamSheet,
                               beam = work->sheet, 1, 1);
    for (i = 0; i != 0x5b2; i++) {
      work->sheet[0x1000 + i] = *beam;
      beam += 2;
    }
    work->transferMode = 2;
    work->transferValue = 50;
  } else {
    Resource_LoadAndDecompress((s32)&ResourceId_IceChipSheet, work->sheet, 1,
                               1);
    Resource_LoadAndDecompress((s32)&ResourceId_MercuryDjinnSmallSheet,
                               work->sheet + 0x65c0, 1, 0);
    work->transferMode = 2;
    work->transferValue = 50;
  }
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);

  // Where each shot starts and how fast it flies.
  user = GetBattleObjectSlotFar(effect->actor)->object;
  shots = Data_08198478[mode][VOLLEY_SHOTS];
  steps = Data_08198478[mode][VOLLEY_STEPS];
  {
    s32 stagger = Data_08198478[mode][VOLLEY_STAGGER];
    s32 delay = Data_08198478[mode][VOLLEY_DELAY];
    s32 height = BattleUnit_GetEffectHeightFar(effect->actor);

    if (shots * effect->count > 63) // the shots must fit particles[64]
      effect->count = 1;

    for (i = 0; i != effect->count; i++) {
      struct MotionObject *target =
          GetBattleObjectSlotFar(effect->actors[i])->object;
      s32 targetHeight = BattleUnit_GetEffectHeightFar(effect->actors[i]);

      for (j = 0; j != shots; j++) {
        struct EffectStep *shot = &work->particles[i * shots + j];

        if (mode >= 12 && mode <= 14)
          shot->x = user->x * 2;
        else
          shot->x = user->x;
        if (mode == 7)
          shot->y = user->y + ((Random16() & 15) << 16) + 0x3a0000;
        else if (mode == 10)
          shot->y = user->y + height / 2;
        else if (mode == 18)
          shot->y = user->y + 0x320000;
        else if (mode == 6)
          shot->y = user->y + height / 2;
        else if (mode >= 12 && mode <= 14)
          shot->y = user->y + height / 2 + ((Random16() % 80 - 16) << 16);
        else if (mode == 9)
          shot->y = user->y + height / 2 + ((16 - (Random16() & 31)) << 16);
        else if (mode == 3 || mode == 4)
          shot->y = user->y + height / 2 + ((16 - (Random16() & 31)) << 16);
        else if (mode == 11)
          shot->y = user->y + height / 2 + ((32 - (Random16() & 63)) << 16);
        else if (mode == 5)
          shot->y = user->y + height / 2;
        else if (mode >= 15 && mode <= 17)
          shot->y = user->y + ((Random16() % 24) << 16);
        else
          shot->y = user->y + height;
        shot->z = user->z;
        shot->velocityX = (target->x - shot->x) / steps;
        shot->velocityZ = (target->z - shot->z) / steps;
        if (mode == 7) {
          shot->velocityY =
              (s32)(target->y + ((Random16() & 63) << 16) - shot->y - 0xc0000) /
              steps;
        } else if (mode == 8) {
          shot->velocityY =
              (s32)(target->y + ((Random16() & 7) << 16) - shot->y + 0x160000) /
              steps;
        } else if (mode == 14) {
          shot->velocityY =
              (s32)(target->y + ((64 - (Random16() & 63)) << 16) - shot->y) /
              steps;
        } else if (mode == 9) {
          shot->velocityY =
              (s32)(target->y + ((64 - (Random16() & 63)) << 16) - shot->y) /
              steps;
        } else if (mode >= 15 && mode <= 17) {
          shot->velocityY = shot->y / 32;
        } else if (mode == 10) {
          // These start two steps along their flight.
          shot->velocityY =
              (s32)(target->y + ((Random16() & 31) << 16) - shot->y + 0x40000) /
              steps;
          shot->x += shot->velocityX * 2;
          shot->y += shot->velocityY * 2;
          shot->z += shot->velocityZ * 2;
        } else if (mode == 18) {
          // As mode 10, tested apart as the native does.
          shot->velocityY =
              (s32)(target->y + ((Random16() & 31) << 16) - shot->y + 0x40000) /
              steps;
          shot->x += shot->velocityX * 2;
          shot->y += shot->velocityY * 2;
          shot->z += shot->velocityZ * 2;
        } else if (mode == 3 || mode == 4 || mode == 11 || mode == 5 ||
                   mode == 12) {
          shot->velocityY = 0;
        } else if (mode == 6) {
          shot->velocityY = (s32)(target->y + targetHeight / 2 +
                                  ((Random16() & 15) << 16) - shot->y) /
                            steps;
        } else {
          shot->velocityY = (s32)(target->y + targetHeight -
                                  ((Random16() & 15) << 16) - shot->y) /
                            steps;
        }
        shot->variant = 0;
      }
    }
    for (i = 0; i != PTCL_MAX; i++)
      PARTICLES[i].variant = 0;

    // The falling-streak particles start on the first target, held.
    EffectPosition_ProjectBattler(effect->actors[0], &origin);
    for (i = 0; i != 64; i++) {
      struct EffectStep *p = &PARTICLES[PTCL_STREAKS + i];

      p->x = (origin.x / 2) << 16;
      p->y = 80 << 16;
      p->z = 0;
      p->velocityX = ((Random16() & 255) - 128) << 9;
      p->velocityY = ((Random16() & 255) - 128) << 9;
      p->velocityZ = 0;
      p->variant = -1;
    }

    if (mode == 6 || mode == 7)
      total = (effect->count - 1) * delay + shots * stagger + 52;
    else if (mode == 2 || mode == 12 || mode == 13 || mode == 14 || mode == 16)
      total = (effect->count - 1) * delay + shots * stagger + 48;
    else
      total = (effect->count - 1) * delay + shots * stagger + 32;

    // Each frame: the cues, the Djinn, the shots and their impacts, then
    // the flashes, sparks, smoke and streaks.
    for (frame = 0; frame != total; frame++) {
      s32 index;

      if (seWait > 0)
        seWait--;
      if (mode == 6 || mode == 12 || mode == 13 || mode == 14) {
        if (frame == 4)
          Sound_PlayFar(CUE_FIRE);
        if (frame == 32)
          Func_081180e8(PHASE_LATE_RESULTS);
      } else if (mode == 2) {
        if (frame == 32)
          Func_081180e8(PHASE_RESULTS);
      } else if (mode == 7) {
        if (frame == 48)
          Func_081180e8(PHASE_RESULTS);
      } else if (mode != 5) {
        if (frame == 16)
          Func_081180e8(PHASE_RESULTS);
      }

      // The prelude: a small Mercury Djinn circles, then from frame 70
      // flies off upwards.
      if (effect->prelude == 1) {
        s32 djinnX = (-Trig_Sin(frame << 11) << 2 >> 16) + screen.x / 2 - 10;
        s32 djinnY = (Trig_Cos(frame << 11) << 1 >> 16) + screen.y - 24;

        if (frame > 69)
          djinnY = djinnY - frame * 2 + 138;
        if (effect->side == 1)
          BattleEffect_LoadBlitter(188,
                                   BLITTER_ADDITIVE | CLIP | COMPOSITE_FLIP_X);
        else
          BattleEffect_LoadBlitter(188, BLITTER_ADDITIVE | CLIP);
        draw[1] = DRAW_ALTERNATE;
        if (frame <= 3)
          draw[1](canvas, work->sheet + 0x65c0, djinnX, djinnY, 20, 40);
        draw[1](canvas, work->sheet + 0x65c0, djinnX, djinnY, 20, 40);
        RELEASE_DRAW_BLITTER();
      }

      // The shots.
      Render_ResetTransformState();
      Transform_SetLookAt(camera->eye, camera->pos);
      index = 0;
      for (i = 0; i != effect->count; i++) {
        struct MotionObject *target =
            GetBattleObjectSlotFar(effect->actors[i])->object;

        for (j = 0; j != shots; j++, index++) {
          struct EffectStep *shot;
          struct EffectPosition *pos;
          s32 targetDelay = i * delay;

          if (j * stagger + targetDelay >= frame)
            continue;
          shot = &work->particles[i * shots + j];
          if (mode >= 15 && mode <= 17 && shot->variant == 1)
            continue;

          // The screen position (the canvas is half as wide).
          pos = &position;
          EffectPosition_ProjectPoint((s32 *)shot, pos);
          pos->x >>= 1;
          shot->x += shot->velocityX;
          shot->y += shot->velocityY;
          shot->z += shot->velocityZ;
          if (mode == 6 || mode == 12) {
            // Two puffs of smoke behind the fireball.
            shift = 0;
            for (k = PTCL_SMOKE; k != PTCL_STREAKS; k++) {
              struct EffectStep *p = &PARTICLES[k];

              if (p->variant == 0) {
                p->x = position.x << 16;
                p->y = position.y << 16;
                p->velocityX = ((Random16() & 255) - 128) << 11;
                p->velocityY = ((Random16() & 255) - 128) << 11;
                p->variant = (Random16() & 7) + 32;
                shift++;
                if (shift == 2)
                  break;
              }
            }
          }
          if (shot->variant == 0) {
            s32 hit;

            // The shot hits when it passes the target's x on its side.
            x = shot->x;
            if (target->x < 0) {
              hit = x < 0;
              /* FAKEMATCH(dead): the twin's cancelling updates under a test
               * that emits nothing; as no updates, 35 of 2278 instructions
               * differ in tla-en. */
              if (target) {
                hit ^= x;
                hit ^= x;
              }
              if (!hit)
                goto draw_shot;
            } else {
              hit = x >= 0;
              /* FAKEMATCH(dead): the same for the other side; as no updates,
               * 28 of 2278 instructions differ in tla-en. */
              if (target) {
                hit ^= x;
                hit ^= x;
              }
              if (!hit)
                goto draw_shot;
            }
            if (ABS(x) >= ABS(target->x)) {
              s32 spawned = 0;
              struct EffectStep *flash = &PARTICLES[index];

              shot->variant = 1;
              if (mode == 5) {
                Func_081180e8(PHASE_LATE_RESULTS);
              } else if (mode != 6 && mode != 12 && mode != 13 && mode != 14 &&
                         seWait == 0) {
                seWait = 8;
                Sound_PlayFar(CUE_IMPACT);
              }
              if (mode == 2) {
                // The pellet bounces off.
                s32 kick = ((Random16() & 31) + 32) << 12;

                if (Random16() & 1)
                  shot->velocityY += kick;
                else
                  shot->velocityY -= kick;
                shot->velocityX = -shot->velocityX;
              }
              flash->variant = 1;
              flash->x = pos->x;
              flash->y = pos->y;
              flash->z = 0;
              if (mode != 7)
                work->shakeFrames = 2;
              Func_0814cd48(effect->actors[i], 7, 5, i, 8);
              if (mode != 7 && mode != 9 && mode != 10 && mode != 18 &&
                  mode != 14 && mode != 15 && mode != 16 && mode != 17) {
                if (mode == 5)
                  Func_08118088(effect->actors[i], 4);
                else
                  Func_08118088(effect->actors[i], 5);
              }
              if (mode == 16 || mode == 17) {
                // The streak lets its particle loose.
                s32 n;
                s32 held;

                for (n = 0, held = 0; n != 1; n++) {
                  /* FAKEMATCH(barrier): the asm operand keeps held a register
                   * the loop carries, so it is allocated r2 before n; as no
                   * asm, 12 of 2278 instructions differ in tla-en. */
                  __asm__("" : "+r"(held));
                  PARTICLES[PTCL_STREAKS + j + n].variant = held;
                }
              } else if (mode != 15) {
                // Sparks.
                shift = mode == 5 ? 12 : 11;
                for (k = PTCL_SPARKS; k != PTCL_SMOKE; k++) {
                  struct EffectStep *p = &PARTICLES[k];

                  if (p->variant == 0) {
                    p->x = position.x << 16;
                    p->y = position.y << 16;
                    p->velocityX = ((Random16() & 255) - 128) << shift;
                    p->velocityY = ((Random16() & 255) - 128) << shift;
                    p->variant = (Random16() & 7) + 16;
                    spawned++;
                    if (spawned == Data_08198478[mode][VOLLEY_SPARKS])
                      break;
                  }
                }
              }
            }
          }
        draw_shot:
          if (mode == 0 || mode == 5 || mode == 8) {
            // Only the comet's upper half is drawn; the lower half is the
            // same picture flipped.
            if (effect->side == 1)
              BattleEffect_LoadBlitter(188,
                                       BLITTER_MAX | CLIP | COMPOSITE_FLIP_X);
            else
              BattleEffect_LoadBlitter(188, BLITTER_MAX | CLIP);
            DRAW_ALTERNATE(canvas, work->sheet + 0x1000, pos->x - 16,
                           pos->y - Data_08198475[kind], 32,
                           Data_08198475[kind]);
            HeapSlot_Release(188);
            if (effect->side == 1)
              BattleEffect_LoadBlitter(188,
                                       BLITTER_MAX | CLIP | COMPOSITE_FLIP);
            else
              BattleEffect_LoadBlitter(188,
                                       BLITTER_MAX | CLIP | COMPOSITE_FLIP_Y);
            DRAW_ALTERNATE(canvas, work->sheet + 0x1000, pos->x - 16, pos->y,
                           32, Data_08198475[kind]);
            RELEASE_DRAW_BLITTER();
          } else if (mode == 1) {
            if (ABS(shot->x) <= ABS(target->x))
              draw[0](canvas, work->sheet + frame % 6 * 768, pos->x - 16,
                      pos->y - 12, 32, 24);
          } else if (mode == 7 || mode == 9 || mode == 10 || mode == 18) {
            draw[0](canvas, work->sheet + Data_08198502[j & 3], pos->x - 4,
                    pos->y - 4, 8, 8);
          } else if (mode == 2) {
            draw[0](canvas, work->sheet + j % 6 * 128, pos->x - 4, pos->y - 8,
                    8, 16);
          } else if (mode == 3) {
            draw[0](canvas, work->sheet + 96, pos->x - 9, pos->y - 7, 18, 13);
          } else if (mode == 4) {
            draw[0](canvas, work->sheet, pos->x - 6, pos->y - 4, 12, 8);
          } else if (mode >= 15 && mode <= 17) {
            draw[0](canvas, work->sheet, pos->x - 8, pos->y - 1, 17, 2);
          } else if (mode == 13) {
            draw[0](canvas, work->sheet, pos->x - 12, pos->y - 12, 24, 24);
          } else if (mode == 14) {
            // Five crystals, each turning through three cels.
            s32 cel = (i * shots + j) % 5 * 3 + frame / 4 % 3;
            u32 width;
            u32 height;
            u8 *picture = work->sheet + gData_08197834[cel] + 0x800;
            s32 left;
            s32 top;

            width = gData_0819781a[cel];
            left = pos->x - (width >> 1);
            height = gData_08197826[cel];
            top = pos->y - (height >> 1);
            draw[0](canvas, picture, left, top, width, height);
          } else if (mode == 11) {
            draw[0](canvas, work->sheet + 330, pos->x - 15, pos->y - 12, 29,
                    23);
          } else {
            draw[0](canvas, work->sheet, pos->x - 20, pos->y - 32, 40, 64);
          }
        }
      }

      // The comets' flashes, 12 frames: half the picture, then the same
      // half flipped (odd flashes use the picture turned on its side).
      if (mode == 0 || mode == 5 || mode == 8) {
        for (i = 0; i != effect->count * shots; i++) {
          struct EffectStep *flash = &PARTICLES[i];

          if (flash->variant == 1) {
            s32 step = flash->z / 2;

            if (!(i & 1)) {
              BattleEffect_LoadBlitter(188, BLITTER_MAX | CLIP);
              DRAW_ALTERNATE(canvas, work->sheet + Data_081984f4[step],
                             flash->x - Data_081984d7[step],
                             flash->y - Data_081984ec[step],
                             Data_081984d7[step], Data_081984de[step]);
              /* FAKEMATCH(dead): an empty once-only block before the release;
               * as no block, 68 of 2278 instructions differ in tla-en. */
              do {
              } while (0);
              HeapSlot_Release(188);
              BattleEffect_LoadBlitter(188,
                                       BLITTER_MAX | CLIP | COMPOSITE_FLIP);
              DRAW_ALTERNATE(
                  canvas, work->sheet + Data_081984f4[step], flash->x,
                  flash->y + Data_081984e5[step] - Data_081984ec[step],
                  Data_081984d7[step], Data_081984de[step]);
              RELEASE_DRAW_BLITTER();
            } else {
              BattleEffect_LoadBlitter(188, BLITTER_MAX | CLIP);
              {
                s32 offset = Data_081984f4[step];
                u8 *picture;
                s32 rise;
                s32 left;
                s32 width;

                // The picture, x and rise are read in a once-only block, after
                // the offset and before the width (otherwise 2 to 4 of 2278
                // differ, 2026-11-10).
                do {
                  picture = work->sheet + offset + 0x128a;
                  left = flash->x;
                  rise = Data_081984ec[step];
                } while (0);
                width = Data_081984d7[step];
                DRAW_ALTERNATE(canvas, picture, left - rise, flash->y - width,
                               Data_081984de[step], width);
              }
              do {
              } while (0);
              HeapSlot_Release(188);
              BattleEffect_LoadBlitter(188,
                                       BLITTER_MAX | CLIP | COMPOSITE_FLIP);
              DRAW_ALTERNATE(
                  canvas, work->sheet + Data_081984f4[step] + 0x128a,
                  flash->x + Data_081984e5[step] - Data_081984ec[step],
                  flash->y, Data_081984de[step], Data_081984d7[step]);
              RELEASE_DRAW_BLITTER();
            }
            flash->z++;
            if (flash->z == 12)
              flash->variant = 0;
          }
        }
      }

      // Sparks fall and bounce at y = 112 (the beam's float up).
      for (i = PTCL_SPARKS; i != PTCL_SMOKE; i++) {
        struct EffectStep *p = &PARTICLES[i];

        if (p->variant > 0) {
          s32 size = (p->variant >> 3) + 1;

          draw[0](canvas, sheet + gData_08197410[size - 1], HI(p->x) - size / 2,
                  HI(p->y) - size, size, size * 2);
          if (mode == 13)
            BattleEffectKernels_IntegrateVector2(p, 60, 0x4000);
          else
            BattleEffectKernels_IntegrateVector2(p, 60, 0x1000);
          if (p->y > 0x700000)
            p->velocityY = -p->velocityY / 2;
          p->variant--;
        }
      }

      // Smoke rises.
      for (i = PTCL_SMOKE; i != PTCL_STREAKS; i++) {
        struct EffectStep *p = &PARTICLES[i];

        if (p->variant > 0) {
          s32 size = (p->variant >> 4) + 1;

          draw[0](canvas, sheet + gData_08197410[size - 1], HI(p->x) - size / 2,
                  HI(p->y) - size, size, size * 2);
          BattleEffectKernels_IntegrateVector2(p, 60, -0x4000);
          p->variant--;
        }
      }

      if (mode == 16) {
        // Smoke billows up where each streak landed: six cels, four frames
        // each.
        for (i = 0; i != 64; i++) {
          struct EffectStep *p = &PARTICLES[PTCL_STREAKS + i];

          if ((u32)p->variant <= 23) {
            draw[0](canvas, work->sheet + 0x22 + p->variant / 4 * 1152,
                    HI(p->x) - 12, HI(p->y) - 24, 24, 48);
            BattleEffectKernels_IntegrateVector2(p, 62, -0x400);
            p->variant++;
          }
        }
      } else if (mode == 17) {
        // Glints of water spray scatter about each landed streak, each with
        // a random palette.
        for (i = 0; i != 64; i++) {
          struct EffectStep *p = &PARTICLES[PTCL_STREAKS + i];

          if ((u32)p->variant <= 7) {
            s32 glint = frame & 3;

            s32 angle = Random16() & 0xffff;
            s32 radius = (Random16() & 31) + 4;
            s32 glintX = HI(p->x) + ((Trig_Sin(angle) * radius) >> 17) -
                         (gData_08197492[glint] >> 1);
            s32 glintY = HI(p->y) - ((Trig_Cos(angle) * radius) >> 17) -
                         (gData_08197498[glint] >> 1);

            Func_08196404(188, 7, 7, Data_0819850a[Random16() & 3] | CLIP, 0);
            do {
              // The empty once-only loop keeps loop.c from hoisting the glint
              // size tables (without it, 1901 of 2278 differ, 2026-11-08).
            } while (0);
            DRAW_ALTERNATE(canvas, work->sheet + 0x22 + gData_08197486[glint],
                           glintX, glintY, gData_08197492[glint],
                           gData_08197498[glint]);
            HeapSlot_Release(188);
            p->variant++;
          }
        }
      }
      BattleCamera_Shake(4, 4);
      BattleEffect_CountDownBattlerTints();
      work->transferPending = 1;
      wait(1);
    }
  }

  // Tear down: stop the transfer, free the blitter, close the layer.
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(104);
  BattleEffect_EndCanvasLayer();
}

#include "b5_context.h"

#include "actor.h"

void BattleEffect_InitializeRisingObject(s32 slot) {
  struct BattleObj *context = GetBattleObjectSlotFar(slot);
  struct Actor *object = context->object;

  object->acceleration = 0x20000;
  object->speedLimit = 0x80000;
  object->velocityY = 0x40000;
  object->gravity = 0xAB85;
  object->bounce = 0;
  object->actionFlags = 0;
  object->snapToTarget = 1;
  Actor_ResetMotion(object);
  Actor_SetTargetPosition(object, context->anchorX, 0, context->anchorZ);
  Actor_SetMode(object, 1);
}

/// Moves the object rate percent of the way to the target's object in the
/// given frames.
// The Broken Seal's BattleEffect_SetApproachMotion always moves 80 percent.
void BattleEffect_SetApproachMotion(s32 id, s32 targetId, s32 frames,
                                    s32 rate) {
  struct BattleObj *slot = GetBattleObjectSlotFar(id);
  struct BattleObj *targetSlot = GetBattleObjectSlotFar(targetId);
  struct Actor *obj = (struct Actor *)slot->object;
  struct Actor *target = (struct Actor *)targetSlot->object;
  s32 diffX = target->x - obj->x;
  s32 startX = obj->x;
  s32 dx = rate * diffX / 100;
  s32 diffZ = target->z - obj->z;
  s32 startZ = obj->z;
  s32 dz = rate * diffZ / 100;
  s32 x = startX + dx;
  s32 z = startZ + dz;
  s32 sx = dx >> 8; // the distance in 1/256 pixels
  s32 sz = dz >> 8;
  s32 dist;

  dist = Iwram_Sqrt(sx * sx + sz * sz);
  dist = (dist << 8) / frames;
  obj->acceleration = dist;
  obj->speedLimit = dist;
  obj->snapToTarget = 1;
  obj->gravity = 0xab85;
  obj->velocityY = 0;
  obj->bounce = 0;
  obj->actionFlags = 1;
  Actor_ResetMotion(obj);
  Actor_SetTargetPosition(obj, x, 0, z);
  Actor_SetMode(obj, 2);
}

// Tiles: copy a byte image rotated 90 degrees clockwise.

/// Rotates w-by-h src clockwise into h-by-w dst.
/// The top row of src becomes the rightmost column of dst.
void Graphics_TransposeCopy(const u8 *src, u8 *dst, s32 w, s32 h) {
  s32 y;
  s32 x;
  s32 col;
  s32 offset;
  u8 *d;
  const u8 *s;

  y = 0;
  if (h != 0) {
    offset = 0;
    col = h;
    do {
      x = 0;
      if (w != 0) {
        d = (u8 *)(col + (u32)dst - 1);
        s = (const u8 *)(offset + (u32)src);
        do {
          x++;
          *d = *s;
          s++;
          d += h;
        } while (x != w);
      }
      y++;
      offset += w;
      col--;
    } while (y != h);
  }
}

// The Mercury Djinn's flames (effect kind 281).

#include "fixed_math.h"

// Byte offsets of the two blitter slots in gHeapSlots (unk64[1], unkb8[1]).
#define HEAP_BLITTER 104
#define HEAP_BLITTER_ALTERNATE 188

// The dots live in the map cell buffer: 32 for each of the five flames.
#define DOTS ((struct EffectStep *)Ram_MapCellBuffer)
#define DOTS_PER_FLAME 32
#define FLAME_COUNT 5

extern char ResourceId_BlueFlameSheet;
/// The frames the Djinn throws its five flames on (☀️'s
/// gDjinnAttacks_LaunchFrames).
extern const u8 Data_0819850e[];

/// Fetches the max and additive blitters, flipped or not.
void BattleEffect_FetchRectangleBlitters(s32 flip, void **out);
/// A small Mercury Djinn circles over the targets, one turn every 32 frames, on
/// a radius of 64 - 2 x frame pixels (half that across) that shrinks through
/// the middle at frame 32 and opens out on the other side. At frames 4, 7, 10,
/// 21 and 34 it throws a blue flame that shoots left 12 pixels a frame and
/// shatters on its fifth frame into 32 dots, with a shake; six frames after
/// each throw every target flashes and recoils. The dots shrink with age and
/// fall back under gravity. The effect runs 96 frames; at frame 56 the battle
/// resumes and resolves the targets (hit cue 133).
void BattleEffect_RunMercury(struct BattleEffectArgument *effect) {
  struct HeapSlots *slots;
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  u8 *sheet;
  DrawRectangle blit[2];
  DrawRectangle *draw;
  struct EffectStep *flame;
  struct EffectStep *dot;
  u8 *cells;
  s32 i;
  s32 j;

  // Setup: the canvas, the blitter pair, the sheets.
  slots = Ram_HeapSlots;
  work = (struct BattleEffectWork *)slots->battleFxWork[0];
  canvas = slots->battleFxWork[1];
  sheet = slots->unk64[0];
  BattleEffect_StartCanvasLayer(0);
  draw = blit;
  BattleEffect_FetchRectangleBlitters(0, draw);

  // The flame picture is the blue flame sheet turned on its side.
  cells = Ram_MapCellBuffer;
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, sheet, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_MercuryDjinnSmallSheet,
                             work->sheet, 1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_BlueFlameSheet, cells, 1, 1);
  Graphics_TransposeCopy(cells, work->sheet + 0x2a8, 17, 104);
  Graphics_TransposeCopy(cells + 0x6e8, work->sheet + 0x990, 34, 65);
  // Each transfer leaves 3/4 of the last frame behind as an afterimage.
  work->transferMode = 2;
  work->transferValue = 75;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);

  // No flame and no dot is alive yet.
  for (i = 0; i != 8; i++)
    work->particles[i].variant = -1;
  for (i = 0; i != 512; i++)
    DOTS[i].variant = -1;

  Sound_PlayFar(162);
  frame = 0;
  do {
    // The battle resumes and resolves the targets, hit cue 133.
    if (frame == 56)
      Func_081180e8(133);

    // Flames fly 12 pixels left a frame and shatter on their fifth frame.
    for (i = 0; i != FLAME_COUNT; i++) {
      flame = &work->particles[i];
      if (flame->variant != -1) {
        blit[0](canvas, work->sheet + 0x990, flame->x - 16, flame->y - 17, 65,
                34);
        flame->x -= 12;
        flame->variant++;
        if (flame->variant == 5) {
          Sound_PlayFar(133);
          work->shakeFrames = 4;
          for (j = 0; j != DOTS_PER_FLAME; j++) {
            s32 direction;
            s32 speed;

            dot = &DOTS[i * DOTS_PER_FLAME + j];
            direction = Random16() & 0xffff;
            speed = (Random16() & 0x1ff) + 256;
            dot->x = flame->x << 16;
            dot->y = flame->y << 16;
            dot->velocityX = (Trig_Sin(direction) * speed) >> 8;
            dot->velocityY = (Trig_Cos(direction) * speed) >> 7;
            // Each dot lives 32 to 47 frames.
            dot->variant = (Random16() & 15) + 32;
          }
        }
      }
    }

    // The Djinn circles on a radius that shrinks through the middle.
    if (frame <= 95) {
      s32 x = ((Trig_Sin(frame << 11) * (64 - frame * 2)) >> 17) + 96;
      s32 y = ((Trig_Cos(frame << 11) * (64 - frame * 2)) >> 16) + 60;

      draw[1](canvas, work->sheet, x - 10, y - 17, 20, 34);
      for (i = 0; i != FLAME_COUNT; i++) {
        // On its launch frame a free flame leaves the Djinn's hand.
        if (frame == Data_0819850e[i]) {
          struct EffectStep *shot = &work->particles[i];

          if (shot->variant == -1) {
            shot->x = x - 8;
            shot->velocityX = x;
            shot->y = y;
            shot->variant = 0;
            break;
          }
        }
        // Six frames later every target flashes and recoils.
        if (frame == Data_0819850e[i] + 6) {
          for (j = 0; j != effect->count; j++) {
            Func_0814cd48(effect->actors[j], 7, 5, j, 6);
            Func_08118088(effect->actors[j], 6);
          }
        }
      }
    }

    // Each live dot shrinks from 4 to 2 pixels and falls.
    for (i = 0; i != 256; i++) {
      dot = &DOTS[i];
      if (dot->variant != -1) {
        s32 size = dot->variant / 16 + 2;

        draw[1](canvas, sheet + gData_08197410[size - 1], HI(dot->x) - size / 2,
                HI(dot->y) - size, size, size * 2);
        BattleEffectKernels_IntegrateVector2(dot, 62, 0x2000);
        dot->variant--;
      }
    }

    BattleCamera_Shake(4, 4);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
    frame++;
  } while (frame != 96);

  // Teardown.
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(HEAP_BLITTER_ALTERNATE);
  HeapSlot_Release(HEAP_BLITTER);
  BattleEffect_EndCanvasLayer();
}

// The dots live in the map cell buffer: 32 for each of the eight balls.

#define DOT_COUNT 512
#define DOTS_PER_BALL 32
#define BALL_COUNT 8

extern char ResourceId_JupiterDjinnSmallSheet;
extern char ResourceId_FlameballSheet;

void BattleEffect_RunJupiter(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  u8 *sheet;
  DrawRectangle blit[2];
  DrawRectangle *draw;
  struct EffectStep *ball;
  struct EffectStep *dot;
  s32 i;
  s32 j;
  s32 angle;

  // Setup: the canvas, the blitter pair, the sheets.
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  sheet = Ram_HeapSlots->unk64[0];
  BattleEffect_StartCanvasLayer(0);
  REG_BLDALPHA = 0x1010;
  draw = blit;
  BattleEffect_FetchRectangleBlitters(0, draw);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, sheet, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_JupiterDjinnSmallSheet,
                             work->sheet, 1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_FlameballSheet,
                             work->sheet + 0x320, 1, 0);
  // Each transfer leaves 3/4 of the last frame behind as an afterimage.
  work->transferMode = 2;
  work->transferValue = 75;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);

  // The balls start above the canvas to the right; no dot is alive yet.
  for (i = 0; i != 32; i++) {
    ball = &work->particles[i];
    ball->x = (Random16() & 63) + 64;
    ball->y = (Random16() & 63) - 80;
  }
  for (i = 0; i != DOT_COUNT; i++)
    DOTS[i].variant = -1;

  Sound_PlayFar(171);
  frame = 0;
  angle = 0x8000;
  do {
    // The battle resumes and resolves the targets, hit cue 133.
    if (frame == 56)
      Func_081180e8(133);

    // The Djinn circles on a radius that shrinks through the middle.
    if (frame <= 95) {
      s32 x = (Trig_Sin(angle) * (64 - frame * 2)) >> 17;
      s32 y = (Trig_Cos(angle) * (64 - frame * 2)) >> 16;

      blit[0](canvas, work->sheet, x + 86, y + 28, 20, 40);
    }

    // Ball i drops from frame 8 + 4i until it passes y 95 and bursts.
    for (i = 0; i != BALL_COUNT; i++) {
      ball = &work->particles[i];
      if (frame >= i * 4 + 8 && ball->y <= 95) {
        blit[0](canvas, work->sheet + 0x320, ball->x - 20, ball->y - 32, 40,
                64);
        ball->x -= 6;
        ball->y += 12;
        if (ball->y > 95) {
          for (j = 0; j != DOTS_PER_BALL; j++) {
            s32 direction;
            s32 speed;

            dot = &DOTS[i * DOTS_PER_BALL + j];
            direction = Random16() & 0xffff;
            speed = (Random16() & 0x1ff) + 256;
            dot->x = ball->x << 16;
            dot->y = ball->y << 16;
            dot->velocityX = (Trig_Sin(direction) * speed) >> 7;
            dot->velocityY = (Trig_Cos(direction) * speed) >> 6;
            // Each dot lives 32 to 47 frames.
            dot->variant = (Random16() & 15) + 32;
          }
          Sound_PlayFar(133);
          work->shakeFrames = 4;
          // Every target flashes for six frames and recoils.
          for (j = 0; j != effect->count; j++) {
            Func_0814cd48(effect->actors[j], 7, 5, j, 6);
            Func_08118088(effect->actors[j], 6);
          }
        }
      }
    }

    // Each live dot shrinks from 3 to 1 pixel and falls.
    for (i = 0; i != DOT_COUNT; i++) {
      dot = &DOTS[i];
      if (dot->variant != -1) {
        s32 size = dot->variant / 16 + 1;

        draw[1](canvas, sheet + gData_08197410[size - 1], HI(dot->x) - size / 2,
                HI(dot->y) - size, size, size * 2);
        BattleEffectKernels_IntegrateVector2(dot, 62, 0x2000);
        dot->variant--;
      }
    }

    BattleCamera_Shake(4, 4);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
    angle -= 0x800;
    frame++;
  } while (frame != 96);

  // Teardown.
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(HEAP_BLITTER_ALTERNATE);
  HeapSlot_Release(HEAP_BLITTER);
  BattleEffect_EndCanvasLayer();
}

// Resources: decompress a resource with a palette.

#include "resource.h"

/// Decompress resource id into dst. If the first 128 bytes are a palette,
/// copy_pal copies it to the start of BG palettes; skip_pal skips it on
/// decoding.
void Resource_LoadAndDecompress(s32 id, void *dst, s32 skip_pal, s32 copy_pal) {
  u8 *res = Resource_GetTableEntry(id);

  if (copy_pal != 0) {
    // The entry held in a local loads after the destination is built; as a
    // plain Iwram_CopyWords call, 2 of 26 instructions differ in tla-en.
    s32 (*copy)(void *, const void *, s32) = Iwram_CopyWordsEntry;

    copy((void *)BG_PLTT, res, 128);
  }
  if (skip_pal != 0)
    res += 128;
  Resource_DecodeType01(res, dst);
}

// BattleEffect_LoadBlitter's packed argument: the mode in the high nibble.
#define BLITTER(mode, flags) ((mode) << 4 | (flags))

#define DOTS_PER_COLUMN 32
#define COLUMN_COUNT 10

extern char ResourceId_VenusDjinnSmallSheet;

void BattleEffect_RunVenus(struct BattleEffectArgument *effect) {
  struct HeapSlots *slots;
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  DrawRectangle draw[2];
  u8 *sheet;
  struct EffectStep *column;
  struct EffectStep *dot;
  s32 i;
  s32 j;
  s32 start;
  s32 angle;

  // Setup: the canvas, the two blitters, the three sheets.
  slots = Ram_HeapSlots;
  work = (struct BattleEffectWork *)slots->battleFxWork[0];
  canvas = slots->battleFxWork[1];
  sheet = slots->unk64[0];
  BattleEffect_StartCanvasLayer(0);
  REG_BLDALPHA = 0x1010;
  BattleEffect_LoadBlitter(
      HEAP_BLITTER, BLITTER(BLIT_MAX, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y |
                                          COMPOSITE_FLIP_Y));
  BattleEffect_LoadBlitter(
      HEAP_BLITTER_ALTERNATE,
      BLITTER(BLIT_ADDITIVE, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y));
  draw[0] = (DrawRectangle)slots->unk64[1];
  draw[1] = (DrawRectangle)slots->unkb8[1];
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, sheet, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_VenusDjinnSmallSheet, work->sheet,
                             1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_BlueFlameSheet,
                             work->sheet + 0x2f8, 1, 0);
  work->transferMode = 2;
  work->transferValue = 75;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);

  // Each column starts somewhere in the canvas's 64 pixels, on the ground.
  for (i = 0; i != 32; i++) {
    column = &work->particles[i];
    column->x = Random16() & 63;
    column->y = 104;
  }
  // No dot is alive yet.
  for (i = 0; i != DOT_COUNT; i++)
    DOTS[i].variant = -1;

  Sound_PlayFar(141);
  frame = 0;
  angle = 0x8000;
  do {
    // The Djinn swings on an arc that narrows by two pixels a frame.
    if (frame <= 79) {
      s32 x = (Trig_Sin(angle) * 24) >> 16;
      s32 y = (Trig_Cos(angle) * (64 - frame * 2)) >> 16;

      draw[1](canvas, work->sheet, x + 22, y + 29, 20, 38);
    }
    // The battle resumes and resolves the targets, hit cue 133.
    if (frame == 56)
      Func_081180e8(133);

    // Column i starts at frame 16 + 4i and climbs 12 pixels a frame.
    for (i = 0, start = 16; i != COLUMN_COUNT; start += 4, i++) {
      column = &work->particles[i];
      if (frame >= start) {
        draw[0](canvas, work->sheet + 0x9e0, column->x - 17, column->y - 32, 34,
                65);
        if (frame == start) {
          // The burst: sixteen dots sprayed upward from the column's foot.
          for (j = 0; j != 16; j++) {
            s32 direction;
            s32 speed;

            dot = &DOTS[i * DOTS_PER_COLUMN + j];
            direction = (Random16() & 0x7fff) + 0x4000;
            speed = (Random16() & 0x1ff) + 256;
            dot->x = column->x << 16;
            dot->y = (column->y + 16) << 16;
            dot->velocityX = (Trig_Sin(direction) * speed) >> 7;
            dot->velocityY = (Trig_Cos(direction) * speed) >> 6;
            dot->variant = (Random16() & 15) + 32;
          }
          if (i & 1)
            Sound_PlayFar(133);
          work->shakeFrames = 4;
          // Every target flashes for six frames and recoils.
          for (j = 0; j != effect->count; j++) {
            Func_0814cd48(effect->actors[j], 7, 5, j, 6);
            Func_08118088(effect->actors[j], 6);
          }
        }
        column->y -= 12;
      }
    }

    // Each live dot shrinks from 4 to 2 pixels as its life runs out.
    for (i = 0; i != DOT_COUNT; i++) {
      dot = &DOTS[i];
      if (dot->variant != -1) {
        s32 size = dot->variant / 16 + 2;

        draw[1](canvas, sheet + gData_08197410[size - 1], HI(dot->x) - size / 2,
                HI(dot->y) - size, size, size * 2);
        BattleEffectKernels_IntegrateVector2(dot, 62, 0x2000);
        dot->variant--;
      }
    }

    BattleCamera_Shake(4, 4);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
    angle -= 0x800;
    frame++;
  } while (frame != 96);

  // Teardown.
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(HEAP_BLITTER_ALTERNATE);
  HeapSlot_Release(HEAP_BLITTER);
  BattleEffect_EndCanvasLayer();
}

// The sparks live in the map cell buffer: sixteen under each of nine puffs.
#define SPARKS ((struct EffectStep *)Ram_MapCellBuffer)
#define PUFF_COUNT 9
#define SPARKS_PER_PUFF 16

extern char ResourceId_MarsDjinnSmallSheet;
extern char ResourceId_GlowOrbSheet;
extern char ResourceId_EmberStreakSheet;

// The ember puff's six cels: ☀️'s gBattleEffect_PuffArcCellWidths, _CellHeights,
// _CellBiasY and _CellSourceOffsets.
extern u8 gData_08197467[];
extern u8 gData_0819746d[];
extern u8 gData_08197473[];
extern u16 gData_0819747a[];

void BattleEffect_RunMars(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work;
  void *canvas;
  u8 *sheet;
  DrawRectangle blit[2];
  DrawRectangle *draw;
  struct EffectStep *puff;
  struct EffectStep *spark;
  s32 member;
  s32 frame;
  s32 i;
  s32 diveY;

  // Setup: the canvas, the blitter pair, the sheets.
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  sheet = Ram_HeapSlots->unk64[0];
  BattleEffect_StartCanvasLayer(0);
  draw = blit;
  BattleEffect_FetchRectangleBlitters(0, draw);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, sheet, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_MarsDjinnSmallSheet, work->sheet,
                             1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_GlowOrbSheet, work->sheet + 0x320,
                             1, 1);
  work->transferMode = 2;
  work->transferValue = 75;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);

  // The puffs sit on an oval, alternately left and right of the middle,
  // each two frames behind the last; sixteen sparks wait under each one.
  member = 0;
  puff = work->particles;
  do {
    s32 angle = member << 11;

    puff->x = (Trig_Sin(angle) * 24) >> 16;
    puff->y = ((Trig_Cos(angle) * 4) >> 16) + 52;
    if (member & 1)
      puff->x = 32 - puff->x;
    else
      puff->x += 32;
    puff->variant = -(member * 2);
    for (i = 0; i != SPARKS_PER_PUFF; i++) {
      spark = &SPARKS[member * SPARKS_PER_PUFF + i];
      spark->x = (((Random16() & 15) + puff->x) - 8) << 16;
      spark->y = ((Random16() & 7) + 96) << 16;
      spark->velocityX = ((Random16() & 127) - 64) << 11;
      spark->velocityY = ((Random16() & 127) - 64) << 10;
      spark->z = Random16() & 0xffff;
      spark->velocityZ = Random16() & 0xffff;
    }
    puff++;
    member++;
  } while (member != PUFF_COUNT);

  Sound_PlayFar(0x88);
  frame = 0;
  diveY = -172;
  do {
    // The battle resumes and resolves the targets, hit cue 0x85.
    if (frame == 56)
      Func_081180e8(0x85);

    // The orb swells, a cel every four frames.
    if (frame <= 23)
      blit[0](canvas, work->sheet + 0x320 + (frame / 4) * 0x640, 40, 20, 40,
              40);
    // The Djinn appears in the orb, then dives down to the left.
    if (frame == 20)
      Iwram_CopyWords(
          (void *)BG_PLTT,
          Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSmallSheet), 128);
    if (frame >= 20 && frame <= 31) {
      if (frame > 23)
        draw[1](canvas, work->sheet, 146 - frame * 4, diveY, 20, 40);
      else
        blit[0](canvas, work->sheet, 50, 20, 20, 40);
    }
    // It lands: the ember sheet replaces the Djinn's.
    if (frame == 32) {
      Sound_PlayFar(0x91);
      work->shakeFrames = 8;
      Resource_LoadAndDecompress((s32)&ResourceId_EmberStreakSheet, work->sheet,
                                 1, 1);
    }
    // Each puff plays its six cels, eight frames each, once its delay ends.
    if (frame > 31) {
      member = 0;
      puff = work->particles;
      do {
        if (puff->variant >= 0 && puff->variant <= 47) {
          s32 cel = puff->variant / 8;
          u32 width;

          blit[0](canvas, work->sheet + gData_0819747a[cel],
                  puff->x - ((width = gData_08197467[cel]) >> 1),
                  puff->y + gData_08197473[cel], width, gData_0819746d[cel]);
        }
        member++;
        puff->variant++;
        puff++;
      } while (member != PUFF_COUNT);
    }

    // The sparks rise from frame 40, a puff's column two frames after the
    // last, wobbling four pixels either way.
    spark = SPARKS;
    member = 0;
    do {
      if (frame >= (member / SPARKS_PER_PUFF) * 2 + 40) {
        s32 size;
        s32 x;
        s32 wobble;

        size = (member & 1) + 3;
        x = HI(spark->x) + ((Trig_Sin(spark->z) * 4) >> 16);
        draw[1](canvas, sheet + gData_08197410[size - 1], x - ((u32)size >> 1),
                HI(spark->y) - size, size, size * 2);
        BattleEffectKernels_IntegrateVector2(spark, 64, -0x2000);
        wobble = spark->z;
        spark->z = wobble + 0x800;
        if (spark->z > 0xffff)
          spark->z = wobble + 0x800 - 0xffff;
      }
      member++;
      spark++;
    } while (member != PUFF_COUNT * SPARKS_PER_PUFF);

    // Every target flashes for sixteen frames and recoils.
    if (frame == 38) {
      for (member = 0; member != effect->count; member++) {
        Func_0814cd48(effect->actors[member], 7, 5, member, 16);
        Func_08118088(effect->actors[member], 6);
      }
    }

    BattleCamera_Shake(8, 8);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
    diveY += 8;
    frame++;
  } while (frame != 112);

  // Teardown.
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(HEAP_BLITTER_ALTERNATE);
  HeapSlot_Release(HEAP_BLITTER);
  BattleEffect_EndCanvasLayer();
}

// The canvas preparer's packed arguments.
#define PREPARE(kind, side, anchor) ((kind) | (side) << 4 | (anchor) << 5)

// The ice chips live in the map cell buffer.
#define CHIPS ((struct EffectStep *)Ram_MapCellBuffer)
#define CHIP_COUNT 64

// The ice chip's four cels: ☀️'s gProjectileVolley_PelletOffsets,
// gHail_CellWidths and gHail_CellHeights.
extern u16 Data_0819851c[];
extern u8 Data_08198513[];
extern u8 Data_08198517[];

void BattleEffectKernels_IntegrateVector3(struct EffectStep *step, s32 damping,
                                          s32 gravity);

void BattleEffect_RunHail(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 screen[3];
  s32 otherPoint[3];
  s32 spawn[3];
  DrawRectangle blit[2];
  struct EffectStep *chip;
  s32 i;
  s32 frame;
  s32 rise;

  // Setup: the canvas, the blitter pair for the caster's side, the water.
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  BattleEffect_StartCanvasLayer(0);
  Func_0815585c(effect, PREPARE(1, effect->side, 2), screen, otherPoint);
  BattleEffect_FetchRectangleBlitters(effect->side, blit);
  Resource_LoadAndDecompress((s32)&ResourceId_WaterSpraySheet, work->sheet, 1,
                             1);
  work->transferMode = 2;
  work->transferValue = 75;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);

  // The chips start in a 16-pixel spread at the first target and fly up
  // at random, each to live 32 to 47 frames.
  EffectPosition_ProjectBattler(effect->actors[0], spawn);
  for (i = 0; i != CHIP_COUNT; i++) {
    s32 angle;
    s32 speed;

    angle = (Random16() & 0x7fff) + 0x4000;
    speed = (Random16() & 0x1ff) + 0x80;
    CHIPS[i].x = ((spawn[0] / 2 + (Random16() & 0xf)) - 8) << 16;
    CHIPS[i].y = (spawn[1] + 8) << 16;
    CHIPS[i].velocityX = (Trig_Sin(angle) * speed) >> 9;
    CHIPS[i].velocityY = (Trig_Cos(angle) * speed) >> 6;
    CHIPS[i].z = Random16() & 0x7f;
    CHIPS[i].velocityZ = Random16() & 0x7f;
    CHIPS[i].variant = (Random16() & 0xf) + 32;
  }

  for (frame = 0; frame != 64; frame++) {
    // The last sixteen frames fade the layer out.
    if (frame > 47)
      REG_BLDALPHA = (64 - frame) | 0x1000;
    if (frame == 1) {
      Resource_LoadAndDecompress((s32)&ResourceId_IceChipSheet,
                                 work->sheet + 0x400, 1, 1);
      Resource_LoadAndDecompress((s32)&ResourceId_MercuryDjinnSmallSheet,
                                 work->sheet + 0x65c0, 1, 0);
    }

    // With its Djinn prelude, the Djinn hovers over the anchor.
    if (effect->prelude == 1) {
      s32 orbit;
      s32 x;
      s32 y;

      orbit = frame << 11;
      x = ((-Trig_Sin(orbit) << 2) >> 16) + screen[0] / 2 - 10;
      y = ((Trig_Cos(orbit) << 1) >> 16) + screen[1] - 22;
      // Never true in 64 frames; ☀️ has the same test.
      if (frame > 69)
        y = (y - frame * 2) + 138;
      blit[1](canvas, work->sheet + 0x65c0, x, y, 20, 40);
      if (frame <= 3)
        blit[1](canvas, work->sheet + 0x65c0, x, y, 20, 40);
    }

    // Chip i flies from frame 4 + i / 4, through the two blitters in turn.
    for (i = 0, chip = CHIPS; i != CHIP_COUNT; i++, chip++) {
      if (frame >= i / 4 + 4) {
        s32 cel;
        u32 width;
        u32 height;

        cel = (chip->z / 128) & 3;
        blit[i & 1](canvas, work->sheet + 0x400 + Data_0819851c[cel],
                    HI(chip->x) - (width = Data_08198513[cel]) / 2,
                    HI(chip->y) - (height = Data_08198517[cel]) / 2, width,
                    height);
        BattleEffectKernels_IntegrateVector3(chip, 63, 0x1000);
      }
    }

    // The hit: an eight-frame shake, the battle resumes, the first target
    // flashes for sixteen frames and recoils.
    if (frame == 8) {
      work->shakeFrames = frame;
      Func_081180e8(0x86);
      Func_0814cd48(effect->actors[0], 7, 5, 0, 16);
      Func_08118088(effect->actors[0], 3);
    }

    // The water rises 4 pixels a frame to 32 and drifts away from the
    // caster's side.
    rise = frame * 4;
    if (rise > 32)
      rise = 32;
    if (effect->side == 0) {
      for (i = 0; i != 5; i++)
        blit[0](canvas, work->sheet, (i << 5) - (frame / 4 & 31), 120 - rise,
                32, 32);
    } else {
      for (i = 0; i != 5; i++)
        blit[0](canvas, work->sheet, ((i << 5) + (frame / 4 & 31)) - 32,
                120 - rise, 32, 32);
    }

    BattleCamera_Shake(4, 8);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  // Teardown.
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(HEAP_BLITTER_ALTERNATE);
  HeapSlot_Release(HEAP_BLITTER);
  BattleEffect_EndCanvasLayer();
}

// BattleEffect_RunGround, effect kind 203: the Venus Djinn's rock toss.

// On screen: the background slowly takes on the portal's colours from
// frame 17. Six rocks leave the caster one after another, every two
// frames, flung out sideways with a random lift and drift; sixteen frames
// after each leaves, it is pulled towards the first target and slowed,
// until it drops below y 20 there: then it vanishes with cue 134, an
// eight-frame shake, and the target flashing and recoiling. Each rock is
// drawn with a rotating highlight and four chips that change shape and
// flip at random every other frame. If the effect has its Djinn prelude,
// a small Venus Djinn hovers over the anchor point, drawn twice as bright
// for the first four frames, and rises off from frame 17.

// Setup: the battle effect canvas and the canvas preparer (kind 0, anchor
// 2, for the caster's side), a max blitter for the side, the portal sheet
// and the Djinn sheet in the effect work, the transfer callback, and the
// eight rocks' starting points and velocities at the caster.
// Frame loop, 96 frames, as above, with a heavy shake throughout.
// Teardown: the callback and the blitter are released and the canvas
// layer is closed.

// Port of The Broken Seal's BattleEffect_RunGround (kind 203 there
// too), with The Lost Age's heap slots, blitter modes (3 minus ☀️'s) and
// task priority, no work->effect store, the canvas preparer's packed
// arguments, and the chips' blitter built by BattleEffect_BuildBlitter
// directly. Other editions place a differently built version; only the
// English listing was compared.

#include "battle_action.h"

#include "transform.h"

#define ROCK_COUNT 6
#define CHIPS_PER_ROCK 4
// The chips' shapes and flips live in particles[32..63].
#define FIRST_CHIP 32

typedef DrawRectangle BlitterPair[2];

extern char ResourceId_PortalSheet;

// The chips: ☀️'s gHail_ChipFlips (a blitter's flags), and, by shape,
// gHail_ChipCells, _ChipX, _ChipY, _ChipWidths and _ChipHeights.
extern const u8 Data_08198524[];
extern const u16 Data_0819853a[];
extern const u8 Data_0819854c[];
extern const u8 Data_08198555[];
extern const u8 Data_08198528[];
extern const u8 Data_08198531[];

// Steps the background colours one shade toward a resource's palette.
void Func_0815f0a0(s32 resource);

void BattleEffect_RunGround(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 i;
  s32 frame;
  BlitterPair draw;
  s32 *camera;
  struct MotionObject *actor;
  s32 screen[3];
  s32 otherPoint[3];
  struct EffectPosition position;
  s32 j;

  // Setup: the canvas, a max blitter for the side, the two sheets.
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  camera = Ram_HeapSlots->cameraWork;
  BattleEffect_StartCanvasLayer(0);
  Func_0815585c(effect, PREPARE(0, effect->side, 2), screen, otherPoint);
  REG_BLDALPHA = 0x1010;
  if (effect->side == 1)
    BattleEffect_LoadBlitter(
        HEAP_BLITTER, BLITTER(BLIT_MAX, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y |
                                            COMPOSITE_FLIP_X));
  else
    BattleEffect_LoadBlitter(
        HEAP_BLITTER, BLITTER(BLIT_MAX, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y));
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  Resource_LoadAndDecompress((s32)&ResourceId_PortalSheet, work->sheet, 1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_VenusDjinnSmallSheet,
                             work->sheet + 0x65c0, 1, 1);
  work->transferMode = 2;
  work->transferValue = 75;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);

  // The rocks start at the caster, 66 high, flung out to the side away
  // from the caster with a random lift and drift.
  actor = GetBattleObjectSlotFar(effect->actor)->object;
  {
    struct MotionObject *target =
        GetBattleObjectSlotFar(effect->actors[0])->object;

    for (i = 0; i != 8; i++) {
      struct EffectStep *rock = &work->particles[i];

      rock->x = actor->x;
      rock->y = 0x420000;
      rock->z = actor->z;
      rock->velocityX = (i * 0x500000) >> 5;
      rock->velocityY = (s32)(((Random16() & 127) - 64) << 16) >> 6;
      rock->velocityZ = (s32)(((Random16() & 255) - 127) << 16) >> 5;
      if (rock->x > 0)
        rock->velocityX = -rock->velocityX;
      rock->variant = 1;
    }

    for (frame = 0; frame != 96; frame++) {
      // The background takes on the portal's colours from frame 17.
      if (frame > 16)
        Func_0815f0a0((s32)&ResourceId_PortalSheet);

      // With its Djinn prelude, the Djinn hovers over the anchor and rises
      // off from frame 17; the first four frames add a bright copy.
      if (effect->prelude == 1) {
        s32 angle = frame << 11;
        s32 x = ((-Trig_Sin(angle) << 2) >> 16) + screen[0] / 2 - 10;
        s32 y = ((Trig_Cos(angle) << 1) >> 16) + screen[1] - 22;

        if (frame > 16)
          y = y - frame * 2 + 32;
        if (effect->side == 1)
          BattleEffect_LoadBlitter(
              HEAP_BLITTER_ALTERNATE,
              BLITTER(BLIT_ADDITIVE,
                      COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y | COMPOSITE_FLIP_X));
        else
          BattleEffect_LoadBlitter(
              HEAP_BLITTER_ALTERNATE,
              BLITTER(BLIT_ADDITIVE, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y));
        if (frame <= 3)
          ((DrawRectangle)Ram_HeapSlots->unkb8[1])(canvas, work->sheet + 0x65c0,
                                                   x, y, 20, 40);
        HeapSlot_Release(HEAP_BLITTER_ALTERNATE);
        draw[0](canvas, work->sheet + 0x65c0, x, y, 20, 40);
      }

      // Every other frame each chip takes a new shape (3 to 8) and flip.
      if ((frame & 1) == 0) {
        for (i = 0; i != 32; i++) {
          struct EffectStep *chip = &work->particles[FIRST_CHIP + i];

          chip->velocityX = Random16() % 6 + 3;
          chip->velocityY = Data_08198524[Random16() & 3];
        }
      }

      Render_ResetTransformState();
      Transform_SetLookAt(camera, camera + 3);
      for (i = 0; i != ROCK_COUNT; i++) {
        struct EffectStep *rock = &work->particles[i];

        if (rock->variant == 1) {
          // Rock i flies from frame 2i: the rock, its highlight (two shapes
          // in turn, two frames each) and its four chips.
          if (frame > i * 2) {
            s32 x;
            s32 y;

            EffectPosition_ProjectPoint(&rock->x, &position);
            position.x >>= 1;
            x = position.x - 12;
            y = position.y - 24;
            draw[0](canvas, work->sheet, x, y, 24, 48);
            if ((frame & 3) <= 1)
              draw[0](canvas, work->sheet + Data_0819853a[1],
                      x + Data_0819854c[1], y + Data_08198555[1],
                      Data_08198528[1], Data_08198531[1]);
            else
              draw[0](canvas, work->sheet + Data_0819853a[2],
                      x + Data_0819854c[2], y + Data_08198555[2],
                      Data_08198528[2], Data_08198531[2]);
            for (j = 0; j != CHIPS_PER_ROCK; j++) {
              struct EffectStep *chip =
                  &work->particles[FIRST_CHIP + i * CHIPS_PER_ROCK + j];
              s32 chipX;
              s32 chipY;

              // A max blitter with the chip's own flip, for this chip only.
              {
                s32 mode = BLIT_MAX;
                s32 flags = chip->velocityY;
                /* FAKEMATCH(barrier): an empty asm after the mode and the flags
                 * are set; without it, 2 of 571 instructions differ in
                 * tla-en. */
                __asm__("");
                Func_08196404(HEAP_BLITTER_ALTERNATE, 7, 7, flags, mode);
              }
              draw[1] = (DrawRectangle)Ram_HeapSlots->unkb8[1];
              if (chip->velocityY & COMPOSITE_FLIP_X)
                chipX = x - Data_08198528[chip->velocityX] -
                        Data_0819854c[chip->velocityX] + 24;
              else
                chipX = x + Data_0819854c[chip->velocityX];
              if (chip->velocityY & COMPOSITE_FLIP_Y)
                chipY = y - Data_08198531[chip->velocityX] -
                        Data_08198555[chip->velocityX] + 48;
              else
                chipY = y + Data_08198555[chip->velocityX];
              draw[1](canvas, work->sheet + Data_0819853a[chip->velocityX],
                      chipX, chipY, Data_08198528[chip->velocityX],
                      Data_08198531[chip->velocityX]);
              HeapSlot_Release(HEAP_BLITTER_ALTERNATE);
            }
            rock->x += rock->velocityX;
            rock->y += rock->velocityY;
            rock->z += rock->velocityZ;
          }
          // Sixteen frames later it is pulled towards the target, 20 high,
          // and slowed until frame 2i + 85; below y 20 it hits.
          if (frame > i * 2 + 16) {
            rock->velocityX += (target->x - rock->x) >> 8;
            rock->velocityY += (0x140000 - rock->y) >> 8;
            rock->velocityZ += (target->z - rock->z) >> 8;
            if (frame < i * 2 + 85) {
              rock->velocityX = rock->velocityX * 60 / 64;
              rock->velocityY = rock->velocityY * 60 / 64;
              rock->velocityZ = rock->velocityZ * 60 / 64;
            }
            if (rock->y < 0x140000) {
              work->shakeFrames = 8;
              rock->variant = 0;
              Sound_PlayFar(134);
              Func_0814cd48(effect->actors[0], 7, 5, 0, 4);
              Func_08118088(effect->actors[0], 4);
            }
          }
        }
      }
      BattleCamera_Shake(16, 16);
      BattleEffect_CountDownBattlerTints();
      work->transferPending = 1;
      wait(1);
    }
  }

  // Teardown.
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(HEAP_BLITTER);
  BattleEffect_EndCanvasLayer();
}

#include "bg_scroll.h"

/// Shakes the battle view while the effect work's shakeFrames lasts, moving
/// BG1 and gCameraSceneParameters[3] and [4] by a random offset; once the
/// shake runs out, BG1 returns to its saved position and both parameters to
/// 120. It takes two random numbers each shaking frame, x first.
///
/// Called by nearly every effect. The x offset is centred on
/// half of rangeY too, so with rangeX smaller the x offset is never positive.
///
/// rangeX: the horizontal range, a power of two.
/// rangeY: the vertical range, a power of two; both offsets are centred on
///         half of it.
void BattleCamera_Shake(s32 rangeX, u32 rangeY) {
  struct BattleEffectWork *work;
  s32 *remainingFrames;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  remainingFrames = &work->shakeFrames;
  if (*remainingFrames > 0) {
    s32 randomX = (rangeX - 1) & Random16();
    s32 halfRange = (s32)rangeY / 2;
    s32 offsetY = ((rangeY - 1) & Random16()) - halfRange;
    s32 offsetX = randomX - halfRange;

    gBgScroll[1].x = offsetX;
    gBgScroll[1].y = offsetY + 32;
    gCameraSceneParameters[3] = 120 - offsetX;
    gCameraSceneParameters[4] = 120 - offsetY;
    (*remainingFrames)--;
    return;
  }
  gBgScroll[1].x = work->savedBg1X;
  gBgScroll[1].y = work->savedBg1Y;
  gCameraSceneParameters[3] = 120;
  gCameraSceneParameters[4] = 120;
}

/// While shakeFrames lasts, moves BG1 and the projection centre up or down
/// by a random offset within range; then restores them.
void Camera_ApplyVerticalShake(s32 range) {
  u8 *fx = Ram_HeapSlots->battleFxWork[0];
  s32 *timer;

  timer = (s32 *)(fx + 0x77a8);
  if (*timer > 0) {
    s32 shake = ((range - 1) & Random16()) - range / 2;

    gBgScroll[1].y = shake + 32;
    gCameraSceneParameters[4] = 120 - shake;
    (*timer)--;
  } else {
    gBgScroll[1].y = *(s32 *)(fx + 0x77a4);
    gCameraSceneParameters[4] = 120;
  }
}

#undef BLITTER

// Thor: a beam of light falls from a dusk sky, then a pillar of lightning and
// violet bolts strike the foes.

#include "input.h"

#include "macros.h"

struct FxPartyList;

void Func_0813baec(void); // BattleEffect_AdvanceScrollOnInterval
void BattleEffect_ConfigureEffectDisplay(void);

void Func_08163c2c(s32 page, s32 value);

void Func_0814cca8(void); // BattleEffect_SetupBlendedDisplay
void BattleEffect_SelectLivingTargets(
    struct FxPartyList *party); // SelectLivingTargets
void BattleEffect_SpawnObjects(s32 entryCount, s32 kind,
                               u32 variant); // SpawnObjects

void Func_0816442c(s32 group, s32 x,
                   s32 y); // BattleEffect_PlaceFormationObjects
void Func_0816467c(s32 group, s32 x, s32 y); // BattleEffect_RunImpactBurst

void Func_0818caa8(void *source, s32 x, s32 y, s32 size); // four motes
void Func_080150e4(s32 angle);  // SceneTransform_ApplyRoll
void Func_080151ac(s32 *scale); // SceneTransform_ApplyScale
void SceneTransform_ApplyPosition(const s32 *pos);
void SceneTransform_ApplyPitch(s32 angle);
void SceneTransform_ApplyYaw(s32 angle);

void Projection_Set(s32 a, s32 b, s32 c);
void BattleBackground_LoadFar(s32 layer, s32 resource, s32 mode);

// The Broken Seal's iwram_call.h forwards these through static inlines; here
// that form keeps a constant from being carried across the call.
static inline s32 CopyWords(void *destination, const void *source, s32 size) {
  /* FAKEMATCH(wrapper): the call through this inline; as a direct call to
   * Iwram_CopyWords, 2 of 1676 instructions of BattleEffect_RunThor differ in
   * tla-en. */
  return Iwram_CopyWords(destination, source, size);
}

static inline s32 RatioMulQ14(s32 divisor, s32 value) {
  /* FAKEMATCH(wrapper): the call through this inline; as a direct call to
   * Iwram_RatioMulQ14, 10 of 1676 instructions of BattleEffect_RunThor differ
   * in tla-en. */
  return Iwram_RatioMulQ14(divisor, value);
}

#define BLIT_MODE(mode, flags) ((mode) << 4 | (flags))
#define BLIT_CLIP (COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y)

// Heap slots 104 and 188 (byte offsets from 0x03000000) hold the blitters.
#define BLITTER ((DrawRectangle)Ram_HeapSlots->unk64[1])
#define BLITTER_ALTERNATE ((DrawRectangle)Ram_HeapSlots->unkb8[1])

extern char ResourceId_BlastSheet;

extern char ResourceId_Data135;
extern char ResourceId_DuskCloudsBackdrop;
extern char ResourceId_JupiterDjinnSheet;
extern char ResourceId_LightFanSheet;
extern char ResourceId_LightningPillarSheet;
extern char ResourceId_MarsDjinnSheet;
extern char ResourceId_MercuryDjinnSheet;
extern char ResourceId_ParticleSpritesC;

extern char ResourceId_VioletLightningSheetB;
extern char ResourceId_VioletMoteSheet;

extern const s16 Data_08196e2c[6][2]; // the three lines' end points

extern const u8 Data_0819855e[]; // gThor_TargetPalettes
extern const u8 Data_08198562[]; // gThor_SmokeFlips

#define STORM_PARTICLES ((struct EffectStep *)(Ram_MapCellBuffer + 0xc58))
#define STORM_MOTE_CELLS 0x6000
#define STORM_PARTICLE_COUNT 910
#define STORM_DIMMED_DOTS 10000
#define STORM_DIMMED_DOT_BYTES 0x302
#define STORM_DUSK_FRAMES 170
#define STORM_LIGHTNING_FRAMES 128
#define STORM_BOLTS 18

/// Thor (scene kind 12). In the dusk part (STORM_DUSK_FRAMES, which A or B cuts
/// short) a circle of 128 dots and three lines across it form over scrolling
/// dusk clouds, turn, sink into the sky and send down a beam of light; in the
/// lightning part (STORM_LIGHTNING_FRAMES) a pillar of lightning strikes the
/// formation and throws sparks, STORM_BOLTS violet bolts follow while a violet
/// mote sways above, and the formation is pushed back in smoke until an
/// impact burst ends the effect.
void BattleEffect_RunThor(struct BattleEffectArgument *effect) {
  u8 ramp[32];
  s32 point[3];
  struct EffectPosition screen[11];
  s32 position[3];
  s32 scale[3];
  struct BattleBackgroundView *view;
  struct BattleEffectWork *work;
  void *canvas;
  u8 *sprites;
  s32 frame;
  DrawRectangle blit[2]; // Unused: blit[1] gives the stack frame its 4 bytes.
  s32 size;
  s32 count;
  s32 i;
  s32 j;
  s32 dotCount;
  s32 spawned;
  struct EffectStep *spark;

  view = (struct BattleBackgroundView *)Ram_HeapSlots->transitionWork;
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  sprites = Ram_HeapSlots->unk64[0];
  BattleEffect_StartCanvasLayer(0x2000);
  REG_BG2PA = 0x100;
  Resource_LoadAndDecompress((s32)&ResourceId_LightFanSheet, work->sheet, 1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesC,
                             work->sheet + 0x1800, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, sprites, 0, 0);

  // Eight copies of the dot pictures, each with its colour levels capped 7
  // lower than the one before.
  {
    s32 offset;
    for (i = 0, offset = 0; i != 8; i++) {
      u8 *src;
      for (j = 0, src = sprites; j != STORM_DIMMED_DOT_BYTES; j++) {
        s32 v = *src++;

        if (v > 64 - i * 7) {
          v = 64 - i * 7;
        }
        if (v < 0) {
          v = 0;
        }
        (work->sheet + STORM_DIMMED_DOTS)[offset + j] = v;
      }
      offset += j;
    }
  }

  BattleEffect_ConfigureEffectDisplay();
  PLTT_BACKDROP = 0;
  BG_PLTT[1] = 0;
  REG_BG2CNT = 0x2784;
  work->scrollTimer = 0;
  work->scrollInterval = 2;
  work->scrollStepX = 1;
  work->scrollStepY = 0;
  view->busy = 1;
  Task_Add(Func_0813baec, 0xc80);
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);
  work->transferMode = 0;
  Func_08163c2c(0, 0);
  BattleBackground_LoadFar(1, (s32)&ResourceId_DuskCloudsBackdrop, 0);
  gCameraSceneParameters[4] = 240;
  Func_08163c2c(0, 1);
  REG_BG2PA = 0x80;
  BattleEffect_LoadBlitter(104, BLIT_MODE(BLIT_ADDITIVE, BLIT_CLIP));
  blit[0] = BLITTER;
  REG_DISPCNT = 0x7741;
  REG_BG2PA = 0x80;
  REG_BLDALPHA = 0x100f;
  REG_BLDCNT = 0x3f44;
  work->transferMode = 2;
  work->transferValue = 75;

  // One row of the beam: brightest in the middle, darker to each side.
  for (i = 0; i != 16; i++) {
    ramp[i] = i * 8 + 3;
    ramp[31 - i] = i * 8 + 3;
    if (ramp[i] > 63) {
      ramp[i] = 63;
    }
    if (ramp[31 - i] > 63) {
      ramp[31 - i] = 63;
    }
  }
  REG_BG2CNT = 0x784;
  CopyWords((void *)BG_PLTT,
            Resource_GetTableEntry((s32)&ResourceId_BlastSheet), 128);

  for (frame = 0; frame != STORM_DUSK_FRAMES; frame++) {
    size = 2;
    count = frame * 2;
    if (frame == 16) {
      Sound_PlayFar(140);
    }
    if (frame == 132) {
      Sound_PlayFar(131);
    }
    if (frame == 151) {
      Sound_PlayFar(145);
    }
    if (gInput.repeat & 3) {
      break;
    }
    for (i = 0; i != 2; i++) {
      if ((frame & 63) <= 15) {
        Func_0815f0a0((s32)&ResourceId_BlastSheet);
      } else if ((frame & 63) <= 31) {
        Func_0815f0a0((s32)&ResourceId_MercuryDjinnSheet);
      } else if ((frame & 63) <= 47) {
        Func_0815f0a0((s32)&ResourceId_MarsDjinnSheet);
      } else {
        Func_0815f0a0((s32)&ResourceId_JupiterDjinnSheet);
      }
    }
    if (frame == 150) {
      work->transferMode = 1;
      work->transferValue = 0x1a1a1a1a;
    } else {
      work->transferMode = 2;
      work->transferValue = 75;
    }
    Projection_Set(0x1fe0000, RatioMulQ14(0x1fe0000, 0xc000), 0x7fff0000);
    Render_ResetTransformState();
    if (frame > 128) {
      s32 sinkFrame = frame - 128;
      if (sinkFrame > 22) {
        sinkFrame = 20;
      }
      SET_VECTOR(position, 0, -sinkFrame << 17, 0x2000000);
      size = (sinkFrame >> 2) + 2;
      if (size > 8) {
        size = 8;
      }
      SceneTransform_ApplyPosition(position);
      SceneTransform_ApplyPitch(-0x1000);
      Func_080150e4(0x8000);
      SceneTransform_ApplyPitch(0x8000);
      SceneTransform_ApplyYaw(sinkFrame << 12);
      if (frame > 150) {
        SET_VECTOR(scale, ((frame * 2 + frame) << 11) - 0xd9400, scale[0],
                   scale[0]);
        size = 5;
      } else {
        SET_VECTOR(scale, 0x10000 - sinkFrame * 3 * 0x400, scale[0], scale[0]);
      }
      Func_080151ac(scale);
    } else {
      SET_VECTOR(position, 0, 0, 0x1000000);
      SceneTransform_ApplyPosition(position);
      SceneTransform_ApplyPitch(-0x1000);
      Func_080150e4(frame << 8);
      SceneTransform_ApplyPitch(frame << 8);
    }

    // The three lines, drawn dot by dot between pairs of points.
    if (frame <= 149) {
      for (i = 0; i != 6; i++) {
        SET_VECTOR(point, (Data_08196e2c[i][0] - 96) << 16, 0,
                   (Data_08196e2c[i][1] - 96) << 16);
        EffectPosition_ProjectPoint(point, &screen[i]);
        screen[i].x = (screen[i].x >> 17) + 64;
        screen[i].y = (screen[i].y >> 16) + 60;
      }
      for (i = 0; i != 3; i++) {
        dotCount = count * 2 - i * 48 - 256;
        if (dotCount > 48) {
          dotCount = 48;
        }
        if (dotCount >= 0) {
          for (j = 0; j != dotCount; j++) {
            s32 x = screen[i * 2].x +
                    Iwram_MulQ16((screen[i * 2 + 1].x - screen[i * 2].x) * j,
                                 0x555);
            s32 y = screen[i * 2].y +
                    Iwram_MulQ16((screen[i * 2 + 1].y - screen[i * 2].y) * j,
                                 0x555);
            blit[0](canvas, sprites + gData_08197410[size - 1], x - size / 2,
                    y - size, size, size * 2);
          }
        }
      }
    }

    // The circle of 128 dots, darkening once the beam has fallen.
    if (frame <= 179) {
      DrawRectangle drawAlt;
      s32 level = 0;

      if (frame > 155) {
        level = frame - 156;
      }
      if (level > 7) {
        level = 7;
      }
      if (frame <= 139) {
        BattleEffect_LoadBlitter(188, BLIT_MODE(BLIT_ADDITIVE, BLIT_CLIP));
        drawAlt = BLITTER_ALTERNATE;
      } else {
        BattleEffect_LoadBlitter(188, BLIT_MODE(BLIT_MAX, BLIT_CLIP));
        drawAlt = BLITTER_ALTERNATE;
      }
      dotCount = count;
      if (dotCount > 128) {
        dotCount = 128;
      }
      for (i = 0; i != dotCount; i++) {
        s32 dotSize = size + 1;
        point[0] = Trig_Sin(i << 9) * 96;
        point[2] = -(Trig_Cos(i << 9) * 96);
        EffectPosition_ProjectPoint(point, &screen[0]);
        screen[0].x = (screen[0].x >> 17) + 64;
        screen[0].y = (screen[0].y >> 16) + 60;
        drawAlt(
            canvas,
            work->sheet +
                (level * STORM_DIMMED_DOT_BYTES + gData_08197410[dotSize - 1]) +
                STORM_DIMMED_DOTS,
            screen[0].x - dotSize / 2, screen[0].y - dotSize, dotSize,
            dotSize * 2);
      }
      HeapSlot_Release(188);
    }

    // The beam: rows of the ramp narrowing at its top, and the light fan
    // falling down it.
    if ((u32)(frame - 151) <= 16) {
      s32 beamHeight;
      s32 inset = 0;
      s32 never = 0;
      s32 neverToo = 0;
      beamHeight = (frame - 151) * 20 + 44;
      if (frame > 151) {
        inset = frame - 152;
      }
      if (inset > 15) {
        inset = 15;
      }
      for (i = 1; i != 10; i++) {
        if (inset + i <= 15) {
          blit[0](canvas, &ramp[inset + i], inset + 48 + i, 16 - i,
                  32 - inset * 2 - i * 2, 1);
        }
      }
      for (i = 0; i != beamHeight; i++) {
        blit[0](canvas, ramp + inset, inset + 48, i + 16, 32 - inset * 2, 1);
      }
      BattleEffect_LoadBlitter(188, BLIT_MODE(BLIT_MAX, BLIT_CLIP));
      BLITTER_ALTERNATE(canvas, work->sheet, 32, (frame - 151) * 20 - 12, 32,
                        96);
      HeapSlot_Release(188);
      /* FAKEMATCH(dead): a never-true test of never and neverToo, both 0, that
       * clears beamHeight; without it, 249 of 1676 instructions differ in
       * tla-en. */
      if (never || neverToo) {
        beamHeight = 0;
      }
      BattleEffect_LoadBlitter(
          188, BLIT_MODE(BLIT_MAX, BLIT_CLIP | COMPOSITE_FLIP_X));
      BLITTER_ALTERNATE(canvas, work->sheet, 64, (frame - 151) * 20 - 12, 32,
                        96);
      HeapSlot_Release(188);
    }
    work->transferPending = 1;
    wait(1);
  }

  // The lightning part.
  view->busy = 0;
  Task_Remove(Func_0813baec);
  Func_0814cca8();
  BattleEffect_SelectLivingTargets(work->effect);
  BattleEffect_SpawnObjects(9, 0x282, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_TornadoSheet, Ram_MapCellBuffer,
                             1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_LightningPillarSheet, work->sheet,
                             1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_VioletMoteSheet,
                             work->sheet + STORM_MOTE_CELLS, 1, 0);
  Iwram_CopyWords((void *)BG_PLTT,
                  Resource_GetTableEntry((s32)&ResourceId_CrystalSheet), 128);
  Resource_LoadAndDecompress((s32)&ResourceId_Data135, sprites, 0, 0);
  REG_BLDALPHA = 0x1010;
  {
    s32 formationX = 0x500000;
    s32 formationY = 0x400000;
    s32 formationSpeedX = 0;
    s32 formationSpeedY = 0;
    s32 pillarX;
    s32 pillarY;

    for (i = 0; i != STORM_PARTICLE_COUNT; i++) {
      STORM_PARTICLES[i].variant = 0;
    }
    for (i = 0; i != 16; i++) {
      work->particles[i].x = (Random16() & 31) - 16;
      work->particles[i].y = Random16() & 63;
      work->particles[i].variant = -(Random16() & 15);
    }
    work->transferMode = 2;
    work->transferValue = 50;
    Sound_PlayFar(145);

    for (frame = 0; frame != STORM_LIGHTNING_FRAMES; frame++) {
      s32 stormFrame;
      if (frame == 20) {
        Resource_LoadAndDecompress((s32)&ResourceId_VioletLightningSheetB,
                                   work->sheet, 1, 0);
      }
      stormFrame = frame - 80;
      if ((u32)stormFrame <= 59 && !(frame & 7)) {
        Sound_PlayFar(134);
      }
      if (frame == 120) {
        Func_081180e8(134);
      }
      for (j = 0; j != 1; j++) {
        s32 start = j * 128 + 8;
        if (frame >= start && frame < j * 128 + 17) {
          pillarX = 60;
          pillarY = 112;
          if (frame >= j * 128 + 9 && frame < j * 128 + 12) {
            blit[0](canvas, work->sheet, 36, 0, 48, pillarY);
          }
          if (frame >= j * 128 + 12 && frame < start + 8) {
            blit[0](canvas, work->sheet + 0x1500, 36, 0, 48, 112);
          }
          if (frame == start + 2) {
            spawned = 0;
            for (i = 0, spark = STORM_PARTICLES; i != STORM_PARTICLE_COUNT;
                 i++, spark++) {
              if (spark->variant == 0) {
                s32 speed = 0x3ff;
                s32 angle;
                speed &= Random16();
                angle = (Random16() & 0x7fff) - 0x4000;
                spark->x = pillarX << 16;
                spark->y = pillarY << 16;
                speed += 32;
                spark->velocityX = (Trig_Sin(angle) * speed) >> 7;
                spark->velocityY = -((Trig_Cos(angle) * speed) << 1) >> 7;
                spark->variant = (Random16() & 7) + 32;
                if (++spawned == 512) {
                  break;
                }
              }
            }
            work->shakeFrames = 8;
          }
        }
      }
      if (frame == 10) {
        for (i = 0; i != effect->count; i++) {
          Func_0814cd48(effect->actors[i], 7, 5, i, 8);
          Func_08118088(effect->actors[i], 4);
        }
      }
      // Frames 80 to 143: the violet bolts, 2 frames apart, and the violet
      // mote swaying in four mirrored copies.
      if ((u32)stormFrame <= 63) {
        s32 moteY;
        s32 mirrorX;
        for (i = 0; i != 6; i++) {
          if (frame == i * 8 + 80) {
            Iwram_FillWords(canvas, 0x4000, 0x10101010);
          }
        }
        for (i = 0; i != STORM_BOLTS; i++) {
          if (frame >= i * 2 + 80) {
            if (frame < i * 2 + 82) {
              s32 boltX = (Random16() & 3) - 3;
              s32 boltY = (Random16() & 3) + 32;
              BattleEffect_LoadBlitter(188, BLIT_MODE(BLIT_MAX, BLIT_CLIP));
              BLITTER_ALTERNATE(canvas, work->sheet + i % 3 * 0x1440, boltX,
                                boltY, 72, 72);
              HeapSlot_Release(188);
            }
            if (frame == i * 2 + 80) {
              spawned = 0;
              for (j = 0, spark = STORM_PARTICLES; j != STORM_PARTICLE_COUNT;
                   j++, spark++) {
                if (spark->variant == 0) {
                  s32 speed = 0x3ff;
                  s32 angle;
                  speed &= Random16();
                  angle = Random16() & 0xffff;
                  spark->x = 0x100000;
                  spark->y = 0x500000;
                  speed += 32;
                  spark->velocityX = (Trig_Sin(angle) * speed) >> 7;
                  spark->velocityY = -((Trig_Cos(angle) * speed) << 1) >> 7;
                  spark->variant = (Random16() & 7) + 32;
                  if (++spawned == 16) {
                    break;
                  }
                }
              }
              work->shakeFrames = 8;
              for (j = 0; j != effect->count; j++) {
                Func_0814cd48(effect->actors[j], Data_0819855e[Random16() & 3],
                              5, j, 4);
                Func_08118088(effect->actors[j], 4);
              }
            }
          }
        }
        {
          s32 radius = 7;
          s32 angle;
          radius &= Random16();
          angle = Random16() & 0xffff;
          mirrorX = ((Trig_Sin(angle) * (radius + 8)) >> 16) + 72;
          moteY = 32 - ((Trig_Cos(angle) * (radius + 8)) >> 16);
        }
        Func_0818caa8(work->sheet + STORM_MOTE_CELLS, mirrorX, moteY, 24);
        spawned = 0;
        for (i = 0, spark = STORM_PARTICLES; i != STORM_PARTICLE_COUNT;
             i++, spark++) {
          if (spark->variant == 0) {
            s32 speed = 63;
            s32 angle;
            speed &= Random16();
            angle = Random16() & 0xffff;
            spark->x = mirrorX << 16;
            spark->y = moteY << 16;
            speed += 64;
            spark->velocityX = (Trig_Sin(angle) * speed) >> 7;
            spark->velocityY = -((Trig_Cos(angle) * speed) << 1) >> 7;
            spark->variant = (Random16() & 7) + 16;
            if (++spawned == 4) {
              break;
            }
          }
        }
      }

      BattleEffect_LoadBlitter(188,
                               BLIT_MODE(BLIT_MAX, BLIT_CLIP | COMPOSITE_FLIP));
      {
        DrawRectangle draw = BLITTER_ALTERNATE;
        struct EffectStep *spark;
        for (i = 0, spark = STORM_PARTICLES; i != STORM_PARTICLE_COUNT;
             i++, spark++) {
          if (spark->variant > 0) {
            spark->variant--;
            BattleEffectKernels_IntegrateVector2(spark, 60, 0);
            if (spark->y > 0x780000) {
              spark->velocityY = -spark->velocityY / 2;
            } else if ((u32)spark->x <= 0x7effff && spark->y >= 0) {
              s32 x = spark->x >> 16;
              s32 y = spark->y >> 16;
              s32 dotSize = spark->variant / 16 + 1;
              draw(canvas, sprites + gData_08197410[dotSize - 1],
                   x - dotSize / 2, y - dotSize, dotSize, dotSize * 2);
            }
          }
        }
      }
      HeapSlot_Release(188);

      // The formation is pushed back from frame 32.
      if (frame > 15) {
        if (frame == 32) {
          formationSpeedX = 0x40000;
          formationSpeedY = -0x4000;
        }
        if (frame > 31) {
          formationX += formationSpeedX;
          formationY += formationSpeedY;
          formationSpeedX = formationSpeedX * 60 / 64;
          formationSpeedY = formationSpeedY * 60 / 64;
        }
        Func_0816442c(0, formationX, formationY);
      }
      // Smoke around the formation.
      if ((u32)(frame - 16) <= 127) {
        s32 formationScreenX;
        for (i = 0, formationScreenX = formationX >> 17; i != 3; i++) {
          s32 cell;
          s32 angle;
          s32 x;
          s32 y;
          s32 never = 0;
          s32 neverToo = 0;
          cell = i & 3;
          angle = Random16() & 0xffff;
          x = ((Trig_Sin(angle) * 8) >> 16) + formationScreenX -
              (gData_08197492[cell] >> 1);
          y = ((Trig_Cos(angle) * 40) >> 16) - (gData_08197498[cell] >> 1);
          /* FAKEMATCH(dead): a never-true test of never and neverToo, both 0,
           * that clears y; without it, 248 of 1676 instructions differ in
           * tla-en. */
          if (never || neverToo) {
            y = 0;
          }
          Func_08196404(188, 7, 7, 3 | Data_08198562[Random16() & 3], 1);
          BLITTER_ALTERNATE(canvas, Ram_MapCellBuffer + gData_08197486[cell], x,
                            y + 56, gData_08197492[cell], gData_08197498[cell]);
          HeapSlot_Release(188);
        }
      }
      if (frame <= 31) {
        BattleCamera_Shake(8, 16);
      } else {
        BattleCamera_Shake(4, 4);
      }
      BattleEffect_CountDownBattlerTints();
      work->transferPending = 1;
      wait(1);
    }

    HeapSlot_Release(104);
    Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
    Func_0816467c(0, formationX, formationY);
  }
  for (i = 0; i != 9; i++) {
    ResourceObject_ReleaseFar(work->objects[i]);
  }
  BattleEffect_EndCanvasLayer();
}

#undef BLITTER

#define BLITTER(mode, flags) ((mode) << 4 | (flags))

// The shards live in the map cell buffer, 21 for each boulder.
#define SHARDS ((struct EffectStep *)Ram_MapCellBuffer)
#define SHARDS_PER_ROCK 21
// A boulder shatters this many frames after it starts to fall.
#define FALL_FRAMES 18

extern char ResourceId_BoulderSheet;
// ☀️'s gSpire_RockCounts, by variant.
extern u8 Data_081985b0[];
// ☀️'s gSpire_ShardOffsets: x, y in the boulder for each shard.
extern u8 Data_08198566[];
// ☀️'s gSpire_RockX and gSpire_DropFrames, by boulder.
extern s8 Data_081985a6[];
extern u8 Data_081985ab[];
// The shard cels: ☀️'s gSpire_ShardCells, _ShardWidths and
// _ShardHeights.
extern u16 gData_081974bc[];
extern u8 gData_0819749e[];
extern u8 gData_081974ad[];

void BattleEffect_RunSpire(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work;
  void *canvas;
  DrawRectangle blit;
  s32 rocks;
  struct EffectPosition first;
  struct EffectPosition final;
  struct EffectStep *rock;
  struct EffectStep *shard;
  s32 frame;
  s32 i;
  s32 j;
  s32 cel;
  u32 width;
  u32 height;

  // Setup: the canvas on BG2, the sheet, a transparent blitter.
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  BattleEffect_StartCanvasLayer(1);
  REG_BG2PA = 0x100;
  REG_BLDCNT = 0;
  Resource_LoadAndDecompress((s32)&ResourceId_BoulderSheet, work->sheet, 1, 1);
  BattleEffect_LoadBlitter(
      HEAP_BLITTER,
      BLITTER(BLIT_TRANSPARENT, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y));
  blit = (DrawRectangle)Ram_HeapSlots->unk64[1];

  // The canvas is centred between the first and the last target.
  EffectPosition_ProjectScaledBattler(effect->actors[0], &first);
  EffectPosition_ProjectScaledBattler(effect->actors[effect->count - 1],
                                      &final);
  first.x += (final.x - first.x) / 2;
  REG_BG2X = (64 - first.x) << 8;
  work->transferMode = 1;
  work->transferValue = 0;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);

  // Each boulder waits 64 pixels above the canvas; each shard sits at its
  // place in its boulder, ready to fly up at random.
  rocks = Data_081985b0[effect->variant];
  for (i = 0; i != rocks; i++) {
    rock = &work->particles[i];
    rock->y = -0x400000;
    rock->velocityY = 0;
  }
  for (i = 0; i != rocks; i++) {
    for (j = 0; j != SHARDS_PER_ROCK; j++) {
      struct EffectStep *seed = &SHARDS[i * SHARDS_PER_ROCK + j];

      seed->x = (Data_08198566[j * 2] + Data_081985a6[i]) << 16;
      seed->y = Data_08198566[j * 2 + 1] << 16;
      seed->velocityX = ((Random16() % 96) - 48) << 10;
      seed->velocityY = -((Random16() & 127) + 32) << 11;
      seed->z = 32;
      seed->variant = 0;
    }
  }

  for (frame = 0; frame != Data_081985ab[rocks - 1] + 65; frame++) {
    // 48 frames after the last drop the battle resumes, hit cue 133.
    if (frame == Data_081985ab[rocks - 1] + 48)
      Func_081180e8(133);
    for (i = 0; i != rocks; i++) {
      rock = &work->particles[i];
      // The crash: the first one resumes the battle, the others sound.
      if (frame == Data_081985ab[i] + FALL_FRAMES) {
        if (i == 0)
          Func_081180e8(134);
        else
          Sound_PlayFar(134);
        work->shakeFrames = 4;
      }
      // The shards fly, each tumbling through its kind's three cels more
      // slowly every other frame.
      if (frame >= Data_081985ab[i] + FALL_FRAMES) {
        for (j = 0; j != SHARDS_PER_ROCK; j++) {
          shard = &SHARDS[i * SHARDS_PER_ROCK + j];
          cel = (j % 5) * 3 + shard->variant / 96 % 3;
          blit(canvas, work->sheet + gData_081974bc[cel] + 0x83c,
               HI(shard->x) - ((width = gData_0819749e[cel]) >> 1),
               HI(shard->y) - ((height = gData_081974ad[cel]) >> 1), width,
               height);
          BattleEffectKernels_IntegrateVector2(shard, 64, 0x4000);
          shard->variant += shard->z;
          if (shard->z > 1 && (frame & 1))
            shard->z--;
        }
      }
      // Until then the boulder falls, a pixel a frame faster each frame,
      // and rests at y 50.
      if (frame < Data_081985ab[i] + FALL_FRAMES) {
        if (frame >= Data_081985ab[i])
          blit(canvas, work->sheet, Data_081985a6[i] + 47, HI(rock->y), 34, 62);
        rock->y += rock->velocityY;
        if (frame > Data_081985ab[i])
          rock->velocityY += 0x10000;
        if (rock->y > 0x320000)
          rock->y = 0x320000;
      }
      // Every target flashes for eight frames at each crash.
      if (frame == Data_081985ab[i] + FALL_FRAMES) {
        for (j = 0; j != effect->count; j++)
          Func_0814cd48(effect->actors[j], 7, 5, j, 8);
      }
    }
    BattleCamera_Shake(2, 4);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  // Teardown.
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(HEAP_BLITTER);
  BattleEffect_EndCanvasLayer();
}

// Effects: burst effect entry points.

struct BattleEffectArgument;

/// Each entry passes a burst type to BattleEffect_RunBurstScene.
/// step: effect arguments.
void BattleEffect_RunGust(struct EffectStep *step) {
  if (step->variant == 0) {
    BattleEffect_RunBurstScene(step, 6);
    return;
  }
  if (step->variant == 1) {
    BattleEffect_RunBurstScene(step, 7);
    return;
  }
  BattleEffect_RunBurstScene(step, 8);
}

void BattleEffect_RunBearClaw(struct EffectStep *step) {
  BattleEffect_RunBurstScene(step, 0);
}

void BattleEffect_RunRansack(struct EffectStep *step) {
  BattleEffect_RunBurstScene(step, 0);
}

void BattleEffect_RunBacteriaRush(struct EffectStep *step) {
  BattleEffect_RunBurstScene(step, 1);
}

void BattleEffect_RunFreebiteRush(struct EffectStep *step) {
  BattleEffect_RunBurstScene(step, 2);
}

void BattleEffect_RunFlyingAttack(struct EffectStep *step) {
  if (step->variant == 0) {
    BattleEffect_RunBurstScene(step, 3);
    return;
  }
  if (step->variant == 1) {
    BattleEffect_RunBurstScene(step, 4);
    return;
  }
  BattleEffect_RunBurstScene(step, 5);
}

void EffectStep_RunAnimationMode9(struct EffectStep *step) {
  BattleEffect_RunBurstScene(step, 9);
}

void EffectStep_RunAnimationMode12(struct EffectStep *step) {
  BattleEffect_RunBurstScene(step, 12);
}

void EffectStep_RunAnimationMode10(struct EffectStep *step) {
  BattleEffect_RunBurstScene(step, 10);
}

void EffectStep_RunAnimationMode11(struct EffectStep *step) {
  BattleEffect_RunBurstScene(step, 11);
}

void EffectStep_RunAnimationMode6Or7Or8B(struct EffectStep *step) {
  if (step->variant == 0) {
    BattleEffect_RunBurstScene(step, 6);
    return;
  }
  if (step->variant == 1) {
    BattleEffect_RunBurstScene(step, 7);
    return;
  }
  BattleEffect_RunBurstScene(step, 8);
}

// The motes live in the map cell buffer, then the wave's, the smoke's and
// the packed orb's cels.
#define MOTES ((struct EffectStep *)Ram_MapCellBuffer)
#define WAVE_CELS (Ram_MapCellBuffer + 0x2400)
#define SMOKE_CELS (Ram_MapCellBuffer + 0x3788)
#define ORB_CELS (Ram_MapCellBuffer + 0x5288)
#define MOTE_COUNT 256

// A variant's record.
#define RECORD(field) Data_081985b3[variant * 8 + (field)]
enum {
  RECORD_ARC_SHEET,
  RECORD_PALETTE,
  RECORD_BLOWS,
  RECORD_MOTES,
  RECORD_INTERVAL,
  RECORD_FRAMES,
  RECORD_FILL,
  RECORD_EXTRA
};
enum { EXTRA_NONE, EXTRA_GLINTS, EXTRA_RIPPLE };

extern char ResourceId_YellowOrbSheet;

extern char ResourceId_BlueArcSheetA;
extern char ResourceId_BlueArcSheetB;
extern char ResourceId_WaveSheet;

// The variants' records, eight bytes each.
extern u8 Data_081985b3[];

// A glint's four flips.
extern const u8 Data_0819861b[];

void BattleEffect_RunBurstScene(struct BattleEffectArgument *effect,
                                s32 variant) {
  struct EffectPosition actorPos;
  struct EffectPosition targetPos;
  s32 screen[3];
  s32 otherPoint[3];
  struct EffectPosition pos;
  struct EffectPosition spot;
  BlitterPair draw;
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  u8 *sheet;
  s32 *camera;
  struct MotionObject *target;
  s32 resource;
  u8 *cels;
  s32 row;
  s32 col;
  s32 i;
  s32 j;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  sheet = Ram_HeapSlots->unk64[0];
  camera = Ram_HeapSlots->cameraWork;
  BattleEffect_StartCanvasLayer(0);
  REG_BLDALPHA = 0x1010;
  if (effect->prelude == 1)
    Func_0815585c(effect, PREPARE(7, effect->side, 2), screen, otherPoint);

  // The pictures and the palette.
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, sheet, 0, 0);
  // The orb (40x288) packed to half width in the map cell buffer: the
  // canvas is shown twice as wide.
  Resource_LoadAndDecompress((s32)&ResourceId_YellowOrbSheet, work->sheet, 1,
                             0);
  cels = WAVE_CELS;
  for (row = 0; row != 288; row++) {
    for (col = 0; col != 40; col++) {
      s32 dst = row * 20 + col / 2;
      s32 src = row * 40 + col;

      (cels + dst)[0x2e88] = work->sheet[src];
    }
  }
  // The glints' sheet takes the orb's place.
  Resource_LoadAndDecompress((s32)&ResourceId_TornadoSheet, work->sheet, 1, 0);
  if (RECORD(RECORD_ARC_SHEET) == 0)
    Resource_LoadAndDecompress((s32)&ResourceId_BlueArcSheetA,
                               work->sheet + 0xc80, 1, 1);
  else
    Resource_LoadAndDecompress((s32)&ResourceId_BlueArcSheetB,
                               work->sheet + 0xc80, 1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_WaveSheet, WAVE_CELS, 1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_SmokeSheet, SMOKE_CELS, 1, 0);
  switch (RECORD(RECORD_PALETTE)) {
  case 0:
    resource = (s32)&ResourceId_MarsDjinnSheet;
    break;
  case 1:
    resource = (s32)&ResourceId_LimePalette;
    break;
  case 2:
    resource = (s32)&ResourceId_BlueArcSheetB;
    break;
  default:
    resource = (s32)&ResourceId_EmberStreakSheet;
    break;
  }
  Iwram_CopyWords((void *)BG_PLTT, Resource_GetTableEntry(resource), 128);

  // The caster closes in; variant 12 lunges further and waits.
  if (variant == 12) {
    BattleMotion_ApproachTargetFar(effect->actor, effect->actors[0], 8,
                                   0x60000);
    wait(7);
  } else {
    BattleMotion_ApproachTargetFar(effect->actor, effect->actors[0], 4, 0);
  }
  wait(1);

  EffectPosition_ProjectScaledBattler(effect->actors[0], &targetPos);
  // Each transfer leaves 3/4 of the last frame behind as an afterimage.
  work->transferMode = 2;
  work->transferValue = 75;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);
  if (RECORD(RECORD_EXTRA) == EXTRA_RIPPLE)
    Task_Add(BattleEffect_ArmBg2AffineHBlankDma, 0xc80);

  // The motes wait 25 pixels above the target, aimed away from the
  // caster; none is out yet.
  target = GetBattleObjectSlotFar(effect->actors[0])->object;
  for (i = 0; i != MOTE_COUNT; i++) {
    struct EffectStep *mote = &MOTES[i];

    mote->x = target->x;
    mote->y = target->y + 0x190000;
    mote->z = target->z;
    mote->velocityX = (Random16() & 255) << 12;
    mote->velocityY = ((Random16() & 255) - 127) << 12;
    mote->velocityZ = ((Random16() & 255) - 127) << 12;
    if (mote->x > 0)
      mote->velocityX = -mote->velocityX;
    mote->variant = -1;
  }
  // The smoke puffs start at the target's feet, one a frame.
  for (i = 0; i != 64; i++) {
    struct EffectStep *puff = &work->particles[i];

    puff->x = (targetPos.x / 2) << 16;
    puff->y = 0x580000;
    puff->z = 0;
    puff->velocityX = ((Random16() & 255) - 128) << 9;
    puff->velocityY = ((Random16() & 255) - 128) << 9;
    puff->variant = -i;
    puff->velocityZ = 0;
  }

  for (frame = 0; frame != RECORD(RECORD_FRAMES); frame++) {
    s32 *lines;
    s32 blows;

    // The ripple's line table: BG2 swings six pixels on a sine wave.
    lines = work->bg2xLines;
    for (i = 0; i != 160; i++)
      *lines++ = (0x60000 - Trig_Sin((frame + i) * 0x800) * 6) >> 10;
    if (variant == 10 && frame == 8)
      Sound_PlayFar(221);
    blows = RECORD(RECORD_BLOWS);
    EffectPosition_ProjectScaledBattler(effect->actor, &actorPos);
    actorPos.x /= 2;

    // The arcs and waves, mirrored on odd blows.
    if (effect->side == 0) {
      BattleEffect_LoadBlitter(
          HEAP_BLITTER, BLITTER(BLIT_MAX, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y));
      BattleEffect_LoadBlitter(HEAP_BLITTER_ALTERNATE,
                               BLITTER(BLIT_MAX, COMPOSITE_CLIP_X |
                                                     COMPOSITE_CLIP_Y |
                                                     COMPOSITE_FLIP_Y));
    } else {
      BattleEffect_LoadBlitter(
          HEAP_BLITTER, BLITTER(BLIT_MAX, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y |
                                              COMPOSITE_FLIP_X));
      BattleEffect_LoadBlitter(HEAP_BLITTER_ALTERNATE,
                               BLITTER(BLIT_MAX, COMPOSITE_CLIP_X |
                                                     COMPOSITE_CLIP_Y |
                                                     COMPOSITE_FLIP));
    }
    draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
    // The slot read through an offset struct: as unkb8[1], 8 of 989
    // instructions differ.
    draw[1] = ((struct {
                u8 before[188];
                DrawRectangle draw;
              } *)Ram_HeapSlots)
                  ->draw;
    for (i = 0; i != blows; i++) {
      s32 start = i * RECORD(RECORD_INTERVAL);

      if (frame >= start && frame < start + 6) {
        s32 step = frame - start;

        if ((i & 3) <= 1 || RECORD(RECORD_ARC_SHEET) == 1) {
          // The arc: six 48x72 cels.
          if (effect->side == 0)
            draw[i & 1](canvas, work->sheet + 0xc80 + step * 3456,
                        targetPos.x / 2 - 16, targetPos.y - 40, 48, 72);
          else
            draw[i & 1](canvas, work->sheet + 0xc80 + step * 3456,
                        targetPos.x / 2 - 32, targetPos.y - 40, 48, 72);
        } else {
          // The wave: six 48x16 cels.
          if (effect->side == 0)
            draw[i & 1](canvas, WAVE_CELS + step * 768, targetPos.x / 2 - 16,
                        actorPos.y - 8, 48, 16);
          else
            draw[i & 1](canvas, WAVE_CELS + step * 768, targetPos.x / 2 - 32,
                        actorPos.y - 8, 48, 16);
        }
      }
      // The hit; the last blow ends the run.
      if (frame == start + 2) {
        if (RECORD(RECORD_FILL) == 1)
          Iwram_FillWords(canvas, 0x4000, 0x2f2f2f2f);
        Func_0814cd48(effect->actors[0], 7, 5, 0, 4);
        if (i == blows - 1) {
          Func_08118088(effect->actors[0], 4);
          work->shakeFrames = 8;
          Func_081180e8(134);
        } else {
          if (i & 1)
            Func_08118088(effect->actors[0], 7);
          work->shakeFrames = 4;
          Sound_PlayFar(134);
        }
        for (j = 0; j != RECORD(RECORD_MOTES); j++)
          MOTES[i * 32 + j].variant = (Random16() & 7) + 15;
      }
      // The orb: six 20x48 cels, two frames each.
      if (frame >= start + 2 && frame < start + 14)
        draw[0](canvas, ORB_CELS + ((frame - start - 2) / 2) * 960,
                targetPos.x / 2 - 10, targetPos.y - 24, 20, 48);
    }
    HeapSlot_Release(HEAP_BLITTER_ALTERNATE);
    HeapSlot_Release(HEAP_BLITTER);

    // The motes, until their life runs out, bouncing off the ground.
    Render_ResetTransformState();
    Transform_SetLookAt(camera, camera + 3);
    BattleEffect_LoadBlitter(
        HEAP_BLITTER,
        BLITTER(BLIT_ADDITIVE, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y));
    BattleEffect_LoadBlitter(
        HEAP_BLITTER_ALTERNATE,
        BLITTER(BLIT_MAX, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y));
    draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
    // The slot read through an offset struct: as unkb8[1], 8 of 989
    // instructions differ.
    draw[1] = ((struct {
                u8 before[188];
                DrawRectangle draw;
              } *)Ram_HeapSlots)
                  ->draw;
    for (i = 0; i != MOTE_COUNT; i++) {
      struct EffectStep *mote = &MOTES[i];
      s32 size = mote->variant;

      if (size > 0) {
        EffectPosition_ProjectPoint(&mote->x, &pos);
        pos.x /= 2;
        size >>= 3;
        size++;
        draw[(i / 2) & 1](canvas, sheet + gData_08197410[size - 1],
                          pos.x - size / 2, pos.y - size, size, size * 2);
        BattleEffectKernels_IntegrateVector3(mote, 60, -0x400);
        if (mote->y < 0x80000)
          mote->velocityY = -mote->velocityY / 2;
        mote->variant--;
      }
    }
    HeapSlot_Release(HEAP_BLITTER_ALTERNATE);
    HeapSlot_Release(HEAP_BLITTER);

    if (frame > 3) {
      // The glints: three a frame around the target, each with its own
      // random flip, until frame 31.
      if (RECORD(RECORD_EXTRA) == EXTRA_GLINTS && frame <= 31) {
        EffectPosition_ProjectScaledBattler(effect->actors[0], &spot);
        for (i = 0; i != 3; i++) {
          s32 angle = Random16() & 0xffff;
          s32 cel = frame & 3;
          s32 radius = (Random16() & 31) + 4;
          s32 x = spot.x / 2 + ((Trig_Sin(angle) * radius) >> 17) -
                  gData_08197492[cel] / 2;
          s32 y = spot.y - ((Trig_Cos(angle) * radius) >> 17) -
                  gData_08197498[cel] / 2;
          u32 flip = Random16();

          // An empty once-only loop ends CSE's block, so the two reads of each
          // size table's address stay apart; without it, 397 of 989
          // instructions differ in tla-en.
          do {
          } while (0);
          Func_08196404(HEAP_BLITTER_ALTERNATE, 7, 7,
                        Data_0819861b[flip & 3] | COMPOSITE_CLIP_X |
                            COMPOSITE_CLIP_Y,
                        BLIT_ADDITIVE);
          ((DrawRectangle)Ram_HeapSlots->unkb8[1])(
              canvas, work->sheet + gData_08197486[cel], x, y,
              gData_08197492[cel], gData_08197498[cel]);
          HeapSlot_Release(HEAP_BLITTER_ALTERNATE);
        }
      }
      // The smoke: sixteen puffs rise from the target's feet, six cels of
      // four frames each.
      if (RECORD(RECORD_EXTRA) == EXTRA_RIPPLE) {
        BattleEffect_LoadBlitter(
            HEAP_BLITTER,
            BLITTER(BLIT_ADDITIVE, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y));
        draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
        for (i = 0; i != 16; i++) {
          struct EffectStep *puff = &work->particles[i];

          if ((u32)puff->variant <= 23) {
            draw[0](canvas, SMOKE_CELS + (puff->variant / 4) * 0x480,
                    HI(puff->x) - 12, HI(puff->y) - 24, 24, 48);
            BattleEffectKernels_IntegrateVector2(puff, 62, -0x1000);
          }
          puff->variant++;
        }
        HeapSlot_Release(HEAP_BLITTER);
      }
    }
    BattleCamera_Shake(8, 8);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  // Teardown.
  if (RECORD(RECORD_EXTRA) == EXTRA_RIPPLE)
    Task_Remove(BattleEffect_ArmBg2AffineHBlankDma);
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}

#undef ROCK_COUNT

// The dust lives in the map cell buffer once the picture is laid out.
#define DUST ((struct EffectStep *)Ram_MapCellBuffer)
#define ROCK_COUNT 11
// The rocks' motion is particles[10..20], the late rocks' columns [16..21].
#define FIRST_ROCK 10

struct Scale {
  s32 x;
  s32 y;
};

struct RockSpritePart {
  u8 unk00[4];
  u16 y : 8;
  u16 affine : 2;
  u16 objectMode : 2;
  u16 mosaic : 1;
  u16 fullColor : 1;
  u16 shape : 2;
  u16 x : 9;
  u16 affineIndex : 5;
  u16 size : 2;
  u16 tile : 10;
  u16 priority : 2;
  u16 palette : 4;
  u8 unk0a[2];
};

struct RockObject {
  struct RockSpritePart part[2];
  u8 unk18[2];
  u8 flags;
};

extern char ResourceId_EarthWallSheet;
extern char ResourceId_OrangePaletteA;

// ☀️'s gGaia_Bg2Shifts, by side and variant.
extern s8 Data_08198648[][3];
// The growing wall's nine rows: ☀️'s gGaia_RowWidths and
// gGaia_RowCells.
extern u8 Data_08198632[];
extern u16 Data_08198620[];
// The mound's three cels: ☀️'s gGaia_MoundWidths, _MoundHeights and
// _MoundSheetOffsets.
extern u8 Data_08198642[];
extern u8 Data_08198645[];
extern u16 Data_0819863c[];
// ☀️'s gGaia_LateRockColumns.
extern u8 Data_0819864e[];

// ☀️'s gRisingWall_UnitScale.
extern struct Scale Data_08196e44;

void Animation_ApplyChildArgumentFar(void *object, s32 value);

void BattleEffect_RunGaia(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  BlitterPair draw;
  u8 *sheet;
  s32 wallX;
  s32 rockX;
  struct Scale scale;
  s32 position[4];
  u8 *source;
  u16 *color;
  void *palette = (void *)BG_PLTT;
  s32 offset;
  s32 i;
  s32 j;
  s32 k;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  sheet = Ram_HeapSlots->unk64[0];
  BattleEffect_StartCanvasLayer(0);

  // The larger variants stretch the wall wider: BG2 at 1.25 and 1.5.
  if (effect->variant == 0)
    REG_BG2PA = 204;
  else if (effect->variant == 1)
    REG_BG2PA = 170;
  if (effect->side == 1) {
    wallX = 8;
    if (effect->variant == 0)
      rockX = 40;
    else if (effect->variant == 1)
      rockX = 36;
    else
      rockX = 40;
  } else {
    wallX = -16;
    rockX = -12;
  }
  REG_BG2X = Data_08198648[effect->side][effect->variant] << 8;

  // The palette: BG colours 1-63 darkened eight steps in each channel.
  Resource_LoadAndDecompress((s32)&ResourceId_EarthWallSheet, Ram_MapCellBuffer,
                             1, 0);
  Iwram_CopyWords(palette,
                  Resource_GetTableEntry((s32)&ResourceId_OrangePaletteA), 128);
  color = (u16 *)&BG_PLTT[1];
  for (i = 0; i != 63; i++) {
    s32 c = *color;
    s32 blue = ((u16)c >> 10) & 31;
    s32 green = ((u16)c >> 5) & 31;
    s32 red = 31;

    red &= c;
    blue -= 8;
    green -= 8;
    red -= 8;

    if (blue < 0)
      blue = 0;
    if (green < 0)
      green = 0;
    if (red < 0)
      red = 0;
    *color++ = (blue << 10) | (green << 5) | red;
  }

  // The picture, laid out again in the effect work:
  //   the growing wall: nine one-line rows, each stretched to 32 lines;
  //   the top of the standing wall at 0x13c0: 48x32, stretched to 48x96;
  //   its foot at 0x25c0: 48x21;
  //   the mound: three cels.
  source = Ram_MapCellBuffer;
  offset = 0;
  for (j = 0; j != 9; j++) {
    for (i = 0; i != 32; i++) {
      for (k = 0; k != Data_08198632[j]; k++, offset++)
        work->sheet[offset] = source[k];
    }
    source += Data_08198632[j];
  }
  for (j = 0; j != 32; j++) {
    for (i = 0; i != 3; i++) {
      for (k = 0; k != 48; k++, offset++)
        work->sheet[offset] = source[k];
    }
    source += 48;
  }
  for (i = 0; i != 1008; i++)
    work->sheet[offset++] = *source++;
  for (i = 0; i != 3; i++) {
    for (k = 0; k != Data_08198642[i] * Data_08198645[i]; k++)
      work->sheet[offset++] = *source++;
  }
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, sheet, 0, 0);

  // The rocks: eleven animated objects, four of each size.
  for (i = 0; i != ROCK_COUNT; i++) {
    struct RockObject *object = ResourceObject_CreateFar(653);

    work->objects[i] = object;
    if (object != 0) {
      object->flags = 0;
      Animation_ApplyChildArgumentFar(object, i / 4);
      ((struct RockObject *)work->objects[i])->part[0].priority = 1;
    }
  }
  // Two max blitters, the second mirrored for the right half.
  BattleEffect_LoadBlitter(
      HEAP_BLITTER, BLITTER(BLIT_MAX, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y));
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  BattleEffect_LoadBlitter(HEAP_BLITTER_ALTERNATE,
                           BLITTER(BLIT_MAX, COMPOSITE_CLIP_X |
                                                 COMPOSITE_CLIP_Y |
                                                 COMPOSITE_FLIP_X));
  draw[1] = (DrawRectangle)Ram_HeapSlots->unkb8[1];
  REG_BLDCNT = 0x3f46;
  REG_BLDALPHA = 0x1010;
  // Each transfer leaves 3/4 of the last frame behind as an afterimage.
  work->transferMode = 2;
  work->transferValue = 75;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);

  // A rock's x is its column, y counts down from 128 by z, velocityX cycles
  // 1 to 12 as its animation step, and variant is its wait before it flies.
  for (i = 0; i != ROCK_COUNT; i++) {
    struct EffectStep *rock = &work->particles[FIRST_ROCK + i];

    rock->x = (Random16() & 15) + 88;
    rock->y = 128;
    rock->z = (Random16() & 15) + 2;
    rock->velocityX = i;
    rock->velocityY = 1;
    rock->velocityZ = 0x8000;
    rock->variant = 44 - i * 4;
  }
  for (i = 0; i != 6; i++)
    work->particles[16 + i].x = Data_0819864e[i];

  // The dust rises from the foot of the wall after a random wait.
  for (i = 0; i != 256; i++) {
    struct EffectStep *dust = &DUST[i];

    dust->x = ((Random16() & 63) + wallX + 32) << 16;
    dust->y = ((Random16() & 7) + 96) << 16;
    dust->velocityY = ((Random16() & 63) + 32) << 13;
    dust->variant = Random16() & 31;
  }
  REG_BG2CNT = 0x785;
  work->shakeFrames = 250;

  for (frame = 0; frame != 192; frame++) {
    if (frame == 0)
      Sound_PlayFar(212);
    if (frame == 40)
      Sound_PlayFar(141);
    if (frame == 96)
      Sound_PlayFar(145);
    // The battle resumes and resolves the targets, hit cue 134.
    if (frame == 120)
      Func_081180e8(134);

    // The ground heaves up (frames 0-81), then pulses between two cels.
    if (frame <= 81) {
      s32 cel = frame / 4;

      if (cel > 2)
        cel = (cel & 1) + 1;
      draw[0](canvas, work->sheet + Data_0819863c[cel],
              wallX - Data_08198642[cel] + 64, 116 - Data_08198645[cel],
              Data_08198642[cel], Data_08198645[cel]);
      draw[1](canvas, work->sheet + Data_0819863c[cel], wallX + 64,
              116 - Data_08198645[cel], Data_08198642[cel], Data_08198645[cel]);
    }

    // The wall grows out of it, a row every three frames (frames 12-87).
    if (frame >= 12 && frame <= 87) {
      s32 row = (frame - 64) / 3;

      if (row < 0)
        row = 0;
      if (row > 7)
        row = 7;
      for (i = 0; i != 4; i++) {
        draw[0](canvas, work->sheet + Data_08198620[row],
                wallX - Data_08198632[row] + 64, i * 32 - 12,
                Data_08198632[row], 32);
        draw[1](canvas, work->sheet + Data_08198620[row], wallX + 64,
                i * 32 - 12, Data_08198632[row], 32);
      }
    }

    // It sinks back the same way (frames 160-183).
    if (frame >= 160 && frame <= 183) {
      s32 row = 7 - (frame - 160) / 3;

      if (row < 0)
        row = 0;
      if (row > 7)
        row = 7;
      for (i = 0; i != 4; i++) {
        draw[0](canvas, work->sheet + Data_08198620[row],
                wallX - Data_08198632[row] + 64, i * 32 - 12,
                Data_08198632[row], 32);
        draw[1](canvas, work->sheet + Data_08198620[row], wallX + 64,
                i * 32 - 12, Data_08198632[row], 32);
      }
    }

    // The standing wall: its top and its foot (frames 88-159).
    if (frame >= 88 && frame <= 159) {
      draw[0](canvas, &work->sheet[0x13c0], wallX + 16, 0, 48, 96);
      draw[1](canvas, &work->sheet[0x13c0], wallX + 64, 0, 48, 96);
      draw[0](canvas, &work->sheet[0x25c0], wallX + 16, 96, 48, 21);
      draw[1](canvas, &work->sheet[0x25c0], wallX + 64, 96, 48, 21);
    }

    // The dust (from frame 88): what leaves the top goes back to the foot
    // until frame 159.
    if (frame > 87) {
      for (i = 0; i != 64; i++) {
        struct EffectStep *dust = &DUST[i];

        if (dust->variant == 0) {
          s32 size = (i & 3) + 5;

          draw[0](canvas, sheet + gData_08197410[size - 1],
                  HI(dust->x) - size / 2, HI(dust->y) - size, size, size * 2);
          dust->y -= dust->velocityY;
          if (dust->y < 0 && frame <= 159)
            dust->y = 0x600000;
        } else {
          dust->variant--;
        }
      }
    }

    // The rocks (from frame 5): eleven and bigger from frame 72.
    if (frame > 4) {
      s32 rocks = 6;

      if (frame > 71)
        rocks = ROCK_COUNT;
      for (k = 0; k != rocks; k++) {
        struct EffectStep *rock = &work->particles[FIRST_ROCK + k];
        s32 slot;

        if (rock->variant == 0) {
          scale = Data_08196e44;
          if (frame > 71) {
            scale.x = (k << 12) + 0x8000;
            scale.x += effect->variant << 14;
          } else {
            scale.x = 0x8000;
          }
          scale.y = scale.x;
          position[3] = 0;
          position[0] = (rock->x + rockX * 2) << 16;
          position[1] = 0x2000000 - (rock->y << 16);
          position[2] = 0x2000000;
          slot = (frame / 2 + k) % ROCK_COUNT;
          if (slot != -1)
            Render_ApplyProjectedPlacementFar(work->objects[slot], position,
                                              &scale, 0);
          rock->y -= rock->z;
          rock->velocityX += rock->velocityY;
          if (rock->velocityX > 12)
            rock->velocityX -= 12;
          // Once it leaves the top it starts again from 128; from frame 88
          // it takes a new column and speed, and after 159 it stops.
          if (rock->y < 0) {
            if (frame > 159) {
              rock->variant = -1;
            } else {
              if (frame > 87) {
                rock->z = (Random16() & 7) + 8;
                if (effect->variant == 0)
                  rock->x = Random16() % 96 + 42;
                else if (effect->variant == 1)
                  rock->x = Random16() % 112 + 34;
                else
                  rock->x = Random16() % 160 + 10;
              }
              rock->y = 128;
              rock->variant = 8;
            }
          }
        } else {
          rock->variant--;
        }
      }
    }

    // The targets (frames 86-158): a flash every 12 frames, a recoil
    // every 4.
    if (frame <= 158) {
      for (i = 0; i != effect->count; i++) {
        if (frame > 85) {
          if (frame % 12 == 0)
            Func_0814cd48(effect->actors[i], 7, 5, i, 6);
          if ((frame & 3) == 0)
            Func_08118088(effect->actors[i], 5);
        }
      }
    }
    // While the wall stands the screen shakes hard, otherwise softly.
    if (frame < 90 || frame > 160)
      BattleCamera_Shake(2, 2);
    else
      BattleCamera_Shake(8, 8);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  // Teardown.
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(HEAP_BLITTER_ALTERNATE);
  HeapSlot_Release(HEAP_BLITTER);
  for (i = 0; i != ROCK_COUNT; i++)
    ResourceObject_ReleaseFar(work->objects[i]);
  BattleEffect_EndCanvasLayer();
}

#include "sprite.h"

/// Takes the first free animation entry of the 64 and clears it for use. With
/// none free it clears the entry at address 0.
struct AnimationEntry *AnimationObject_AllocateEmpty(void) {
  struct AnimationEntry *entry = Ram_HeapSlots->unk0c[0];
  struct AnimationEntry *found = NULL;
  s32 i = 0;
  // The script's NULL held from the start lives longest, so the word zero
  // takes r3 and the byte zero r2; as a plain NULL, 24 of 30 instructions
  // differ in tla-en.
  void *none = NULL;

  while (i <= 63) {
    if (entry->kind == 0) {
      found = entry;
      break;
    }
    i++;
    entry++;
  }
  found->animId = 0;
  found->scripts = NULL;
  found->pictures = NULL;
  found->script = none;
  found->mode = 0;
  found->picture = 0;
  found->pos = 0;
  found->kind = 1;
  found->param = 0;
  return found;
}

#include "vram_tab.h"

struct AnimationEntry;

struct EffectSprite {
  s32 unk00;
  s32 attributes;
  union {
    u32 word;
    struct {
      u32 tile : 10;
      u32 rest : 22;
    } bits;
  } oam;
  s32 unk0c;
  u8 vramEntry;
  u8 unk11_0 : 1;
  u8 unk11_1 : 7;
  u16 unk12;
  u8 width;
  u8 height;
  u8 unk16;
  u8 unk17;
  u8 unk18[2];
  u8 unk1a;
  u8 unk1b;
  s32 unk1c;
  s32 unk20;
  s32 unk24;
  struct AnimationEntry *entry;
  u8 unk2c[0x0c];
};

/// Makes an effect sprite with tiles of its own in object VRAM: a free VRAM
/// cache entry and the first free effect sprite of the 64, as Func_0815b3b0
/// takes, set up for Animation_ApplyChildArgumentFar. The Lost Age's own.
struct EffectSprite *Func_0815b290(s32 width, s32 height, s32 attributes,
                                   s32 flags) {
  struct EffectSprite *found = NULL;
  s32 vramEntry = Resource_FindFreeEntry();
  struct EffectSprite *sprite = Ram_HeapSlots->unk0c[1];
  s32 i = 0;
  struct ObjectSystemWork *objects;
  s32 size;
  s32 tile;
  struct VramBlockCacheEntry *entries;

  while (i <= 63) {
    if (sprite->width == 0) {
      found = sprite;
      break;
    }
    i++;
    sprite++;
  }
  tile = VramCache_Load(vramEntry, 0, 0);
  found->vramEntry = vramEntry;
  found->unk12 = 0;
  found->unk1a = 0;
  found->unk11_0 = 0;
  {
    u32 *words = (u32 *)found;

    *words++ = 0;
    *words++ = attributes;
    *words = flags | tile | 0x800;
  }
  {
    s32 *p = &found->unk1c;

    *p = 0;
  }
  found->unk20 = 0x6000;
  entries = gResourceTableEntries;
  found->unk24 = (((u16 *)entries)[187] >> 5) | 0x800;
  sprite->entry = AnimationObject_AllocateEmpty();
  sprite->width = width;
  sprite->unk0c = 0x10000;
  sprite->height = height;
  sprite->unk17 = 0;
  sprite->unk16 = 0;
  sprite->unk1b = 1;
  sprite->unk1a = 0;
  Animation_ApplyChildArgumentFar(sprite, 0);

  objects = Ram_HeapSlots->objectSystem;
  size = sprite->width * sprite->height;
  if (!(attributes & 0x2000))
    size = (u32)size >> 1;
  VramCache_Load(sprite->vramEntry, size, 0);
  objects->used += size;
  sprite->oam.bits.tile = entries[sprite->vramEntry].offset >> 5;
  return found;
}

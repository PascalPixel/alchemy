// Draft of BattleEffect_RunProjectileVolley, the body of the volley family:
// the user fires a volley of shots at each target, staggered by shot and
// by target, and each shot that reaches its target flashes and throws
// sparks. The mode picks the shot's picture and flight; its wrappers are C
// in projectile_volley.c.

// Modes, by the sheets they load and the shots they draw:
// 0, 5, 8      comets (kind by strength; mode 5 an icy comet, mode 8
// always the first), drawn as two mirrored halves, with
// large flashes on impact
// 1            spinning shuriken
// 2            bouncing pellets
// 3, 4, 11     yellow sparks, three sizes
// 6, 12        fireballs trailing smoke (mode 12 from twice the user's x)
// 7, 9, 10, 18 ice chips (10 and 18 start two steps ahead)
// 13           a blue beam, its sheet halved into place
// 14           tumbling crystals
// 15, 16, 17   streaks falling to the ground; 16 raises smoke and 17
// sprays glints of water around the first target
// A Mercury Djinn may circle the user first (effect->prelude).

// Twin: The Broken Seal's BattleEffect_RunProjectileVolley (effect.c), modes
// 0-11. The Lost Age adds modes 12-18, the falling-streak particles, and
// no longer opens the canvas differently for mode 10.
// Listing 2278 instructions, draft 2282, differ 1002.

// What remains: the control flow and the calls are the native's, block for
// block, and the four positions sit at the native's frame slots (screen
// 188, world 176, origin 164, position 152). The rows left are allocation
// and order:
// - mode lives in r10 here and in r9 in the native, which keeps steps in
// r10; most branches on mode differ by that register alone.
// - the shot and target loops (about 80 rows): the native keeps
// i * shots, i * delay, the actor offset and the flash pointer as
// spilled induction variables at sp+24..44 in its own order.
// - the comet, crystal and glint draws (about 25 rows each): argument
// order and the reuse of table bases.
// - four instructions more than the native, in the comet halves.
// - literal pools placed from these lengths, so most pool offsets after
// the first differ by a few words.

// Tried (differ):
// the twin's modes 0-11 extended with the Lost Age's 12-18     1266
// the four positions declared in the native's slot order       1258
// velocity cases 14 and 9 written apart, as the native tests   1474
// ... with 10 and 18 apart as well                             1203
// 10 and 18 apart alone                                        1245
// index declared in the frame loop, as the twin (kept)         1169
// shift reused as the smoke counter and spawned declared in
// the hit block, as the twin                                   2224

// Changes from the twin: the effect argument is read directly (the Lost
// Age never stores work->effect), blitters load through BattleEffect_LoadBlitter and
// Func_08196404 with the Lost Age's mode numbers (max is 1, additive 0),
// the callback order is 0xc80, the canvas preparer takes its flags packed,
// the volley time counts count - 1 target delays, and a shot's z velocity
// is worked out before its y velocity.

#include "types.h"
#include "battle_display.h"
#include "battle_effect.h"
#include "battle_effect_work.h"
#include "effect_step.h"
#include "io_reg.h"
#include "iwram_call.h"
#include "motobj.h"
#include "ram_buffer.h"

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
extern u8 Data_08198475[];  // _CometHeights, by kind
extern u8 Data_08198478[];  // _Modes: five bytes for each mode
extern u8 Data_081984d7[];  // _FlashWidths
extern u8 Data_081984de[];  // _FlashHeights
extern u8 Data_081984e5[];  // _FlashSteps
extern u8 Data_081984ec[];  // _FlashRises
extern u16 Data_081984f4[]; // _FlashOffsets
extern u16 Data_08198502[]; // _PelletOffsets
extern u8 Data_0819850a[];  // the glints' palettes
// gBattleEffect_MoteCells; gBattleEffect_GlintCell* (offsets, widths,
// heights); gPrism_ShardOffsets and the shards' widths and heights.
extern u16 gData_08197410[];
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

void BattleEffect_BeginCanvasLayer(s32 control);
void BattleEffect_EndCanvasLayer(void);
void Func_0815585c(struct BattleEffectArgument *effect, s32 flags,
                   struct EffectPosition *screen, struct EffectPosition *world);
s32 BattleEffect_LoadBlitter(s32 slot, s32 modeFlags);
s32 Func_08196404(s32 slot, s32 widthShift, s32 heightShift, s32 flags,
                  u32 mode);
s32 Scheduler_AddOrUpdateCallback(s32 (*callback)(void), s32 order);
s32 Scheduler_RemoveCallback(s32 (*callback)(void));
void Runtime_ReleaseHeapBlock(s32 slot);
void *Resource_GetTableEntry(s32 resourceId);
void Audio_PlayCue(s32 cue);
void Func_081180e8(s32 phase);
u32 Random16(void);
s32 Trig_Sin(s32 angle);
s32 Trig_Cos(s32 angle);
void Func_0814cd48(s32 actor, s32 palette, s32 frames, s32 timerSlot,
                   s32 timerFrames);
void Func_08118088(s32 actor, s32 motion);
void Func_08158ce0(s32 x, s32 y);
void ObjectGroup_TickMemberTimers(void);
void WaitFrames(s32 frames);
s32 BattleUnit_GetEffectHeightFar(s32 unit);
void Render_ResetTransformState(void);
void Transform_SetLookAt(s32 *eye, s32 *target);
void EffectPosition_ProjectBattler(s32 id,
                                                 struct EffectPosition *pos);
void BattleEffectKernels_IntegrateVector2(struct EffectStep *step, s32 damping,
                                      s32 gravity);

// BattleEffect_LoadBlitter(slot, mode << 4 | flags) loads a blitter into a heap slot.
#define BLITTER_ADDITIVE (0 << 4)
#define BLITTER_MAX (1 << 4)
#define CLIP (COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y)

// The second blitter, loaded again for each draw that needs its own.
#define DRAW_ALTERNATE ((DrawRectangle)Ram_HeapSlots->unkb8[1])

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
  s32 spawned;
  s32 x;

  // Set up: the work, canvas, camera and sprite sheet, the canvas layer and
  // the Djinn's place when it circles first.
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  camera = Ram_HeapSlots->cameraWork;
  sheet = Ram_HeapSlots->unk64[0];
  seWait = 0;
  kind = effect->variant;
  BattleEffect_BeginCanvasLayer(1);
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
    if (mode == 5)
      Iwram_CopyWords((void *)BG_PLTT,
                      Resource_GetTableEntry((s32)&ResourceId_IceBlockSheet),
                      128);
    Resource_LoadAndDecompress((s32)&ResourceId_WindStreakSheet,
                               work->sheet + 0x2000, 1, 0);
    if (mode == 5)
      Iwram_CopyWords((void *)BG_PLTT,
                      Resource_GetTableEntry((s32)&ResourceId_IceBlockSheet),
                      128);
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
    u8 *beam = work->sheet;

    Resource_LoadAndDecompress((s32)&ResourceId_BlueBeamSheet, beam, 1, 1);
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
  Scheduler_AddOrUpdateCallback(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);

  // Where each shot starts and how fast it flies.
  user = GetBattleObjectSlotFar(effect->actor)->object;
  shots = Data_08198478[mode * 5 + VOLLEY_SHOTS];
  steps = Data_08198478[mode * 5 + VOLLEY_STEPS];
  {
    s32 stagger = Data_08198478[mode * 5 + VOLLEY_STAGGER];
    s32 delay = Data_08198478[mode * 5 + VOLLEY_DELAY];
    s32 height = BattleUnit_GetEffectHeightFar(effect->actor);

    if (effect->count * shots > 63) // the shots must fit particles[64]
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
          Audio_PlayCue(CUE_FIRE);
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
          BattleEffect_LoadBlitter(188, BLITTER_ADDITIVE | CLIP | COMPOSITE_FLIP_X);
        else
          BattleEffect_LoadBlitter(188, BLITTER_ADDITIVE | CLIP);
        draw[1] = DRAW_ALTERNATE;
        if (frame <= 3)
          draw[1](canvas, work->sheet + 0x65c0, djinnX, djinnY, 20, 40);
        draw[1](canvas, work->sheet + 0x65c0, djinnX, djinnY, 20, 40);
        Runtime_ReleaseHeapBlock(188);
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

          if (j * stagger + i * delay >= frame)
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
            spawned = 0;
            for (k = PTCL_SMOKE; k != PTCL_STREAKS; k++) {
              struct EffectStep *p = &PARTICLES[k];

              if (p->variant == 0) {
                p->x = position.x << 16;
                p->y = position.y << 16;
                p->velocityX = ((Random16() & 255) - 128) << 11;
                p->velocityY = ((Random16() & 255) - 128) << 11;
                p->variant = (Random16() & 7) + 32;
                spawned++;
                if (spawned == 2)
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
              /* FAKEMATCH: the twin's cancelling updates, which end CSE
               * before the side test and emit nothing (effect.c). */
              if (target) {
                hit ^= x;
                hit ^= x;
              }
              if (!hit)
                goto draw_shot;
            } else {
              hit = x >= 0;
              /* FAKEMATCH: the same, for the other side. */
              if (target) {
                hit ^= x;
                hit ^= x;
              }
              if (!hit)
                goto draw_shot;
            }
            if (ABS(x) >= ABS(target->x)) {
              struct EffectStep *flash = &PARTICLES[index];
              s32 shift;

              spawned = 0;
              shot->variant = 1;
              if (mode == 5) {
                Func_081180e8(PHASE_LATE_RESULTS);
              } else if (mode != 6 && mode != 12 && mode != 13 && mode != 14 &&
                         seWait == 0) {
                seWait = 8;
                Audio_PlayCue(CUE_IMPACT);
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
                for (k = 0; k != 1; k++)
                  PARTICLES[PTCL_STREAKS + j + k].variant = 0;
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
                    if (spawned == Data_08198478[mode * 5 + VOLLEY_SPARKS])
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
              BattleEffect_LoadBlitter(188, BLITTER_MAX | CLIP | COMPOSITE_FLIP_X);
            else
              BattleEffect_LoadBlitter(188, BLITTER_MAX | CLIP);
            DRAW_ALTERNATE(canvas, work->sheet + 0x1000, pos->x - 16,
                           pos->y - Data_08198475[kind], 32,
                           Data_08198475[kind]);
            Runtime_ReleaseHeapBlock(188);
            if (effect->side == 1)
              BattleEffect_LoadBlitter(188, BLITTER_MAX | CLIP | COMPOSITE_FLIP);
            else
              BattleEffect_LoadBlitter(188, BLITTER_MAX | CLIP | COMPOSITE_FLIP_Y);
            DRAW_ALTERNATE(canvas, work->sheet + 0x1000, pos->x - 16, pos->y,
                           32, Data_08198475[kind]);
            Runtime_ReleaseHeapBlock(188);
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
            s32 width = gData_0819781a[cel];
            s32 height = gData_08197826[cel];

            draw[0](canvas, work->sheet + 0x800 + gData_08197834[cel],
                    pos->x - (width >> 1), pos->y - (height >> 1), width,
                    height);
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
        for (i = 0; i != shots * effect->count; i++) {
          struct EffectStep *flash = &PARTICLES[i];

          if (flash->variant == 1) {
            s32 step = flash->z / 2;

            if (!(i & 1)) {
              BattleEffect_LoadBlitter(188, BLITTER_MAX | CLIP);
              DRAW_ALTERNATE(canvas, work->sheet + Data_081984f4[step],
                             flash->x - Data_081984d7[step],
                             flash->y - Data_081984ec[step],
                             Data_081984d7[step], Data_081984de[step]);
              Runtime_ReleaseHeapBlock(188);
              BattleEffect_LoadBlitter(188, BLITTER_MAX | CLIP | COMPOSITE_FLIP);
              DRAW_ALTERNATE(
                  canvas, work->sheet + Data_081984f4[step], flash->x,
                  flash->y + Data_081984e5[step] - Data_081984ec[step],
                  Data_081984d7[step], Data_081984de[step]);
              Runtime_ReleaseHeapBlock(188);
            } else {
              BattleEffect_LoadBlitter(188, BLITTER_MAX | CLIP);
              DRAW_ALTERNATE(canvas, work->sheet + Data_081984f4[step] + 0x128a,
                             flash->x - Data_081984ec[step],
                             flash->y - Data_081984d7[step],
                             Data_081984de[step], Data_081984d7[step]);
              Runtime_ReleaseHeapBlock(188);
              BattleEffect_LoadBlitter(188, BLITTER_MAX | CLIP | COMPOSITE_FLIP);
              DRAW_ALTERNATE(
                  canvas, work->sheet + Data_081984f4[step] + 0x128a,
                  flash->x + Data_081984e5[step] - Data_081984ec[step],
                  flash->y, Data_081984de[step], Data_081984d7[step]);
              Runtime_ReleaseHeapBlock(188);
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
        s32 glint = frame & 3;

        for (i = 0; i != 64; i++) {
          struct EffectStep *p = &PARTICLES[PTCL_STREAKS + i];

          if ((u32)p->variant <= 7) {
            s32 angle = Random16() & 0xffff;
            s32 radius = (Random16() & 31) + 4;
            s32 glintX = HI(p->x) + ((radius * Trig_Sin(angle)) >> 17) -
                         (gData_08197492[glint] >> 1);
            s32 glintY = HI(p->y) - ((radius * Trig_Cos(angle)) >> 17) -
                         (gData_08197498[glint] >> 1);

            Func_08196404(188, 7, 7, Data_0819850a[Random16() & 3] | CLIP, 0);
            DRAW_ALTERNATE(canvas, work->sheet + 0x22 + gData_08197486[glint],
                           glintX, glintY, gData_08197492[glint],
                           gData_08197498[glint]);
            Runtime_ReleaseHeapBlock(188);
            p->variant++;
          }
        }
      }
      Func_08158ce0(4, 4);
      ObjectGroup_TickMemberTimers();
      work->transferPending = 1;
      WaitFrames(1);
    }
  }

  // Tear down: stop the transfer, free the blitter, close the layer.
  Scheduler_RemoveCallback(BattleEffect_ProcessPendingGraphicsTransfer);
  Runtime_ReleaseHeapBlock(104);
  BattleEffect_EndCanvasLayer();
}

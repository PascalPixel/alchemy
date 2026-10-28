// Draft, differ 50 of 1681 (the two nested helpers first, then the body;
// alchemy diff DRAFT 08164c0c,08164c70,08164cc4):
// BattleEffect_RunParticleStreams, summon 11 and summon 0's default
// (BattleEffect_RunSceneEffect's default case, through BattleEffect_RunMeteor and
// BattleEffect_RunOuterSpace in particle_stream_modes.c), with its nested
// helpers AddFlame (Func_08164c0c) and AddMote (Func_08164c70, unused), which
// adopt with it (S1 notes; alchemy permute cannot parse them).

// Twin: ☀️ BattleEffect_RunParticleStreams (games/tbs/src/effect/effect.c),
// ported: the heap blocks read from Ram_HeapSlots (canvas first), the effect
// kept as the argument, Func_08163c2c for the canvas wipe, the fills through
// IwramFillWords where ☀️ uses DMA, mode 1's two objects resource 638 + i *
// 0x2001 (☀️ 483), mode 0's spawn 650 (☀️ 381), the palette lines at 0x7640 in
// the work (☀️ 0x1f80), the blitter modes counted down from 3, and the callback
// orders 0xc80, 0xcfe and 0xcff (☀️ 0x480, 0x4fe, 0x4ff).

// Devices, as ☀️'s twin has them (each tag gives its measured difference):
// the mode test through direction, the frame count and work pinned in the
// heat loop with their barrier, the r6 clobber around the first cue, and the
// level pin. The two nested helpers, AddFlame (08164c0c) and AddMote
// (08164c70), now match alone (2026-10-26, under -mthumb-slow-loads; they
// differed by 4 rows each before it).

// Left, 50 rows (2026-10-26; 73 before the IWRAM calls went through local
// inline forwarders, as f143d80.c makes them and as The Broken Seal's IWRAM
// call layer makes every IWRAM call, which reloads the map cell buffer for
// each fill as the native does): the prologue's argument spill and heap
// reads in the other order (the 0x03000000 base first here), and loads
// issued one slot earlier in the native, the known store-then-load
// scheduling limit; and the constant 3 that the native keeps in r6 across
// the blitter build for the transfer mode, where here it is set again.

// Tried (differ): the plain port without devices and with the unused fill
// word ☀️ needs for its DMA (877 listed parent first, 709 helpers first);
// without fill, which fixes the frame of the nested functions' chain (546);
// the heat devices one by one (477 without the clobber block, 236 without
// the pins, 219 without the level pin); mode tested directly (189).

#include "battle_display.h"
#include "battle_effect.h"
#include "battle_effect_work.h"
#include "battle_session.h"
#include "bg_scroll.h"
#include "task.h"
#include "fixed_math.h"
#include "input.h"
#include "io_reg.h"
#include "iwram_call.h"
#include "motobj.h"
#include "ram_buffer.h"
#include "resource.h"
#include "system.h"
#include "types.h"

static __inline__ s32 CopyWords_(void *destination, const void *source,
                                 s32 size) {
  return Iwram_CopyWords(destination, source, size);
}

static __inline__ s32 FillWords_(void *destination, s32 size, u32 value) {
  return Iwram_FillWords(destination, size, value);
}

// The pixel half of a 16.16 fixed-point value.
#define HI(v) (((s16 *)&(v))[1])

// The heap blocks the dispatcher hands every canvas effect, as byte offsets
// into Ram_HeapSlots (☀️ uses slot indices).
#define HEAP_BLITTER 104

// The map cell buffer: first a 256 x 128 picture the streams draw into (BG1's
// tiles), then, in the second act, the flashes and the embers.
#define FLASHES ((struct EffectStep *)Ram_MapCellBuffer)
#define EMBERS (&FLASHES[128])

// The scanline palette the HBlank DMA feeds into BG colour 0, in the work.
#define HEAT_LINES(work) ((u16 *)((u8 *)(work) + 0x7640))

// BattleEffect_LoadBlitter's argument: The Lost Age's blitter mode (☀️'s counted down
// from 3) in the high nibble, the COMPOSITE_ flags in the low one.
#define BLITTER(mode, flags) ((mode) << 4 | (flags))
#define BLIT_MAX 1

#define STREAM_FRAMES 192
#define BLAST_FRAMES 54
#define BLAST_FLASH_FRAME 16 // the flashes rise from here
#define BLAST_HIT_FRAME 48

#define PHASE_SUMMON_RESULTS 134
#define SE_RISE 145
#define SE_STREAMS 141
#define SE_BLAST 156

struct Scale {
  s32 x;
  s32 y;
};

struct FxAnimObject {
  u8 unk00[9];
  u8 unk09_0 : 2;
  u8 priority : 2;
  u8 unk09_4 : 4;
  u8 unk0a[16];
  u8 flags;
};

struct EffectCamera {
  s32 eye[3];
  s32 pos[3];
};

struct ScaledSprite;

extern char ResourceId_FlameSheetB;
extern char ResourceId_LightningBoltSheet;
extern char ResourceId_BlastSheet;
extern s32 gCameraSceneParameters[];
extern s16 Data_08198a02[][3]; // ☀️ gParticleStreams_OrbitPoints
extern s16 Data_08198a2c[][2]; // ☀️ gParticleStreams_DropPoints
extern u16 Data_08198a48[];    // ☀️ gParticleStreams_FlashCells
extern u16 Data_08198a56[];    // ☀️ gParticleStreams_FlashSizes
extern u16 gData_08197410[];   // ☀️ gBattleEffect_MoteCells

void BattleEffect_StartCanvasLayer(s32 bgControl);
void Func_0814cd48(s32 actor, s32 palette, s32 a, s32 timer,
                   s32 frames); // BattleEffect_TintBattler
void Sound_PlayFar(s32 cue);
void Func_0813ba50(void); // BattleEffect_ConfigureEffectDisplay
void BattleEffect_ProcessPendingGraphicsTransfer(void);
void Func_08163c2c(s32 mode, s32 value);
void Animation_ApplyChildArgumentFar(void *object, s32 animation);
void BattleEffect_SpawnObjects(s32 count, s32 resource,
                   s32 priority);
void BattleEffect_SelectLivingTargets(
    struct BattleEffectArgument *effect);
void BattleEffect_AdvanceBg2OnInterval(void);
void BattleEffect_FlushPendingGraphicsTransfer(void);
void BattleEffect_ArmPaletteHBlankDma(void);
s32 Func_08196404(s32 slot, s32 widthShift, s32 heightShift, s32 flags,
                  s32 mode);                   // BattleEffect_BuildBlitter
s32 BattleEffect_LoadBlitter(s32 slot, s32 modeAndFlags); // BattleEffect_BuildBlitter(slot,
                                               // 7, 7, flags, mode)
void Render_ApplyProjectedPlacementFar(struct ScaledSprite *sprite,
                                       s32 *position, struct Scale *scale,
                                       s32 mode);
void Render_ResetTransformState(void);
void SceneTransform_ApplyPosition(s32 *position);
void Func_080150e4(s32 roll); // SceneTransform_ApplyRoll
void SceneTransform_ApplyYaw(s32 yaw);
void Func_0814cca8(void); // BattleEffect_SetupBlendedDisplay
void Func_08164a4c(s32 red, s32 green, s32 blue); // Palette_BrightenBgEntries
void BattleActor_CommitPlacementFar(void);
void Func_081180e8(s32 phase); // BattlePlayback_StartFar
void Transform_SetLookAt(s32 *eye, s32 *position);
void BattleEffectKernels_IntegrateVector3(struct EffectStep *step, s32 damping,
                                      s32 gravity);
void Func_08158ce0(s32 x, s32 y); // Camera_ApplyShake
void BattleEffect_EndCanvasLayer(void);

// Summon 11: the screen goes dark and the ground dissolves away from the top;
// a sun (or, in mode 1, two great flames the user throws) climbs over a
// spinning ring of seven orbs while burning drops rain and the sky glows from
// a heat palette under every scanline; then the scene bursts: embers fountain
// up, flashes rise from the target, and the sun swells over the enemies.
//
// Act one (192 frames, A or B ends it after frame 4, or 16 in mode 1): the
// dark backdrop, a noise of 256 bright pixels on the canvas, the ground
// eaten away row by row, the HBlank heat palette and the BG2 scroll; each
// frame the heat lines, the objects, a new mote, the orbs, the drops (each
// becomes a flame where it lands) and the motes. Act two (54 frames): the
// blast pictures, 32 motes, 128 flashes and 512 embers; the palette
// brightens from frame 36, the hit at 48.
void BattleEffect_RunParticleStreams(struct BattleEffectArgument *effect,
                                     s32 mode) {
  u8 offsets[128];
  void *canvas;
  DrawRectangle draw;
  s32 direction;
  struct BattleEffectWork *work;

  // A flame on the ground where a drop lands, in the first free of 16
  // slots after the motes.
  void AddFlame(s32 x, s32 y, s32 velocityX) {
    s32 i;

    for (i = 0; i != 16; i++) {
      struct EffectStep *flame = &work->particles[32 + i];

      if (flame->variant == -1) {
        flame->variant = 0;
        flame->x = x;
        flame->y = y;
        flame->velocityX = velocityX;
        break;
      }
    }
  }

  // A mote in the first free of the 32 slots (unused).
  void AddMote(s32 x, s32 y) {
    s32 i;

    for (i = 0; i != 32; i++) {
      struct EffectStep *mote = &work->particles[i];

      if (mote->variant == -1) {
        mote->variant = 0;
        mote->x = x;
        mote->y = y;
        break;
      }
    }
  }

  s32 position[4];
  s32 vector[3];
  struct EffectPosition spot;
  struct EffectPosition *pos;
  struct EffectPosition point;
  struct Scale scale;
  s32 frame;
  s32 i;

  canvas = Ram_HeapSlots->battleFxWork[1];
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  BattleEffect_StartCanvasLayer(0x2000);
  REG_BG2PA = 0x100;

  // Mode 1: the user leaps and flashes; the streams lean the user's way.
  /* FAKEMATCH: tested through direction so that CSE keeps the mode reload
   * in this arm, as ☀️'s twin; testing mode directly turns ldr r4, [sp, #76]
   * into movs r4, #1 (differ 189 instead of 187). */
  direction = mode;
  if (direction == 1) {
    struct MotionObject *user = GetBattleObjectSlotFar(effect->actor)->object;

    user->velocityY = 0xa0000;
    user->verticalMotionStrength = 0x91eb;
    Func_0814cd48(effect->actor, -1, 2, -1, 0);
    Sound_PlayFar(SE_RISE);
    direction = effect->side == 1 ? mode : -1;
  } else {
    direction = -1;
  }

  // A dark backdrop and an empty canvas.
  Func_0813ba50();
  *(u16 *)BG_PLTT = 0;
  *(u16 *)(BG_PLTT + 1) = 0;
  work->transferMode = 0;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);
  Func_08163c2c(0, 0);
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);

  // Mode 1's two flames, or mode 0's sun.
  if (mode == 1) {
    for (i = 0; i != 2; i++) {
      struct FxAnimObject *flame = ResourceObject_CreateFar(638 + i * 0x2001);

      work->objects[i] = flame;
      if (flame != 0) {
        flame->flags = 0;
        Animation_ApplyChildArgumentFar(flame, 2);
        ((struct FxAnimObject *)work->objects[i])->priority = 3;
      }
    }
  } else {
    BattleEffect_SpawnObjects(1, 650, 3);
  }
  Resource_LoadAndDecompress((s32)&ResourceId_FlameSheetB, work->sheet, 1, 1);
  if (mode == 1)
    CopyWords_((void *)BG_PLTT,
                    Resource_GetTableEntry((s32)&ResourceId_LightningBoltSheet),
                    128);

  // BG1 shows the ground: the map cell buffer filled with colour 1.
  FillWords_(Ram_MapCellBuffer, 0x8000, 0x01010101);
  CopyWords_((void *)0x06008000, Ram_MapCellBuffer, 0x7800);
  wait(1);
  REG_BLDCNT = 0;
  REG_BG2PA = 0x100;
  REG_BG1CNT = 0x1f80;
  REG_BG2CNT = 0x2787;

  // 63 pale colours, and 256 of them scattered over the canvas.
  for (i = 0; i != 63; i++) {
    s32 red = (Random16() & 15) + 16;
    s32 green = (Random16() & 15) + 16;
    s32 blue = (Random16() & 15) + 16;

    ((u16 *)0x05000100)[i] = (blue << 10) | (green << 5) | red;
  }
  FillWords_(canvas, 0x4000, 0);
  for (i = 0; i != 256; i++) {
    s32 x = Random16() & 127;
    s32 y = Random16() & 127;
    s32 shade = (Random16() & 63) + 64;

    ((u8 *)canvas)[((y / 8 * 16 + x / 8) * 8 + (y & 7)) * 8 + (x & 7)] = shade;
  }
  CopyWords_((void *)0x06004000, canvas, 0x4000);
  gCameraSceneParameters[4] = 240;
  BattleEffect_SelectLivingTargets(work->effect);

  // BG2 scrolls a pixel every other frame, down and toward the user's side.
  work->bg2x = 0;
  work->bg2y = 0;
  work->scrollTimer = 0;
  work->scrollInterval = 2;
  work->scrollStepX = direction << 7;
  work->scrollStepY = 256;
  Task_Add(BattleEffect_AdvanceBg2OnInterval, TASK_PASS_FRAME + 127);
  Task_Add(BattleEffect_FlushPendingGraphicsTransfer, TASK_PASS_FRAME);

  // The ground dissolves: each column starts at its own random row, and the
  // rows go faster and faster.
  for (i = 0; i != 128; i++)
    offsets[i] = Random16() & 63;
  {
    s32 limit = 0;
    s32 step = 1;
    s32 row = 0;

    do {
      limit += step / 2;
      step += 4;
      for (; row != limit; row++) {
        s32 x;

        for (x = 0; x != 256; x++) {
          s32 y = row - offsets[x & 127];

          if (y >= 0)
            if (y <= 127)
              Ram_MapCellBuffer[((y / 8 * 32 + x / 8) * 8 + (y & 7)) * 8 +
                                (x & 7)] = 0;
        }
      }
      work->transferPending = 1;
      wait(1);
    } while (limit <= 191);
  }
  REG_BLDCNT = 0x3f42;
  REG_BLDALPHA = 0x1010;
  {
    s32 savedX = gBgScroll[1].x;
    s32 savedY = gBgScroll[1].y;
    struct BattlePresentationTransition *transition =
        Ram_HeapSlots->transitionWork;

    gBgScroll[1].x = 0;
    gBgScroll[1].y = 32;
    Func_08196404(HEAP_BLITTER, 8, 7, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y,
                  BLIT_MAX);
    draw = (DrawRectangle)Ram_HeapSlots->unk64[1];
    work->transferMode = 3;
    work->transferValue = 0x02020202;
    Task_Add(BattleEffect_ArmPaletteHBlankDma, TASK_PASS_FRAME + 126);
    for (i = 0; i != 64; i++)
      work->particles[i].variant = -1;
    transition->active = 1;
    work->frame = 0;
    frame = 0;
    do {
      s32 angle = frame << 8;
      /* FAKEMATCH: the frame count pinned to r2 and the work's copy to r0, as
       * in ☀️'s twin, so that the division's temp stays canonical and the line
       * pointer starts from r0 (differ 236 without these and the barrier
       * below, 187 with them). */
      register s32 count asm("r2") = work->frame;
      /* FAKEMATCH: see count. */
      register u8 *base asm("r0") = (u8 *)work;
      s32 heat = count / 4;
      u16 *line;
      s32 size;
      s32 x;
      s32 y;

      /* FAKEMATCH: count read and base passed through, keeping count's load
       * apart from the division (☀️'s measured form). */
      asm("" : "=r"(base) : "0"(base), "r"(count));
      line = HEAT_LINES(base);

      if (mode == 1) {
        if ((gInput.repeat & 3) && frame > 16)
          break;
      } else {
        if ((gInput.repeat & 3) && frame > 4)
          break;
      }
      {
        s32 hold;

        /* FAKEMATCH: hold takes r7 across the call and r6 is clobbered, so
         * heat takes r4 and the native's sp+8 caller-save around the cue
         * (differ 477 without this block, 187 with it). */
        asm("" : "=r"(hold));
        /* FAKEMATCH: see hold. */
        asm("" : : : "r6");
        if (frame == 0)
          Sound_PlayFar(SE_STREAMS);
        /* FAKEMATCH: see hold. */
        asm("" : : "r"(hold));
      }

      // The heat palette: black at the top and bottom, rising through red
      // to yellow as the frames pass.
      for (i = 0; i != 15; i++)
        *line++ = 0;
      for (; i != 135; i++) {
        s32 green = i - 16;
        s32 red = green;
        /* FAKEMATCH: level pinned to r2, as in ☀️'s twin: unpinned it
         * swaps registers with the quotient (differ 219 without it, 187 with
         * it). The quotient keeps the explicit bias of ☀️'s form. */
        register s32 level asm("r2");

        if (green < 0)
          red += 3;
        red >>= 2;
        level = red + heat;
        red = level - 32;
        green = level - 80;

        if (red < 0)
          red = 0;
        if (red > 31)
          red = 31;
        if (green < 0)
          green = 0;
        if (green > 31)
          green = 31;
        *line++ = (red << 10) | (green << 5) | (green >> 1);
      }
      for (; i != 160; i++)
        *line++ = 0;

      // The sun (or the flames) climbs, growing.
      if (direction == 1)
        x = frame / 4;
      else
        x = 64 - frame / 4;
      y = 96 - frame;
      position[3] = 0;
      position[1] = 0xff0000;
      if (mode == 1) {
        size = angle + 0xa000;
        scale.x = size;
        scale.y = size;
        position[0] = (x << 16) + 0x500000;
        position[2] = (64 - y) << 16;
        Render_ApplyProjectedPlacementFar(work->objects[0], position, &scale,
                                          0);
        Render_ApplyProjectedPlacementFar(work->objects[1], position, &scale,
                                          0);
      } else {
        size = angle + 0x10000;
        scale.x = size;
        scale.y = size;
        position[0] = (x << 16) + 0x600000;
        position[2] = (96 - y) << 16;
        Render_ApplyProjectedPlacementFar(work->objects[0], position, &scale,
                                          0);
      }

      // A new mote on the sun's rim.
      for (i = 0, y = 32 - y; i != 32; i++) {
        struct EffectStep *mote = &work->particles[i];

        if (mote->variant == -1) {
          s32 angle = (Random16() & 0x7fff) + 0x4000;

          mote->variant = 0;
          mote->x = ((x + 96) << 16) + Trig_Sin(angle) * 30 / 65536 * size;
          mote->y = (y << 16) - Trig_Cos(angle) * 30 / 65536 * size;
          break;
        }
      }

      // The ring of seven orbs, turning.
      point.x = 0;
      point.y = 0;
      point.depth = 0x2000000;
      Render_ResetTransformState();
      SceneTransform_ApplyPosition(&point.x);
      Func_080150e4(0x800);
      SceneTransform_ApplyYaw(frame << 8);
      for (i = 0; i != 7; i++) {
        vector[0] = (Data_08198a02[i][0] / 2) << 16;
        vector[1] = (Data_08198a02[i][1] + frame) << 16;
        vector[2] = (Data_08198a02[i][2] / 2) << 16;
        EffectPosition_ProjectPoint(vector, pos = &spot);
        pos->x = HI(pos->x) + 128;
        pos->y = HI(pos->y) + 60;
        draw(Ram_MapCellBuffer, work->sheet + 0x1f40, pos->x - 4, pos->y - 4, 8,
             8);
      }

      // The drops fall, and each becomes a flame where it lands.
      if (direction == 1)
        x = frame / 4 - 16;
      else
        x = 16 - frame / 4;
      y = frame - 96;
      for (i = 0; i != 7; i++) {
        s32 drop = Data_08198a2c[i][1] + y;

        if (drop <= 93)
          draw(Ram_MapCellBuffer, work->sheet + 0x1c80,
               Data_08198a2c[i][0] + x - 12, drop - 12, 24, 24);
        else if (drop <= 95)
          AddFlame((Data_08198a2c[i][0] + x) << 16, drop << 16, 1);
      }

      // The motes drift up and away, six pictures each.
      for (i = 0; i != 32; i++) {
        struct EffectStep *mote = &work->particles[i];

        if (mote->variant >= 0) {
          draw(Ram_MapCellBuffer, work->sheet + (mote->variant << 10),
               HI(mote->x) - 16, HI(mote->y) - 16, 32, 32);
          mote->x -= direction * 0x14000;
          mote->y -= 0x50000;
          mote->variant++;
          if (mote->variant == 6)
            mote->variant = -1;
        }
      }
      work->transferPending = 1;
      wait(1);
      frame++;
      work->frame++;
    } while (frame != STREAM_FRAMES);
    wait(1);
    transition->active = 0;
    Task_Remove(BattleEffect_AdvanceBg2OnInterval);
    Task_Remove(BattleEffect_ArmPaletteHBlankDma);
    Task_Remove(BattleEffect_FlushPendingGraphicsTransfer);
    gBgScroll[1].x = savedX;
    gBgScroll[1].y = savedY;
  }

  // Act two: the blast.
  HeapSlot_Release(HEAP_BLITTER);
  Func_0814cca8();
  REG_BG2PA = 0x80;
  REG_BG2X = 0;
  REG_BG2Y = 0xfffff000;
  REG_BLDALPHA = 0x1010;
  REG_BG2CNT = 0x2784;
  BattleEffect_LoadBlitter(HEAP_BLITTER,
                BLITTER(BLIT_MAX, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y));
  draw = (DrawRectangle)Ram_HeapSlots->unk64[1];
  Resource_LoadAndDecompress((s32)&ResourceId_BlastSheet, work->sheet, 1, 0);
  for (i = 0; i != 32; i++) {
    struct EffectStep *mote = &work->particles[i];

    mote->x = Random16() & 127;
    mote->y = (Random16() & 127) + 127;
  }
  for (i = 0; i != 128; i++) {
    struct EffectStep *flash = &FLASHES[i];

    flash->x = 0;
    flash->y = 0;
    flash->z = 0;
    flash->velocityX = ((Random16() & 255) - 127) << 12;
    flash->velocityY = (Random16() & 255) << 11;
    flash->velocityZ = ((Random16() & 255) - 127) << 12;
    flash->variant = 0;
  }
  for (i = 0; i != 512; i++) {
    struct EffectStep *ember = &EMBERS[i];

    ember->x = 0;
    ember->y = 0;
    ember->z = 0;
    ember->velocityX = ((Random16() & 255) - 128) << 13;
    ember->velocityY = (Random16() & 255) << 11;
    ember->velocityZ = ((Random16() & 255) - 128) << 13;
    ember->variant = 0;
  }
  work->transferMode = 1;
  work->transferValue = 0x10101010;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);

  for (frame = 0; frame != BLAST_FRAMES; frame++) {
    s32 size = (frame + 0x1d0) << 8;
    struct EffectCamera *camera = Ram_HeapSlots->cameraWork;
    s32 elapsed = frame - BLAST_FLASH_FRAME;
    struct Scale scale;
    s32 x;
    s32 y;
    s32 grow;

    if (elapsed > 19)
      Func_08164a4c(2, 2, 2);
    if (frame == 0)
      Sound_PlayFar(SE_BLAST);
    if (frame == 40)
      Sound_PlayFar(SE_RISE);
    if (frame == BLAST_HIT_FRAME) {
      if (mode == 1) {
        ResourceObject_ReleaseFar(work->objects[0]);
        ResourceObject_ReleaseFar(work->objects[1]);
        BattleActor_CommitPlacementFar();
      }
      Func_081180e8(PHASE_SUMMON_RESULTS);
    }

    // The embers fountain up from the ground in front of the camera.
    Render_ResetTransformState();
    Transform_SetLookAt(camera->eye, camera->pos);
    for (i = 0; i != 64; i++) {
      struct EffectStep *ember = &EMBERS[i];

      if (ember->y >= 0) {
        s32 size;

        EffectPosition_ProjectPoint(&ember->x, &point);
        point.x >>= 1;
        if (point.depth <= 159)
          point.depth = 160;
        if (point.depth > 799)
          point.depth = 799;
        size = 9 - (point.depth - 160) / 64;
        draw(canvas,
             work->sheet + (gData_08197410[size - 1] + (i & 1) * 770) + 0x3200,
             point.x - size / 2, point.y - size, size, size * 2);
        BattleEffectKernels_IntegrateVector3(ember, 64, -0x2000);
        if (ember->y <= 0x140000) {
          ember->x = 0;
          ember->z = 0;
          ember->y = 0x140000;
          ember->velocityX = ((Random16() & 63) - 32) << 15;
          ember->velocityY = (Random16() & 63) << 13;
          ember->velocityZ = ((Random16() & 63) - 32) << 15;
        }
      }
    }

    // The motes rise.
    for (i = 0; i != 64; i++) {
      struct EffectStep *mote = &work->particles[i];
      u32 rise = i & 7;
      s32 size = rise + 3;

      draw(canvas,
           work->sheet + (gData_08197410[size - 1] + (i & 1) * 770) + 0x3200,
           mote->x - size / 2, mote->y - size, size, size * 2);
      mote->y = mote->y - rise - 8;
      if (mote->y < -10)
        mote->y = 128;
    }

    // The flashes rise from the target, three more every frame.
    for (i = 0; i != 64; i++) {
      struct EffectStep *flash = &FLASHES[i];

      if (i / 3 < elapsed && flash->y >= 0) {
        EffectPosition_ProjectPoint(&flash->x, &point);
        point.x >>= 1;
        if ((u32)flash->variant <= 13) {
          s32 cell = flash->variant / 2;

          draw(canvas, work->sheet + Data_08198a48[cell],
               point.x - Data_08198a56[cell] / 2,
               point.y - Data_08198a56[cell] / 2, Data_08198a56[cell],
               Data_08198a56[cell]);
        }
        flash->variant++;
        if (flash->variant == 14) {
          flash->y = 0x140000;
          flash->x = 0;
          flash->z = ((Random16() & 255) - 127) << 16;
          flash->velocityX = 0;
          flash->velocityY = (Random16() & 255) << 11;
          flash->velocityZ = 0;
          flash->variant = 0;
        } else {
          BattleEffectKernels_IntegrateVector3(flash, 64, 1);
        }
      }
    }

    // The sun swells over the enemies.
    if (direction == 1)
      x = frame / 2 + 24;
    else
      x = 56 - frame / 2;
    y = 64 - frame * 2;
    grow = (frame << 8) + 0x20000;
    position[3] = 0;
    position[1] = 0xff0000;
    if (mode == 1) {
      scale.x = size;
      scale.y = size;
      position[0] = (x << 16) + 0x600000;
      position[2] = (96 - y) << 16;
      Render_ApplyProjectedPlacementFar(work->objects[0], position, &scale, 0);
      Render_ApplyProjectedPlacementFar(work->objects[1], position, &scale, 0);
    } else {
      scale.x = grow;
      scale.y = grow;
      position[0] = (x << 16) + 0x600000;
      position[2] = (96 - y) << 16;
      Render_ApplyProjectedPlacementFar(work->objects[0], position, &scale, 0);
    }
    work->shakeFrames = 1;
    Func_08158ce0(8, 8);
    work->transferPending = 1;
    wait(1);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(HEAP_BLITTER);
  if (mode == 0)
    ResourceObject_ReleaseFar(work->objects[0]);
  BattleEffect_EndCanvasLayer();
}

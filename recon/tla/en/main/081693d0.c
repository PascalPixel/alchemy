// Draft, differ 360 of 1632 (1628 here): BattleEffect_RunNeptune, summon 6
// (BattleEffect_RunSceneEffect's case 6, actflags 0x8006), The Broken Seal's
// BattleEffect_RunNeptune as The Lost Age reworked it.

// Twin: ☀️ BattleEffect_RunNeptune (games/tbs/src/effect/effect.c), with The
// Lost Age's changes: no BattleEffect_SpawnObjects; instead its own object build
// (the palette by Iwram_CopyWords to OBJ palette 0x200, the tiles by
// Resource_DecodeType01 into the map cell buffer, eight
// Func_0815b290(32, 32, 0x80002000, 0) with priority 1 and a 1 KB tile copy
// each to 0x06010000 + gResourceTableEntries[slot].offset, as summons 4 and 24
// do); battle work byte 0x867 set; the object grid computed from i (no
// ObjectX/Y tables); Palette_LoadBasicObjColors and a Dma_Set of 16 palette
// words after the first act; blitter modes 3 and 19 for The Broken Seal's 3
// and 2.

// Left: allocation (spill slots, and the tile pointer the native holds in r8
// from the start) and order.

// Tried (differ), in effects trial 2 (2026-10-15): the first port (449); the
// tile pointer in a local set first (446); work declared right after the
// canvas, found by a greedy scan of declaration positions (360); alchemy
// permute 6 minutes: 349 with a reused variable, not kept. Renaming the locals
// for this draft left 360. alchemy permute 10 minutes, 2 jobs, on this draft
// (2026-10-15): 278 to 298, each only through an invented temporary (the
// 0x03000000 base held in the frame variable, the canvas in a two-word array,
// a spare u16 pointer), not kept; they say the native holds the heap base in
// a register the frame later takes.
#include "types.h"
#include "ram_buffer.h"
#include "io_reg.h"
#include "system.h"
#include "fixed_math.h"
#include "callback_scheduler.h"
#include "input.h"
#include "iwram_call.h"
#include "bg_scroll.h"
#include "irq.h"
#include "dma.h"
#include "vram_tab.h"
#include "battle_effect.h"
#include "battle_effect_work.h"

// The pixel half of a 16.16 fixed-point value.
#define HI(v) (((s16 *)&(v))[1])

// The map cell buffer holds the spray (and, at the start, the serpent's
// tiles and the window's column noise).
#define PARTICLES ((struct EffectStep *)Ram_MapCellBuffer)

#define SERPENT_PARTS 8 // four across, two down, 48 pixels apart
#define SPRAY_COUNT 1024
#define SPARKLE_SHEET 0x4e20 // the sparkle cells after the spray picture

#define SCENE_FRAMES 244  // act one; A or B ends it after frame 16
#define OPEN_FRAMES 16    // the window opens over the sea
#define SERPENT_FRAME 22  // the serpent rises
#define SECOND_SPRAY 76   // a second fountain of spray
#define GATHER_FRAME 152  // light gathers at its mouth
#define RAYS_FRAME 176    // the rays reach out
#define BREATH_FRAME 222  // and it breathes
#define PILLAR_FRAMES 144 // act two
#define RESULTS_FRAME 96

#define PHASE_SUMMON_RESULTS 134
#define SE_RISE 162
#define SE_SPRAY 164
#define SE_GATHER 142
#define SE_BREATH 145
#define SE_PILLARS 390
#define SE_PILLAR 154

struct Scale {
  s32 x;
  s32 y;
};

struct SpriteObject {
  u16 unk00;
  u16 unk02;
  u16 unk04;
  u16 unk06;
  u16 tile : 10;
  u16 priority : 2;
  u16 palette : 4;
  u8 unk0a[6];
  u8 slot;
};

struct ScaledSprite;

extern char ResourceId_SeaSerpentTiles;
extern char ResourceId_DuskCloudsBackdrop;
extern char ResourceId_Data134; // ☀️ ParticleSpritesA
extern char ResourceId_WaterSpraySheet;
extern char ResourceId_ParticleSpritesD;
extern char ResourceId_BlueFlameSheet;
extern u32 gCameraSceneParameters[];
extern struct Scale Data_08196e8c; // the serpent's scale
extern u16 Data_08197424[];        // the sparkle cells by size
extern s32 Data_08198aec[];        // the spray's four gravities
extern u16 gData_08197410[];       // ☀️ gBattleEffect_MoteCells

void BattleEffect_BeginCanvasLayer(s32 bgControl);
void Func_0813ba50(void); // BattleEffect_ConfigureEffectDisplay
void BattleEffect_ProcessPendingGraphicsTransfer(void); // BattleEffect_ProcessPendingGraphicsTransfer
void Func_08163c2c(s32 mode, s32 value);        // Func_08163c2c
void BattleEffect_LoadBlitter(s32 slot, s32 modeAndFlags); // BattleEffect_BuildBlitter(slot,
                                                // 7, 7, flags, mode)
void BattleEffect_SelectLivingTargets(
    struct BattleEffectArgument *effect); // BattleEffect_SelectLivingTargets
struct SpriteObject *Func_0815b290(s32 width, s32 height, u32 attributes,
                                   u32 mode);
void Func_08143eb4(s32 x0, s32 y0, s32 x1, s32 y1,
                   s32 width); // BattleEffect_DrawClippedCanvasLine
void Func_0814cca8(void);      // BattleEffect_SetupBlendedDisplay
void Func_081180e8(s32 phase); // BattleEventRuntime_BeginPhaseFar
void Func_08118088(s32 actor,
                   s32 variant); // BattleMotion_ApplyVariantMotionFar
void Func_0814cd48(s32 actor, s32 palette, s32 a, s32 timer,
                   s32 frames);   // ObjectGroup_UpdateMembers
void Func_08158ce0(s32 x, s32 y); // Camera_ApplyShake
void ObjectGroup_TickMemberTimers(void);         // ObjectGroup_TickMemberTimers
void BattleEffect_EndCanvasLayer(void);         // BattleEffect_EndCanvasLayer
void Palette_LoadBasicObjColors(void);
void Graphics_ResetBg2Pa(void);
void Graphics_SetBg2AffineScaleHalf(void);
void BattleEffect_BuildWindowEdgeTable(void);
void BattleBackground_LoadFar(s32 layer, s32 resourceId, s32 mode);
void Audio_PlayCue(s32 cue);
void Render_ApplyProjectedPlacementFar(struct ScaledSprite *sprite,
                                       s32 *position, s32 *scale, s32 mode);
void ResourceObject_ReleaseFar(void *object);
void BattleEffectKernels_IntegrateVector2(struct EffectStep *step, s32 damping,
                                      s32 gravity);

// Summon 6: at dusk the window opens on a sea; a great serpent rises out of
// the spray, light gathers at its mouth, rays reach out and it breathes a
// blast; then blue flames fall as pillars on each enemy in turn, splashing.
//
// Act one (244 frames): the dusk backdrop, the serpent built from eight
// 32 x 32 sprites, the spray and sparkle pictures, 712 drops of spray; the
// window opens in 16 frames over a ragged edge; the serpent glides in from
// frame 22 and settles; a second fountain at frame 76; glints converge on its
// mouth from 152 and the glow grows; rays from 176; the breath from 222 with
// a ring of sparks; the sea rolls along the bottom all the while. Act two
// (144 frames): a flame pillar falls on each target 16 frames apart, splashing
// sparks where it lands and striking that target; results at frame 96.
void BattleEffect_RunNeptune(struct BattleEffectArgument *effect) {
  s32 position[4];
  s32 target[3];
  void *canvas;
  struct BattleEffectWork *work;
  u8 *cells;
  s32 openStep;
  s32 serpentX;
  s32 serpentY;
  s32 serpentVX;
  s32 serpentVY;
  s32 savedScrollX;
  s32 *transition;
  s32 j;
  s32 splashes;
  s32 pillarX;
  s32 frame;
  s32 beamFrame;
  s32 i;
  s32 rays;
  s32 glintSize;
  struct EffectStep *glintSeed;
  struct EffectStep *glint;
  struct EffectStep *spark;
  struct EffectStep *pillar;
  DrawRectangle draw[2];
  struct Scale scale;
  s32 ground = 112;
  u8 *tiles;

  canvas = Ram_HeapSlots->battleFxWork[1];
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  tiles = Ram_MapCellBuffer;
  cells = Ram_HeapSlots->unk64[0];

  // Open the canvas over a dark backdrop; BG2 is drawn at half scale
  // below the line the IRQ handler picks.
  BattleEffect_BeginCanvasLayer(0);
  Func_0813ba50();
  *(u16 *)0x05000000 = 0;
  *(u16 *)0x05000002 = 0;
  Scheduler_AddOrUpdateCallback(Graphics_ResetBg2Pa, 0xc80);
  Runtime_SetIrqHandler(2, 0, Graphics_SetBg2AffineScaleHalf);
  work->transferMode = 0;
  Scheduler_AddOrUpdateCallback(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);
  REG_WININ = 0x2137;
  Func_08163c2c(1, 0);
  REG_WIN0H = 0xf0f0;
  BattleEffect_SelectLivingTargets(work->effect);
  // Build the serpent: its palette, its tiles, and eight sprites.
  ((u8 *)Ram_HeapSlots->battleWork)[0x867] = 1;
  {
    u8 *sheet = Resource_GetTableEntry((s32)&ResourceId_SeaSerpentTiles);

    Iwram_CopyWords((void *)0x05000200, sheet, 0x200);
    sheet += 0x200;
    Resource_DecodeType01(sheet, tiles);
    for (i = 0; i != 8; i++) {
      struct SpriteObject *object = Func_0815b290(32, 32, 0x80002000, 0);

      object->priority = 1;
      work->objects[i] = object;
      Iwram_CopyWords(
          (void *)(0x06010000 + gResourceTableEntries[object->slot].offset),
          tiles, 0x400);
      tiles += 0x400;
    }
  }
  gCameraSceneParameters[4] = 240;
  WaitFrames(1);
  BattleBackground_LoadFar(1, (s32)&ResourceId_DuskCloudsBackdrop, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, cells, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_WaterSpraySheet, work->sheet, 1,
                             1);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesD,
                             work->sheet + 0x4e20, 0, 0);
  REG_DISPCNT = 0x7741;
  REG_BG2PA = 0x80;
  REG_BLDALPHA = 0x1010;
  REG_BLDCNT = 0x3f44;
  // The serpent starts low on the right, rising to the left.
  openStep = 1;
  serpentX = 140 << 17;
  serpentY = 200 << 16;
  serpentVX = -0x40000;
  serpentVY = -0x80000;
  savedScrollX = gBgScroll[1].x;
  transition = (s32 *)Ram_HeapSlots->transitionWork;
  work->transferMode = 2;
  work->transferValue = 75;
  transition[4] = 1;
  BattleEffect_LoadBlitter(104, 3);
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  BattleEffect_LoadBlitter(188, 19);
  draw[1] = (DrawRectangle)Ram_HeapSlots->unkb8[1];
  REG_BG2CNT = 0x784;

  // The glints wait on a wide arc above; the first spray fountains up from
  // the bottom.
  for (i = 0; i != 1024; i++)
    PARTICLES[i].variant = -1;
  for (i = 0, glintSeed = work->particles; i != 32; i++, glintSeed++) {
    s32 a = (Random16() & 0x3fff) + 0x8000;
    s32 r = (Random16() & 127) + 255;

    glintSeed->x = (Trig_Sin(a) * r) >> 2;
    glintSeed->y = (Trig_Cos(a) * r) >> 2;
    glintSeed->velocityX = -glintSeed->x / 32;
    glintSeed->velocityY = -glintSeed->y / 32;
    glintSeed->variant = 0;
  }
  for (i = 0; i != 712; i++) {
    s32 r = Random16() & 255;
    s32 a = Random16() & 0xffff;

    PARTICLES[i].x = ((Trig_Sin(a) * r) >> 3) + 0x800000;
    PARTICLES[i].y = ((Trig_Cos(a) * r) >> 4) + (192 << 15);
    PARTICLES[i].velocityX = (-(Random16() & 0x1fff) - 0x800) << 5;
    PARTICLES[i].velocityY = (-(Random16() & 0xfff) - 0x1800) << 5;
    PARTICLES[i].variant = 0;
  }

  frame = 0;
  do {
    if (frame == 24)
      Audio_PlayCue(SE_RISE);
    if (frame == 76)
      Audio_PlayCue(SE_SPRAY);
    if (frame == 154)
      Audio_PlayCue(SE_GATHER);
    if (frame == 222)
      Audio_PlayCue(SE_BREATH);

    // The window opens: each column's edge from its own noise, the edge
    // moving faster each frame.
    if (frame >= 0 && frame < 16) {
      u16 *phase = (u16 *)Ram_MapCellBuffer;

      if (frame == 1) {
        u8 *noise = Ram_MapCellBuffer + 2;
        s32 k;

        for (k = 0; k != 128; k++)
          noise[k] = Random16() & 63;
        *phase = 0;
        Scheduler_AddOrUpdateCallback(BattleEffect_BuildWindowEdgeTable, 0xc80);
      }
      *phase += openStep;
      openStep += 3;
      if (frame == 15)
        Scheduler_RemoveCallback(BattleEffect_BuildWindowEdgeTable);
    }

    // The serpent rises, slows, then glides back right and settles.
    if (frame > 21) {
      scale = Data_08196e8c;
      if (frame <= 83) {
        if (frame > 48)
          serpentVX = serpentVX * 60 / 64;
        serpentVY = serpentVY * 60 / 64;
        if (serpentY > 0x840000)
          serpentVY -= 0x2000;
        else
          serpentVY += 0x2000;
        serpentX += serpentVX;
        serpentY += serpentVY;
      } else if (frame <= 170) {
        if (frame <= 107)
          serpentVX += 0x2000;
        serpentVX = serpentVX * 60 / 64;
        serpentX += serpentVX;
        if (frame <= 117) {
          if (serpentY > 0x8a0000)
            serpentVY -= 0x2000;
          else
            serpentVY += 0x2000;
          serpentVY = serpentVY * 56 / 64;
          serpentY += serpentVY;
        }
      }
      position[3] = 0;
      position[1] = 0;
      for (i = 0; i != 8; i++) {
        position[0] = ((i & 3) * 48 << 16) + serpentX - 0x200000;
        position[2] = ((i / 4) * 48 << 16) + serpentY - 0x180000;
        Render_ApplyProjectedPlacementFar(work->objects[i], position, &scale.x,
                                          0);
      }
    }

    // The second fountain fills the free spray.
    if (frame == 76) {
      for (i = 0; i != 1024; i++) {
        if (PARTICLES[i].variant == -1) {
          s32 r = Random16() & 255;
          s32 a = Random16() & 0xffff;

          PARTICLES[i].x = ((Trig_Sin(a) * r) >> 3) + 0x400000;
          PARTICLES[i].y = ((Trig_Cos(a) * r) >> 4) + (192 << 15);
          PARTICLES[i].velocityX = (32 - (Random16() & 63)) << 12;
          PARTICLES[i].velocityY = (-(Random16() & 31) - 16) << 13;
          PARTICLES[i].variant = 0;
        }
      }
    }
    if (frame == 152) {
      Runtime_SetIrqHandler(2, 96, Graphics_SetBg2AffineScaleHalf);
      work->fadeFrames = 24;
      work->fadeStep = 0;
    }

    // The glints converge on the mouth.
    beamFrame = frame - 152;
    if (beamFrame >= 0 && beamFrame < 88) {
      for (i = 0, glint = work->particles; i != 32; i++, glint++) {
        if (frame >= i / 4 + 152 && frame < i / 4 + 152 + 32) {
          s32 fx = HI(glint->x) + 112;
          s32 fy = HI(glint->y) + 62;
          s32 r = (i & 3) + 5;

          glintSize = r * 2;
          draw[0](canvas, work->sheet + Data_08197424[r - 1] + 0x4e20, fx - r,
                  fy - r, glintSize, glintSize);
          glint->x += glint->velocityX;
          glint->y += glint->velocityY;
        }
      }
    }

    // The breath: a ring of sparks streams from the mouth.
    if (frame == 222) {
      for (i = 0; i != 64; i++) {
        spark = &work->particles[i];
        spark->x = (Random16() & 15) - 8;
        spark->y = (Random16() & 15) - 8;
        if (spark->x < 0)
          spark->x -= 4;
        else
          spark->x += 4;
        if (spark->y < 0)
          spark->y -= 4;
        else
          spark->y += 4;
        spark->x += 100;
        spark->y += 52;
        spark->velocityX = -6;
        spark->z = spark->x;
        spark->velocityY = spark->y;
        spark->variant = 0;
      }
    }
    if (frame > 221) {
      for (i = 0; i != 64; i++) {
        u16 *sparkCells = Data_08197424;
        s32 r = 1;

        if (frame >= i / 2 + 222) {
          draw[0](canvas, work->sheet + sparkCells[r - 1] + 0x4e20,
                  work->particles[i].x - r, work->particles[i].y - r, r * 2,
                  r * 2);
          work->particles[i].x += work->particles[i].velocityX;
          work->particles[i].y += work->particles[i].velocityY;
          if (work->particles[i].y < 0) {
            work->particles[i].y = work->particles[i].velocityY;
            work->particles[i].x = work->particles[i].z;
          }
        }
      }
    }

    // The rays: lines fanning out from the mouth, widest at the breath.
    if (frame > 175) {
      rays = 5;
      if (frame > 221) {
        rays = 60;
        if (frame != 223 && frame != 224)
          rays = 40;
      }
      if (rays > 60)
        rays = 60;
      for (i = 0; i != rays; i++) {
        s32 w;
        s32 d = (i + 1) / 2;

        w = rays * 2 - d * 4;
        if (w < 12)
          w = 12;
        if (w > 63)
          w = 63;
        if (i & 1)
          d = -d;
        Func_08143eb4(d + 48, 0, d / 2 + 111, 63 - (d + 1) / 2, w);
      }
    }

    // The glow at the mouth grows, then the breath's own pictures.
    if (beamFrame >= 0 && beamFrame < 40) {
      s32 z;

      z = beamFrame / 2 + 1;
      if (z > 4)
        z = 4;
      draw[1](canvas, work->sheet + Data_08197424[z - 1] + 0x4e20, 108 - z,
              60 - z, z * 2, z * 2);
    }
    if (frame >= 192 && frame < 200) {
      s32 z;

      z = (frame - 192) / 2 + 3;
      if (z > 6)
        z = 6;
      draw[1](canvas, work->sheet + Data_08197424[z - 1] + 0x4e20, 108 - z,
              60 - z, z * 2, z * 2);
    }
    if (frame > 199) {
      s32 z;
      s32 u = frame - 200;

      z = u / 4 + 5;
      if (z > 8)
        z = 8;
      draw[1](canvas, work->sheet + Data_08197424[z - 1] + 0x4e20, 108 - z,
              60 - z, z * 2, z * 2);
      if (frame <= 213) {
        z = u / 4 + 1;
        if (z > 4)
          z = 4;
        draw[1](canvas, work->sheet + Data_08197424[z - 1] + 0x4e20, 100 - z,
                52 - z, z * 2, z * 2);
      }
    }
    if (frame >= 214 && frame < 222) {
      s32 z;

      z = (frame - 214) / 4;
      if (z > 2)
        z = 2;
      draw[1](canvas, work->sheet + z * 9 * 256 + 0x400, 63, 18, 48, 48);
    }
    if (frame > 221) {
      s32 z;

      z = (frame - 222) / 2 % 4 + 3;
      draw[0](canvas, work->sheet + z * 9 * 256 + 0x400, 72, 30, 48, 48);
      if (z == 5)
        draw[0](canvas, work->sheet + 0x1600, 66, 22, 48, 48);
    }

    // The spray, rising and falling back into the sea.
    if (frame > 27) {
      DrawRectangle drawSpray = draw[1];

      for (i = 0; i != 1024; i++) {
        s32 m = 3;

        if (PARTICLES[i].variant >= 0) {
          s32 k = i % 3 + 2;

          drawSpray(canvas, cells + gData_08197410[k - 1],
                    HI(PARTICLES[i].x) - k / 2, HI(PARTICLES[i].y) - k, k,
                    k * 2);
          BattleEffectKernels_IntegrateVector2(&PARTICLES[i], 62,
                                           Data_08198aec[i & m]);
          PARTICLES[i].variant++;
          if (PARTICLES[i].velocityY > 0 && HI(PARTICLES[i].y) > 104)
            PARTICLES[i].variant = -1;
        }
      }
    }

    // The sea rolls along the bottom.
    for (i = 0; i != 5; i++)
      draw[1](canvas, work->sheet, i * 32 + ((frame / 4) & 31) - 32, 88, 32,
              32);
    work->transferPending = 1;
    WaitFrames(1);
  } while (++frame != 244 && (frame <= 16 || !(gInput.repeat & 3)));

  // Act two: restore the display; a pillar waits above each target.
  gBgScroll[1].x = savedScrollX;
  transition[4] = 0;
  Runtime_SetIrqHandler(2, 0, 0);
  Func_0814cca8();
  Palette_LoadBasicObjColors();
  Dma_Set((const void *)0x05000200, (void *)0x050000a0, 0x80000010,
          (volatile u32 *)0x040000d4);
  *(u16 *)0x050000bc = *(u16 *)0x050001e8;
  Scheduler_RemoveCallback(Graphics_ResetBg2Pa);
  REG_WIN0H = 0xf0;
  work->transferMode = 2;
  work->transferValue = 75;
  for (i = 0; i != 1024; i++)
    PARTICLES[i].variant = 0;
  for (i = 0; i != 16; i++) {
    work->particles[i].x = (Random16() & 31) + 32;
    work->particles[i].y = 0;
    work->particles[i].variant = 0;
  }
  for (i = 0; i != effect->count; i++) {
    EffectPosition_ProjectScaledBattler(effect->actors[i],
                                       (struct EffectPosition *)target);
    work->particles[i].x = target[0] / 2;
  }
  Resource_LoadAndDecompress((s32)&ResourceId_BlueFlameSheet, work->sheet, 1,
                             1);
  Audio_PlayCue(SE_PILLARS);
  REG_BG2PA = 0x80;
  REG_BLDALPHA = 0x1010;

  frame = 0;
  do {
    if (frame == 96)
      Func_081180e8(134);
    // Each pillar falls 16 pixels a frame and splashes where it lands.
    for (i = 0; i != 5; i++) {
      pillar = &work->particles[i];
      if (frame == i * 16 + 7)
        Audio_PlayCue(SE_PILLAR);
      if (frame >= i * 16 && pillar->variant <= 31) {
        s32 wobble = frame * 16 % 104;

        draw[1](canvas, work->sheet, pillar->x - 8, pillar->y + wobble - 216,
                17, 104);
        draw[1](canvas, work->sheet, pillar->x - 8, pillar->y + wobble - 112,
                17, 104 - wobble);
        draw[1](canvas, work->sheet + 0x6e8, pillar->x - 17, pillar->y - 65, 34,
                65);
        if (pillar->y <= 111)
          pillar->y += 16;
        else
          pillar->variant++;
        if (pillar->y > 111) {
          splashes = 0;
          pillarX = pillar->x;
          if (pillar->variant <= 7)
            work->shakeFrames = 4;
          for (j = 0; j != 1024; j++) {
            if (PARTICLES[j].variant == 0) {
              s32 r = 0x3ff;
              s32 a;

              r &= Random16();
              a = (Random16() & 0x7fff) - 0x4000;
              PARTICLES[j].x = pillarX << 16;
              PARTICLES[j].y = ground << 16;
              r += 32;
              PARTICLES[j].velocityX = (Trig_Sin(a) * r) >> 7;
              PARTICLES[j].velocityY = -(Trig_Cos(a) * r * 2) >> 7;
              PARTICLES[j].variant = (Random16() & 7) + 32;
              if (++splashes == 4)
                break;
            }
          }
        }
      }
    }
    for (i = 0; i != effect->count; i++) {
      if (frame == i * 16 + 8) {
        Func_0814cd48(effect->actors[i], 7, 5, i, 8);
        Func_08118088(effect->actors[i], 2);
      }
    }
    for (i = 0; i != 1024; i++) {
      if (PARTICLES[i].variant > 0) {
        s32 x;
        s32 y;

        PARTICLES[i].variant--;
        BattleEffectKernels_IntegrateVector2(&PARTICLES[i], 60, 0x4000);
        y = PARTICLES[i].y >> 16;
        if (y > 120) {
          PARTICLES[i].velocityY = -PARTICLES[i].velocityY / 2;
        } else if (PARTICLES[i].x >= 0 && (x = PARTICLES[i].x >> 16) <= 126 &&
                   PARTICLES[i].y >= 0) {
          s32 k = PARTICLES[i].variant / 8 + 3;

          draw[i & 1](canvas, cells + gData_08197410[k - 1], x - k / 2, y - k,
                      k, k * 2);
        }
      }
    }
    Func_08158ce0(4, 8);
    ObjectGroup_TickMemberTimers();
    work->transferPending = 1;
    WaitFrames(1);
  } while (++frame != 144);

  for (i = 0; i != 8; i++)
    ResourceObject_ReleaseFar(work->objects[i]);
  Scheduler_RemoveCallback(BattleEffect_ProcessPendingGraphicsTransfer);
  Runtime_ReleaseHeapBlock(188);
  Runtime_ReleaseHeapBlock(104);
  BattleEffect_EndCanvasLayer();
}

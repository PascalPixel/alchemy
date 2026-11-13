/// @file
/// Eclipse, then the canvas layer: transfers, opening and closing, tile maps,
/// lines.
// Part of battle_effect.c.

#include "../battle_effect_internal.h"
#include "gamepak/far_call/battle.h"
#include "module/battle_effect/battle_effect.h"
#include "module/battle_effect/battle_effect.h"
#include "module/system/system.h"
#include "resource_ids.h"

struct Model;

// A texture for the IWRAM renderer.
/* VIEW(f14153c): tag struct ModelTexture as struct ModelTexture__f14153c;
 * render_model.h has another definition; tried with the definition the rest of
 * the object has, BattleEffect_RunEclipse moves: 1 of 2142 instructions,
 * Func_08142944 moves: 1 of 730 instructions in tla-ja. */
struct ModelTexture__f14153c {
  u32 logWidth : 8;
  u32 logHeight : 8;
  u32 unk02 : 16;
  const u8 *pixels;
};

/* VIEW(f14153c): tag struct RenderModel as struct RenderModel__f14153c;
 * render_model.h has another definition. */
struct RenderModel__f14153c {
  s32 kind;
  s32 unk04;
  const void *mesh;
  void *vertices;
  struct ModelTexture__f14153c *texture;
  s32 shade;
  u8 scrollU;
  u8 scrollV;
};

void Render_DrawModelList(struct Model *model);

// The blitter's heap slot, as a byte offset into Ram_HeapSlots.
#define SLOT_BLITTER 104 // unk64[1]

#define BLITTER(mode, flags) ((mode) << 4 | (flags))

// Rows of Resource_DirectoryTable; the directory has yet to name most of them.
extern char ResourceId_EarthHorizonBackdrop, ResourceId_AerialViewBackdrop;
extern char ResourceId_GoldenDragonTiles, ResourceId_GoldenDragonHeadTiles;
extern char ResourceId_BlueFlameSheet, ResourceId_DustBlobSheet;
extern char ResourceId_Data134, ResourceId_TornadoSheet, ResourceId_Data0B4;
extern char ResourceId_Data0C1, ResourceId_Data0C9, ResourceId_Data0CA;

extern u8 gMapCellBuffer[];
/* VIEW(f14153c): variable gData_02014000 as extern struct EffectStep
 * gData_02014000[]; f13eb70 has extern u8 gData_02014000[]; tried as the rest
 * of the object declares it, it does not compile in tla-ja:
 * games/tla/src/battle_effect/battle_effect.c:4528: `gData_02014000__f14153c'
 * undeclared (first use in this function). */
extern struct EffectStep gData_02014000__f14153c[] __asm__("gData_02014000");

struct SpriteObject *Func_0815b290(s32 width, s32 height, u32 shape,
                                   u32 attributes);

#define SLOT_ALT 188 // unkb8[1]
#define OBJ_TILES(obj) (0x06010000 + gResourceTableEntries[(obj)->slot].offset)

// Forwarded so that a constant size reaches the call's register at each call.

// Eclipse: the battlefield scrolls up, thirty-two objects (31 with
// Summon_GoldenDragonTiles, 1 with Summon_GoldenDragonHeadTiles) rise over
// the foes, a ring flares and collapses, then a beam strikes and a sphere
// bursts.
// XXX: what the player sees in each stage; a tbs-en-style render of the
// effect would settle it.
void BattleEffect_RunEclipse(struct BattleEffectArgument *effect) {
  u8 *canvas;
  struct BattleEffectWork *work;
  // Unused: blit[1] gives the stack frame its 4 bytes.
  DrawRectangle blit[2];
  s32 frame;
  s32 savedScroll;
  s32 *transition;
  u8 *sprites;
  u8 *battle;
  s32 i;
  s32 j;
  s32 k;
  s32 scroll;
  s32 baseX;
  s32 baseY;
  s32 velX;
  s32 velY;
  s32 cx;
  s32 cy;
  u32 u;
  s32 count;
  s32 ring;
  s32 shade;
  s32 offset;
  u8 *cells;
  s32 fade;
  s16 list[14];
  s32 place[4];
  struct WordPair req;
  struct ModelTexture__f14153c texture;
  struct ModelTexture__f14153c texture2;
  struct ModelTexture__f14153c texture3;
  struct RenderModel__f14153c *model;
  struct EffectStep *p;
  struct EclipseObject *obj;

  /* FAKEMATCH(wrapper): every fill through FillWords_, which sets a constant
   * size in the call's register at each call; as direct Iwram_FillWords calls,
   * 501 of 2159 instructions differ in tla-en. */
  /* FAKEMATCH(wrapper): every copy through CopyWords_, which sets a constant
   * size in the call's register at each call; as direct Iwram_CopyWords calls,
   * 133 of 2159 instructions differ in tla-en. */
  canvas = Ram_HeapSlots->battleFxWork[1];
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  savedScroll = gBgScroll[1].x;
  transition = (s32 *)Ram_HeapSlots->transitionWork;
  sprites = Ram_HeapSlots->unk64[0];
  battle = (u8 *)Ram_HeapSlots->battleWork;
  BattleEffect_StartCanvasLayer(0);
  Func_08179e6c();
  work->transferMode = 0;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 3200);
  scroll = -80;
  for (frame = 0; frame != 52; frame++) {
    if (frame == 27) {
      u8 *tiles;

      list[0] = 0xff;
      BattleActor_SpawnObjectsForListFar(list, 0);
      Resource_ResetEntry(*(s32 *)(battle + 84));
      VramBlock_Reset93Far();
      gCameraSceneParameters[4] = 240;
      BattleBackground_LoadFar(1, (s32)&ResourceId_EarthHorizonBackdrop, 0);
      work->scrollTimer = 0;
      work->scrollInterval = 1;
      work->scrollStepX = -1;
      work->scrollStepY = 0;
      Task_Add(BattleEffect_AdvanceScrollOnInterval, 0x480);
      transition[4] = 1;
      cells = Resource_GetTableEntry((s32)&ResourceId_GoldenDragonTiles);
      Iwram_CopyWords((void *)OBJ_PLTT, cells, 480);
      cells += 480;
      Resource_DecodeType01(cells, gMapCellBuffer);
      tiles = gMapCellBuffer;
      for (i = 0; i != 31; i++) {
        obj = Func_0815b290(32, 32, 0x80002000, 0);
        obj->priority = 2;
        work->objects[i] = obj;
        Iwram_CopyWords((void *)OBJ_TILES(obj), tiles, 0x400);
        tiles += 0x400;
      }
      cells = Resource_GetTableEntry((s32)&ResourceId_GoldenDragonHeadTiles);
      Iwram_CopyWords((void *)OBJ_PLTT, cells, 480);
      cells += 480;
      Resource_DecodeType01(cells, gMapCellBuffer);
      obj = Func_0815b290(32, 32, 0x80002000, 0);
      work->objects[31] = obj;
      obj->priority = 2;
      Iwram_CopyWords((void *)OBJ_TILES(obj), gMapCellBuffer, 0x400);
    }
    scroll += 4;
    REG_BG2X = scroll << 8;
    work->transferPending = 1;
    wait(1);
  }
  Iwram_FillWords(canvas, 0x3c00, 0);
  Iwram_FillWords((void *)0x06004000, 0x3c00, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, sprites, 0, 0);
  REG_DISPCNT = 0x7741;
  REG_BG2PA = 0x80;
  REG_BLDALPHA = 0x1010;
  REG_BLDCNT = 0x3f44;
  REG_BG2X = 0;
  baseX = 0;
  baseY = 0;
  velX = 0;
  velY = 0;
  work->transferMode = 2;
  work->transferValue = 50;
  BattleEffect_LoadBlitter(SLOT_BLITTER, 19);
  blit[0] = Ram_HeapSlots->unk64[1];
  Iwram_CopyWords((void *)BG_PLTT,
                  Resource_GetTableEntry((s32)&ResourceId_BlueFlameSheet), 128);
  {
    const u8 *texturePixels = (const u8 *)gData_02014000__f14153c;

    for (frame = 0; frame != 220 && !(gInput.repeat & 3); frame++) {
      if (frame == 0) {
        baseX = -0x300000;
        baseY = 0xd40000;
        velX = 0x40000;
        velY = -0xa0000;
        work->fadeFrames = 1;
        work->fadeStep = frame;
        work->particles[16].variant = frame;
        Resource_LoadAndDecompress((s32)&ResourceId_Data0B4,
                                   gData_02014000__f14153c, 0, 0);
      }
      if (frame == 136) {
        u8 *dst = Data_02010400;
        for (i = 0; i != 7; i++) {

          void *o = (void *)OBJ_TILES(
              (struct EclipseObject *)work->objects[Data_08197748[i]]);
          // Default: set for real below; as s32 size = 0x400, 762 differ.
          s32 size = 0;
          size = 0x400;
          Iwram_CopyWordsEntry(o, &dst[i * 0x400], size);
        }
      }
      if (frame == 48) {
        Sound_PlayFar(139);
      }
      if (frame == 136) {
        Sound_PlayFar(140);
      }
      if (frame == 172) {
        Sound_PlayFar(144);
      }
      for (i = 0; i != 17; i++) {
        if (frame == Data_0819774f[i]) {
          Iwram_FillWords(canvas, 0x3c00, Data_08197760[i]);
        }
      }
      req = Data_08196de0;
      place[3] = 0;
      place[1] = 0xff0000;
      baseX += velX;
      baseY += velY;
      if (frame > 171) {
        if (frame > 183) {
          velX += -0x200;
          velY += 0x400;
        }
        if (frame <= 205) {
          velX = velX * 54 / 64;
          velY = velY * 54 / 64;
        }
      } else {
        if (velY < 0) {
          velY += 0x4000;
        }
        if (velX > 0) {
          velX += -0x2000;
        }
        if (frame > 31) {
          baseY += Trig_Sin((frame - 32) << 9) << 2 >> 4;
          if (frame <= 159) {
            baseX += 0x4000;
          }
        }
      }
      for (i = 0; i != 32; i++) {
        if (i == 25 && frame > 135) {
          place[0] = baseX + 0x200000;
          place[2] = baseY + 0x400000;
        } else {
          place[0] = baseX + (Data_081977a4[i] << 16);
          place[2] = baseY + (Data_081977c4[i] << 16);
        }
        if (place[2] <= 0xc7ffff) {
          Render_ApplyProjectedPlacementFar(work->objects[i], place,
                                            (s32 *)&req, 0);
        }
      }
      if (frame == 136) {
        work->fadeFrames = 24;
        work->fadeStep = 0;
      }
      cx = baseX / 2 - (work->particles[16].variant << 13) + 0x1c0000;
      cy = baseY + (work->particles[16].variant << 14) + 0x380000;
      u = frame - 136;
      if (u <= 84) {
        if (frame == 136) {
          work->particles[16].variant = 4;
          work->particles[16].x = 0;
          work->particles[16].z = 0;
          Func_0815b434(work->sheet, work->particles[16].variant * 2, 0x10000);
        }
        if (frame > 171) {
          if (frame == 172) {
            work->particles[16].variant = 64;
            Func_0815b434(work->sheet, 128, 0x10000);
            velX = 0x30000;
            velY = -0x30000;
          }
          if (frame > 203 && work->particles[16].variant > 1) {
            work->particles[16].variant -= 4;
            if (work->particles[16].variant <= 1) {
              work->particles[16].variant = 2;
            }
            Func_0815b434(work->sheet, work->particles[16].variant * 2,
                          0x10000);
          }
        } else if (frame > 135) {
          velX += -0x800;
          velY += 0x800;
          if (++work->particles[16].variant <= 32) {
            Func_0815b434(work->sheet, work->particles[16].variant * 2,
                          0x10000);
          } else {
            work->particles[16].variant = 32;
          }
        }
        {
          /* FAKEMATCH(temporary): the variant copied into size in a block of
           * its own; as Func_0818caa8(..., work->particles[16].variant * 2), 50
           * of 2159 instructions differ in tla-en. */
          s32 size = work->particles[16].variant;

          Func_0818caa8(work->sheet, cx >> 16, cy >> 16, size * 2);
        }
      }
      if (frame > 47) {
        struct RenderModel__f14153c *model;
        void *verts;
        s32 modelShade;

        if (frame == 48) {
          for (i = 0; i != 16; i++) {
            work->particles[i].x = Random16() & 0xffff;
            work->particles[i].y = (Random16() & 255) + 128;
            if (Random16() & 1) {
              work->particles[i].y = -work->particles[i].y;
            }
            work->particles[i].variant = i * -8;
          }
          Resource_LoadAndDecompress((s32)&ResourceId_TornadoSheet,
                                     work->sheet + 0x2000, 1, 0);
          Resource_LoadAndDecompress((s32)&ResourceId_Data0C9,
                                     work->sheet + 0x4000, 0, 0);
        }
        verts = Heap_AllocEwram(32);
        model = Render_AllocateModelList(1);
        modelShade = 0;
        model->shade = modelShade;
        texture.logWidth = 5;
        texture.logHeight = 2;
        texture.pixels = work->sheet + 0x4000;
        model->kind = 6;
        model->mesh = Data_08199220;
        model->texture = &texture;
        model->vertices = verts;
        for (i = 0; i != 8; i++) {
          work->particles[i].variant++;
          work->particles[i].x += work->particles[i].y;
          if (work->particles[i].variant >= 0 &&
              work->particles[i].variant <= 47) {
            Render_ResetTransformState();
            {
              s32 x = ((i & 3) << 17) + 0x20000;
              s32 o = -0x400000;
              Func_08015160(cx + x + o, cy - (((i & 3) + 2) << 16) + o, 0);
            }
            SceneTransform_ApplyScale3(0x8000, 0x10000, 0x10000);
            SceneTransform_ApplyRoll(work->particles[i].x);
            SceneTransform_ApplyScale3(
                Trig_Sin(work->particles[i].variant * 682) + 0x4000, 0x20000,
                0x10000);
            Render_TransformVertices(gData_081991b0, verts, 4);
            Render_DrawModelList(model);
          }
        }
        if (frame >= 136 && frame <= 171) {
          s32 s = 0x40000 - u * 6000;

          modelShade = u * 4 - 64;
          if (modelShade > -16) {
            modelShade = -16;
          }
          texture.logWidth = 6;
          model->shade = modelShade;
          texture.logHeight = 6;
          model->kind = 7;
          texture.pixels = texturePixels;
          model->texture = &texture;
          model->mesh = gData_08199340;
          model->vertices = verts;
          Render_ResetTransformState();
          Func_08015160(cx + -0x3e0000, cy + -0x420000, 0);
          SceneTransform_ApplyScale3(s >> 1, s, s);
          SceneTransform_ApplyPitch(0x2000);
          SceneTransform_ApplyRoll(0x2800);
          SceneTransform_ApplyYaw(-frame << 10);
          Render_TransformVertices(gData_08199210, verts, 4);
          Render_DrawModelList(model);
        }
        if (frame >= 172 && frame <= 220) {
          Render_ResetTransformState();
          Func_08015160(cx + -0x3e0000, cy + -0x420000, 0);
          SceneTransform_ApplyScale3(0x8000, 0x10000, 0x10000);
          SceneTransform_ApplyRoll(0x2800);
          if (frame > 203) {
            SceneTransform_ApplyScale3(0x80000 - ((frame - 204) << 15), 0x20000,
                                       0x10000);
          } else {
            SceneTransform_ApplyScale3(0x80000, 0x20000, 0x10000);
          }
          Render_TransformVertices(Data_081991c0, verts, 4);
          Render_DrawModelList(model);
        }
        Heap_Release(model);
        Heap_Release(verts);
      }
      if (frame >= 148 && frame <= 219) {
        for (j = 0; j != 4; j++) {
          s32 t = (frame + j) & 3;
          // Default: set for real below.
          s32 x = 0;
          s32 y = 0;

          if (frame <= 171) {
            s32 size = work->particles[16].variant * 2;

            x = Random16() % size + (cx >> 16) - (size >> 1) -
                (gData_08197492[t] >> 1);
            y = Random16() % size + (cy >> 16) - (size >> 1) -
                (gData_08197498[t] >> 1);
          } else {
            u32 a = Random16() & 63;
            s32 r = work->particles[16].variant;

            x = (cx >> 16) - (a >> 1) - (gData_08197492[t] >> 1) +
                Random16() % r - r / 2 - 8;
            y = a + (cy >> 16) - (gData_08197498[t] >> 1);
          }
          Func_08196404(SLOT_ALT, 7, 7, Data_08197744[Random16() & 3] | 3, 1);
          ((DrawRectangle)Ram_HeapSlots->unkb8[1])(
              canvas, work->sheet + 0x2000 + gBattleEffect_GlintCells[t], x, y,
              gData_08197492[t], gData_08197498[t]);
          HeapSlot_Release(SLOT_ALT);
        }
      }
      HeapSlot_Release(SLOT_ALT);
      work->transferPending = 1;
      wait(1);
    }
  }
  Task_Remove(BattleEffect_AdvanceScrollOnInterval);
  gBgScroll[1].x = savedScroll;
  transition[4] = 0;
  for (i = 0; i != 32; i++) {
    ResourceObject_ReleaseFar(work->objects[i]);
  }
  VramBlock_Load93Far();
  *(s32 *)(battle + 84) = VramCache_AllocEntry(128);
  BattleEffect_SelectLivingTargets(work->effect);
  for (i = 0; i != effect->count; i++) {
    BattleEffect_TintBattler(effect->actors[i], 1, -1, -1, 0);
    BattleEffect_TintBattler(effect->actors[i], 0, -1, -1, 0);
  }
  Palette_LoadBasicObjColors();
  REG_DISPCNT = 1;
  gBgScroll[1].y = 32;
  BattleBackground_LoadFar(1, *(u16 *)(battle + 0x670), 24);
  // Two empty once-only blocks before the fills: without them, 18 differ.
  do {
  } while (0);
  Iwram_FillWords(canvas, 0x4000, 0);
  do {
  } while (0);
  Iwram_FillWords((void *)0x06004000, 0x4000, 0);
  wait(1);
  REG_BLDALPHA = 0x1010;
  REG_BLDCNT = 0x3f46;
  REG_DISPCNT = 0x7741;
  gCameraSceneParameters[4] = 120;
  work->transferMode = 3;
  work->transferValue = 0x02020202;
  Iwram_FillWords(canvas, 0x4000, 0x3f3f3f3f);
  REG_BG2CNT = 0x784;
  effect->actors[effect->count] = 0xff;
  BattleActor_SpawnObjectsForListFar(effect->actors, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data0CA, work->sheet + 0x3840, 0,
                             0);
  REG_BLDALPHA = 0x1010;
  for (i = 0; i != 120; i++) {
    s32 v = i / 2;

    if (v > 63) {
      v = 63;
    }
    for (j = 0; j != 120; j++) {
      work->sheet[j * 120 + i] = v;
    }
  }
  fade = 0;
  for (frame = 0; frame != 48; frame++) {
    if (frame > 39) {
      work->transferMode = 4;
      work->transferValue = 0x04040404;
    }
    if (frame == 16) {
      Sound_PlayFar(140);
    }
    if (frame == 18) {
      work->shakeFrames = 90;
      Sound_PlayFar(145);
      Iwram_FillWords(canvas, 0x4000, 0x3f3f3f3f);
      for (i = 0; i != effect->count; i++) {
        BattleEffect_TintBattler(effect->actors[i], 7, 5, i, 4);
      }
    }
    if (frame <= 109) {
      // Default: set for real below.
      struct RenderModel__f14153c *model = 0;
      void *verts = 0;

      verts = Heap_AllocEwram(32);
      model = Render_AllocateModelList(1);
      texture2.logWidth = 5;
      texture2.logHeight = 2;
      texture2.pixels = work->sheet + 0x3840;
      model->mesh = Data_08199220;
      model->shade = fade;
      model->texture = &texture2;
      model->vertices = verts;
      model->kind = 6;
      if (frame > 15) {
        struct EffectStep *q = &work->particles[63];

        if (frame == 16) {
          q->x = 0x800000;
          q->y = -0x280000;
        }
        if (frame <= 19) {
          q->x += -0x200000;
          q->y += 0x400000;
        }
        if (frame <= 27) {
          cells = work->sheet + 0x38c0;
          Func_0815b434(cells, 96, 0x10000);
          Func_0818caa8(cells, q->x >> 16, q->y >> 16, 96);
        }
        Render_ResetTransformState();
        Func_08015160(q->x + -0x3f0000, q->y + -0x400000, 0);
        SceneTransform_ApplyScale3(0x8000, 0x10000, 0x8000);
        SceneTransform_ApplyRoll(0x9800);
        SceneTransform_ApplyScale3(0xdc500, 0x29c40, 0x8000);
        Render_TransformVertices(Data_081991c0, verts, 4);
        Render_DrawModelList(model);
      }
      Heap_Release(model);
      Heap_Release(verts);
    }
    {
      s32 r = 8;

      if (frame > 130) {
        r = 2;
      } else if (frame > 120) {
        r = 4;
      } else if (frame > 110) {
        r = 6;
      }
      BattleCamera_Shake(r, r);
    }
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }
  count = -1;
  transition[4] = 1;
  BattleBackground_LoadFar(1, (s32)&ResourceId_AerialViewBackdrop, 0);
  Func_08164b2c(31, 31, 31);
  gBgScroll[1].x = 0;
  gCameraSceneParameters[4] = 240;
  wait(1);
  Resource_LoadAndDecompress((s32)&ResourceId_DustBlobSheet, work->sheet, 1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data0C1, work->sheet + 0x400, 0,
                             0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data0CA, work->sheet + 0x3840, 0,
                             0);
  {
    s32 *v = &gData_02014000__f14153c[0].variant;
    s32 c = -1;

    for (i = 0; i != 512; i++) {
      *v = c;
      v += 7;
    }
  }
  work->transferMode = 2;
  work->transferValue = 50;
  gBgScroll[1].y = 32;
  {
    const void *mesh = Data_02011000;
    void *vertices = gMapCellBuffer;
    const void *mesh2 = Data_02012800;

    for (frame = 0; frame != 26; frame++) {
      if ((gInput.repeat & 3) && count == -1) {
        count = 8;
      }
      if (frame <= 6 && count == -1) {
        s32 b = 31 - frame * 8;
        s32 a = b;
        s32 c = b;

        if (c < -10) {
          a = -10;
        }
        if (c < -15) {
          b = -15;
        }
        if (c <= 3) {
          c = 4;
        }
        Func_08164b2c(a, b, c);
      }
      if (frame > 17 || count > 0) {
        Func_08164abc(4, 4, 4);
        if (--count == 0) {
          break;
        }
      }
      for (i = 0; i != 16; i++) {
        // A constant here and in p below, so both share one pool word (as
        // gData_02014000, 70 to 786 differ).
        struct EffectStep *q =
            (struct EffectStep *)(Ram_MapCellBuffer + 0x4000) +
            ((frame * 16 + i) & 511);
        // Default: set for real below.
        s32 a = 0;
        s32 r = 0;

        a = Random16();
        r = (Random16() & 15) + frame * 2;
        q->x = Trig_Sin(a) * r + 0x3c0000;
        q->y = Trig_Cos(a) * r + 0x3c0000;
        q->velocityX = 0;
        q->velocityY = 0;
        q->variant = (Random16() & 15) + 16;
      }
      {
        const u16 *sizeOffsets = gBattleEffect_MoteCells;
        for (i = 0; i != 512; i++) {
          p = (struct EffectStep *)(Ram_MapCellBuffer + 0x4000) + i;
          if (p->variant >= 0) {
            s32 n = (p->variant >> 3) + 1;
            // sizeOffsets[n - 1] spelled as a byte offset: plain, 49 differ.
            s32 off = n * 2 - 2;

            blit[0](canvas,
                    sprites + *(const u16 *)((const u8 *)sizeOffsets + off),
                    (p->x >> 16) - n / 2, (p->y >> 16) - n, n, n * 2);
            BattleEffectKernels_IntegrateVector2(p, 60, -0x4000);
            p->variant--;
          }
        }
      }
      model = Render_AllocateModelList(1);
      shade = 0;
      if (frame <= 31) {
        s32 s = 0x10000;

        if (frame > 15) {
          s += 0x10000 - (frame << 12);
        }
        texture3.logWidth = 5;
        texture3.logHeight = 2;
        texture3.pixels = work->sheet + 0x3840;
        model->mesh = Data_08199220;
        model->shade = shade;
        model->vertices = gMapCellBuffer;
        model->kind = 6;
        model->texture = &texture3;
        Render_ResetTransformState();
        Func_08015160(-0x20000, 0x100000, 0);
        SceneTransform_ApplyScale3(0x8000, 0x10000, 0x8000);
        SceneTransform_ApplyRoll(0x9800);
        SceneTransform_ApplyScale3(s, 0x20000, 0x8000);
        Render_TransformVertices(Data_081991c0, gMapCellBuffer, 4);
        Render_DrawModelList(model);
      }
      if (frame == 0) {
        Sound_PlayFar(208);
        Func_0818de3c(Data_02011000, 16, 8, 8, 8);
        {
          s32 ringAngle = 0;
          for (ring = 0, offset = 0; ring != 8;
               ringAngle += 0x888, offset += 64, ring++) {
            s32 y = -Trig_Cos(ringAngle) * 96 >> 16;
            s32 rr = Trig_Sin(ringAngle) * 48 >> 16;
            struct EclipseVertex *v = (struct EclipseVertex *)Data_02010800;

            k = 0;
            v = (struct EclipseVertex *)((u8 *)v + offset);
            for (; k != 16; k++) {
              v->x = Trig_Sin(k << 12) * rr >> 16;
              v->y = y;
              v->z = Trig_Cos(k << 12) * rr >> 16;
              v++;
            }
          }
        }
        Func_081885f0(Data_02012000, Data_02012800, 32);
      }
      texture3.logWidth = 5;
      texture3.logHeight = 5;
      texture3.pixels = work->sheet;
      {
        model->kind = 6;
        model->mesh = mesh;
        model->vertices = vertices;
        model->texture = &texture3;
        model->shade = shade;
        model->unk04 = 3;
      }
      {
        s32 s = (frame << 9) + 0x8000;

        if (s > 0x10000) {
          s = 0x10000;
        }
        Render_ResetTransformState();
        Func_08015160(-0x20000, 0xc0000, 0);
        SceneTransform_ApplyPitch(0x2000);
        SceneTransform_ApplyYaw(-frame << 10);
        Func_0801521c(s);
        Render_TransformVertices(Data_02010800, vertices, 128);
        Render_DrawModelList(model);
      }
      texture3.logHeight = 5;
      texture3.logWidth = 7;
      texture3.pixels = work->sheet + 0x400;
      model->texture = &texture3;
      model->kind = 7;
      model->mesh = mesh2;
      model->unk04 = shade;
      model->vertices = vertices;
      model->scrollU = shade;
      model->scrollV = shade;
      {
        for (i = 0; i != 4; i++) {
          s32 t = frame + 24 - i * 6;

          if (t >= 0 && t <= 63) {
            s32 s = (Data_081977e4[i] * t << 10) + 0x4000;

            shade = 0;
            if (t <= 15) {
              shade = t * 4 - 64;
            }
            if (t > 40) {
              shade = (40 - t) * 4;
            }
            model->shade = shade;
            Render_ResetTransformState();
            Func_08015160(-0x20000, 0x200000, 0);
            SceneTransform_ApplyScale3(s / 4, s / 4, s / 4);
            SceneTransform_ApplyPitch(10000);
            SceneTransform_ApplyYaw((frame + i * 8) << 10);
            SceneTransform_ApplyScale3(0x10000, 0x60000, 0x10000);
            Func_08015160(0, Data_081977e8[i] << 16, 0);
            Render_TransformVertices(Data_02012000, vertices, 64);
            Render_DrawModelList(model);
          }
        }
      }
      Heap_Release(model);
      work->transferPending = 1;
      wait(1);
    }
  }
  Iwram_FillWords((void *)0x06004000, 0x3c00, 0x3f3f3f3f);
  gBgScroll[1].x = savedScroll;
  transition[4] = 0;
  gCameraSceneParameters[4] = 120;
  BattleBackground_LoadFar(1, *(u16 *)(battle + 0x670), 24);
  wait(1);
  work->transferMode = 3;
  work->transferValue = 0x02020202;
  Graphics_LoadGreyRamp();
  Iwram_FillWords(canvas, 0x3c00, 0x3f3f3f3f);
  BattlePlayback_StartFar(134);
  for (frame = 0; frame != 32; frame++) {
    work->transferPending = 1;
    wait(1);
  }
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(SLOT_BLITTER);
  BattleEffect_EndCanvasLayer();
}

// Unused: two empty functions the ROM keeps after Eclipse; nothing calls them.
void Func_0814293c(void) {}

void Func_08142940(void) {}

struct Model;

// Heap slots, as byte offsets into Ram_HeapSlots.
#define SLOT_BATTLE_EFFECT 92 // battleFxWork[0]
#define SLOT_BATTLE_CANVAS 96 // battleFxWork[1]

// The whole effect work block: struct BattleEffectWork names only its first
// 0x78f4 bytes.
#define BATTLE_EFFECT_WORK_SIZE 0x7b7c

#define CLIP (COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y)

// The pixel half of a 16.16 value.
#define HI(value) (((s16 *)&(value))[1])

// One model for the IWRAM renderer.
/* VIEW(f14153c): tag struct ModelDraw as struct ModelDraw__f14153c;
 * battle_effect.c has another definition; tried with the definition the rest of
 * the object has, it does not compile in tla-ja:
 * games/tla/src/battle_effect/battle_effect.c:5359: structure has no member
 * named `depthBias'. */
struct ModelDraw__f14153c {
  s32 kind;
  s32 next;
  const void *model;
  void *corners;
  struct ModelTexture__f14153c *texture;
  s32 depthBias;
  u8 unk18;
  u8 shade;
  u8 unk1a[2];
};

// Rings and crystals gather on the user, for the scenes.

// A glow on the canvas map widens over the user for 56 frames while 16
// spinning rings rise around it, four abreast. From frame 48 the screen
// shakes, flashes burst upward and four crystals grow out of the ground one
// after another, spinning. The user flashes at frames 32 to 68. Kinds 78
// and 79 stand to either side and hand over to the battle (Func_08118150)
// at frame 62; kind 80 runs 100 frames, fades to white and restores the
// scene's view itself.

void Func_08142944(struct BattleEffectArgument *effect, s32 kind) {
  struct EffectPosition center;
  struct ModelTexture__f14153c texture;
  struct BattleEffectWork *work;
  void *canvas;
  DrawRectangle draw[2];
  s32 frames;
  s32 frame;
  s32 index;
  s32 i;

  work = HeapSlot_Alloc(SLOT_BATTLE_EFFECT, BATTLE_EFFECT_WORK_SIZE);
  canvas = HeapSlot_Alloc(SLOT_BATTLE_CANVAS, 0x4000);
  work->vblanksSinceTransfer = 1;
  BattleEffect_StartCanvasLayer(0);
  REG_BLDALPHA = 0x1010;
  BattleEffect_LoadBlitter(SLOT_BLITTER, BLITTER(BLIT_ADDITIVE, CLIP));
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet, work->sheet, 1,
                             0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data0C9, work->sheet + 0x3000, 0,
                             0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data0DA, Data_02012000, 0, 0);
  work->transferMode = 2;
  work->transferValue = 50;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);
  work->fadeFrames = 24;
  work->fadeStep = 0;

  frames = 100;
  if (kind != KIND_FINALE)
    frames = 76;
  for (frame = 0; frame != frames; frame++) {
    index = (frame - 48) * 2;
    // The effect's centre, beside or above the user.
    EffectPosition_ProjectBattler(effect->actor, &center);
    if (kind == KIND_RIGHT) {
      center.x += 40;
      center.y -= 24;
    } else if (kind == KIND_LEFT) {
      center.x -= 16;
      center.y -= 24;
    } else {
      center.y -= 48;
      /* FAKEMATCH(barrier): an empty asm taking the centre's address gives it
       * two more references, so the register allocator ranks it above the
       * corners; without it, 189 of 734 instructions differ in tla-en. */
      __asm__("" : : "r"(&center));
    }

    if (frame == 0) {
      work->transferMode = 2;
      work->transferValue = 50;
      if (kind == KIND_LEFT)
        Iwram_CopyWordsEntry(
            (void *)BG_PLTT,
            Resource_GetTableEntry((s32)&ResourceId_OrangePaletteB), 128);
      else
        Iwram_CopyWordsEntry(
            (void *)BG_PLTT,
            Resource_GetTableEntry((s32)&ResourceId_EffectPalette03), 128);
      if (frame == 0)
        Sound_PlayFar(140);
    }
    if (frame == 48)
      Sound_PlayFar(144);
    if (frame == 56)
      Sound_PlayFar(148);
    if (frame == 32)
      BattleEffect_TintBattler(effect->actor, 7, -1, -1, -1);
    if (frame == 68)
      BattleEffect_TintBattler(effect->actor, 0, -1, -1, -1);
    if (kind == KIND_FINALE) {
      // The finale fades to white from frame 62.
      if (frame > 61) {
        work->transferMode = 1;
        work->transferValue = 0x10101010;
        Palette_BrightenBgEntries(2, 2, 2);
      }
    } else if (frame == 62) {
      Func_08118150(kind);
    }

    // From frame 48: shake, clear the canvas, and for 8 frames burst two
    // flashes a frame from a point near the centre.
    if (frame >= 48) {
      struct EffectStep *flashes;

      if (frame == 48) {
        {
          s32 *variant = &gData_02014000__f14153c[0].variant;

          for (i = 0; i != 64; i++) {
            *variant = -1;
            variant += 7;
          }
        }
        work->shakeFrames = 8;
        Iwram_FillWordsEntry(canvas, 0x3c00, 0x3f3f3f3f);
      }
      flashes = gData_02014000__f14153c;
      if (frame >= 48 && frame <= 55) {
        s32 x = ((Random16() & 63) + center.x / 2 - 32) << 16;
        s32 y = ((Random16() & 31) + center.y) << 16;
        s32 k = 0;
        s32 spread = 15;
        struct EffectStep *burst = &flashes[index];

        for (; k != 2; k++) {
          struct EffectStep *flash = &burst[k];
          s32 speed = 31; // Set first: the mask is the tied operand.
          s32 angle;

          speed &= Random16();
          angle = Random16();

          flash->x = x + (((Random16() & spread) - 8) << 16);
          flash->y = y + (((Random16() & spread) - 8) << 16);
          flash->velocityX = Trig_Sin(angle) * (speed + 32) >> 3;
          flash->velocityY = Trig_Cos(angle) * (speed + 32) >> 3;
          flash->variant = 0;
        }
      }
      {
        struct EffectStep *flash = flashes;

        for (i = 0; i != 16; i++, flash++) {
          if (flash->variant >= 0 && flash->variant <= 17) {
            draw[0](canvas, work->sheet + (flash->variant / 3 << 11),
                    HI(flash->x) - 16, HI(flash->y) - 48, 32, 64);
            BattleEffectKernels_IntegrateVector2(flash, 62, -0x800);
            flash->variant++;
          }
        }
      }
    }

    if (frame >= 0) {
      s32 cx = center.x / 2 << 16;
      s32 cy = center.y << 16;
      struct ModelDraw__f14153c *model;
      void *corners;
      s32 bias;

      // The glow on the canvas map widens for 56 frames.
      if (frame <= 63) {
        s32 size = 128;
        s32 width = frame + 8;

        if (width <= 64) {
          size = width * 2;
          Func_0815b434(Ram_MapCellBuffer, size, 0x10000);
        }
        Func_0818caa8(Ram_MapCellBuffer, cx >> 16, cy >> 16, size);
      }

      // Each ring has an angle (x), a spin (y) and an age (variant), the
      // later ones 4 frames apart.
      if (frame == 0) {
        for (i = 0; i != 16; i++) {
          struct EffectStep *ring = &work->particles[i];

          ring->x = Random16() & 0xffff;
          ring->y = (Random16() & 255) + 128;
          if (Random16() & 1)
            ring->y = -ring->y;
          ring->variant = -4 * i;
        }
      }

      corners = Heap_AllocEwram(0x200);
      model = Render_AllocateModelList(1);
      texture.logWidth = 5;
      bias = 0;
      model->depthBias = bias;
      texture.logHeight = 2;
      texture.pixels = work->sheet + 0x3000;
      model->kind = 7;
      model->texture = &texture;
      model->model = Data_08199220;
      model->corners = corners;

      // The rings rise for 24 frames, spinning and swelling.
      for (i = 0; i != 16; i++) {
        struct EffectStep *ring = &work->particles[i];

        ring->variant += 2;
        ring->x += ring->y * 2;
        if (ring->variant >= 0 && ring->variant <= 47) {
          Render_ResetTransformState();
          {
            s32 column = i & 3;

            Func_08015160(cx + ((column * 2 + 2) << 16) - 0x400000,
                          cy - ((column + 2) << 16) - 0x400000, 0);
          }
          SceneTransform_ApplyScale3(0x8000, 0x10000, 0x10000);
          SceneTransform_ApplyRoll(ring->x);
          SceneTransform_ApplyScale3(Trig_Sin(ring->variant * 682) + 0x10000,
                                     0x20000, 0x10000);
          Render_TransformVertices(gData_081991b0, corners, 4);
          Render_DrawModelList(model);
        }
      }

      // The four crystals grow out of the ground from frame 48 on.
      model->texture = &texture;
      model->corners = corners;
      texture.logWidth = 6;
      texture.pixels = Data_02012000;
      model->kind = 7;
      texture.logHeight = 6;
      model->model = gData_081990d0;
      for (i = 0; i != 4; i++) {
        s32 start = Data_081977ec[i] + 48;

        model->shade = (frame * 8 + i * 32) & 127;
        if (frame > start) {
          bias = (start - frame) * 8 + 56;
          if (bias > -16)
            bias = -16;
          if (bias > -64) {
            s32 growth = (frame - start) * Data_081977f0[i] * 2000 + 0x4000;

            model->depthBias = bias;
            Render_ResetTransformState();
            Func_08015160((center.x / 2 - 60) << 16, 0x200000, 0);
            SceneTransform_ApplyScale3(
                growth * 2, Data_081977f4[i] * growth + 0x40000, growth / 4);
            SceneTransform_ApplyPitch(1000);
            if (i & 1)
              SceneTransform_ApplyYaw((i * 8 + frame) << 10);
            else
              SceneTransform_ApplyYaw((i * 8 - frame) << 10);
            Render_TransformVertices(gData_08199090, corners, 32);
            Render_DrawModelList(model);
          }
        }
      }
      Heap_Release(model);
      Heap_Release(corners);
    }

    BattleCamera_Shake(8, 8);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(SLOT_BLITTER);
  if (kind == KIND_FINALE) {
    // Put the scene's view back by hand.
    Sound_PlayFar(390);
    gBgScroll[1].x = work->savedBg1X;
    gBgScroll[1].y = work->savedBg1Y;
    gCameraSceneParameters[3] = 120;
    gCameraSceneParameters[4] = 120;
    Task_Remove(Palette_StepFadeTransfer);
  } else {
    BattleEffect_EndCanvasLayer();
  }
  HeapSlot_Release(SLOT_BATTLE_CANVAS);
  HeapSlot_Release(SLOT_BATTLE_EFFECT);
}

/// The frame callback that copies the effect canvas to BG character block 1
/// when a transfer is pending, in the way the transfer mode asks for.
void BattleEffect_ProcessPendingGraphicsTransfer(void) {
  struct CanvasWork *work = (struct CanvasWork *)Ram_HeapSlots->battleFxWork[0];
  void *source = Ram_HeapSlots->battleFxWork[1];
  u32 ime;

  IRQ_LOCK(ime);
  if (work->transferPending == 1) {
    work->transferPending = 0;
    IRQ_UNLOCK(ime);
    switch (work->transferMode) {
    case 0:
      /* FAKEMATCH(wrapper): the copy forwarded through CopyWords_, which keeps
       * each call's own constants, as the scene layer does; as direct
       * Iwram_CopyWords calls in this callback and in
       * BattleEffect_ProcessPendingStampTransfer,
       * BattleEffect_FlushPendingGraphicsTransfer and
       * BattleEffect_ProcessPendingBlock2Transfer, 128 of 369 instructions
       * differ in tla-en. */
      Iwram_CopyWords((void *)0x06004000, source, 0x4000);
      break;
    case 1:
      Iwram_CopyWords((void *)0x06004000, source, 0x4000);
      /* FAKEMATCH(wrapper): the fill forwarded through FillWords_, which keeps
       * each call's own constants, as the scene layer does; as direct
       * Iwram_FillWords calls in this callback and in the same three
       * callbacks, 16 of 369 instructions differ in tla-en. */
      Iwram_FillWords(source, 0x4000, work->transferValue);
      break;
    case 2:
      if (work->transferValue == 50)
        ColorBuffer_Halve(source, (void *)0x06004000, 0x4000);
      else
        ColorBuffer_ScaleThreeQuarters(source, (void *)0x06004000, 0x4000);
      break;
    case 3:
      ColorBuffer_Darken(source, work->transferValue, (void *)0x06004000,
                         0x4000);
      break;
    case 4:
      ColorBuffer_Brighten(source, work->transferValue, (void *)0x06004000,
                           0x4000);
      break;
    }
    // DEAD: nothing uses the counter read here; its load is in the ROM.
    (void)*(volatile s32 *)&work->vblanksSinceTransfer;
    work->vblanksSinceTransfer = 1;
  } else {
    IRQ_UNLOCK(ime);
    work->vblanksSinceTransfer++;
  }
}

/// When a transfer is pending, copies the 0x1000-byte canvas at work 0x6780
/// to the stamp's tiles (the pointer at work 0x7b74), then clears it.
void BattleEffect_ProcessPendingStampTransfer(void) {
  u8 *work = Ram_HeapSlots->battleFxWork[0];
  u32 ime;
  s32 *pending;

  IRQ_LOCK(ime);
  pending = (s32 *)(work + 0x78ec);
  if (*pending == 1) {
    *pending = 0;
    IRQ_UNLOCK(ime);
    Iwram_CopyWords(*(void **)(work + 0x7b74), work + 0x6780, 0x1000);
    Iwram_FillWords(work + 0x6780, 0x1000, 0);
  } else {
    IRQ_UNLOCK(ime);
  }
}

/// The same callback for the 0x7800-byte canvas in the map cell buffer, copied
/// to BG character block 2.
void BattleEffect_FlushPendingGraphicsTransfer(void) {
  struct CanvasWork *work = (struct CanvasWork *)Ram_HeapSlots->battleFxWork[0];
  void *source = Ram_MapCellBuffer;
  u32 ime;

  IRQ_LOCK(ime);
  if (work->transferPending == 1) {
    work->transferPending = 0;
    IRQ_UNLOCK(ime);
    switch (work->transferMode) {
    case 0:
      Iwram_CopyWords((void *)0x06008000, source, 0x7800);
      break;
    case 1:
      Iwram_CopyWords((void *)0x06008000, source, 0x7800);
      Iwram_FillWords(source, 0x7800, work->transferValue);
      break;
    case 2:
      if (work->transferValue == 50)
        ColorBuffer_Halve(source, (void *)0x06008000, 0x7800);
      else
        ColorBuffer_ScaleThreeQuarters(source, (void *)0x06008000, 0x7800);
      break;
    case 3:
      ColorBuffer_Darken(source, work->transferValue, (void *)0x06008000,
                         0x7800);
      break;
    }
    // DEAD: nothing uses the counter read here; its load is in the ROM.
    (void)*(volatile s32 *)&work->vblanksSinceTransfer;
    work->vblanksSinceTransfer = 1;
  } else {
    IRQ_UNLOCK(ime);
    work->vblanksSinceTransfer++;
  }
}

/// The same for the canvas in the second effect work block.
void BattleEffect_ProcessPendingBlock2Transfer(void) {
  struct CanvasWork *work = (struct CanvasWork *)Ram_HeapSlots->battleFxWork[0];
  void *source;
  u32 ime;

  IRQ_LOCK(ime);
  if (work->transferPending == 1) {
    source = Ram_HeapSlots->battleFxWork[1];
    work->transferPending = 0;
    IRQ_UNLOCK(ime);
    switch (work->transferMode) {
    case 0:
      Iwram_CopyWords((void *)0x06008000, source, 0x7800);
      break;
    case 1:
      Iwram_CopyWords((void *)0x06008000, source, 0x7800);
      Iwram_FillWords(source, 0x3c00, work->transferValue);
      break;
    case 2:
      if (work->transferValue == 50)
        ColorBuffer_Halve(source, (void *)0x06008000, 0x3c00);
      else
        ColorBuffer_ScaleThreeQuarters(source, (void *)0x06008000, 0x3c00);
      break;
    case 3:
      ColorBuffer_Darken(source, work->transferValue, (void *)0x06008000,
                         0x3c00);
      break;
    }
    // DEAD: nothing uses the counter read here; its load is in the ROM.
    (void)*(volatile s32 *)&work->vblanksSinceTransfer;
    work->vblanksSinceTransfer = 1;
  } else {
    IRQ_UNLOCK(ime);
    work->vblanksSinceTransfer++;
  }
}

void Func_08143354(void) {
  u32 *map = (u32 *)0x0600f800;
  u32 tiles;
  s32 row;
  s32 column;

  Iwram_FillWords((void *)map, 0x100, -1);
  map += 0x40;
  Iwram_FillWords((void *)map, 0x80, 0x03ff03ff);
  map += 0x20;
  tiles = (u32)(Ram_MapCellBuffer + 0x200);
  for (row = 0; row != 16; row++) {
    for (column = 0; column != 8; column++) {
      *map++ = tiles;
      tiles += 0x00020002;
    }
    for (column = 0; column != 8; column++)
      *map++ = 0x03ff03ff;
  }
}

// The scenes' frame callbacks that copy the display registers the effect
// keeps in its work to the hardware.

/// Writes the window, display and blend registers, all at once.
void Graphics_ApplyWindowBlendRegisters(void) {
  /* FAKEMATCH(pin): the work pointer kept in r2, so the registers' address
   * takes r1; as a plain local, 31 of 55 instructions differ in tla-en. */
  register struct BattleFxWork *work asm("r2") =
      (struct BattleFxWork *)Ram_HeapSlots->battleFxWork[0];

  {
    u32 value = work->win0h;

    /* FAKEMATCH(barrier): an empty asm through the first value loads it before
     * the register's address is built; without it, 14 of 55 instructions
     * differ in tla-en. */
    __asm__ volatile("" : "+l"(value));
    REG_WIN0H = value;
  }
  REG_WIN0V = work->win0v;
  REG_WIN1H = work->win1h;
  REG_WIN1V = work->win1v;
  REG_WININ = work->winin;
  REG_WINOUT = work->winout;
  REG_DISPCNT = work->dispcnt;
  REG_BLDCNT = work->bldcnt;
  REG_BLDALPHA = work->bldalpha;
}

/// Writes background 2's scale (BG2PA and BG2PD) and its reference point.
void Display_ApplyBg2Affine(void) {
  /* FAKEMATCH(pin): the work pointer kept in r2, so the registers' address
   * takes r1; as a plain local, 32 of 30 instructions differ in tla-en. */
  register struct BattleFxWork *work asm("r2") =
      (struct BattleFxWork *)Ram_HeapSlots->battleFxWork[0];

  {
    u32 value = work->bg2pa;

    /* FAKEMATCH(barrier): an empty asm through the first value loads it before
     * the register's address is built; without it, 14 of 30 instructions
     * differ in tla-en. */
    __asm__ volatile("" : "+l"(value));
    REG_BG2PA = value;
  }
  REG_BG2PD = work->bg2pd;
  /* FAKEMATCH(barrier): an empty asm keeps the next offset's step after the
   * store before it; without it, 2 of 30 instructions differ in tla-en. */
  __asm__ volatile("");
  REG_BG2X = work->bg2x;
  REG_BG2Y = work->bg2y;
}

/// Writes background 2's reference point.
void Display_ApplyBg2Reference(void) {
  /* FAKEMATCH(pin): the work pointer kept in r2, so the registers' address
   * takes r1; as a plain local, 20 of 20 instructions differ in tla-en. */
  register struct BattleFxWork *work asm("r2") =
      (struct BattleFxWork *)Ram_HeapSlots->battleFxWork[0];
  u32 hold;

  /* FAKEMATCH(barrier): an empty asm sets hold, which keeps r1 until the
   * first value is read, so the offset takes r0; without hold and its asm,
   * 8 of 20 instructions differ in tla-en. */
  __asm__ volatile("" : "=l"(hold));
  {
    u32 value = work->bg2x;

    /* FAKEMATCH(barrier): an empty asm through the first value loads it before
     * the register's address is built, and ends hold; as an asm on hold
     * alone, 14 of 20 instructions differ in tla-en. */
    __asm__ volatile("" : "+l"(value) : "l"(hold));
    REG_BG2X = value;
  }
  REG_BG2Y = work->bg2y;
}

void Palette_StepFadeTransfer(void) {
  u8 *work = Ram_HeapSlots->battleFxWork[0];
  s32 *framesLeft;
  u8 *battle;

  framesLeft = (s32 *)(work + 0x77b4);
  battle = (u8 *)Ram_HeapSlots->battleWork;

  if (*framesLeft > 0) {
    s32 *step = (s32 *)(work + 0x77b8);

    (*step)++;
    ColorBuffer_ScaleFar(battle + 0x56c, (void *)&BG_PLTT_COLOR(6, 0),
                         0x10000 - *step * 1092, 128);
    (*framesLeft)--;
  }
}

/// Clears every target's flash timer.
void BattleEffect_ClearActorTimers(void) {
  Iwram_ClearWords(Ram_HeapSlots->battleFxWork[0] + 0x78dc, 8);
}

/// Counts down each target's flash timer; when one runs out, the target gets
/// its own palette back.
void BattleEffect_CountDownBattlerTints(void) {
  u8 *base;
  s32 i;
  s32 index;

  base = Ram_HeapSlots->battleFxWork[0];
  i = 0;
  do {
    if (base[0x78dc + i] != 0) {
      if ((base[0x78dc + i] = base[0x78dc + i] - 1) == 0) {
        index = i * 2 + 36;
        BattleEffect_TintBattler(*(s16 *)(*(u8 **)(base + 0x78f0) + index), 0,
                                 -1, -1, 0);
      }
    }
    i += 1;
  } while (i != 8);
}

void Func_0814355c(void) {
  REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  REG_BLDALPHA = BLEND_ALPHA(14, 16);
  REG_BG2X = 0;
  REG_BG2Y = 0xfffff000;
  REG_BG2PA = Q_8_8(0.5);
  REG_BG2PB = 0;
  REG_BG2PC = 0;
  REG_BG2PD = Q_8_8(1);
  REG_WIN0H = WIN_RANGE(0, 240);
  REG_WIN0V = WIN_RANGE(16, 136);
  REG_WIN1H = WIN_RANGE(0, 240);
  REG_WIN1V = WIN_RANGE(16, 136);
  REG_WINOUT = WINOUT_WINOBJ_ALL | WINOUT_WIN01_CLR | WINOUT_WIN01_BG0;
}

#undef HI

// Twin: The Broken Seal's BattleEffect_StartCanvasLayer. The Lost Age's
// changes: BattleEffect_ConfigureEffectDisplay for the display set-up, DISPCNT
// 0x1741, Palette_StepFadeTransfer at 0x480, Func_0814355c for the display
// registers and one more work word cleared.
void BattleEffect_StartCanvasLayer(s32 bgControl) {
  struct CanvasWork *work = (struct CanvasWork *)Ram_HeapSlots->battleFxWork[0];
  struct BattleWork *battle = Ram_HeapSlots->battleWork;
  void *canvas = Ram_HeapSlots->battleFxWork[1];
  struct BattleBackgroundView *display =
      (struct BattleBackgroundView *)Ram_HeapSlots->transitionWork;
  s32 row;
  s32 col;
  s32 offset;

  BattleEffect_ConfigureEffectDisplay();
  BattleEffect_ClearActorTimers();
  display->secondMode = 1;
  wait(1);
  REG_BLDCNT = 0;
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT,
                      DISPCNT_OBJ_ON | DISPCNT_BG2_ON | DISPCNT_BG1_ON |
                          DISPCNT_BG0_ON | DISPCNT_OBJ_1D_MAP | DISPCNT_MODE_1,
                      IO_WRITE_16);
  gBgScroll[1].y = 32;
  wait(1);

  BattleStaging_ConfigurePaletteFadeFar(1, *(u16 *)battle->unk670, 0);
  work->fadeFrames = 0;
  work->fadeStep = 0;
  Task_Add(Palette_StepFadeTransfer, TASK_PASS_VBLANK);

  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT,
                      DISPCNT_WIN1_ON | DISPCNT_WIN0_ON | DISPCNT_OBJ_ON |
                          DISPCNT_BG1_ON | DISPCNT_BG0_ON | DISPCNT_OBJ_1D_MAP |
                          DISPCNT_MODE_1,
                      IO_WRITE_16);
  wait(1);
  REG_BG2CNT =
      bgControl | BGCNT_SCREEN_BASE(7) | BGCNT_256_COLORS | BGCNT_CHAR_BASE(1);
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT,
                      DISPCNT_WIN1_ON | DISPCNT_WIN0_ON | DISPCNT_OBJ_ON |
                          DISPCNT_BG1_ON | DISPCNT_BG0_ON | DISPCNT_OBJ_1D_MAP |
                          DISPCNT_MODE_1,
                      IO_WRITE_16);
  BattleCamera_SetViewFar(0, 0, 0, 100);
  display->secondMode = 0;
  wait(1);

  Func_0814355c();
  REG_WININ = WININ_WIN1_CLR | WININ_WIN1_OBJ | WININ_WIN1_BG2 |
              WININ_WIN1_BG0 | WININ_WIN0_CLR | WININ_WIN0_OBJ |
              WININ_WIN0_BG2 | WININ_WIN0_BG1 | WININ_WIN0_BG0;
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT,
                      DISPCNT_WIN1_ON | DISPCNT_WIN0_ON | DISPCNT_OBJ_ON |
                          DISPCNT_BG2_ON | DISPCNT_BG1_ON | DISPCNT_BG0_ON |
                          DISPCNT_OBJ_1D_MAP | DISPCNT_MODE_1,
                      IO_WRITE_16);

  offset = 0;
  for (row = 0; row != 16; row++) {
    for (col = 0; col != 8; col++, offset += 2) {
      s32 chr;
      s16 pair;

      chr = row * 16 + col * 2;
      pair = ((chr + 1) << 8) | chr;
      *(volatile s16 *)((u8 *)BG_CANVAS_MAP + offset) = pair;
    }
  }

  Iwram_ClearWords(canvas, 0x4000);
  Iwram_ClearWords(BG_CHAR_BLOCK(1), 0x4000);
  work->shakeFrames = 0;
  work->transferPending = 0;
  work->savedBg1X = gBgScroll[1].x;
  work->savedBg1Y = gBgScroll[1].y;
  wait(1);
}

/// Opens the effect canvas layer: BG2 becomes a 2x-wide 128x128 canvas of
/// 16x16 characters, blended over the battle background, whose palette
/// starts fading.
void BattleEffect_OpenCanvasLayer(s32 bgControl) {
  struct CanvasWork *work = (struct CanvasWork *)Ram_HeapSlots->battleFxWork[0];
  struct BattleWork *battle = Ram_HeapSlots->battleWork;
  void *canvas = Ram_HeapSlots->battleFxWork[1];
  struct BattleBackgroundView *display =
      (struct BattleBackgroundView *)Ram_HeapSlots->transitionWork;
  s32 row;
  s32 col;
  s32 offset;

  BattleEffect_ClearActorTimers();
  display->secondMode = 1;
  wait(1);
  REG_BLDCNT = 0;
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT,
                      DISPCNT_OBJ_ON | DISPCNT_BG2_ON | DISPCNT_BG1_ON |
                          DISPCNT_BG0_ON | DISPCNT_OBJ_1D_MAP | DISPCNT_MODE_1,
                      IO_WRITE_16);
  gBgScroll[1].y = 32;
  wait(1);
  BattleStaging_ConfigurePaletteFadeFar(1, *(u16 *)battle->unk670, 128);
  work->fadeFrames = 24;
  work->fadeStep = 0;
  Task_Add(Palette_StepFadeTransfer, TASK_PASS_VBLANK);
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT,
                      DISPCNT_OBJ_ON | DISPCNT_BG1_ON | DISPCNT_BG0_ON |
                          DISPCNT_OBJ_1D_MAP | DISPCNT_MODE_1,
                      IO_WRITE_16);
  wait(1);
  REG_BG2CNT =
      bgControl | BGCNT_SCREEN_BASE(7) | BGCNT_256_COLORS | BGCNT_CHAR_BASE(1);
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT,
                      DISPCNT_OBJ_ON | DISPCNT_BG1_ON | DISPCNT_BG0_ON |
                          DISPCNT_OBJ_1D_MAP | DISPCNT_MODE_1,
                      IO_WRITE_16);
  BattleCamera_SetViewFar(0, 0, 0, 100);
  display->secondMode = 0;
  wait(1);

  Func_0814355c();
  REG_WININ = WININ_WIN1_CLR | WININ_WIN1_OBJ | WININ_WIN1_BG2 |
              WININ_WIN1_BG0 | WININ_WIN0_CLR | WININ_WIN0_OBJ |
              WININ_WIN0_BG2 | WININ_WIN0_BG1 | WININ_WIN0_BG0;
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT,
                      DISPCNT_WIN1_ON | DISPCNT_WIN0_ON | DISPCNT_OBJ_ON |
                          DISPCNT_BG2_ON | DISPCNT_BG1_ON | DISPCNT_BG0_ON |
                          DISPCNT_OBJ_1D_MAP | DISPCNT_MODE_1,
                      IO_WRITE_16);
  wait(1);

  // The canvas map: 16 rows of 8 character pairs.
  offset = 0;
  for (row = 0; row != 16; row++) {
    for (col = 0; col != 8; col++, offset += 2) {
      s32 chr;
      s16 pair;

      chr = row * 16 + col * 2;
      pair = ((chr + 1) << 8) | chr;
      *(volatile s16 *)((u8 *)BG_CANVAS_MAP + offset) = pair;
    }
  }

  /* FAKEMATCH(wrapper): the clear forwarded through an inline, which keeps each
   * call's own constants and the routine in a callee-saved register; as direct
   * Iwram_ClearWords calls, 24 of 259 instructions of
   * BattleEffect_OpenCanvasLayer differ in tla-en. */
  Iwram_ClearWords(canvas, 0x4000);
  Iwram_ClearWords(BG_CHAR_BLOCK(1), 0x4000);
  work->shakeFrames = 0;
  work->transferPending = 0;
  wait(1);
}

void BattleEffect_StartTiledCanvas(s32 bg_control) {
  s32 row;
  s32 col;
  s32 offset;

  BattleEffect_StartCanvasLayer(bg_control);
  REG_BG2CNT = bg_control | BGCNT_SIZE(1) | BGCNT_SCREEN_BASE(7) |
               BGCNT_256_COLORS | BGCNT_CHAR_BASE(1);
  offset = 0;
  for (row = 0; row != 16; row++) {
    for (col = 0; col != 8; col++, offset += 2) {
      s32 tile;
      s16 pair;

      tile = row * 16 + col * 2;
      pair = ((tile + 1) << 8) | tile;
      *(volatile s16 *)(0x06003800 + offset) = pair;
    }
    for (col = 0; col != 8; col++, offset += 2) {
      *(volatile u16 *)(0x06003800 + offset) = 0;
    }
  }
}

void BattleEffect_StartTiledCanvasFilled(s32 bg_control) {
  s32 row;
  s32 col;
  s32 offset;

  BattleEffect_StartCanvasLayer(bg_control);
  REG_BG2CNT = bg_control | BGCNT_SIZE(1) | BGCNT_SCREEN_BASE(7) |
               BGCNT_256_COLORS | BGCNT_CHAR_BASE(1);
  offset = 0;
  for (row = 0; row != 16; row++) {
    for (col = 0; col != 8; col++, offset += 2) {
      s32 tile;
      s16 pair;

      tile = row * 16 + col * 2;
      pair = ((tile + 1) << 8) | tile;
      *(volatile s16 *)(0x06003800 + offset) = pair;
    }
    for (col = 0; col != 8; col++, offset += 2) {
      *(volatile u16 *)(0x06003800 + offset) = 0xf0f;
    }
  }
}

/// Closes the effect canvas layer: BG1 back where it was, BG2 and its
/// characters cleared, then the background palette faded back in.
void BattleEffect_EndCanvasLayer(void) {
  struct CanvasWork *work = (struct CanvasWork *)Ram_HeapSlots->battleFxWork[0];
  struct BattleWork *battle = Ram_HeapSlots->battleWork;
  s32 i;

  Sound_PlayFar(390);
  gBgScroll[1].x = work->savedBg1X;
  gBgScroll[1].y = work->savedBg1Y;
  gCameraSceneParameters[3] = 120;
  gCameraSceneParameters[4] = 120;
  REG_BG2CNT = BGCNT_SCREEN_BASE(7) | BGCNT_256_COLORS | BGCNT_CHAR_BASE(1) |
               BGCNT_PRIORITY(3);
  Iwram_ClearWords(BG_CHAR_BLOCK(1), 0x4000);
  Task_Remove(Palette_StepFadeTransfer);
  gBgScroll[1].y = 32;
  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT,
                      DISPCNT_WIN1_ON | DISPCNT_WIN0_ON | DISPCNT_OBJ_ON |
                          DISPCNT_BG1_ON | DISPCNT_BG0_ON | DISPCNT_OBJ_1D_MAP |
                          DISPCNT_MODE_1,
                      IO_WRITE_16);
  REG_BLDCNT = 0;
  wait(1);

  BattleStaging_ConfigurePaletteFadeFar(2, *(u16 *)battle->unk670, 7);
  wait(1);
  for (i = 0; i != 8; i++) {
    BattleBackground_SetPaletteLevelFar(*(u16 *)battle->unk670, 21 - i * 3);
    wait(1);
  }

  IO_WRITE_QUEUE_PUSH((u32)&REG_DISPCNT,
                      DISPCNT_WIN1_ON | DISPCNT_WIN0_ON | DISPCNT_OBJ_ON |
                          DISPCNT_BG2_ON | DISPCNT_BG0_ON | DISPCNT_OBJ_1D_MAP |
                          DISPCNT_MODE_1,
                      IO_WRITE_16);
  wait(1);
}

/// Hides the canvas layer: marks the background view, queues DISPCNT 0x1541,
/// fades the background palette back and queues BG1's control word.
void BattleEffect_HideBg1AndRestoreBackdrop(void) {
  struct BattleWork *battle;
  s32 *flag;

  flag = &((struct BattleBackgroundView *)Ram_HeapSlots->transitionWork)
              ->secondMode;
  battle = Ram_HeapSlots->battleWork;
  *flag = 1;
  QueueIoWriteDelay2((u32)&REG_DISPCNT,
                     DISPCNT_OBJ_ON | DISPCNT_BG2_ON | DISPCNT_BG0_ON |
                         DISPCNT_OBJ_1D_MAP | DISPCNT_MODE_1);
  wait(1);
  BattleStaging_ConfigurePaletteFadeFar(2, *(u16 *)battle->unk670, 0);
  IO_WRITE_QUEUE_PUSH((u32)&REG_BG1CNT,
                      BGCNT_SCREEN_BASE(31) | BGCNT_256_COLORS |
                          BGCNT_PRIORITY(3),
                      IO_WRITE_16);
  wait(1);
}

void BattleEffect_SetupCanvasTileMap(void) {
  void *canvas = Ram_HeapSlots->battleFxWork[1];
  struct BattleWork *screen = Ram_HeapSlots->battleWork;
  struct BattleBackgroundView *layer =
      (struct BattleBackgroundView *)Ram_HeapSlots->transitionWork;
  s32 row;
  s32 col;
  s32 off;

  BattleEffect_ClearActorTimers();
  BattleStaging_ConfigurePaletteFadeFar(2, *(u16 *)screen->unk670, 0);
  gBgScroll[1].y = 32;
  layer->secondMode = 0;
  Iwram_ClearWords((void *)0x06003fc0, 64);
  Iwram_FillWords((void *)0x0600f900, 0x200, -1);
  off = 0;
  for (row = 0; row != 16; row++) {
    for (col = 0; col != 32; col++, off += 2) {
      if (col > 15)
        *(volatile u16 *)(0x0600fb00 + off) = 0xff;
      else
        *(volatile u16 *)(0x0600fb00 + off) = row * 16 + 0x100 + col;
    }
  }
  REG_DISPCNT = DISPCNT_WIN1_ON | DISPCNT_WIN0_ON | DISPCNT_OBJ_ON |
                DISPCNT_BG2_ON | DISPCNT_BG1_ON | DISPCNT_BG0_ON |
                DISPCNT_OBJ_1D_MAP | DISPCNT_MODE_1;
  REG_BG1CNT = BGCNT_SCREEN_BASE(31) | BGCNT_256_COLORS | BGCNT_PRIORITY(1);
  REG_BLDCNT = BLDCNT_TGT1_BG1 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  REG_WIN0H = WIN_RANGE(0, 240);
  REG_WIN0V = WIN_RANGE(16, 136);
  REG_WIN1H = WIN_RANGE(0, 240);
  REG_WIN1V = WIN_RANGE(16, 136);
  REG_WININ = WININ_WIN1_CLR | WININ_WIN1_OBJ | WININ_WIN1_BG2 |
              WININ_WIN1_BG0 | WININ_WIN0_CLR | WININ_WIN0_OBJ |
              WININ_WIN0_BG2 | WININ_WIN0_BG1 | WININ_WIN0_BG0;
  REG_WINOUT = WINOUT_WINOBJ_ALL | WINOUT_WIN01_CLR | WINOUT_WIN01_BG0;
  REG_BLDALPHA = BLEND_ALPHA(14, 16);
  Iwram_ClearWords(canvas, 0x4000);
  Iwram_ClearWords(BG_CHAR_BLOCK(1), 0x4000);
  wait(1);
}

#define ABS(v) ((v) < 0 ? -(v) : (v))

/// The Broken Seal's routine, with the fraction in
/// bit 31 (started at 0x80 << 23, carried when negative and cleared with a
/// shift pair) and dx and dy reused as the step direction.
void BattleEffect_DrawClippedCanvasLine(s32 x0, s32 y0, s32 x1, s32 y1,
                                        s32 color) {
  s32 dx = x1 - x0;
  s32 dy = y1 - y0;
  s32 frac = 0x80 << 23;
  u8 *canvas = Ram_HeapSlots->battleFxWork[1];
  s32 step;
  s32 i;
  s32 x;
  s32 y;
  s32 tmp;
  u32 offset;

  if (y0 < 0)
    y0 = 0;
  if (y0 > 127)
    y0 = 127;
  if (y1 < 0)
    y1 = 0;
  if (y1 > 127)
    y1 = 127;

  if (ABS(dx) < ABS(dy)) {
    if (dy < 0) {
      tmp = x0;
      x0 = x1;
      x1 = tmp;
      tmp = y0;
      y0 = y1;
      y1 = tmp;
      dx = x1 - x0;
      dy = y1 - y0;
    }
    step = (ABS(x1 - x0) << 8) / ABS(y1 - y0);
    step <<= 23;
    x = x0;
    if (dx < 0)
      dx = -1;
    else
      dx = 1;
    for (i = y0; i != y1; i++) {
      offset = (((((u32)i >> 3) << 4) + ((u32)x >> 3)) << 3) + (i & 7);
      offset = (offset << 3) + (x & 7);
      if (canvas[offset] < color)
        canvas[offset] = color;
      frac += step;
      if (frac < 0) {
        x += dx;
        frac = (u32)(frac << 1) >> 1;
      }
    }
  } else {
    if (dx < 0) {
      tmp = x0;
      x0 = x1;
      x1 = tmp;
      tmp = y0;
      y0 = y1;
      y1 = tmp;
      dx = x1 - x0;
      dy = y1 - y0;
    }
    step = (ABS(y1 - y0) << 8) / ABS(x1 - x0);
    step <<= 23;
    y = y0;
    if (dy < 0)
      dy = -1;
    else
      dy = 1;
    for (i = x0; i != x1; i++) {
      offset = (((((u32)y >> 3) << 4) + ((u32)i >> 3)) << 3) + (y & 7);
      offset = (offset << 3) + (i & 7);
      if (canvas[offset] < color)
        canvas[offset] = color;
      frac += step;
      if (frac < 0) {
        y += dy;
        frac = (u32)(frac << 1) >> 1;
      }
    }
  }
}

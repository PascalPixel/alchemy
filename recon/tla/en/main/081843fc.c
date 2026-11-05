// Measured 2026-11-05 (tla-en): differ 1999 of 4580, 1561 with slots and pool
// offsets normalized, 393 with registers normalized as well. Every stack slot
// has the listing's reference count at the listing's offset. Measure
// 081843fc,081848ec together.
// What round 3 settled by reading the listing:
// - e is one function-level variable; each section's cells its own. The first
//   cells is the fixed address, the decode's argument the symbol, so CSE keeps
//   them apart. The step comes before the counter (cells += 1024, i++).
// - No loop has a counter of its own beyond i and j: the burst, the wave of
//   particles and the debris all count on i (r9 in the listing). The tornado's
//   inner counter is a block's own j, with a row local holding i << 12.
// - OBJ_TILES reads the slot through its field: the char read made a dependency
//   that moved every object store. The first fade writes the object palette
//   (literal 0x05000202); the instruction diff cannot show a wrong literal, so
//   every pool still needs checking at adoption.
// - The meteor has one b = 0x80000 - drift across its two sites (slot 88).
// What is left:
// - PRE (gcse) shares (b >> 31) and off + idx across a join here, where the
//   listing rebuilds them after it (the second off + idx is lexically idx +
//   off, with its own n * 2). The sign shift is avoided with a plain-C spelling
//   of the halves; n is not, and the burst rotates from there. No limit in
//   gcse.c explains it (469 blocks, 28 PRE substitutions).
// - e takes r6 where the listing has r7, so each resource section rotates among
//   r5, r6, r7: e's priority (20 refs over 40 insns) beats cells and the copy's
//   address here, and the listing has them first.
// - The tone is s16 here, a sign extension the listing lacks; as s32 the last
//   subtraction sinks past two calls and the figure rises by 34.
// BattleEffect_RunCatastrophe; the same calls and control flow as the listing.
// The function runs on through listing 081848ec, whose Func_081848ec and
// Func_08185b9c are labels inside it (the second the first loop's test).
// What the listing's stack and registers proved:
// - The frame is 272, as in the listing, and the opening 137 instructions
// match.
// - blit is DrawRectangle blit[2]: a DImode pseudo, so the first load's store
//   stays (sp+156, with sp+160 unused) and Ram_HeapSlots stays in r5 across the
//   opening calls. list is s16 list[14], as in the family.
// - apply is s32 apply[2] = {0, 0} in a block of its own: the listing stores
// the
//   pair as one DImode zero.
// - i (r9) counts almost every loop; j (r10) counts the fade's outer loop, the
//   particle and burst loops, the wave rows and the bolt's debris. The p loops
//   set p = &work->particles[i] inside the loop, so p is a reduced giv.
// - Stack slots below the declared locals are loop.c's givs: the zoom and tilt
//   (frame multiples), frame * 3 (t), and i * 12 (off), i * 8 + 80 (start) and
//   i * 8 + 16 (lim), each set at its loop's top. Slot order is first pass,
//   inner loops first, then second pass.
// - (frame - 370) * k folds apart, so d holds it; (frame - 128) << 1 does not.
// - The colour channels read through *src three times (HImode masks, the pooled
//   31); both palette loops match that way.
// - The burst sets its sprite, 10 and 20 first and computes bx after the p->x
//   test, which is also what stops the 0x80000 - drift hoist out of its loop.
// - Scratch registers rotate from the first difference; most of what is left
//   from line 170 on has the listing's shape with other registers.
// What is left:
// - First difference: the listing rebuilds 384 for e += 384 after the first
//   copy, where cse2 here reuses the size's pseudo, so e, 384 and the copy's
//   address take other registers and reload's rotation starts there. cse2
//   ignores the loop-end note, so the listing has a label there or a different
//   expression; e + 384, u16 *e, a block and a local n did not separate them.
// - frame * 3's giv is second pass in the listing (slot 20 below the spins), as
//   is the burst base (slot 24); here the base is first pass, and spins and off
//   sit 4 lower than the listing's, so those slots differ.
// - The flash table loop reads [r4, ip] with i * 2 reduced, here a stepped
//   address.
// - Searches over the wave mesh's declaration order and over j's scope (each
//   loop its own j moves frame and registers away) have converged.
// Measured here: a resource id passed as (s32)&ResourceId_X loads from the pool
// as in the listing; ids without a name in resource_directory.s are declared
// below and need one there before adoption. COPY_WORDS's one-pass block keeps
// its form: without it differ rises by about 300. A u16 local is sign-extended
// (PROMOTE_MODE), so savedScroll is s32. z, w, h, cells and the sprite's src
// are one-constant locals; no device was added.

#include "battle_effect.h"
#include "battle_effect_work.h"
#include "effect_step.h"
#include "heap_release.h"
#include "io_reg.h"
#include "iwram_call.h"
#include "ram_buffer.h"
#include "types.h"

struct ModelTexture {
  u32 logWidth : 8;
  u32 logHeight : 8;
  u32 unk02 : 16;
  const void *pixels;
};

struct RenderModel {
  s32 kind;
  s32 unk04;
  const void *mesh;
  void *vertices;
  struct ModelTexture *texture;
  s32 shade;
  u8 scrollU;
  u8 scrollV;
};

void BattleEffect_StartCanvasLayer(s32 bgControl);
void BattleEffect_EndCanvasLayer(void);
void BattleEffect_ProcessPendingGraphicsTransfer(void);
void BattleEffect_CountDownBattlerTints(void);
s32 BattleEffect_LoadBlitter(s32 slot,
                             s32 modeAndFlags); // BattleEffect_BuildBlitter
void Func_0814cd48(s32 actor, s32 palette, s32 pose, s32 timerSlot,
                   s32 timerFrames);              // BattleEffect_TintBattler
void Func_0815b434(u8 *map, s32 size, s32 scale); // shaded circles
void Func_0816ae40(u8 *src, u8 *dst, s32 width, s32 height);
void Func_0815585c(struct BattleEffectArgument *effect, s32 packed, s32 *screen,
                   s32 *otherPoint);
void Func_0818caa8(void *cells, s32 x, s32 y, s32 size);
struct RenderModel *Render_AllocateModelList(s32 count);
void Render_TransformVertices(const void *corners, void *vertices, s32 count);
void Render_DrawModelList(struct RenderModel *model);
void Func_08015160(s32 x, s32 y, s32 z);
void Func_0801521c(s32 scale);
void SceneTransform_ApplyPitch(s32 angle);
void SceneTransform_ApplyYaw(s32 angle);
void Render_ResetTransformState(void);
void Func_080151e4(s32 x, s32 y, s32 z); // SceneTransform_ApplyScale3
void EffectPosition_ProjectBattler(s32 id, struct EffectPosition *out);
void BattleEffect_SpawnObjects(s32 count, s32 resource,
                               s32 mode); // SpawnObjects
void BattleEffect_SelectLivingTargets(
    struct BattleEffectArgument *effect); // SelectLivingTargets
void Func_080150e4(s32 angle);            // SceneTransform_ApplyRoll
void BattleEffect_StartActorMotion(s32 id, s32 force, s32 acceleration,
                                   s32 speedLimit, s32 velocityY, s32 percent);
void Render_ApplyProjectedPlacementFar(void *object, s32 *request, s32 *source,
                                       s32 flags);
void BattleEffect_FetchRectangleBlitters(s32 flip, void **out);
void Func_08118088(s32 actor, s32 motion); // BattleMotion_ApplyVariantMotionFar
void Func_081180e8(s32 phase);             // BattlePlayback_StartFar
void Func_08158ce0(s32 randomMask, u32 shakeRange); // Camera_ApplyShake
void BattleEffectKernels_IntegrateVector2(struct EffectStep *step, s32 gravity,
                                          s32 drag);
void **GetBattleObjectSlotFar(s32 objectId);
void *Heap_AllocEwram(s32 size);
void Resource_LoadAndDecompress(s32 resource, void *destination, s32 a, s32 b);
void *Resource_GetTableEntry(s32 resource);
s32 Task_Add(void (*func)(void), s32 order);
s32 Task_Remove(void (*func)(void));
void HeapSlot_Release(s32 slot);
void Sound_PlayFar(s32 cue);
void wait(s32 frames);
u32 Random16(void);
s32 Trig_Sin(s32 angle);
s32 Trig_Cos(s32 angle);

void Render_BuildQuadStrip(void *mesh, s32 columns, s32 width, s32 height);
void BattleEffectKernels_IntegrateVector3(struct EffectStep *step, s32 damping,
                                          s32 gravity);
s32 BattleUnit_GetEffectHeightFar(s32 unit);
void Transform_SetLookAt(s32 *eye, s32 *target);
void BattleEffect_StartTiledCanvas(s32 bgControl);
void BattleEffect_StartTiledCanvasFilled(s32 bgControl);
void Transform_Push(void);
void Transform_Pop(void);

// The blitter's heap slot, as a byte offset into Ram_HeapSlots.
#define SLOT_BLITTER 104 // unk64[1]

// ⚓️'s blitter modes run the other way from The Broken Seal's.
enum { BLIT_ADDITIVE, BLIT_MAX, BLIT_TRANSPARENT, BLIT_COPY };
#define BLITTER(mode, flags) ((mode) << 4 | (flags))
#define HI(v) (((s16 *)&(v))[1])

#include "bg_scroll.h"
#include "input.h"

// An effect sprite as this effect reaches it: its OAM priority and its VRAM
// cache entry.
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

struct ResourceTableEntry {
  u16 unk0;
  u16 tile;
};

struct WordPair {
  s32 a;
  s32 b;
};

extern char ResourceId_AerialViewBackdrop;
extern char ResourceId_ChainedDemonTiles;
extern char ResourceId_ClawSlashSheet;
extern char ResourceId_Data0B4;
extern char ResourceId_Data0C2;
extern char ResourceId_Data0EE;
extern char ResourceId_Data0EF;
extern char ResourceId_Data0F8;
extern char ResourceId_Data0F9;
extern char ResourceId_Data134;
extern char ResourceId_EffectPalette02;
extern char ResourceId_FlashBurstSheet;
extern char ResourceId_GoldenScalesTiles;
extern char ResourceId_LightningBoltSheet;
extern char ResourceId_PurpleArmourTiles;
extern char ResourceId_RedHazeBackdrop;
extern char ResourceId_SmokeBandSheet;
extern char ResourceId_TornadoSheet;
extern char ResourceId_WhiteSpikesTiles;
extern struct ResourceTableEntry gResourceTableEntries[];
extern u8 gMapCellBuffer[];
extern u8 gData_02014000[];
extern u8 Data_02011000[];
extern u8 Data_02012000[];
extern u8 Data_02016000[];
extern struct EffectStep Data_02016d00[];
extern u32 gCameraSceneParameters[];
extern const struct WordPair Data_08196ef8;
extern const u16 gData_08197410[];
extern const u16 gData_08197486[];
extern const u8 gData_08197492[];
extern const u8 gData_08197498[];
extern const u8 gData_081991b0[];
extern const u8 Data_081991d0[];
extern const u8 Data_081991e0[];
extern const u8 gData_081991f0[];
extern const u8 gData_08199268[];
extern const u8 Data_0819931c[];
extern const u8 gData_08199340[];
extern const u8 gData_08199364[];
extern const u16 Data_08199768[];
extern const u16 Data_0819978c[];
extern const u8 Data_081997b0[];
extern const u8 Data_081997cc[];
extern const u8 Data_081997e8[];
extern const u16 Data_081997f4[];
extern const u8 Data_0819980a[];
extern const u8 Data_08199810[];
extern const u16 Data_08199816[];
extern const u8 Data_08199824[];
extern const u8 Data_0819982b[];
extern const u8 Data_08199832[];
extern const u16 Data_0819983a[];
extern const u8 Data_0819985c[];
extern const u8 Data_08199880[];
extern const u16 Data_081998a4[];
extern const u8 Data_081998f2[];
extern const s16 Data_08199916[];
extern const s16 Data_0819995e[];
extern const s16 Data_081999a6[];
extern const u8 Data_081999ee[];
extern const s16 Data_081999f2[];

s32 Resource_ResetEntry(u32 entry);
void VramBlock_Reset93Far(void);
void Func_08020388(void);
s32 VramCache_AllocEntry(s32 size);
void Transform_Push(void);
void Transform_Pop(void);
struct SpriteObject *Func_0815b290(s32 width, s32 height, u32 shape, s32 mode);
struct SpriteObject *Func_0815b3b0(s32 width, s32 height, u32 shape, s32 mode);
void ResourceObject_ReleaseFar(void *object);
void Func_0813ba50(void);
void Func_08163c2c(s32 a, s32 b);
void Func_0814cca8(void); // BattleEffect_SetupBlendedDisplay
void BattleActor_SpawnObjectsForListFar(s16 *list, s32 mode);
void BattleBackground_LoadFar(s32 layer, s32 background, s32 palette);
s32 Resource_DecodeType01(const void *source, void *destination);
void Palette_LoadBasicObjColors(void);
void Graphics_LoadGreyRamp(void);
void Func_0815f0a0(s32 resource); // BattleEffect_StepPaletteToResource
void Camera_ApplyVerticalShake(s32 range);
void Func_08164a4c(s32 red, s32 green, s32 blue); // Palette_BrightenBgEntries
void Func_08164b2c(s32 r, s32 g, s32 b);
void Func_08196404(s32 slot, s32 width, s32 height, s32 mode, s32 flags);
void ColorBuffer_ScaleFar(void *source, void *destination, s32 scale,
                          s32 count);
void Heap_Release(void *buffer);

// Copies words through the IWRAM routine as one statement.
static inline s32 COPY_WORDS(void *dst, const void *src, s32 size) {
  return Iwram_CopyWords(dst, src, size);
}

// Sets up the alternate blitter as one statement.
#define LOAD_ALT_BLITTER(slot, colour)                                         \
  do {                                                                         \
    Func_08196404(slot, 7, 7, colour, 0);                                      \
  } while (0)

#define SLOT_ALT 188 // unkb8[1]
#define OBJ_TILES(obj)                                                         \
  (0x06010000 + gResourceTableEntries[((struct SpriteObject *)(obj))->slot].tile)
#define HALF_HI(v) (((s16 *)&(v))[1])

// Catastrophe: a black moon rises over a burning city, its towers crack and
// fall in fire, the sky tears open, and a meteor of shadow crushes the foes.
void BattleEffect_RunCatastrophe(struct BattleEffectArgument *effect) {
  u8 *canvas;
  struct BattleEffectWork *work;
  DrawRectangle blit[2];
  s32 savedScroll;
  s32 *transition;
  u8 *sprites;
  u8 *battle;
  s32 camZ;
  s32 swapped;
  s32 drift;
  s32 sway;
  s32 cueTimer;
  s32 cueIndex;
  void *verts;
  s32 shade;
  struct RenderModel *model;
  struct RenderModel *model2;
  struct RenderModel *model3;
  s32 frame;
  s32 i;
  s32 j;
  struct EffectStep *p;
  struct EffectStep *q;
  struct SpriteObject *obj;
  struct ModelTexture texture2;
  s16 list[14];
  s32 place[4];
  struct EffectPosition pos;
  struct WordPair req;
  struct ModelTexture texture;
  struct ModelTexture texture3;
  struct EffectStep *p2;
  struct EffectStep *p3;
  struct EffectStep *p4;
  struct EffectStep *q2;
  u16 *src2;
  u8 *e;

  canvas = Ram_HeapSlots->battleFxWork[1];
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  savedScroll = gBgScroll[1].x;
  transition = (s32 *)Ram_HeapSlots->transitionWork;
  sprites = Ram_HeapSlots->unk64[0];
  battle = Ram_HeapSlots->battleWork;
  Resource_ResetEntry(*(u32 *)(battle + 84));
  VramBlock_Reset93Far();
  BattleEffect_StartCanvasLayer(0x2000);
  REG_BG2PA = 0x100;
  Func_0813ba50();
  *(volatile u16 *)BG_PLTT = 0;
  *(volatile u16 *)(BG_PLTT + 1) = 0;
  BattleEffect_LoadBlitter(SLOT_BLITTER, 3);
  blit[0] = Ram_HeapSlots->unk64[1];
  work->transferMode = 0;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 3200);
  Func_08163c2c(0, 0);
  list[0] = 255;
  BattleActor_SpawnObjectsForListFar(list, 0);
  gCameraSceneParameters[4] = 240;
  HeapSlot_Release(SLOT_BLITTER);
  BattleBackground_LoadFar(1, (s32)&ResourceId_RedHazeBackdrop, 0);
  Func_08164b2c(-31, -31, -31);
  transition[4] = 1;
  gBgScroll[1].x = 0;
  REG_BG2PA = 0x80;
  REG_BLDALPHA = 0x1010;
  REG_BLDCNT = 0x3f44;
  {
    u16 *map = (u16 *)0x0600f800;

    {
      s32 i;

      for (i = 0; i != 30; i++) {
        if (i > 14) {
          {
            s32 j;

            for (j = 0; j != 32; j++) {
              *map++ = i * 32 + (j & 15) + 48;
            }
          }
        } else {
          {
            s32 j;

            for (j = 0; j != 32; j++) {
              *map++ = i * 32 + (j & 15) + 0x200;
            }
          }
        }
      }
    }
  }
  {
    u8 *cells;

    e = Resource_GetTableEntry((s32)&ResourceId_ChainedDemonTiles);
    COPY_WORDS((void *)OBJ_PLTT, e, 384);
    e += 384;
    Resource_DecodeType01(e, gMapCellBuffer);
    cells = Ram_MapCellBuffer;
    for (i = 0; i != 24; cells += 1024, i++) {
      obj = Func_0815b290(32, 32, 0x80002000, 0);
      obj->priority = 2;
      work->objects[i] = obj;
      COPY_WORDS((void *)OBJ_TILES(obj), cells, 1024);
    }
    cells = Data_02016000;
    for (i = 0; i != 4; cells += 1024, i++) {
      obj = Func_0815b3b0(32, 32, 0x80002000, 0);
      work->objects[24 + i] = obj;
      COPY_WORDS(obj, work->objects[i], 24);
      COPY_WORDS((void *)OBJ_TILES(work->objects[24 + i]), cells, 1024);
    }
  }
  {
    u8 *cells = gMapCellBuffer;

    COPY_WORDS(cells + 0x1000, Data_02016000, 0x1000);
  }
  for (i = 0; i != 6; i++) {
    obj = Func_0815b3b0(32, 32, 0x80002000, 0);
    work->objects[28 + i] = obj;
    COPY_WORDS(obj, work->objects[i], 24);
  }
  {
    u8 *cells;

    e = Resource_GetTableEntry((s32)&ResourceId_GoldenScalesTiles);
    COPY_WORDS((void *)(OBJ_PLTT + 0xc0), e, 32);
    cells = work->sheet;
    e += 32;
    Resource_DecodeType01(e, cells);
    for (i = 0; i != 5; cells += 512, i++) {
      obj = Func_0815b290(32, 32, 0x80000000, 0xc000);
      obj->priority = 1;
      work->objects[34 + i] = obj;
      COPY_WORDS((void *)OBJ_TILES(obj), cells, 512);
    }
  }
  {
    u8 *cells;

    e = Resource_GetTableEntry((s32)&ResourceId_PurpleArmourTiles);
    COPY_WORDS((void *)(OBJ_PLTT + 0xd0), e, 32);
    cells = work->sheet;
    e += 32;
    Resource_DecodeType01(e, cells);
    for (i = 0; i != 5; cells += 512, i++) {
      obj = Func_0815b290(32, 32, 0x80000000, 0xd000);
      obj->priority = 1;
      work->objects[39 + i] = obj;
      COPY_WORDS((void *)OBJ_TILES(obj), cells, 512);
    }
  }
  {
    u8 *cells;

    e = Resource_GetTableEntry((s32)&ResourceId_WhiteSpikesTiles);
    COPY_WORDS((void *)(OBJ_PLTT + 0xe0), e, 32);
    cells = work->sheet;
    e += 32;
    Resource_DecodeType01(e, cells);
    for (i = 0; i != 6; cells += 512, i++) {
      obj = Func_0815b290(32, 32, 0x80000000, 0xe000);
      obj->priority = 1;
      work->objects[44 + i] = obj;
      COPY_WORDS((void *)OBJ_TILES(obj), cells, 512);
    }
  }
  COPY_WORDS(work->sheet + 0x5000, (void *)OBJ_PLTT, 480);
  Resource_LoadAndDecompress((s32)&ResourceId_Data0F9, work->sheet, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data0F8, work->sheet + 0x2000, 0,
                             0);
  COPY_WORDS((void *)BG_PLTT,
             Resource_GetTableEntry((s32)&ResourceId_LightningBoltSheet), 512);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, sprites, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data0B4, Data_02012000, 0, 0);
  camZ = 0;
  swapped = 0;
  drift = 0;
  sway = 0;
  cueTimer = 0;
  cueIndex = 0;
  work->transferMode = 2;
  work->transferValue = 50;
  BattleEffect_LoadBlitter(SLOT_BLITTER, 19);
  blit[0] = Ram_HeapSlots->unk64[1];
  frame = 0;
  while (frame != 637) {
    s32 level;
    s32 d;

    // Fades the screen in over the first frames; a button skips the scene.
    if (frame <= 15) {
      s32 v = frame * 2 - 30;

      Func_08164b2c(v, v, v);
    }
    if (gInput.repeat & 3) {
      break;
    }
    if (frame == 0) {
      REG_BG2CNT = 0x786;
      for (i = 3; i != 64; i++) {
        work->particles[i].variant = -1;
      }
    }
    if (frame == 440) {
      REG_BG2CNT = 0x784;
    }
    if (frame <= 369) {
      camZ = (frame * 334 / 379 << 16) + -0x11a0000;
    } else if ((u32)(d = frame - 370) <= 19) {
      camZ = Trig_Sin(d * 1638) * 9 + 0x2b0000;
    } else if ((u32)(frame - 390) <= 95) {
      camZ += -0x8000;
    } else if ((u32)(frame - 486) <= 19) {
      camZ += -0x10000;
    } else if ((u32)(frame - 506) <= 47) {
      s32 v = (frame * 57 << 7) + -0x378eb0;

      if (v > 120000) {
        v = 120000;
      }
      camZ -= v << 1;
    }
    if (frame <= 379) {
      gBgScroll[1].y = 104 - frame * 104 / 379;
    } else {
      s32 v = (frame - 380) / 4;

      gBgScroll[1].y = v;
      if ((u32)(v << 16) > 0x680000) {
        gBgScroll[1].y = 104;
      }
    }
    if ((u32)(d = frame - 370) <= 59) {
      sway = Trig_Sin(d * 273 / 2) << 4;
    }
    if ((u32)(frame - 420) <= 51) {
      drift = (420 - frame) << 12;
      sway += -0x4000;
    } else if ((u32)(frame - 472) <= 13) {
      drift = drift - ((frame - 472) << 12) + 120000;
      sway += 75536;
    } else if ((u32)(frame - 486) <= 8) {
      drift += 0x2000;
      sway -= Trig_Sin((frame << 7) + -0xf300);
    } else if ((u32)(frame - 554) <= 29) {
      sway += -0x10000;
    } else if ((u32)(frame - 584) <= 9) {
      sway += -0x80000;
    }
    if (frame == 0) {
      cueTimer = -1;
      cueIndex = 0;
    }
    if (cueIndex <= 17) {
      level = -1;
      if (frame == Data_08199768[cueIndex]) {
        cueTimer = Data_0819978c[cueIndex];
        switch (cueIndex & 3) {
        case 0:
          Sound_PlayFar(227);
          break;
        case 1:
          Sound_PlayFar(171);
          break;
        case 2:
          Sound_PlayFar(208);
          break;
        case 3:
          Sound_PlayFar(172);
          break;
        }
      }
      if (cueTimer >= 0) {
        level = cueTimer * 31 / Data_0819978c[cueIndex];
        if (--cueTimer == -1) {
          cueIndex++;
        }
      }
      if (level >= 0) {
        u16 *dst = (u16 *)(BG_PLTT + 0x60);
        src2 = (u16 *)(battle + 0x56c);

        for (i = 0; i != 128; i++) {
          s32 r = *src2 & 31;
          s32 g = (*src2 >> 5) & 31;
          s32 b = ((*src2 >> 10) & 31) + level;

          if (r > 31) {
            r = 31;
          }
          if (g > 31) {
            g = 31;
          }
          if (b > 31) {
            b = 31;
          }
          *dst++ = b << 10 | g << 5 | r;
          src2++;
        }
      }
    }
    {
      s32 fr;
      s32 fg;
      s32 fb;
      s32 apply[2] = {0, 0};

      fr = 0;
      fg = 0;
      fb = 0;
      if (frame == 0) {
        fg = -31;
        fr = fg;
        fb = fg;
        apply[0] = 1;
      } else if (frame == 106) {
        fg = -31;
        fr = fg;
        fb = -15;
        apply[0] = 1;
      } else if ((u32)(frame - 154) <= 59) {
        s32 h = (frame - 154) / 2;

        fg = 5 - h * 2;
        fr = fg;
        fb = 5 - h;
        apply[0] = 1;
      } else if ((u32)(frame - 226) <= 15) {
        fg = (frame - 226) / 2 - 31;
        fr = fg;
        fb = frame - 257;
        apply[0] = 1;
      } else if (frame == 242) {
        fr = -24;
        fg = -31;
        apply[0] = 1;
      } else if ((u32)(frame - 256) <= 21) {
        s32 d = frame - 256;

        fr = -24 - d * 2;
        fg = -31 - d * 2;
        fb = -d;
        apply[0] = 1;
      } else if ((u32)(frame - 282) <= 40) {
        s32 d = frame - 282;

        fg = -d;
        fr = fg;
        fb = 16 - d;
        apply[0] = 1;
      } else if (frame == 324) {
        fr = 31;
        fg = 31;
        fb = 31;
        apply[0] = 1;
        apply[1] = 1;
      } else if ((u32)(frame - 332) <= 59) {
        fg = 30 - (frame - 332) / 2;
        apply[0] = 1;
        fr = fg;
        fb = fg;
        apply[1] = 1;
      } else if ((u32)(frame - 489) <= 42) {
        s32 d = frame - 489;

        fr = d / 4;
        fg = d / 6;
        fb = d / 16;
        apply[0] = 1;
      }
      for (j = 0; j != 2; j++) {
        if (apply[j] != 0) {
          u16 *src;
          u16 *dst;
          s32 n;

          if (j == 0) {
            src = (u16 *)(work->sheet + 0x5002);
            n = 240;
            dst = (u16 *)(OBJ_PLTT + 1);
          } else {
            dst = (u16 *)(BG_PLTT + 0x60);
            src = (u16 *)(battle + 0x56c);
            n = 128;
          }
          for (i = 1; i != n; i++) {
            s32 r = (*src & 31) + fr;
            s32 g = ((*src >> 5) & 31) + fg;
            s32 b = ((*src >> 10) & 31) + fb;

            if (r > 31) {
              r = 31;
            }
            if (g > 31) {
              g = 31;
            }
            if (b > 31) {
              b = 31;
            }
            if (r < 0) {
              r = 0;
            }
            if (g < 0) {
              g = 0;
            }
            if (b < 0) {
              b = 0;
            }
            *dst++ = b << 10 | g << 5 | r;
            src++;
          }
        }
      }
    }
    if ((u32)(frame - 554) <= 31 && !((frame - 554) & 1)) {
      ColorBuffer_ScaleFar(battle + 0x56c, (void *)(BG_PLTT + 0x60),
                           0x10000 - ((frame - 554) << 10), 128);
    }
    if (swapped == 0) {
      if (frame == 339) {
        swapped = 1;
        for (i = 0; i != 4; i++) {
          COPY_WORDS((void *)OBJ_TILES(work->objects[24 + i]),
                     gMapCellBuffer + (i << 10), 1024);
        }
      }
    } else if (frame == 450) {
      u8 *cells;

      swapped = 0;
      cells = Data_02011000;
      for (i = 0; i != 4; cells += 1024, i++) {
        COPY_WORDS((void *)OBJ_TILES(work->objects[24 + i]), cells, 1024);
      }
    }
    if (frame <= 593) {
      req = Data_08196ef8;
      place[3] = 0;
      place[1] = 0xff0000;
      {

        for (i = 0; i != 28; i++) {
          place[0] = (Data_081997b0[i] << 16) + 0x80000;
          place[2] = (Data_081997cc[i] << 16) + camZ;
          if (place[2] > 0xfffff) {
            if (place[2] <= 0xa80000) {
              Render_ApplyProjectedPlacementFar(work->objects[i], place,
                                                (s32 *)&req, 0);
            }
          }
        }
      }
      for (i = 0; i != 5; i++) {
        s32 h = i / 2;

        place[0] = ((i - h * 2) << 21) + drift + 0x480000;
        place[2] = (h << 21) + camZ + sway + 0x3c0000;
        if (place[2] > 0xfffff) {
          if (place[2] <= 0xa80000) {
            Render_ApplyProjectedPlacementFar(work->objects[34 + i], place,
                                              (s32 *)&req, 0);
          }
        }
      }
      for (i = 0; i != 11; i++) {
        place[0] = (Data_081997e8[i] << 16) - drift + 0x840000;
        place[2] = (Data_081997f4[i] << 16) + camZ + sway + 0x1d0000;
        if (place[2] > 0xfffff) {
          if (place[2] <= 0xa80000) {
            Render_ApplyProjectedPlacementFar(work->objects[39 + i], place,
                                              (s32 *)&req, 0);
          }
        }
      }
      req.a = -0x10001;
      for (i = 0; i != 6; i++) {
        place[0] = (Data_0819980a[i] << 16) + 0x80000;
        place[2] = (Data_08199810[i] << 16) + camZ;
        if (place[2] > 0xfffff) {
          if (place[2] <= 0xa80000) {
            Render_ApplyProjectedPlacementFar(work->objects[28 + i], place,
                                              (s32 *)&req, 0);
          }
        }
      }
    }
    verts = Heap_AllocEwram(512);
    model = Render_AllocateModelList(1);
    texture.logWidth = 5;
    shade = 0;
    texture.logHeight = 6;
    model->mesh = Data_0819931c;
    model->texture = &texture;
    model->kind = 6;
    model->vertices = verts;
    for (i = 0; i != 7; i++) {
      s32 st = Data_08199816[i];

      if (frame >= st && frame < st + 8) {
        s32 c = Data_08199832[i];

        shade = (st - frame) << 3;
        model->shade = shade;
        texture.pixels = work->sheet + c * 0x600;
        Render_ResetTransformState();
        Func_08015160((Data_08199824[i] - 48) << 16,
                      (Data_0819982b[i] - 44) << 16, 0);
        Func_080150e4(0);
        Func_080151e4(0x8000, 0x10000, 0x8000);
        Render_TransformVertices(Data_081991e0, verts, 4);
        Render_DrawModelList(model);
      }
    }
    if (frame == 489) {
      for (i = 0; i != 3; i++) {
        work->particles[i].variant = 0;
      }
    }
    for (j = 3; j != 64; j++) {
      p = &work->particles[j];
      if (p->variant >= 0) {
        s32 n = (p->variant >> 3) + 1;

        blit[0](canvas, sprites + gData_08197410[n - 1], HALF_HI(p->x) - n / 2,
                HALF_HI(p->y) - n, n, n * 2);
        BattleEffectKernels_IntegrateVector2(p, 63, 0);
        p->variant--;
      }
    }
    {
      for (i = 0; i != 17; i++) {
        u16 t = Data_0819983a[i];

        if (frame == t + 242) {
          work->transferMode = 1;
          work->transferValue = 0x1f1f1f1f;
        }
        if (frame == t + 243) {
          work->transferMode = 2;
          work->transferValue = 50;
        }
      }
    }
    if ((u32)(frame - 537) <= 55) {
      if (frame & 1) {
        work->transferMode = 1;
        work->transferValue = 0x1f1f1f1f;
      } else {
        work->transferMode = 2;
        work->transferValue = 50;
      }
    }
    if (frame == 324) {
      Sound_PlayFar(206);
    }
    if (frame == 560) {
      Sound_PlayFar(220);
    }
    if (frame == 602) {
      Sound_PlayFar(145);
    }
    texture.logWidth = 5;
    texture.logHeight = 7;
    model->shade = shade;
    model->kind = 6;
    model->mesh = gData_08199268;
    model->texture = &texture;
    model->vertices = verts;
    if ((u32)(frame - 489) <= 130) {
      shade = 0;
      for (i = 0; i != 3; i++) {
        s32 off = i * 12;
        s32 idx;

        p = &work->particles[i];
        idx = p->variant;

        if (frame == (Data_081998a4[i * 13 + idx] - 492) / 2 + 489) {
          s32 n = off + idx;
          s32 count = 0;
          s32 a = Data_08199916[n];
          u8 *src;
          s32 bx;
          s32 w;
          s32 h;
          s32 x;
          s32 x0;
          s32 y0;
          s32 y;

          if (a != 0) {
            p->x = Data_0819985c[n] << 3;
            p->y = Data_08199880[n] << 3;
            p->z = a << 8;
          }
          __asm__("" : "=r"(idx) : "0"(idx));
          p->velocityY = Data_081998f2[idx + off];
          p->velocityZ = Data_0819995e[idx + off];
          p->velocityX = Data_081999a6[idx + off];
          p->variant = idx + 1;
          if (p->velocityX == -1) {
            continue;
          }
          w = 10;
          src = sprites + gData_08197410[9];
          h = 20;
          x0 = p->x / 8 - 5;
          bx = 0x80000 - drift;
          x = x0 + (bx / 2 >> 16);
          y0 = p->y / 8 - 10;
          y = y0 + ((camZ + sway) >> 16);
          blit[0](canvas, src, x, y, w, h);
          switch (i) {
          case 0:
            Sound_PlayFar(186);
            break;
          case 1:
            Sound_PlayFar(187);
            break;
          case 2:
            Sound_PlayFar(222);
            break;
          }
          {
            s32 half = (bx + (s32)((u32)bx >> 31)) >> 1;
            s32 j;

            q = &work->particles[3];
            for (j = 3; j != 64; j++, q++) {
              if (q->variant < 0) {
                s32 angle;
                s32 speed;

                q->x = (p->x << 13) + half;
                q->y = (p->y << 13) + camZ + sway;
                angle = Random16();
                speed = (Random16() & 255) + 127;
                q->velocityX = Trig_Sin(angle) * speed >> 6;
                q->velocityY = Trig_Cos(angle) * speed >> 6;
                q->variant = (Random16() & 7) + 16;
                if (++count == 10) {
                  break;
                }
              }
            }
          }
        }
        if (p->velocityX != -1) {
          model->shade = shade;
          texture.pixels = work->sheet + 0x2000 + (p->velocityX << 12);
          Render_ResetTransformState();
          {
            s32 b = 0x80000 - drift;

            Func_08015160(((p->x / 8 - 64) << 16) + b / 2,
                          ((p->y / 8 - 64) << 16) + camZ + sway, 0);
          }
          Func_080151e4(0x8000, 0x10000, 0x8000);
          Func_080150e4(p->z);
          Func_080151e4(0x40000, 85536, 0x8000);
          Render_TransformVertices(Data_081991d0, verts, 4);
          Render_DrawModelList(model);
          p->y += p->velocityY * 2;
          p->z += p->velocityZ * 2;
        }
      }
    }
    if (frame == 602) {
      Iwram_FillWords(canvas, 0x4000, 0x3f3f3f3f);
    }
    if (frame == 630) {
      Iwram_FillWords(canvas, 0x4000, 0x1f1f1f1f);
    }
    if (frame == 634) {
      Iwram_FillWords(canvas, 0x4000, 0x1f1f1f1f);
    }
    if ((u32)(frame - 552) <= 43) {
      p = work->particles;
      if (!(frame & 1)) {
        p->y = (Random16() & 0x3fff) + 0x6000;
        p->z = (Random16() % 3 << 12) + 0x2000;
      }
      texture.pixels = work->sheet + p->z;
      Render_ResetTransformState();
      Func_08015160((0x80000 - drift) / 2 + 0x40000, camZ + 0xcc0000, 0);
      Func_080151e4(0x8000, 0x10000, 0x8000);
      Func_080150e4(p->y);
      Func_080151e4(0x40000, 85536, 0x8000);
      Render_TransformVertices(Data_081991d0, verts, 4);
      Render_DrawModelList(model);
    }
    if ((u32)(frame - 602) <= 15 || (u32)(frame - 630) <= 3) {
      s32 n = 1;
      s32 base = Random16() % 3;
      s32 sx = 0x50000;
      s32 spread = 8;

      if (frame > 611) {
        sx = sx - frame * 61845 + 0x2418834;
      }
      if (sx <= 65535) {
        sx = 0x10000;
      }
      if (frame <= 613) {
        if (frame > 605) {
          n = 3;
        } else if (frame > 603) {
          n = 2;
        }
      }
      model->kind = 6;
      model->shade = shade;
      p4 = work->particles;
      for (i = 0; i != n; i++, p4++) {
        p4->x = (Random16() % spread - spread / 2) << 16;
        p4->y = camZ + 0xc80000;
        p4->z = ((base + i) % 3 << 12) + 0x2000;
        texture.pixels = work->sheet + p4->z;
        Render_ResetTransformState();
        Func_08015160(0x10000, p4->y, 0);
        Func_080151e4(sx, 0x10000, 0x8000);
        Func_08015160(p4->x, 0, 0);
        Func_080151e4(0x8000, 0x20000, 0x10000);
        Func_080150e4(0x8000);
        Render_TransformVertices(Data_081991d0, verts, 4);
        Render_DrawModelList(model);
      }
    }
    if (frame == 552) {
      Resource_LoadAndDecompress((s32)&ResourceId_Data0B4, Data_02012000, 0, 0);
    }
    if (frame == 551) {
      Resource_LoadAndDecompress((s32)&ResourceId_Data0EE, work->sheet, 0, 0);
      Resource_LoadAndDecompress((s32)&ResourceId_Data0EF, work->sheet + 0x1000,
                                 0, 0);
    }
    if (frame == 602) {
      Resource_LoadAndDecompress((s32)&ResourceId_Data0C2, Data_02012000, 0, 0);
    }
    if ((u32)(frame - 602) <= 6) {
      camZ += 0x100000;
    }
    {
      s32 d = frame - 552;

      if ((u32)d <= 58) {
        s32 s = d * 4424;
        s32 a;
        s32 b;

        shade = -16;
        if (s > 0x22538) {
          s = 0x22538;
        }
        texture.logWidth = 6;
        texture.logHeight = 6;
        model->shade = shade;
        model->kind = 7;
        model->texture = &texture;
        model->mesh = gData_08199340;
        model->vertices = verts;
        texture.pixels = work->sheet + 0x1000;
        Render_ResetTransformState();
        b = 0x80000 - drift;
        Func_08015160(b / 2 + 0x50000, camZ + 0xe60000, 0);
        Func_080151e4(0x8000, 0x10000, 0x8000);
        SceneTransform_ApplyPitch(18768);
        a = -frame << 10;
        Func_080150e4(a);
        Func_0801521c(s);
        Func_08015160(0, -0xf0000, 0);
        Render_TransformVertices(Data_081991e0, verts, 4);
        Func_08015160(0, 0xf0000, 0);
        Render_DrawModelList(model);
        texture.pixels = work->sheet;
        a += 10922;
        for (i = 0; i != 3; i++, a += 21845) {
          Transform_Push();
          Func_080150e4(i * 21845 + 10922);
          model->shade = -24 - (Trig_Cos(a) << 3 >> 16);
          Func_08015160(0, -0x100000, 0);
          if (frame > 579) {
            s32 v = (frame - 580) * 1096;

            if (v > 19712) {
              v = 19712;
            }
            SceneTransform_ApplyPitch(-v);
          }
          Render_TransformVertices(gData_081991f0, verts, 4);
          Render_DrawModelList(model);
          Transform_Pop();
        }
        if (frame > 589) {
          s = s + frame * 11060 + -0x6391d8;
        }
        if (s > 0 && frame <= 601) {
          shade = -16;
          if (frame > 589) {
            shade = shade - frame * 2 + 1180;
          }
          if (shade > -64) {
            model->shade = shade;
            texture.pixels = Data_02012000;
            Render_ResetTransformState();
            if (frame > 553) {
              s32 v = (554 - frame) * 120000;

              if (v < -0xcd140) {
                v = -0xcd140;
              }
              Func_08015160(0, v, 0);
            }
            Func_08015160(((b + (s32)((u32)b >> 31)) >> 1) + 0x50000, camZ + 0xdc0000, 0);
            Func_080151e4(0x10000, 32000, 0x10000);
            Func_080150e4(frame << 11);
            Func_0801521c(s);
            Render_TransformVertices(Data_081991e0, verts, 4);
            Render_DrawModelList(model);
          }
        }
      }
    }
    if (frame > 601) {
      s32 s = (frame - 602) * 12192 + 0x2000;

      shade = 0;
      if (frame > 611) {
        shade = shade - frame * 8 + 4896;
      }
      if (shade > -64) {
        model->shade = shade;
        model->mesh = gData_08199364;
        model->kind = 7;
        texture.pixels = Data_02012000;
        texture.logWidth = 7;
        texture.logHeight = 7;
        Render_ResetTransformState();
        Func_08015160(0x10000, -0x100000, 0);
        Func_080151e4(0x10000, 32000, 0x10000);
        Func_080150e4(frame << 10);
        Func_0801521c(s);
        Render_TransformVertices(Data_081991e0, verts, 4);
        Render_DrawModelList(model);
      }
    }
    Heap_Release(model);
    Heap_Release(verts);
    if ((u32)(frame - 554) <= 51) {
      s32 r = frame - 553;
      s32 y = 90 - (frame - 554) / 2;

      if (frame > 600) {
        y = y + (frame << 4) - 9616;
      }
      if (y <= 63) {
        y = 64;
      }
      if (r <= 38) {
        Func_0815b434(gMapCellBuffer, r, 96000);
      } else {
        r = 38;
      }
      Func_0818caa8(gMapCellBuffer, 62, y, r);
    }
    work->transferPending = 1;
    wait(1);
    frame++;
  }
  for (i = 0; i != 50; i++) {
    ResourceObject_ReleaseFar(work->objects[i]);
  }
  Func_08020388();
  *(s32 *)(battle + 84) = VramCache_AllocEntry(128);
  BattleEffect_SelectLivingTargets(work->effect);
  for (i = 0; i != effect->count; i++) {
    Func_0814cd48(effect->actors[i], 1, -1, -1, 0);
    Func_0814cd48(effect->actors[i], 0, -1, -1, 0);
  }
  Palette_LoadBasicObjColors();
  Func_0814cca8();
  gCameraSceneParameters[4] = 240;
  work->fadeFrames = 0;
  work->fadeStep = 0;
  gBgScroll[1].y = 32;
  BattleBackground_LoadFar(1, (s32)&ResourceId_AerialViewBackdrop, 0);
  Func_08164b2c(0, -16, -16);
  REG_BLDALPHA = 0x1010;
  Resource_LoadAndDecompress((s32)&ResourceId_LightningBoltSheet,
                             gMapCellBuffer, 1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_TornadoSheet, work->sheet + 0x800,
                             1, 0);
  Iwram_ClearWords(work->sheet + 0x1456, 0x4000);
  for (i = 0; i != 4; i++) {
    {
      s32 j;
      s32 row = i << 12;
      u8 *cells = Ram_MapCellBuffer;

      for (j = 0; j != 120; j++) {
        u8 *dst = work->sheet + (row + j * 32) + 0x145a;
        u8 *src = cells + (i * 45 * 64 + j * 24);
        s32 m;

        for (m = 0; m != 24; m++) {
          *dst++ = *src++;
        }
      }
    }
  }
  Resource_LoadAndDecompress((s32)&ResourceId_Data0EE, gMapCellBuffer, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data0EF, Data_02011000, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data0C2, Data_02012000, 0, 0);
  {
    s32 rise;
    s32 top;
    void *verts2;
    s32 sh2;
    s32 r;
    s32 ry;

    rise = 0;
    work->transferMode = 2;
    work->transferValue = 50;
    frame = 0;
    if (!(gInput.repeat & 3)) {
      do {
        if (frame == 0) {
          Iwram_FillWords(canvas, 0x4000, 0);
          rise = -0x400000;
          Func_08164b2c(0, -16, -16);
          work->transferMode = 2;
          work->transferValue = 50;
          COPY_WORDS((void *)BG_PLTT,
                     Resource_GetTableEntry((s32)&ResourceId_EffectPalette02),
                     512);
        }
        if (rise <= 0xfffff) {
          rise += 0x10000;
        }
        {
          s32 d = frame - 96;

          if ((u32)d <= 23) {
            Func_08164b2c(-d, -16, frame - 112);
            work->shakeFrames = 8;
          }
        }
        if (frame > 139) {
          work->transferMode = 1;
          if (frame > 153) {
            work->transferValue = 0x3f3f3f3f;
          } else {
            work->transferValue = 0x10101010;
          }
          Func_08164a4c(1, 1, 1);
        }
        if (frame == 80 || frame == 83) {
          u16 *c = (u16 *)(BG_PLTT + 0x60);

          for (i = 0; i != 128; i++, c++) {
            *c ^= 0x7fff;
          }
        }
        if (frame == 82) {
          work->shakeFrames = 8;
          Sound_PlayFar(208);
          Iwram_FillWords(canvas, 0x4000, 0x3f3f3f3f);
        }
        if (frame == 96) {
          Sound_PlayFar(104);
        }
        if (frame == 128) {
          u16 *c;

          work->shakeFrames = 16;
          c = (u16 *)(BG_PLTT + 0x60);
          for (i = 0; i != 128; i++, c++) {
            *c ^= 0x7fff;
          }
        }
        if (frame > 128) {
          s32 d = (frame - 128) << 1;

          Func_08164b2c(d - 11, frame - 136, 3 - d);
        }
        ry = rise >> 16;
        r = 16;
        if (frame > 92) {
          r = frame - 75;
        }
        if (frame <= 79) {
          r += Trig_Sin(frame << 11) * 2 >> 16;
        }
        if (r <= 64) {
          Func_0815b434(work->sheet, r, 96000);
        } else {
          r = 64;
        }
        top = ry - r / 2 + 60;
        Func_0818caa8(work->sheet, 60, top, r);
        if (frame > 95) {
          {
            for (i = 0; i != 4; i++) {
              s32 n = i & 3;
              s32 angle = Random16();
              s32 x =
                  (Trig_Sin(angle) * r >> 17) + 60 - (gData_08197492[n] >> 1);
              s32 y =
                  top + (Trig_Cos(angle) * r >> 17) - (gData_08197498[n] >> 1);

              LOAD_ALT_BLITTER(SLOT_ALT, Data_081999ee[Random16() & 3] | 3);
              ((DrawRectangle)Ram_HeapSlots->unkb8[1])(
                  canvas, work->sheet + 0x800 + gData_08197486[n], x, y,
                  gData_08197492[n], gData_08197498[n]);
              HeapSlot_Release(SLOT_ALT);
            }
          }
        }
        verts2 = Heap_AllocEwram(512);
        model2 = Render_AllocateModelList(1);
        if (frame <= 127) {
          s32 s = 85536;
          s32 a;

          if (frame > 71) {
            s = (frame << 11) + -0xf1e0;
          }
          sh2 = -16;
          if (frame > 119) {
            sh2 = 944 - frame * 8;
          }
          if (s > 0x22538) {
            s = 0x22538;
          }
          texture2.logWidth = 6;
          texture2.logHeight = 6;
          model2->shade = sh2;
          model2->kind = 6;
          model2->texture = &texture2;
          model2->mesh = gData_08199340;
          model2->vertices = verts2;
          texture2.pixels = Data_02011000;
          Render_ResetTransformState();
          Func_08015160(-0x20000, rise, 0);
          Func_080151e4(0x8000, 0x10000, 0x8000);
          SceneTransform_ApplyPitch(18768);
          a = frame << 9;
          Func_080150e4(a);
          Func_0801521c(s);
          Func_08015160(0, -0xf0000, 0);
          Render_TransformVertices(Data_081991e0, verts2, 4);
          Func_08015160(0, 0xf0000, 0);
          Render_DrawModelList(model2);
          texture2.pixels = gMapCellBuffer;
          for (i = 0; i != 3; i++) {
            s16 tone = sh2 + (Trig_Cos(a + i * 21845 + 10922) << 3 >> 16) - 16;
            s32 v;

            Transform_Push();
            Func_080150e4(i * 21845 + 10922);
            model2->shade = tone;
            Func_08015160(0, -0x100000, 0);
            v = 19712;
            if (frame > 63) {
              v = v - (frame << 10) + 0x10000;
            }
            if (v < 0) {
              v = 0;
            }
            SceneTransform_ApplyPitch(-v);
            Render_TransformVertices(gData_081991f0, verts2, 4);
            Render_DrawModelList(model2);
            Transform_Pop();
          }
        }
        {
          s32 t = frame * 3;

          {
            for (i = 0; i != 2; i++) {
              s32 d = i * 8;
              s32 start = d + 80;
              s32 s;

              if (frame < start) {
                continue;
              }
              sh2 = 0;
              if (i == 0) {
                s = (t - start * 3) * 127 * 32 + 0x2000;
                if (frame >= d + 96) {
                  sh2 = (start - frame) * 8 + 128;
                }
              } else {
                s = (t - start * 3) * 127 * 16 + 0x2000;
                if (frame >= d + 112) {
                  sh2 = (start - frame) * 8 + 256;
                }
              }
              if (sh2 > -64) {
                model2->shade = sh2;
                model2->mesh = gData_08199364;
                model2->kind = 6;
                texture2.pixels = Data_02012000;
                texture2.logWidth = 7;
                texture2.logHeight = 7;
                Render_ResetTransformState();
                Func_08015160(0, 0x100000, 0);
                Func_080151e4(0x10000, 32000, 0x10000);
                Func_080150e4(frame << 10);
                Func_0801521c(s);
                Render_TransformVertices(Data_081991e0, verts2, 4);
                Render_DrawModelList(model2);
              }
            }
          }
        }
        texture2.logWidth = 5;
        texture2.logHeight = 7;
        model2->kind = 6;
        model2->mesh = gData_08199268;
        model2->shade = 0;
        if (frame == 0) {
          p = work->particles;
          for (i = 0; i != 16; i++, p++) {
            p->z = Data_081999f2[i];
            p->velocityZ = (Random16() & 255) + 128;
            if (Random16() & 1) {
              p->velocityZ = -p->velocityZ;
            }
            p->variant = Random16() & 3;
          }
        }
        for (i = 0; i != 8; i++) {
          if (frame == 128 + i * 4) {
            Sound_PlayFar(144);
          }
        }
        p = work->particles;
        for (i = 0; i != 16; i++, p++) {
          s32 st = i + 128;

          if (frame >= st && frame < i + 144) {
            s32 s = (frame - st) << 13;

            if (s > 0x10000) {
              s = 0x10000;
            }
            texture2.pixels = work->sheet + 0x1456 + (p->variant << 12);
            Render_ResetTransformState();
            Func_080150e4(p->z);
            p->z += p->velocityZ;
            Func_080151e4(s << 2, s, 0x10000);
            Render_TransformVertices(gData_081991b0, verts2, 4);
            Render_DrawModelList(model2);
          }
        }
        Heap_Release(model2);
        Heap_Release(verts2);
        if (frame > 133) {
          if (frame == 134) {
            struct EffectStep *p = &work->particles[16];

            for (i = 0; i != 48; i++, p++) {
              s32 a = Random16();
              s32 angle = i * 1365;
              s32 rr = (a & 31) + 32;

              p->x = (rr * Trig_Sin(angle) >> 16) + 60;
              p->y = (rr * Trig_Cos(angle) >> 16) + 60;
              p->z = (Random16() & 3) + 5;
              p->variant = -(Random16() & 15);
            }
          }
          p3 = &work->particles[16];
          for (i = 0; i != 48; i++, p3++) {
            if ((u32)p3->variant <= 7) {
              s32 n = (Trig_Sin(p3->variant << 12) * p3->z >> 16) + 1;

              blit[0](canvas, sprites + gData_08197410[n - 1], p3->x - n / 2,
                      p3->y - n, n, n * 2);
            }
            p3->variant++;
          }
        }
        if (work->shakeFrames > 0) {
          s32 a = Random16();
          s32 b = Random16();

          a = (a & 7) - 4;
          b = (b & 7) + 28;
          gBgScroll[1].x = a;
          gBgScroll[1].y = b;
          work->shakeFrames--;
        } else {
          gBgScroll[1].x = 0;
          gBgScroll[1].y = work->savedBg1Y;
        }
        work->transferPending = 1;
        wait(1);
        frame++;
      } while (frame != 160 && !(gInput.repeat & 3));
    }
  }
  {
    s32 slide;
    s32 slideStep;
    u8 *bolts;

    slide = 0;
    slideStep = 0;
    Iwram_FillWords(canvas, 0x4000, 0x3f3f3f3f);
    work->transferPending = 1;
    wait(1);
    gBgScroll[1].x = savedScroll;
    BattleBackground_LoadFar(1, *(u16 *)(battle + 0x670), 24);
    Func_08164b2c(-16, -16, -4);
    Resource_LoadAndDecompress((s32)&ResourceId_SmokeBandSheet, work->sheet, 1,
                               1);
    Resource_LoadAndDecompress((s32)&ResourceId_ClawSlashSheet,
                               work->sheet + 0x800, 1, 0);
    Resource_LoadAndDecompress((s32)&ResourceId_Data134, sprites, 0, 0);
    bolts = gData_02014000;
    Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet,
                               work->sheet + 0x1800, 1, 0);
    Resource_LoadAndDecompress((s32)&ResourceId_LightningBoltSheet, bolts, 1,
                               0);
    Render_BuildQuadStrip(Data_02012000, 4, 32, 11);
    REG_BG2CNT = 0x784;
    frame = 0;
    do {
      if ((gInput.repeat & 3) && frame > 2 && frame <= 87) {
        frame = 88;
      }
      if (frame == 88) {
        Iwram_FillWords(canvas, 0x4000, 0x3f3f3f3f);
        work->transferMode = 3;
        work->transferValue = 0x02020202;
        gBgScroll[1].x = savedScroll;
        gCameraSceneParameters[3] = 120;
        work->scrollStepX = 0;
        Func_081180e8(134);
      }
      if (frame == 0) {
        Iwram_FillWords(canvas, 0x4000, 0x3f3f3f3f);
        work->scrollStepX = -16;
        gCameraSceneParameters[3] = -968;
        gCameraSceneParameters[4] = 120;
        gBgScroll[1].x = savedScroll + 1088;
        Graphics_LoadGreyRamp();
        work->transferMode = 3;
        work->transferValue = 0x02020202;
        for (i = 0; i != 164; i++) {
          FIXED_ADDRESS((struct EffectStep *), 0x02016d00, Data_02016d00)[i].variant = -1;
        }
        slideStep = 0;
        slide = 0x800000;
      }
      if (frame == 16) {
        work->transferMode = 2;
        work->transferValue = 75;
      }
      if ((u32)(frame - 16) <= 47) {
        Func_0815f0a0((s32)&ResourceId_EffectPalette02);
      }
      if (frame == 66) {
        for (i = 0; i != effect->count; i++) {
          s32 j;

          BattleEffect_StartActorMotion(effect->actors[i], 1, 0x40000, 0x80000,
                                        0x40000, 150);
          Func_0814cd48(effect->actors[i], 7, 5, i, 16);
          EffectPosition_ProjectBattler(effect->actors[i], &pos);
          q = &Data_02016d00[64 + i * 16];
          for (j = 0; j != 16; j++, q++) {
            s32 angle;
            s32 speed;

            q->x = (pos.x / 2) << 16;
            q->y = 0x500000;
            angle = (Random16() & 0x7fff) + 0x4000;
            speed = (Random16() & 127) + 128;
            q->velocityX = Trig_Sin(angle) * speed >> 5;
            q->velocityY = Trig_Cos(angle) * speed >> 4;
            q->variant = 16;
          }
        }
        work->shakeFrames = 16;
        Sound_PlayFar(145);
      }
      if ((u32)(frame - 72) <= 15) {
        work->transferMode = 4;
        work->transferValue = 0x01010101;
        Func_08164a4c(1, 1, 1);
      }
      if (frame >= 0) {
        s32 x = 0;

        if (frame > 71) {
          x = x - frame * 8 + 576;
        }
        x += 116;
        q = &Data_02016d00[(frame & 31) * 2];
        for (i = 0; i != 2; i++, q++) {
          s32 angle;
          s32 speed;

          q->x = x << 16;
          q->y = (Random16() % 112 + 4) << 16;
          angle = (Random16() & 0x7fff) + 0x8000;
          speed = (Random16() & 127) + 128;
          q->velocityX = Trig_Sin(angle) * speed >> 6;
          q->velocityY = Trig_Cos(angle) * speed >> 6;
          q->variant = 0;
        }
        q2 = Data_02016d00;
        for (i = 0; i != 64; i++, q2++) {
          if ((u32)q2->variant <= 11) {
            blit[0](canvas, work->sheet + 0x1800 + (q2->variant / 2 << 11),
                    HALF_HI(q2->x) - 16, HALF_HI(q2->y) - 32, 32, 64);
            BattleEffectKernels_IntegrateVector2(q2, 63, 0);
            q2->variant++;
          }
        }
      }
      if (frame <= 87) {
        s32 jitter = 0;
        s32 z = 0;
        s32 y = 0;
        s32 off = 0;
        s32 a;
        s32 b;
        s32 c;

        for (j = 0; j != 12; j++, off += 8) {
          s8 *v = (s8 *)gMapCellBuffer + off;
          s32 x = 0;

          for (i = 0; i != 2; i++, x += 10, v += 4) {
            v[1] = y;
            v[0] = x + (Trig_Sin((frame + j * 2) << 10) * 2 >> 16) + jitter - 5;
            v[2] = z;
          }
          jitter = jitter + (Random16() & 7) - 4;
          y -= 6;
        }
        model3 = Render_AllocateModelList(1);
        model3->kind = 6;
        model3->texture = &texture3;
        model3->scrollV = 0;
        model3->shade = 0;
        model3->vertices = Data_02011000;
        if (frame == 16) {
          slideStep = -0x80000;
        }
        if ((u32)(frame - 48) <= 5) {
          slideStep = 0x80000;
        }
        if (frame > 59) {
          slideStep = -0x100000;
        }
        if (frame > 31) {
          slideStep = slideStep * 48 / 64;
        }
        slide += slideStep;
        texture3.logWidth = 6;
        texture3.logHeight = 5;
        texture3.pixels = work->sheet;
        model3->scrollU = (frame << 3) & 127;
        model3->mesh = Data_02012000;
        Render_ResetTransformState();
        Func_080151e4(0x8000, 0x10000, 0x8000);
        a = frame * 3 * 127 * 16;
        b = frame << 13;
        c = Trig_Sin(a);
        Func_08015160(slide, (c << 5) + Trig_Cos(b) * 24, 0);
        Func_0801521c(55536);
        Transform_Push();
        Func_080150e4(0x4000);
        Func_0801521c(0x42ee0);
        Render_TransformVertices(gMapCellBuffer, Data_02011000, 24);
        Render_DrawModelList(model3);
        Transform_Pop();
        model3->mesh = gData_08199340;
        texture3.logWidth = 6;
        texture3.logHeight = 6;
        model3->scrollU = 0;
        texture3.pixels = work->sheet + 0x800;
        Func_08015160(-0x20000, (Trig_Sin(frame << 10) << 2) + -0x60000, 0);
        Func_080150e4(-0x800);
        {
          s32 s0 = Trig_Sin(a);
          s32 y0 = (s0 << 5) + Trig_Cos(b) * 24;
          s32 s1;
          s32 y1;

          a += 6096;
          s1 = Trig_Sin(a);
          b += 0x2000;
          y1 = (s1 << 5) + Trig_Cos(b) * 24;
          Func_080150e4((y0 - y1) / 512 + -0x400);
        }
        Func_080151e4(0x21170, 105536, 0x10000);
        Render_TransformVertices(Data_081991e0, Data_02011000, 4);
        Render_DrawModelList(model3);
        Heap_Release(model3);
        if (frame <= 87) {

          p2 = work->particles;
          {

            for (i = 0; i != 6; i++, p2++) {
              s32 lim = i * 8 + 16;

              if (frame == lim) {
                p2->x = (Random16() & 31) + 64;
                p2->variant = Random16();
                work->shakeFrames = 4;
                switch (i & 3) {
                case 0:
                  Sound_PlayFar(227);
                  break;
                case 1:
                  Sound_PlayFar(171);
                  break;
                case 2:
                  Sound_PlayFar(208);
                  break;
                case 3:
                  Sound_PlayFar(172);
                  break;
                }
                q = &Data_02016d00[64 + (i & 7) * 12];
                for (j = 0; j != 12; j++, q++) {
                  s32 angle;
                  s32 speed;

                  q->x = p2->x << 16;
                  q->y = 0x680000;
                  angle = (Random16() & 0x7fff) + 0x4000;
                  speed = (Random16() & 127) + 128;
                  q->velocityX = Trig_Sin(angle) * speed >> 5;
                  q->velocityY = Trig_Cos(angle) * speed >> 4;
                  q->variant = 16;
                }
              }
              if (frame >= lim && frame < lim + 8) {
                blit[0](canvas, bolts + ((p2->variant / 2) & 3) * 2880,
                        p2->x - 12, 0, 24, 120);
                p2->variant++;
              }
            }
          }
          q = &Data_02016d00[64];
          for (i = 0; i != 100; i++, q++) {
            if (q->variant >= 0) {
              s32 n = (q->variant >> 2) + 2;

              blit[0](canvas, sprites + gData_08197410[n - 1],
                      HALF_HI(q->x) - n / 2, HALF_HI(q->y) - n, n, n * 2);
              BattleEffectKernels_IntegrateVector2(q, 56, 0x2000);
              q->velocityX -= work->scrollStepX << 12;
              q->variant--;
            }
          }
        }
      }
      gCameraSceneParameters[3] -= work->scrollStepX;
      gBgScroll[1].x += work->scrollStepX;
      for (i = 0; i != 16; i++) {
        if (frame == i + 62) {
          work->scrollStepX++;
        }
      }
      Camera_ApplyVerticalShake(8);
      BattleEffect_CountDownBattlerTints();
      frame++;
      work->transferPending = 1;
      wait(1);
    } while (frame != 104);
    transition[4] = 0;
    Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
    HeapSlot_Release(SLOT_BLITTER);
    BattleEffect_EndCanvasLayer();
  }
}

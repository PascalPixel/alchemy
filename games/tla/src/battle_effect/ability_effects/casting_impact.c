/// @file
/// The casting impacts, and the step of the background colours toward a
/// palette.
// Part of ability_effects.c.

#include "../ability_effects_internal.h"

// Steps the 64 background colours one shade toward the resource's palette.
void BattleEffect_StepPaletteToResource(s32 resource) {
  u16 dst[64];
  u16 *col = (u16 *)BG_PLTT;
  s32 i;

  Dma_Set(Resource_GetTableEntry(resource), dst, DMA_COPY_WORDS(0x20),
          REG_DMA3);
  dst[0] = 0;
  for (i = 0; i != 64; i++, col++) {
    s32 r = *col & 0x1f;
    s32 g = (*col >> 5) & 0x1f;
    s32 b = (*col >> 10) & 0x1f;
    s32 dst_r = dst[i] & 0x1f;
    s32 dst_g = (dst[i] >> 5) & 0x1f;
    s32 dst_b = (dst[i] >> 10) & 0x1f;

    if (r < dst_r)
      r++;
    else if (r > dst_r)
      r--;
    if (g < dst_g)
      g++;
    else if (g > dst_g)
      g--;
    if (b < dst_b)
      b++;
    else if (b > dst_b)
      b--;
    dst[i] = (b << 10) | (g << 5) | r;
  }
  Dma_Set(dst, (void *)BG_PLTT, DMA_COPY_HALFWORDS(64), REG_DMA3);
}

struct Particle {
  s32 x;
  s32 y;
  s32 z;
  s32 vx;
  s32 vy;
  s32 vz;
  s32 life;
};

struct SummonWork {
  struct Particle particle[64];
  u8 frames[0x5100];
  u8 frames2[0x1880];
  s32 wave[160];
  u8 unk7300[0x480];
  s32 unk7780;
  s32 unk7784;
  u8 unk_7788[0x20];
  s32 unk77a8;
  u8 unk77ac[8];
  s32 unk77b4;
  s32 unk77b8;
  u8 unk77bc[0x12c];
  s32 unk78e8;
};

typedef s32 (*DrawFunc)(void *, const void *, s32, s32, s32, s32);

struct DrawFuncs {
  DrawFunc draw[2];
};

struct Texture {
  u8 width;
  u8 height;
  u16 unk2;
  u8 *pixels;
};

struct Model {
  s32 kind;
  s32 unk4;
  const void *mesh;
  void *matrix;
  struct Texture *texture;
  long fade;
  u8 unk18;
  u8 frame;
};

struct IwramPointers {
  u8 unk00[0x30];
  void *bg;
  u8 unk34[0x28];
  struct SummonWork *summon;
  void *screen;
  void *tiles;
  DrawFunc slot68;
  u8 unk6c[0x50];
  DrawFunc slotbc;
};

/// The heap-slot table (gHeapSlots), read through the slots this effect uses.
#define IWRAM ((struct IwramPointers *)Ram_HeapSlots)

extern char ResourceId_Data0B4;
extern char ResourceId_EffectPalette01;
extern char ResourceId_Data0C1;
extern char ResourceId_Data0C2;
extern char ResourceId_ImpFaceSheet;
extern char ResourceId_Data0DA;
extern char ResourceId_Data0DD;
extern char ResourceId_BoulderPicture;
extern char ResourceId_Data0F0;
extern char ResourceId_Data0F1;
extern char ResourceId_CloudPuffSheet;
extern char ResourceId_FlameSheetA;
extern char ResourceId_FireballSheet;
extern char ResourceId_BlueFlameSheet;
extern char ResourceId_Data134;
extern char ResourceId_RuneSheet;
extern char ResourceId_FlashBurstSheet;
extern char ResourceId_RedPaletteB;
extern char ResourceId_OrangePaletteB;
extern char ResourceId_BoulderSheet;
extern char ResourceId_MarsDjinnSheet;
extern char ResourceId_JupiterDjinnSmallSheet;
extern char ResourceId_MercuryDjinnSmallSheet;
extern char ResourceId_VenusDjinnSmallSheet;
extern char ResourceId_FireStreakSheet;
extern char ResourceId_SmokeSheet;
extern char ResourceId_LimePalette;
extern char ResourceId_CyanPalette;
extern char ResourceId_TanPalette;
extern char ResourceId_EffectPalette05;
extern char ResourceId_SkullSheet;
extern char ResourceId_CounterRevealSheet;
extern char ResourceId_RedCrescentSheetA;
extern char ResourceId_BlueBeastSheet;
extern char ResourceId_RedCrescentSheetB;
extern char ResourceId_EmberStreakSheet;
extern char ResourceId_IceChipSheet;
extern char ResourceId_IceBlockSheet;
extern char ResourceId_PinkBurstSheet;
extern char ResourceId_DustPuffSheet;
extern char ResourceId_BlastSheet;
extern char ResourceId_RockSpireSheet;
extern char ResourceId_LightningBoltSheet;
extern char ResourceId_TwelveModeImage;
extern char ResourceId_TornadoSheet;
extern char ResourceId_CrystalSheet;

extern u16 gBattleEffect_MoteCells[];
extern u8 gData_08197467[];
extern u8 gBattleEffect_PuffArcCellHeights[];
extern u8 gBattleEffect_PuffArcCellBiasY[];
extern u16 gBattleEffect_PuffArcCells[];
extern u16 gBattleEffect_GlintCells[];
extern u8 gData_08197492[];
extern u8 gData_08197498[];
extern u8 gSpire_ShardWidths[];
extern u8 gData_081974ad[];
extern u16 gSpire_ShardCells[];
extern u8 gData_0819750c[];
extern u8 gData_0819751a[];
extern u8 gPrism_ShardStarts[];
extern u8 gPrism_ShardWidths[];
extern u8 gData_08197826[];
extern s32 gData_08197834[];
extern const u8 gData_0819887e[];
extern const u8 gData_08198882[];
extern const u8 gData_08198886[];
extern const u16 gData_0819888a[];
extern const u16 gData_0819889e[];
extern const u16 gData_081988b2[];
extern u16 gData_081988c6[];
extern u16 gData_081988da[];
extern const u16 gData_081988ee[];
extern const u16 gData_081988fc[];
extern const u16 gData_0819890a[];
extern const u16 gData_08198918[];
extern u16 gData_08198926[];
extern const u16 gData_08198934[];
extern const u8 gData_0819893a[];
extern const u8 gData_0819893d[];
extern u8 gData_08198940[][2];
extern const u16 gData_08198946[];
extern const u8 gData_0819894e[];
extern const u8 gData_08198952[];
extern const u8 gData_08198956[];
extern const s8 gData_0819895a[];
extern const u8 gData_0819895e[];
extern const u8 gData_08198962[];
extern const u8 gData_08198966[];
extern const u8 gData_08198c6c[];
extern const u8 gData_08198d2c[];
extern const u8 gData_08199090[];
extern const u8 gData_081990d0[];
extern const u8 gData_081991b0[];
extern const u8 gData_08199210[];
extern const u8 gData_08199268[];
extern const u8 gData_08199340[];
extern const u8 gData_08199364[];

s32 Trig_Cos(s32);
s32 Trig_Sin(s32);

void Func_08015160(s32, s32, s32);
void SceneTransform_ApplyScale3(s32, s32, s32);
void Func_0801521c(s32);

void BattleEffectKernels_IntegrateVector2(struct EffectStep *step, s32 damping,
                                          s32 gravity);
void BattleEffect_CountDownBattlerTints(void);
void BattleEffect_StartCanvasLayer(s32);
void BattleEffect_StartTiledCanvas(s32);
void BattleEffect_EndCanvasLayer(void);
void BattleEffect_FetchRectangleBlitters(s32 flip, void **out);
void BattleEffect_RunMysticFlame(struct BattleEffectArgument *);
void BattleEffect_TintBattler(s32, s32, s32, s32, s32);
void SummonEffect_RunCanvasMode3(void *action);
void SummonEffect_RunCanvasMode1(void *action);
void SummonEffect_RunCanvasMode2(void *action);
void BattleCamera_Shake(s32 rangeX, u32 rangeY);
void Func_0815b434(u8 *map, s32 size, s32 scale);

/* VIEW(fxdisp): routine Graphics_PackTileRows as void
 * Graphics_PackTileRows(void *, void *, s32, s32); the definition has void
 * Graphics_PackTileRows(u8 *src, u8 *dst, s32 stride); tried as the
 * definition's void Graphics_PackTileRows(u8 *src, u8 *dst, s32 stride), which
 * the object already declares, Func_0815f16c moves: 1 of 7568 instructions in
 * tla-ja. */
extern void Graphics_PackTileRows__fxdisp(void *, void *, s32,
                                          s32) __asm__("Graphics_PackTileRows");
void Func_0818caa8(void *, s32, s32, s32);
s32 Func_08196404(s32 slot, s32 widthShift, s32 heightShift, s32 flags,
                  u32 mode);
void Render_TransformVertices(const void *corners, void *vertices, s32 count);
struct PoolEntry *Render_AllocateModelList(s32 count);
void Render_DrawModelList(struct Model *);

#define PLTT ((void *)0x05000000)
#define BG_VRAM ((void *)0x06004000)
#define SPARKS sparks
#define RINGS ((struct Particle *)(Ram_MapCellBuffer + 0x6e00))
#define IWRAM_SLOT_68 (IWRAM->slot68)
#define IWRAM_SLOT_BC (IWRAM->slotbc)
// The second release in a once-only block, whose loop note sets kinds 68 and
// 84's scheduling; the other sites spell the releases out.
#define RELEASE_BLITTERS()                                                     \
  HeapSlot_Release(188);                                                       \
  do {                                                                         \
    HeapSlot_Release(104);                                                     \
  } while (0)

void Func_0815f16c(struct BattleEffectArgument *action, s32 kind) {
  u8 *buffer;
  struct Particle *sparks;
  struct SummonWork *work;
  void *screen;
  s32 frame;
  void *bg;
  void *tiles;
  s32 scrollA;
  s32 scrollB;
  s32 duration;
  s32 base[3];
  s32 target[3];
  s32 origin[3];
  s32 center[3];
  s32 pos[3];
  s32 point[3];
  s32 point2[3];
  struct DrawFuncs drawFuncs;
  struct Texture texture164;
  struct Texture texture156;
  struct Texture texture148;
  struct Texture texture140;
  struct Texture texture132;
  s32 step;
  s32 i;
  u8 *src;
  u8 *dst;
  s32 v;
  s32 *vec;
  /* FAKEMATCH(wrapper): every clear through CLEAR, an inline that calls the
   * fixed IWRAM clear; as direct Iwram_ClearWords calls, 211 of 7666
   * instructions differ in tla-en. */
  /* FAKEMATCH(wrapper): every fill through FILL, an inline that calls the fixed
   * IWRAM fill; as direct Iwram_FillWords calls, 76 of 7666 instructions differ
   * in tla-en. */

  buffer = Ram_MapCellBuffer;
  sparks = (struct Particle *)(Ram_MapCellBuffer + 0x6000);
  work = IWRAM->summon;
  screen = IWRAM->screen;
  bg = IWRAM->bg;
  tiles = IWRAM->tiles;
  scrollA = 0;
  scrollB = 0;

  if (kind == 41 || kind == 38 || kind == 62 || kind == 85 || kind == 86 ||
      kind == 87 || kind == 88)
    BattleEffect_StartTiledCanvas(0x2000);
  else
    BattleEffect_StartCanvasLayer(0);

  REG_BLDALPHA = 0x1010;
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, tiles, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_FireStreakSheet, work->frames, 1,
                             0);
  Resource_LoadAndDecompress((s32)&ResourceId_YellowOrbSheet, buffer, 1, 0);
  Graphics_PackTileRows__fxdisp(buffer, work->frames + 0x5100, 40, 0x120);

  if (kind == 5 || kind == 53 || kind == 78) {
    Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet, buffer, 1, 0);
  } else if (kind == 42 || kind == 15) {
    Resource_LoadAndDecompress((s32)&ResourceId_SkullSheet, buffer, 1, 0);
  } else if (kind == 16) {

    s32 j;
    s32 v;
    u8 *src;

    Resource_LoadAndDecompress((s32)&ResourceId_ImpFaceSheet, buffer, 1, 0);
    for (i = 1; i != 8; i++) {
      s32 fade;
      j = 0;
      fade = i * 8;
      src = buffer;
      for (; j != 0x400; j++) {
        v = *src++;
        if (v == 0) {
          buffer[j + i * 0x400] = 0;
          continue;
        }
        v -= fade;
        if (v <= 0)
          v = 1;
        buffer[j + i * 0x400] = v;
      }
    }
  } else if (kind == 10) {
    Resource_LoadAndDecompress((s32)&ResourceId_CloudPuffSheet, buffer, 1, 0);
  } else if (kind == 6 || kind == 57 || kind == 14 || kind == 17) {
    Resource_LoadAndDecompress((s32)&ResourceId_TornadoSheet, buffer, 1, 0);
    Resource_LoadAndDecompress((s32)&ResourceId_LightningBoltSheet,
                               buffer + 0xc56, 1, 0);
  } else if (kind == 68) {
    Resource_LoadAndDecompress((s32)&ResourceId_TornadoSheet, buffer, 1, 0);
    Resource_LoadAndDecompress((s32)&ResourceId_SmokeSheet, buffer + 0x1809, 1,
                               0);
  } else if (kind == 61) {
    Resource_LoadAndDecompress((s32)&ResourceId_RuneSheet, buffer, 1, 1);
  } else if (kind == 38) {
    Resource_LoadAndDecompress((s32)&ResourceId_RockSpireSheet, buffer, 1, 1);
  } else if (kind == 44) {
    Resource_LoadAndDecompress((s32)&ResourceId_BlueFlameSheet, buffer, 1, 0);
  } else if (kind == 60) {
    Resource_LoadAndDecompress((s32)&ResourceId_TornadoSheet, buffer, 1, 0);
  } else if (kind == 46) {
    Resource_LoadAndDecompress((s32)&ResourceId_IceChipSheet, buffer, 1, 0);
  } else if (kind == 50) {
    Resource_LoadAndDecompress((s32)&ResourceId_EmberStreakSheet, buffer, 1, 0);
  } else if (kind == 90) {
    Resource_LoadAndDecompress((s32)&ResourceId_Data0DA, buffer, 0, 0);
  } else if (kind == 91) {
    Resource_LoadAndDecompress((s32)&ResourceId_Data0C1, buffer + 0x4000, 0, 0);
  } else if (kind == 84) {
    s32 j;
    s32 k;

    Resource_LoadAndDecompress((s32)&ResourceId_TornadoSheet, buffer, 1, 0);
    Resource_LoadAndDecompress((s32)&ResourceId_LightningBoltSheet,
                               work->frames, 1, 1);
    Iwram_ClearWords(buffer + 0xc56, 0x4000);
    for (i = 0; i != 4; i++) {
      for (j = 0; j != 120; j++) {
        for (k = 0; k != 24; k++)
          buffer[0xc5a + i * 0x1000 + j * 32 + k] =
              work->frames[i * 2880 + j * 24 + k];
      }
    }
    Resource_LoadAndDecompress((s32)&ResourceId_FireStreakSheet, work->frames,
                               1, 0);
  } else if (kind == 63 || kind == 64) {
    Resource_LoadAndDecompress((s32)&ResourceId_FlameSheetA, buffer, 1, 0);
  } else if (kind == 71 || kind == 82) {
    Resource_LoadAndDecompress((s32)&ResourceId_Data0B4, buffer, 0, 0);
  } else if (kind == 96 || kind == 97 || kind == 98) {
    Resource_LoadAndDecompress((s32)&ResourceId_TwelveModeImage, buffer, 0, 0);
    Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet, buffer + 0xd80,
                               1, 0);
  } else if (kind == 80) {
    s32 k;
    s32 i;

    Resource_LoadAndDecompress((s32)&ResourceId_RuneSheet, buffer + 0x1000, 1,
                               0);
    Iwram_ClearWords(buffer, 0x1000);
    for (i = 0; i != 48; i++) {
      for (k = 0; k != 24; k++) {
        buffer[0x208 + i * 64 + k] = buffer[0x1000 + i * 24 + k];
        buffer[0x237 + i * 64 - k] = buffer[0x1000 + i * 24 + k];
      }
    }
  } else if (kind == 79) {
    s32 i;
    s32 k;

    Func_0815b434(buffer, 48, 0x10000);
    for (i = 0; i != 48; i++) {
      for (k = 0; k != 24; k++) {
        buffer[0x800 + i * 48 + k] = buffer[i * 24 + k];
        buffer[0x800 + i * 48 + 47 - k] = buffer[i * 24 + k];
        buffer[0x800 + (95 - i) * 48 + k] = buffer[i * 24 + k];
        buffer[0x800 + (95 - i) * 48 + 47 - k] = buffer[i * 24 + k];
      }
    }
  } else if (kind == 94) {
    Resource_LoadAndDecompress((s32)&ResourceId_BoulderSheet, buffer, 1, 1);
    Resource_LoadAndDecompress((s32)&ResourceId_BoulderPicture, buffer + 0x2000,
                               1, 1);
  } else if (kind == 73) {
    REG_BLDALPHA = 0xc10;
  } else if (kind == 70) {
    Resource_LoadAndDecompress((s32)&ResourceId_CrystalSheet, buffer, 1, 0);
  } else if (kind == 66) {
    Resource_LoadAndDecompress((s32)&ResourceId_FireballSheet, buffer, 1, 1);
  } else if (kind != 41 && kind != 62) {
    Resource_LoadAndDecompress((s32)&ResourceId_SmokeSheet, buffer, 1, 0);
  }

  switch (kind) {
  case 0:
  case 4:
  case 7:
  case 8:
  case 12:
  case 38:
  case 39:
  case 40:
  case 41:
  case 42:
  case 43:
  case 63:
  case 72:
  case 73:
  case 79:
  case 85:
  case 94:
  case 97:
  case 98:
    Dma_Set(Resource_GetTableEntry((s32)&ResourceId_BlastSheet), PLTT,
            0x84000020, REG_DMA3);
    if (action->prelude == 1)
      Resource_LoadAndDecompress((s32)&ResourceId_VenusDjinnSmallSheet,
                                 buffer + 0x3c56, 1, 0);
    break;
  case 2:
  case 9:
  case 10:
  case 11:
  case 13:
  case 44:
  case 45:
  case 46:
  case 47:
  case 48:
  case 49:
  case 68:
  case 69:
  case 70:
  case 71:
  case 80:
  case 82:
  case 99:
    if (action->prelude == 1)
      Resource_LoadAndDecompress((s32)&ResourceId_MercuryDjinnSmallSheet,
                                 buffer + 0x3c56, 1, 1);
    else
      Dma_Set(Resource_GetTableEntry((s32)&ResourceId_MercuryDjinnSmallSheet),
              PLTT, 0x84000020, REG_DMA3);
    break;
  case 3:
  case 5:
  case 14:
  case 15:
  case 50:
  case 51:
  case 52:
  case 53:
  case 54:
  case 55:
  case 64:
  case 65:
  case 66:
  case 67:
  case 74:
  case 78:
  case 81:
  case 90:
  case 91:
    if (action->prelude == 1)
      Resource_LoadAndDecompress((s32)&ResourceId_MarsDjinnSmallSheet,
                                 buffer + 0x3c56, 1, 1);
    else
      Dma_Set(Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSmallSheet),
              PLTT, 0x84000020, REG_DMA3);
    break;
  case 1:
  case 6:
  case 16:
  case 17:
  case 56:
  case 57:
  case 58:
  case 59:
  case 60:
  case 61:
  case 62:
  case 75:
  case 76:
  case 77:
  case 83:
  case 84:
  case 86:
  case 88:
  case 89:
  case 92:
    if (action->prelude == 1)
      Resource_LoadAndDecompress((s32)&ResourceId_JupiterDjinnSmallSheet,
                                 buffer + 0x3c56, 1, 1);
    else
      Dma_Set(Resource_GetTableEntry((s32)&ResourceId_JupiterDjinnSmallSheet),
              PLTT, 0x84000020, REG_DMA3);
    break;
  case 93:
  case 95:
  case 96:
    Dma_Set(Resource_GetTableEntry((s32)&ResourceId_DustPuffSheet), PLTT,
            0x84000020, REG_DMA3);
    break;
  case 100:
    Resource_LoadAndDecompress((s32)&ResourceId_MercuryDjinnSmallSheet,
                               buffer + 0x3c56, 1, 1);
    break;
  }

  if (kind == 81 || kind == 83 || kind == 91 || kind == 92 || kind == 93)
    Resource_LoadAndDecompress((s32)&ResourceId_Data0C2, buffer, 0, 0);
  work->unk7780 = 2;
  if (kind == 42)
    work->unk7784 = 75;
  else
    work->unk7784 = 50;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 3200);
  EffectPosition_ProjectScaledBattler(action->actors[0], target);
  EffectPosition_ProjectScaledBattler(action->actor, base);
  BattleEffect_FetchRectangleBlitters(action->side, &drawFuncs);
  {
    struct MotionObject *object;
    s32 save24;
    s32 save28;
    s32 save2c;
    s32 save34;
    s32 save48;
    struct MotionObject *object2;
    s32 n;
    s32 m;
    s32 t;
    s32 r;
    s32 x;
    s32 y;
    s32 h;
    s32 count;
    s32 start;
    s32 half;
    s32 height;
    s32 angle;
    s32 top;
    s32 scroll;
    s32 life;
    s32 row;
    s32 limit;
    s32 size;
    s32 fade;
    s32 scale;
    void *matrix;
    struct Model *model;
    const u8 *table;
    s32 k;
    struct Particle *p;

    work->unk77b4 = 24;
    work->unk77b8 = 0;
    object = GetBattleObjectSlotFar(action->actor)->object;
    for (i = 0; i != 64; i++) {
      work->particle[i].x = (Random16() & 63) + 32;
      work->particle[i].y = 0;
      work->particle[i].z = 0;
      work->particle[i].vx = Random16() & 0xffff;
      work->particle[i].vy = Random16() & 0xffff;
      work->particle[i].vz = Random16() & 0xffff;
    }
    Actor_SetAnimationSpeedFar(object, 0);
    /* FAKEMATCH(temporary): vec holds origin and then base while each is
     * filled; as origin[...] and base[...], 62 of 7666 instructions differ in
     * tla-en. */
    vec = origin;
    vec[0] = object->x;
    vec[1] = object->y + 0x500000;
    vec[2] = object->z;
    save24 = object->velocityX;
    save28 = object->velocityY;
    save2c = object->velocityZ;
    save34 = object->acceleration;
    save48 = object->verticalMotionStrength;
    object->velocityX = 0;
    object->velocityY = 0;
    object->velocityZ = 0;
    object->acceleration = 0;
    object->verticalMotionStrength = 0;
    vec = base;
    EffectPosition_ProjectScaledBattler(action->actor, vec);
    vec[0] = vec[0] / 2;
    Sound_PlayFar(212);

    for (frame = 0; frame != 32; frame++) {
      struct Particle *p;
      s32 n;
      count = 0;
      p = work->particle;
      for (i = 0; i != 64; i++, p++) {
        if (p->x >= 0) {
          if (frame >= i / 4) {
            n = 5;
            Render_ResetTransformState();
            SceneTransform_ApplyRoll(p->vz);
            SceneTransform_ApplyPitch(p->vx);
            SceneTransform_ApplyYaw(p->vy);
            EffectPosition_ProjectPoint(p, pos);
            pos[0] = pos[0] / 2 + base[0];
            if (kind <= 37)
              pos[1] = pos[1] + base[1] - 8;
            else if (kind == 65)
              pos[1] = pos[1] + base[1] + 44;
            else
              pos[1] = pos[1] + base[1] + 12;
            if (pos[2] < -60)
              pos[2] = -60;
            if (pos[2] > 60)
              pos[2] = 60;
            pos[2] += 60;
            drawFuncs.draw[1](screen,
                              (u8 *)tiles + gBattleEffect_MoteCells[n - 1],
                              pos[0] - n / 2, pos[1] - n, n, n * 2);
            p->x -= 4;
          }
          count++;
        }
      }
      if (kind <= 37 && count < 64) {
        Render_ResetTransformState();
        Transform_SetLookAt(bg, (u8 *)bg + 12);
        EffectPosition_ProjectPoint(origin, pos);
        pos[0] = pos[0] / 2;
        drawFuncs.draw[0](screen, buffer + 0x3c56, pos[0] - 10, pos[1] - 4, 20,
                          40);
        /* FAKEMATCH(barrier): an empty asm reading origin keeps it live past
         * the draw; without it, 287 of 7666 instructions differ in tla-en. */
        __asm__("" : : "X"(origin));
      }
      work->unk78e8 = 1;
      wait(1);
    }

    if (kind == 41) {
      REG_BG2PA = 0x100;
      if (action->side == 0)
        REG_BG2X = (frame - target[0]) << 8;
      else
        REG_BG2X = (96 - target[0]) << 8;
    } else if (kind == 85) {
      REG_BG2PA = 0x100;
      if (action->side == 0)
        REG_BG2X = (64 - target[0]) << 8;
      else
        REG_BG2X = (64 - target[0]) << 8;
    } else if (kind == 88) {
      REG_BG2PA = 0x100;
      if (action->side == 0) {
        scrollA = 0xffc00000;
        scrollB = 0x20000;
      } else {
        scrollA = 0xffc80000;
        scrollB = 0xfffe0000;
      }
      REG_BG2X = (scrollA >> 16) << 8;
    } else if (kind == 62 || kind == 86) {
      REG_BG2PA = 0x100;
      if (action->side == 0) {
        scrollA = 0xff800000;
        scrollB = 0xc0000;
      } else {
        scrollA = 0;
        scrollB = 0xfff40000;
      }
      REG_BG2X = (scrollA >> 16) << 8;
    }
    if (kind == 38) {
      REG_BG2PA = 0x100;
      REG_BG2X = (64 - target[0]) << 8;
      work->unk7780 = 1;
      work->unk7784 = 0;
      Iwram_ClearWords(BG_VRAM, 0x3c00);
      Iwram_ClearWords(screen, 0x3c00);
      REG_BLDCNT = 0;
    }
    if (kind == 61) {
      REG_BG2PA = 0x100;
      if (action->side == 0)
        REG_BG2X = (32 - target[0]) << 8;
      else
        REG_BG2X = (96 - target[0]) << 8;
    }
    if (kind == 45 || kind == 47 || kind == 54 || kind == 56) {
      Iwram_ClearWords(BG_VRAM, 0x3c00);
      Iwram_ClearWords(screen, 0x3c00);
      action->prelude = 0;
      Task_Remove(Palette_StepFadeTransfer);
      Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
      HeapSlot_Release(188);
      HeapSlot_Release(104);
      Actor_SetMode(object, 3);
      if (kind == 45)
        BattleEffect_RunProjectileVolley(action, 9);
      if (kind == 54)
        BattleEffect_RunMysticFlame(action);
      if (kind == 56)
        BattleEffect_RunProjectileVolley(action, 8);
      return;
    }
    Actor_SetAnimationSpeedFar(object, 16);
    object->velocityX = save24;
    object->velocityY = save28;
    object->velocityZ = save2c;
    object->acceleration = save34;
    object->verticalMotionStrength = save48;
    if (kind == 65) {
      Iwram_ClearWords(BG_VRAM, 0x3c00);
      Iwram_ClearWords(screen, 0x3c00);
      action->prelude = 0;
      Task_Remove(Palette_StepFadeTransfer);
      Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
      HeapSlot_Release(188);
      HeapSlot_Release(104);
      action->variant = 3;
      BattleEffect_RunSparkGroups(action, 2);
      return;
    }
    Iwram_ClearWords(screen, 0x3c00);
    Iwram_ClearWords(BG_VRAM, 0x3c00);
    if (kind == 79) {
      for (i = 1; i != 64; i++) {
        v = 31 - ((Trig_Cos(i << 8) << 5) >> 16);
        BG_PLTT[i] = (v << 10) | (v << 5) | v;
      }
    }
    object2 = GetBattleObjectSlotFar(action->actors[0])->object;
    {
      center[0] = (object2->x - origin[0]) / 6;
      center[1] = (object2->y - origin[1] + 0x1e0000) / 6;
      center[2] = (object2->z - origin[2]) / 6;
    }
    for (i = 0; i != 64; i++)
      work->particle[i].life = 0;
    if (kind != 44) {
      half = BattleUnit_GetEffectHeightFar(action->actors[0]) / 2;
      i = 0;
      p = work->particle;
      for (; i != 32; i++, p++) {
        p->x = object2->x;
        p->y = half;
        p->z = object2->z;
        if (kind == 61) {
          p->vx = ((Random16() & 255) - 127) << 12;
          p->vy = ((Random16() & 255) - 64) << 10;
          p->vz = ((Random16() & 255) - 127) << 12;
        } else {
          p->vx = ((Random16() & 255) - 127) << 12;
          p->vy = ((Random16() & 255) - 64) << 12;
          p->vz = ((Random16() & 255) - 127) << 12;
        }
        p->vx = p->vx / 2;
        p->vy = p->vy / 2;
        p->vz = p->vz / 2;
        p->life = i / 2 + 32;
      }
    }
    if (kind == 41) {
      Resource_LoadAndDecompress((s32)&ResourceId_CounterRevealSheet,
                                 work->frames, 1, 1);
      Resource_LoadAndDecompress((s32)&ResourceId_RedCrescentSheetA, buffer, 1,
                                 0);
      REG_BLDALPHA = 0xe10;
    }
    if (kind == 86) {
      Resource_LoadAndDecompress((s32)&ResourceId_Data0F1, buffer, 1, 1);
      REG_BLDALPHA = 0x810;
    }
    if (kind == 88) {
      Resource_LoadAndDecompress((s32)&ResourceId_Data0DD, buffer, 1, 1);
      REG_BLDALPHA = 0x810;
    }
    if (kind == 62) {
      Resource_LoadAndDecompress((s32)&ResourceId_BlueBeastSheet, work->frames,
                                 1, 1);
      Resource_LoadAndDecompress((s32)&ResourceId_RedCrescentSheetB, buffer, 1,
                                 0);
      REG_BLDALPHA = 0xe10;
    }
    if (kind == 85) {
      Resource_LoadAndDecompress((s32)&ResourceId_Data0F0, buffer, 1, 1);
      REG_BLDALPHA = 0xe10;
    }

    if (kind == 7 || kind == 43 || kind == 48 || kind == 73 || kind == 77 ||
        kind == 78 || kind == 41 || kind == 62 || kind == 49 || kind == 12 ||
        kind == 83 || kind == 84 || kind == 85 || kind == 86 || kind == 88) {
    } else if (kind == 66) {
      for (i = 0; i != 64; i++) {
        struct Particle *q;
        q = &SPARKS[i];
        q->x = object2->x;
        q->y = 0x140000;
        q->z = object2->z;
        if (object2->x < 0)
          q->vx = -((Random16() & 255) + 128) << 11;
        else
          q->vx = ((Random16() & 255) + 128) << 11;
        q->vy = ((Random16() & 255) - 127) << 10;
        q->vz = 0;
        q->life = 0;
      }
    } else {
      struct Particle *p;
      if (kind == 42 || kind == 15)
        height = 0;
      else
        height = 0x140000;
      p = SPARKS;
      for (i = 0; i != 64; i++, p++) {
        p->x = object2->x;
        p->y = height;
        p->z = object2->z;
        if (kind == 5 || kind == 53) {
          p->vx = ((Random16() & 255) - 127) << 11;
          p->vy = (Random16() & 255) << 11;
          p->vz = ((Random16() & 255) - 127) << 11;
        } else if (kind == 68 || kind == 69 || kind == 74 || kind == 75 ||
                   kind == 16 || kind == 97 || kind == 10) {
          p->vx = ((Random16() & 255) - 127) << 13;
          p->vy = ((Random16() & 255) - 127) << 12;
          p->vz = ((Random16() & 255) - 127) << 13;
        } else if (kind == 98) {
          p->vx = ((Random16() & 255) - 127) << 14;
          p->vy = ((Random16() & 255) - 127) << 13;
          p->vz = ((Random16() & 255) - 127) << 14;
        } else if (kind == 55) {
          p->vx = ((Random16() & 255) - 127) << 11;
          p->vy = (Random16() & 127) << 10;
          p->vz = ((Random16() & 255) - 127) << 11;
        } else {
          p->vx = ((Random16() & 255) - 127) << 10;
          p->vy = (Random16() & 127) << 10;
          p->vz = ((Random16() & 255) - 127) << 10;
        }
        p->vx = p->vx / 4;
        p->vy = p->vy / 4;
        p->vz = p->vz / 4;
        p->life = 0;
      }
    }

    if (kind == 2 || kind == 3 || kind == 42 || kind == 15 || kind == 52 ||
        kind == 72 || kind == 59 || kind == 58 || kind == 69 || kind == 89)
      Task_Add(BattleEffect_ArmBg2AffineHBlankDma, 3200);

    if (kind == 4 || kind == 5 || kind == 6 || kind == 17 || kind == 53 ||
        kind == 60 || kind == 57 || kind == 14 || kind == 63 || kind == 64 ||
        kind == 100 || kind == 83 || kind == 90 || kind == 91 || kind == 76 ||
        kind == 66 || kind == 92 || kind == 93 || kind == 96 || kind == 97)
      duration = 32;
    else if (kind == 0 || kind == 1 || kind == 2 || kind == 3 || kind == 13 ||
             kind == 38 || kind == 39 || kind == 74 || kind == 75 ||
             kind == 40 || kind == 52 || kind == 72 || kind == 69 ||
             kind == 55 || kind == 59 || kind == 61 || kind == 44 ||
             kind == 70 || kind == 86 || kind == 88 || kind == 89 ||
             kind == 16 || kind == 10)
      duration = 48;
    else if (kind == 51 || kind == 67 || kind == 9 || kind == 11 ||
             kind == 95 || kind == 99)
      duration = 20;
    else if (kind == 41 || kind == 62 || kind == 50 || kind == 81 ||
             kind == 84 || kind == 91 || kind == 79 || kind == 98)
      duration = 40;
    else if (kind == 85)
      duration = 72;
    else if (kind == 58 || kind == 42 || kind == 15 || kind == 68)
      duration = 64;
    else if (kind == 8 || kind == 71 || kind == 80 || kind == 94)
      duration = 54;
    else
      duration = 80;

    for (frame = 0; frame != duration; frame++) {
      struct Particle *p;
      struct Particle *particle;
      if (kind != 41 && kind != 62 && kind != 83 && kind != 85 && kind != 86 &&
          kind != 88) {
        s32 *w;
        w = work->wave;
        for (i = 0; i != 160; i++)
          *w++ = (0x40000 - (Trig_Sin((frame << 12) + (i << 11)) << 2)) >> 10;
      }
      switch (kind) {
      case 10:
        BattleEffect_StepPaletteToResource((s32)&ResourceId_CloudPuffSheet);
        break;
      case 66:
        BattleEffect_StepPaletteToResource((s32)&ResourceId_FireballSheet);
        break;
      case 69:
        BattleEffect_StepPaletteToResource((s32)&ResourceId_EffectPalette01);
        break;
      case 63:
        BattleEffect_StepPaletteToResource((s32)&ResourceId_FlameSheetA);
        break;
      case 44:
      case 68:
        BattleEffect_StepPaletteToResource((s32)&ResourceId_BlueFlameSheet);
        break;
      case 61:
        BattleEffect_StepPaletteToResource((s32)&ResourceId_RuneSheet);
        break;
      case 38:
        BattleEffect_StepPaletteToResource((s32)&ResourceId_RockSpireSheet);
        break;
      case 0:
      case 40:
      case 74:
        BattleEffect_StepPaletteToResource((s32)&ResourceId_MarsDjinnSheet);
        break;
      case 42:
      case 43:
      case 55:
        BattleEffect_StepPaletteToResource((s32)&ResourceId_PinkBurstSheet);
        break;
      case 48:
      case 77:
        BattleEffect_StepPaletteToResource((s32)&ResourceId_IceBlockSheet);
        break;
      case 49:
        BattleEffect_StepPaletteToResource((s32)&ResourceId_BlastSheet);
        break;
      case 2:
      case 59:
      case 75:
      case 82:
      case 91:
        BattleEffect_StepPaletteToResource((s32)&ResourceId_TanPalette);
        break;
      case 1:
      case 13:
      case 58:
      case 89:
        BattleEffect_StepPaletteToResource((s32)&ResourceId_CyanPalette);
        break;
      case 3:
      case 50:
      case 52:
      case 78:
        BattleEffect_StepPaletteToResource((s32)&ResourceId_EmberStreakSheet);
        break;
      case 39:
      case 83:
        BattleEffect_StepPaletteToResource((s32)&ResourceId_LimePalette);
        break;
      case 5:
      case 53:
        BattleEffect_StepPaletteToResource((s32)&ResourceId_FlashBurstSheet);
        break;
      case 81:
      case 90:
      case 98:
        BattleEffect_StepPaletteToResource((s32)&ResourceId_OrangePaletteB);
        break;
      case 73:
        BattleEffect_StepPaletteToResource((s32)&ResourceId_RedPaletteB);
        break;
      case 84:
        BattleEffect_StepPaletteToResource((s32)&ResourceId_EffectPalette05);
        break;
      }
      if (frame < 6) {
        EffectPosition_ProjectScaledBattler(action->actor, base);
        base[0] = base[0] / 2;
        base[1] += 8;
      }
      if (kind != 41 && kind != 38 && kind != 62 && kind != 63 && kind != 64 &&
          kind != 79 && kind != 70 && kind != 82 && kind != 85 && kind != 86 &&
          kind != 88 && kind != 89 && kind != 94 && kind != 96 && frame < 12) {
        if (action->side == 0)
          drawFuncs.draw[0](screen, work->frames + (frame / 2) * 0xd80,
                            base[0] - 28, base[1] - 40, 48, 72);
        else
          drawFuncs.draw[0](screen, work->frames + (frame / 2) * 0xd80,
                            base[0] - 20, base[1] - 40, 48, 72);
      }
      if (kind != 41 && kind != 38 && kind != 62 && kind != 83 && kind != 85 &&
          kind != 86 && kind != 88 && kind != 94) {
        if (frame >= 4 && frame < 16)
          drawFuncs.draw[1](screen,
                            work->frames + 0x5100 + ((frame - 4) / 2) * 0x3c0,
                            target[0] / 2 - 8, base[1] - 24, 20, 48);
        Render_ResetTransformState();
        Transform_SetLookAt(bg, (u8 *)bg + 12);
        if (frame > 3) {
          if (kind == 79) {
            struct Particle *r;
            for (i = 0; i != 64; i++) {
              r = &work->particle[i];
              if (r->life > 0) {
                s32 n;
                n = (r->life >> 2) + 1;
                if (n > 10)
                  n = 10;
                EffectPosition_ProjectPoint(r, point);
                point[0] = point[0] / 2;
                drawFuncs.draw[0](screen,
                                  (u8 *)tiles + gBattleEffect_MoteCells[n - 1],
                                  point[0] - n / 2, point[1] - n, n, n * 2);
                // DEAD: this arm runs only for kind 79, so the kind 89 test
                // never holds; the source keeps it.
                {
                  s32 gravity;
                  if (kind == 89)
                    gravity = 0;
                  else
                    gravity = -0x800;
                  BattleEffectKernels_IntegrateVector3(r, 62, gravity);
                }
                r->life -= 2;
              }
            }
          } else {
            for (i = 0; i != 64; i++) {
              p = &work->particle[i / 2];
              if (p->life > 0) {
                s32 n;
                n = p->life / 20 + 2;
                EffectPosition_ProjectPoint(p, point);
                point[0] = point[0] / 2;
                drawFuncs.draw[(i / 2) & 1](
                    screen, (u8 *)tiles + gBattleEffect_MoteCells[n - 1],
                    point[0] - n / 2, point[1] - n, n, n * 2);
                BattleEffectKernels_IntegrateVector3(p, 62, -0x800);
                p->life--;
              }
            }
          }
        }
      }

      if (kind == 7 || kind == 43 || kind == 48 || kind == 73 || kind == 77 ||
          kind == 78 || kind == 49 || kind == 12 || kind == 82) {
        s32 start;
        struct MotionObject *object3;
        s32 half;
        s32 dx;
        s32 dy;
        s32 dz;
        s32 slot;

        start = 12;
        if (frame == 12)
          Sound_PlayFar(142);
        if (kind == 78)
          start = 0;
        if (frame == 50)
          BattleEffect_TintBattler(action->actor, 7, -1, -1, 0);
        if (frame == 79)
          BattleEffect_TintBattler(action->actor, 0, -1, -1, 0);
        if (frame == start) {
          struct Particle *p;
          p = SPARKS;
          for (i = 0; i != 64; i++, p++) {
            p->x = object2->x;
            p->y = 0x140000;
            p->z = object2->z;
            p->vx = ((Random16() & 255) - 128) << 10;
            p->vy = ((Random16() & 255) - 128) << 10;
            p->vz = ((Random16() & 255) - 128) << 10;
            if (kind == 78) {
              p->vx <<= 1;
              p->vy <<= 1;
              p->vz <<= 1;
            }
            p->life = 0;
          }
        }
        if (frame >= start) {
          p = SPARKS;
          object3 = GetBattleObjectSlotFar(action->actor)->object;
          half = BattleUnit_GetEffectHeightFar(action->actor) / 2;
          slot = kind == 78;
          for (i = 0; i != 32; i++, p++) {
            if (p->life >= 0) {
              n = (i & 1) + 6;
              EffectPosition_ProjectPoint(p, point);
              point[0] = point[0] >> 1;
              drawFuncs.draw[slot](screen,
                                   (u8 *)tiles + gBattleEffect_MoteCells[n - 1],
                                   point[0] - n / 2, point[1] - n, n, n * 2);
              BattleEffectKernels_IntegrateVector3(p, 62, 0);
              if (frame > i + 22) {
                dx = (object3->x - p->x) >> 8;
                dy = (object3->y + half - p->y) >> 8;
                dz = (object3->z - p->z) >> 8;
                p->vx += dx;
                p->vy += dy;
                p->vz += dz;
                if ((dx > -0x1000 && dx < 0x1000) &&
                    (dz > -0x1000 && dz < 0x1000))
                  p->life = -1;
              }
            }
          }
        }
      }

      if (kind == 7 || kind == 43 || kind == 48 || kind == 73 || kind == 77 ||
          kind == 49 || kind == 12) {
      } else if (kind == 66) {
        struct Particle *p;
        p = SPARKS;
        for (i = 0; i != 16; i++, p++) {
          if (frame >= i + 4 && p->life <= 23) {
            EffectPosition_ProjectPoint(p, point);
            point[0] = point[0] / 2;
            drawFuncs.draw[0](screen, buffer, point[0] - 20, point[1] - 32, 40,
                              64);
            BattleEffectKernels_IntegrateVector3(p, 64, 0);
            p->life++;
          }
        }
      } else if (kind == 51 || kind == 67 || kind == 9 || kind == 95 ||
                 kind == 99) {
      } else if (kind == 6 || kind == 57 || kind == 14 || kind == 17) {
        s32 y;
        if (kind == 17) {
          if (frame == 4)
            work->unk77a8 = 12;
          if (frame == 8)
            Sound_PlayFar(221);
        }
        if (kind == 6 || kind == 57) {
          if (frame >= 6 && frame < 20) {
            for (i = 0; i != 2; i++)
              drawFuncs.draw[0](
                  screen, buffer + 0xc56 + (((frame + i * 3) / 2) & 3) * 2880,
                  target[0] / 2 - 8, 0, 24, 104);
          }
        } else if (kind == 17 && (frame >= 6 && frame < 20)) {
          s32 x;
          for (i = 0; i != 4; i++) {
            s32 m;
            angle = frame * 1848 + i * 0x4000;
            m = ((frame + i * 3) / 2) & 3;
            x = target[0] / 2 + ((Trig_Sin(angle) << 5) >> 16);
            // DEAD: the cosine is computed and dropped; the source keeps the
            // call.
            Trig_Cos(angle);
            drawFuncs.draw[0](screen, buffer + 0xc56 + m * 2880, x - 8, 0, 24,
                              104);
          }
        }
        if (frame >= 8 && frame < 24) {
          s32 angle;
          s32 off;
          s32 amp;

          if (kind == 14) {
            amp = 12;
            off = 24;
          } else {
            off = 0;
            amp = 32;
          }
          for (i = 0; i != 3; i++) {
            s32 m;
            s32 x;
            m = i & 3;
            angle = Random16() & 0xffff;
            x = ((Trig_Sin(angle) << 3) >> 16) + target[0] / 2 -
                gData_08197492[m] / 2;
            y = ((Trig_Cos(angle) * amp) >> 16) + off - gData_08197498[m] / 2;
            HeapSlot_Release(188);
            HeapSlot_Release(104);
            // Once-only block: its loop notes keep sched2 from moving the
            // releases' argument loads, as the listing has them.
            do {
              Func_08196404(188, 7, 7, gData_0819887e[Random16() & 3] | 3, 0);
            } while (0);
            IWRAM_SLOT_BC(screen, buffer + gBattleEffect_GlintCells[m], x,
                          y + 56, gData_08197492[m], gData_08197498[m]);
            HeapSlot_Release(188);
            BattleEffect_FetchRectangleBlitters(action->side, &drawFuncs);
          }
        }
      } else if (kind == 68) {
        s32 angle;
        if (frame >= 8 && frame < 24) {
          s32 y;
          for (i = 0; i != 3; i++) {
            s32 m;
            s32 x;
            m = i & 3;
            angle = Random16() & 0xffff;
            x = ((Trig_Sin(angle) << 4) >> 16) + target[0] / 2 -
                gData_08197492[m] / 2;
            y = ((Trig_Cos(angle) << 4) >> 16) - gData_08197498[m] / 2;
            RELEASE_BLITTERS();
            Func_08196404(188, 7, 7, gData_08198882[Random16() & 3] | 3, 1);
            IWRAM_SLOT_BC(screen, buffer + gBattleEffect_GlintCells[m], x,
                          y + 80, gData_08197492[m], gData_08197498[m]);
            HeapSlot_Release(188);
            BattleEffect_FetchRectangleBlitters(action->side, &drawFuncs);
          }
        }
        p = SPARKS;
        for (i = 0; i != 16; i++, p++) {
          if (frame >= i + 4) {
            s32 life;
            life = p->life;
            if (life <= 23) {
              s32 m;
              m = life / 4;
              EffectPosition_ProjectPoint(p, point);
              point[0] = point[0] / 2;
              drawFuncs.draw[1](screen, buffer + 0x1809 + m * 0x480,
                                point[0] - 12, point[1] - 24, 24, 48);
              BattleEffectKernels_IntegrateVector3(p, 60, 0x1000);
              p->life++;
            }
          }
        }
        if (frame == 6)
          Sound_PlayFar(221);
      } else if (kind == 44) {
        s32 count;
        HeapSlot_Release(188);
        HeapSlot_Release(104);
        if (frame >= 0 && frame < 24) {
          s32 x;
          x = target[0] / 2;
          top = frame * 32 - 232;
          scroll = frame * 16 - 48;
          if (top > 0)
            top = 0;
          while (scroll > 104)
            scroll -= 104;
          BattleEffect_LoadBlitter(188, 19);
          IWRAM_SLOT_BC(screen, buffer, x - 8, top + scroll - 104, 17, 104);
          IWRAM_SLOT_BC(screen, buffer, x - 8, top + scroll, 17, 104 - scroll);
          IWRAM_SLOT_BC(screen, buffer + 0x6e8, x - 17, top + 47, 34, 65);
          HeapSlot_Release(188);
          if (frame == 8)
            work->unk77a8 = 8;
          if (frame > 1) {
            struct Particle *p;
            i = 0;
            p = work->particle;
            count = 0;
            for (; i != 64; i++, p++) {
              if (p->life == 0) {
                count++;
                p->x = object2->x;
                p->y = 0x140000;
                p->z = object2->z;
                p->vx = ((Random16() & 255) - 127) << 12;
                p->vy = ((Random16() & 255) - 64) << 10;
                p->vz = ((Random16() & 255) - 127) << 12;
                p->life = i / 2 + 32;
                if (count == 4)
                  break;
              }
            }
          }
        }
        BattleEffect_FetchRectangleBlitters(action->side, &drawFuncs);
      } else if (kind == 61) {
        s32 x;
        HeapSlot_Release(188);
        HeapSlot_Release(104);
        if (frame >= 4 && frame < 24) {
          s32 h;
          x = target[0] / 2;
          h = 48;
          BattleEffect_LoadBlitter(188, 19);
          IWRAM_SLOT_BC(screen, buffer, x - 24, h, 24, h);
          HeapSlot_Release(188);
          BattleEffect_LoadBlitter(188, 23);
          IWRAM_SLOT_BC(screen, buffer, x, h, 24, h);
          HeapSlot_Release(188);
        }
        BattleEffect_FetchRectangleBlitters(action->side, &drawFuncs);
      } else if (kind == 60) {
        s32 m;
        if (frame > 15)
          REG_BLDALPHA = (32 - frame) | 0x1000;
        if (frame > 5) {
          s32 x;
          s32 o1;
          x = target[0] / 2;
          m = frame / 2 % 3;
          x -= 20;
          o1 = m * 0xa00;
          drawFuncs.draw[0](screen, buffer + 0xc56 + o1, x, 16, 40, 32);
          drawFuncs.draw[0](screen, buffer + 0x2a56 + m * 0x500, x, 48, 40, 32);
          {
            u8 *o0;
            o0 = buffer + 0x1156 + o1;
            drawFuncs.draw[0](screen, o0, x, 80, 40, 32);
          }
        }
      } else if (kind == 5 || kind == 53 || kind == 78) {
        if (frame < 24) {
          p = SPARKS;
          for (i = 0; i != 16; i++, p++) {
            if (frame >= i / 2 + 4) {
              s32 life;
              life = p->life;
              if (life <= 11) {
                s32 half;
                half = life / 2;
                EffectPosition_ProjectPoint(p, point);
                point[0] = point[0] / 2;
                drawFuncs.draw[0](screen, buffer + half * 0x800, point[0] - 16,
                                  point[1] - 32, 32, 64);
                BattleEffectKernels_IntegrateVector3(p, 60, 0x1000);
                p->life++;
              }
            }
          }
        }
      } else if (kind == 4) {
      } else if (kind == 8) {
      } else if (kind == 41) {
        y = (target[1] >> 16) + ((Trig_Cos(frame << 9) << 2) >> 16) + 16;
        if (frame < 4) {
          drawFuncs.draw[0](screen, work->frames,
                            gData_0819750c[action->side * 7],
                            y + gData_0819751a[0], 57, 98);
        } else {
          if (frame < 8)
            drawFuncs.draw[0](screen, work->frames,
                              gData_0819750c[action->side * 7],
                              y + gData_0819751a[0], 57, 98);
          drawFuncs.draw[0](screen, work->frames + 0x15d2,
                            gData_0819750c[action->side * 7 + 1],
                            y + gData_0819751a[1], 99, 69);
          if (frame >= 4 && frame < 6)
            Iwram_FillWords(screen, 0x4000, 0x3f3f3f3f);
          if (frame >= 6 && frame < 8)
            drawFuncs.draw[0](screen, work->frames + 0x3081,
                              gData_0819750c[action->side * 7 + 2],
                              y + gData_0819751a[2], 128, 91);
          if (frame >= 8 && frame < 10)
            drawFuncs.draw[0](screen, buffer,
                              gData_0819750c[action->side * 7 + 3],
                              y + gData_0819751a[3], 128, 91);
          if (frame >= 10 && frame < 12)
            drawFuncs.draw[0](screen, buffer + 0x2d80,
                              gData_0819750c[action->side * 7 + 4],
                              y + gData_0819751a[4], 128, 59);
          if (frame >= 12 && frame < 14)
            drawFuncs.draw[0](screen, buffer + 0x4b00,
                              gData_0819750c[action->side * 7 + 5],
                              y + gData_0819751a[5], 122, 29);
          if (frame >= 14 && frame < 16)
            drawFuncs.draw[0](screen, buffer + 0x58d2,
                              gData_0819750c[action->side * 7 + 6],
                              y + gData_0819751a[6], 76, 25);
        }
      } else if (kind == 85) {
        s32 h;
        s32 y;
        REG_BLDCNT = 0;
        work->unk7780 = 1;
        work->unk7784 = 0;
        HeapSlot_Release(188);
        HeapSlot_Release(104);
        BattleEffect_LoadBlitter(104, 35);
        drawFuncs.draw[0] = IWRAM_SLOT_68;
        if (frame == 0) {
          for (i = 0; i != 1104; i++)
            work->frames[i] = Random16() & 31;
        }
        y = frame * 28 - 96;
        if (y > 16)
          y = 16;
        {
          /* FAKEMATCH(pin): hold, an input of the empty asm below, keeps r2 for
           * h; as a plain local, 1602 of 7666 instructions differ in tla-en. */
          register s32 hold __asm__("r2");
          h = 96;
          /* FAKEMATCH(barrier): an empty asm through h, with hold as an
           * input, keeps r2 held until h is set; as h = 96 alone, 16 of 7666
           * instructions differ in tla-en. */
          __asm__("" : "=r"(h) : "0"(h), "r"(hold));
        }
        if (frame > 7) {
          s32 t;
          i = 0;
          t = 0;
          row = 112 - frame * 2;
          for (; i != 34; i++, t++, row++) {
            if (row >= 0 && row < 96) {
              s32 k;
              s32 threshold;
              for (k = 0, threshold = t; k != 69; k++) {
                /* FAKEMATCH(barrier): an empty asm through threshold keeps it a
                 * copy of t in a register of its own; without it, 48 of 7666
                 * instructions differ in tla-en. */
                __asm__("" : "=r"(threshold) : "0"(threshold));
                if (work->frames[(row & 15) * 69 + k] < threshold)
                  buffer[row * 69 + k] = 0;
              }
            }
          }
        }
        drawFuncs.draw[0](screen, buffer, 29, y, 69, h);
        if (frame > 5) {
          switch ((frame - 6) / 4) {
          case 0:
            drawFuncs.draw[0](screen, buffer + 0x19e0, 32, 92, 59, 24);
            break;
          case 1:
            drawFuncs.draw[0](screen, buffer + 0x1f68, 30, 87, 70, 27);
            break;
          case 2:
            drawFuncs.draw[0](screen, buffer + 0x26ca, 23, 57, 84, 55);
            break;
          case 3:
            drawFuncs.draw[0](screen, buffer + 0x38d6, 27, 55, 75, 57);
            break;
          case 4:
            drawFuncs.draw[0](screen, buffer + 0x4989, 28, 19, 67, 86);
            break;
          case 5:
            drawFuncs.draw[0](screen, buffer + 0x600b, 33, 22, 50, 65);
            break;
          case 6:
            drawFuncs.draw[0](screen, buffer + 0x6cbd, 41, 21, 48, 29);
            break;
          }
        }
        HeapSlot_Release(104);
        BattleEffect_FetchRectangleBlitters(action->side, &drawFuncs);
      } else if (kind == 96 || kind == 97 || kind == 98) {
        if (frame < 20) {
          s32 m;
          if (frame < 8)
            m = gData_08198886[frame / 2];
          else
            m = 3;
          HeapSlot_Release(188);
          // Once-only block: it ends CSE's block before the second draw reads
          // target again, with no scheduling barrier inside the first draw.
          do {
            /* FAKEMATCH(pin): pinned, set to 56 among the first draw's
             * arguments, takes r3 while they are evaluated; as a plain 56, 2 of
             * 7666 instructions differ in tla-en. */
            register s32 pinned __asm__("r3");
            HeapSlot_Release(104);
            BattleEffect_LoadBlitter(104, 3);
            IWRAM_SLOT_68(screen, buffer + m * 0x360, target[0] / 2 - 18,
                          pinned = 56, 18, 48);
            HeapSlot_Release(104);
            BattleEffect_LoadBlitter(104, 7);
          } while (0);
          IWRAM_SLOT_68(screen, buffer + m * 0x360, target[0] / 2, 56, 18, 48);
          HeapSlot_Release(104);
          BattleEffect_FetchRectangleBlitters(action->side, &drawFuncs);
        }
        if ((kind == 97 || kind == 98) && frame < 24) {
          if (kind == 97)
            limit = 16;
          else
            limit = 24;
          for (i = 0; i != limit; i++) {
            struct Particle *p;
            p = &SPARKS[i];
            if (frame >= i / 2 + 4) {
              s32 life;
              life = p->life;
              if (life <= 9) {
                s32 half;
                half = life / 2;
                EffectPosition_ProjectPoint(p, point);
                point[0] = point[0] / 2;
                drawFuncs.draw[0](screen, buffer + 0xd80 + half * 0x800,
                                  point[0] - 16, point[1] - 32, 32, 64);
                BattleEffectKernels_IntegrateVector3(p, 64, 0x1000);
                p->life++;
              }
            }
          }
        }
      } else if (kind == 86) {
        s32 m;
        work->unk7780 = 1;
        work->unk7784 = 0;
        HeapSlot_Release(188);
        HeapSlot_Release(104);
        if (action->side == 0)
          BattleEffect_LoadBlitter(104, 35);
        else
          BattleEffect_LoadBlitter(104, 39);
        drawFuncs.draw[0] = IWRAM_SLOT_68;
        scrollA += scrollB;
        if (frame > 6)
          scrollB = scrollB * 48 / 64;
        REG_BG2X = (scrollA >> 16) << 8;
        t = frame - 28;
        if (t >= 0 && t < 16)
          REG_BLDALPHA = ((t / 2 + 8) << 8) | (16 - t);
        if (frame >= 4 && frame < 8)
          Iwram_FillWords(screen, 0x4000, 0x01010101);
        if (frame > 3) {
          if (action->side == 0)
            drawFuncs.draw[0](screen, buffer, 32, 21, 96, 88);
          else
            drawFuncs.draw[0](screen, buffer, 0, 21, 96, 88);
        }
        m = frame / 2;
        if (m < 10) {
          if (action->side == 0)
            drawFuncs.draw[0](screen, buffer + gData_0819888a[m],
                              gData_081988c6[m], gData_081988da[m],
                              gData_0819889e[m], gData_081988b2[m]);
          else
            drawFuncs.draw[0](screen, buffer + gData_0819888a[m],
                              128 - gData_081988c6[m] - gData_0819889e[m],
                              gData_081988da[m], gData_0819889e[m],
                              gData_081988b2[m]);
        }
        HeapSlot_Release(104);
        BattleEffect_FetchRectangleBlitters(action->side, &drawFuncs);
      } else if (kind == 88) {
        s32 m;
        work->unk7780 = 1;
        work->unk7784 = 0;
        if (frame == 7) {
          work->unk77a8 = 16;
          Sound_PlayFar(134);
        }
        if (frame == 11)
          Sound_PlayFar(134);
        if (frame == 15)
          Sound_PlayFar(134);
        HeapSlot_Release(188);
        HeapSlot_Release(104);
        if (action->side == 0)
          BattleEffect_LoadBlitter(104, 35);
        else
          BattleEffect_LoadBlitter(104, 39);
        drawFuncs.draw[0] = IWRAM_SLOT_68;
        scrollA += scrollB;
        if (frame == 5)
          scrollA += 0x200000;
        if (frame > 6)
          scrollB = scrollB * 48 / 64;
        REG_BG2X = (scrollA >> 16) << 8;
        t = frame - 29;
        if (t >= 0 && t < 16)
          REG_BLDALPHA = ((t / 2 + 8) << 8) | (16 - t);
        if (frame > 4)
          drawFuncs.draw[0](screen, buffer, 35, 17, 60, 95);
        if (frame == 5)
          Iwram_FillWords(screen, 0x4000, 0x3f3f3f3f);
        if (frame >= 0 && (m = frame / 3) < 7) {
          if (action->side == 0)
            drawFuncs.draw[0](screen, buffer + gData_081988ee[m],
                              gData_08198918[m], gData_08198926[m] - 16,
                              gData_081988fc[m], gData_0819890a[m]);
          else
            drawFuncs.draw[0](screen, buffer + gData_081988ee[m],
                              128 - gData_08198918[m] - gData_081988fc[m],
                              gData_08198926[m] - 16, gData_081988fc[m],
                              gData_0819890a[m]);
        }
        for (i = 0; i != 3; i++) {
          s32 m;
          t = frame - 8 - i * 2;
          if (t >= 0 && t <= 8) {
            m = t / 3;
            if (action->side == 0)
              drawFuncs.draw[0](screen, buffer + gData_08198934[m],
                                gData_08198940[i][0] - gData_0819893a[m] / 2,
                                gData_08198940[i][1] - gData_0819893d[m] / 2 -
                                    16,
                                gData_0819893a[m], gData_0819893d[m]);
            else
              drawFuncs.draw[0](
                  screen, buffer + gData_08198934[m],
                  128 - gData_08198940[i][0] - gData_0819893a[m] / 2,
                  gData_08198940[i][1] - gData_0819893d[m] / 2 - 16,
                  gData_0819893a[m], gData_0819893d[m]);
          }
        }
        HeapSlot_Release(104);
        BattleEffect_FetchRectangleBlitters(action->side, &drawFuncs);
      } else if (kind == 62) {
        s32 t;
        scrollA += scrollB;
        if (frame > 6)
          scrollB = scrollB * 48 / 64;
        REG_BG2X = (scrollA >> 16) << 8;
        if (frame == 0)
          REG_BLDALPHA = 0x810;
        t = frame - 24;
        if (t >= 0 && t < 16)
          REG_BLDALPHA = (16 - t) | 0x1000;
        if (frame >= 4 && frame < 6)
          Iwram_FillWords(screen, 0x4000, 0x3f3f3f3f);
        if (frame < 4) {
          if (action->side == 1)
            drawFuncs.draw[0](screen, work->frames, 0, 24, 80, 104);
          else
            drawFuncs.draw[0](screen, work->frames, 48, 24, 80, 104);
        } else {
          if (frame < 8) {
            if (action->side == 1)
              drawFuncs.draw[0](screen, work->frames, 0, 24, 80, 104);
            else
              drawFuncs.draw[0](screen, work->frames, 48, 24, 80, 104);
          }
          if (action->side == 1)
            drawFuncs.draw[0](screen, work->frames + 0x1e00, 16, 16, 80, 104);
          else
            drawFuncs.draw[0](screen, work->frames + 0x1e00, 32, 16, 80, 104);
          if (frame >= 6 && frame < 8)
            drawFuncs.draw[0](screen, work->frames + 0x3e80, 0, 16, 128, 91);
          if (frame >= 8 && frame < 10)
            drawFuncs.draw[0](screen, buffer, 0, 16, 128, 91);
          if (frame >= 10 && frame < 12)
            drawFuncs.draw[0](screen, buffer + 0x2d80, 0, 16, 128, 59);
          if (frame >= 12 && frame < 14)
            drawFuncs.draw[0](screen, buffer + 0x4b00, 0, 16, 128, 29);
          if (frame >= 14 && frame < 16)
            drawFuncs.draw[0](screen, buffer + 0x5980, 0, 16, 128, 26);
        }
      } else if (kind == 50) {
        s32 m;
        for (i = 0; i != 12; i++) {
          if (frame >= i + 6 && frame < i + 18) {
            s32 x;
            m = (frame - i - 6) / 2;
            x = target[0] / 2 - gData_08197467[m] / 2;
            if (i & 1)
              x += (i + 1) / 2 * 3;
            else
              x -= (i + 1) / 2 * 3;
            {
              s32 which;
              which = 1;
              if (i != 0)
                which = ((i - 1) & 3) > 1;
              drawFuncs.draw[which](
                  screen, buffer + gBattleEffect_PuffArcCells[m], x,
                  gBattleEffect_PuffArcCellBiasY[m] + 48, gData_08197467[m],
                  gBattleEffect_PuffArcCellHeights[m]);
            }
          }
        }
      } else if (kind == 46) {
        if (frame == 0) {
          struct Particle *p;
          p = SPARKS;
          for (i = 0; i != 64; i++, p++) {
            p->x = (Random16() & 127) + 32;
            p->y = 0;
            p->z = 0;
            p->vx = Random16() & 0xffff;
            p->vy = Random16() & 0xffff;
            p->vz = Random16() & 0xffff;
          }
          SPARKS[63].y = 159;
        }
        p = SPARKS;
        for (i = 0; i != 64; i++, p++) {
          if (p->x >= 0 && frame >= i / 2) {
            s32 m;
            m = i & 3;
            Render_ResetTransformState();
            SceneTransform_ApplyPitch(p->vx);
            SceneTransform_ApplyYaw(p->vy);
            EffectPosition_ProjectPoint(p, point);
            point[0] = point[0] / 2 + target[0] / 2;
            point[1] = point[1] + target[1] + 32;
            drawFuncs.draw[1](screen, buffer + gData_08198946[m], point[0] - 4,
                              point[1] - 4, 8, 8);
            p->x -= 6;
            if (p->x < 0 && ((i & 7) == 0 || i == 63)) {
              Sound_PlayFar(133);
              BattleEffect_TintBattler(action->actors[0], 7, 5, 0, 4);
            }
          }
        }
      } else if (kind == 38) {
        if (frame >= 5 && frame < 50) {
          if (frame > 25)
            h = 196 - frame * 4;
          else
            h = frame * 16 - 64;
          if (h > 96)
            h = 96;
          drawFuncs.draw[0](screen, buffer, 48, 104 - h, 32, h);
        }
      } else if (kind == 79) {
        struct Particle *p;
        p = SPARKS;
        if (frame == 0) {
          p->x = (target[0] / 2) << 16;
          p->y = (target[1] + 48) << 16;
          if (action->side == 0)
            p->vx = -0x30000;
          else
            p->vx = 0x30000;
          p->vy = 0x80000;
          p->x += -p->vx * 12;
          p->y += -p->vy * 12;
        }
        if (frame <= 150) {
          REG_BLDCNT = 0;
          work->unk7780 = 1;
          work->unk7784 = 0;
        } else {
          work->unk7780 = 2;
          work->unk7784 = 50;
          REG_BLDCNT = 0x3f44;
        }
        if (frame < 32) {
          drawFuncs.draw[1](screen, buffer + 0x800, (p->x >> 16) - 24,
                            (p->y >> 16) - 96, 48, 96);
          if (frame < 12)
            BattleEffectKernels_IntegrateVector2(SPARKS, 64, 0);
          else if (frame == 12)
            p->vy = -p->vy / 3;
          else if (frame < 13 || frame > 18)
            BattleEffectKernels_IntegrateVector2(SPARKS, 64, 0x4000);
        }
        if (frame == 11) {
          Sound_PlayFar(144);
          work->unk77a8 = 16;
        }
      } else if (kind == 63 || kind == 64) {
        s32 x;
        s32 y;
        if (frame < 6) {
          if (action->side == 0) {
            x = target[0] / 2 + (6 - frame) * 3 * 2;
            y = target[1] - (6 - frame) * 3 * 4 + 24;
          } else {
            x = target[0] / 2 - (6 - frame) * 3 * 2;
            y = target[1] - (6 - frame) * 3 * 4 + 24;
          }
          drawFuncs.draw[1](screen, buffer, x - 16, y - 32, 32, 64);
        }
      } else if (kind == 70) {
        s32 x;
        s32 y;
        if (frame > 31)
          REG_BLDALPHA = (48 - frame) | 0x1000;
        if (frame < 14) {
          struct Particle *p;
          if (action->side == 0) {
            x = target[0] / 2 + (14 - frame) * 3 * 2;
            y = target[1] - (14 - frame) * 3 * 4 + 24;
          } else {
            x = target[0] / 2 - (14 - frame) * 3 * 2;
            y = target[1] - (14 - frame) * 3 * 4 + 24;
          }
          drawFuncs.draw[1](screen, buffer, x - 16, y - 32, 32, 64);
          if (frame == 13) {
            Sound_PlayFar(134);
            work->unk77a8 = 8;
            BattleEffect_StartActorMotion(action->actors[0], 1, 0x20000,
                                          0x80000, 0x30000, 150);
            p = SPARKS;
            table = gPrism_ShardStarts;
            for (i = 0; i != 16; i++, p++) {
              p->x = (*table++ + x - 40) << 16;
              p->y = *table++ << 16;
              p->vx = ((Random16() & 127) - 64) << 11;
              p->vy = -(Random16() & 127) << 11;
              p->z = 32;
              p->life = 0;
            }
          }
        } else {
          for (i = 0; i != 16; i++) {
            s32 m;
            particle = &SPARKS[i];
            {
              s32 a;
              a = i % 5 * 3;
              m = a + particle->life / 96 % 3;
            }
            drawFuncs.draw[i > 2 ? 0 : 1](
                screen, buffer + 0x800 + gData_08197834[m],
                (particle->x >> 16) - gPrism_ShardWidths[m] / 2,
                (particle->y >> 16) - gData_08197826[m] / 2,
                gPrism_ShardWidths[m], gData_08197826[m]);
            BattleEffectKernels_IntegrateVector2(particle, 64, 0x2000);
            particle->life += particle->z;
            if (particle->z > 1 && (frame & 1))
              particle->z--;
          }
        }
      } else if (kind == 94) {
        s32 x;
        s32 y;
        if (frame == 0) {
          struct Particle *p;
          Resource_LoadAndDecompress((s32)&ResourceId_BoulderPicture,
                                     buffer + 0x2000, 1, 1);
          REG_BLDALPHA = 0x410;
          work->unk7780 = 1;
          work->unk7784 = 0;
          REG_BG2PA = 0x100;
          p = work->particle;
          for (i = 0; i != 32; i++, p++) {
            p->x = ((Random16() & 31) + 48) << 16;
            p->y = ((Random16() & 31) + 64) << 16;
            p->z = Random16();
            p->vx = ((Random16() & 63) - 32) << 12;
            p->vy = -((Random16() & 127) + 16) << 12;
            p->vz = (Random16() & 15) + 16;
            p->life = 0;
          }
        }
        if (frame == 14) {
          BattleEffect_StartActorMotion(action->actors[0], 1, 0x20000, 0x80000,
                                        0x30000, 150);
          Sound_PlayFar(134);
          work->unk77a8 = 8;
        }
        i = 0;
        particle = work->particle;
        if (frame < 14) {
          if (action->side == 0) {
            x = target[0] + (14 - frame) * 3 * 2;
            y = target[1] - (14 - frame) * 3 * 4 + 24;
          } else {
            x = target[0] - (14 - frame) * 3 * 2;
            y = target[1] - (14 - frame) * 3 * 4 + 24;
          }
          drawFuncs.draw[0](screen, buffer + 0x2000, 32, y - 32, 64, 64);
          REG_BG2X = (64 - x) << 8;
        } else {
          for (; i != 32; i++, particle++) {
            s32 m;
            s32 w;
            s32 h;
            s32 left;
            s32 top;
            m = i % 5 * 3;
            m += particle->z / 96 % 3;
            w = gSpire_ShardWidths[m];
            left = (particle->x >> 16) - w / 2;
            top = particle->y >> 16;
            h = gData_081974ad[m];
            top -= h / 2;
            drawFuncs.draw[0](screen, buffer + 0x83c + gSpire_ShardCells[m],
                              left, top, w, h);
            BattleEffectKernels_IntegrateVector2(particle, 63, 0x8000);
            particle->z += particle->vz;
            if ((frame & 1) && particle->vz > 0)
              particle->vz--;
          }
        }
      } else if (kind == 84) {
        s32 fade;
        s32 angle;
        if (frame == 0) {
          struct Particle *p;
          p = RINGS;
          for (i = 0; i != 32; i++, p++) {
            p->z = Random16();
            p->vz = (Random16() & 255) + 128;
            if (Random16() & 1)
              p->vz = -p->vz;
            p->life = Random16() & 3;
          }
        }
        if (frame == 4)
          work->unk77a8 = 8;
        if (frame == 6)
          Sound_PlayFar(221);
        if (frame == 24)
          Sound_PlayFar(390);
        if (frame >= 8 && frame < 24) {
          s32 y;
          for (i = 0; i != 3; i++) {
            s32 m;
            s32 x;
            m = i & 3;
            angle = Random16() & 0xffff;
            x = ((Trig_Sin(angle) << 4) >> 16) + target[0] / 2 -
                gData_08197492[m] / 2;
            y = ((Trig_Cos(angle) << 4) >> 16) - gData_08197498[m] / 2;
            RELEASE_BLITTERS();
            Func_08196404(188, 7, 7, gData_0819894e[Random16() & 3] | 3, 1);
            IWRAM_SLOT_BC(screen, buffer + gBattleEffect_GlintCells[m], x,
                          y + 80, gData_08197492[m], gData_08197498[m]);
            HeapSlot_Release(188);
            BattleEffect_FetchRectangleBlitters(action->side, &drawFuncs);
          }
        }
        if (frame > 3) {
          struct Particle *p;
          struct Model *model;
          matrix = Heap_AllocEwram(32);
          model = Render_AllocateModelList(1);
          texture164.width = 5;
          texture164.height = 7;
          model->kind = 6;
          model->texture = &texture164;
          model->mesh = gData_08199268;
          model->matrix = matrix;
          for (i = 0; i != 20; i++) {
            struct Particle *p;
            p = &RINGS[i];
            if (frame >= i && frame < i + 8) {
              fade = 0;
              if (frame == i)
                fade = -16;
              else if (frame >= i + 4)
                fade = -((frame - i - 4) * 16);
              model->fade = fade;
              {
                s32 life = p->life;
                texture164.pixels = buffer + 0xc56 + (life << 12);
                p->life = (life + 1) & 3;
              }
              Render_ResetTransformState();
              Func_08015160((target[0] / 2 - 64) << 16, (target[1] - 64) << 16,
                            0);
              SceneTransform_ApplyRoll(p->z);
              p->z += p->vz;
              SceneTransform_ApplyScale3(0x40000, 0x10000, 0x10000);
              Render_TransformVertices(gData_081991b0, matrix, 4);
              Render_DrawModelList(model);
            }
          }
          Heap_Release(model);
          Heap_Release(matrix);
        }
      } else if (kind == 71 || kind == 80 || kind == 82) {
        void *matrix;
        struct Model *model;
        s32 fade;
        matrix = Heap_AllocEwram(0x200);
        model = Render_AllocateModelList(1);
        fade = 0;
        if (frame > 39)
          fade = (40 - frame) * 4;
        else
          work->unk77a8 = 1;
        texture156.width = 6;
        texture156.height = 6;
        texture156.pixels = buffer;
        model->kind = 7;
        model->texture = &texture156;
        model->mesh = gData_08199340;
        model->matrix = matrix;
        model->fade = fade;
        Render_ResetTransformState();
        Func_08015160((target[0] / 2 - 64) << 16, (target[1] - 64) << 16, 0);
        SceneTransform_ApplyScale3(0x8000, 0x10000, 0x8000);
        if (frame == 8)
          Sound_PlayFar(104);
        if (fade >= -63) {
          fade = frame << 12;
          if (kind == 82) {
            SceneTransform_ApplyPitch(-0x800);
            SceneTransform_ApplyYaw(-frame << 11);
            SceneTransform_ApplyPitch(-frame << 4);
          } else {
            SceneTransform_ApplyPitch(-0x1000);
            SceneTransform_ApplyYaw(frame << 11);
          }
          Func_0801521c(fade);
          Render_TransformVertices(gData_08199210, matrix, 4);
          Render_DrawModelList(model);
          if (kind != 82) {
            SceneTransform_ApplyYaw(-frame << 12);
            Func_0801521c(0x8000);
            Render_TransformVertices(gData_08199210, matrix, 4);
            Render_DrawModelList(model);
          }
        }
        Heap_Release(model);
        Heap_Release(matrix);
        if (kind == 82) {
          s32 m;
          s32 two;
          m = frame / 4;
          two = m * 2;
          fade = 0x10000;
          if (frame > 39)
            fade = 0x38000 - (frame << 12);
          if (two > 1 && fade > 0) {
            Func_0815b434(work->frames, m * 4, fade);
            HeapSlot_Release(188);
            Func_0818caa8(work->frames, target[0] / 2, target[1], m * 4);
            BattleEffect_LoadBlitter(188, 15);
          }
        }
      } else if (kind == 81 || kind == 83 || kind == 92 || kind == 93) {
        void *matrix;
        s32 t;
        matrix = Heap_AllocEwram(0x200);
        model = Render_AllocateModelList(1);
        texture148.width = 7;
        texture148.height = 7;
        texture148.pixels = buffer;
        model->texture = &texture148;
        model->kind = 7;
        model->mesh = gData_08199364;
        model->matrix = matrix;
        if (kind == 92 || kind == 93) {
          s32 fade;
          for (i = 0; i != 3; i++) {
            t = frame - i * 2;
            if (t >= 0) {
              angle = t * 3 << 12;
              fade = 0;
              if (t > 7)
                fade = (8 - t) * 8;
              model->fade = fade;
              if (fade > -64) {
                Render_ResetTransformState();
                Func_08015160((target[0] / 2 - 64) << 16,
                              (target[1] - 64) << 16, 0);
                SceneTransform_ApplyScale3(0x8000, 0x10000, 0x8000);
                SceneTransform_ApplyRoll((i << 14) + 0x2000);
                SceneTransform_ApplyPitch(-0x1000);
                Func_0801521c(angle >> 1);
                Render_TransformVertices(gData_08199210, matrix, 4);
                Render_DrawModelList(model);
              }
            }
          }
        } else if (kind == 83) {
          s32 fade;
          s32 angle;
          t = frame - 4;
          if (t >= 0) {
            angle = t << 13;
            fade = 0;
            if (t > 7)
              fade = (8 - t) * 8;
            model->fade = fade;
            if (fade >= -63) {
              Render_ResetTransformState();
              Func_08015160((target[0] / 2 - 64) << 16, (target[1] - 64) << 16,
                            0);
              SceneTransform_ApplyScale3(0x8000, 0x10000, 0x8000);
              SceneTransform_ApplyPitch(0x4000);
              Func_0801521c(angle);
              Render_TransformVertices(gData_08199210, matrix, 4);
              Render_DrawModelList(model);
            }
          }
        } else {
          for (i = 0; i != 3; i++) {
            t = frame - i * 4;
            if (t >= 0) {
              s32 fade;
              fade = 0;
              if (t > 7)
                fade = (8 - t) * 8;
              model->fade = fade;
              if (fade > -64) {
                Render_ResetTransformState();
                Func_08015160((target[0] / 2 - 64) << 16,
                              (target[1] - 64) << 16, 0);
                SceneTransform_ApplyScale3(0x8000, 0x10000, 0x8000);
                SceneTransform_ApplyRoll((i << 14) + 0x2000);
                SceneTransform_ApplyPitch(-0x1000);
                Func_0801521c(t * 3 << 12);
                Render_TransformVertices(gData_08199210, matrix, 4);
                Render_DrawModelList(model);
              }
            }
          }
        }
        Heap_Release(model);
        Heap_Release(matrix);
        if (kind != 92 && kind != 93) {
          s32 x;
          if (frame == 4) {
            struct Particle *p;
            for (i = 0; i != 64; i++) {
              struct Particle *p;
              p = &SPARKS[i];
              p->x = Random16();
              p->y = 0;
              p->vy = ((Random16() & 3) + 3) << 17;
            }
          }
          if (frame > 4) {
            for (i = 0; i != 64; i++) {
              struct Particle *p;
              s32 k;
              p = &SPARKS[i];
              for (k = 0; k != 12; k++) {
                r = (p->y >> 16) - k * 2;
                if (r >= 0) {
                  s32 n;
                  x = target[0] / 2 + ((Trig_Sin(p->x) * r) >> 16);
                  y = target[1] + ((Trig_Cos(p->x) * r) >> 16);
                  n = 2 - k / 8;
                  drawFuncs.draw[0](
                      screen, (u8 *)tiles + gBattleEffect_MoteCells[n - 1],
                      x - n / 2, y - n, n, n * 2);
                }
              }
              p->y += p->vy;
              p->vy = p->vy * 62 / 64;
            }
          }
        }
      } else if (kind == 91) {
        void *matrix;
        s32 t;
        matrix = Heap_AllocEwram(0x200);
        model = Render_AllocateModelList(1);
        texture140.width = 7;
        texture140.height = 7;
        texture140.pixels = buffer;
        model->kind = 7;
        model->texture = &texture140;
        model->mesh = gData_08199364;
        model->matrix = matrix;
        t = frame - 4;
        if (t >= 0) {
          s32 fade;
          angle = t << 14;
          fade = 0;
          if (t > 7)
            fade = (8 - t) * 8;
          model->fade = fade;
          if (fade >= -63) {
            Render_ResetTransformState();
            Func_08015160((target[0] / 2 - 64) << 16, 0x280000, 0);
            SceneTransform_ApplyScale3(0x8000, 0x10000, 0x8000);
            SceneTransform_ApplyPitch(1000);
            Func_0801521c(angle);
            Render_TransformVertices(gData_08199210, matrix, 4);
            Render_DrawModelList(model);
          }
        }
        texture140.width = 7;
        texture140.height = 4;
        texture140.pixels = buffer + 0x4000;
        model->mesh = gData_08198d2c;
        for (i = 0; i != 2; i++) {
          s32 start;
          start = gData_08198952[i] + 4;
          if (frame > start) {
            fade = (start - frame) * 8;
            if (fade > -16)
              fade = -16;
            if (fade > -64) {
              scale = (frame - start) * gData_08198956[i] * 1500 + 0x4180;
              model->fade = fade;
              Render_ResetTransformState();
              Func_08015160((target[0] / 2 - 60) << 16, gData_0819895a[i] << 16,
                            0);
              SceneTransform_ApplyScale3(scale * 2, scale * 4, scale / 4);
              SceneTransform_ApplyPitch(1000);
              if (i & 1)
                SceneTransform_ApplyYaw((i * 2 + frame) << 12);
              else
                SceneTransform_ApplyYaw((i * 2 - frame) << 12);
              Render_TransformVertices(gData_08198c6c, matrix, 16);
              Render_DrawModelList(model);
            }
          }
        }
        Heap_Release(model);
        Heap_Release(matrix);
        if (frame == 4)
          work->unk77a8 = 8;
      } else if (kind == 90) {
        struct Texture *tex;
        s32 scale;
        matrix = Heap_AllocEwram(0x200);
        model = Render_AllocateModelList(1);
        /* FAKEMATCH(temporary): tex holds &texture132; as texture132.width and
         * so on, 106 of 7666 instructions differ in tla-en. */
        tex = &texture132;
        tex->width = 6;
        tex->height = 6;
        model->texture = tex;
        model->matrix = matrix;
        tex->pixels = buffer;
        model->kind = 7;
        model->mesh = gData_081990d0;
        for (i = 0; i != 4; i++) {
          start = gData_0819895e[i] + 4;
          model->frame = (frame * 8 + i * 32) & 127;
          if (frame == start + 4)
            Sound_PlayFar(104);
          if (frame > start) {
            fade = (start - frame) * 8 + 56;
            if (fade > -16)
              fade = -16;
            if (fade > -64) {
              scale = (frame - start) * gData_08198962[i] * 2000 + 0x4000;
              model->fade = fade;
              Render_ResetTransformState();
              Func_08015160((target[0] / 2 - 60) << 16, 0x280000, 0);
              SceneTransform_ApplyScale3(scale, gData_08198966[i] * scale * 2,
                                         scale / 4);
              SceneTransform_ApplyPitch(1000);
              if (i & 1)
                SceneTransform_ApplyYaw((i * 8 + frame) << 10);
              else
                SceneTransform_ApplyYaw((i * 8 - frame) << 10);
              Render_TransformVertices(gData_08199090, matrix, 32);
              Render_DrawModelList(model);
            }
          }
        }
        Heap_Release(model);
        Heap_Release(matrix);
        if (frame == 4)
          work->unk77a8 = 20;
      } else if (kind == 15) {
        if (frame == 0) {
          struct Particle *p;
          p = SPARKS;
          for (i = 0; i != 16; i++, p++) {
            p->life = -i * 2;
            p->x = Random16();
            p->vx = (Random16() & 511) + 512;
            p->z = i * 2 + 80;
          }
        }
        for (i = 0; i != 0x400; i++) {
          if (buffer[0x400 + i] >> 1 == frame / 2)
            buffer[0x800 + i] = buffer[i];
        }
        EffectPosition_ProjectScaledBattler(action->actors[0], point2);
        point2[0] = point2[0] / 2;
        for (i = 0; i != 16; i++) {
          struct Particle *p;
          p = &SPARKS[i];
          if (p->life >= 0 && p->z > 0) {
            s32 m;
            s32 x;
            s32 y;
            m = i % 3;
            x = point2[0] + ((Trig_Sin(p->x) * p->z) >> 16);
            y = point2[1] + ((Trig_Cos(p->x) * p->z) >> 16);
            p->z -= 4;
            p->x += p->vx;
            // Unused: this arm runs only for kind 15, so the kind 16 draw never
            // runs; the source keeps it.
            if (kind == 16)
              drawFuncs.draw[0](screen, buffer, x - 16, y - 16, 32, 32);
            else
              drawFuncs.draw[0](screen, buffer + m * 0x240, x - 12, y - 12, 24,
                                24);
          }
          p->life++;
        }
      } else if (kind == 16) {
        p = SPARKS;
        for (i = 0; i != 32; i++, p++) {
          if (frame >= i / 2 + 4 && p->life <= 23) {
            s32 m;
            m = 0;
            if (p->life > 11)
              m = (p->life - 12) / 2;
            EffectPosition_ProjectPoint(p, point);
            point[0] = point[0] / 2;
            drawFuncs.draw[1](screen, buffer + m * 0x400, point[0] - 16,
                              point[1] - 16, 32, 32);
            BattleEffectKernels_IntegrateVector3(p, 62, 0x800);
            BattleEffectKernels_IntegrateVector3(p, 62, 0x800);
            p->life++;
          }
        }
        if (frame == 4)
          work->unk77a8 = 12;
        if (frame == 8)
          Sound_PlayFar(103);
      } else if (kind == 10) {
        if (frame == 0) {
          work->unk7780 = 1;
          work->unk7784 = 0;
          REG_BLDALPHA = 0x610;
        }
        p = SPARKS;
        for (i = 0; i != 24; i++, p++) {
          if (frame >= i / 2 + 4 && p->life <= 23) {
            s32 m;
            m = 0;
            if (p->life > 11)
              m = (p->life - 12) / 3;
            EffectPosition_ProjectPoint(p, point);
            point[0] = point[0] / 2;
            drawFuncs.draw[0](screen, buffer + m * 0x800, point[0] - 16,
                              point[1] - 32, 32, 64);
            BattleEffectKernels_IntegrateVector3(p, 58, 0x1000);
            p->life++;
          }
        }
        if (frame == 4)
          work->unk77a8 = 12;
        if (frame == 8)
          Sound_PlayFar(103);
      } else if (kind == 42) {
        s32 m;
        if (frame > 47)
          REG_BLDALPHA = (64 - frame) | 0x1000;
        p = SPARKS;
        for (i = 0; i != 16; i++, p++) {
          m = i % 3;
          EffectPosition_ProjectPoint(p, point);
          point[0] = point[0] / 2;
          drawFuncs.draw[i & 1](screen, buffer + m * 0x240, point[0] - 12,
                                point[1] - 12, 24, 24);
          BattleEffectKernels_IntegrateVector3(p, 60, 1 << ((i & 3) + 11));
          p->life++;
        }
      } else if (kind == 76) {
        if (frame == 18)
          Sound_PlayFar(390);
        for (i = 0; i != 3; i++) {
          if (frame == i * 3 + 3) {
            Sound_PlayFar(212);
            Iwram_FillWords(screen, 0x4000, 0x2f2f2f2f);
          }
        }
      } else if (kind == 74) {
        if (frame == 8)
          Sound_PlayFar(103);
        p = SPARKS;
        for (i = 0; i != 32; i++, p++) {
          if (frame >= i / 2 + 4) {
            s32 life;
            life = p->life;
            if (life <= 23) {
              s32 m;
              m = life / 4;
              EffectPosition_ProjectPoint(p, point);
              point[0] = point[0] / 2;
              drawFuncs.draw[1](screen, buffer + m * 0x480, point[0] - 12,
                                point[1] - 24, 24, 48);
              BattleEffectKernels_IntegrateVector3(p, 62, 0x800);
              BattleEffectKernels_IntegrateVector3(p, 62, 0x800);
              p->life++;
            }
          }
        }
      } else if (kind != 100) {
        s32 m;
        p = SPARKS;
        for (i = 0; i != 16; i++, p++) {
          if (frame >= i + 4) {
            s32 life;
            life = p->life;
            if (life <= 23) {
              m = life / 4;
              EffectPosition_ProjectPoint(p, point);
              point[0] = point[0] / 2;
              // DEAD: kind 74 has its own arm above, so only kind 69 reaches
              // this test.
              if (kind == 69 || kind == 74)
                drawFuncs.draw[1](screen, buffer + m * 0x480, point[0] - 12,
                                  point[1] - 24, 24, 48);
              else
                drawFuncs.draw[i & 1](screen, buffer + m * 0x480, point[0] - 12,
                                      point[1] - 24, 24, 48);
              if (kind == 55)
                BattleEffectKernels_IntegrateVector3(p, 60, 0x400);
              else
                BattleEffectKernels_IntegrateVector3(p, 60, 0x1000);
              p->life++;
            }
          }
        }
      }

      if (kind == 8 && frame < 44) {
        s32 half;
        EffectPosition_ProjectPoint(origin, point);
        point[0] = point[0] / 2;
        drawFuncs.draw[1](screen, buffer + 0x3c56, point[0] - 10, point[1] - 4,
                          20, 40);
        if (frame == 23)
          (GetBattleObjectSlotFar(action->actor)->object)->velocityY = 0x30000;
        if (frame > 19)
          origin[1] += 0x40000;
        if (frame == 23) {
          struct Particle *p;
          half =
              BattleUnit_GetEffectHeightFar(action->actors[0]) / 2 + 0x1e0000;
          p = work->particle;
          for (i = 0; i != 64; i++, p++) {
            p->x = object2->x;
            p->y = half;
            p->z = object2->z;
            p->vx = ((Random16() & 255) - 127) << 11;
            p->vy = ((Random16() & 255) - 64) << 11;
            p->vz = ((Random16() & 255) - 127) << 11;
            p->life = i / 2 + 32;
          }
        }
        if (frame == 24) {
          BattleEffect_StartActorMotion(action->actors[0], 1, 0, 0, 0x90000,
                                        100);
          BattleEffect_TintBattler(action->actors[0], 7, 5, 0, 4);
          BattlePlayback_StartFar(134);
          work->unk77a8 = 8;
        }
        if (frame >= 24 && frame < 36)
          drawFuncs.draw[1](screen,
                            work->frames + 0x5100 + ((frame - 24) / 2) * 0x3c0,
                            target[0] / 2 - 8, 28, 20, 48);
      }
      if (kind == 66) {
        for (i = 0; i != 4; i++) {
          if (frame == i * 4 + 8) {
            BattleEffect_TintBattler(action->actors[0], 7, 5, 0, 2);
            work->unk77a8 = 2;
            Sound_PlayFar(133);
          }
        }
      }
      if (kind <= 37 && frame <= 5) {
        EffectPosition_ProjectPoint(origin, point);
        point[0] = point[0] / 2;
        drawFuncs.draw[1](screen, buffer + 0x3c56, point[0] - 10, point[1] - 4,
                          20, 40);
        origin[0] += center[0];
        origin[1] += center[1];
        origin[2] += center[2];
      }
      if (kind != 8 && frame == 3)
        BattlePlayback_StartFar(-1);
      if (frame == 4) {
        if (kind == 91)
          Sound_PlayFar(145);
        else if (kind == 78 || kind == 81 || kind == 98)
          Sound_PlayFar(144);
        else
          Sound_PlayFar(134);
      }
      if (frame == 6) {
        if (kind == 4 || kind == 5 || kind == 7 || kind == 43 || kind == 48 ||
            kind == 73 || kind == 77 || kind == 78 || kind == 49 ||
            kind == 12 || kind == 53 || kind == 64 || kind == 100 ||
            kind == 66 || kind == 81 || kind == 83 || kind == 76 ||
            kind == 98) {
          BattleMotion_ApplyKnockbackFar(action->actors[0], 4);
          work->unk77a8 = 12;
        } else if (kind == 8 || kind == 79 || kind == 85) {
          work->unk77a8 = 8;
        } else if (kind == 50 || kind == 44 || kind == 63) {
          BattleMotion_ApplyKnockbackFar(action->actors[0], 1);
          work->unk77a8 = 2;
        } else if (kind == 60 || kind == 38) {
          BattleMotion_ApplyKnockbackFar(action->actors[0], 3);
          work->unk77a8 = 16;
        } else if (kind == 95 || kind == 99 || kind == 96 || kind == 97 ||
                   kind == 92 || kind == 93 || kind == 86 || kind == 74) {
          BattleMotion_ApplyKnockbackFar(action->actors[0], 0);
          work->unk77a8 = 12;
        }
      }
      if (frame == 6)
        BattleEffect_TintBattler(action->actors[0], 7, 5, 0, 4);
      if (frame == 14)
        BattleEffect_TintBattler(action->actors[0], 7, 5, 0, 4);
      BattleCamera_Shake(8, 8);
      BattleEffect_CountDownBattlerTints();
      work->unk78e8 = 1;
      wait(1);
    }

    if (kind == 51 || kind == 67 || kind == 9 || kind == 10 || kind == 11 ||
        kind == 70 || kind == 79 || kind == 90) {
      Iwram_ClearWords(BG_VRAM, 0x3c00);
      Iwram_ClearWords(screen, 0x3c00);
      action->prelude = 0;
      Task_Remove(Palette_StepFadeTransfer);
      Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
      HeapSlot_Release(188);
      HeapSlot_Release(104);
      if (kind == 51 || kind == 10 || kind == 70 || kind == 79 || kind == 90)
        SummonEffect_RunCanvasMode1(action);
      else if (kind == 9 || kind == 67)
        SummonEffect_RunCanvasMode2(action);
      else if (kind == 11)
        SummonEffect_RunCanvasMode3(action);
      return;
    }
    if (kind == 2 || kind == 3 || kind == 42 || kind == 15 || kind == 52 ||
        kind == 72 || kind == 58 || kind == 59 || kind == 69 || kind == 89)
      Task_Remove(BattleEffect_ArmBg2AffineHBlankDma);
    Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
    HeapSlot_Release(188);
    HeapSlot_Release(104);
    BattleEffect_EndCanvasLayer();
  }
}

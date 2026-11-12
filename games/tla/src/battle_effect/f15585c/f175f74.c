/// @file
/// Routines first written as f175f74.c; none has a twin in The Broken Seal.
// Part of f15585c.c.

#include "../f15585c_internal.h"

// A pair of 16.16 scales; the game keeps each start value in the rodata.
struct ScalePair {
  s32 v[2];
};

/* VIEW(f175f74): tag struct Particle as struct Particle__f175f74; f15585c.c has another definition; tried with the definition the rest of the object has, it does not compile in tla-ja: games/tla/src/battle_effect/f15585c.c:23763: structure has no member named `timer'. */
struct Particle__f175f74 {
  s32 x;
  s32 y;
  s32 z;
  s32 vx;
  s32 vy;
  s32 vz;
  s32 timer;
};

/* VIEW(f175f74): tag struct SpriteObject as struct SpriteObject__f175f74; f15585c.c has another definition; tried with the definition the rest of the object has, it does not compile in tla-ja: games/tla/src/battle_effect/f15585c.c:23652: structure has no member named `oamPriority'. */
struct SpriteObject__f175f74 {
  u16 unk00;
  u16 unk02;
  u16 unk04 : 10;
  u16 oamPriority : 2;
  u16 unk04Palette : 4;
  u16 unk06;
  u16 tile : 10;
  u16 priority : 2;
  u16 palette : 4;
  u8 unk0a[6];
  u8 slot;
  u8 unk11[6];
  u8 unk17;
};

struct TileSlot {
  u16 unk0;
  u16 offset;
};

struct ModelDesc {
  u32 a : 8;
  u32 b : 8;
  u32 c : 16;
  u8 *tiles;
};

/* VIEW(f175f74): tag struct Model as struct Model__f175f74; f15585c.c has another definition; tried with the definition the rest of the object has, it does not compile in tla-ja: games/tla/src/battle_effect/f15585c.c:24127: structure has no member named `depth'. */
struct Model__f175f74 {
  s32 kind;
  s32 unk04;
  const void *mesh;
  void *texture;
  struct ModelDesc *desc;
  s32 depth;
};

struct ActorPos {
  u8 unk00[8];
  s32 x;
  s32 y;
  s32 z;
  u8 unk14[0x34];
  s32 unk48;
};

/* VIEW(f175f74): tag struct SummonWork as struct SummonWork__f175f74; f15585c.c has another definition; tried with the definition the rest of the object has, it does not compile in tla-ja: games/tla/src/battle_effect/f15585c.c:23602: structure has no member named `transferMode'. */
struct SummonWork__f175f74 {
  struct Particle__f175f74 drops[16];
  struct Particle__f175f74 members[16];
  struct Particle__f175f74 homes[16];
  struct Particle__f175f74 columns[12];
  u8 unk690[0x6c];
  s32 wave;
  u8 buffer[0x7080];
  s32 transferMode;
  s32 transferValue;
  u8 unk_7788[0x20];
  s32 unk77a8;
  u8 unk77ac[0x30];
  struct SpriteObject__f175f74 *objects[14];
  u8 unk7814[0xd0];
  // volatile: the VBlank handler counts it up; every effect that waits on it
  // reads it twice per test (f14153c.c, the drafts at 0813fd84,
  // 08173aac).
  volatile u32 frameSync;
  s32 frameDone;
};


struct Scene68 {
  u8 unk000[0x56c];
  u16 palette[0x82];
  u16 unk670;
  u8 unk672[0x1f5];
  u8 unk867;
};

struct Unit30 {
  u8 unk00[0x36];
  s16 unk36;
};

struct Unit88 {
  u8 unk00[0x10];
  s32 unk10;
};

struct Local264 {
  u16 value;
  u8 unk02[26];
};

struct SystemWork {
  u8 unk00[0x24];
  struct Scene68 *scene;
  u8 unk28[0x8];
  struct Unit30 *unit30;
  u8 unk34[0x28];
  struct SummonWork__f175f74 *summon;
  u8 *canvas;
  u8 *glyphs;
  s32 (*blit)(void *, const void *, s32, s32, s32, s32);
  u8 unk6c[0x44];
  struct Unit88 *unit88;
  u8 unkb4[0x8];
  s32 (*blitBc)(void *, const void *, s32, s32, s32, s32);
};

struct RegisterQueueEntry {
  u32 value;
  u32 address;
  u32 mode;
};

struct RegisterQueue {
  u16 count;
  u16 unk2;
  struct RegisterQueueEntry entries[32];
};


#define SYSTEM_WORK ((struct SystemWork *)Ram_HeapSlots)
extern u32 gCameraSceneParameters[];
#define REGISTER_QUEUE ((struct RegisterQueue *)&gIoWriteQueue)

#define TILE_SLOTS ((struct TileSlot *)gResourceTableEntries)
extern u8 gMapCellBuffer[];
#define SPRITES ((struct SpriteObject__f175f74 **)Ram_MapCellBuffer)
#define SPRITE_BUFFER gMapCellBuffer
extern u8 gData_02014000[];
#define OAM_BUFFER ((u32 *)gData_02014000)
#define REVEAL_BUFFER (Ram_MapCellBuffer + 0x4000)
#define PARTICLES ((struct Particle__f175f74 *)(Ram_MapCellBuffer + 0x4200))
#define OBJ_VRAM ((u8 *)0x06010000)
#define BG_VRAM_A900 ((u8 *)0x0600a900)
#define PLTT ((u16 *)0x05000000)

extern char ResourceId_AerialViewBackdrop;
extern char ResourceId_CrimsonBeastTiles;
extern char ResourceId_RedSlashTiles;
extern char ResourceId_RedCrescentTiles;
extern char ResourceId_WhiteArrowsTiles;
extern char ResourceId_WhiteWispsTiles;
extern char ResourceId_Data0B4;
extern char ResourceId_Data0B5;
extern char ResourceId_CharmRingSheet;
extern char ResourceId_EffectPalette00;
extern char ResourceId_BoltSheet;
extern char ResourceId_CurseSealSheet;
extern char ResourceId_Data0C2;
extern char ResourceId_Data0C3;
extern char ResourceId_Data134;
extern char ResourceId_ParticleSpritesD;
extern char ResourceId_FlashBurstSheet;

extern const struct ScalePair Data_08196ea8;
extern const struct ScalePair Data_08196eb0;
extern const struct ScalePair Data_08196eb8;
extern const struct ScalePair Data_08196ec0;
extern u16 gData_08197410[];
extern u8 gData_08198c67[];
extern const u8 gData_08199210[];
extern const u8 gData_08199340[];
extern const u8 gData_08199364[];
void BattleEffect_ProcessPendingGraphicsTransfer(void);

void BattleEffect_StartCanvasLayer(s32);
void BattleEffect_ConfigureEffectDisplay(void);
void Func_08163c2c(s32, s32);
void BattleActor_SpawnObjectsForListFar(s16 *list, s32 mode);
void wait(s32);
void Palette_LoadBasicObjColors(void);
void BattleBackground_LoadFar(s32, s32, s32);
void Func_08164b2c(s32, s32, s32);
void BattleEffect_FetchRectangleBlitters(s32 flip, void **out);
/* VIEW(f175f74): routine Func_0815b3b0 as struct SpriteObject__f175f74 *Func_0815b3b0(s32, s32, u32, u32); the definition has struct EffectSprite__f15b3b0 *Func_0815b3b0(s32 kind, s32 value); tried as the definition's struct EffectSprite__f15b3b0 *Func_0815b3b0(s32 kind, s32 value), which the object already declares, it does not compile in tla-ja: games/tla/src/battle_effect/f15585c.c:23681: too many arguments to function `Func_0815b3b0'. */
extern struct SpriteObject__f175f74 *Func_0815b3b0__f175f74(s32, s32, u32, u32) __asm__("Func_0815b3b0");
void Resource_LoadAndDecompress(s32, void *, s32, s32);
void Sound_PlayFar(s32);
u32 Random16(void);
s32 Func_0800206c(s32, s32);

s32 Trig_Sin(s32);
s32 Trig_Cos(s32);
s32 __divsi3(s32, s32);
struct PoolEntry *Render_AllocateModelList(s32 count);
void Render_ResetTransformState(void);
void Func_080151e4(s32, s32, s32);
void Func_08015160(s32, s32, s32);
void SceneTransform_ApplyPitch(s32);
void Func_080150e4(s32);
void SceneTransform_ApplyYaw(s32);
void Func_0801521c(s32);
void Render_TransformVertices(const void *corners, void *vertices, s32 count);
void Graphics_LoadGreyRamp(void);
void Func_0815b434(u8 *map, s32 size, s32 scale);
void Func_0818caa8(void *, s32, s32, s32);
s32 BattleEffect_LoadBlitter(s32, s32);
void ColorBuffer_ScaleFar(void *, void *, s32, s32);
/* VIEW(f175f74): routine GetBattleObjectSlotFar as struct ActorPos **GetBattleObjectSlotFar(s32); motobj.h has struct BattleObj *GetBattleObjectSlotFar(s32 objectId); tried as the 11 declarations' void **GetBattleObjectSlotFar(s32 objectId), it does not compile in tla-ja: games/tla/src/battle_effect/f15585c.c:23540: conflicting types for `GetBattleObjectSlotFar'. */
extern struct ActorPos **GetBattleObjectSlotFar__f175f74(s32) __asm__("GetBattleObjectSlotFar");
void Func_0814cd48(s32, s32, s32, s32, s32);
void Func_081180e8(s32);
void BattleEffect_StartActorMotion(s32, s32, s32, s32, s32, s32);
void BattleEffectKernels_IntegrateVector2(struct EffectStep *step, s32 damping,
                                          s32 gravity);

void Camera_ApplyVerticalShake(s32);
void BattleEffect_CountDownBattlerTints(void);

void BattleEffect_EndCanvasLayer(void);

static __inline__ s32 CpuCopy(void *destination, const void *source, s32 size) {
  /* FAKEMATCH(wrapper): every copy goes through this inline; as direct calls
   * to Iwram_CopyWords, 196 of 4354 instructions of BattleEffect_RunCharon
   * differ in tla-en. */
  return Iwram_CopyWords(destination, source, size);
}

static __inline__ s32 CpuFill(void *destination, s32 size, u32 value) {
  /* FAKEMATCH(wrapper): every fill goes through this inline; as direct calls
   * to Iwram_FillWords, 204 of 4354 instructions of BattleEffect_RunCharon
   * differ in tla-en. */
  return Iwram_FillWords(destination, size, value);
}

void BattleEffect_RunCharon(struct BattleEffectArgument *arg) {
  u16 tiles[16];
  struct Local264 local264;
  s32 pos248[4];
  s32 pos232[4];
  s32 pos216[4];
  s32 pos200[4];
  s32 pos184[4];
  DrawRectangleFn routine[2];
  struct ModelDesc desc144;
  struct ModelDesc desc128;
  struct ModelDesc desc112;
  struct ModelDesc desc104;
  u8 *canvas;
  struct SummonWork__f175f74 *work;
  struct Unit88 *unit88;
  s32 j;
  s32 frame;
  u32 savedWidth;
  u8 *glyphs;
  struct Scene68 *scene;
  DrawRectangleFn *blit;
  s32 posX;
  s32 posZ;
  s32 t56;
  s32 red;
  s32 green;
  s32 blue;
  struct Unit30 *unit30;
  s32 shake;
  s16 savedUnit30;
  s32 i;
  s32 fade;
  u16 *palette;
  u8 *source;
  s32 t;
  struct SpriteObject__f175f74 **sprites;
  struct TileSlot *slots;
  struct TileSlot *objectSlots;
  s32 cell;
  s32 phase;
  s32 halfCell;
  s32 twenty;
  s32 width;

  canvas = SYSTEM_WORK->canvas;
  work = SYSTEM_WORK->summon;
  unit88 = SYSTEM_WORK->unit88;
  savedWidth = gBgScroll[1].x;
  glyphs = SYSTEM_WORK->glyphs;
  sprites = (struct SpriteObject__f175f74 **)gMapCellBuffer;
  scene = SYSTEM_WORK->scene;
  BattleEffect_StartCanvasLayer(0);
  BattleEffect_ConfigureEffectDisplay();
  REG_BG2CNT = 0x784;
  PLTT[0] = 0;
  PLTT[1] = 0;
  work->transferMode = 0;
  {
    s32 order = 0xc80;

    Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, order);
  }
  Func_08163c2c(1, 0);
  gCameraSceneParameters[4] = 240;
  unit88->unk10 = 1;
  gBgScroll[1].x = 0;
  local264.value = 255;
  BattleActor_SpawnObjectsForListFar(&local264, 0);
  REG_WININ = 0x2737;
  REG_WIN0H = 240;
  wait(1);
  Palette_LoadBasicObjColors();
  scene->unk867 = 1;
  BattleBackground_LoadFar(1, (s32)&ResourceId_AerialViewBackdrop, 0);
  Func_08164b2c(0, -20, -20);
  Func_08163c2c(1, 1);
  REG_BLDALPHA = 0x1010;
  REG_BLDCNT = 0x3f44;
  REG_BG2PA = 0x80;
  REG_BG2X = 0;
  blit = routine;
  BattleEffect_FetchRectangleBlitters(0, blit);

  source = (u8 *)Resource_GetTableEntry((s32)&ResourceId_RedSlashTiles);
  CpuCopy(PLTT + 0x120, source, 0x140);
  source += 0x140;
  Resource_DecodeType01(source, work->buffer);
  for (i = 0; i != 0x1000; i++) {
    if (work->buffer[i] != 0) {
      work->buffer[i] += 32;
    }
  }
  {
    struct SpriteObject__f175f74 *object = Func_0815b290(64, 64, 0xc0002000, 0);

    work->objects[0] = object;
    object->priority = 3;
    slots = TILE_SLOTS;
    CpuCopy(OBJ_VRAM + slots[object->slot].offset, work->buffer, 0x1000);
  }
  work->objects[0]->oamPriority = 0;
  work->objects[0]->unk17 = 32;

  source = (u8 *)Resource_GetTableEntry((s32)&ResourceId_WhiteArrowsTiles);
  CpuCopy(PLTT + 0x1f0, source, 32);
  source += 32;
  Resource_DecodeType01(source, work->buffer);
  for (i = 0; i != 16; i++) {
    struct SpriteObject__f175f74 *object = Func_0815b290(16, 16, 0, 0xf000);
    SPRITES[i] = object;
    object->priority = 1;
    CpuCopy(OBJ_VRAM + slots[object->slot].offset, work->buffer + i * 32, 32);
    tiles[i] = SPRITES[i]->tile;
  }
  for (i = 0; i != 32; i++) {
    struct SpriteObject__f175f74 *object = Func_0815b3b0__f175f74(8, 8, 0, 0xf000);
    SPRITES[16 + i] = object;
    CpuCopy(object, sprites[0], 24);
  }
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, glyphs, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, work->buffer, 0, 0);
  for (j = 0; j != 10; j++) {
    cell = 4;
    for (i = 0; i != cell * cell * 2; i++) {
      s32 value = work->buffer[gData_08197410[cell - 1] + i];

      if (value != 0) {
        value -= j * 7;
        if (value <= 0) {
          value = 1;
        }
      }
      glyphs[cell * cell * j * 2 + i] = value;
    }
  }
  Resource_LoadAndDecompress((s32)&ResourceId_Data0B4, work->buffer, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_CharmRingSheet,
                             work->buffer + 0x1000, 1, 1);
  source = (u8 *)Resource_GetTableEntry((s32)&ResourceId_BoltSheet);
  CpuCopy(PLTT, source, 128);
  REG_BG2CNT = 0x784;
  REG_BG2X = 0x800;
  posX = 0;
  posZ = 0;

  for (frame = 0; frame != 394 && !(gInput.repeat & 3); frame++) {
    u8 *order = REVEAL_BUFFER + (frame - 244) * 2;

    t = frame - 32;
    if (t >= 0 && t < 22) {
      Func_08164b2c(-t, -20, frame - 52);
    }
    if (frame == 55) {
      Func_08164b2c(0, -10, -20);
    }
    if (frame == 4) {
      Sound_PlayFar(107);
    }
    if (frame == 55) {
      Sound_PlayFar(208);
    }
    if (frame == 78) {
      Sound_PlayFar(219);
    }
    if (frame == 127) {
      Sound_PlayFar(212);
    }
    if (frame == 131) {
      Sound_PlayFar(149);
    }
    if (frame == 206) {
      Sound_PlayFar(142);
    }
    if (frame == 244) {
      Sound_PlayFar(212);
    }
    if (frame == 302) {
      Sound_PlayFar(212);
    }
    if (frame == 308) {
      Sound_PlayFar(212);
    }
    if (frame == 312) {
      Sound_PlayFar(212);
    }
    if (frame == 350) {
      Sound_PlayFar(104);
    }
    if (frame == 0) {
      work->transferMode = 2;
      work->transferValue = 75;
      REG_BLDALPHA = 0x1010;
      {
        u32 hidden;
        /* FAKEMATCH(temporary): the mask held in a halfword struct set in the
         * for clause; as a literal 0xf000, 4 of 4354 instructions differ in
         * tla-en. */
        struct {
          u16 value;
        } mask;
        s32 *timer;
        u32 *b;
        u32 *a;

        for (i = 0, hidden = (240 << 16) + 160, mask.value = 0xf000,
            timer = &PARTICLES[0].timer, b = (u32 *)(REVEAL_BUFFER + 4),
            a = OAM_BUFFER;
             i != 64; i++) {
          s32 t;

          *a = hidden;
          t = *timer;
          timer += 7;
          *b = tiles[t / 16] | mask.value;
          b += 2;
          a += 2;
        }
      }
    }
    if (frame >= 132 && frame < 240) {
      struct ScalePair scale168 = Data_08196ea8;

      scale168.v[1] = 0x14000;
      scale168.v[0] = 0x14000;
      if (frame == 132) {
        for (i = 0; i != 128; i++) {
          PARTICLES[i].x = (Random16() % 160 + 40) << 16;
          PARTICLES[i].y = ((Random16() & 63) + 32) << 16;
          PARTICLES[i].timer = Random16() & 255;
          PARTICLES[i].vx = ((Random16() & 255) - 127) << 9;
          PARTICLES[i].vy = (-(Random16() & 255) - 128) << 8;
        }
      }
      for (i = 0; i != 48; i++) {
        if (i == 10) {
          scale168.v[1] = 0x10000;
          scale168.v[0] = 0x10000;
        }
        pos248[3] = 0;
        pos248[1] = 0xff0000;
        pos248[0] = PARTICLES[64 + i].x;
        pos248[2] = PARTICLES[64 + i].y + 0x200000;
        Render_ApplyProjectedPlacementFar(SPRITES[i], pos248, scale168.v, 0);
        SPRITES[i]->tile = tiles[PARTICLES[64 + i].timer / 16];
      }
      for (i = 0; i != 128; i++) {
        if (PARTICLES[i].y > -0x100000 && PARTICLES[i].x > -0x100000 &&
            PARTICLES[i].x < 0x1000000) {
          PARTICLES[i].x += PARTICLES[i].vx;
          PARTICLES[i].y += PARTICLES[i].vy;
        }
        PARTICLES[i].timer += i / 16 + 2;
        if (PARTICLES[i].timer > 255) {
          PARTICLES[i].timer -= 256;
        }
      }
    }
    t56 = frame - 56;
    if (t56 >= 0 && t56 < 76) {
      struct ScalePair scale160 = Data_08196eb0;

      scale160.v[0] = scale160.v[1] = Trig_Sin(t56 << 7) * 2 + 0x4000;
      pos232[3] = 0;
      pos232[1] = 0xff0000;
      pos232[0] = 114 << 16;
      pos232[2] = (t56 >= 0 ? 78 : 100) << 16;
      Render_ApplyProjectedPlacementFar(work->objects[0], pos232, scale160.v,
                                        0);
    }
    if (frame == 131) {
      BattleBackground_LoadFar(1, (s32)&ResourceId_AerialViewBackdrop, 0);
      CpuFill(PLTT + 0x60, 0x100, 0x7fff7fff);
      CpuFill(canvas, 0x3c00, 0x3f3f3f3f);
    }
    if (frame >= 132 && frame < 147) {
      CpuFill(canvas, 0x3c00, (0x3f - (frame - 132) * 4) * 0x01010101);
    }
    if ((frame & 3) == 0 && (frame >= 132 && frame < 240)) {
      u16 *target = Resource_GetTableEntry((s32)&ResourceId_AerialViewBackdrop);
      u16 *color = PLTT + 0x60;

      for (i = 0; i != 128; i++, target++, color++) {
        s32 r = (*color >> 10) & 31;
        s32 g = (*color >> 5) & 31;
        s32 b = *color & 31;
        s32 tr;
        s32 tg;
        s32 tb;

        if (frame < 168) {
          tr = (*target >> 10) & 31;
          tg = (*target >> 5) & 31;
          tb = *target & 31;
        } else {
          tr = ((*target >> 10) & 31) - 16;
          tg = ((*target >> 5) & 31) - 16;
          tb = (*target & 31) + 8;
        }
        if (tr < 0) {
          tr = 0;
        }
        if (tg < 0) {
          tg = 0;
        }
        if (tb > 31) {
          tb = 31;
        }
        if (r > tr) {
          r--;
        }
        if (g > tg) {
          g--;
        }
        if (b > tb) {
          b--;
        }
        *color = (r << 10) | (g << 5) | b;
      }
      for (i = 0; i != 15; i++) {
        s32 r = 31 - (frame - 132) / 4;
        s32 g = r;
        s32 b = r;

        if (r > 31) {
          r = 31;
        }
        if (r < 12) {
          r = 12;
        }
        if (g > 31) {
          g = 31;
        }
        if (g < 4) {
          g = 4;
        }
        if (b > 31) {
          b = 31;
        }
        if (b < 4) {
          b = 4;
        }
        PLTT[0x1e1 + i] = (b << 10) | (g << 5) | r;
      }
    }
    if (frame >= 132 && frame < 240) {
      u16 *target = Resource_GetTableEntry((s32)&ResourceId_CrimsonBeastTiles);
      u16 *color = PLTT + 0x100;

      for (i = 0; i != 192; i++, target++, color++) {
        s32 r = (*color >> 10) & 31;
        s32 g = (*color >> 5) & 31;
        s32 b = *color & 31;
        s32 tr = (*target >> 10) & 31;
        s32 tg = (*target >> 5) & 31;
        s32 tb = *target & 31;

        if (r > tr) {
          r--;
        }
        if (g > tg) {
          g--;
        }
        if (b > tb) {
          b--;
        }
        *color = (r << 10) | (g << 5) | b;
      }
    }
    fade = 0;
    if (frame >= 214 && frame < 246) {
      s32 t = (frame - 214) / 4;

      red = t / 2;
      green = 0;
      blue = t;
      fade = 1;
    }
    if (frame >= 251 && frame < 283) {
      s32 t = (frame - 251) / 2;

      red = 4 - t / 4;
      green = -(t / 2);
      blue = 8 - t / 2;
      fade = 1;
    }
    if (frame >= 348 && frame < 412) {
      red = (frame - 348) / 4;
      green = red / 2 - 8;
      blue = red / 8 - 4;
      fade = 1;
    }
    if (fade == 1) {
      u16 *color = Resource_GetTableEntry((s32)&ResourceId_CrimsonBeastTiles);

      for (i = 0; i != 192; i++, color++) {
        s32 r = (*color & 31) + red;
        s32 g = ((*color >> 5) & 31) + green;
        s32 b = ((*color >> 10) & 31) + blue;

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
        PLTT[0x100 + i] = (b << 10) | (g << 5) | r;
      }
    }
    if (frame >= 132 && frame < 860) {
      if (frame == 132) {
        ResourceObject_ReleaseFar(
            (struct ResourceObjectWork *)work->objects[0]);
        source =
            (u8 *)Resource_GetTableEntry((s32)&ResourceId_CrimsonBeastTiles);
        CpuFill(PLTT + 0x100, 0x180, 0x7fff7fff);
        source += 0x180;
        Resource_DecodeType01(source, work->buffer + 0x2400);
        for (i = 0, objectSlots = TILE_SLOTS; i != 12; i++) {
          struct SpriteObject__f175f74 *object = Func_0815b290(32, 32, 0x80002000, 0);

          object->priority = 3;
          work->objects[i] = object;
          CpuCopy(OBJ_VRAM + objectSlots[object->slot].offset,
                  work->buffer + 0x2400 + i * 0x400, 0x400);
        }
        source = (u8 *)Resource_GetTableEntry((s32)&ResourceId_WhiteWispsTiles);
        CpuCopy(PLTT + 0x1e0, source, 32);
        CpuFill(PLTT + 0x1e0, 32, 0x7fff7fff);
        source += 32;
        Resource_DecodeType01(source, work->buffer + 0x2400);
        {
          struct SpriteObject__f175f74 *object =
              Func_0815b290(64, 64, 0xc0000000, 0xe000);
          struct TileSlot *tileSlots;

          work->objects[12] = object;
          object->priority = 3;
          tileSlots = TILE_SLOTS;
          {
            u8 *vram = OBJ_VRAM;

            /* FAKEMATCH(temporary): vram holds the base, then the destination;
             * as OBJ_VRAM + the slot's offset in the call, 71 of 4354
             * instructions differ in tla-en. */
            vram += tileSlots[object->slot].offset;
            CpuCopy(vram, work->buffer + 0x2400, 0x800);
          }
          object = Func_0815b290(64, 64, 0xc0002000, 0);
          work->objects[13] = object;
          object->priority = 2;
          object->unk17 = 32;
          CpuFill(OBJ_VRAM + tileSlots[object->slot].offset, 0x1000, 0);
        }
        source = (u8 *)Resource_GetTableEntry((s32)&ResourceId_EffectPalette00);
        CpuCopy(PLTT, source, 128);
        Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesD,
                                   work->buffer + 0x2400, 0, 0);
        CpuCopy(glyphs, work->buffer + 0x2400, 0x302);
        Resource_LoadAndDecompress((s32)&ResourceId_CurseSealSheet,
                                   work->buffer + 0x1000, 1, 1);
        posX = 144 << 15;
        posZ = 224 << 14;
      }
      if (frame == 243) {
        source =
            (u8 *)Resource_GetTableEntry((s32)&ResourceId_RedCrescentTiles);
        CpuFill(PLTT + 0x1c0, 64, 0);
        source += 64;
        Resource_DecodeType01(source, gData_02014000);
        for (i = 0; i != 0x1000; i++) {
          if (gData_02014000[i] != 0) {
            gData_02014000[i] += 192;
          }
        }
        for (i = 0; i != 32; i++) {
          REVEAL_BUFFER[0x1000 + i] = i;
        }
        {
          u8 *slot;
          s32 half = 0x1000;

          for (i = 0, slot = REVEAL_BUFFER + 0x1000; i != 32;) {
            u8 *other;
            s32 swap;
            s32 value;
            s32 mask;

            mask = 31;
            half = 0x1000;
            other = (Random16() & mask) + gData_02014000;
            other += half;
            swap = *slot;
            value = *other;
            // These casts change nothing; they keep the loop long enough that
            // the buffer's address is rebuilt in it.
            swap = (u8)swap;
            value = (u8)value;
            i++;
            *slot = value;
            *other = swap;
            slot++;
          }
        }
      }
      {
        s32 scale152[2];

        pos216[3] = 0;
        pos216[1] = 0xff0000;
        if (frame < 160) {
          posX += 0x2000;
          posZ -= 0x4000;
        } else if (frame < 332) {
          posX += 0x2000;
          posZ += 0x1000;
        } else {
          posX += 0x2000;
        }
        scale152[1] = 0x14000;
        scale152[0] = 0x14000;
        scale152[0] = scale152[1] = posZ / 48 + 0x4000;
        pos216[0] = posX + 0x400000;
        pos216[2] = 144 << 16;
        Render_ApplyProjectedPlacementFar(work->objects[12], pos216, scale152,
                                          0);
        scale152[1] = 0x10000;
        scale152[0] = 0x10000;
        for (i = 0; i != 12; i++) {
          pos216[0] = ((i % 4) << 21) + posX;
          pos216[2] = ((i / 4) << 21) + posZ;
          Render_ApplyProjectedPlacementFar(work->objects[i], pos216, scale152,
                                            0);
        }
      }
      if (frame == 200) {
        source = (u8 *)Resource_GetTableEntry((s32)&ResourceId_BoltSheet);
        CpuCopy(PLTT, source, 128);
      }
      if (frame >= 200 && frame < 302) {
        void *texture = Heap_AllocEwram(32);
        struct Model__f175f74 *model = Render_AllocateModelList(1);
        s32 t = frame - 200;
        s32 depth;
        s32 scale;

        depth = t * 4 - 64;
        scale = 100000 - t * 1524;
        if (frame > 253) {
          depth = (254 - frame) * 8;
        }
        if (scale < 13800) {
          scale = 13800;
        }
        if (depth > 0) {
          depth = 0;
        }
        model->depth = depth;
        work->transferMode = 2;
        work->transferValue = 75;
        desc144.a = 7;
        desc144.b = 7;
        desc144.tiles = work->buffer + 0x1000;
        model->kind = 7;
        model->desc = &desc144;
        model->mesh = gData_08199364;
        model->texture = texture;
        if (frame <= 282) {
          Render_ResetTransformState();
          Func_080151e4(scale * 2, scale * 2, scale * 2);
          Func_08015160(-0x100000, -0x80000, 0);
          SceneTransform_ApplyPitch(0xe000);
          Func_080150e4(0x4000 - t * 256);
          SceneTransform_ApplyYaw(-t << 10);
          Render_TransformVertices(gData_08199210, texture, 4);
          Render_DrawModelList(model);
        }
        if (frame > 231) {
          t = frame - 232;
          depth = t * 2 - 64;
          scale = 100000 - t * 1200;
          if (depth < -64) {
            depth = -64;
          }
          if (depth > 0) {
            depth = 0;
          }
          desc144.a = 6;
          desc144.b = 6;
          model->depth = depth;
          desc144.tiles = work->buffer;
          model->desc = &desc144;
          model->mesh = gData_08199340;
          model->kind = 7;
          model->texture = texture;
          Render_ResetTransformState();
          Func_08015160(-0x80000, -0x80000, 0);
          Func_080151e4(scale, scale, scale);
          SceneTransform_ApplyPitch(0xc000);
          SceneTransform_ApplyYaw(-t << 10);
          Render_TransformVertices(gData_08199210, texture, 4);
          Render_DrawModelList(model);
        }
        Heap_Release(model);
        Heap_Release(texture);
      }
      if (frame >= 244 && frame < 310) {
        struct ScalePair scale136 = Data_08196eb8;

        if (frame < 260) {
          u8 *low = REVEAL_BUFFER;
          u8 *high = REVEAL_BUFFER + 1;
          /* FAKEMATCH(temporary): orderCopy, a copy of order set again in the
           * loop; as order itself, 20 of 4354 instructions differ in tla-en. */
          u8 *orderCopy = order;

          for (i = 0; i != 64; i++) {
            s32 column = (orderCopy = order)[0x1000] * 2;
            s32 row = i * 64;
            s32 pixel = row + column;
            u8 *lowByte = low + pixel;
            u8 *highByte = high + pixel;
            s32 texel = *lowByte | (*highByte << 8);

            *(u16 *)(OBJ_VRAM +
                     (row + TILE_SLOTS[work->objects[13]->slot].offset) +
                     column) = texel;
            column = orderCopy[0x1001] * 2;
            pixel = row + column;
            lowByte = low + pixel;
            highByte = high + pixel;
            texel = *lowByte | (*highByte << 8);
            *(u16 *)(OBJ_VRAM +
                     (row + TILE_SLOTS[work->objects[13]->slot].offset) +
                     column) = texel;
          }
        }
        scale136.v[1] = scale136.v[0] = (155 << 12) - (frame << 11);
        pos200[3] = 0;
        pos200[1] = 0xff0000;
        pos200[0] = 184 << 15;
        pos200[2] = 144 << 15;
        Render_ApplyProjectedPlacementFar(work->objects[13], pos200, scale136.v,
                                          0);
        {
          u16 *target =
              Resource_GetTableEntry((s32)&ResourceId_RedCrescentTiles);
          u16 *color = PLTT + 0x1c0;

          for (i = 0; i != 32; i++, target++, color++) {
            s32 r = (*color >> 10) & 31;
            s32 g = (*color >> 5) & 31;
            s32 b = *color & 31;
            s32 tr = (*target >> 10) & 31;
            s32 tg = (*target >> 5) & 31;
            s32 tb = *target & 31;

            if (r < tr) {
              r++;
            }
            if (g < tg) {
              g++;
            }
            if (b < tb) {
              b++;
            }
            *color = (r << 10) | (g << 5) | b;
          }
        }
      }
      if (frame > 301) {
        if (frame == 302) {
          CpuFill(canvas, 0x3c00, 0x3f3f3f3f);
          CpuFill(PLTT + 0x1c0, 64, 0x7fff7fff);
          CpuCopy(PLTT + 0x60,
                  Resource_GetTableEntry((s32)&ResourceId_AerialViewBackdrop),
                  0x100);
          source = (u8 *)Resource_GetTableEntry((s32)&ResourceId_BoltSheet);
          CpuCopy(PLTT, source, 128);
          Resource_LoadAndDecompress((s32)&ResourceId_Data0C2,
                                     work->buffer + 0x1000, 0, 0);
          Graphics_LoadGreyRamp();
        }
        if (frame > 303) {
          u16 *target =
              Resource_GetTableEntry((s32)&ResourceId_AerialViewBackdrop);
          u16 *color = PLTT + 0x60;

          for (i = 0; i != 128; i++, target++, color++) {
            s32 r = (*color >> 10) & 31;
            s32 g = (*color >> 5) & 31;
            s32 b = *color & 31;
            s32 tr = ((*target >> 10) & 31) - 20;
            s32 tg = ((*target >> 5) & 31) - 20;
            s32 tb = (*target & 31) + 8;

            if (tr < 0) {
              tr = 0;
            }
            if (tg < 0) {
              tg = 0;
            }
            if (tb > 31) {
              tb = 31;
            }
            if (r > tr) {
              r--;
            }
            if (g > tg) {
              g--;
            }
            if (b > tb) {
              b--;
            }
            *color = (r << 10) | (g << 5) | b;
          }
        }
        if (frame > 301) {
          s32 k;
          s32 x;
          s32 depth2;
          s32 size;
          void *texture = Heap_AllocEwram(32);
          struct Model__f175f74 *model = Render_AllocateModelList(1);

          desc128.a = 7;
          desc128.b = 7;
          desc128.tiles = work->buffer + 0x1000;
          model->kind = 7;
          model->desc = &desc128;
          model->mesh = gData_08199364;
          model->texture = texture;
          for (k = 0; k != 3; k++) {
            x = frame - 302 - k * 8;
            if (x >= 0) {
              depth2 = x * 4 - 16;
              size = 140000 - x * 6192;
              if (size < 0) {
                size = 0;
              }
              if (depth2 > 0) {
                depth2 = 0;
              }
              model->depth = depth2;
              Render_ResetTransformState();
              Func_08015160(-0x80000, -0x80000, 0);
              Func_080151e4(size / 2, size, size / 2);
              if (k == 0) {
                Func_080150e4(0xf000);
              } else if (k == 1) {
                Func_080150e4(0x2000);
              } else {
                Func_080150e4(0xe000);
              }
              SceneTransform_ApplyPitch(0x800);
              Render_TransformVertices(gData_08199210, texture, 4);
              Render_DrawModelList(model);
            }
          }
          Heap_Release(model);
          Heap_Release(texture);
        }
      }
      if (frame == 330) {
        CpuFill(PLTT + 0x1c0, 64, 0x7c1f7c1f);
      }
      if (frame >= 330 && frame < 351) {
        struct ScalePair scale120 = Data_08196ec0;
        s32 dx = 0;
        s32 dz = 0;

        if (frame > 346) {
          dx = 347 - frame;
          dz = (frame - 347) / 4;
        }
        scale120.v[0] = scale120.v[1] = (frame << 11) - 0xa5000;
        pos184[3] = 0;
        pos184[1] = 0xff0000;
        pos184[0] = (dx << 16) + (184 << 15);
        pos184[2] = (dz << 16) + (144 << 15);
        Render_ApplyProjectedPlacementFar(work->objects[13], pos184, scale120.v,
                                          0);
      }
      if (frame == 347) {
        work->transferMode = 1;
        work->transferValue = 0;
        REG_BLDCNT = 0;
        {
          u16 *entry = PLTT + 1;
          for (i = 0; i != 63; i++, entry++) {
            s32 r = 31 - i;
            s32 b = r;

            if (r < 0) {
              r = 0;
            }
            if (b < 0) {
              b = 0;
            }
            *entry = (b << 10) | r;
          }
        }
      }
      if (frame > 346) {
        s32 t = frame * 4 - 1380;
        s32 height = 54;

        if (frame > 349) {
          height = ((-(Trig_Sin((frame << 10) - 0x57800) * 54)) >> 16) + 54;
        }
        if (frame == 360) {
          work->transferMode = 0;
        }
        if (frame <= 371) {
          Func_0815b434(work->buffer + 0x1000, t, 0x10000);
        } else {
          t = 104;
        }
        HeapSlot_Release(188);
        Func_0818caa8(work->buffer + 0x1000, height, 58, t);
        BattleEffect_LoadBlitter(188, 3);
        blit[1] = SYSTEM_WORK->blitBc;
      }
      for (j = 0; j != 2; j++) {
        s32 t = frame - 357 - j * 19;

        if (t == 0) {
          CpuFill(PLTT + 0x60, 0x100, 0x7fff7fff);
        }
        if (t >= 0 && t < 160) {
          u16 *target =
              Resource_GetTableEntry((s32)&ResourceId_AerialViewBackdrop);
          u16 *color = PLTT + 0x60;

          for (i = 0; i != 128; i++, target++, color++) {
            s32 r = (*color >> 10) & 31;
            s32 g = (*color >> 5) & 31;
            s32 b = *color & 31;
            s32 tr;
            s32 tg;
            s32 tb;

            if (t < 8) {
              tr = (*target >> 10) & 31;
              tg = (*target >> 5) & 31;
              tb = *target & 31;
            } else {
              tr = ((*target >> 10) & 31) - 16;
              tg = ((*target >> 5) & 31) - 16;
              tb = (*target & 31) + 8;
            }
            if (tr < 0) {
              tr = 0;
            }
            if (tg < 0) {
              tg = 0;
            }
            if (tb > 31) {
              tb = 31;
            }
            if (r > tr + 1) {
              r -= 2;
            }
            if (g > tg + 1) {
              g -= 2;
            }
            if (b > tb + 1) {
              b -= 2;
            }
            *color = (r << 10) | (g << 5) | b;
          }
        }
      }
    }
    if (t56 >= 0 && t56 < 24) {
      s32 radius = frame * 2 - 104;
      s32 row = (radius - 10) / 6;
      s32 x;
      s32 y;
      s32 w;
      s32 h;
      s32 cosine;

      if (row < 0) {
        row = 0;
      }
      w = 4;
      for (i = 0, h = 8; i != 256; i++) {
        x = (Trig_Sin(i << 8) * radius * 2) >> 16;
        cosine = Trig_Cos(i << 8);
        y = ((cosine * radius) >> 16) + 68;
        x += 66;
        routine[0](canvas, glyphs + row * 32, x - w / 2, y, w, h);
      }
    }
    if (frame >= 108 && frame < 132) {
      u16 *color;

      for (i = 0, color = PLTT + 0x60; i != 128; i++, color++) {
        s32 r = *color & 31;
        s32 g = (*color >> 5) & 31;
        s32 b = (*color >> 10) & 31;

        g--;
        if (g < 0) {
          r--;
        }
        if (r > 31) {
          r = 31;
        } else if (r < 0) {
          r = 0;
        }
        if (g > 31) {
          g = 31;
        } else if (g < 0) {
          g = 0;
        }
        if (b > 31) {
          b = 31;
        } else if (b < 0) {
          b = 0;
        }
        *color = (b << 10) | (g << 5) | r;
      }
    }
    if (t56 >= 0 && t56 < 36) {
      if (frame == 56) {
        s32 x;
        s32 y;

        work->transferMode = 3;
        work->transferValue = 0x04040404;
        source = (u8 *)Resource_GetTableEntry((s32)&ResourceId_Data0B5);
        source += 32;
        Resource_DecodeType01(source, work->buffer + 0x2400);
        for (y = 0; y != 80; y++) {
          for (x = 0; x != 184; x += 2) {
            u16 *dst =
                (u16 *)(BG_VRAM_A900 +
                        ((((y / 8) * 32 + x / 8) * 8 + (y & 7)) * 8 + (x & 7)));
            u8 hi = *dst >> 8;
            u8 lo = *dst;

            if (work->buffer[0x2400 + y * 184 + x + 1] != 0) {
              hi = work->buffer[0x2400 + y * 184 + x + 1] - 32;
            }
            if (work->buffer[0x2400 + y * 184 + x] != 0) {
              lo = work->buffer[0x2400 + y * 184 + x] - 32;
            }
            *dst = (hi << 8) | lo;
          }
        }
      }
      if (frame < 88) {
        u16 *color = Resource_GetTableEntry((s32)&ResourceId_Data0B5);
        u16 *entry = PLTT + 0xe0;

        for (i = 0; i != 16; i++, color++, entry++) {
          s32 add = ((Trig_Cos(t56 << 9) * 12) >> 16) + 16;
          s32 r = (*color & 31) + add;
          s32 g = ((*color >> 5) & 31) + add;
          s32 b = ((*color >> 10) & 31) + add;

          if (r > 31) {
            r = 31;
          }
          if (g > 31) {
            g = 31;
          }
          if (b > 31) {
            b = 31;
          }
          *entry = (b << 10) | (g << 5) | r;
        }
      }
    }
    if (frame == 54) {
      CpuFill(canvas, 0x3c00, 0x20202020);
    }
    if (frame == 55) {
      CpuFill(canvas, 0x3c00, 0x3f3f3f3f);
    }
    if (frame < 132) {
      s32 scale112;
      s32 depth;
      s32 rise = -52576;
      s32 base;
      s32 quarter;
      s32 twice;
      void *texture = Heap_AllocEwram(32);
      struct Model__f175f74 *model = Render_AllocateModelList(1);

      work->transferMode = 2;
      work->transferValue = 50;
      twice = frame * 2;
      quarter = frame * 381;
      depth = twice - 64;
      base = quarter * 4;
      scale112 = 115536 - base;
      if (frame > 55) {
        scale112 = base + rise;
      }
      if (depth >= -8) {
        depth = -8;
      }
      if (frame > 95) {
        depth = 184 - frame * 2;
        model->depth = depth;
      }
      desc112.a = 6;
      desc112.b = 6;
      desc112.tiles = work->buffer;
      model->kind = 7;
      model->desc = &desc112;
      model->mesh = gData_08199340;
      model->texture = texture;
      if (depth > -48) {
        model->depth = depth;
        Render_ResetTransformState();
        Func_08015160(0x40000, 0, 0);
        Func_080151e4(scale112, scale112, scale112);
        SceneTransform_ApplyPitch(0x2000);
        if (frame > 55) {
          SceneTransform_ApplyYaw(frame << 10);
        } else {
          SceneTransform_ApplyYaw(-frame << 10);
        }
        Render_TransformVertices(gData_08199210, texture, 4);
        Render_DrawModelList(model);
      }
      if (frame > 95) {
        depth = 568 - frame * 6;
        model->depth = depth;
      }
      if (depth > -48) {
        Render_ResetTransformState();
        Func_08015160(0x40000, -0x100000, 0);
        Func_0801521c(-0xa710);
        SceneTransform_ApplyPitch(0x2000);
        SceneTransform_ApplyYaw(-frame << 9);
        Render_TransformVertices(gData_08199210, texture, 4);
        Render_DrawModelList(model);
      }
      if (frame > 55) {
        s32 last = frame * 2 - 208;

        desc112.tiles = work->buffer + 0x1000;
        if (last >= -32) {
          last = -32;
        }
        model->depth = last;
        Render_ResetTransformState();
        Func_08015160(0x20000, -0x200000, 0);
        Func_0801521c(0x58f0);
        SceneTransform_ApplyPitch(0x2000);
        SceneTransform_ApplyYaw(frame << 10);
        Render_TransformVertices(gData_08199210, texture, 4);
        Render_DrawModelList(model);
        /* FAKEMATCH(dead): twice raised to frame though nothing reads it again;
         * flow deletes the store and its test; as no test, 290 of 4354
         * instructions differ in tla-en. */
        if (frame > twice) {
          twice = frame;
        }
        if (frame > -48) {
          Render_ResetTransformState();
          Func_08015160(0x20000, -0x300000, 0);
          Func_0801521c(-0x31e0);
          SceneTransform_ApplyPitch(0x2000);
          SceneTransform_ApplyYaw(-frame << 10);
          Render_TransformVertices(gData_08199210, texture, 4);
          Render_DrawModelList(model);
        }
      }
      Heap_Release(model);
      Heap_Release(texture);
      while (work->frameSync, work->frameSync <= 1) {
      }
    }
    work->frameDone = 1;
    wait(1);
  }

  Sound_PlayFar(162);
  if (frame < 132) {
    ResourceObject_ReleaseFar((struct ResourceObjectWork *)work->objects[0]);
  } else {
    for (i = 0; i != 14; i++) {
      ResourceObject_ReleaseFar((struct ResourceObjectWork *)work->objects[i]);
    }
  }
  for (i = 0; i != 48; i++) {
    ResourceObject_ReleaseFar((struct ResourceObjectWork *)SPRITES[i]);
  }
  HeapSlot_Release(188);
  HeapSlot_Release(104);
  Palette_LoadBasicObjColors();
  REG_DISPCNT = 0x141;
  wait(1);
  scene->unk867 = 0;
  BattleBackground_LoadFar(1, scene->unk670, 24);
  // An empty once-only loop before the canvas fill, so its entry loads before
  // the size; without it, 2 of 4354 instructions differ in tla-en.
  do {
  } while (0);
  CpuFill(canvas, 0x4000, 0);
  // An empty once-only loop before the BG fill, so its address loads before
  // the size; without it, 10 of 4354 instructions differ in tla-en.
  do {
  } while (0);
  CpuFill((void *)0x06004000, 0x4000, 0);
  gCameraSceneParameters[4] = 120;
  unit30 = SYSTEM_WORK->unit30;
  shake = 0;
  savedUnit30 = unit30->unk36;
  unit30->unk36 = shake;
  work->transferMode = 1;
  work->transferValue = 0;
  work->transferMode = 2;
  work->transferValue = 50;
  REG_BLDALPHA = 0x410;
  REG_BG2X = 0;
  REG_BG2CNT = 0x786;
  for (i = 1; i != 64; i++) {
    s32 q = i * 2 / 3;
    s32 r = 31 - q;
    s32 g = 8 - i / 2;
    s32 b = 27 - q;

    if (r < 0) {
      r = 0;
    }
    if (g < 0) {
      g = 0;
    }
    if (b < 0) {
      b = 0;
    }
    PLTT[i] = (b << 10) | (g << 5) | r;
  }
  arg->actors[arg->count] = 0xff;
  BattleActor_SpawnObjectsForListFar((struct Local264 *)arg->actors, 0);
  Func_0815b434(work->buffer, 56, 0x10000);
  Func_0815b434(work->buffer + 0x1000, 16, 0x10000);
  CpuFill(work->buffer + 0x2000, 0x1000, 0x3f3f3f3f);
  Resource_LoadAndDecompress((s32)&ResourceId_Data0C3, work->buffer + 0x3000, 0,
                             0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, glyphs, 0, 0);
  {
    s32 limit;

    i = 0;
    limit = 0x302;
  empty:
    i += 35;
    if (i != limit) {
      goto empty;
    }
  }
  for (i = 0; i != 0x1000; i++) {
    if (work->buffer[0x3000 + i] != 0) {
      work->buffer[0x3000 + i] = 63;
    }
  }
  IO_WRITE_QUEUE_PUSH(0x04000000, 0x7741, 0x20000);
  for (i = 0; i != 16; i++) {
    work->drops[i].timer = -i;
  }
  for (i = 0; i != 128; i++) {
    PARTICLES[i].timer = -1;
  }
  for (i = 0; i != arg->count; i++) {
    struct ActorPos *actor = *GetBattleObjectSlotFar__f175f74(arg->actors[i]);

    work->homes[i].x = actor->x;
    work->homes[i].y = actor->y;
    work->homes[i].z = actor->z;
  }

  width = 56;
  twenty = 20;
  frame = 0;
  do {
    if ((gInput.repeat & 3) && frame < 120) {
      frame = 120;
    }
    if (frame == 0) {
      Sound_PlayFar(162);
      work->wave = 160;
      shake = 0;
      for (i = 0; i != arg->count; i++) {
        work->members[i].timer = -1;
      }
      for (i = 0; i != arg->count; i++) {
        struct ActorPos *actor = *GetBattleObjectSlotFar__f175f74(arg->actors[i]);

        Func_0814cd48(arg->actors[i], 0, 5, -1, -1);
        actor->x = -(i + 4) * 0x800000;
        actor->z = 0;
        actor->y = 0x140000;
        actor->unk48 = 0;
      }
    }
    if (frame == 120) {
      work->transferMode = 1;
      work->transferValue = 0;
      ColorBuffer_ScaleFar(scene->palette, PLTT + 0x60, 0x99a0, 128);
      for (i = 0; i != arg->count; i++) {
        struct ActorPos *actor = *GetBattleObjectSlotFar__f175f74(arg->actors[i]);

        actor->x = 0;
        actor->y = 0x7c0000;
        actor->z = 0;
        actor->unk48 = 0;
      }
    }
    if (frame == 160) {
      Func_081180e8(134);
    }
    if (frame >= 140) {
      if (frame == 140) {
        for (i = 0; i != 32; i++) {
          s32 speed = (Random16() & 511) + 32;
          s32 angle = Random16();

          PARTICLES[96 + i].x = 240 << 14;
          PARTICLES[96 + i].y = 240 << 14;
          PARTICLES[96 + i].vx = (Trig_Sin(angle) * speed) >> 5;
          PARTICLES[96 + i].vy = (Trig_Cos(angle) * speed) >> 5;
          PARTICLES[96 + i].timer = 0;
        }
        for (i = 0; i != 128; i++) {
          s32 speed = (Random16() & 255) + 32;
          s32 angle = Random16();

          PARTICLES[128 + i].x = 240 << 14;
          PARTICLES[128 + i].y = 240 << 14;
          PARTICLES[128 + i].vx = (Trig_Sin(angle) * speed) >> 6;
          PARTICLES[128 + i].vy = (Trig_Cos(angle) * speed) >> 6;
          PARTICLES[128 + i].timer = (Random16() & 15) + 16;
        }
        Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet,
                                   SPRITE_BUFFER, 1, 0);
        Sound_PlayFar(144);
        CpuFill(canvas, 0x3c00, 0x3f3f3f3f);
        REG_BG2CNT = 0x784;
        work->transferMode = 2;
        work->transferValue = 50;
        for (i = 0; i != arg->count; i++) {
          struct ActorPos *actor = *GetBattleObjectSlotFar__f175f74(arg->actors[i]);

          actor->x = 0;
          actor->y = 0x200000;
          actor->z = work->homes[i].z;
        }
        for (i = 0; i != arg->count; i++) {
          s32 spin = Random16();
          s32 delay = (Random16() & 31) + 120;
          s32 speed = spin * 2 + 0x20000;

          BattleEffect_StartActorMotion(arg->actors[i], 1, 0x40000, 0x80000,
                                        speed, delay);
          Func_0814cd48(arg->actors[i], 7, 5, i, 32);
        }
        work->unk77a8 = 3;
      }
      BattleEffect_LoadBlitter(104, 31);
      routine[0] = SYSTEM_WORK->blit;
      for (i = 0; i != 16; i++) {
        struct Particle__f175f74 *p = &PARTICLES[96 + i];

        if (p->timer >= 0 && p->timer < 24) {
          s32 row = (p->timer / 4) * 2048;
          u8 *buffer = SPRITE_BUFFER;
          s32 x = (p->x >> 16) - 16;

          row += (s32)buffer;
          buffer = (u8 *)row;
          routine[0](canvas, buffer, x, (p->y >> 16) - 32, 32, 64);
          BattleEffectKernels_IntegrateVector2(p, 60, -0x2000);
        }
        p->timer++;
      }
      for (i = 0; i != 128; i++) {
        struct Particle__f175f74 *p = &PARTICLES[128 + i];

        if (p->timer >= 0) {
          s32 size = (p->timer >> 3) + 2;
          s32 k;

          for (k = 0; k != 4; k++) {
            routine[0](canvas, glyphs + gData_08197410[size - 1],
                       (p->x >> 16) - size / 2, (p->y >> 16) - size, size,
                       size * 2);
            BattleEffectKernels_IntegrateVector2(p, 63, 0);
            if (p->y > 224 << 15) {
              p->vy = -p->vy;
            }
          }
          p->timer--;
        }
      }
      HeapSlot_Release(104);
    }
    if (frame < 120) {
      for (i = 0; i != 3; i++) {
        if (frame == gData_08198c67[i]) {
          CpuFill(PLTT + 0x60, 0x100, 0x7fff7fff);
        }
        if (frame >= gData_08198c67[i] + 1 && frame < gData_08198c67[i] + 16) {
          u16 *target = SYSTEM_WORK->scene->palette;
          u16 *color = PLTT + 0x60;

          for (j = 0; j != 128; j++) {
            s32 tr = (target[j] & 31) - twenty / 2;
            s32 tg = ((target[j] >> 5) & 31) - 20;
            s32 tb = ((target[j] >> 10) & 31) - twenty / 2;
            s32 r = color[j] & 31;
            s32 g = (color[j] >> 5) & 31;
            s32 b = (color[j] >> 10) & 31;

            if (tr < 0) {
              tr = 0;
            }
            if (tg < 0) {
              tg = 0;
            }
            if (tb < 0) {
              tb = 0;
            }
            if (tr < r - 1) {
              r -= 2;
            }
            if (tg < g - 1) {
              g -= 2;
            }
            if (tb < b - 1) {
              b -= 2;
            }
            color[j] = (b << 10) | (g << 5) | r;
          }
        }
      }
    }
    if (frame > 120) {
      if (frame == 121) {
        gBgScroll[1].x = savedWidth;
        unit88->unk10 = 0;
        unit30->unk36 = savedUnit30;
      }
      if (frame < 148) {
        s32 step = 8 - (frame - 120) / 6;
        s32 wave = work->wave;

        work->wave = wave - step;
        if (wave > 0) {
          Func_0815b434(work->buffer, wave, 0x10000);
          Func_0818caa8(work->buffer, 60, 60, wave);
        }
      }
    } else {
      if (frame >= 4 && frame < 26) {
        shake += 4;
      }
      if (frame > 107) {
        shake -= 8;
        if (shake < 0) {
          shake = 0;
        }
      }
      for (i = 0; i != arg->count; i++) {
        struct ActorPos *actor = *GetBattleObjectSlotFar__f175f74(arg->actors[i]);
        struct Particle__f175f74 *member = &work->members[i];
        s32 landed = 0;

        if (actor->y < 0x640000) {
          if (member->timer == -1) {
            actor->x += 0xa0000;
          } else {
            actor->x += 0x20000;
          }
          if (member->timer == -1) {
            if (actor->x > 0x300000) {
              landed = 1;
            }
            if (landed == 1) {
              member->timer = 0;
              EffectPosition_ProjectScaledBattler(arg->actors[i], member);
              Sound_PlayFar(103);
              for (j = 0; j != 16; j++) {
                s32 angle = Random16() & 0x7fff;
                s32 speed;
                struct Particle__f175f74 *p = &PARTICLES[i * 16 + j];

                speed = (Random16() & 255) + 256;

                p->x = member->x << 15;
                p->y = member->y << 16;
                p->vx = -(Trig_Sin(angle) * speed) >> 7;
                p->vy = (Trig_Cos(angle) * speed) >> 6;
                p->timer = (Random16() & 15) + 16;
              }
            }
          }
        }
        if (member->timer == 2 || member->timer == 7) {
          Func_0814cd48(arg->actors[i], 7, 5, i, 32);
        }
        /* FAKEMATCH(dead): landed set to 2 though nothing reads it again; flow
         * deletes the store and its test; as no test, 402 of 4354 instructions
         * differ in tla-en. */
        if (actor == 0) {
          landed = 2;
        }
        if (member->timer == 6) {
          Func_0814cd48(arg->actors[i], 15, -1, i, 32);
        }
        if (member->timer == 8) {
          actor->x = 0;
          actor->y = 148 << 16;
          actor->z = 0;
        }
      }
      if (frame >= 0) {
        gBgScroll[1].x += 0xfff4;
        BattleEffect_LoadBlitter(104, 27);
        {
          struct Particle__f175f74 *column;
          s32 x;
          s32 y;
          s32 y2;
          for (i = 0, y2 = -26, y = 0, x = shake, phase = frame << 10,
              column = work->columns, halfCell = width / 2;
               i != 4;
               y2 += 30, y += 30, x += 8, phase += 0x2000, column++, i++) {
            column->x = x + ((Trig_Sin(phase) * 4) >> 16);
            column->y = y;
            if (i == 3) {
              column->x -= 8;
              column->y = y2;
              HeapSlot_Release(104);
              BattleEffect_LoadBlitter(104, 19);
            }
            SYSTEM_WORK->blit(canvas, work->buffer, column->x - halfCell,
                              column->y, halfCell, width);
          }
        }
        HeapSlot_Release(104);
        for (i = 0; i != 12; i++) {
          struct Particle__f175f74 *drop = &work->drops[i];

          if (++drop->timer == 4) {
            drop->timer = 0;
          }
          if (drop->timer == 0) {
            drop->x = (Random16() % 24 + shake - i * 2) << 16;
            drop->y = (160 << 15) + i * (192 << 10);
            drop->vx = -(Random16() & 0xffff) - 0x4000;
            drop->vy = -0x10000;
          }
          if (drop->timer >= 0) {
            Func_0818caa8(work->buffer + 0x1000, drop->x >> 16, drop->y >> 16,
                          16);
            drop->x += drop->vx;
            drop->y += drop->vy;
          }
        }
        BattleEffect_LoadBlitter(104, 31);
        routine[0] = SYSTEM_WORK->blit;
        for (i = 0; i != 4; i++) {
          s32 width = 120 - work->columns[i].x;

          if (width > 0) {
            routine[0](canvas, work->buffer + 0x2000, work->columns[i].x,
                       i * 30, width, 30);
          }
        }
        HeapSlot_Release(104);
        BattleEffect_LoadBlitter(104, 47);
        routine[0] = SYSTEM_WORK->blit;
        for (i = 0; i != 96; i++) {
          struct Particle__f175f74 *p = &PARTICLES[i];

          if (p->timer >= 0) {
            s32 size = (p->timer >> 3) + 2;

            routine[0](canvas, glyphs + gData_08197410[size - 1],
                       (p->x >> 16) - size / 2, (p->y >> 16) - size, size,
                       size * 2);
            BattleEffectKernels_IntegrateVector2(p, 60, 0);
            p->timer--;
          }
        }
        HeapSlot_Release(104);
      }
      {
        void *texture = Heap_AllocEwram(32);
        struct Model__f175f74 *model = Render_AllocateModelList(1);

        desc104.a = 6;
        desc104.b = 6;
        desc104.tiles = work->buffer + 0x3000;
        model->kind = 9;
        model->mesh = gData_08199340;
        model->desc = &desc104;
        model->texture = texture;
        for (i = 0; i != arg->count; i++) {
          struct Particle__f175f74 *member = &work->members[i];
          s32 timer = member->timer;

          if (timer >= 0 && timer < 18) {
            s32 step = timer * 3;
            s32 depth = step - 64;
            s32 size = step * 1016 + 0x800;

            member->timer = timer + 1;
            model->depth = depth;
            Render_ResetTransformState();
            Func_08015160((member->x / 2 - 56) << 16, (member->y - 56) << 16,
                          0);
            Func_080151e4(size / 3, size, size);
            SceneTransform_ApplyPitch(0xc000);
            Render_TransformVertices(gData_08199210, texture, 4);
            Render_DrawModelList(model);
          }
          Heap_Release(model);
          Heap_Release(texture);
        }
      }
    }
    Camera_ApplyVerticalShake(8);
    BattleEffect_CountDownBattlerTints();
    work->frameDone = 1;
    wait(1);
    frame++;
  } while (frame != 180);
  for (i = 0; i != arg->count; i++) {
  }
  HeapSlot_Release(188);
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}


/// Two triangles of a strip: three vertex indices and a texture coordinate
/// pair for each corner.
struct StripTriangle {
  u8 vertex[3];
  u8 unk03;
  u8 uv[3][2];
  u8 unk0a[2];
};

struct StripQuad {
  struct StripTriangle upper;
  struct StripTriangle lower;
};

/// Builds count quads of a vertex strip two vertices apart, each width texels
/// wide and height texels tall, and ends the list with an empty triangle.
void Render_BuildQuadStrip(struct StripQuad *quad, s32 width, s32 height,
                           s32 count) {
  s32 i;

  for (i = 0; i != count; i++) {
    quad[i].upper.uv[0][1] = 1;
    quad[i].upper.uv[2][1] = 1;
    quad[i].lower.uv[2][1] = 1;
    quad[i].upper.vertex[0] = i * 2;
    quad[i].upper.uv[0][0] = i * width;
    quad[i].upper.uv[1][0] = i * width;
    quad[i].upper.uv[2][0] = i * width + width - 1;
    quad[i].lower.uv[0][0] = i * width;
    quad[i].lower.uv[1][0] = i * width + width - 1;
    quad[i].lower.uv[2][0] = i * width + width - 1;
    quad[i].upper.vertex[1] = i * 2 + 1;
    quad[i].upper.vertex[2] = i * 2 + 2;
    quad[i].lower.vertex[0] = i * 2 + 1;
    quad[i].lower.vertex[2] = i * 2 + 2;
    quad[i].lower.vertex[1] = i * 2 + 3;
    quad[i].upper.uv[1][1] = height - 1;
    quad[i].lower.uv[0][1] = height - 1;
    quad[i].lower.uv[1][1] = height - 1;
  }
  SET_VECTOR(quad[count].upper.vertex, 0, 0, 0);
}
#undef SYSTEM_WORK
#undef REGISTER_QUEUE
#undef TILE_SLOTS
#undef SPRITES
#undef SPRITE_BUFFER
#undef OAM_BUFFER
#undef REVEAL_BUFFER
#undef PARTICLES
#undef OBJ_VRAM
#undef BG_VRAM_A900
#undef PLTT

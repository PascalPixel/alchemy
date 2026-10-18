// Draft of Func_0815f16c, decompiled from the listing: differ 8410 of 7666
// (draft 7672), control flow and calls as the listing except one shared
// FetchRectangleBlitters tail: jump2 merges kind 44 and 61 into later copies
// (86 and 88) there but into kind 61 here, because the copies load action
// through different registers (an allocation artifact, 516 vs 515 calls). Seven
// of its data tables are read as writable, as the listing reloads them.
#include "mem.h"
#include "types.h"

#include "dma.h"

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
  u8 unk7788[0x20];
  s32 unk77a8;
  u8 unk77ac[8];
  s32 unk77b4;
  s32 unk77b8;
  u8 unk77bc[0x12c];
  s32 unk78e8;
};

struct SummonAction {
  s32 id;
  s32 side;
  s32 target;
  u8 unk0c[0xc];
  s32 unk18;
  s32 unk1c;
  u8 unk20[4];
  s16 actor;
};

struct BattleObject {
  u8 unk00[8];
  s32 x;
  s32 y;
  s32 z;
  u8 unk14[0x10];
  s32 unk24;
  s32 unk28;
  s32 unk2c;
  u8 unk30[4];
  s32 unk34;
  u8 unk38[0x10];
  s32 unk48;
};

typedef void (*DrawFunc)(void *, const void *, s32, s32, s32, s32);

struct DrawFuncs {
  DrawFunc draw[2];
};

struct Texture {
  u8 width;
  u8 height;
  u16 unk2;
  void *pixels;
};

struct Model {
  s32 kind;
  s32 unk4;
  const void *mesh;
  void *matrix;
  struct Texture *texture;
  s32 fade;
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

#define IWRAM ((struct IwramPointers *)0x03000000)

extern u8 Value_000000b4;
extern u8 Value_000000b9;
extern u8 Value_000000c1;
extern u8 Value_000000c2;
extern u8 Value_000000d9;
extern u8 Value_000000da;
extern u8 Value_000000dd;
extern u8 Value_000000e6;
extern u8 Value_000000f0;
extern u8 Value_000000f1;
extern u8 Value_00000105;
extern u8 Value_00000115;
extern u8 Value_00000129;
extern u8 Value_00000130;
extern u8 Value_00000134;
extern u8 Value_0000013a;
extern u8 Value_0000013e;
extern u8 Value_00000147;
extern u8 Value_00000148;
extern u8 Value_0000014d;
extern u8 Value_00000150;
extern u8 Value_00000151;
extern u8 Value_00000153;
extern u8 Value_00000155;
extern u8 Value_00000157;
extern u8 Value_00000159;
extern u8 Value_0000015c;
extern u8 Value_00000161;
extern u8 Value_00000163;
extern u8 Value_00000166;
extern u8 Value_00000167;
extern u8 Value_00000169;
extern u8 Value_0000016d;
extern u8 Value_0000016f;
extern u8 Value_00000170;
extern u8 Value_00000171;
extern u8 Value_00000172;
extern u8 Value_00000178;
extern u8 Value_0000017c;
extern u8 Value_0000017d;
extern u8 Value_0000017f;
extern u8 Value_00000182;
extern u8 Value_00000184;
extern u8 Value_00000187;
extern u8 Value_00000188;
extern u8 Value_0000018a;
extern u8 Value_00000192;
extern u8 Value_00000193;

extern const u16 gData_08197410[];
extern const u8 gData_08197467[];
extern const u8 gData_0819746d[];
extern const u8 gData_08197473[];
extern const u16 gData_0819747a[];
extern const u16 gData_08197486[];
extern u8 gData_08197492[];
extern u8 gData_08197498[];
extern const u8 gData_0819749e[];
extern const u8 gData_081974ad[];
extern const u16 gData_081974bc[];
extern u8 gData_0819750c[];
extern u8 gData_0819751a[];
extern const u8 gData_081977f8[];
extern const u8 gData_0819781a[];
extern const u8 gData_08197826[];
extern const u32 gData_08197834[];
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
extern const u16 gData_08198926[];
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

s32 __divsi3(s32, s32);
s32 __modsi3(s32, s32);
s32 Trig_Cos(s32);
s32 Trig_Sin(s32);
void Runtime_ReleaseHeapBlock(s32);
void MemFree(void *);
void *Resource_GetTableEntry(s32);
void WaitFrames(s32);
void Scheduler_AddOrUpdateCallback(void *, s32);
void Scheduler_RemoveCallback(void *);
s32 Random16(void);
void Render_ResetTransformState(void);
void SceneTransform_ApplyPitch(s32);
void SceneTransform_ApplyYaw(s32);
void Func_080150e4(s32);
void Func_08015160(s32, s32, s32);
void Func_080151e4(s32, s32, s32);
void Func_0801521c(s32);
void Transform_SetLookAt(void *, void *);
void Object_SetMode(struct BattleObject *, s32);
void ObjectDispatch_ApplyValueToChildrenFar(struct BattleObject *, s32);
s32 Battle_GetObjectTableValueFar(s32);
void Func_08118088(s32, s32);
struct BattleObject **GetBattleObjectSlotFar(s32);
void Func_081180e8(s32);
void BattleFxKernels_IntegrateVector3(struct Particle *, s32, s32);
void BattleFxKernels_IntegrateVector2(struct Particle *, s32, s32);
void Func_08143001(void);
void Func_08143489(void);
void ObjectGroup_TickMemberTimers(void);
void BattleFx_BeginCanvasLayer(s32);
void BattleFx_BeginTiledCanvas(s32);
void BattleFx_EndCanvasLayer(void);
void BattleFx_FetchRectangleBlitters(s32, struct DrawFuncs *);
void BattleFx_RunSparkGroups(struct SummonAction *, s32);
void BattleFx_RunMysticFlame(struct SummonAction *);
void Func_0814cd48(s32, s32, s32, s32, s32);
void SummonFx_RunCanvasMode3(struct SummonAction *);
void SummonFx_RunCanvasMode1(struct SummonAction *);
void SummonFx_RunCanvasMode2(struct SummonAction *);
void Func_08152475(void);
void BattleFx_RunProjectileVolley(struct SummonAction *, s32);
s32 Resource_LoadAndDecompress(s32, void *, s32, s32);
void Func_08158ce0(s32, s32);
void Func_0815b434(void *, s32, s32);
void EffectPosition_ProjectPoint(void *, s32 *);
void EffectPosition_ProjectScaledBattler(s32, s32 *);
void BattleFx_StartActorMotion(s32, s32, s32, s32, s32, s32);
void Func_0815f0a0(s32);
void Graphics_PackTileRows(void *, void *, s32, s32);
void Func_0818caa8(void *, s32, s32, s32);
void BattleFx_LoadBlitter(s32, s32);
void Func_08196404(s32, s32, s32, s32, s32);
void Render_TransformVertices(const void *, void *, s32);
struct Model *Render_AllocateModelList(s32);
void Render_DrawModelList(struct Model *);
void Audio_PlayCue(s32);

struct IoRegisters {
  u8 unk00[0x20];
  u16 bg2pa;
  u8 unk22[6];
  s32 bg2x;
  u8 unk2c[0x24];
  u16 bldcnt;
  u16 bldalpha;
};

#define REG ((volatile struct IoRegisters *)0x04000000)
#define REG_BLDCNT (REG->bldcnt)
#define REG_BLDALPHA (REG->bldalpha)
#define REG_BG2PA (REG->bg2pa)
#define REG_BG2X (REG->bg2x)
#define DMA3 ((volatile u32 *)0x040000d4)
#define PLTT ((void *)0x05000000)
#define PLTT_U16 ((volatile u16 *)0x05000000)
#define BG_VRAM ((void *)0x06004000)
#define EWRAM_BUF ((u8 *)0x02010000)
#define SPARKS ((struct Particle *)0x02016000)
#define RINGS ((struct Particle *)0x02016e00)
#define IWRAM_SLOT_68 (IWRAM->slot68)
#define IWRAM_SLOT_BC (IWRAM->slotbc)
#define CLEAR(dst, size) ((void (*)(void *, s32))0x03000258)(dst, size)

void Func_0815f16c(struct SummonAction *action, s32 kind) {
  struct SummonWork *work;
  void *screen;
  s32 frame;
  void *bg;
  void *tiles;
  s32 scrollA;
  s32 scrollB;
  s32 duration;
  s32 save24;
  s32 save28;
  s32 save2c;
  s32 save34;
  s32 save48;
  struct BattleObject *object;
  struct BattleObject *object2;
  struct BattleObject *object3;
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
  s32 i;
  s32 j;
  s32 k;
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
  s32 amp;
  s32 off;
  s32 top;
  s32 scroll;
  s32 life;
  s32 row;
  s32 limit;
  s32 dx;
  s32 dy;
  s32 dz;
  s32 size;
  s32 fade;
  s32 scale;
  void *matrix;
  struct Model *model;
  const u8 *table;
  u8 *src;
  u8 *dst;
  s32 v;
  struct Particle *p;

  work = IWRAM->summon;
  screen = IWRAM->screen;
  bg = IWRAM->bg;
  tiles = IWRAM->tiles;
  scrollA = 0;
  scrollB = 0;

  if (kind == 41 || kind == 38 || kind == 62 || kind == 85 || kind == 86 ||
      kind == 87 || kind == 88)
    BattleFx_BeginTiledCanvas(0x2000);
  else
    BattleFx_BeginCanvasLayer(0);

  REG_BLDALPHA = 0x1010;
  Resource_LoadAndDecompress((s32)&Value_00000134, tiles, 0, 0);
  Resource_LoadAndDecompress((s32)&Value_00000159, work->frames, 1, 0);
  Resource_LoadAndDecompress((s32)&Value_0000015c, EWRAM_BUF, 1, 0);
  Graphics_PackTileRows(EWRAM_BUF, work->frames + 0x5100, 40, 0x120);

  if (kind == 5 || kind == 53 || kind == 78) {
    Resource_LoadAndDecompress((s32)&Value_0000013e, EWRAM_BUF, 1, 0);
  } else if (kind == 42 || kind == 15) {
    Resource_LoadAndDecompress((s32)&Value_0000016d, EWRAM_BUF, 1, 0);
  } else if (kind == 16) {
    Resource_LoadAndDecompress((s32)&Value_000000d9, EWRAM_BUF, 1, 0);
    for (i = 1; i != 8; i++) {
      src = EWRAM_BUF;
      dst = EWRAM_BUF + i * 0x400;
      for (j = 0; j != 0x400; j++) {
        v = *src++;
        if (v != 0) {
          v -= i * 8;
          if (v <= 0)
            v = 1;
        }
        *dst++ = v;
      }
    }
  } else if (kind == 10) {
    Resource_LoadAndDecompress((s32)&Value_00000105, EWRAM_BUF, 1, 0);
  } else if (kind == 6 || kind == 57 || kind == 14 || kind == 17) {
    Resource_LoadAndDecompress((s32)&Value_00000192, EWRAM_BUF, 1, 0);
    Resource_LoadAndDecompress((s32)&Value_00000188, EWRAM_BUF + 0xc56, 1, 0);
  } else if (kind == 68) {
    Resource_LoadAndDecompress((s32)&Value_00000192, EWRAM_BUF, 1, 0);
    Resource_LoadAndDecompress((s32)&Value_00000161, EWRAM_BUF + 0x1809, 1, 0);
  } else if (kind == 61) {
    Resource_LoadAndDecompress((s32)&Value_0000013a, EWRAM_BUF, 1, 1);
  } else if (kind == 38) {
    Resource_LoadAndDecompress((s32)&Value_00000187, EWRAM_BUF, 1, 1);
  } else if (kind == 44) {
    Resource_LoadAndDecompress((s32)&Value_00000130, EWRAM_BUF, 1, 0);
  } else if (kind == 60) {
    Resource_LoadAndDecompress((s32)&Value_00000192, EWRAM_BUF, 1, 0);
  } else if (kind == 46) {
    Resource_LoadAndDecompress((s32)&Value_0000017c, EWRAM_BUF, 1, 0);
  } else if (kind == 50) {
    Resource_LoadAndDecompress((s32)&Value_00000178, EWRAM_BUF, 1, 0);
  } else if (kind == 90) {
    Resource_LoadAndDecompress((s32)&Value_000000da, EWRAM_BUF, 0, 0);
  } else if (kind == 91) {
    Resource_LoadAndDecompress((s32)&Value_000000c1, EWRAM_BUF + 0x4000, 0, 0);
  } else if (kind == 84) {
    Resource_LoadAndDecompress((s32)&Value_00000192, EWRAM_BUF, 1, 0);
    Resource_LoadAndDecompress((s32)&Value_00000188, work->frames, 1, 1);
    ((void (*)(void *, s32))0x03000258)(EWRAM_BUF + 0xc56, 0x4000);
    for (i = 0; i != 4; i++) {
      for (j = 0; j != 120; j++) {
        for (k = 0; k != 24; k++)
          EWRAM_BUF[0xc5a + i * 0x1000 + j * 32 + k] =
              work->frames[i * 2880 + j * 24 + k];
      }
    }
    Resource_LoadAndDecompress((s32)&Value_00000159, work->frames, 1, 0);
  } else if (kind == 63 || kind == 64) {
    Resource_LoadAndDecompress((s32)&Value_00000115, EWRAM_BUF, 1, 0);
  } else if (kind == 71 || kind == 82) {
    Resource_LoadAndDecompress((s32)&Value_000000b4, EWRAM_BUF, 0, 0);
  } else if (kind == 96 || kind == 97 || kind == 98) {
    Resource_LoadAndDecompress((s32)&Value_0000018a, EWRAM_BUF, 0, 0);
    Resource_LoadAndDecompress((s32)&Value_0000013e, EWRAM_BUF + 0xd80, 1, 0);
  } else if (kind == 80) {
    Resource_LoadAndDecompress((s32)&Value_0000013a, EWRAM_BUF + 0x1000, 1, 0);
    ((void (*)(void *, s32))0x03000258)(EWRAM_BUF, 0x1000);
    for (i = 0; i != 48; i++) {
      for (k = 0; k != 24; k++) {
        EWRAM_BUF[0x208 + i * 64 + k] = EWRAM_BUF[0x1000 + i * 24 + k];
        EWRAM_BUF[0x208 + i * 64 + 47 - k] = EWRAM_BUF[0x1000 + i * 24 + k];
      }
    }
  } else if (kind == 79) {
    Func_0815b434(EWRAM_BUF, 48, 0x10000);
    for (i = 0; i != 48; i++) {
      for (k = 0; k != 24; k++) {
        EWRAM_BUF[0x800 + i * 48 + k] = EWRAM_BUF[i * 24 + k];
        EWRAM_BUF[0x800 + i * 48 + 47 - k] = EWRAM_BUF[i * 24 + k];
        EWRAM_BUF[0x800 + (95 - i) * 48 + k] = EWRAM_BUF[i * 24 + k];
        EWRAM_BUF[0x800 + (95 - i) * 48 + 47 - k] = EWRAM_BUF[i * 24 + k];
      }
    }
  } else if (kind == 94) {
    Resource_LoadAndDecompress((s32)&Value_0000014d, EWRAM_BUF, 1, 1);
    Resource_LoadAndDecompress((s32)&Value_000000e6, EWRAM_BUF + 0x2000, 1, 1);
  } else if (kind == 73) {
    REG_BLDALPHA = 0xc10;
  } else if (kind == 70) {
    Resource_LoadAndDecompress((s32)&Value_00000193, EWRAM_BUF, 1, 0);
  } else if (kind == 66) {
    Resource_LoadAndDecompress((s32)&Value_00000129, EWRAM_BUF, 1, 1);
  } else if (kind != 41 && kind != 62) {
    Resource_LoadAndDecompress((s32)&Value_00000161, EWRAM_BUF, 1, 0);
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
    Dma_Set(Resource_GetTableEntry((s32)&Value_00000184), PLTT, 0x84000020,
            DMA3);
    if (action->unk1c == 1)
      Resource_LoadAndDecompress((s32)&Value_00000157, EWRAM_BUF + 0x3c56, 1,
                                 0);
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
    if (action->unk1c == 1)
      Resource_LoadAndDecompress((s32)&Value_00000155, EWRAM_BUF + 0x3c56, 1,
                                 1);
    else
      Dma_Set(Resource_GetTableEntry((s32)&Value_00000155), PLTT, 0x84000020,
              DMA3);
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
    if (action->unk1c == 1)
      Resource_LoadAndDecompress((s32)&Value_00000151, EWRAM_BUF + 0x3c56, 1,
                                 1);
    else
      Dma_Set(Resource_GetTableEntry((s32)&Value_00000151), PLTT, 0x84000020,
              DMA3);
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
    if (action->unk1c == 1)
      Resource_LoadAndDecompress((s32)&Value_00000153, EWRAM_BUF + 0x3c56, 1,
                                 1);
    else
      Dma_Set(Resource_GetTableEntry((s32)&Value_00000153), PLTT, 0x84000020,
              DMA3);
    break;
  case 93:
  case 95:
  case 96:
    Dma_Set(Resource_GetTableEntry((s32)&Value_00000182), PLTT, 0x84000020,
            DMA3);
    break;
  case 100:
    Resource_LoadAndDecompress((s32)&Value_00000155, EWRAM_BUF + 0x3c56, 1, 1);
    break;
  }

  if (kind == 81 || kind == 83 || kind == 91 || kind == 92 || kind == 93)
    Resource_LoadAndDecompress((s32)&Value_000000c2, EWRAM_BUF, 0, 0);
  work->unk7780 = 2;
  if (kind == 42)
    work->unk7784 = 75;
  else
    work->unk7784 = 50;
  Scheduler_AddOrUpdateCallback(Func_08143001, 3200);
  EffectPosition_ProjectScaledBattler(action->actor, target);
  EffectPosition_ProjectScaledBattler(action->target, base);
  BattleFx_FetchRectangleBlitters(action->side, &drawFuncs);
  work->unk77b4 = 24;
  work->unk77b8 = 0;
  object = *GetBattleObjectSlotFar(action->target);
  for (i = 0; i != 64; i++) {
    work->particle[i].x = (Random16() & 63) + 32;
    work->particle[i].y = 0;
    work->particle[i].z = 0;
    work->particle[i].vx = Random16() & 0xffff;
    work->particle[i].vy = Random16() & 0xffff;
    work->particle[i].vz = Random16() & 0xffff;
  }
  ObjectDispatch_ApplyValueToChildrenFar(object, 0);
  origin[0] = object->x;
  origin[1] = object->y + 0x500000;
  origin[2] = object->z;
  save24 = object->unk24;
  save28 = object->unk28;
  save2c = object->unk2c;
  save34 = object->unk34;
  save48 = object->unk48;
  object->unk24 = 0;
  object->unk28 = 0;
  object->unk2c = 0;
  object->unk34 = 0;
  object->unk48 = 0;
  EffectPosition_ProjectScaledBattler(action->target, base);
  base[0] = base[0] / 2;
  Audio_PlayCue(212);

  for (frame = 0; frame != 32; frame++) {
    count = 0;
    p = work->particle;
    for (i = 0; i != 64; i++, p++) {
      if (p->x >= 0) {
        if (frame >= i / 4) {
          n = 5;
          Render_ResetTransformState();
          Func_080150e4(p->vz);
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
          drawFuncs.draw[1](screen, (u8 *)tiles + gData_08197410[n - 1],
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
      drawFuncs.draw[0](screen, EWRAM_BUF + 0x3c56, pos[0] - 10, pos[1] - 4, 20,
                        40);
    }
    work->unk78e8 = 1;
    WaitFrames(1);
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
    CLEAR(BG_VRAM, 0x3c00);
    CLEAR(screen, 0x3c00);
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
    CLEAR(BG_VRAM, 0x3c00);
    CLEAR(screen, 0x3c00);
    action->unk1c = 0;
    Scheduler_RemoveCallback(Func_08143489);
    Scheduler_RemoveCallback(Func_08143001);
    Runtime_ReleaseHeapBlock(188);
    Runtime_ReleaseHeapBlock(104);
    Object_SetMode(object, 3);
    if (kind == 45)
      BattleFx_RunProjectileVolley(action, 9);
    if (kind == 54)
      BattleFx_RunMysticFlame(action);
    if (kind == 56)
      BattleFx_RunProjectileVolley(action, 8);
    return;
  }
  ObjectDispatch_ApplyValueToChildrenFar(object, 16);
  object->unk24 = save24;
  object->unk28 = save28;
  object->unk2c = save2c;
  object->unk34 = save34;
  object->unk48 = save48;
  if (kind == 65) {
    CLEAR(BG_VRAM, 0x3c00);
    CLEAR(screen, 0x3c00);
    action->unk1c = 0;
    Scheduler_RemoveCallback(Func_08143489);
    Scheduler_RemoveCallback(Func_08143001);
    Runtime_ReleaseHeapBlock(188);
    Runtime_ReleaseHeapBlock(104);
    action->unk18 = 3;
    BattleFx_RunSparkGroups(action, 2);
    return;
  }
  CLEAR(screen, 0x3c00);
  CLEAR(BG_VRAM, 0x3c00);
  if (kind == 79) {
    for (i = 1; i != 64; i++) {
      v = 31 - ((Trig_Cos(i << 8) << 5) >> 16);
      PLTT_U16[i] = (v << 10) | (v << 5) | v;
    }
  }
  object2 = *GetBattleObjectSlotFar(action->actor);
  center[0] = __divsi3(object2->x - origin[0], 6);
  center[1] = __divsi3(object2->y - origin[1] + 0x1e0000, 6);
  center[2] = __divsi3(object2->z - origin[2], 6);
  for (i = 0; i != 64; i++)
    work->particle[i].life = 0;
  if (kind != 44) {
    half = Battle_GetObjectTableValueFar(action->actor) / 2;
    p = work->particle;
    for (i = 0; i != 32; i++, p++) {
      p->x = object2->x;
      p->y = half;
      p->z = object2->z;
      if (kind == 61) {
        p->vx = ((Random16() & 255) - 127) << 12;
        p->vy = ((Random16() & 255) - 64) << 10;
      } else {
        p->vx = ((Random16() & 255) - 127) << 12;
        p->vy = ((Random16() & 255) - 64) << 12;
      }
      p->vz = ((Random16() & 255) - 127) << 12;
      p->vx = p->vx / 2;
      p->vy = p->vy / 2;
      p->vz = p->vz / 2;
      p->life = i / 2 + 32;
    }
  }
  if (kind == 41) {
    Resource_LoadAndDecompress((s32)&Value_0000016f, work->frames, 1, 1);
    Resource_LoadAndDecompress((s32)&Value_00000170, EWRAM_BUF, 1, 0);
    REG_BLDALPHA = 0xe10;
  }
  if (kind == 86) {
    Resource_LoadAndDecompress((s32)&Value_000000f1, EWRAM_BUF, 1, 1);
    REG_BLDALPHA = 0x810;
  }
  if (kind == 88) {
    Resource_LoadAndDecompress((s32)&Value_000000dd, EWRAM_BUF, 1, 1);
    REG_BLDALPHA = 0x810;
  }
  if (kind == 62) {
    Resource_LoadAndDecompress((s32)&Value_00000171, work->frames, 1, 1);
    Resource_LoadAndDecompress((s32)&Value_00000172, EWRAM_BUF, 1, 0);
    REG_BLDALPHA = 0xe10;
  }
  if (kind == 85) {
    Resource_LoadAndDecompress((s32)&Value_000000f0, EWRAM_BUF, 1, 1);
    REG_BLDALPHA = 0xe10;
  }

  if (kind == 7 || kind == 43 || kind == 48 || kind == 73 || kind == 77 ||
      kind == 78 || kind == 41 || kind == 62 || kind == 49 || kind == 12 ||
      kind == 83 || kind == 84 || kind == 85 || kind == 86 || kind == 88) {
  } else if (kind == 66) {
    p = SPARKS;
    for (i = 0; i != 64; i++, p++) {
      p->x = object2->x;
      p->y = 0x140000;
      p->z = object2->z;
      if (object2->x < 0)
        p->vx = -((Random16() & 255) + 128) << 11;
      else
        p->vx = ((Random16() & 255) + 128) << 11;
      p->vy = ((Random16() & 255) - 127) << 10;
      p->vz = 0;
      p->life = 0;
    }
  } else {
    height = 0x140000;
    if (kind == 42 || kind == 15)
      height = 0;
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
    Scheduler_AddOrUpdateCallback(Func_08152475, 3200);

  if (kind == 4 || kind == 5 || kind == 6 || kind == 17 || kind == 53 ||
      kind == 60 || kind == 57 || kind == 14 || kind == 63 || kind == 64 ||
      kind == 100 || kind == 83 || kind == 90 || kind == 91 || kind == 76 ||
      kind == 66 || kind == 92 || kind == 93 || kind == 96 || kind == 97)
    duration = 32;
  else if (kind == 0 || kind == 1 || kind == 2 || kind == 3 || kind == 13 ||
           kind == 38 || kind == 39 || kind == 74 || kind == 75 || kind == 40 ||
           kind == 52 || kind == 72 || kind == 69 || kind == 55 || kind == 59 ||
           kind == 61 || kind == 44 || kind == 70 || kind == 86 || kind == 88 ||
           kind == 89 || kind == 16 || kind == 10)
    duration = 48;
  else if (kind == 51 || kind == 67 || kind == 9 || kind == 11 || kind == 95 ||
           kind == 99)
    duration = 20;
  else if (kind == 41 || kind == 62 || kind == 50 || kind == 81 || kind == 84 ||
           kind == 91 || kind == 79 || kind == 98)
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
    if (kind != 41 && kind != 62 && kind != 83 && kind != 85 && kind != 86 &&
        kind != 88) {
      for (i = 0; i != 160; i++)
        work->wave[i] =
            (0x40000 - (Trig_Sin((frame << 12) + (i << 11)) << 2)) >> 10;
    }
    switch (kind) {
    case 10:
      Func_0815f0a0((s32)&Value_00000105);
      break;
    case 66:
      Func_0815f0a0((s32)&Value_00000129);
      break;
    case 69:
      Func_0815f0a0((s32)&Value_000000b9);
      break;
    case 63:
      Func_0815f0a0((s32)&Value_00000115);
      break;
    case 44:
    case 68:
      Func_0815f0a0((s32)&Value_00000130);
      break;
    case 61:
      Func_0815f0a0((s32)&Value_0000013a);
      break;
    case 38:
      Func_0815f0a0((s32)&Value_00000187);
      break;
    case 0:
    case 40:
    case 74:
      Func_0815f0a0((s32)&Value_00000150);
      break;
    case 42:
    case 43:
    case 55:
      Func_0815f0a0((s32)&Value_0000017f);
      break;
    case 48:
    case 77:
      Func_0815f0a0((s32)&Value_0000017d);
      break;
    case 49:
      Func_0815f0a0((s32)&Value_00000184);
      break;
    case 2:
    case 59:
    case 75:
    case 82:
    case 91:
      Func_0815f0a0((s32)&Value_00000167);
      break;
    case 1:
    case 13:
    case 58:
    case 89:
      Func_0815f0a0((s32)&Value_00000166);
      break;
    case 3:
    case 50:
    case 52:
    case 78:
      Func_0815f0a0((s32)&Value_00000178);
      break;
    case 39:
    case 83:
      Func_0815f0a0((s32)&Value_00000163);
      break;
    case 5:
    case 53:
      Func_0815f0a0((s32)&Value_0000013e);
      break;
    case 81:
    case 90:
    case 98:
      Func_0815f0a0((s32)&Value_00000148);
      break;
    case 73:
      Func_0815f0a0((s32)&Value_00000147);
      break;
    case 84:
      Func_0815f0a0((s32)&Value_00000169);
      break;
    }
    if (frame < 6) {
      EffectPosition_ProjectScaledBattler(action->target, base);
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
          p = work->particle;
          for (i = 0; i != 64; i++, p++) {
            if (p->life > 0) {
              n = (p->life >> 2) + 1;
              if (n > 10)
                n = 10;
              EffectPosition_ProjectPoint(p, point);
              point[0] = point[0] / 2;
              drawFuncs.draw[0](screen, (u8 *)tiles + gData_08197410[n - 1],
                                point[0] - n / 2, point[1] - n, n, n * 2);
              BattleFxKernels_IntegrateVector3(p, 62, kind == 89 ? 0 : -0x800);
              p->life -= 2;
            }
          }
        } else {
          for (i = 0; i != 64; i++) {
            p = &work->particle[i / 2];
            if (p->life > 0) {
              n = __divsi3(p->life, 20) + 2;
              EffectPosition_ProjectPoint(p, point);
              point[0] = point[0] / 2;
              drawFuncs.draw[(i / 2) & 1](
                  screen, (u8 *)tiles + gData_08197410[n - 1], point[0] - n / 2,
                  point[1] - n, n, n * 2);
              BattleFxKernels_IntegrateVector3(p, 62, -0x800);
              p->life--;
            }
          }
        }
      }
    }

    if (kind == 7 || kind == 43 || kind == 48 || kind == 73 || kind == 77 ||
        kind == 78 || kind == 49 || kind == 12 || kind == 82) {
      start = 12;
      if (frame == 12)
        Audio_PlayCue(142);
      if (kind == 78)
        start = 0;
      if (frame == 50)
        Func_0814cd48(action->target, 7, -1, -1, 0);
      if (frame == 79)
        Func_0814cd48(action->target, 0, -1, -1, 0);
      if (frame == start) {
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
        object3 = *GetBattleObjectSlotFar(action->target);
        half = Battle_GetObjectTableValueFar(action->target) / 2;
        for (i = 0; i != 32; i++, p++) {
          if (p->life >= 0) {
            n = (i & 1) + 6;
            EffectPosition_ProjectPoint(p, point);
            point[0] = point[0] >> 1;
            drawFuncs.draw[kind == 78](
                screen, (u8 *)tiles + gData_08197410[n - 1], point[0] - n / 2,
                point[1] - n, n, n * 2);
            BattleFxKernels_IntegrateVector3(p, 62, 0);
            if (frame > i + 22) {
              dx = (object3->x - p->x) >> 8;
              dy = (object3->y + half - p->y) >> 8;
              dz = (object3->z - p->z) >> 8;
              p->vx += dx;
              p->vy += dy;
              p->vz += dz;
              if (dx > -0x1000 && dx < 0x1000 && dz > -0x1000 && dz < 0x1000)
                p->life = -1;
            }
          }
        }
      }
    }

    if (kind == 7 || kind == 43 || kind == 48 || kind == 73 || kind == 77 ||
        kind == 49 || kind == 12) {
    } else if (kind == 66) {
      p = SPARKS;
      for (i = 0; i != 16; i++, p++) {
        if (frame >= i + 4 && p->life <= 23) {
          EffectPosition_ProjectPoint(p, point);
          point[0] = point[0] / 2;
          drawFuncs.draw[0](screen, EWRAM_BUF, point[0] - 20, point[1] - 32, 40,
                            64);
          BattleFxKernels_IntegrateVector3(p, 64, 0);
          p->life++;
        }
      }
    } else if (kind == 51 || kind == 67 || kind == 9 || kind == 95 ||
               kind == 99) {
    } else if (kind == 6 || kind == 57 || kind == 14 || kind == 17) {
      if (kind == 17) {
        if (frame == 4)
          work->unk77a8 = 12;
        if (frame == 8)
          Audio_PlayCue(221);
      }
      if (kind == 6 || kind == 57) {
        if (frame >= 6 && frame < 20) {
          for (i = 0; i != 2; i++)
            drawFuncs.draw[0](
                screen, EWRAM_BUF + 0xc56 + (((frame + i * 3) / 2) & 3) * 2880,
                target[0] / 2 - 8, 0, 24, 104);
        }
      } else if (kind == 17 && frame >= 6 && frame < 20) {
        for (i = 0; i != 4; i++) {
          angle = frame * 1848 + i * 0x4000;
          m = ((frame + i * 3) / 2) & 3;
          x = target[0] / 2 + ((Trig_Sin(angle) << 5) >> 16);
          Trig_Cos(angle);
          drawFuncs.draw[0](screen, EWRAM_BUF + 0xc56 + m * 2880, x - 8, 0, 24,
                            104);
        }
      }
      if (frame >= 8 && frame < 24) {
        if (kind == 14) {
          amp = 12;
          off = 24;
        } else {
          off = 0;
          amp = 32;
        }
        for (i = 0; i != 3; i++) {
          m = i & 3;
          angle = Random16() & 0xffff;
          x = ((Trig_Sin(angle) << 3) >> 16) + target[0] / 2 -
              gData_08197492[m] / 2;
          y = ((amp * Trig_Cos(angle)) >> 16) + off - gData_08197498[m] / 2;
          Runtime_ReleaseHeapBlock(188);
          Runtime_ReleaseHeapBlock(104);
          Func_08196404(188, 7, 7, gData_0819887e[Random16() & 3] | 3, 0);
          IWRAM_SLOT_BC(screen, EWRAM_BUF + gData_08197486[m], x, y + 56,
                        gData_08197492[m], gData_08197498[m]);
          Runtime_ReleaseHeapBlock(188);
          BattleFx_FetchRectangleBlitters(action->side, &drawFuncs);
        }
      }
    } else if (kind == 68) {
      if (frame >= 8 && frame < 24) {
        for (i = 0; i != 3; i++) {
          angle = Random16() & 0xffff;
          m = i & 3;
          x = ((Trig_Sin(angle) << 4) >> 16) + target[0] / 2 -
              gData_08197492[m] / 2;
          y = ((Trig_Cos(angle) << 4) >> 16) - gData_08197498[m] / 2;
          Runtime_ReleaseHeapBlock(188);
          Runtime_ReleaseHeapBlock(104);
          Func_08196404(188, 7, 7, gData_08198882[Random16() & 3] | 3, 1);
          IWRAM_SLOT_BC(screen, EWRAM_BUF + gData_08197486[m], x, y + 80,
                        gData_08197492[m], gData_08197498[m]);
          Runtime_ReleaseHeapBlock(188);
          BattleFx_FetchRectangleBlitters(action->side, &drawFuncs);
        }
      }
      p = SPARKS;
      for (i = 0; i != 16; i++, p++) {
        if (frame >= i + 4) {
          life = p->life;
          if (life <= 23) {
            EffectPosition_ProjectPoint(p, point);
            point[0] = point[0] / 2;
            drawFuncs.draw[1](screen, EWRAM_BUF + 0x1809 + (life / 4) * 0x480,
                              point[0] - 12, point[1] - 24, 24, 48);
            BattleFxKernels_IntegrateVector3(p, 60, 0x1000);
            p->life++;
          }
        }
      }
      if (frame == 6)
        Audio_PlayCue(221);
    } else if (kind == 44) {
      Runtime_ReleaseHeapBlock(188);
      Runtime_ReleaseHeapBlock(104);
      if (frame >= 0 && frame < 24) {
        x = target[0] / 2;
        top = frame * 32 - 232;
        scroll = frame * 16 - 48;
        if (top > 0)
          top = 0;
        while (scroll > 104)
          scroll -= 104;
        BattleFx_LoadBlitter(188, 19);
        IWRAM_SLOT_BC(screen, EWRAM_BUF, x - 8, top + scroll - 104, 17, 104);
        IWRAM_SLOT_BC(screen, EWRAM_BUF, x - 8, top + scroll, 17, 104 - scroll);
        IWRAM_SLOT_BC(screen, EWRAM_BUF + 0x6e8, x - 17, top + 47, 34, 65);
        Runtime_ReleaseHeapBlock(188);
        if (frame == 8)
          work->unk77a8 = 8;
        if (frame > 1) {
          count = 0;
          p = work->particle;
          for (i = 0; i != 64; i++, p++) {
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
      BattleFx_FetchRectangleBlitters(action->side, &drawFuncs);
    } else if (kind == 61) {
      Runtime_ReleaseHeapBlock(188);
      Runtime_ReleaseHeapBlock(104);
      if (frame >= 4 && frame < 24) {
        x = target[0] / 2;
        BattleFx_LoadBlitter(188, 19);
        IWRAM_SLOT_BC(screen, EWRAM_BUF, x - 24, 48, 24, 48);
        Runtime_ReleaseHeapBlock(188);
        BattleFx_LoadBlitter(188, 23);
        IWRAM_SLOT_BC(screen, EWRAM_BUF, x, 48, 24, 48);
        Runtime_ReleaseHeapBlock(188);
      }
      BattleFx_FetchRectangleBlitters(action->side, &drawFuncs);
    } else if (kind == 60) {
      if (frame > 15)
        REG_BLDALPHA = (32 - frame) | 0x1000;
      if (frame > 5) {
        x = target[0] / 2;
        m = __modsi3(frame / 2, 3);
        drawFuncs.draw[0](screen, EWRAM_BUF + 0xc56 + m * 0xa00, x - 20, 16, 40,
                          32);
        drawFuncs.draw[0](screen, EWRAM_BUF + 0x2a56 + m * 0x500, x - 20, 48,
                          40, 32);
        drawFuncs.draw[0](screen, EWRAM_BUF + 0x1156 + m * 0xa00, x - 20, 80,
                          40, 32);
      }
    } else if (kind == 5 || kind == 53 || kind == 78) {
      if (frame < 24) {
        p = SPARKS;
        for (i = 0; i != 16; i++, p++) {
          if (frame >= i / 2 + 4) {
            life = p->life;
            if (life <= 11) {
              EffectPosition_ProjectPoint(p, point);
              point[0] = point[0] / 2;
              drawFuncs.draw[0](screen, EWRAM_BUF + (life / 2) * 0x800,
                                point[0] - 16, point[1] - 32, 32, 64);
              BattleFxKernels_IntegrateVector3(p, 60, 0x1000);
              p->life++;
            }
          }
        }
      }
    } else if (kind == 4) {
    } else if (kind == 8) {
    } else if (kind == 41) {
      y = ((Trig_Cos(frame << 9) << 2) >> 16) + ((s16 *)target)[3] + 16;
      if (frame < 4) {
        drawFuncs.draw[0](screen, work->frames,
                          gData_0819750c[action->side * 7],
                          y + gData_0819751a[0], 57, 98);
      } else {
        if (frame < 8)
          drawFuncs.draw[0](screen, work->frames,
                            gData_0819750c[action->side * 7],
                            y + gData_0819751a[0], 57, 98);
        drawFuncs.draw[0](screen, work->frames + 0x1cd2,
                          gData_0819750c[action->side * 7 + 1],
                          y + gData_0819751a[1], 99, 69);
        if (frame >= 4 && frame < 6)
          ((void (*)(void *, s32, s32))0x03000260)(screen, 0x4000, 0x3f3f3f3f);
        if (frame >= 6 && frame < 8)
          drawFuncs.draw[0](screen, work->frames + 0x3781,
                            gData_0819750c[action->side * 7 + 2],
                            y + gData_0819751a[2], 128, 91);
        if (frame >= 8 && frame < 10)
          drawFuncs.draw[0](screen, EWRAM_BUF,
                            gData_0819750c[action->side * 7 + 3],
                            y + gData_0819751a[3], 128, 91);
        if (frame >= 10 && frame < 12)
          drawFuncs.draw[0](screen, EWRAM_BUF + 0x2d80,
                            gData_0819750c[action->side * 7 + 4],
                            y + gData_0819751a[4], 128, 59);
        if (frame >= 12 && frame < 14)
          drawFuncs.draw[0](screen, EWRAM_BUF + 0x4b00,
                            gData_0819750c[action->side * 7 + 5],
                            y + gData_0819751a[5], 122, 29);
        if (frame >= 14 && frame < 16)
          drawFuncs.draw[0](screen, EWRAM_BUF + 0x58d2,
                            gData_0819750c[action->side * 7 + 6],
                            y + gData_0819751a[6], 76, 25);
      }
    } else if (kind == 85) {
      REG_BLDCNT = 0;
      work->unk7780 = 1;
      work->unk7784 = 0;
      Runtime_ReleaseHeapBlock(188);
      Runtime_ReleaseHeapBlock(104);
      BattleFx_LoadBlitter(104, 35);
      drawFuncs.draw[0] = IWRAM_SLOT_68;
      if (frame == 0) {
        for (i = 0; i != 1104; i++)
          work->frames[i] = Random16() & 31;
      }
      y = frame * 28 - 96;
      if (y > 16)
        y = 16;
      h = 96;
      if (frame > 7) {
        for (i = 0; i != 34; i++) {
          row = 112 - frame * 2 + i;
          if (row >= 0 && row < 96) {
            src = work->frames + (row & 15) * 69;
            dst = EWRAM_BUF + row * 69;
            for (k = 0; k != 69; k++) {
              if (*src++ < i)
                *dst = 0;
              dst++;
            }
          }
        }
      }
      drawFuncs.draw[0](screen, EWRAM_BUF, 29, y, 69, h);
      if (frame > 5) {
        switch ((frame - 6) / 4) {
        case 0:
          drawFuncs.draw[0](screen, EWRAM_BUF + 0x19e0, 32, 92, 59, 24);
          break;
        case 1:
          drawFuncs.draw[0](screen, EWRAM_BUF + 0x1f68, 30, 87, 70, 27);
          break;
        case 2:
          drawFuncs.draw[0](screen, EWRAM_BUF + 0x26ca, 23, 57, 84, 55);
          break;
        case 3:
          drawFuncs.draw[0](screen, EWRAM_BUF + 0x38d6, 27, 55, 75, 57);
          break;
        case 4:
          drawFuncs.draw[0](screen, EWRAM_BUF + 0x4989, 28, 19, 67, 86);
          break;
        case 5:
          drawFuncs.draw[0](screen, EWRAM_BUF + 0x600b, 33, 22, 50, 65);
          break;
        case 6:
          drawFuncs.draw[0](screen, EWRAM_BUF + 0x6cbd, 41, 21, 48, 29);
          break;
        }
      }
      Runtime_ReleaseHeapBlock(104);
      BattleFx_FetchRectangleBlitters(action->side, &drawFuncs);
    } else if (kind == 96 || kind == 97 || kind == 98) {
      if (frame < 20) {
        if (frame < 8)
          m = gData_08198886[frame / 2];
        else
          m = 3;
        Runtime_ReleaseHeapBlock(188);
        Runtime_ReleaseHeapBlock(104);
        BattleFx_LoadBlitter(104, 3);
        IWRAM_SLOT_68(screen, EWRAM_BUF + m * 0x6c0, target[0] / 2 - 18, 56, 18,
                      48);
        Runtime_ReleaseHeapBlock(104);
        BattleFx_LoadBlitter(104, 7);
        IWRAM_SLOT_68(screen, EWRAM_BUF + m * 0x6c0, target[0] / 2, 56, 18, 48);
        Runtime_ReleaseHeapBlock(104);
        BattleFx_FetchRectangleBlitters(action->side, &drawFuncs);
      }
      if ((kind == 97 || kind == 98) && frame < 24) {
        if (kind == 97)
          limit = 16;
        else
          limit = 24;
        p = SPARKS;
        for (i = 0; i != limit; i++, p++) {
          if (frame >= i / 2 + 4) {
            life = p->life;
            if (life <= 9) {
              EffectPosition_ProjectPoint(p, point);
              point[0] = point[0] / 2;
              drawFuncs.draw[0](screen, EWRAM_BUF + 0xd80 + (life / 2) * 0x800,
                                point[0] - 16, point[1] - 32, 32, 64);
              BattleFxKernels_IntegrateVector3(p, 64, 0x1000);
              p->life++;
            }
          }
        }
      }
    } else if (kind == 86) {
      work->unk7780 = 1;
      work->unk7784 = 0;
      Runtime_ReleaseHeapBlock(188);
      Runtime_ReleaseHeapBlock(104);
      if (action->side == 0)
        BattleFx_LoadBlitter(104, 35);
      else
        BattleFx_LoadBlitter(104, 39);
      drawFuncs.draw[0] = IWRAM_SLOT_68;
      scrollA += scrollB;
      if (frame > 6)
        scrollB = scrollB * 48 / 64;
      REG_BG2X = (scrollA >> 16) << 8;
      t = frame - 28;
      if (t >= 0 && t < 16)
        REG_BLDALPHA = ((t / 2 + 8) << 8) | (16 - t);
      if (frame >= 4 && frame < 8)
        ((void (*)(void *, s32, s32))0x03000260)(screen, 0x4000, 0x01010101);
      if (frame > 3) {
        if (action->side == 0)
          drawFuncs.draw[0](screen, EWRAM_BUF, 32, 21, 96, 88);
        else
          drawFuncs.draw[0](screen, EWRAM_BUF, 0, 21, 96, 88);
      }
      m = frame / 2;
      if (m < 10) {
        if (action->side == 0)
          drawFuncs.draw[0](screen, EWRAM_BUF + gData_0819888a[m],
                            gData_081988c6[m], gData_081988da[m],
                            gData_0819889e[m], gData_081988b2[m]);
        else
          drawFuncs.draw[0](screen, EWRAM_BUF + gData_0819888a[m],
                            128 - gData_081988c6[m] - gData_0819889e[m],
                            gData_081988da[m], gData_0819889e[m],
                            gData_081988b2[m]);
      }
      Runtime_ReleaseHeapBlock(104);
      BattleFx_FetchRectangleBlitters(action->side, &drawFuncs);
    } else if (kind == 88) {
      work->unk7780 = 1;
      work->unk7784 = 0;
      if (frame == 7) {
        work->unk77a8 = 16;
        Audio_PlayCue(134);
      }
      if (frame == 11)
        Audio_PlayCue(134);
      if (frame == 15)
        Audio_PlayCue(134);
      Runtime_ReleaseHeapBlock(188);
      Runtime_ReleaseHeapBlock(104);
      if (action->side == 0)
        BattleFx_LoadBlitter(104, 35);
      else
        BattleFx_LoadBlitter(104, 39);
      drawFuncs.draw[0] = IWRAM_SLOT_68;
      scrollA += scrollB;
      if (frame == 5)
        scrollA += 0x20000;
      if (frame > 6)
        scrollB = scrollB * 48 / 64;
      REG_BG2X = (scrollA >> 16) << 8;
      t = frame - 29;
      if (t >= 0 && t < 16)
        REG_BLDALPHA = ((t / 2 + 8) << 8) | (16 - t);
      if (frame > 4)
        drawFuncs.draw[0](screen, EWRAM_BUF, 35, 17, 60, 95);
      if (frame == 5)
        ((void (*)(void *, s32, s32))0x03000260)(screen, 0x4000, 0x3f3f3f3f);
      if (frame >= 0 && (m = __divsi3(frame, 3)) < 7) {
        if (action->side == 0)
          drawFuncs.draw[0](screen, EWRAM_BUF + gData_081988ee[m],
                            gData_08198918[m], gData_08198926[m] - 16,
                            gData_081988fc[m], gData_0819890a[m]);
        else
          drawFuncs.draw[0](screen, EWRAM_BUF + gData_081988ee[m],
                            128 - gData_08198918[m] - gData_081988fc[m],
                            gData_08198926[m] - 16, gData_081988fc[m],
                            gData_0819890a[m]);
      }
      for (i = 0; i != 3; i++) {
        t = frame - 8 - i * 2;
        if (t >= 0 && t <= 8) {
          m = __divsi3(t, 3);
          if (action->side == 0)
            drawFuncs.draw[0](screen, EWRAM_BUF + gData_08198934[m],
                              gData_08198940[i][0] - gData_0819893a[m] / 2,
                              gData_08198940[i][1] - gData_0819893d[m] / 2 - 16,
                              gData_0819893a[m], gData_0819893d[m]);
          else
            drawFuncs.draw[0](screen, EWRAM_BUF + gData_08198934[m],
                              128 - gData_08198940[i][0] -
                                  gData_0819893a[m] / 2,
                              gData_08198940[i][1] - gData_0819893d[m] / 2 - 16,
                              gData_0819893a[m], gData_0819893d[m]);
        }
      }
      Runtime_ReleaseHeapBlock(104);
      BattleFx_FetchRectangleBlitters(action->side, &drawFuncs);
    } else if (kind == 62) {
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
        ((void (*)(void *, s32, s32))0x03000260)(screen, 0x4000, 0x3f3f3f3f);
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
          drawFuncs.draw[0](screen, EWRAM_BUF, 0, 16, 128, 91);
        if (frame >= 10 && frame < 12)
          drawFuncs.draw[0](screen, EWRAM_BUF + 0x2d80, 0, 16, 128, 59);
        if (frame >= 12 && frame < 14)
          drawFuncs.draw[0](screen, EWRAM_BUF + 0x4b00, 0, 16, 128, 29);
        if (frame >= 14 && frame < 16)
          drawFuncs.draw[0](screen, EWRAM_BUF + 0x5980, 0, 16, 128, 26);
      }
    } else if (kind == 50) {
      for (i = 0; i != 12; i++) {
        if (frame >= i + 6 && frame < i + 18) {
          m = (frame - i - 6) / 2;
          x = target[0] / 2 - gData_08197467[m] / 2;
          if (i & 1)
            x += (i + 1) / 2 * 3;
          else
            x -= (i + 1) / 2 * 3;
          drawFuncs.draw[i == 0 || ((i - 1) & 3) > 1](
              screen, EWRAM_BUF + gData_0819747a[m], x, gData_08197473[m] + 48,
              gData_08197467[m], gData_0819746d[m]);
        }
      }
    } else if (kind == 46) {
      if (frame == 0) {
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
          m = i & 3;
          Render_ResetTransformState();
          SceneTransform_ApplyPitch(p->vx);
          SceneTransform_ApplyYaw(p->vy);
          EffectPosition_ProjectPoint(p, point);
          point[0] = point[0] / 2 + target[0] / 2;
          point[1] = point[1] + target[1] + 32;
          drawFuncs.draw[1](screen, EWRAM_BUF + gData_08198946[m], point[0] - 4,
                            point[1] - 4, 8, 8);
          p->x -= 6;
          if (p->x < 0 && ((i & 7) == 0 || i == 63)) {
            Audio_PlayCue(133);
            Func_0814cd48(action->actor, 7, 5, 0, 4);
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
        drawFuncs.draw[0](screen, EWRAM_BUF, 48, 104 - h, 32, h);
      }
    } else if (kind == 79) {
      p = SPARKS;
      if (frame == 0) {
        p->x = (target[0] / 2) << 16;
        p->y = (target[1] + 48) << 16;
        if (action->side == 0)
          p->vx = -0x30000;
        else
          SPARKS->vx = 0x30000;
        p->x += -p->vx * 12;
        p->vy = 0x80000;
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
        drawFuncs.draw[1](screen, EWRAM_BUF + 0x800, (p->x >> 16) - 24,
                          (p->y >> 16) - 96, 48, 96);
        if (frame < 12)
          BattleFxKernels_IntegrateVector2(SPARKS, 64, 0);
        else if (frame == 12)
          p->vy = __divsi3(-p->vy, 3);
        else if (frame < 13 || frame > 18)
          BattleFxKernels_IntegrateVector2(SPARKS, 64, 0x4000);
      }
      if (frame == 11) {
        Audio_PlayCue(144);
        work->unk77a8 = 16;
      }
    } else if (kind == 63 || kind == 64) {
      if (frame < 6) {
        if (action->side == 0)
          x = target[0] / 2 + (6 - frame) * 6;
        else
          x = target[0] / 2 - (6 - frame) * 6;
        y = target[1] - (6 - frame) * 12 + 24;
        drawFuncs.draw[1](screen, EWRAM_BUF, x - 16, y - 32, 32, 64);
      }
    } else if (kind == 70) {
      if (frame > 31)
        REG_BLDALPHA = (48 - frame) | 0x1000;
      p = SPARKS;
      i = 0;
      if (frame < 14) {
        if (action->side == 0)
          x = target[0] / 2 + (14 - frame) * 6;
        else
          x = target[0] / 2 - (14 - frame) * 6;
        y = target[1] - (14 - frame) * 12 + 24;
        drawFuncs.draw[1](screen, EWRAM_BUF, x - 16, y - 32, 32, 64);
        if (frame == 13) {
          Audio_PlayCue(134);
          work->unk77a8 = 8;
          BattleFx_StartActorMotion(action->actor, 1, 0x20000, 0x80000, 0x30000,
                                    150);
          p = SPARKS;
          table = gData_081977f8;
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
        for (; i != 16; i++, p++) {
          m = __modsi3(i, 5) * 3 + __modsi3(__divsi3(p->life, 96), 3);
          drawFuncs.draw[i <= 2](screen, EWRAM_BUF + 0x800 + gData_08197834[m],
                                 (p->x >> 16) - gData_0819781a[m] / 2,
                                 (p->y >> 16) - gData_08197826[m] / 2,
                                 gData_0819781a[m], gData_08197826[m]);
          BattleFxKernels_IntegrateVector2(p, 64, 0x2000);
          p->life += p->z;
          if (p->z > 1 && (frame & 1))
            p->z--;
        }
      }
    } else if (kind == 94) {
      if (frame == 0) {
        Resource_LoadAndDecompress((s32)&Value_000000e6, EWRAM_BUF + 0x2000, 1,
                                   1);
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
        BattleFx_StartActorMotion(action->actor, 1, 0x20000, 0x80000, 0x30000,
                                  150);
        Audio_PlayCue(134);
        work->unk77a8 = 8;
      }
      i = 0;
      p = work->particle;
      if (frame < 14) {
        if (action->side == 0)
          x = target[0] + (14 - frame) * 6;
        else
          x = target[0] - (14 - frame) * 6;
        y = target[1] - (14 - frame) * 12 + 24;
        drawFuncs.draw[0](screen, EWRAM_BUF + 0x2000, 32, y - 32, 64, 64);
        REG_BG2X = (64 - x) << 8;
      } else {
        for (; i != 32; i++, p++) {
          m = __modsi3(i, 5) * 3 + __modsi3(__divsi3(p->z, 96), 3);
          drawFuncs.draw[0](screen, EWRAM_BUF + 0x83c + gData_081974bc[m],
                            (p->x >> 16) - gData_0819749e[m] / 2,
                            (p->y >> 16) - gData_081974ad[m] / 2,
                            gData_0819749e[m], gData_081974ad[m]);
          BattleFxKernels_IntegrateVector2(p, 63, 0x8000);
          p->z += p->vz;
          if ((frame & 1) && p->vz > 0)
            p->vz--;
        }
      }
    } else if (kind == 84) {
      if (frame == 0) {
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
        Audio_PlayCue(221);
      if (frame == 24)
        Audio_PlayCue(390);
      if (frame >= 8 && frame < 24) {
        for (i = 0; i != 3; i++) {
          angle = Random16() & 0xffff;
          m = i & 3;
          x = ((Trig_Sin(angle) << 4) >> 16) + target[0] / 2 -
              gData_08197492[m] / 2;
          y = ((Trig_Cos(angle) << 4) >> 16) - gData_08197498[m] / 2;
          Runtime_ReleaseHeapBlock(188);
          Runtime_ReleaseHeapBlock(104);
          Func_08196404(188, 7, 7, gData_0819894e[Random16() & 3] | 3, 1);
          IWRAM_SLOT_BC(screen, EWRAM_BUF + gData_08197486[m], x, y + 80,
                        gData_08197492[m], gData_08197498[m]);
          Runtime_ReleaseHeapBlock(188);
          BattleFx_FetchRectangleBlitters(action->side, &drawFuncs);
        }
      }
      if (frame > 3) {
        matrix = Runtime_BumpAllocateAlternatePool(32);
        model = Render_AllocateModelList(1);
        texture164.width = 5;
        texture164.height = 7;
        model->kind = 6;
        model->mesh = gData_08199268;
        model->matrix = matrix;
        model->texture = &texture164;
        p = RINGS;
        for (i = 0; i != 20; i++, p++) {
          if (frame >= i && frame < i + 8) {
            if (frame == i)
              model->fade = -16;
            else if (frame >= i + 4)
              model->fade = 64 - (frame - i) * 16;
            else
              model->fade = 0;
            texture164.pixels = EWRAM_BUF + 0xc56 + (p->life << 12);
            p->life = (p->life + 1) & 3;
            Render_ResetTransformState();
            Func_08015160((target[0] / 2 - 64) << 16, (target[1] - 64) << 16,
                          0);
            Func_080150e4(p->z);
            p->z += p->vz;
            Func_080151e4(0x40000, 0x10000, 0x10000);
            Render_TransformVertices(gData_081991b0, matrix, 4);
            Render_DrawModelList(model);
          }
        }
        MemFree(model);
        MemFree(matrix);
      }
    } else if (kind == 71 || kind == 80 || kind == 82) {
      matrix = Runtime_BumpAllocateAlternatePool(0x200);
      model = Render_AllocateModelList(1);
      fade = 0;
      if (frame > 39)
        fade = (40 - frame) * 4;
      else
        work->unk77a8 = 1;
      texture156.width = 6;
      texture156.height = 6;
      texture156.pixels = EWRAM_BUF;
      model->kind = 7;
      model->texture = &texture156;
      model->mesh = gData_08199340;
      model->matrix = matrix;
      model->fade = fade;
      Render_ResetTransformState();
      Func_08015160((target[0] / 2 - 64) << 16, (target[1] - 64) << 16, 0);
      Func_080151e4(0x8000, 0x10000, 0x8000);
      if (frame == 8)
        Audio_PlayCue(104);
      if (fade >= -63) {
        angle = frame << 12;
        if (kind == 82) {
          SceneTransform_ApplyPitch(-0x800);
          SceneTransform_ApplyYaw(-frame << 11);
          SceneTransform_ApplyPitch(-frame << 4);
        } else {
          SceneTransform_ApplyPitch(-0x1000);
          SceneTransform_ApplyYaw(frame << 11);
        }
        Func_0801521c(angle);
        Render_TransformVertices(gData_08199210, matrix, 4);
        Render_DrawModelList(model);
        if (kind != 82) {
          SceneTransform_ApplyYaw(-frame << 12);
          Func_0801521c(0x8000);
          Render_TransformVertices(gData_08199210, matrix, 4);
          Render_DrawModelList(model);
        }
      }
      MemFree(model);
      MemFree(matrix);
      if (kind == 82) {
        m = frame / 4;
        size = 0x10000;
        if (frame > 39)
          size = 0x38000 - (frame << 12);
        if (m * 2 > 1 && size > 0) {
          Func_0815b434(work->frames, m * 4, size);
          Runtime_ReleaseHeapBlock(188);
          Func_0818caa8(work->frames, target[0] / 2, target[1], m * 4);
          BattleFx_LoadBlitter(188, 15);
        }
      }
    } else if (kind == 81 || kind == 83 || kind == 92 || kind == 93) {
      matrix = Runtime_BumpAllocateAlternatePool(0x200);
      model = Render_AllocateModelList(1);
      texture148.width = 7;
      texture148.height = 7;
      texture148.pixels = EWRAM_BUF;
      model->texture = &texture148;
      model->kind = 7;
      model->mesh = gData_08199364;
      model->matrix = matrix;
      if (kind == 92 || kind == 93) {
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
              Func_08015160((target[0] / 2 - 64) << 16, (target[1] - 64) << 16,
                            0);
              Func_080151e4(0x8000, 0x10000, 0x8000);
              Func_080150e4((i << 14) + 0x2000);
              SceneTransform_ApplyPitch(-0x1000);
              Func_0801521c(angle >> 1);
              Render_TransformVertices(gData_08199210, matrix, 4);
              Render_DrawModelList(model);
            }
          }
        }
      } else if (kind == 83) {
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
            Func_080151e4(0x8000, 0x10000, 0x8000);
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
            fade = 0;
            if (t > 7)
              fade = (8 - t) * 8;
            model->fade = fade;
            if (fade > -64) {
              Render_ResetTransformState();
              Func_08015160((target[0] / 2 - 64) << 16, (target[1] - 64) << 16,
                            0);
              Func_080151e4(0x8000, 0x10000, 0x8000);
              Func_080150e4((i << 14) + 0x2000);
              SceneTransform_ApplyPitch(-0x1000);
              Func_0801521c(t * 3 << 12);
              Render_TransformVertices(gData_08199210, matrix, 4);
              Render_DrawModelList(model);
            }
          }
        }
      }
      MemFree(model);
      MemFree(matrix);
      if (kind != 92 && kind != 93) {
        if (frame == 4) {
          p = SPARKS;
          for (i = 0; i != 64; i++, p++) {
            p->x = Random16();
            p->y = 0;
            p->vy = ((Random16() & 3) + 3) << 17;
          }
        }
        if (frame > 4) {
          p = SPARKS;
          for (i = 0; i != 64; i++, p++) {
            for (k = 0; k != 12; k++) {
              r = (p->y >> 16) - k * 2;
              if (r >= 0) {
                x = target[0] / 2 + ((r * Trig_Sin(p->x)) >> 16);
                y = target[1] + ((r * Trig_Cos(p->x)) >> 16);
                n = 2 - k / 8;
                drawFuncs.draw[0](screen, (u8 *)tiles + gData_08197410[n - 1],
                                  x - n / 2, y - n, n, n * 2);
              }
            }
            p->y += p->vy;
            p->vy = p->vy * 62 / 64;
          }
        }
      }
    } else if (kind == 91) {
      matrix = Runtime_BumpAllocateAlternatePool(0x200);
      model = Render_AllocateModelList(1);
      texture140.width = 7;
      texture140.height = 7;
      texture140.pixels = EWRAM_BUF;
      model->kind = 7;
      model->texture = &texture140;
      model->mesh = gData_08199364;
      model->matrix = matrix;
      t = frame - 4;
      if (t >= 0) {
        angle = t << 14;
        fade = 0;
        if (t > 7)
          fade = (8 - t) * 8;
        model->fade = fade;
        if (fade >= -63) {
          Render_ResetTransformState();
          Func_08015160((target[0] / 2 - 64) << 16, 0x280000, 0);
          Func_080151e4(0x8000, 0x10000, 0x8000);
          SceneTransform_ApplyPitch(1000);
          Func_0801521c(angle);
          Render_TransformVertices(gData_08199210, matrix, 4);
          Render_DrawModelList(model);
        }
      }
      texture140.width = 7;
      texture140.height = 4;
      texture140.pixels = EWRAM_BUF + 0x4000;
      model->mesh = gData_08198d2c;
      for (i = 0; i != 2; i++) {
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
            Func_080151e4(scale * 2, scale * 4, scale / 4);
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
      MemFree(model);
      MemFree(matrix);
      if (frame == 4)
        work->unk77a8 = 8;
    } else if (kind == 90) {
      matrix = Runtime_BumpAllocateAlternatePool(0x200);
      model = Render_AllocateModelList(1);
      texture132.width = 6;
      texture132.height = 6;
      texture132.pixels = EWRAM_BUF;
      model->kind = 7;
      model->texture = &texture132;
      model->matrix = matrix;
      model->mesh = gData_081990d0;
      for (i = 0; i != 4; i++) {
        start = gData_0819895e[i] + 4;
        model->frame = (frame * 8 + i * 32) & 127;
        if (frame == start + 4)
          Audio_PlayCue(104);
        if (frame > start) {
          fade = (start - frame) * 8 + 56;
          if (fade > -16)
            fade = -16;
          if (fade > -64) {
            scale = (frame - start) * gData_08198962[i] * 2000 + 0x4000;
            model->fade = fade;
            Render_ResetTransformState();
            Func_08015160((target[0] / 2 - 60) << 16, 0x280000, 0);
            Func_080151e4(scale, gData_08198966[i] * scale * 2, scale / 4);
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
      MemFree(model);
      MemFree(matrix);
      if (frame == 4)
        work->unk77a8 = 20;
    } else if (kind == 15) {
      if (frame == 0) {
        p = SPARKS;
        for (i = 0; i != 16; i++, p++) {
          p->life = -i * 2;
          p->x = Random16();
          p->z = i * 2 + 80;
          p->vx = (Random16() & 511) + 512;
        }
      }
      for (i = 0; i != 0x400; i++) {
        if (EWRAM_BUF[0x400 + i] >> 1 == frame / 2)
          EWRAM_BUF[0x800 + i] = EWRAM_BUF[i];
      }
      EffectPosition_ProjectScaledBattler(action->actor, point2);
      point2[0] = point2[0] / 2;
      p = SPARKS;
      for (i = 0; i != 16; i++, p++) {
        if (p->life >= 0 && p->z > 0) {
          m = __modsi3(i, 3);
          x = point2[0] + ((p->z * Trig_Sin(p->x)) >> 16);
          y = point2[1] + ((p->z * Trig_Cos(p->x)) >> 16);
          p->z -= 4;
          p->x += p->vx;
          if (kind == 16)
            drawFuncs.draw[0](screen, EWRAM_BUF, x - 16, y - 16, 32, 32);
          else
            drawFuncs.draw[0](screen, EWRAM_BUF + m * 0x240, x - 12, y - 12, 24,
                              24);
        }
        p->life++;
      }
    } else if (kind == 16) {
      p = SPARKS;
      for (i = 0; i != 32; i++, p++) {
        if (frame >= i / 2 + 4 && p->life <= 23) {
          m = 0;
          if (p->life > 11)
            m = (p->life - 12) / 2;
          EffectPosition_ProjectPoint(p, point);
          point[0] = point[0] / 2;
          drawFuncs.draw[1](screen, EWRAM_BUF + m * 0x400, point[0] - 16,
                            point[1] - 16, 32, 32);
          BattleFxKernels_IntegrateVector3(p, 62, 0x800);
          BattleFxKernels_IntegrateVector3(p, 62, 0x800);
          p->life++;
        }
      }
      if (frame == 4)
        work->unk77a8 = 12;
      if (frame == 8)
        Audio_PlayCue(103);
    } else if (kind == 10) {
      if (frame == 0) {
        work->unk7780 = 1;
        work->unk7784 = 0;
        REG_BLDALPHA = 0x610;
      }
      p = SPARKS;
      for (i = 0; i != 24; i++, p++) {
        if (frame >= i / 2 + 4 && p->life <= 23) {
          m = 0;
          if (p->life > 11)
            m = __divsi3(p->life - 12, 3);
          EffectPosition_ProjectPoint(p, point);
          point[0] = point[0] / 2;
          drawFuncs.draw[0](screen, EWRAM_BUF + m * 0x800, point[0] - 16,
                            point[1] - 32, 32, 64);
          BattleFxKernels_IntegrateVector3(p, 58, 0x1000);
          p->life++;
        }
      }
      if (frame == 4)
        work->unk77a8 = 12;
      if (frame == 8)
        Audio_PlayCue(103);
    } else if (kind == 42) {
      if (frame > 47)
        REG_BLDALPHA = (64 - frame) | 0x1000;
      p = SPARKS;
      for (i = 0; i != 16; i++, p++) {
        m = __modsi3(i, 3);
        EffectPosition_ProjectPoint(p, point);
        point[0] = point[0] / 2;
        drawFuncs.draw[i & 1](screen, EWRAM_BUF + m * 0x240, point[0] - 12,
                              point[1] - 12, 24, 24);
        BattleFxKernels_IntegrateVector3(p, 60, 1 << ((i & 3) + 11));
        p->life++;
      }
    } else if (kind == 76) {
      if (frame == 18)
        Audio_PlayCue(390);
      for (i = 0; i != 3; i++) {
        if (frame == i * 3 + 3) {
          Audio_PlayCue(212);
          ((void (*)(void *, s32, s32))0x03000260)(screen, 0x4000, 0x2f2f2f2f);
        }
      }
    } else if (kind == 74) {
      if (frame == 8)
        Audio_PlayCue(103);
      p = SPARKS;
      for (i = 0; i != 32; i++, p++) {
        if (frame >= i / 2 + 4) {
          life = p->life;
          if (life <= 23) {
            EffectPosition_ProjectPoint(p, point);
            point[0] = point[0] / 2;
            drawFuncs.draw[1](screen, EWRAM_BUF + (life / 4) * 0x480,
                              point[0] - 12, point[1] - 24, 24, 48);
            BattleFxKernels_IntegrateVector3(p, 62, 0x800);
            BattleFxKernels_IntegrateVector3(p, 62, 0x800);
            p->life++;
          }
        }
      }
    } else if (kind != 100) {
      p = SPARKS;
      for (i = 0; i != 16; i++, p++) {
        if (frame >= i + 4) {
          life = p->life;
          if (life <= 23) {
            EffectPosition_ProjectPoint(p, point);
            point[0] = point[0] / 2;
            if (kind == 69 || kind == 74)
              drawFuncs.draw[1](screen, EWRAM_BUF + (life / 4) * 0x480,
                                point[0] - 12, point[1] - 24, 24, 48);
            else
              drawFuncs.draw[i & 1](screen, EWRAM_BUF + (life / 4) * 0x480,
                                    point[0] - 12, point[1] - 24, 24, 48);
            if (kind == 55)
              BattleFxKernels_IntegrateVector3(p, 60, 0x400);
            else
              BattleFxKernels_IntegrateVector3(p, 60, 0x1000);
            p->life++;
          }
        }
      }
    }

    if (kind == 8 && frame < 44) {
      EffectPosition_ProjectPoint(origin, point);
      point[0] = point[0] / 2;
      drawFuncs.draw[1](screen, EWRAM_BUF + 0x3c56, point[0] - 10, point[1] - 4,
                        20, 40);
      if (frame == 23)
        (*GetBattleObjectSlotFar(action->target))->unk28 = 0x30000;
      if (frame > 19)
        origin[1] += 0x40000;
      if (frame == 23) {
        half = Battle_GetObjectTableValueFar(action->actor) / 2 + 0x1e0000;
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
        BattleFx_StartActorMotion(action->actor, 1, 0, 0, 0x90000, 100);
        Func_0814cd48(action->actor, 7, 5, 0, 4);
        Func_081180e8(134);
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
          Func_0814cd48(action->actor, 7, 5, 0, 2);
          work->unk77a8 = 2;
          Audio_PlayCue(133);
        }
      }
    }
    if (kind <= 37 && frame <= 5) {
      EffectPosition_ProjectPoint(origin, point);
      point[0] = point[0] / 2;
      drawFuncs.draw[1](screen, EWRAM_BUF + 0x3c56, point[0] - 10, point[1] - 4,
                        20, 40);
      origin[0] += center[0];
      origin[1] += center[1];
      origin[2] += center[2];
    }
    if (kind != 8 && frame == 3)
      Func_081180e8(-1);
    if (frame == 4) {
      if (kind == 91)
        Audio_PlayCue(145);
      else if (kind == 78 || kind == 81 || kind == 98)
        Audio_PlayCue(144);
      else
        Audio_PlayCue(134);
    }
    if (frame == 6) {
      if (kind == 4 || kind == 5 || kind == 7 || kind == 43 || kind == 48 ||
          kind == 73 || kind == 77 || kind == 78 || kind == 49 || kind == 12 ||
          kind == 53 || kind == 64 || kind == 100 || kind == 66 || kind == 81 ||
          kind == 83 || kind == 76 || kind == 98) {
        Func_08118088(action->actor, 4);
        work->unk77a8 = 12;
      } else if (kind == 8 || kind == 79 || kind == 85) {
        work->unk77a8 = 8;
      } else if (kind == 50 || kind == 44 || kind == 63) {
        Func_08118088(action->actor, 1);
        work->unk77a8 = 2;
      } else if (kind == 60 || kind == 38) {
        Func_08118088(action->actor, 3);
        work->unk77a8 = 16;
      } else if (kind == 95 || kind == 99 || kind == 96 || kind == 97 ||
                 kind == 92 || kind == 93 || kind == 86 || kind == 74) {
        Func_08118088(action->actor, 0);
        work->unk77a8 = 12;
      }
    }
    if (frame == 6)
      Func_0814cd48(action->actor, 7, 5, 0, 4);
    if (frame == 14)
      Func_0814cd48(action->actor, 7, 5, 0, 4);
    Func_08158ce0(8, 8);
    ObjectGroup_TickMemberTimers();
    work->unk78e8 = 1;
    WaitFrames(1);
  }

  if (kind == 51 || kind == 67 || kind == 9 || kind == 10 || kind == 11 ||
      kind == 70 || kind == 79 || kind == 90) {
    CLEAR(BG_VRAM, 0x3c00);
    CLEAR(screen, 0x3c00);
    action->unk1c = 0;
    Scheduler_RemoveCallback(Func_08143489);
    Scheduler_RemoveCallback(Func_08143001);
    Runtime_ReleaseHeapBlock(188);
    Runtime_ReleaseHeapBlock(104);
    if (kind == 51 || kind == 10 || kind == 70 || kind == 79 || kind == 90)
      SummonFx_RunCanvasMode1(action);
    else if (kind == 9 || kind == 67)
      SummonFx_RunCanvasMode2(action);
    else if (kind == 11)
      SummonFx_RunCanvasMode3(action);
    return;
  }
  if (kind == 2 || kind == 3 || kind == 42 || kind == 15 || kind == 52 ||
      kind == 72 || kind == 58 || kind == 59 || kind == 69 || kind == 89)
    Scheduler_RemoveCallback(Func_08152475);
  Scheduler_RemoveCallback(Func_08143001);
  Runtime_ReleaseHeapBlock(188);
  Runtime_ReleaseHeapBlock(104);
  BattleFx_EndCanvasLayer();
}

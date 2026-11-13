/// @file
/// The Azul summon, the effect steps under gravity and the projection of effect
/// positions onto the screen.
// Part of battle_effect.c.

#include "../battle_effect_internal.h"

struct FxPartyList;
struct MeshTriangle;
struct Model;
struct StripQuad;

void BattleEffect_StartCanvasLayer(s32 bgControl);
void BattleEffect_EndCanvasLayer(void);
void BattleEffect_CountDownBattlerTints(void);
s32 BattleEffect_LoadBlitter(s32 slot,
                             s32 modeAndFlags); // BattleEffect_BuildBlitter
void BattleEffect_TintBattler(s32 actor, s32 palette, s32 pose, s32 timerSlot,
                              s32 timerFrames);
void Picture_MirrorIntoQuadrants(u8 *src, u8 *dst, s32 width, s32 height);
void BattleEffect_RunDjinniEntrance(struct BattleEffectArgument *effect,
                                    u32 packed, s32 *screen, s32 *world);
void Canvas_BlitAtFoot(void *cells, s32 x, s32 y, s32 size);
struct PoolEntry *Render_AllocateModelList(s32 count);
void Render_TransformVertices(const void *corners, void *vertices, s32 count);
void Render_DrawModelList__ability_effects(struct Model *model);
void SceneTransform_ApplyPosition3(s32 x, s32 y, s32 z);
void SceneTransform_ApplyUniformScale(s32 scale);
void Transform_ApplyPitch(s32 angle);
void Transform_ApplyYaw(s32 angle);
void SceneTransform_ApplyScale3(s32 x, s32 y, s32 z);
void EffectPosition_ProjectBattler(s32 id, struct EffectPosition *out);
void BattleEffect_SpawnObjects(s32 entryCount, s32 kind,
                               u32 variant); // SpawnObjects
void BattleEffect_SelectLivingTargets(
    struct FxPartyList *party); // SelectLivingTargets
void Transform_ApplyRoll(s32 angle);
void BattleEffect_StartActorMotion(s32 id, s32 force, s32 acceleration,
                                   s32 speedLimit, s32 velocityY, s32 percent);

void BattleEffect_FetchRectangleBlitters(s32 flip, void **out);
void BattleMotion_ApplyKnockbackFar(
    s32 actor, s32 motion); // BattleMotion_ApplyVariantMotionFar
void BattlePlayback_StartFar(s32 phase);
void BattleCamera_Shake(s32 rangeX, u32 rangeY);
void BattleEffectKernels_IntegrateVector2(struct EffectStep *step, s32 gravity,
                                          s32 drag);

void Render_BuildQuadStrip(struct StripQuad *quad, s32 width, s32 height,
                           s32 count);
void BattleEffectKernels_IntegrateVector3(struct EffectStep *step, s32 damping,
                                          s32 gravity);
s32 BattleUnit_GetEffectHeightFar(s32 unit);
void BattleEffect_StartTiledCanvas(s32 bgControl);
void BattleEffect_StartTiledCanvasFilled(s32 bgControl);
void Transform_Pop(void);

// The blitter's heap slot, as a byte offset into Ram_HeapSlots.
#define SLOT_BLITTER 104 // unk64[1]

#define BLITTER(mode, flags) ((mode) << 4 | (flags))
#define HI(v) (((s16 *)&(v))[1])

struct AzulPoint {
  s32 x;
  s32 y;
};

struct AzulVertex {
  s8 x;
  s8 y;
  s8 z;
  s8 unk3;
};

struct AzulFace {
  u8 corner[3];
  u8 unk3;
  u8 uv[3][2];
  u8 unkA[2];
};

// A sprite object as Azul edits it: its tile number in the low ten bits of
// the halfword at 8, its priority bits in the byte at 9 and its shape at 18.
struct AzulObject {
  u8 unk0[8];
  u16 tile : 10;
  u16 priority : 2;
  u16 unk08 : 4;
  u8 unk0a[8];
  u16 shape;
};

extern char ResourceId_Data134;
extern char ResourceId_BlueBeamSheet;
extern char ResourceId_WispSheet;
extern char ResourceId_CornerRuneASheet;
extern char ResourceId_Data0C2;
extern char ResourceId_Data0B4;
extern u8 gMapCellBuffer[];
extern u8 gData_02014000[];
extern const u8 Data_081986f2[][2];
extern const u8 Data_08198828[];
extern const u8 Data_08198830[];
extern const u8 Data_0819883c[];
extern const u8 Data_08198848[];
extern const s8 Data_08198850[];
extern const s8 Data_08198858[];
extern const u8 Data_081991e0[];
extern const u8 gData_08199340[];
extern const u8 gData_08199364[];

/* VIEW(f15cde8): routine EffectSprite_Acquire as struct AzulObject
 * *EffectSprite_Acquire(s32 width, s32 height, u32 shape, s32 mode); the
 * definition has struct EffectSprite *EffectSprite_Acquire(s32 kind, s32
 * value); tried as the definition's struct EffectSprite__f15b3b0
 * *EffectSprite_Acquire(s32 kind, s32 value), which the object already
 * declares, it does not compile in tla-ja:
 * games/tla/src/battle_effect/battle_effect.c:23109: too many arguments to
 * function `EffectSprite_Acquire'. */
extern struct AzulObject *
EffectSprite_Acquire__f15cde8(s32 width, s32 height, u32 shape,
                              s32 mode) __asm__("EffectSprite_Acquire");

void BattleEffect_ConfigureEffectDisplay(void);
void BattleEffect_ArmPaletteHBlankDma(void);
void Mesh_BuildGrid(struct MeshTriangle *triangles, s32 columns, s32 rows,
                    s32 cellWidth, s32 cellHeight);
void ColorBuffer_ScaleFar(void *source, void *destination, s32 scale,
                          s32 count);
void BattleBackground_LoadFar(s32 layer, s32 background, s32 palette);
void ReleaseBattleObjectRecordsFar(s32 actor);
void *GetMotionRecordFar(void *object, s32 index);
void Sprite_RemoveAnimationFar(void *record, s32 entry);
void BattleObject_SpawnObjectsForListFar(s16 *actors, s32 a, s32 b);

#define SLOT_ALT 188 // unkb8[1]

// Clamps a sprite's projected y to the screen's band.
#define CLAMP_Y(place)                                                         \
  do {                                                                         \
    if ((place)[2] < -0x10000)                                                 \
      (place)[2] = -0x10000;                                                   \
    if ((place)[2] > 0x9fffff)                                                 \
      (place)[2] = 0xa00000;                                                   \
  } while (0)

// Azul: the sea rises into a wall of water, a great serpent coils out of it
// and swallows the foes, then the wave breaks over them in a spray.
/// Azul: the sea rises into a wall of water, a great serpent coils out of it
/// and swallows the foes, then the wave breaks over them in a spray.
void BattleEffect_RunAzul(struct BattleEffectArgument *effect) {
  u8 *canvas;
  struct BattleEffectWork *work;
  u8 *sprites;
  DrawRectangle blit2;
  DrawRectangle blit;
  s32 frame;
  u8 *battle;
  struct AzulPoint base;
  struct AzulPoint pos;
  struct AzulPoint vel;
  struct AzulVertex *verts;
  struct AzulFace *faces;
  u8 *sphere;
  u8 *grid;
  s32 i;
  s32 j;
  s32 k;
  s32 n;
  u32 t;
  u32 u;
  s32 place[4];
  struct ModelTexture texture;
  struct ModelTexture texture2;
  struct EffectStep *p;
  struct RenderModel *model;
  void *vertices;
  struct AzulObject *obj;
  const u8 *xs;
  u8 *mapCells;
  u8 *planes;
  s32 stripEnd;

  canvas = Ram_HeapSlots->battleFxWork[1];
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  sprites = Ram_HeapSlots->unk64[0];
  BattleEffect_ConfigureEffectDisplay();
  BattleEffect_StartCanvasLayer(3);
  REG_BLDCNT = 0x3f44;
  BattleEffect_LoadBlitter(SLOT_BLITTER, 3);
  blit = Ram_HeapSlots->unk64[1];
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, sprites, 0, 0);
  BattleEffect_SelectLivingTargets(work->effect);
  work->transferMode = 1;
  work->transferValue = 0;
  Task_Add(BattleEffect_ArmPaletteHBlankDma, 0xcfe);
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 3200);
  battle = (u8 *)Ram_HeapSlots->battleWork;
  verts = (struct AzulVertex *)work->sheet;
  faces = (struct AzulFace *)(work->sheet + 0x1f0);
  sphere = work->sheet + 0x800;
  grid = work->sheet + 0xc00;
  {
    u8 *cells = gMapCellBuffer;

    for (i = 0; i < 120; i++) {
      for (n = 0; n < 0x800; n += 64) {
        Iwram_CopyWordsEntry(
            cells, (void *)(0x06008000 + ((i / 8 * 256 + (i & 7)) * 8 + n)), 8);
        cells += 8;
      }
    }
  }
  {
    u8 *cells = work->sheet + 0x2bb8;
    u8 *dst;
    s32 count;

    Resource_LoadAndDecompress((s32)&ResourceId_BlueBeamSheet, cells, 1, 1);
    i = 0;
    count = 288;
    dst = work->sheet + 0x2df8;
    for (; i != count; i++) {
      *dst++ = *cells;
      cells += 2;
    }
  }
  wait(1);
  BattleEffect_SpawnObjects(12, 0x28c, 1);
  for (i = 0; i != 16; i++) {
    obj = EffectSprite_Acquire__f15cde8(32, 32, 0x80002000, 0);
    work->objects[i + 12] = obj;
    Iwram_CopyWordsEntry(obj, work->objects[5 + (i & 3)], 24);
    ((struct AzulObject *)work->objects[i])->priority = 1;
  }
  mapCells = gMapCellBuffer;
  planes = gData_02014000;
  stripEnd = 0x3a08;
  for (frame = 0; frame != 228; frame++) {
    if ((gInput.repeat & 3) && frame > 15 && frame <= 185) {
      Iwram_FillWordsEntry(canvas, 0x4000, 0x3f3f3f3f);
      work->transferPending = 1;
      wait(1);
      frame = 186;
      REG_BLDALPHA = 0x1010;
      for (i = 0; i != 28; i++) {
        ((struct AzulObject *)work->objects[i])->shape = 0x4000;
      }
    }
    if (frame <= 8) {
      s32 fade = 24 - frame * 3;

      ColorBuffer_ScaleFar(battle + 0x56c, (void *)&BG_PLTT_COLOR(6, 0),
                           0x10000 - fade * 1092, 128);
    }
    if (frame == 186) {
      base.x = 0x300000;
      base.y = 0x420003;
      vel.x = 0;
      vel.y = -0x60000;
    }
    if (frame == 0) {
      REG_BLDALPHA = 0x10;
      work->transferMode = 1;
      work->transferValue = frame;
      for (i = 0; i != 28; i++) {
        ((struct AzulObject *)work->objects[i])->shape = 0xc000;
      }
      base.x = 0x780000;
      base.y = -0x780000;
      vel.x = 0;
      vel.y = 0xc0000;
      IO_WRITE_QUEUE_PUSH(0x04000000, 0x7741, 0x20000);
      for (n = 0; n != 31; n++) {
        for (i = 0; i != 4; i++) {
          verts[n * 4 + i].x =
              Data_081986f2[n * 5 + i + 1][0] - Data_081986f2[n * 5][0];
          verts[n * 4 + i].y =
              Data_081986f2[n * 5 + i + 1][1] - Data_081986f2[n * 5][1];
          verts[n * 4 + i].z = 0;
        }
        faces[n * 3].corner[1] = 1;
        faces[n * 3].corner[2] = 2;
        faces[n * 3].corner[0] = 0;
        faces[n * 3 + 1].corner[1] = 2;
        faces[n * 3 + 1].corner[2] = 3;
        faces[n * 3 + 1].corner[0] = 0;
        faces[n * 3 + 2].corner[0] = 0;
        faces[n * 3 + 2].corner[1] = 0;
        faces[n * 3 + 2].corner[2] = 0;
        for (i = 0; i != 3; i++) {
          faces[n * 3].uv[i][0] =
              Data_081986f2[n * 5 + faces[n * 3].corner[i] + 1][0];
          faces[n * 3].uv[i][1] =
              Data_081986f2[n * 5 + faces[n * 3].corner[i] + 1][1];
          faces[n * 3 + 1].uv[i][0] =
              Data_081986f2[n * 5 + faces[n * 3 + 1].corner[i] + 1][0];
          faces[n * 3 + 1].uv[i][1] =
              Data_081986f2[n * 5 + faces[n * 3 + 1].corner[i] + 1][1];
        }
      }
      for (i = 0; i != effect->count; i++) {
        ((s32 *)BattleObject_GetFar(effect->actors[i])->object)[3] = 0;
      }
      REG_BG2CNT = 0x787;
    }
    t = frame - 10;
    if (t <= 16) {
      REG_BLDALPHA = t << 8 | 0x10;
    }
    if ((u32)(frame - 16) <= 63) {
      if (frame == 16) {
        s32 mask;

        i = 0;
        mask = 31;
        p = &work->particles[31];
        for (; i != 27; i++, p++) {
          p->x = ((Random16() & 63) + 32) << 16;
          p->y = ((Random16() & 15) + 120) << 16;
          p->velocityX = ((Random16() & mask) - 16) << 13;
          p->velocityY = ((Random16() & mask) - 63) << 12;
          p->variant = (Trig_Cos(i << 9) << 5 >> 16) - 32;
        }
      }
      i = 0;
      p = &work->particles[31];
      for (; i != 27; i++, p++) {
        if (p->variant >= 0) {
          s32 s = Trig_Sin((frame << 10) + i * 0x1a16);

          blit(canvas, work->sheet + 0x2df8, ((p->x + (s << 4)) >> 16) - 6,
               HI(p->y) - 12, 12, 24);
          BattleEffectKernels_IntegrateVector2(p, 63, -0x4000);
        }
        p->variant++;
      }
    }
    if (frame <= 95 || (u32)(frame - 160) <= 31) {
      u16 *c = (u16 *)(work->unk7300 + 0x340);
      s32 level = frame / 8;

      if (frame > 159) {
        level = (frame - 160) / 4 + 12;
        if (level > 20) {
          level = 20;
        }
      } else if (level > 12) {
        level = 12;
      }
      for (i = 0; i != 15; i++) {
        *c++ = 0;
      }
      for (; i != 135; i++) {
        s32 d = i - 15;
        s32 b = -(d / 15) - level + 40;
        s32 g = -(d / 16) - level + 32;
        s32 r = -(d / 17) - level + 16;

        if (b < 0) {
          b = 0;
        }
        if (b > 31) {
          b = 31;
        }
        if (g < 0) {
          g = 0;
        }
        if (g > 31) {
          g = 31;
        }
        if (r < 0) {
          r = 0;
        }
        if (r > 31) {
          r = 31;
        }
        *c++ = b << 10 | g << 5 | r;
      }
      for (; i != 160; i++) {
        *c++ = 0;
      }
    }
    // DEAD: frame never falls below 0; the source keeps the test.
    if (frame >= 0) {
      base.x += vel.x;
      base.y += vel.y;
    }
    if (frame == 168) {
      base.x = 0x300000;
      base.y = 0xa80000;
      vel.x = 0;
      vel.y = -0x60000;
      for (i = 0; i != 28; i++) {
        ((struct AzulObject *)work->objects[i])->shape = 0x4000;
      }
      for (i = 0; i != effect->count; i++) {
        s32 *actor = BattleObject_GetFar(effect->actors[i])->object;

        BattleEffect_StartActorMotion(effect->actors[i], 1, 0, 0, 0x40000, 100);
        actor[18] = 0;
      }
    }
    if (frame == 186) {
      vel.y = -0x100000;
    }
    if ((frame < 54 || frame > 167) && frame < 214) {
      place[3] = 0;
      place[1] = 0xff0000;
      if (frame > 167) {
        pos.y -= 0x60000;
      }
      if (frame <= 53) {
        for (i = 0; i != 12; i++) {
          place[0] = ((40 - Data_08198830[i]) << 16) + base.x;
          place[2] = ((128 - Data_0819883c[i] * 31 / 32) << 16) + base.y;
          if (i > 8) {
            place[2] += -0xf00000;
          }
          CLAMP_Y(place);
          Render_ApplyProjectedPlacementFar(work->objects[i], place,
                                            (s32 *)Data_08198828, 0);
        }
        for (i = 0; i != 16; i++) {
          place[0] = ((39 - Data_08198830[(i & 3) + 5]) << 16) + base.x;
          place[2] =
              ((-(Data_0819883c[(i & 3) + 5] * 30 / 32) - i / 4 * 60) << 16) +
              base.y + 0x420000;
          // CLAMP_Y written out: the macro's once-only block holds the clamp's
          // constant after the store (4 of 2209 differ).
          if (place[2] < -0x10000)
            place[2] = -0x10000;
          if (place[2] > 0x9fffff)
            place[2] = 0xa00000;
          obj = work->objects[i + 12];
          obj->tile = ((struct AzulObject *)work->objects[(i & 3) + 5])->tile;
          Render_ApplyProjectedPlacementFar(obj, place, (s32 *)Data_08198828,
                                            0);
        }
      } else {
        for (i = 0; i != 12; i++) {
          place[0] = (Data_08198830[i] << 16) + base.x;
          place[2] = (Data_0819883c[i] << 16) + base.y;
          if (i > 8) {
            place[2] += 0x1000000;
          }
          CLAMP_Y(place);
          Render_ApplyProjectedPlacementFar(work->objects[i], place,
                                            (s32 *)Data_08198828, 0);
        }
        i = 0;
        xs = Data_08198830;
        for (; i != 16; i++) {
          s32 m = i & 3;
          s32 row = m + 5;

          place[0] = (xs[row] << 16) + base.x + -0x10000;
          place[2] =
              ((Data_0819883c[row] + i / 4 * 64) << 16) + base.y + 0x400000;
          CLAMP_Y(place);
          obj = work->objects[i + 12];
          obj->tile = ((struct AzulObject *)work->objects[m + 5])->tile;
          Render_ApplyProjectedPlacementFar(obj, place, (s32 *)Data_08198828,
                                            0);
        }
      }
    } else if (frame >= 54 && frame < 168) {
      s32 x;
      s32 y;

      if (frame == 54) {
        for (i = 0; i != 28; i++) {
          ((struct AzulObject *)work->objects[i])->shape = 0;
        }
        base.x = 0x1000000;
        base.y = 0x480000;
        vel.x = -0x60000;
        vel.y = 0;
      }
      if (frame == 138) {
        vel.x = 0x80000;
      }
      place[3] = 0;
      place[1] = 0xff0000;
      vel.x = vel.x * 56 / 64;
      vel.y = vel.y * 56 / 64;
      if (frame <= 137) {
        pos.x = (Trig_Sin(frame << 9) << 4) + base.x;
        x = pos.x;
        pos.y = (Trig_Cos(frame << 9) << 3) + base.y;
        y = pos.y;
      } else {
        x = base.x + (Trig_Sin(frame << 9) << 4);
        y = (Trig_Cos(frame << 9) << 3) + base.y;
      }
      for (i = 0; i != 8; i++) {
        place[0] = (Data_08198850[i] << 16) + x;
        place[2] = (Data_08198858[i] << 16) + y;
        Render_ApplyProjectedPlacementFar(work->objects[i], place,
                                          (s32 *)Data_08198848, 0);
      }
    }
    if ((u32)(frame - 138) <= 29) {
      s32 d = -0x20000;

      if (frame > 151) {
        d = (frame << 13) + -0x150000;
      }
      pos.x = pos.x + d;
    }
    if (frame == 78) {
      Sound_PlayFar(140);
    }
    if (frame == 138) {
      Sound_PlayFar(104);
    }
    if (frame == 152) {
      Sound_PlayFar(163);
    }
    if (frame == 7) {
      Sound_PlayFar(212);
    }
    if (frame == 10) {
      const u8 *piece;

      Sound_PlayFar(144);
      i = 0;
      p = work->particles;
      piece = Data_081986f2[0];
      for (; i != 31; i++, piece += 10, p++) {
        s32 cx = piece[0] >> 1;
        s32 cy = piece[1];

        p->x = cx << 16;
        p->y = cy << 16;
        p->z = 0;
        p->velocityX = (cx - 60) << 12;
        p->velocityY = (cy - 60) << 10;
        p->velocityZ = ((Random16() & 127) << 2) + 512;
        if (Random16() & 1) {
          p->velocityZ = -p->velocityZ;
        }
      }
      IO_WRITE_QUEUE_PUSH(0x04000000, 0x7541, 0x20000);
    }
    if (frame == 10 || frame == 186) {
      for (i = 0; i != effect->count; i++) {
        struct BattleObj *slot = BattleObject_GetFar(effect->actors[i]);
        s32 *actor = slot->object;
        void *record;

        if (frame == 186) {
          ReleaseBattleObjectRecordsFar(effect->actors[i]);
        }
        if (frame == 10) {
          for (j = 0; (record = GetMotionRecordFar(slot->object, j)) != 0;
               j++) {
            Sprite_RemoveAnimationFar(record, (s32)slot->effectEntry);
          }
          BattleEffect_TintBattler(effect->actors[i], 9, 5, i, 8);
          BattleEffect_StartActorMotion(effect->actors[i], 1, 0, 0,
                                        (Random16() << 1) + 0x10000, 100);
          // record is 0 here; the store reuses its register.
          actor[18] = (s32)record;
        }
      }
      if (frame == 186) {
        effect->actors[effect->count] = 0xff;
        BattleObject_SpawnObjectsForListFar(effect->actors, 0, 0);
      }
    }
    if (frame > 9) {
      for (i = 0; i != effect->count; i++) {
        s32 *actor = BattleObject_GetFar(effect->actors[i])->object;

        actor[10] = actor[10] * 60 / 64;
      }
    }
    if (t <= 43) {
      vertices = Heap_AllocEwram(32);
      model = Render_AllocateModelList(1);
      texture.logWidth = 8;
      texture.logHeight = 7;
      texture.pixels = mapCells;
      model->texture = &texture;
      model->kind = 8;
      model->vertices = vertices;
      model->scrollU = gBgScroll[1].x;
      for (i = 0; i != 31; i++) {
        p = &work->particles[i];
        model->mesh = &faces[i * 3];
        Transform_Reset();
        SceneTransform_ApplyPosition3(p->x + -0x400000, p->y + -0x400000, 0);
        SceneTransform_ApplyScale3(0x8000, 0x10000, 0x8000);
        switch (i & 3) {
        case 0:
          Transform_ApplyPitch(p->z);
          Transform_ApplyRoll(p->z);
          break;
        case 1:
          Transform_ApplyPitch(p->z);
          Transform_ApplyRoll(-p->z);
          break;
        case 2:
          Transform_ApplyPitch(p->z);
          Transform_ApplyYaw(p->z);
          break;
        case 3:
          Transform_ApplyRoll(p->z);
          Transform_ApplyYaw(-p->z);
          break;
        }
        Render_TransformVertices(&verts[i * 4], vertices, 4);
        BattleEffectKernels_IntegrateVector3(p, 62, 0);
        if (frame >= (8 - i) * 2 + 16) {
          p->velocityY += -0x8000;
        }
        Render_DrawModelList(model);
      }
      Heap_Release(model);
      Heap_Release(vertices);
    }
    if (frame == 78) {
      s32 angle;
      s32 offset;

      Mesh_BuildGrid(grid, 16, 16, 8, 8);
      k = 0;
      angle = 0;
      offset = 0;
      for (; k != 16; angle += 0x888, offset += 64, k++) {
        s32 y = -Trig_Cos(angle) * 48 >> 16;
        s32 radius = Trig_Sin(angle) * 48 >> 16;
        struct AzulVertex *v;

        j = 0;
        v = (struct AzulVertex *)(sphere + offset);
        for (; j != 16; j++) {
          v->x = Trig_Sin(j << 12) * radius >> 16;
          v->y = y;
          v->z = Trig_Cos(j << 12) * radius >> 16;
          v++;
        }
      }
      Resource_LoadAndDecompress((s32)&ResourceId_WispSheet, gData_02014000, 1,
                                 0);
      for (i = 0; i != 6; i++) {
        for (k = 0; k != 16; k++) {
          for (j = 0; j != 64; j++) {
            ((u8 *)work)[stripEnd + (i * 64 - j) * 16 + k] =
                (planes + i * 2048 + k * 128)[j];
          }
        }
      }
      Resource_LoadAndDecompress((s32)&ResourceId_CornerRuneASheet,
                                 gData_02014000, 1, 0);
      Picture_MirrorIntoQuadrants(gData_02014000, gMapCellBuffer, 64, 64);
      Resource_LoadAndDecompress((s32)&ResourceId_Data0C2, gData_02014000, 0,
                                 0);
      work->transferMode = 2;
      work->transferValue = 50;
    }
    if (frame == 152) {
      Resource_LoadAndDecompress((s32)&ResourceId_Data0B4, gData_02014000, 0,
                                 0);
    }
    u = frame - 80;
    if (u <= 107) {
      s32 scale;
      s32 shade;
      s32 size;

      REG_BG2CNT = 0x784;
      if ((u32)(frame - 168) <= 17) {
        s32 y;
        s32 f;
        s32 width;
        s32 height;

        BattleEffect_LoadBlitter(SLOT_ALT, 7);
        blit2 = Ram_HeapSlots->unkb8[1];
        i = 0;
        y = (base.y >> 16) - 32;
        width = 16;
        height = 64;
        f = frame;
        for (; i != 2; i++, f += 3) {
          u8 *cells = work->sheet + 0x2f18 + (f % 6 << 10);

          blit(canvas, cells, 30, y, width, height);
          blit2(canvas, cells, 46, y, width, height);
        }
        HeapSlot_Release(SLOT_ALT);
      }
      if (frame > 185) {
        goto spray;
      }
      model = Render_AllocateModelList(1);
      scale = u * 3 << 8;
      shade = 0;
      if (scale > 0x10000) {
        scale = 0x10000;
      }
      texture2.logWidth = 7;
      texture2.logHeight = 7;
      texture2.pixels = mapCells;
      model->kind = 7;
      model->texture = &texture2;
      model->mesh = grid;
      model->vertices = verts;
      model->shade = shade;
      model->scrollU = shade;
      model->scrollV = shade;
      Transform_Reset();
      SceneTransform_ApplyPosition3(pos.x / 2 + -0x440000, pos.y + -0x500000,
                                    0);
      SceneTransform_ApplyScale3(scale, scale * 2, scale);
      SceneTransform_ApplyPosition3(-0x280000, 0, 0);
      Transform_ApplyPitch(frame << 10);
      Transform_ApplyYaw(frame << 10);
      Render_TransformVertices(sphere, verts, 256);
      Render_DrawModelList(model);
      if (frame > 137) {
        shade = 0x5a4 - frame * 10;
        size = (frame << 13) + -0x110000;

        if (shade > 0) {
          shade = 0;
        }
        if (shade > -64) {
          texture2.pixels = gData_02014000;
          texture2.logWidth = 7;
          model->mesh = gData_08199364;
          // A once-only block: its end parts the scheduler's rows, as the
          // native's order needs (without it, 6 of 2209 differ).
          do {
            texture2.logHeight = 7;
          } while (0);
          model->shade = shade;
          Transform_Reset();
          SceneTransform_ApplyPosition3(0x2c0000, 0, 0);
          Transform_ApplyYaw(0x3448);
          SceneTransform_ApplyUniformScale(size);
          Render_TransformVertices(Data_081991e0, verts, 4);
          Render_DrawModelList(model);
        }
      }
      if (frame > 151) {
        work->shakeFrames = 2;
        shade = 0;
        size = (frame << 13) + -0x130000;
        texture2.logWidth = 6;
        texture2.logHeight = 6;
        texture2.pixels = gData_02014000;
        model->shade = shade;
        model->mesh = gData_08199340;
        Transform_Reset();
        SceneTransform_ApplyPosition3(-0x100000, pos.y + -0x500000, 0);
        SceneTransform_ApplyScale3(size >> 1, size, size >> 1);
        Transform_ApplyRoll(frame << 11);
        Render_TransformVertices(Data_081991e0, verts, 4);
        Render_DrawModelList(model);
      }
      Heap_Release(model);
    }
    if (frame > 185) {
    spray:
      if (frame == 186) {
        struct EffectStep *drop = (struct EffectStep *)gMapCellBuffer;

        for (i = 0; i != 256; i++, drop++) {
          s32 angle = i << 8;
          s32 r = Random16() & 127;
          s32 vy;

          drop->velocityX = Trig_Sin(angle) * r >> 5;
          vy = Trig_Cos(angle) * r >> 4;
          drop->x = drop->velocityX * 6 + 0x300000;
          drop->y = vy * 5;
          drop->velocityY = vy;
          drop->variant = (Random16() & 31) + 32;
        }
      }
      p = (struct EffectStep *)gMapCellBuffer;
      for (i = 0; i != 256; i++, p++) {
        if (p->variant >= 0) {
          blit(canvas, work->sheet + 0x2df8, HI(p->x) - 6, HI(p->y) - 12, 12,
               24);
          BattleEffectKernels_IntegrateVector2(p, 63, 0x10000);
          if (p->y > 0x600000) {
            p->velocityY = -p->velocityY / 2;
            p->velocityX <<= 2;
            p->variant -= 8;
          }
        }
        p->variant--;
      }
    }
    if (frame == 186) {
      BattleBackground_LoadFar(1, *(u16 *)(battle + 0x670), 24);
      Iwram_FillWordsEntry(canvas, 0x4000, 0x3f3f3f3f);
      IO_WRITE_QUEUE_PUSH(0x04000000, 0x7741, 0x20000);
      for (i = 0; i != effect->count; i++) {
        BattleEffect_TintBattler(effect->actors[i], 7, 5, i, 16);
        BattleMotion_ApplyKnockbackFar(effect->actors[i], 4);
      }
      Sound_PlayFar(145);
      work->shakeFrames = 16;
    }
    if (frame == 214) {
      BattlePlayback_StartFar(134);
    }
    if (frame > 185) {
      BattleCamera_Shake(8, 8);
    } else {
      BattleCamera_Shake(2, 2);
    }
    if (frame > 185) {
      BattleEffect_CountDownBattlerTints();
    }
    work->transferPending = 1;
    wait(1);
  }
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  Task_Remove(BattleEffect_ArmPaletteHBlankDma);
  HeapSlot_Release(SLOT_BLITTER);
  for (i = 0; i != 28; i++) {
    Sprite_DestroyFar(work->objects[i]);
  }
  BattleEffect_EndCanvasLayer();
}

void Particle_AdvanceWithGravity3D(struct EffectStep *step, s32 damping,
                                   s32 gravity) {
  s32 vx = step->velocityX;
  s32 vy = step->velocityY;
  s32 vz = step->velocityZ;

  step->x += vx;
  step->y += vy;
  step->z += vz;
  SET_VELOCITY(step, vx * damping / 64, (vy + gravity) * damping / 64,
               vz * damping / 64);
}

void Particle_AdvanceWithGravity2D(struct EffectStep *step, s32 damping,
                                   s32 gravity) {
  s32 x = step->x;
  s32 vx = step->velocityX;
  s32 vy = step->velocityY;

  step->x = x + vx;
  step->y += vy;
  step->velocityX = vx * damping / 64;
  step->velocityY = (vy + gravity) * damping / 64;
}

s32 EffectPosition_ProjectPoint(const void *point,
                                struct EffectPosition *position) {
  s32 result = Render_ProjectPoint(point, EffectPosition_Coordinates(position));
  position->y -= 16;
  return result;
}

void EffectPosition_ProjectSprite(s32 id, struct EffectPosition *position) {
  BattleUnit_ProjectToScreen(id, EffectPosition_Coordinates(position));
  position->y -= 16;
}

void EffectPosition_ProjectScaledBattler(s32 id,
                                         struct EffectPosition *position) {
  BattleMotion_ProjectScaledPositionFar(id,
                                        EffectPosition_Coordinates(position));
  position->y -= 16;
}

void EffectPosition_ProjectBattler(s32 id, struct EffectPosition *position) {
  BattleMotion_ProjectPositionFar(id, EffectPosition_Coordinates(position));
  position->y -= 16;
}

struct BattleFxWorkTail {
  u8 unk0000[0x78e4];
  s32 active;
  s32 unk78e8;
  s32 unk78ec;
  struct BattleEffectArgument *effect;
};

// Midpoint between the first and last targets.
void BattleEffect_GetTargetsMidpoint(struct EffectPosition *position) {
  struct BattleFxWorkTail *work;
  struct EffectPosition first;
  struct EffectPosition last;

  work = (struct BattleFxWorkTail *)Ram_HeapSlots->battleFxWork[0];
  EffectPosition_ProjectBattler(work->effect->actors[0], &first);
  EffectPosition_ProjectBattler(work->effect->actors[work->effect->count - 1],
                                &last);
  position->x = first.x + (last.x - first.x) / 2;
  position->y = first.y;
  position->depth = first.depth;
}
#undef SLOT_BLITTER
#undef BLITTER
#undef HI
#undef SLOT_ALT
#undef CLAMP_Y

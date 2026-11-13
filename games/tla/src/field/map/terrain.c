// ---- games/tla/src/field/map/terrain.c ----
#include "battle.h"
#include "task.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "ram_buffer.h"
#include "map.h"
#include "scene.h"
#include "dma.h"
#include "heap_release.h"
#include "io_reg.h"
#include "ram_code.h"

extern u32 gMapBlocks[];

u8 GetWorldMapTerrainBehavior(struct WorldPosition *position,
                              s32 *terrainKind) {
  s32 selector = GetWorldMapCollision(position);
  s32 x = position->x;
  s32 y;
  s32 flag = 0;
  u32 tileX;
  u32 index;
  u32 *tile;
  u32 kind;

  if (x < 0)
    x += 0x1fffff;
  tileX = (x >> 21) & 31;
  y = position->y;
  if (y < 0)
    y += 0x1fffff;

  index = tileX + (((y >> 21) & 31) << 5);
  tile = gMapBlocks + index;
  if (((u8 *)tile)[3] & 0x80)
    flag = 0x10;

  kind = (*tile << 2) >> 26;
  *terrainKind = kind;
  if (kind > 40)
    flag = 0x20;
  if (kind == 36)
    flag = 0x40;
  if (kind == 12 || kind == 15)
    flag = 0x30;

  return gWorldMap_TerrainBehaviorTable[flag + selector];
}

void Map_SetScreenShake(s32 value0, s32 value1, s32 value2) {
  struct MapWork *work;

  work = Ram_HeapSlots->mapWork;
  if (value0 >= 0) {
    work->shakeX = value0;
  }
  if (value1 >= 0) {
    work->shakeY = value1;
  }
  if (value2 >= 0) {
    work->shakeDecay = value2;
  }
}

#define FIELD_AT_OFFSET(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

void Map_WaitForShakeToSettle(void) {
  s32 *work;
  s32 cnt;

  work = (s32 *)Ram_HeapSlots->mapWork;
  cnt = 0;
  if (work[1] > 255 || work[2] > 255) {
    goto body;
  body:
    wait(1);
    ++cnt;
    if (cnt >= 300) {
      goto done;
    }
    if (work[1] > 255) {
      goto body;
    }
    if (work[2] > 255) {
      goto body;
    }
  }
done:
  work[3] = 0;
}

extern const u8 Render_DrawMapCode[];
extern u8 Render_DrawMapCodeSize[];
extern u8 gData_0201c000[];
extern u8 gData_0203c000[];

void Func_0802dd08(s32 arg0, s32 arg1) {
  void (*routine)(s32, s32, u8 *, u8 *);
  u8 *tiles = gData_0201c000;

  RamCode_Load(routine, Render_DrawMapCode, Render_DrawMapCodeSize);
  routine(arg0, arg1, gData_0203c000, tiles + 0x1000);
  Heap_Release(routine);
}

#include "iwram_call.h"
#include "fixed_math.h"

struct ScanlineRow {
  s32 scale;
  s32 depth;
  s32 offsetX;
  s32 offsetY;
  s32 unk10;
};

// An inline function over the fixed address, as ☀️'s iwram_call.h reaches
// this entry (☀️'s returns the entry's s32).
#undef Iwram_TransformVector
static inline void Iwram_TransformVector(const void *vector, s32 *result) {
  Iwram_TransformVectorEntry(vector, result);
}

void Func_0802dd70(s32 pitchSlope, s32 *position, struct ScanlineRow *row) {
  s32 vector[3];
  s32 result[3];
  s32 height;
  s32 nearZ;
  s32 i;
  s32 z;
  s32 ratio;
  s32 dx;
  s32 sum;
  s32 zero;

  zero = 0;
  vector[0] = position[0];
  vector[1] = zero;
  vector[2] = position[2];
  /* FAKEMATCH(wrapper): the call through this file's inline; as the direct
   * Iwram_TransformVector macro call, 41 of 132 instructions differ in
   * tla-en. */
  Iwram_TransformVector(vector, result);
  height = result[1] - Iwram_MulQ16(result[2], pitchSlope);
  nearZ = -gCameraSceneParameters[0];
  for (i = 0; i < 160; i++) {
    s32 distance;

    z = Iwram_RatioMulQ14(nearZ, (gCameraSceneParameters[4] - i) * Q16_ONE);
    distance = z - pitchSlope;
    if (distance == 0)
      distance = 1;
    ratio = Iwram_RatioMulQ14(distance, height);
    if (ratio < 0) {
      row->scale = Iwram_RatioMulQ14(gCameraSceneParameters[0],
                                     Iwram_MulQ16(-ratio, Q16_ONE / 2));
      z = Iwram_MulQ16(ratio, z);
      dx = (result[2] - ratio) >> 4;
      z -= result[1];
      sum = Iwram_MulQ16(dx, dx);
      z >>= 4;
      sum += Iwram_MulQ16(z, z);
      {
        s32 length = Iwram_Sqrt(sum) << 12;

        if (z < 0)
          length = -length;
        row->depth = Iwram_MulQ16(length, Q16_ONE / 2);
      }
    } else {
      row->scale = 0;
      row->depth = 0;
    }
    row->offsetX = 0;
    row->offsetY = 0;
    row++;
  }
}

// The debug sprite viewer.

#include "input.h"
#include "module/system/system.h"

struct SpriteViewerLayer {
  s16 spriteId;
  s16 animationIndex;
  u8 enabled;
  s8 priority;
  s8 colorMode;
};

struct ViewerMetadata {
  u8 unk00[4];
  u8 drawKind; ///< 20 for a sprite drawn in two parts
};

struct ViewerSprite {
  u8 unk00[0x1a];
  u8 flags;
  u8 unk1b[0x1d];
};

extern u8 gDecodeFillByte;
extern u8 gDebugMode;
extern u32 gMapCellBuffer[];
extern u32 Data_02018000[];
extern u8 gResource_Data012[];
extern const u16 gData_0802f054[8];

void VramCache_Reset(void);
void Func_080230e0(s32 mode);
s32 ResourceSlot_Load(u32 slot, u32 *buffer, s32 number, u32 variant);
/* VIEW(terrain): routine Resource_GetMetadataRecordFar as struct ViewerMetadata *Resource_GetMetadataRecordFar(s32 id); sprite_metadata.h has struct AnimationMetadata *Resource_GetMetadataRecordFar(u32 number); tried as the definition's struct AnimationMetadata *Resource_GetMetadataRecordFar(u32 id), it does not compile in tla-ja: games/tla/src/field/map/terrain.c:285: dereferencing pointer to incomplete type. */
extern struct ViewerMetadata *Resource_GetMetadataRecordFar__terrain(s32 id) __asm__("Resource_GetMetadataRecordFar");
struct ViewerSprite *ResourceObject_Create(s32 id);
s32 ResourceMetadata_Register(struct ViewerSprite *sprite, s32 id);
void SpriteViewer_SetGridColumnPriority(s32 slot, s32 value);
void SpriteViewer_SetGridColumnColorMode(s32 slot, s32 value);
void SpriteViewer_StartStaggeredAnimations(s32 slot, s32 value);
void SpriteViewer_SetGridColumnNumber(s32 slot, s32 value);
s32 Func_0802e444(s32 spriteId, s32 direction);
struct SpritePlacement;
void SpriteViewer_BuildProbeRing(s32 x, s32 z, struct SpritePlacement *out);
void Battle_PlaceActorsByFormationKind(void);
void Fade_FromBlack(s32 vblanks);

extern char ResourceId_Data012;

// The native reads the held keys afresh at each test.
#define HELD (*(volatile u32 *)&gInput.held)

/// The debug sprite viewer, in The Broken Seal's form with this game's
/// changes; it never returns.
void Func_0802de8c(void) {
  struct SpriteViewerLayer layers[4];
  volatile u32 zero;
  s32 x;
  s32 y;
  u32 editMode;
  s32 shadowsEnabled;
  u32 backdrop;
  void *ring;
  u32 selectedLayer;
  u32 i;
  s32 spriteId;
  s32 changed;

  x = 144;
  y = 96;
  editMode = 1;
  selectedLayer = 0;
  shadowsEnabled = 1;
  backdrop = 0;
  ring = HeapSlot_AllocEwram(36, 160);
  gDecodeFillByte = 3;
  Dma_Fill32Cell(zero, 0, ring, 4);
  Dma_Fill32Cell(zero, 0, layers, sizeof(layers));
  spriteId = Func_0802e444(-1, 1);
  for (i = 0; i < 4; i++) {
    layers[i].animationIndex = 1;
    layers[i].priority = 1;
    layers[i].spriteId = spriteId;
  }
  layers[0].enabled = 1;
  gDebugMode = 2;
  REG_BLDCNT = 0x3f42;
  *(volatile u16 *)0x05000000 = 0x7c00;
  REG_DISPCNT = 0x1140;
  Fade_FromBlack(1);

restart:
  Heap_Initialize();
  Task_ResetAll();
  ring = HeapSlot_AllocEwram(36, 160);
  VramCache_Reset();
  Func_080230e0(2);
  ResourceSlot_Load(0, gMapCellBuffer, layers[0].spriteId, 0);
  if (Resource_GetMetadataRecordFar__terrain(layers[0].spriteId)->drawKind == 20)
    ResourceSlot_Load(1, Data_02018000, layers[0].spriteId + 1, 0);
  for (i = 0; i < 10; i++) {
    struct ViewerSprite *sprite;
    s32 part = 0;

    if (Resource_GetMetadataRecordFar__terrain(layers[0].spriteId)->drawKind == 20 &&
        (i & 1))
      part = 1;
    sprite = ResourceObject_Create(layers[0].spriteId + part + (part << 12));
    ResourceMetadata_Register(sprite, layers[1].spriteId);
    ResourceMetadata_Register(sprite, layers[2].spriteId);
    ResourceMetadata_Register(sprite, layers[3].spriteId);
    sprite->flags = shadowsEnabled;
  }
  for (i = 0; i < 4; i++) {
    if (layers[i].enabled != 0)
      SpriteViewer_SetGridColumnPriority(i, layers[i].priority);
    else
      SpriteViewer_SetGridColumnPriority(i, 8);
    SpriteViewer_SetGridColumnColorMode(i, layers[i].colorMode);
    {
      s32 a;

      // Once-only block: the animation index is read before i moves into r0.
      do {
        a = layers[i].animationIndex;
      } while (0);
      SpriteViewer_StartStaggeredAnimations(i, a);
    }
  }
  SpriteViewer_BuildProbeRing(x, y, ring);
  Task_Add(Battle_PlaceActorsByFormationKind, 0x480);

next:
  wait(1);
  for (;;) {
    if (HELD & 8) {
      if (HELD & 32)
        x--;
      if (HELD & 16)
        x++;
      if (HELD & 64)
        y--;
      if (HELD & 128)
        y++;
      SpriteViewer_BuildProbeRing(x, y, ring);
    } else {
      // DEAD: the repeated keys are read once and the value goes unused.
      (void)gInput.repeat;
      if (gInput.repeat & 64)
        selectedLayer = (selectedLayer - 1) & 3;
      if (gInput.repeat & 128)
        selectedLayer = (selectedLayer + 1) & 3;
      if (gInput.repeat & 32)
        editMode = (editMode - 1) & 3;
      if (gInput.repeat & 16)
        editMode = (editMode + 1) & 3;
    }

    if (gInput.pressed & 8) {
      struct ViewerSprite *sprites = Ram_HeapSlots->unk0c[1];

      shadowsEnabled ^= 1;
      for (i = 0; i < 10; i++) {
        sprites->flags = shadowsEnabled;
        sprites++;
      }
    }

    switch (editMode) {
    case 0:
      if (selectedLayer == 1)
        break;
      if (gInput.repeat & 0x200) {
        layers[selectedLayer].animationIndex--;
        if (layers[selectedLayer].animationIndex < 0)
          layers[selectedLayer].animationIndex = 0;
        if (selectedLayer == 0)
          layers[1].animationIndex = layers[0].animationIndex;
        goto next;
      }
      if (!(gInput.repeat & 0x100))
        break;
      layers[selectedLayer].animationIndex++;
      if (layers[selectedLayer].animationIndex > 99)
        layers[selectedLayer].animationIndex = 99;
      if (selectedLayer == 0)
        layers[1].animationIndex = layers[0].animationIndex;
      goto next;

    case 1: {
      s32 steps;

      changed = 0;
      steps = 1;
      if (HELD & 8)
        steps = 10;
      if (gInput.repeat & 0x200) {
        layers[selectedLayer].spriteId =
            Func_0802e444(layers[selectedLayer].spriteId, -steps);
        changed = 1;
      }
      if (gInput.repeat & 0x100) {
        layers[selectedLayer].spriteId =
            Func_0802e444(layers[selectedLayer].spriteId, steps);
        changed = 1;
      }
      if (changed == 0)
        break;
      if (selectedLayer == 0)
        goto restart;
      if (layers[selectedLayer].enabled != 0) {
        SpriteViewer_SetGridColumnNumber(selectedLayer,
                                         layers[selectedLayer].spriteId);
        SpriteViewer_SetGridColumnPriority(selectedLayer,
                                           layers[selectedLayer].priority);
        SpriteViewer_SetGridColumnColorMode(selectedLayer,
                                            layers[selectedLayer].colorMode);
        SpriteViewer_StartStaggeredAnimations(
            selectedLayer, layers[selectedLayer].animationIndex);
      }
      goto next;
    }

    case 2:
      changed = 0;
      if (gInput.repeat & 0x200) {
        layers[selectedLayer].priority--;
        if (layers[selectedLayer].priority < 0)
          layers[selectedLayer].priority = 3;
        changed = 1;
      }
      if (gInput.repeat & 0x100) {
        layers[selectedLayer].priority++;
        if (layers[selectedLayer].priority > 3)
          layers[selectedLayer].priority = 0;
        changed = 1;
      }
      if (changed == 0)
        break;
      if (layers[selectedLayer].enabled != 0)
        SpriteViewer_SetGridColumnPriority(selectedLayer,
                                           layers[selectedLayer].priority);
      goto next;

    case 3:
      changed = 0;
      if (gInput.repeat & 0x200) {
        layers[selectedLayer].colorMode--;
        if (layers[selectedLayer].colorMode < 0)
          layers[selectedLayer].colorMode = 15;
        changed = 1;
      }
      if (gInput.repeat & 0x100) {
        layers[selectedLayer].colorMode++;
        if (layers[selectedLayer].colorMode > 15)
          layers[selectedLayer].colorMode = 0;
        changed = 1;
      }
      if (changed == 0)
        break;
      if (layers[selectedLayer].enabled != 0)
        SpriteViewer_SetGridColumnColorMode(selectedLayer,
                                            layers[selectedLayer].colorMode);
      goto next;
    }

    if (gInput.repeat & 1) {
      if (editMode == 0) {
        if (selectedLayer != 1) {
          if (layers[selectedLayer].enabled != 0) {
            SpriteViewer_StartStaggeredAnimations(
                selectedLayer, layers[selectedLayer].animationIndex);
            if (selectedLayer == 0)
              SpriteViewer_StartStaggeredAnimations(1,
                                                    layers[1].animationIndex);
          }
          goto next;
        }
      } else if (selectedLayer != 0) {
        layers[selectedLayer].enabled ^= 1;
        SpriteViewer_SetGridColumnPriority(selectedLayer,
                                           layers[selectedLayer].enabled != 0
                                               ? layers[selectedLayer].priority
                                               : 8);
        goto next;
      }
    }
    if (gInput.repeat & 4) {
      Task_ResetAll();
      System_ReturnZero((s32)&ResourceId_Data012, gResource_Data012);
      goto restart;
    }
    if (gInput.pressed & 2) {
      volatile u16 *palette = (volatile u16 *)0x05000000;
      const u16 *colors = gData_0802f054;
      backdrop++;
      backdrop &= 7;
      // Spelled as an address sum: colors[backdrop] swaps the load's registers.
      *palette = *(const u16 *)((u32)colors + (backdrop << 1));
    }
    wait(1);
  }
}

extern const u16 gData_0802f064[];

/// Steps a number from 0 to 715 by delta: a step of one wraps around the
/// range, a larger step forward goes to the next stop above value and one back
/// to the last stop below it (gData_0802f064, ended by 0xffff, from which a
/// step forward wraps to 0). A delta of 0 returns value unchanged.
s32 Func_0802e444(s32 value, s32 delta) {
  s32 step;

  if (delta != 0) {
    step = delta;
    if (delta < 0)
      step = -delta;
    if (step == 1) {
      value += delta;
      if (value < 0)
        value = 715;
      if (value >= 716)
        value = 0;
    } else if (delta > 0) {
      u32 i;

      for (i = 0; i < 10; i++) {
        if (value < gData_0802f064[i]) {
          value = gData_0802f064[i];
          if (value == 0xffff)
            value = 0;
          break;
        }
      }
    } else if (value == 0) {
      value = gData_0802f064[8];
    } else {
      s32 i;

      for (i = 8; i >= 0; i--) {
        if (value > gData_0802f064[i]) {
          value = gData_0802f064[i];
          break;
        }
      }
    }
    return value;
  }
#ifdef UBFIX
  return value;
#else
  // UB: no return statement for a delta of 0; the native leaves value in r0.
#endif
}

#include "sprite.h"
#include "render_placement.h"

extern s32 gVBlankCount;

struct ProbeShape {
  u8 unk00[4];
  u8 kind;
};

struct ProbeObjects {
  u8 unk00[40];
  struct ProbeShape *shape;
};

void Vector_AddPolarOffset(s32 radius, s32 angle, s32 *point);

/// Lays out the sprite viewer's ring around pixel (x, z): one position per
/// facing of the first sprite's facing kind (6, 8, 10 or 4, or two opposite
/// pairs for ANIM_FACING_TWO_PART), 56 pixels out (40 for a two-part
/// sprite). B held turns the ring. Called by SpriteViewer_Run when it starts
/// and while Start moves the ring.
void SpriteViewer_BuildProbeRing(s32 centerX, s32 centerY, struct SpritePlacement *positions) {
  struct SpritePlacement *pair;
  s32 angle;
  s32 i;
  u16 turn;
  u8 kind;

  angle = 0;
  kind = ((struct ProbeObjects *)Ram_HeapSlots->unk0c[1])->shape->kind;
  if ((gInput.held & 2) != 0)
    angle = (s16)(gVBlankCount << 8);
  switch ((u32)kind) {
  case 3:
    for (i = 0; i < 6; i++) {
      angle <<= 16;
      positions->x = centerX << 16;
      angle = (u32)angle >> 16;
      positions->height = 0;
      positions->y = centerY << 16;
      Vector_AddPolarOffset(0x380000, (u16)angle, &positions->x);
      positions++;
      angle = (s16)(angle + 0x2aaa);
    }
    for (; i < 10; i++)
      positions++;
    break;
  case 5:
  case 8:
  case 44:
  case 88:
    for (i = 0; i < 8; i++) {
      angle <<= 16;
      positions->x = centerX << 16;
      angle = (u32)angle >> 16;
      positions->height = 0;
      positions->y = centerY << 16;
      Vector_AddPolarOffset(0x380000, (u16)angle, &positions->x);
      positions++;
      angle = (s16)(angle + 0x2000);
    }
    for (; i < 10; i++)
      positions++;
    break;
  case 4:
  case 6:
    for (i = 0; i < 10; i++) {
      angle <<= 16;
      positions->x = centerX << 16;
      angle = (u32)angle >> 16;
      positions->height = 0;
      positions->y = centerY << 16;
      Vector_AddPolarOffset(0x380000, (u16)angle, &positions->x);
      positions++;
      angle = (s16)(angle + 0x1999);
    }
    break;
  case 20:
    turn = angle;
    angle = (s16)(turn + 0x4000);
    i = 0;
    pair = positions;
    while (i < 2) {
      pair[i * 2].x = centerX << 16;
      pair[i * 2].height = 0;
      pair[i * 2].y = centerY << 16;
      angle <<= 16;
      angle = (u32)angle >> 16;
      Vector_AddPolarOffset(0x280000, angle, &positions[0].x);
      pair[i * 2 + 1].x = centerX << 16;
      {
        s32 *point = &positions[1].x;
        pair[i * 2 + 1].height = 0;
        pair[i * 2 + 1].y = centerY << 16;
        Vector_AddPolarOffset(0x280000, (u16)angle, point);
      }
      angle = (angle + 0x8000) << 16;
      i++;
      positions += 2;
      angle >>= 16;
    }
    break;
  default:
    angle = ((angle << 16) + 0x20000000) >> 16;
    for (i = 0; i < 4; i++) {
      angle <<= 16;
      angle = (u32)angle >> 16;
      positions->x = centerX << 16;
      positions->height = 0;
      positions->y = centerY << 16;
      Vector_AddPolarOffset(0x380000, (u16)angle, &positions->x);
      positions++;
      angle = (s16)(angle + 0x4000);
    }
    for (i = 4; i < 10; i++)
      positions++;
    break;
  }
}
#undef FIELD_AT_OFFSET
#undef HELD

// ---- games/tla/src/field/grid.c ----
#include "sprite_metadata.h"

extern u8 gData_03001e60[];

void SpriteViewer_SetGridColumnColorMode(s32 slot, s32 value) {
  u8 *base = Ram_HeapSlots->unk0c[1];
  s32 offset = (slot & 3) * 4 + 40;
  s32 count = 9;

  do {
    u8 *entry = *(u8 **)(base + offset);

    count--;
    entry[5] = value;
    base += 56;
  } while (count >= 0);
}

void SpriteViewer_SetGridColumnPriority(s32 slot, s32 value) {
  u8 *base = Ram_HeapSlots->unk0c[1];
  s32 offset = (slot & 3) * 4 + 40;
  s32 count = 9;

  do {
    u8 *entry = *(u8 **)(base + offset);

    count--;
    entry[6] = value;
    base += 56;
  } while (count >= 0);
}

struct UiGridEntry {
  s16 no;
  u16 x;
  u8 value04;
  u8 unk05[7];
  s32 *table0c;
  s32 value10;
  u8 value14;
  u8 value15;
  u8 value16;
  u8 value17;
};

void SpriteViewer_StartStaggeredAnimations(s32 slot, s32 value) {
  s32 index;
  s32 count;
  s32 offset;
  struct AnimationMetadata *metadata;
  struct UiGridEntry *entry;
  u8 *work;

  work = Ram_HeapSlots->unk0c[1];
  count = 0;
  offset = ((3 & slot) * 4) + 0x28;
  index = 0;
  do {
    entry = *(struct UiGridEntry **)(work + offset);
    if (entry->table0c != 0) {
      metadata = Resource_GetMetadataRecordFar(entry->no);
      if (value < metadata->animationCount) {
        entry->value04 = metadata->drawKind;
        entry->value10 = entry->table0c[value];
        entry->x = count * 0x10;
        entry->value15 = 0x10;
        entry->value14 = index;
        entry->value17 = index;
        entry->value16 = 0xFF;
      }
      work[23] = (u8)metadata->adjustY;
      *(s16 *)(work + 18) = index;
    }
    count += 1;
    work += 0x38;
  } while (count <= 9);
}

void SpriteViewer_SetGridColumnNumber(s32 slot, s32 no) {
  u8 *base = Ram_HeapSlots->unk0c[1];
  s32 offset;
  s32 count;

  Resource_GetMetadataRecordFar(no);
  offset = (slot & 3) * 4 + 40;
  count = 9;
  do {
    u8 *entry = *(u8 **)(base + offset);

    count--;
    *(u16 *)entry = no;
    Animation_InitializeWorkFromMetadata(entry);
    base += 56;
  } while (count >= 0);
}


struct BattleCells {
  u8 *actors;
  u8 *unk04;
  u8 *unk08;
  u8 *unk0c;
  u8 *unk10;
  u8 *work;
};

struct Vec2 {
  s32 x;
  s32 y;
};

extern struct BattleCells gSpriteObjects;

extern struct Vec2 gBattle_FormationPlacementScale;

void Battle_PlaceActorsByFormationKind(void) {
  u8 *actor = Ram_HeapSlots->unk0c[1];
  u32 kind = (*(u8 **)(actor + 40))[4];
  struct Vec2 scale;
  u8 *table;
  u16 angle;
  u16 step;
  u16 odd = 0;
  u32 cnt;
  u32 i;

  scale = gBattle_FormationPlacementScale;
  table = (u8 *)Ram_HeapSlots->battleWork;

  switch (kind) {
  case 3:
    angle = 0;
    step = 0x2aaa;
    cnt = 6;
    break;
  case 5:
  case 8:
  case 44:
  case 88:
    angle = 0;
    step = 0x2000;
    cnt = 8;
    break;
  case 4:
  case 6:
    angle = 0;
    step = 0x1999;
    cnt = 10;
    break;
  case 20:
    angle = 0;
    step = 0;
    odd = 0x8000;
    cnt = 4;
    break;
  default:
    angle = 0x2000;
    step = 0x4000;
    cnt = 4;
    break;
  }

  for (i = 0; i < cnt; i++) {
    Render_ApplyProjectedPlacement(actor, (s32 *)(table + i * 16),
                                   (s32 *)&scale, angle);
    actor += 56;
    angle += step;
    if (i & 1)
      angle += odd;
  }
}

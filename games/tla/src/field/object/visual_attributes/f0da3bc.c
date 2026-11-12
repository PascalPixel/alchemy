/// @file
/// Routines first written as f0da3bc.c; none has a twin in The Broken Seal.
// Part of visual_attributes.c.

#include "../visual_attributes_internal.h"

s32 ArcTan2(s32 y, s32 x);
void wait(s32 frames);

s32 Func_080da3bc(s32 index, s32 x, s32 y, s32 z, s32 toX, s32 toY, s32 toZ) {
  struct WorkA0 *work = WORK_A0;
  s32 dx = toX - x;
  s32 ex = dx >> 8;
  s32 dy = toY - y;
  s32 ey = dy >> 8;
  s32 dz = toZ - z;
  s32 ez = dz >> 8;
  s32 segments = Iwram_SqrtEntry(Iwram_MulQ16(ex, ex) + Iwram_MulQ16(ey, ey) +
                                 Iwram_MulQ16(ez, ez)) /
                     6 +
                 1;
  s32 frame;
  struct WorkA0Node *node = work->slots[index].head;
  s32 sine;
  s32 height;
  s32 cosine;
  s32 lastX, lastY, lastZ;
  struct WorkA0Node **nodes;
  struct WorkA0Node **cursor;
  struct WorkA0Slot *slot;
  s32 i;
  u16 angle;

  cursor = nodes = Heap_Alloc(0x200);
  if (segments >= 0) {
    i = segments + 1;
    do {
      i += -1;
      /* FAKEMATCH(barrier): an empty asm reading the counter in a low register,
       * which copies it for the compare ahead of the store; without it, 2 of
       * 319 instructions differ in tla-en. */
      asm("" : : "l"(i));
      *cursor++ = node;
      node = node->next;
    } while (i != 0);
  }
  angle = ArcTan2(ex, ey);
  sine = Trig_Sin(angle);
  cosine = Trig_Cos(angle);
  for (frame = 1; frame <= segments * 2; frame++) {
    for (i = segments; i >= 0; i--) {
      node = nodes[i];
      // XXX: byte 17 of a node is the slot it belongs to, plus one; likely
      // what the per-frame callbacks read to draw it.
      node->unk11[0] = index + 1;
      if (i == segments) {
        s32 offsetX;
        s32 offsetY;

        height = Iwram_MulQ16(Trig_Sin((frame << 14) / i), 0x80000);
        offsetX = Iwram_MulQ16(height, cosine);
        lastX = node->x = x + dx * frame / 2 / i + offsetX;
        offsetY = Iwram_MulQ16(height, sine);
        lastY = node->y = y + dy * frame / 2 / i + offsetY;
        lastZ = node->z = z + dz * frame / 2 / i;
      } else {
        ex = (lastX - node->x) >> 8;
        ey = (lastY - node->y) >> 8;
        ez = (lastZ - node->z) >> 8;
        height = Iwram_SqrtEntry(Iwram_MulQ16(ex, ex) + Iwram_MulQ16(ey, ey) +
                                 Iwram_MulQ16(ez, ez));

        if (height <= 6)
          break;
        lastX = node->x = lastX + (node->x - lastX) * 6 / height;
        lastY = node->y = lastY + (node->y - lastY) * 6 / height;
        lastZ = node->z = lastZ + (node->z - lastZ) * 6 / height;
        node->used = 1;
      }
    }
    wait(1);
  }
  slot = &work->slots[index];
  // XXX: the field worka0.h calls tail is tested as a flag by Func_080d9d40;
  // a pointer here only because the adopted header types it so.
  slot->tail = (struct WorkA0Node *)1;
  slot->amplitude = 0x40000;
  slot->angle = 0x4000;
  Heap_Release(nodes);
  return 0;
}


extern s32 gData_080f394c;
s32 Func_080da6e0(s32 slot, s32 x1, s32 y1, s32 z1, s32 x2, s32 y2, s32 z2,
                  s32 wave);

void Func_080da660(s32 arg, s32 firstId, s32 secondId) {
  struct Actor **work;
  struct Actor *first;
  struct Actor *second;

  work = (struct Actor **)WORK_A0;
  first = ObjectTable_Get(firstId);
  second = ObjectTable_Get(secondId);

  if (first != NULL && second != NULL) {
    work[0] = first;
    work[1] = second;
    Func_080da6e0(arg, first->x, first->y + 0x80000, first->z, second->x,
                  second->y + 0x80000, second->z,
                  Trig_Sin(gData_080f394c) << 2);
    gData_080f394c += 0x1000;
  }
}


s32 Func_080da6e0(s32 slot, s32 x1, s32 y1, s32 z1, s32 x2, s32 y2, s32 z2,
                  s32 wave) {
  struct WorkA0 *work = WORK_A0;
  struct WorkA0Node *tail = NULL;
  struct WorkA0Node *node;
  s32 dx, dy, dz;
  s32 dist;
  s32 count;
  s32 frame;
  s32 i;
  s32 angle;
  s32 x, y, z;
  struct WorkA0Node **nodes;
  struct WorkA0Node **entry;

  dx = (x2 - x1) >> 8;
  dy = (y2 - y1) >> 8;
  dz = (z2 - z1) >> 8;
  dist = Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dy, dy) + Iwram_MulQ16(dz, dz);
  count = Iwram_Sqrt(dist) / 6 + 1;
  node = work->slots[slot].head;
  entry = nodes = Heap_Alloc(0x200);
  work->slots[slot].tail = tail;
  if (count >= 0) {
    i = count + 1;
    do {
      i += -1;
      /* FAKEMATCH(barrier): an empty asm reading the counter in a low register,
       * which copies it for the compare ahead of the store; without it, 2 of
       * 259 instructions differ in tla-en. */
      asm("" : : "l"(i));
      *entry++ = node;
      node = node->next;
    } while (i != 0);
  }
  Actor_SetMode(((void **)work)[0], 3);
  Actor_SetMode(((void **)work)[1], 3);
  wait(4);
  for (frame = 0; frame <= count; frame++) {
    angle = frame << 12;
    dist = 0x40000;
    for (i = 0; i <= count; i++) {
      node = nodes[i];
      node->unk11[0] = 0;
      if (i <= frame) {
        x = node->x = x1 + Iwram_MulQ16(Trig_Cos(angle), dist);
        y = node->y = y1;
        z = node->z = z1 + Iwram_MulQ16(Trig_Sin(angle), dist);
        node->used = 2;
        {
          u16 step = ArcTan2(0x60000, dist);

          angle += step;
          dist += step * 4;
        }
      } else {
        dx = (x - node->x) >> 8;
        dy = (y - node->y) >> 8;
        dz = (z - node->z) >> 8;
        dist =
            Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dy, dy) + Iwram_MulQ16(dz, dz);
        dist = Iwram_Sqrt(dist);
        if (dist <= 6)
          break;
        x = node->x = x + (node->x - x) * 6 / dist;
        y = node->y = y + (node->y - y) * 6 / dist;
        node->z = z + (node->z - z) * 6 / dist;
        node->used = 2;
        z = node->z;
      }
    }
    wait(1);
  }
  Heap_Release(nodes);
  return 0;
}


extern s32 gData_080f3950;

void Func_080da060(s32 a0, s32 a1, s32 a2, s32 a3);

void Func_080da908(s32 index) {
  struct WorkA0 *work = WORK_A0;
  struct WorkA0Node *node;

  for (node = work->slots[index].head; node != NULL; node = node->next)
    node->used = 0;
  work->slots[index].head = NULL;
}

void Func_080da938(s32 index) {
  struct WorkA0 *work = WORK_A0;
  struct WorkA0Node *node;

  for (node = work->slots[index].head; node != NULL; node = node->next)
    node->used = 2;
  work->slots[index].tail = NULL;
}

void Func_080da968(struct Actor *object) {
  u8 *parts = object->sprite;

  parts[22] = 253;
  parts[23] = 12;
  if (gInput.repeat & 0x200)
    gData_080f3950 = 1;
  if (gData_080f3950 != 0)
    Func_080da060(0, 12, 13, 0);
}


// The four 256-byte pictures of each set (type-01 compressed).
extern const u8 Data_080f0c1c[], Data_080f0c63[], Data_080f0cb8[];
extern const u8 Data_080f0d07[], Data_080f0d5e[], Data_080f0dbb[];
extern const u8 Data_080f0e00[];

s32 Resource_FindFreeEntry(void);
s32 VramCache_Load(u32 slot, u32 size, const void *source);
void Func_080daab0(void);
void Func_080dacb8(void);

struct TileEffectWork {
  s32 vramEntry;
  u8 unk04[0x10e0];
};

#define TILE_EFFECT_SLOT 164
#define TILE_EFFECT_KIND_ALT 372 // the kind that takes the first picture set
#define PICTURE_SIZE 0x100

/// Starts a field effect that animates sixteen tiles of its own: takes a
/// 0x10e4-byte work block at heap offset 164 (worka0.h's WorkA4 lies there),
/// clears it, decodes four 256-byte tile pictures into a scratch buffer (one
/// set for kind 372, another for the rest; the second set repeats one
/// picture), loads them into a VRAM slot and schedules the two per-frame
/// routines Func_080daab0 and Func_080dacb8.
struct TileEffectWork *Func_080da9a8(s32 kind) {
  struct TileEffectWork *work;
  u8 *pictures;
  s32 size;

  work = HeapSlot_AllocEwram(TILE_EFFECT_SLOT, sizeof(struct TileEffectWork));
  size = 4 * PICTURE_SIZE;
  pictures = Heap_Alloc(size);
  Dma_Fill32(0, work, sizeof(struct TileEffectWork));
  work->vramEntry = Resource_FindFreeEntry();
  if (kind == TILE_EFFECT_KIND_ALT) {
    Resource_DecodeType01(Data_080f0c1c, pictures);
    Resource_DecodeType01(Data_080f0c63, pictures + PICTURE_SIZE);
    Resource_DecodeType01(Data_080f0cb8, pictures + 2 * PICTURE_SIZE);
    Resource_DecodeType01(Data_080f0d07, pictures + 3 * PICTURE_SIZE);
    VramCache_Load(work->vramEntry, size, pictures);
    /* FAKEMATCH(dead): an empty once-only block closing the arm keeps each
     * decode's destination set before its source loads; without it, 4 of 95
     * instructions differ in tla-en. */
    do {
    } while (0);
  } else {
    Resource_DecodeType01(Data_080f0d5e, pictures);
    Resource_DecodeType01(Data_080f0dbb, pictures + PICTURE_SIZE);
    Resource_DecodeType01(Data_080f0dbb, pictures + 2 * PICTURE_SIZE);
    Resource_DecodeType01(Data_080f0e00, pictures + 3 * PICTURE_SIZE);
    VramCache_Load(work->vramEntry, size, pictures);
    /* FAKEMATCH(barrier): an empty asm closing the arm keeps each decode's
     * destination set before its source loads; without it, 4 of 95
     * instructions differ in tla-en. */
    asm("");
  }
  {
    s32 priority = 0x480;

    Task_Add(Func_080daab0, priority);
  }
  Task_Add(Func_080dacb8, 0x48f);
  Heap_Release(pictures);
  return work;
}



// The transform as this function builds it, a word at a time.

struct ChainLink {
  struct ChainLink *next;
  s32 x;
  s32 y;
  s32 z;
  u16 angle;
  s8 kind; ///< 0 or 1: not drawn; else its picture, from 2
  u8 unk13;
  struct AnimationSpritePart sprite;
};

/* VIEW(f0da3bc): tag struct ChainSlot as struct ChainSlot__f0da3bc; visual_attributes.c has another definition; tried with the definition the rest of the object has, it does not compile in tla-ja: games/tla/src/field/object/visual_attributes.c:6634: structure has no member named `root'. */
struct ChainSlot__f0da3bc {
  s16 id;
  u8 unk02[22];
  struct ChainLink *root;
};

/* VIEW(f0da3bc): tag struct ChainWork as struct ChainWork__f0da3bc; visual_attributes.c has another definition; tried with the definition the rest of the object has, it does not compile in tla-ja: games/tla/src/field/object/visual_attributes.c:6624: structure has no member named `sheet'. */
struct ChainWork__f0da3bc {
  s32 sheet; ///< the resource entry of the links' tiles
  struct ChainSlot__f0da3bc slots[8];
};

void Func_080daab0(void) {
  struct ChainWork__f0da3bc *work = Ram_HeapSlots->unk84[8];
  s32 i;
  u32 angle;
  s32 prevX = 0;
  s32 prevY = 0;
  u32 tiles = gResourceTableEntries[work->sheet].offset >> 5;
  s32 prevZ = 0;
  struct MapWork *map = Ram_HeapSlots->mapWork;
  s32 *view = &map->viewX;
  s32 viewX = view[0] & 0xffff0000;
  s32 viewZ = view[1] & 0xffff0000;
  s32 height = map->origin[1];
  struct ChainTransform transform;
  struct ChainLink *link;

  for (i = 0; i < 8; i++) {
    for (link = work->slots[i].root; link != NULL; link = link->next) {
      s32 x = link->x;
      s32 y = link->y;
      s32 z = link->z;

      if (link->kind > 1) {
        s32 dz = prevZ - z;
        s32 absZ = dz >= 0 ? dz : z - prevZ;
        s32 dy = prevY - y;
        s32 screenX, screenY;
        u32 tile;

        if (dy >= 0 ? absZ < dy : absZ < y - prevY)
          angle = (u16)ArcTan2(prevX - x, dy);
        else
          angle = (u16)ArcTan2(prevX - x, dz);
        tile = tiles + link->kind * 8 - 16;
        screenX = (prevX + x) / 2 - viewX;
        prevY = (prevY + y) / 2 - height;
        prevZ = (prevZ + z) / 2 - viewZ - height;
        screenY = prevZ - prevY;
        if ((u32)(screenX + 0x1fffff) <= 0x12ffffe && screenY > -0x200000 &&
            screenY <= 0xdfffff) {
          s32 sx = ((screenX >> 16) - 8) & 0x1ff;
          s32 sy = ((screenY >> 16) - 8) & 0xff;
          s32 priority = ((prevY + prevZ) >> 16) + 34;
          struct AnimationSpritePart *sprite = &link->sprite;

          ((u32 *)sprite)[0] = 0;
          ((u32 *)&link->sprite)[1] = sy | (sx << 16) | 0x40002000;
          ((u32 *)&link->sprite)[2] = tile | 0x800;
          if (angle != 0) {
            transform.angle = angle;
            transform.scaleX = 0x100;
            transform.scaleY = 0x100;
            link->sprite.affine = 1;
            link->sprite.affineIndex = AffineMatrix_BuildForEffect(
                (struct AffineTransform *)&transform);
          }
          link->angle = angle;
          Func_080140d8(sprite, priority);
        }
      }
      prevX = x;
      prevY = y;
      prevZ = z;
    }
    prevX = 0;
    prevY = 0;
    link = work->slots[i].root;
    prevZ = 0;
    if (link != NULL)
      link->angle = angle;
  }
}
#undef TILE_EFFECT_SLOT
#undef TILE_EFFECT_KIND_ALT
#undef PICTURE_SIZE

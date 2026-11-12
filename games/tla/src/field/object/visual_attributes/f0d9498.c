/// @file
/// Routines first written as f0d9498.c; none has a twin in The Broken Seal.
// Part of visual_attributes.c.

#include "../visual_attributes_internal.h"

s32 ArcTan2(s32 y, s32 x);
void GameFlag_SetBit(s32 flag);
void wait(s32 frames);

/* VIEW(f0d9498): tag struct ChainNode as struct ChainNode__f0d9498; visual_attributes.c has another definition; tried with the definition the rest of the object has, it does not compile in tla-ja: games/tla/src/field/object/visual_attributes.c:5629: structure has no member named `visible'. */
struct ChainNode__f0d9498 {
  struct ChainNode__f0d9498 *next;
  s32 x;
  s32 y;
  s32 z;
  u8 unk10[2];
  s8 visible;
  u8 unk13;
};

/* VIEW(f0d9498): tag struct ChainSlot as struct ChainSlot__f0d9498; visual_attributes.c has another definition; tried with the definition the rest of the object has, it does not compile in tla-ja: games/tla/src/field/object/visual_attributes.c:5586: structure has no member named `active'. */
struct ChainSlot__f0d9498 {
  u8 unk00[4];
  s16 segments;
  u8 unk06[2];
  s16 flag;
  s16 active;
  s32 unk0c;
  u8 unk10[4];
  s32 unk14;
  s32 unk18;
  struct ChainNode__f0d9498 *head;
};


s32 Func_080d9498(s32 index, s32 x, s32 y, s32 z, s32 toX, s32 toY, s32 toZ) {
  struct ChainWork *work = Ram_HeapSlots->unk84[6];
  s32 dx = toX - x;
  s32 ex = dx >> 8;
  s32 dy = toY - y;
  s32 ey = dy >> 8;
  s32 dz = toZ - z;
  s32 segments;
  s32 frame;
  s32 sine;
  s32 cosine;
  s32 lastX;
  s32 lastY;
  s32 lastZ;
  struct ChainSlot__f0d9498 *slot = &work->slots[index];
  struct ChainNode__f0d9498 *node = slot->head;
  void **cursor;
  s32 i;
  u16 angle;
  struct ChainNode__f0d9498 **nodes;

  if (slot->active == 1)
    return;
  slot->active = 1;
  segments = slot->segments;
  cursor = nodes = Heap_Alloc(0x200);
  for (i = 0; i <= segments; i++) {
    *cursor++ = node;
    node = node->next;
  }
  angle = ArcTan2(ex, ey);
  sine = Trig_Sin(angle);
  cosine = Trig_Cos(angle);
  for (frame = 1; frame <= segments * 2; frame++) {
    for (i = segments; i >= 0; i--) {
      node = nodes[i];
      node->unk13 = index + 1;
      if (i == segments) {
        ex = Iwram_MulQ16(Trig_Sin((frame << 14) / i), 0x80000);

        node->x = x + dx * frame / 2 / i + Iwram_MulQ16(ex, cosine);
        lastX = node->x;
        node->y = y + dy * frame / 2 / i + Iwram_MulQ16(ex, sine);
        lastY = node->y;
        node->z = z + dz * frame / 2 / i;
        lastZ = node->z;
      } else {
        s32 ez;

        ex = (lastX - node->x) >> 8;
        ey = (lastY - node->y) >> 8;
        ez = (lastZ - node->z) >> 8;
        ex = Iwram_SqrtEntry(Iwram_MulQ16(ex, ex) + Iwram_MulQ16(ey, ey) +
                             Iwram_MulQ16(ez, ez));

        if (ex > 6) {
          node->x = lastX + (node->x - lastX) * 6 / ex;
          lastX = node->x;
          node->y = lastY + (node->y - lastY) * 6 / ex;
          lastY = node->y;
          node->z = lastZ + (node->z - lastZ) * 6 / ex;
          node->visible = 1;
          lastZ = node->z;
        }
      }
    }
    wait(1);
  }
  slot = &work->slots[index];
  slot->unk18 = 1;
  slot->unk0c = 0x40000;
  slot->unk14 = 0x4000;
  if (slot->flag != 0)
    GameFlag_SetBit(slot->flag);
  Heap_Release(nodes);
  return 0;
}

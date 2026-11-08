// Matches: differ 0 (tla-en, 2026-11-08)
// Also 0 in tla-ja, tla-de, tla-es, tla-fr and tla-it. Func_02001ad0 of overlay
// 64c: overlay 64b's Func_02004634 (the linked pair of effects, ported from
// ☀️'s WorldMap_CreateAttachedEffects) under this overlay's names, written out
// here rather than included.
// One tagged device: the sprite written through LinkedSpriteView. Bit 0 of the
// byte at 0x11 is retainCacheEntry, as AnimationObject and LayeredSprite name
// it, set through a one-bit field instead of `unk11 |= 1`, whose bit-field
// store needs a byte zero mask; CSE took that from motionFlags' zero mask,
// which kept it alive into the sprite block so local-alloc put it in r3
// across motionFlags' address and global gave the address r1. Without it the
// address gets r3, and reload's move2add derives the action's address from it
// (adds r3, #15), as the native does. The honest form needs no device:
// FieldSprite's byte 0x11 split as `retainCacheEntry:1; unk11:7` and its
// flipX/flipY as `size:2` (OAM size, as AnimationSpritePart declares those
// bits) measures 0 in all six editions with plain member writes; that split
// belongs to games/tla/include/field_sprite.h and needs make
// compare-editions, so it is left for adoption, together with 64b's twin.
#include "types.h"
#include "call.h"
#include "ram_buffer.h"
#include "actor.h"
#include "field_sprite.h"
#include "vram_tab.h"
#include "actor_view.h"
#include "graphics/resource.h"

struct LinkedDetail {
  u8 unk00[22];
  u8 unk16;
};

struct LinkedEffectWork {
  u8 unk000[0x7c0];
  u16 vramBlock;
};

struct LinkedSpriteView {
  u8 unk00[6];
  u16 x : 9;
  u16 affineIndex : 5;
  u16 size : 2;
  u8 unk08[9];
  u8 retainCacheEntry : 1;
  u8 unk11 : 7;
};

struct Actor *Func_02001fcc(s32 kind, s32 x, s32 y, s32 z);
void Func_02001a34(struct Actor *object);
void Func_02001a80(struct Actor *object);

void Func_02001ad0(struct Actor *parent) {
  struct Actor *pair[2];
  struct Actor *child;
  struct FieldSprite *sprite;
  struct LinkedEffectWork *work = Ram_HeapSlots->effectWork;
  s32 i;

  for (i = 0; i < 2; i++) {
    child = Func_02001fcc(271, parent->x, parent->y, parent->z);
    pair[i] = child;
    if (child != NULL) {
      child->terrainHeight = parent->terrainHeight;
      sprite = child->animation;
      child->motionFlags = 0;
      child->action = 0;
      child->watchedObject = parent;
      if (sprite != NULL) {
        Animation_ApplyChildArgument((struct ActorSpriteView *)sprite, 0);
        sprite->nextFrame = 0;
        Resource_ResetEntry(sprite->vramBlock);
        sprite->vramBlock = work->vramBlock;
        /* FAKEMATCH(signature): the sprite written through LinkedSpriteView,
         * which splits the byte at 0x11 and attribute 1's bits 14-15 that
         * FieldSprite leaves whole; as sprite->unk11 |= 1, flipX = 0 and
         * flipY = 1, 44 of 120 instructions differ in tla-en. */
        ((struct LinkedSpriteView *)sprite)->retainCacheEntry = 1;
        sprite->tile =
            (gResourceTableEntries[sprite->vramBlock].offset >> 5) & 0x3ff;
        sprite->fullColor = 0;
        sprite->shape = 1;
        ((struct LinkedSpriteView *)sprite)->size = 2;
        ((struct LinkedDetail *)sprite->record)->unk16 = 0;
      }
    }
  }
  {
    struct Actor *p = pair[0];
    struct FieldSprite *sp = p->animation;

    p->update = (void *)Func_02001a80;
    sp->priority = 1;
  }
  {
    struct Actor *p = pair[1];
    struct FieldSprite *sp = p->animation;

    sp->priority = 1;
    p->update = (void *)Func_02001a34;
    p->priorityFlags = 2;
  }
}

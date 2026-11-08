// Matches: differ 0 (tla-en, 2026-11-08)
// Also 0 in tla-ja, tla-de, tla-es, tla-fr and tla-it; no device. Func_02001ad0
// of overlay 64c: overlay 64b's Func_02004634 (the linked pair of effects,
// ported from ☀️'s WorldMap_CreateAttachedEffects) under this overlay's names,
// now written out here rather than included. The same source with 64b's names
// measures 0 against Func_02004634 too.
// From 15 (8 November): bit 0 of the byte at 0x11 is set through a one-bit
// field (keepEntry, as 6ac's and 6ad's drafts type it) instead of
// `unk11 |= 1`, whose bit-field store needs a byte zero mask; CSE took that
// from motionFlags' zero mask, which kept it alive into the sprite block so
// local-alloc put it in r3 across motionFlags' address and global gave the
// address r1. Without it the address gets r3, and reload's move2add derives
// the action's address from it (adds r3, #15), as the native does. Then the
// native store order (terrainHeight, animation, motionFlags, action,
// watchedObject) matches, and the loop can be a plain for loop again.
#include "types.h"
#include "call.h"
#include "ram_buffer.h"
#include "actor.h"
#include "field_sprite.h"
#include "vram_tab.h"

struct LinkedDetail {
  u8 unk00[22];
  u8 field16;
};

struct LinkedSprite {
  struct FieldSprite sprite;
  struct LinkedDetail *detail;
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
  u8 keepEntry : 1;
  u8 unk11 : 7;
};

struct Actor *Func_02001fcc(s32 kind, s32 x, s32 y, s32 z);
void Animation_ApplyChildArgument(void *sprite, s32 argument);
void Resource_ResetEntry(s32 entry);
void Func_02001a34(struct Actor *object);
void Func_02001a80(struct Actor *object);

void Func_02001ad0(struct Actor *parent) {
  struct Actor *pair[2];
  struct Actor *child;
  struct LinkedSprite *part;
  struct FieldSprite *sprite;
  struct LinkedEffectWork *work = Ram_HeapSlots->effectWork;
  s32 i;

  for (i = 0; i < 2; i++) {
    child = Func_02001fcc(271, parent->x, parent->y, parent->z);
    pair[i] = child;
    if (child != NULL) {
      child->terrainHeight = parent->terrainHeight;
      part = child->animation;
      child->motionFlags = 0;
      child->action = 0;
      child->watchedObject = parent;
      if (part != NULL) {
        sprite = &part->sprite;
        Animation_ApplyChildArgument(sprite, 0);
        sprite->nextFrame = 0;
        Resource_ResetEntry(sprite->vramBlock);
        sprite->vramBlock = work->vramBlock;
        ((struct LinkedSpriteView *)sprite)->keepEntry = 1;
        sprite->tile =
            (gResourceTableEntries[sprite->vramBlock].offset >> 5) & 0x3ff;
        sprite->fullColor = 0;
        sprite->shape = 1;
        ((struct LinkedSpriteView *)sprite)->size = 2;
        ((struct LinkedDetail *)sprite->record)->field16 = 0;
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

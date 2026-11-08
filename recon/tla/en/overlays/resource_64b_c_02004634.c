// Matches: differ 0 (tla-en, 2026-11-08)
// The linked pair of effects, ported from ☀️'s WorldMap_CreateAttachedEffects
// (⚓️ creates kind 271, its updates are Func_020045e4 and Func_02004598, the
// sprite's detail is its record at 0x28). Two things closed it, with no
// device: the child's stores in ☀️'s order (terrainHeight, animation,
// motionFlags, action), and the keep-cache-entry bit written as a one-bit
// field. As `sprite->unk11 |= 1` the byte store masks the old byte with a
// zero register, CSE takes motionFlags' QI zero for it, and that zero, dead
// after combine, is local-allocated to r3 inside the motionFlags address's
// life; the address prefers STACK_REG, so global-alloc places it after and
// it lands in r1, and reload's move2add never turns the action's address into
// `adds r3, #15` (differ 15; in the old order 15, in ☀️'s order 30). The
// plain for loop and a FieldSprite pointer straight from the animation also
// match now. All six editions measure differ 0 (2026-11-08). Adopting it
// wants FieldSprite's unk11 split into its bits in field_sprite.h, which
// takes the SpriteCacheBits cast away.
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

struct LinkedEffectWork {
  u8 unk000[0x7c0];
  u16 vramBlock;
};

/// FieldSprite's byte 0x11 read as its bits.
struct SpriteCacheBits {
  u8 retainCacheEntry : 1; ///< keeps the VRAM cache entry when it is released
};

struct WorldMapOam {
  u8 unk00[6];
  u16 x : 9;
  u16 affineIndex : 5;
  u16 size : 2;
};

struct Actor *Func_02004b30(s32 kind, s32 x, s32 y, s32 z);
void Animation_ApplyChildArgument(void *sprite, s32 argument);
void Resource_ResetEntry(s32 entry);
void Func_02004598(struct Actor *object);
void Func_020045e4(struct Actor *object);

void Func_02004634(struct Actor *parent) {
  struct Actor *pair[2];
  struct Actor *child;
  struct FieldSprite *sprite;
  struct LinkedEffectWork *work = Ram_HeapSlots->effectWork;
  s32 i;

  for (i = 0; i < 2; ++i) {
    child = Func_02004b30(271, parent->x, parent->y, parent->z);
    pair[i] = child;
    if (child != NULL) {
      child->terrainHeight = parent->terrainHeight;
      sprite = child->animation;
      child->motionFlags = 0;
      child->action = 0;
      child->watchedObject = parent;
      if (sprite != NULL) {
        Animation_ApplyChildArgument(sprite, 0);
        sprite->nextFrame = 0;
        Resource_ResetEntry(sprite->vramBlock);
        sprite->vramBlock = work->vramBlock;
        // A one-bit field: as a byte `|= 1` its zero mask moves the
        // motionFlags address out of r3 (differ 15).
        ((struct SpriteCacheBits *)&sprite->unk11)->retainCacheEntry = 1;
        sprite->tile =
            (gResourceTableEntries[sprite->vramBlock].offset >> 5) & 0x3ff;
        sprite->fullColor = 0;
        sprite->shape = 1;
        ((struct WorldMapOam *)sprite)->size = 2;
        ((struct LinkedDetail *)sprite->record)->field16 = 0;
      }
    }
  }
  {
    struct Actor *p = pair[0];
    struct FieldSprite *sp = p->animation;

    p->update = (void *)Func_020045e4;
    sp->priority = 1;
  }
  {
    struct Actor *p = pair[1];
    struct FieldSprite *sp = p->animation;

    sp->priority = 1;
    p->update = (void *)Func_02004598;
    p->priorityFlags = 2;
  }
}

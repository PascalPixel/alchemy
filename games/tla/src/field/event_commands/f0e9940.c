#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "effect_sprite.h"

#define OBJ_PALETTE_14 ((u16 *)0x050003c0)

// The glow's strength, in frames: full at 112, and it fades out a frame at a
// time once the player leaves the place that feeds it.
#define GLOW_FULL 112
#define GLOW_SPENT_CUE 0x2099

// Kind 24's work (battle_fx_work[0]).
struct GlowWork {
  u8 unk0000[0x1000];
  struct EffectSprite sprite;
  s32 scale;
  u8 unk102c[2];
  u16 tile;
  u8 unk1030[2];
  u16 phase;
  s16 fading;
};

struct GlowEvent {
  u8 unk000[0x166];
  u16 cue;
  u8 unk168[0xbdc];
  s16 glow;
  s16 glowHeld;
};

extern u32 gFrameCount;

s32 Party_GetLeaderActor(void);
s32 Func_080dbcc0(s32 x, s32 z, s32 layer);
s32 BattleEffect_GetObjectPriority(struct Actor *object);
s32 BattleEffect_GetObjectDepth(struct Actor *object);
s32 Func_080e989c(s32 counter, s32 level);
void Camera_WorldToScreen(s32 *position);

// Kind 24's frame callback: keeps the glow's strength full while the player
// stands where Func_080dbcc0 allows, and runs it down otherwise (once it is
// fading, the glow shrinks with it and the event takes its cue when it is
// spent). It cycles OBJ palette 14 through Func_080e989c's colours, flickers
// between two pictures every other frame, and draws the glow under the
// player, flattened to three quarters of its height.
void Func_080e9940(void) {
  struct GlowWork *work = Ram_HeapSlots->battleFxWork[0];
  struct Actor *player = ObjectTable_Get(Party_GetLeaderActor());
  struct GlowEvent *event = Ram_HeapSlots->eventWork;
  u16 *palette;
  s32 position[3];
  s32 level;
  s32 i;
  s32 scaleX;
  s32 scaleY;
  struct EffectSprite *sprite;

  // The glow's strength.
  if (Func_080dbcc0(player->x, player->z, player->terrainId) == 0) {
    if (event->glowHeld != 0)
      event->glow--;
  } else {
    event->glow = GLOW_FULL;
  }

  if (work->fading == 1) {
    level = event->glow;
    if (level > GLOW_FULL)
      level = GLOW_FULL;
    if (level < 0)
      level = 0;
    work->scale = (level << 15) / GLOW_FULL + 0x8000;
    if (level == 0)
      event->cue = GLOW_SPENT_CUE;
  }

  // Cycle the glow's colours; the last takes the 15th's level.
  palette = OBJ_PALETTE_14;
  for (i = 0; i < 16; i++) {
    // The colour's store in a once-only block, which ends the scheduler's
    // region there; as the plain store, 2 of 194 instructions differ in
    // tla-en.
    do {
      *palette++ = Func_080e989c((((s16)work->phase >> 2) + i / 2) % 24,
                                 i == 15 ? 14 : i);
    } while (0);
  }
  work->phase++;

  // Draw it under the player.
  scaleX = work->scale;
  scaleY = scaleX * 6 / 8;
  sprite = &work->sprite;
  sprite->oam.index = work->tile + ((gFrameCount >> 1) & 1) * 32;
  sprite->oam.priority = BattleEffect_GetObjectPriority(player);
  sprite->depth = BattleEffect_GetObjectDepth(player) - 1;
  position[0] = player->x;
  position[1] = player->y;
  position[2] = player->z;
  Camera_WorldToScreen(position);
  sprite->x = position[0];
  sprite->y = position[2];
  sprite->scaleX = scaleX;
  sprite->scaleY = scaleY;
  Func_080eb01c(sprite);
}

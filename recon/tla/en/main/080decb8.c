// Near miss, differ 24 of 91 (2026-10-15), The Broken Seal's
// BattleFx_RunFlashingCallbackSequence: Cloak. ⚓️ takes the caster from the
// effect scene, sets its mode directly and schedules the ground check at
// 0x480 where ☀️ uses 0xc80.
// Left: allocation. The native counts the flicker in r5 and keeps the 2 in
// r6, which also stores blend = 2 after the loop; this swaps them, and the
// stores after the loop come in another order.
// Tried: ☀️'s do/while with n reused as the zero after the loop, differ 59
// (n then lives in a high register); a for loop and literal zeros, 24.
#include "types.h"
#include "ram_buffer.h"
#include "fx_scene.h"
#include "party_state.h"
#include "object_runtime.h"
#include "animspr.h"

// ⚓️'s composite sprite (as polar_target.c's CompositeAnimation).
struct CloakSprite {
  u8 unk00[0x19];
  u8 dirty; // redraw the parts
  u8 blend; // 2 draws the caster half transparent, 1 shows it plainly
  u8 unk1b[0x0d];
  struct AnimationEntry *entries[4];
};

struct CloakCaster {
  u8 unk00[0x50];
  struct CloakSprite *sprite;
  u8 unk54[0x18];
  void (*hook)(void);
};

// The field event work's pending Psynergy request (eventWork + 0x166).
struct CloakEvent {
  u8 unk000[0x166];
  s16 psynergyRequest;
};

void Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 order);
void FieldFx_CheckCloakTile(void);
void Func_080debd8(void);
void WaitFrames(s32 frames);

#define SE_CLOAK 130

// The caster's palette while it flickers.
#define CLOAK_FLICKER_PALETTE 7

// What the ground check (FieldFx_CheckCloakTile) requests when the caster does not
// stand on a tile it can hide on.
#define CLOAK_END 0x2092

// Cloak the caster: flicker it ten times, leave it half transparent, then
// check the ground under it every frame. When it cannot hide where it
// stands, show it again at once.
void Func_080decb8(void) {
  struct CloakEvent *event = Ram_HeapSlots->eventWork;
  struct CloakCaster *caster =
      ((struct BattleFxScene *)Ram_HeapSlots->effectWork)->mainObject;
  struct CloakSprite *sprite = caster->sprite;
  struct AnimationEntry *entry = sprite->entries[0];
  u32 n;
  void (*check)(void);

  Audio_PlayCue(SE_CLOAK);
  Object_SetMode((struct ObjectRuntime *)caster, 0);
  caster->hook = 0;

  for (n = 0; n < 10; n++) {
    entry->param = CLOAK_FLICKER_PALETTE;
    sprite->dirty = 1;
    sprite->blend = 2;
    WaitFrames(2);
    sprite->dirty = 1;
    sprite->blend = 0;
    WaitFrames(2);
  }

  sprite->blend = 2;
  entry->param = 0;
  sprite->dirty = 1;
  check = FieldFx_CheckCloakTile;
  Scheduler_AddOrUpdateCallback(check, 0x480);
  gPartyState.cloaked = 1;
  check();
  if (event->psynergyRequest == CLOAK_END) {
    Func_080debd8();
    event->psynergyRequest = 0;
  }
}

// Matches: differ 0 of 91 (tla-en, 2026-10-30).
// The Broken Seal's FieldPsynergy_StartCloak: Cloak. ⚓️ takes the caster from
// the effect scene, sets its mode directly and schedules the ground check in
// TASK_PASS_VBLANK where ☀️ uses TASK_PASS_FRAME.
// The last two instructions (r1's shift before r0's copy) came from declaring
// Task_Add with its real s32 return (system/task.h) instead of void: the call
// then sets r0, so the check's copy into r0 loses one dependant and sched2's
// tie falls to the native order (was differ 2 of 91, 2026-10-26).
// The task priority through a local set before the stores after the loop
// (found by alchemy-devtools permute, tagged) gives the native's allocation: the
// flicker count in r5, the 2 in r6.
// Tried (with the s32 return, 2026-10-30): the priority as a literal, 18; the
// count as s32 (counts down), 10; as u8 (masks), 6; direct calls without the
// check pointer, 18; the check set at its declaration, 78; ☀️'s do/while with
// n reused as the zero after the loop, 55.
#include "types.h"
#include "ram_buffer.h"
#include "field_psynergy_work.h"
#include "main_routine_work.h"
#include "actor.h"
#include "sprite.h"
#include "system/task.h"

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

void FieldEffect_CheckCloakTile(void);
void Func_080debd8(void);
void wait(s32 frames);

#define SE_CLOAK 130

// The caster's palette while it flickers.
#define CLOAK_FLICKER_PALETTE 7

// What the ground check (FieldEffect_CheckCloakTile) requests when the caster does not
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
  s32 order;

  Sound_PlayFar(SE_CLOAK);
  Actor_SetMode((struct Actor *)caster, 0);
  caster->hook = 0;

  for (n = 0; n < 10; n++) {
    entry->param = CLOAK_FLICKER_PALETTE;
    sprite->dirty = 1;
    sprite->blend = 2;
    wait(2);
    sprite->dirty = 1;
    sprite->blend = 0;
    wait(2);
  }

  /* FAKEMATCH(temporary): the task priority through a local set before the
   * stores, which gives the flicker count r5 and the 2 r6; as a literal
   * TASK_PASS_VBLANK in the call, 18 of 91 instructions differ in tla-en. */
  order = TASK_PASS_VBLANK;
  sprite->blend = 2;
  entry->param = 0;
  sprite->dirty = 1;
  check = FieldEffect_CheckCloakTile;
  Task_Add(check, order);
  gMainRoutineWork.cloaked = 1;
  check();
  if (event->psynergyRequest == CLOAK_END) {
    Func_080debd8();
    event->psynergyRequest = 0;
  }
}

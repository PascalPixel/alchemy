// Near miss, differ 4 of 79 (2026-10-15), The Broken Seal's
// MapEvent_RunTileTriggerSequence: the end of Cloak. ⚓️ takes the caster
// from the effect scene instead of the selected actor and sets its mode
// directly.
// Left: the last statement. The native stores the blend before it starts
// the cloak flag's offset (movs r1, #183) and loads the party state between
// the offset's halves; this starts the offset first.
// Tried: the party state through a pointer after an empty asm (the form
// polar_target.c tags) moves the address load but sets the zero first,
// still differ 4; the zero as i - 5 differ 7; a volatile store, or a pointer
// to the flag, differ 4.
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

void Scheduler_RemoveCallback(void (*callback)(void));
void FieldEffect_CheckCloakTile(void);
void WaitFrames(s32 frames);

#define SE_CLOAK_END 154

// The caster's palette while it flickers.
#define CLOAK_FLICKER_PALETTE 7

// Show the Cloaked caster again: stop checking the ground under it, then
// flicker it back into view over twenty frames and clear the cloak.
void Func_080debd8(void) {
  struct CloakCaster *caster =
      ((struct BattleFxScene *)Ram_HeapSlots->effectWork)->mainObject;
  struct CloakSprite *sprite = caster->sprite;
  struct AnimationEntry *entry = sprite->entries[0];
  u32 i;

  Audio_PlayCue(SE_CLOAK_END);
  Scheduler_RemoveCallback(FieldEffect_CheckCloakTile);
  Object_SetMode((struct ObjectRuntime *)caster, 0);
  caster->hook = 0;

  // Blink between the flicker palette half transparent and solid...
  for (i = 0; i < 5; ++i) {
    entry->param = CLOAK_FLICKER_PALETTE;
    sprite->blend = 2;
    sprite->dirty = 1;
    WaitFrames(2);
    sprite->dirty = 1;
    sprite->blend = 0;
    WaitFrames(2);
  }

  // ...then between the flicker palette and the caster's own.
  for (i = 0; i < 5; ++i) {
    entry->param = CLOAK_FLICKER_PALETTE;
    sprite->blend = 0;
    sprite->dirty = 1;
    WaitFrames(2);
    entry->param = 0;
    sprite->dirty = 1;
    WaitFrames(2);
  }

  sprite->blend = 1;
  gPartyState.cloaked = 0;
}

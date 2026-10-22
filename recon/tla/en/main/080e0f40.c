// Near miss, differ 2 of 49 (one reordered pair): the twin of The Broken Seal's
// BattleEffect_UpdateDescendingParticlePositiveArc, on object_runtime.h's fields.
// The listing loads the 0xfffe0000 literal before source->x (ldr r4, =-0x20000;
// ldr r3, [r6, #8]); this loads source->x first. Measured: without the party
// pointer and its one-pass block, differ 4 (particle->terrainHeight is also
// loaded after 0x1fa is built); the floor in a local, the -0x20000 in a local,
// and an empty asm barrier before the x store, each the same 2. No statement
// order in the tail moved the pair. A forced -0x20000 through an empty asm
// after the scale stores loads first, but reallocates the tail (differ 16
// pinned to r4, more unpinned; 2026-10-15). 080e0fb0 is the same function with
// the signs turned.
#include "types.h"
#include "object_runtime.h"
#include "party_state.h"
#include "scene_ids.h"

void Object_Destroy(struct ObjectRuntime *object);

void BattleEffect_UpdateDescendingParticlePositiveArc(
    struct ObjectRuntime *particle) {
  struct ObjectRuntime *source;
  s32 y;
  s32 limit;
  s32 scale;
  struct PartyState *party = &gPartyState;

  /* FAKEMATCH: the one-pass block after the party pointer loads the particle's
   * floor before 0x1fa is built (one reordered pair, differ 2 more without it).
   */
  do {
  } while (0);

  limit = particle->terrainHeight + 0xA0000;
  source = particle->linkedObject;
  if (party->map == (s32)&SceneId_Ov64a)
    limit = particle->terrainHeight + 0x40000;

  y = particle->y;
  if (y <= limit) {
    Object_Destroy(particle);
    return;
  }

  scale = particle->scaleX + 0xC00;
  if (scale > 0x10000)
    scale = 0x10000;
  particle->scaleX = scale;
  particle->scaleY = scale;
  particle->x = source->x;
  particle->y = y - 0x20000;
  particle->z = source->z + ((0x10000 - scale) * 5) + 0x90000;
}

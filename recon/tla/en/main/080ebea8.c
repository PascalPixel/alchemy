/* 2026-10-03 effect-slot suffix probe.
 * Reused the current TBS EFFECT43 owner and the TLA child-argument veneer.
 * EN native extent is 12 bytes, including its trailing alignment.
 * T0 standalone score 100 is only the absent final 2-byte alignment.
 * In the natural four-helper group, this complete 12-byte extent matches,
 * and its call relocation resolves to Animation_ApplyChildArgumentFar.
 * No production adoption or all-edition linking credit is claimed.
 */
#include "TYPES.H"
#include "EFFECT_SLOT.H"
#include "OBJECT_DISPATCH.H"

s32 Animation_ApplyChildArgumentFar(struct DispatchChild *child, s32 argument);

void EffectSlot_SetObjectMode(struct EffectSlot *effect, s32 mode)
{
    Animation_ApplyChildArgumentFar((struct DispatchChild *)effect->object, mode);
}

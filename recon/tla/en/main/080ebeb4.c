/* 2026-10-03 effect-slot suffix probe.
 * The existing draft could not compile: EffectCallback had no owner include.
 * Added the canonical EffectSlot owner without changing the original body.
 * EN native extent is 20 bytes, including its zero literal pool word.
 * T0: 20 bytes, score 60, one reordered zero-pool load.
 * T1: chained state/age/callback_delay reset: same 20 bytes and score 60.
 * Retained the original separate stores; neither ordinary form matches.
 * In the 236-byte group, this extent has four differing bytes.
 * D0: a genuinely used s8 zero fixed to r2 was folded into r3 and
 * removed the zero-pool load and pool: 16-byte extent versus native 20.
 * The pin changes instruction presence and is removed. Axis stopped.
 * STOP: move the zero-pool load before the zero immediate; no adoption.
 */
#include "TYPES.H"
#include "EFFECT_SLOT.H"

void EffectSlot_SetCallback(struct EffectSlot *effect, EffectCallback callback)
{
    effect->callback = callback;
    effect->callback_delay = 0;
    effect->age = 0;
    effect->state = 0;
}

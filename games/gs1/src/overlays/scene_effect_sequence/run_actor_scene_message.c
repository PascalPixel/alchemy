#include "types.h"

/*
 * One-shot message beat for overlay resource_3a0.  Each Func_ name spells
 * the overlay's own relocated call word, not a runtime address.
 */

#include "scene_effect_sequence.h"

void Func_020020b0();          /* scene open */
void Func_02002156();
s32 Func_02002176();
void Func_020020ca();          /* scene close */

/*
 * The 28-byte owner includes its one pool word: 0x17f7 is an identifier
 * passed as an argument, not an address.  The first and last calls are the
 * scene bracket and must stay in that order.
 */
void SceneEffect_RunActorSceneMessage(void)
{
    Func_020020b0();
    Func_02002156(0x17f7);
    Func_02002176(17, 0);
    Func_020020ca();
}

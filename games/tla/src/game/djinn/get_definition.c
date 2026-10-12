#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "btlsmn.h"
#include "types.h"


const struct SummonDefinition *SummonDefinition_GetNear(u32 summon_id)
{
    if (summon_id > 28)
        return NULL;
    return &Summon_DefinitionTable[summon_id];
}

u32 *Trade_GetOfferState();

void Func_080aff94(s32 bit)
{
    u32 *state = Trade_GetOfferState(0);
    /* FAKEMATCH: the native loads the flags word before it builds the bit; the plain form builds the bit first (one reordered pair, differ 2). The word is pinned to the r3 it already takes: in a plain local the two registers trade places (differ 10). */
    register u32 flags __asm__("r3") = *state;

    /* FAKEMATCH: the one-pass block's loop note holds the bit until the pinned word is loaded. */
    do { } while (0);
    flags |= 1 << bit;
    *state = flags;
}

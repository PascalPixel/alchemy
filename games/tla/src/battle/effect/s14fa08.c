#include "types.h"
#include "battle_effect_work.h"
#include "ram_buffer.h"

void Func_0814fab8(struct BattleEffectArgument *effect, u32 mode);

void Func_0814fa08(struct BattleEffectArgument *effect)
{
    Func_0814fab8(effect, 0);
}

void Func_0814fa14(struct BattleEffectArgument *effect)
{
    Func_0814fab8(effect, 1);
}

void Func_0814fa20(struct BattleEffectArgument *effect)
{
    Func_0814fab8(effect, 6);
}

void Func_0814fa2c(struct BattleEffectArgument *effect)
{
    Func_0814fab8(effect, 7);
}

void Func_0814fa38(struct BattleEffectArgument *effect)
{
    Func_0814fab8(effect, 2);
}

void Func_0814fa44(struct BattleEffectArgument *effect)
{
    Func_0814fab8(effect, 11);
}

void Func_0814fa50(struct BattleEffectArgument *effect)
{
    Func_0814fab8(effect, 3);
}

void Func_0814fa5c(struct BattleEffectArgument *effect)
{
    Func_0814fab8(effect, 5);
}

void Func_0814fa68(struct BattleEffectArgument *effect)
{
    Func_0814fab8(effect, 8);
}

void Func_0814fa74(struct BattleEffectArgument *effect)
{
    Func_0814fab8(effect, 12);
}

void Func_0814fa80(struct BattleEffectArgument *effect)
{
    Func_0814fab8(effect, 4);
}

void Func_0814fa8c(struct BattleEffectArgument *effect)
{
    Func_0814fab8(effect, 9);
}

void Func_0814fa98(struct BattleEffectArgument *effect)
{
    struct BattleEffectWork *work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];

    /* FAKEMATCH: ⚓️ loads the effect work before it builds the fade step's offset; the plain order starts the offset first (one pair of instructions swapped). */
    asm volatile("");
    work->fadeStep = 0;
    effect->prelude = 0;
    Func_0814fab8(effect, 10);
}

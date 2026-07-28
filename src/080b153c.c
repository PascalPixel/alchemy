#include "types.h"

struct Effect_080b153c {
    s16 value;
    u8 pad_02;
    u8 flags;
};

struct BattleState_080b153c {
    u8 pad_000[0x3a8];
    u8 action;
    u8 pad_3a9;
    s8 mode;
};

struct GlobalState_080b153c {
    u8 pad_00[16];
    s32 field_10;
};

struct ActorState_080b153c {
    u8 pad_000[0xd8];
    u16 slots[1];
};

extern struct GlobalState_080b153c Data_02000240;
extern u8 Value_00000ca0;
extern u8 *Func_08077008(s32);
extern struct Effect_080b153c *Func_08077018(s32);
extern void Func_080b04dc(s32);
extern s32 Func_08077038(s32, s32);
extern s32 Func_080022f4(s32, s32);
extern s32 Func_08077240(s32, s32);
extern void Func_080b0a6c(void *, s32, s32);
extern s32 Func_080b1614(s32, s32, s32);

s32 Func_080b153c(s32 actor, s32 item)
{
    struct BattleState_080b153c *battle;
    struct ActorState_080b153c *actor_state;
    struct Effect_080b153c *effect;
    s32 quantity;
    s32 chance;
    s32 slot;
    s32 maximum;
    s32 result;

    battle = *(struct BattleState_080b153c **)0x03001f2c;
    actor_state = Func_08077008(actor);
    effect = Func_08077018(item);
    result = 1;
    if (effect->flags & 0x10) {
        Func_080b04dc((s32)&Value_00000ca0);
        slot = Func_08077038(actor, item);
        if (slot != -1) {
            quantity = (actor_state->slots[slot] >> 11) + 1;
        } else {
            quantity = 0;
        }

        chance = 30;
        if (effect->value != 0)
            chance = Func_080022f4(Data_02000240.field_10, effect->value);

        if (battle->mode == 2) {
            maximum = Func_08077240(item, 0);
            if (chance > maximum)
                maximum = Func_08077240(item, 0);
            else
                maximum = chance;
            chance = maximum;
        }

        chance += quantity;
        if (chance > 30)
            chance = 30;

        battle->action = 12;
        Func_080b0a6c(0, 0x80, 0x30);
        result = Func_080b1614(quantity, chance, effect->value);
    }
    return result;
}

#include "types.h"

struct CombatantEffects_080b2328 {
    u8 pad000[0xD8];
    u16 effects[15];
};

struct EffectMetadata_080b2328 {
    u8 pad00[2];
    u8 target_kind;
    u8 restrictions;
    u8 pad04[8];
    u8 category;
};

struct EffectMenu_080b2328 {
    u8 pad000[0x20];
    void *content;
};

struct CombatantEffects_080b2328 *Func_08077008(s32 combatant);
struct EffectMetadata_080b2328 *Func_08077018(s32 effect);
s32 Func_08077228(s32 combatant, u8 target_kind);
s32 Func_080b20e8(s32 encoded_effect);
void Func_08015120(s32 value, s32 style);
void Func_080b0574(s32 message);
s32 Func_080b0634(s32 mode);
void Func_080b1dec(void *target, s32 combatant);
void Func_08015140(void);
void Func_080030f8(u32 frames);
void Func_080f9010(s32 sound);
void Func_08077068(s32 combatant, s32 index);
void Func_08077230(s32 amount);
void Func_080b10cc(void);
s32 Func_080b1868(s32 combatant, s32 index);
void Func_080b196c(s32 combatant, s32 target);

/*
 * Validate and purchase the selected combat effect. Validation failures are
 * reported immediately; a successful purchase temporarily removes the entry
 * while the effect animation runs, restores it, charges its cost, refreshes
 * the menu, and applies any follow-up target operation.
 */
void Func_080b2328(s32 combatant, s32 index)
{
    struct EffectMenu_080b2328 *menu =
        *(struct EffectMenu_080b2328 **)0x03001F2C;
    struct CombatantEffects_080b2328 *state =
        Func_08077008(combatant);
    u16 encoded = state->effects[index];
    s32 effect = encoded & 0x1FF;
    struct EffectMetadata_080b2328 *metadata =
        Func_08077018(effect);
    s32 target = Func_08077228(combatant, metadata->target_kind);
    s32 cost = Func_080b20e8(encoded);

    if (metadata->category != 2) {
        Func_08015120(effect, 2);
        Func_080b0574(0xCBA);
        return;
    }
    if ((encoded & 0x400) == 0) {
        Func_08015120(effect, 2);
        Func_080b0574(0xCBB);
        return;
    }
    if ((encoded & 0x200) != 0 &&
        (metadata->restrictions & metadata->category) != 0) {
        Func_08015120(effect, 2);
        Func_080b0574(0xCBC);
        return;
    }
    if ((u32)cost > *(u32 *)0x02000250) {
        Func_080b0574(0xCBD);
        return;
    }

    Func_08015120(effect, 2);
    Func_08015120(cost, 5);
    Func_080b0574(0xCBE);
    if (Func_080b0634(0) != 0) {
        Func_080b0574(0xCBF);
        return;
    }

    state->effects[index] = 0;
    Func_080b1dec(menu->content, combatant);
    Func_08015120(effect, 2);
    Func_080b0574(0xCC0);
    Func_08015140();
    Func_080030f8(10);
    Func_080f9010(0x64);
    Func_080030f8(110);
    Func_080f9010(0x64);
    Func_080030f8(110);
    Func_080f9010(0x64);
    Func_080030f8(110);
    Func_080f9010(0x70);
    Func_080030f8(20);

    state->effects[index] = encoded;
    Func_08077068(combatant, index);
    Func_08077230(-cost);
    Func_080b10cc();
    Func_080b1dec(menu->content, combatant);
    Func_08015120(effect, 2);
    Func_080b0574(0xCC1);

    if (Func_080b1868(combatant, index) != 0)
        Func_080b196c(combatant, target);
}

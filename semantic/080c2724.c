/*
 * Correctness fix, veneer audit (2026-08-01).
 *
 * `Func_080072f0` is not a function.  0x080072e4 begins the GCC
 * `__call_via_rN` veneer bank -- fifteen four-byte `bx rN; nop` entries,
 * r0..lr, ending at 0x08007320 -- so 0x080072f0 is `__call_via_r3` and
 * `bl 0x80072f0` calls whatever r3 holds.
 *
 * At every site in this file the ROM loads r3 from the literal pool with
 * the constant 0x03001388, so the callee is the relocated IWRAM word copy
 * at that address.  Its signature is not guessed: the EXACT source
 * src/080d40ec.c declares it as
 * `void *(*)(void *destination, const void *source, s32 size)` and
 * src/080e0524.c casts the same address to the same shape.
 *
 * Note what the previous draft had already half-seen: it passed
 * 0x03001388 as a fourth ARGUMENT.  That value was never an argument --
 * it is the callee, and the register load that produced it is the call
 * target, not a parameter.
 */
#include "layout_guard.h"
#include "types.h"

typedef void *(*WordCopy)(void *destination, const void *source, s32 size);

typedef struct RewardTotals {
    s32 currency;
    s32 experience;
    s32 encounters;
    u16 rare_items[4];
} RewardTotals;

typedef struct LevelUpResult {
    s32 unknown00;
    s16 gains[6];
} LevelUpResult;

typedef struct AbilityEntry_080c2724 {
    u16 ability;
    u16 padding02;
} AbilityEntry_080c2724;

typedef struct Character_080c2724 {
    u8 padding000[0x0f];
    u8 portrait_style;
    u8 padding010[0x48];
    AbilityEntry_080c2724 abilities[32];
    u8 padding0d8[0x4c];
    s32 experience;
    u8 padding128;
    u8 icon;
    u8 padding12a[0x22];
} Character_080c2724;

typedef struct WorldState_080c2724 {
    u8 padding000[0x530];
    RewardTotals rewards;
} WorldState_080c2724;

LAYOUT_OFFSET_GUARD(
    RewardTotals080c2724_RareItems,
    RewardTotals,
    rare_items,
    0x0c);
LAYOUT_SIZE_GUARD(
    AbilityEntry080c2724_Size,
    AbilityEntry_080c2724,
    4);
LAYOUT_OFFSET_GUARD(
    Character080c2724_PortraitStyle,
    Character_080c2724,
    portrait_style,
    0x0f);
LAYOUT_OFFSET_GUARD(
    Character080c2724_Abilities,
    Character_080c2724,
    abilities,
    0x58);
LAYOUT_OFFSET_GUARD(
    Character080c2724_Experience,
    Character_080c2724,
    experience,
    0x124);
LAYOUT_OFFSET_GUARD(
    Character080c2724_Icon,
    Character_080c2724,
    icon,
    0x129);
LAYOUT_SIZE_GUARD(
    Character080c2724_Size,
    Character_080c2724,
    0x14c);
LAYOUT_OFFSET_GUARD(
    WorldState080c2724_Rewards,
    WorldState_080c2724,
    rewards,
    0x530);

extern WorldState_080c2724 *Data_03001e74;
extern volatile u16 Data_0200047c;

void Func_08002df0(void *);
void *Func_08004970(s32);
void Func_08015118(void);
void Func_08015120(s32, s32);
void Func_080151c8(s32);
Character_080c2724 *Func_08077008(u16);
s32 Func_08077030(u16);
s32 Func_080770b8(s32, LevelUpResult *);
void Func_08077230(s32);
s32 Func_080b6b40(s32, u16 *);
void Func_080bb65c(void);
s32 Func_080c2470(u16);
void Func_080f9010(s32);

/*
 * Distribute the accumulated battle rewards, reporting every intermediate
 * level and learned ability before awarding currency and rare items.
 */
void Func_080c2724(void)
{
    RewardTotals *rewards =
        &Data_03001e74->rewards;
    u16 party_ids[8];
    LevelUpResult level_up;
    Character_080c2724 *snapshot;
    s32 party_count;
    u32 i;

    if (rewards->experience != 0) {
        Func_08015120(rewards->experience, 5);
        Func_080151c8(0x83a);
        Func_080bb65c();
    }

    party_count = Func_080b6b40(1, party_ids);
    snapshot = Func_08004970(0x14c);

    for (i = 0; i < party_count; i++) {
        u16 character_id = party_ids[i];
        Character_080c2724 *character = Func_08077008(character_id);

        character->experience += rewards->experience;
        for (;;) {
            u32 ability;

            ((WordCopy)0x03001388)(snapshot, character, 0x14c);
            if (Func_080770b8(character_id, &level_up) == 0)
                break;

            Func_080f9010(0x59);
            Func_08015118();
            Func_08015120(character->icon, 3);
            Func_08015120(character_id, 1);
            Func_08015120(character->portrait_style, 5);
            Func_080151c8(0x89a);
            Func_080bb65c();

            for (ability = 31; ability >= 0; ability--) {
                u16 learned = character->abilities[ability].ability;
                u32 old_index;

                if ((learned & 0x3fff) == 0 || (learned & 0x8000) == 0)
                    continue;

                for (old_index = 0; old_index < 32; old_index++) {
                    if (snapshot->abilities[old_index].ability == learned)
                        break;
                }
                if (old_index != 32)
                    continue;

                Func_08015118();
                Func_08015120(character->icon, 3);
                Func_08015120(character_id, 1);
                Func_08015120(learned & 0x3fff, 4);
                Func_080f9010(0x9a);
                Func_080151c8(0x89b);
                Func_080bb65c();
            }

            for (ability = 0; ability < 6; ability++) {
                if (level_up.gains[ability] != 0) {
                    Func_08015120(level_up.gains[ability], 5);
                    Func_080151c8(0x89c + ability);
                    Func_080bb65c();
                }
            }
        }
    }
    Func_08002df0(snapshot);

    if (rewards->currency != 0) {
        Func_08015120(rewards->currency, 5);
        Func_080151c8(0x83b);
        Func_08077230(rewards->currency);
        Func_080bb65c();
    }

    for (;;) {
        s32 selected = -1;
        s32 best_priority = -1;

        for (i = 0; i < 4; i++) {
            u16 item = rewards->rare_items[i];

            if (item != 0) {
                s32 priority = Func_080c2470(item);
                if (priority >= best_priority) {
                    best_priority = priority;
                    selected = i;
                }
            }
        }

        if (selected < 0)
            return;

        Func_08015120(rewards->rare_items[selected], 2);
        Func_080151c8(0x83c);
        Func_080bb65c();
        if (Func_08077030(rewards->rare_items[selected]) == -1) {
            Data_0200047c = rewards->rare_items[selected];
            return;
        }
        rewards->rare_items[selected] = 0;
    }
}

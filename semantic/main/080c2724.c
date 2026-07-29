#include "types.h"

#define FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

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

void Func_08002df0(void *);
void *Func_08004970(s32);
void Func_080072f0(void *, const void *, s32, u32);
void Func_08015118(void);
void Func_08015120(s32, s32);
void Func_080151c8(s32);
u8 *Func_08077008(u16);
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
        (RewardTotals *)(*(u8 **)0x03001e74 + 0x530);
    u16 party_ids[8];
    LevelUpResult level_up;
    u8 *snapshot;
    s32 party_count;
    s32 i;

    if (rewards->experience != 0) {
        Func_08015120(rewards->experience, 5);
        Func_080151c8(0x83a);
        Func_080bb65c();
    }

    party_count = Func_080b6b40(1, party_ids);
    snapshot = Func_08004970(0x14c);

    for (i = 0; i < party_count; i++) {
        u16 character_id = party_ids[i];
        u8 *character = Func_08077008(character_id);

        FIELD(character, s32, 0x124) += rewards->experience;
        for (;;) {
            s32 ability;

            Func_080072f0(snapshot, character, 0x14c, 0x03001388);
            if (Func_080770b8(character_id, &level_up) == 0)
                break;

            Func_080f9010(0x59);
            Func_08015118();
            Func_08015120(FIELD(character, u8, 0x129), 3);
            Func_08015120(character_id, 1);
            Func_08015120(FIELD(character, u8, 0x0f), 5);
            Func_080151c8(0x89a);
            Func_080bb65c();

            for (ability = 0; ability < 32; ability++) {
                u16 learned = FIELD(character, u16, 0x58 + ability * 4);
                s32 old_index;

                if ((learned & 0x3fff) == 0 || (learned & 0x8000) == 0)
                    continue;

                for (old_index = 0; old_index < 32; old_index++) {
                    if (FIELD(snapshot, u16, 0x58 + old_index * 4) == learned)
                        break;
                }
                if (old_index != 32)
                    continue;

                Func_08015118();
                Func_08015120(FIELD(character, u8, 0x129), 3);
                Func_08015120(character_id, 1);
                Func_08015120(learned & 0x3fff, 4);
                Func_080f9010(0x9a);
                Func_080151c8(0x89b);
                Func_080bb65c();
            }

            if (level_up.gains[0] != 0) {
                Func_08015120(level_up.gains[0], 5);
                Func_080151c8(0x89c);
                Func_080bb65c();
            }
            if (level_up.gains[1] != 0) {
                Func_08015120(level_up.gains[1], 5);
                Func_080151c8(0x89d);
                Func_080bb65c();
            }
            if (level_up.gains[2] != 0) {
                Func_08015120(level_up.gains[2], 5);
                Func_080151c8(0x89e);
                Func_080bb65c();
            }
            if (level_up.gains[3] != 0) {
                Func_08015120(level_up.gains[3], 5);
                Func_080151c8(0x89f);
                Func_080bb65c();
            }
            if (level_up.gains[4] != 0) {
                Func_08015120(level_up.gains[4], 5);
                Func_080151c8(0x8a0);
                Func_080bb65c();
            }
            if (level_up.gains[5] != 0) {
                Func_08015120(level_up.gains[5], 5);
                Func_080151c8(0x8a1);
                Func_080bb65c();
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
            *(volatile u16 *)0x0200047c = rewards->rare_items[selected];
            return;
        }
        rewards->rare_items[selected] = 0;
    }
}

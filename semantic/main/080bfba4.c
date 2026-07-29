#include "types.h"

#define U8_AT(p, o)  (*(u8 *)((u8 *)(p) + (o)))
#define S8_AT(p, o)  (*(s8 *)((u8 *)(p) + (o)))
#define S16_AT(p, o) (*(s16 *)((u8 *)(p) + (o)))
#define S32_AT(p, o) (*(s32 *)((u8 *)(p) + (o)))
#define U32_AT(p, o) (*(u32 *)((u8 *)(p) + (o)))

void *Func_08077008(s32);
void *Func_08077000(s32);
void Func_080771b8(s32, s32, s32);
s32 Func_080b6b40(s32, void *);
void *Func_08004970(s32);
void Func_080072f0();
void Func_08077010(s32);
void Func_080bdfec(void);
void Func_080bd808(s32);
void Func_080bbabc(u32, u32);
void Func_080f9010(s32);
void **Func_080b7dd0(s32);
void Func_08009080(void *, s32);
void Func_08009088(void *, s32);
void Func_080c1798(s32, s32, s32, s32);
void Func_080be02c(void);
void Func_08002df0(void *);
void Func_080bb938(void);
s32 Func_08077118(s32, s32);
s32 Func_080022ec(s32, s32);

void Func_080bfba4(u8 *result)
{
    s32 character = result[0];
    u8 *profile = Func_08077008(character);
    u8 *party = Func_08077000(character > 7);
    u8 *entries = party + 8;
    s32 counts[4] = {0, 0, 0, 0};
    s32 linked;
    s32 entry_count = S32_AT(party, 0x108);
    s32 i;

    for (i = 0; i < entry_count; i++) {
        u8 *entry = entries + i * 4;
        if (entry[2] == character && (s8)entry[3] == -1)
            Func_080771b8(character, entry[0], entry[1]);
    }

    linked =
        Func_080b6b40(1, 0) != 0 && Func_080b6b40(2, 0) != 0;
    party = Func_08077000(character > 7);
    entries = party + 8;
    entry_count = S32_AT(entries, 0x100);

    for (;;) {
        s32 group = -1;
        s32 next_index;

        for (i = 0; i < entry_count; i++) {
            u8 *entry = entries + i * 4;
            if ((s8)entry[3] == -2) {
                group = entry[2];
                break;
            }
        }
        if (group < 0)
            break;

        next_index = -1;
        for (i = 0; i < entry_count; i++) {
            u8 *entry = entries + i * 4;
            if (entry[2] == group && (s8)entry[3] > next_index)
                next_index = (s8)entry[3];
        }
        next_index++;
        if (next_index <= 1)
            next_index = 2;

        for (i = 0; i < entry_count; i++) {
            u8 *entry = entries + i * 4;
            if (entry[2] == group && (s8)entry[3] == -2) {
                entry[3] = next_index++;
                counts[entry[0]]++;
            }
        }
    }

    if (linked) {
        u8 *snapshot = Func_08004970(0x14c);
        s32 largest_count = 0;
        s32 largest_category = -1;

        Func_080072f0(snapshot, profile, 0x14c, (void *)0x03001388);
        for (i = 0; i < 4; i++) {
            if (counts[i] > largest_count) {
                largest_count = counts[i];
                largest_category = i;
            }
        }
        if (largest_category >= 0 &&
            S8_AT(profile, largest_category + 0x12c) < largest_count)
            S8_AT(profile, largest_category + 0x12c) = largest_count;

        Func_08077010(character);
        for (i = 0; i < 4; i++) {
            s32 increase =
                S32_AT(profile, 0x48 + i * 4) -
                S32_AT(snapshot, 0x48 + i * 4);
            if (increase > 0) {
                Func_080bdfec();
                Func_080bd808(0x19);
                Func_080bbabc(0, character);
                Func_080bbabc(1, increase);
                Func_080bbabc(0x0e, 0xaf);
                Func_080bbabc(4, i + 0x879);
                Func_080bbabc(0x0b, character);
                Func_080f9010(0xd4);
                Func_08009080(*Func_080b7dd0(character), 3);
                Func_08009088(*Func_080b7dd0(character), 0x20);
                Func_080c1798(character, i, 2, largest_count - 1);
                Func_080be02c();
            }
        }
        Func_08002df0(snapshot);

        Func_080bdfec();
        if (U32_AT(result, 0x60) != 0) {
            u32 amount = U32_AT(result, 0x60);
            Func_080bbabc(8, character);
            Func_080bbabc(0, character);
            Func_080bbabc(1, amount);
            Func_080bbabc(4, 0x84b);
            if (Func_08077118(character, -amount) == 0) {
                Func_080bbabc(9, character);
                Func_080bbabc(0, character);
                if (character <= 7)
                    Func_080bbabc(4, 0x825);
                else
                    Func_080bbabc(4, 0x82b);
            } else {
                Func_080bbabc(0x0b, character);
            }
        }
        Func_080bb938();

        Func_080bdfec();
        if (S8_AT(profile, 0x131) != 0) {
            s32 amount =
                Func_080022ec(S8_AT(profile, 0x131) *
                                  S16_AT(profile, 0x34),
                              10);
            u8 *runtime = *(u8 **)0x03001e74;

            Func_080bbabc(8, character);
            Func_080bbabc(0, character);
            Func_080bbabc(1, amount);
            Func_080bbabc(4, 0x851);
            runtime[0x820] = S8_AT(profile, 0x131) != 0 ? 0x86 : 0x85;
            if (Func_08077118(character, -amount) == 0) {
                Func_080bbabc(9, character);
                Func_080bbabc(0, character);
                if (character <= 7)
                    Func_080bbabc(4, 0x825);
                else
                    Func_080bbabc(4, 0x82b);
            } else {
                Func_080bbabc(0x0b, character);
            }
        }
        Func_080bb938();

        Func_080bdfec();
        if (U8_AT(profile, 0x141) != 0) {
            U8_AT(profile, 0x141)--;
            if (U8_AT(profile, 0x141) == 0 &&
                Func_08077118(character, (s32)0xc0000000) == 0) {
                Func_080bbabc(0, character);
                Func_080bbabc(4, 0x828);
                Func_080bbabc(8, character);
                Func_080bbabc(9, character);
                Func_080bbabc(0, character);
                if (character <= 7)
                    Func_080bbabc(4, 0x825);
                else
                    Func_080bbabc(4, 0x82b);
            }
        }
        Func_080bb938();
    }

    Func_08077010(character);
}

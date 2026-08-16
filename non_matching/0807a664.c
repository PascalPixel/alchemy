#include "types.h"

struct OwnerState { u8 pad[0xd8]; u16 entries[15]; };
struct ItemDefinition { u8 pad[2]; u8 category; };

struct OwnerState *Func_08077394(s32 owner);
struct ItemDefinition *Func_08078414(s32 item);
void Func_08079ae8(s32 owner);
s32 Func_08077428(s32 owner);
void Func_0807a628(s32 owner, s32 target);
s32 Func_08079358(s32 flag);
void Func_0807808c(s32 mode);

void Func_0807a664(void)
{
    u16 *snapshot = (u16 *)0x02001078;
    u16 *cursor = snapshot;
    s16 value0;
    s16 value1;
    s16 *savedBase;
    u16 *pair;
    s32 owner;

    if (*cursor != 0x6774) {
        *cursor = 0x6774;
        cursor++;
        savedBase = (s16 *)0x02000240;
        value0 = savedBase[0x110];
        value1 = *(s16 *)((u8 *)savedBase + 0x222);

        owner = 0;
        do {
            struct OwnerState *state = Func_08077394(owner);
            s32 left;
            u16 *read;
            u16 *write;
            s32 offset;
            s32 count;

            {
                u16 *source = (u16 *)((u8 *)state + 0xd8);
                u32 value;
                left = 14;
                do {
                    value = *source;
                    left--;
                    source++;
                    *cursor = value;
                    cursor++;
                } while (left >= 0);
            }

            offset = 0xd8;
            left = 14;
            do {
                u16 entry = *(u16 *)(offset + (u32)state);
                if (Func_08078414(entry)->category != 6)
                    *(u16 *)(offset + (u32)state) = 0;
                left--;
                offset += 2;
            } while (left >= 0);

            {
                s32 compact_left;
                s32 count;
                u16 *read;
                u16 *write;
                count = 0;
                compact_left = 14;
                read = state->entries;
                write = state->entries;
                do {
                    u16 entry = *(volatile u16 *)read++;
                    u32 test = entry;
                    test <<= 16;
                    if (test != 0) {
                        *write++ = entry;
                        count++;
                    }
                    compact_left--;
                } while (compact_left >= 0);
                if (count <= 14) {
                    compact_left = 15 - count;
                    do {
                        *write++ = 0;
                        compact_left--;
                    } while (compact_left != 0);
                }
            }

            Func_08079ae8(owner);
            Func_08077428(owner);
            owner++;
        } while (owner <= 3);

        *cursor++ = value0;
        *cursor++ = value1;
        pair = (u16 *)0x02000438;
        *cursor++ = *pair;
        *cursor = pair[1];
        Func_0807a628(0, 16);
        Func_08079358(0x952);
    }
    Func_0807808c(1);
}

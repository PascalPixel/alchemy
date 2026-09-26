/* Not-yet-C, complete 344-byte pair-joined message and pool.
 * Separate scalar display fields were removed by GCC: only the first scalar
 * escaped through each box pointer. Three-word records recover both records
 * and the reference's 48-byte frame. Separate records in reference stack
 * order give 340 bytes and 62 aligned halfword edits; reverse order gave 45
 * edits but put the records in the wrong slots.
 * A branch-local zero and first-record pointer still have different lifetimes;
 * fields use the saved pointer instead of direct stack stores. A shared pair
 * array reduces the frame to 44 and adds a long branch. Value_0000001d forces
 * the observed pool load but changes zero rematerialization and grows code.
 * Three structural hypotheses stopped. No adoption or byte credit. */
#include "TYPES.H"

struct JoinedMessageBox {
    u32 flags;
    u32 position;
    u32 tiles;
};
struct JoinedMessageWork {
    u8 unknown_0000[0xea3];
    u8 message_mode;
    u8 unknown_0ea4[0x450];
    u16 first_count;
    u16 second_count;
};

extern struct JoinedMessageWork *Data_03001e8c;

s32 Func_080162d4(s32, s32, s32, s32, s32);
void Func_0801e41c(s32, s32, s32, s32, s32);
s32 Func_08021360(u32);
s32 Func_08019d2c(void);
void Func_0801a4fc(s32, s32, s32 *, s32 *, s32, s32);
void Func_08019908(u32, u32);
s32 Func_08019ba0(s32);
void Func_080165d8(s32, s32, s32, s32, s32);
void Func_080f9010(s32);
void Func_08003dec(void *, s32);
void Func_080030f8(s32);
s32 Func_080f9048(void);
void Func_08016418(s32, s32);
s32 Func_08003f3c(s32);

void Func_08021488(s32 msg0, s32 msg1) {
    s32 spC;
    s32 sp10;
    s32 sp14;
    struct JoinedMessageBox second;
    struct JoinedMessageBox first;
    s32 window;
    struct JoinedMessageWork *base;
    struct JoinedMessageBox *box1;
    struct JoinedMessageBox *box2;
    u32 zero;

    base = Data_03001e8c;
    box1 = &first;
    window = 0;
    window = Func_080162d4(1, 1, 0x1C, 5, window);
    if (window != 0) {
        zero = 0;
        Func_0801e41c(window, 8, 0, 4, 4);
        base->message_mode = 1;
        Func_08021360(msg0);
        Func_0801a4fc(Func_08019d2c(), 0, &sp14, &sp10, 0xE, zero);
        first.flags = zero;
        first.position = 0x800C000C;
        first.tiles = sp10 | 0xE000;
        box2 = &second;
        Func_08021360(msg1);
        Func_0801a4fc(Func_08019d2c(), 0, &spC, &sp10, 0xF, zero);
        second.flags = zero;
        second.position = 0x802C000C;
        second.tiles = sp10 | 0xF000;
        base->first_count = zero;
        base->second_count = zero;
        Func_08019908(msg0, 1);
        Func_08019908(msg1, 1);
        Func_080165d8(window, Func_08019ba0(0x1D), 0x44, 2, zero);
        Func_080f9010(0x51);
    loop_2:
        Func_08003dec(box1, 0xFA);
        Func_08003dec(box2, 0xFA);
        Func_080030f8(1);
        if (Func_080f9048() != 0) {
            if (!(*(s32 *)0x03001C94 & 0x303)) {
                goto loop_2;
            }
        }
        Func_08016418(window, 2);
        Func_080030f8(1);
        Func_08003f3c(sp14);
        Func_08003f3c(spC);
    }
}

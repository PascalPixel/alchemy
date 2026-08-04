#include "types.h"

/*
 * resource_3bc scene owner at 0x02001c20, 472 bytes including its five-word
 * literal pool.  This is state-machine slot 1: the zero state plays the
 * complete two-actor exchange, state 1 plays the closing message, and every
 * non-guard path is handed to the shared state-machine tail.
 */

extern s16 Data_02000240[];

extern void Func_0200288c(void);
extern s32 Func_02002a94(s32 actor, s32 slot);
extern void Func_02002b50(s32 actor, s32 slot);
extern void Func_02002bac(s32 state, s32 actor, s32 slot);
extern void Func_02003a58(s32 x, s32 z, s32 mode);
extern void Func_02003abc(s32 x, s32 z, s32 mode);
extern void Func_02003b18(void);
extern void Func_080000c0(s32 frames);
extern void Func_08009150(void *record, s32 x, s32 z, s32 y);
extern void Func_08009158(void *record);
extern void Func_0808a010(s32 frames);
extern void Func_0808a018(void);
extern void Func_0808a020(void);
extern u8 *Func_0808a080(s32 actor);
extern void Func_0808a170(s32 message);
extern void Func_0808a180(s32 actor, s32 mode);
extern void Func_0808a200(s32 actor, s32 mode);
extern void Func_0808a208(s32 x, s32 z);
extern void Func_0808a210(s32 x, s32 y, s32 z, s32 mode);
extern void Func_0808a218(void);

void Func_02001c20(s32 actor)
{
    s32 state;
    u8 *record;

    if (Data_02000240[225] == 2) {
        Func_0200288c();
        return;
    }

    Func_0808a018();
    state = Func_02002a94(actor, 1);

    if (state == 0) {
        Func_0808a170(0x20b2);
        Func_0808a208(0x30000, 0x6000);
        Func_0808a210(0x980000, -1, 0xc80000, 1);
        Func_0808a218();
        Func_0808a010(30);
        Func_0808a180(actor, 0);
        Func_02003a58(0x68, 0x44, 0);
        Func_0808a010(60);
        Func_02003abc(0xa8, 0x60, 10);
        Func_0808a010(70);
        Func_0808a180(actor, 0);
        Func_02003b18();
        Func_080000c0(2);

        record = Func_0808a080(10);
        record[0x55] = (u8)state;
        *(s32 *)(record + 0x30) = 0xcccc;
        *(s32 *)(record + 0x34) = 0x6666;
        Func_08009150(record, *(s32 *)(record + 8), 0x40000,
                      *(s32 *)(record + 0x10));
        record = Func_0808a080(11);
        record[0x55] = (u8)state;
        *(s32 *)(record + 0x30) = 0xcccc;
        *(s32 *)(record + 0x34) = 0x6666;
        Func_08009150(record, *(s32 *)(record + 8), 0x200000,
                      *(s32 *)(record + 0x10));
        Func_08009158(record);

        Func_0808a010(45);
        record = Func_0808a080(10);
        record[0x55] = (u8)state;
        *(s32 *)(record + 0x30) = 0xcccc;
        *(s32 *)(record + 0x34) = 0x6666;
        Func_08009150(record, *(s32 *)(record + 8), 0x200000,
                      *(s32 *)(record + 0x10));
        record = Func_0808a080(11);
        record[0x55] = (u8)state;
        *(s32 *)(record + 0x30) = 0xcccc;
        *(s32 *)(record + 0x34) = 0x6666;
        Func_08009150(record, *(s32 *)(record + 8), 0x40000,
                      *(s32 *)(record + 0x10));
        Func_08009158(record);

        Func_0808a010(15);
        Func_0808a180(actor, 0);
        Func_02003a58(0x68, 0x44, 0);
        Func_0808a010(30);
        Func_02003abc(0xa8, 0x60, 10);
        Func_0808a010(40);
        Func_02003abc(0x68, 0x44, 10);
        Func_0808a010(70);
        Func_0808a180(actor, 0);
        Func_02003b18();
        Func_080000c0(2);
        Func_0808a200(0, 0);
        Func_02002b50(actor, 1);
    } else if (state == 1) {
        Func_0808a170(0x20b1);
        Func_0808a180(actor, 0);
    }

    Func_02002bac(state, actor, 1);
    Func_0808a020();
}

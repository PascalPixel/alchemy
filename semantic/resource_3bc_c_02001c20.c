#include "types.h"

/*
 * resource_3bc scene owner at 0x02001c20, 472 bytes including its five-word
 * literal pool.  This is state-machine slot 1: the zero state plays the
 * complete two-actor exchange, state 1 plays the closing message, and every
 * non-guard path is handed to the shared state-machine tail.
 */

extern s16 Data_02000240[];






















extern void Func_020044c8(void);
extern void Func_02006642(void);
extern s32 Func_020046de(s32 actor, s32 slot);
extern void Func_02006708(s32 message);
extern void Func_0200675c(s32 x, s32 z);
extern void Func_02006776(s32 x, s32 y, s32 z, s32 mode);
extern void Func_02006782(void);
extern void Func_02006678(s32 frames);
extern void Func_02006748(s32 actor, s32 mode);
extern void Func_020056ea(s32 x, s32 z, s32 mode);
extern void Func_02006690(s32 frames);
extern void Func_0200575e(s32 x, s32 z, s32 mode);
extern void Func_020066a0(s32 frames);
extern void Func_02006770(s32 actor, s32 mode);
extern void Func_020057cc(void);
extern void Func_020064fa(s32 frames);
extern u8 * Func_020066d8(s32 actor);
extern void Func_020065ee(void *record, s32 x, s32 z, s32 y);
extern u8 * Func_020066fc(s32 actor);
extern void Func_02006610(void *record, s32 x, s32 z, s32 y);
extern void Func_0200661e(void *record);
extern void Func_02006704(s32 frames);
extern u8 * Func_0200672a(s32 actor);
extern void Func_0200663c(void *record, s32 x, s32 z, s32 y);
extern u8 * Func_0200674a(s32 actor);
extern void Func_0200665e(void *record, s32 x, s32 z, s32 y);
extern void Func_0200666c(void *record);
extern void Func_02006752(s32 frames);
extern void Func_02006822(s32 actor, s32 mode);
extern void Func_020057c4(s32 x, s32 z, s32 mode);
extern void Func_0200676a(s32 frames);
extern void Func_02005838(s32 x, s32 z, s32 mode);
extern void Func_0200677a(s32 frames);
extern void Func_02005848(s32 x, s32 z, s32 mode);
extern void Func_0200678a(s32 frames);
extern void Func_0200685a(s32 actor, s32 mode);
extern void Func_020058b6(void);
extern void Func_020065e4(s32 frames);
extern void Func_0200689c(s32 actor, s32 mode);
extern void Func_02004904(s32 actor, s32 slot);
extern void Func_02006870(s32 message);
extern void Func_02006888(s32 actor, s32 mode);
extern void Func_0200497e(s32 state, s32 actor, s32 slot);
extern void Func_020067de(void);
void Func_02001c20(s32 actor)
{
    s32 state;
    u8 *record;

    if (Data_02000240[225] == 2) {
        return;
        Func_020044c8();
    }

    Func_02006642();
    state = Func_020046de(actor, 1);

    if (state == 0) {
        Func_02006708(0x20b2);
        Func_0200675c(0x30000, 0x6000);
        Func_02006776(0x980000, -1, 0xc80000, 1);
        Func_02006782();
        Func_02006678(30);
        Func_02006748(actor, 0);
        Func_020056ea(0x68, 0x44, 0);
        Func_02006690(60);
        Func_0200575e(0xa8, 0x60, 10);
        Func_020066a0(70);
        Func_02006770(actor, 0);
        Func_020057cc();
        Func_020064fa(2);

        *(s32 *)(record + 0x30) = 0xcccc;
        *(s32 *)(record + 0x34) = 0x6666;
        record[0x55] = (u8)state;
        record = Func_020066d8(10);
        Func_020065ee(record, *(s32 *)(record + 8), 0x40000,
                      *(s32 *)(record + 0x10));
        record = Func_020066fc(11);
        *(s32 *)(record + 0x34) = 0x6666;
        record[0x55] = (u8)state;
        *(s32 *)(record + 0x30) = 0xcccc;
        Func_02006610(record, *(s32 *)(record + 8), 0x200000,
                      *(s32 *)(record + 0x10));
        Func_0200661e(record);

        Func_02006704(45);
        record = Func_0200672a(10);
        record[0x55] = (u8)state;
        *(s32 *)(record + 0x34) = 0x6666;
        *(s32 *)(record + 0x30) = 0xcccc;
        Func_0200663c(record, *(s32 *)(record + 8), 0x200000,
                      *(s32 *)(record + 0x10));
        record = Func_0200674a(11);
        record[0x55] = (u8)state;
        *(s32 *)(record + 0x34) = 0x6666;
        *(s32 *)(record + 0x30) = 0xcccc;
        Func_0200665e(record, *(s32 *)(record + 8), 0x40000,
                      *(s32 *)(record + 0x10));
        Func_0200666c(record);

        Func_02006752(15);
        Func_02006822(actor, 0);
        Func_020057c4(0x68, 0x44, 0);
        Func_0200676a(30);
        Func_02005838(0xa8, 0x60, 10);
        Func_0200677a(40);
        Func_02005848(0x68, 0x44, 10);
        Func_0200678a(70);
        Func_0200685a(actor, 0);
        Func_020058b6();
        Func_020065e4(2);
        Func_0200689c(0, 0);
        Func_02004904(actor, 1);
    } else if (state == 1) {
        Func_02006870(0x20b1);
        Func_02006888(actor, 0);
    }

    Func_0200497e(state, actor, 1);
    Func_020067de();
}

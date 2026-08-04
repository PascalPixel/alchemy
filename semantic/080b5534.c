#include "layout_guard.h"
#include "types.h"

typedef struct ActorName_080b5534 {
    u8 text[14];
    u8 terminator;
} ActorName_080b5534;

LAYOUT_OFFSET_GUARD(
    ActorName080b5534_Terminator,
    ActorName_080b5534,
    terminator,
    14);

void Func_080030f8(s32);
void Func_08015000(void);
void Func_08015018(s32, s32);
void Func_08015020(s32, u16 *);
s32 Func_08015038(s32, s32, s32, s32);
s32 Func_08015048(void);
void Func_08015118(void);
void Func_08015120(s32, s32);
void Func_08015148(s32);
ActorName_080b5534 *Func_08077008(s32);
void Func_080f9010(s32);

/*
 * Controller-driven debug display.  This mode intentionally never returns:
 * every accepted input closes the current panel and redraws another one.
 */
void Func_080b5534(void)
{
    ActorName_080b5534 *actor = Func_08077008(0);
    u16 decoded[64];
    s32 value = 0;
    s32 alternate = 0;
    s32 i;

    Func_08015020(0x0903, decoded);
    actor->text[0] = decoded[0];
    if (decoded[0] != 0) {
        for (i = 1; i <= 13; i++) {
            actor->text[i] = decoded[i];
            if (decoded[i] == 0)
                break;
        }
    }
    actor->terminator = 0;

    Func_08015000();
    Func_080f9010(0x47);
    *(volatile u16 *)0x04000000 = 0x1341;

    for (;;) {
        s32 panel;
        s32 resource;
        s32 width;

        *(volatile u8 *)0x0200044c = 2;
        Func_08015118();
        Func_08015120(0x03e7, 5);
        Func_08015120(0, 3);
        Func_08015120(1, 1);
        Func_08015120(1, 2);
        Func_08015120(2, 4);

        if (alternate == 0) {
            resource = value + 0x26fa;
            width = 10;
        } else {
            resource = value + 0x0ad0;
            width = 2;
        }
        panel = Func_08015038(resource, 2, width, 4);

        Func_080030f8(10);
        for (;;) {
            if ((*(volatile u32 *)0x03001b04 & 2) != 0) {
                if (alternate != 0) {
                    alternate = 0;
                } else {
                    value++;
                    alternate = 1;
                }
            }
            if ((*(volatile u32 *)0x03001b04 & 0x10) != 0)
                value++;
            if ((*(volatile u32 *)0x03001b04 & 0x20) != 0)
                value -= 2;
            if ((*(volatile u32 *)0x03001b04 & 0x40) != 0)
                alternate = 1;
            if ((*(volatile u32 *)0x03001b04 & 0x80) != 0)
                alternate = 0;
            if ((*(volatile u32 *)0x03001b04 & 0x100) != 0)
                value += 10;
            if ((*(volatile u32 *)0x03001b04 & 0x200) != 0)
                value -= 10;

            if (value < 0)
                value = 0;
            if ((u32)value >= 0x15b)
                value = 0x15b;

            if ((*(volatile u32 *)0x03001b04 & 0x3f2) != 0)
                break;
            if (Func_08015048() != 0 &&
                (*(volatile u32 *)0x03001b04 & 1) != 0)
                break;
            Func_080030f8(1);
        }

        Func_08015148(1);
        Func_08015018(panel, 1);
        (*(volatile u8 **)0x03001e8c)[0x12f8] = 0;
    }
}

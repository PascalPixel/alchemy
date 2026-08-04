#include "types.h"

void Func_080984c0(void);
void Func_08097540(s16, s16);
void Func_08097c3c(s16);
void Func_080994c0(s16);
void Func_08098ccc(s16);
void Func_080999e4(s16);
void Func_08099128(s16);
void Func_08098848(s16);
void Func_080985a8(s16);
void Func_0809ade8(s16);
void Func_0809ad90(s16);
void Func_0809ab98(s16);
void Func_08099838(void);
void Func_08099da4(s16);
void Func_0809ae58(s16);
void Func_0809a8b8(s16);
void Func_080983a0(void);
void Func_0809b208(void);
void Func_0809b698(void);

void Func_08096960(void)
{
    u8 *scene = *(u8 **)0x03001f30;
    s16 argument = *(s16 *)(scene + 0x1a);
    s16 stage = *(s16 *)(scene + 0x1e);

    scene[0x20] = 0;
    switch (stage - 1) {
    case 0:
        Func_08097c3c(argument);
        break;
    case 1:
        if (*(s16 *)(*(u8 **)0x03001ebc + 0xcb8) != 0)
            Func_080984c0();
        if (*(s16 *)0x0200048a != argument)
            (*(u8 **)(scene + 0x14))[0x5b] = 1;
        Func_08097540(*(s16 *)(scene + 0x18), argument);
        break;
    case 2:
        Func_080994c0(argument);
        break;
    case 3:
        Func_08098ccc(argument);
        break;
    case 4:
        Func_080999e4(argument);
        break;
    case 5:
        Func_08099128(argument);
        break;
    case 6:
        Func_08098848(argument);
        break;
    case 7:
        Func_080983a0();
        break;
    case 8:
        if (*(s16 *)0x0200048a != -1) {
            Func_0809ade8(*(s16 *)0x0200048a);
            *(s16 *)0x0200048a = -1;
        }
        Func_0809ad90(argument);
        *(s16 *)0x0200048a = argument;
        Func_0809ab98(argument);
        break;
    case 9:
        Func_08099838();
        break;
    case 10:
        Func_08099da4(argument);
        break;
    case 11:
        Func_080985a8(argument);
        break;
    case 12:
        Func_0809ae58(argument);
        break;
    case 13:
        Func_0809a8b8(argument);
        break;
    case 14:
        Func_0809b208();
        break;
    case 15:
        Func_0809b698();
        break;
    }
}

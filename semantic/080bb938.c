#include "types.h"

void Func_080bb928(void *);
void Func_080bb8e8(s32);
void Func_08015120(s32, s32);
void Func_080151c8(s32);
void Func_080bb65c(void);
void Func_08015118(void);
void Func_080f9010(s32);
void Func_080babdc(s32, s32, s32);
void Func_080c24f0(s32, s32);
void Func_080bb588(s32);
void Func_080bace8(s32);
void Func_08015130(u8);
s32 Func_080b7dd0(s32);
void Func_080b78e4(s32, s32);
void Func_080b7aac(s32);
void Func_080bdfec(void);

void Func_080bb938(void)
{
    u8 *battle = *(u8 **)0x03001e74;
    u8 *commands = battle + 0x6b8;
    s32 count = *(s32 *)(commands + 0x144);
    s32 index;

    for (index = 0; index < count; index++) {
        s32 argument = *(s32 *)(commands + 0x40 + index * 4);

        switch (commands[index]) {
        case 0:
            Func_08015120(argument, 1);
            break;
        case 1:
            Func_08015120(argument, 5);
            break;
        case 2:
            Func_08015120(argument & 0x1ff, 2);
            break;
        case 3:
            Func_08015120(argument & 0x3fff, 4);
            break;
        case 4:
            if (argument >= 0)
                Func_080151c8(argument);
            Func_080bb65c();
            Func_08015118();
            break;
        case 5:
            if (argument >= 0)
                Func_080151c8(argument);
            Func_08015118();
            break;
        case 6:
            (*(s32 **)0x03001ee4)[2] = 1;
            break;
        case 7:
            Func_08015118();
            break;
        case 8:
            if (*(s32 *)(commands + 0x168) > 0)
                Func_080f9010(*(s32 *)(commands + 0x168));
            Func_080babdc(argument, 0, 0);
            break;
        case 9:
            Func_080c24f0(argument, *(s32 *)(commands + 0x16c));
            Func_080bb588(argument);
            Func_080bace8(argument);
            break;
        case 10:
            Func_08015130((*(u8 **)0x03001e74)[0x41]);
            break;
        case 11:
            Func_080b78e4(argument, Func_080b7dd0(argument));
            Func_080b7aac(argument);
            break;
        case 12:
            Func_080bb8e8(argument);
            break;
        case 13:
            Func_080bb928(commands);
            break;
        }
    }

    Func_080bdfec();
}

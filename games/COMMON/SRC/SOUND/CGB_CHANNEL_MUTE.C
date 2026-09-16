#include "TYPES.H"

void CgbChannel_Mute(u8 channel)
{
    switch (channel) {
    case 1:
        *(u8 *)0x04000063 = 8;
        *(u8 *)0x04000065 = 0x80;
        break;
    case 2:
        *(u8 *)0x04000069 = 8;
        *(u8 *)0x0400006d = 0x80;
        break;
    case 3:
        *(u8 *)0x04000070 = 0;
        break;
    default:
        *(u8 *)0x04000079 = 8;
        *(u8 *)0x0400007d = 0x80;
        break;
    }
}

typedef unsigned char u8;
typedef signed int s32;

/* Resource 378 position copy/phase adjustment at 0x0200267c (116 bytes). */

extern s32 Data_03001e40;
extern u8 *Func_0808a080();

void Func_0200267c(u8 *object)
{
    u8 *source = Func_0808a080(8);
    s32 x = *(s32 *)(source + 8);
    s32 y = *(s32 *)(source + 12);
    s32 z = *(s32 *)(source + 16) - 0x20000;

    *(s32 *)(object + 8) = x;
    *(s32 *)(object + 56) = x;
    *(s32 *)(object + 12) = y;
    *(s32 *)(object + 60) = y;
    *(s32 *)(object + 16) = z;
    *(s32 *)(object + 64) = z;

    switch (Data_03001e40 & 3) {
    case 0:
        x = *(s32 *)(source + 8) - 0x38000;
        *(s32 *)(object + 8) = x;
        *(s32 *)(object + 56) = x;
        break;
    case 1:
        x = *(s32 *)(source + 8) + 0x30000;
        *(s32 *)(object + 8) = x;
        *(s32 *)(object + 56) = x;
        break;
    case 2:
        y = *(s32 *)(source + 12) + 0x20000;
        *(s32 *)(object + 12) = y;
        *(s32 *)(object + 60) = y;
        break;
    case 3:
        *(s32 *)(object + 16) = *(s32 *)(source + 16);
        *(s32 *)(object + 64) = *(s32 *)(source + 16);
        break;
    default:
        break;
    }
}

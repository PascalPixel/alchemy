typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

void Func_080048b0(s32, s32);
void Func_08002dd8(s32);
void Func_080e823c(s32 *);
void Func_080d2d98(s32 *);
void Func_080eb754(s32 *);
void Func_080dc968(s32 *);
void Func_080d6970(s32 *);
void Func_080ec100(s32 *);
void Func_080d2458(s32 *);
void Func_080d1714(s32 *);
void Func_080ea0d8(s32 *);
void Func_080d765c(s32 *);
void Func_080e7320(s32 *);
void Func_080e15e8(s32 *);

void Func_080d6578(s32 *arg0) {
    Func_080048b0(41, 0x302);
    Func_080048b0(39, 0x782c);
    Func_080048b0(40, 0x4000);

    if (*arg0 == 0) {
        Func_080e7320(arg0);
    } else {
        switch (*arg0) {
        case 1:
            Func_080e823c(arg0);
            break;
        case 2:
            Func_080d2d98(arg0);
            break;
        case 3:
            Func_080eb754(arg0);
            break;
        case 4:
            Func_080dc968(arg0);
            break;
        case 5:
            Func_080d6970(arg0);
            break;
        case 6:
            Func_080ec100(arg0);
            break;
        case 7:
            Func_080d2458(arg0);
            break;
        case 8:
            Func_080d1714(arg0);
            break;
        case 9:
            Func_080ea0d8(arg0);
            break;
        case 10:
            Func_080d765c(arg0);
            break;
        case 11:
            Func_080e7320(arg0);
            break;
        case 12:
            Func_080e15e8(arg0);
            break;
        }
    }

    Func_08002dd8(40);
    Func_08002dd8(39);
    Func_08002dd8(41);
}

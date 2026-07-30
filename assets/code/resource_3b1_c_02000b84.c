typedef unsigned char u8;
typedef unsigned short u16;
typedef signed short s16;
typedef signed int s32;

void Func_02007050(void);
s32 Func_02007030(s32 n);
u8 *Func_02001e74(void);
void Func_02001d2e(u8 *o);
void Func_0200714c(s32 n);
void Func_02005416(s32 n);
void Func_02007102(u8 *o, s32 n);
u8 *Func_020070a0(s32 n);
void Func_020070e2(u8 *o, s32 a, s32 b);
void Func_02007110(u8 *o);
void Func_02007122(u8 *o, s32 a, s32 b);
void Func_02007164(s32 a, s32 b);
void Func_020070aa(s32 n);
void Func_02007198(s32 n);
s32 Func_020071a8(s32 a, s32 b);
s32 Func_020070e0(s32 a, s32 b);
void Func_02005476(s32 n);
void Func_02007162(s32 a, s32 b);
u8 *Func_02007100(s32 n);
void Func_02007142(s32 n, s32 a, s32 b);
void Func_02007170(s32 n);
void Func_02007182(s32 n, s32 a, s32 b);
void Func_020070ea(s32 n);
s32 Func_020070e8(s32 n);
s32 Func_020070fa();
s32 Func_0200710c();
void Func_0200711e(s32 n);
void Func_02007128(s32 n);
void Func_02005504(s32 n);
void Func_0200716c(void);

extern u8 Value_00001ea1;
extern u8 Value_00001e81;
extern u8 Value_0000092b;
extern u8 Value_00000994;
extern u8 Value_0000092a;
extern u8 Value_0000091b;
extern u8 Value_00000929;
extern u8 Value_00000939;
extern u16 *Data_03001ebc;

void Func_02000b84(void) {
    Func_02007050();
    if (Func_02007030(0x300) != 0) {
        u8 *o = Func_02001e74();
        u8 *p;

        Func_02001d2e(o);
        Func_0200714c((s32)&Value_00001ea1);
        Func_02005416(12);
        Func_02007102(o, 2);
        p = Func_020070a0(0);
        if (p != 0) {
            Func_020070e2(o, *(s16 *)(p + 10), *(s16 *)(p + 18));
        }
        Func_02007110(o);
        Func_02007122(o, 0, 0);
    } else {
        Func_02007164(12, 2);
        Func_020070aa(20);
        Func_02007198((s32)&Value_00001e81);
        Func_020071a8(12, 0);
        if (Func_020070e0(0, 0) == 0) {
            u8 *p2;

            Func_02005476(12);
            Func_02007162(12, 2);
            p2 = Func_02007100(0);
            if (p2 != 0) {
                Func_02007142(12, *(s16 *)(p2 + 10), *(s16 *)(p2 + 18));
            }
            Func_02007170(12);
            Func_02007182(12, 0, 0);
            Func_020070ea(0x300);
            if (Func_020070e8((s32)&Value_0000092b) != 0) {
                Func_020070fa((s32)&Value_00000994);
            } else if (Func_020070fa((s32)&Value_0000092a) != 0) {
                Func_0200710c((s32)&Value_0000091b);
            } else if (Func_0200710c((s32)&Value_00000929) != 0) {
                Func_0200711e((s32)&Value_00000939);
            } else {
                Func_02007128(0x930);
            }
        } else {
            u16 *q = &Data_03001ebc[236];
            *q = *q + 1;
            Func_02005504(12);
        }
    }
    Func_0200716c();
}

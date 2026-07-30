typedef unsigned char u8;
typedef unsigned short u16;
typedef signed short s16;
typedef signed int s32;

void Func_02007194(void);
s32 Func_02007174(s32 n);
u8 *Func_02001fb8(void);
void Func_02001e72(u8 *o);
void Func_02007290(s32 n);
void Func_0200555a(s32 n);
void Func_02007246(u8 *o, s32 n);
u8 *Func_020071e4(s32 n);
void Func_02007226(u8 *o, s32 a, s32 b);
s32 Func_02007254();
s32 Func_02007266();
void Func_020072ce(s32 n);
void Func_020072f0();
void Func_020072b8();
s32 Func_02007228(s32 a, s32 b);
void Func_020055be(s32 n);
void Func_020072aa(s32 a, s32 b);
u8 *Func_02007248(s32 n);
void Func_0200728a(s32 n, s32 a, s32 b);
void Func_020072ca(s32 n, s32 a, s32 b);
void Func_02007232(s32 n);
s32 Func_02007230(s32 n);
s32 Func_02007242();
void Func_0200726e(s32 n);
void Func_0200564a(s32 n);
void Func_020072b2(void);

extern u8 Value_00001ea2;
extern u8 Value_00001e84;
extern u8 Value_0000092b;
extern u8 Value_00000991;
extern u8 Value_0000092a;
extern u8 Value_00000918;
extern u8 Value_00000929;
extern u8 Value_00000936;
extern u8 Value_0000092d;
extern u16 *Data_03001ebc;

void Func_02000cc8(void) {
    Func_02007194();
    if (Func_02007174(0x300) != 0) {
        u8 *o = Func_02001fb8();
        u8 *p;

        Func_02001e72(o);
        Func_02007290((s32)&Value_00001ea2);
        Func_0200555a(9);
        Func_02007246(o, 2);
        p = Func_020071e4(0);
        if (p != 0) {
            Func_02007226(o, *(s16 *)(p + 10), *(s16 *)(p + 18));
        }
        Func_02007254(o);
        Func_02007266(o, 0, 0);
    } else {
        Func_020072ce((s32)&Value_00001e84);
        Func_020072f0(9, 0, 60);
        Func_020072b8(9, 1);
        ((s32 (*)())Func_020072f0)(9, 0);
        if (Func_02007228(0, 0) == 0) {
            u8 *p2;

            Func_020055be(9);
            Func_020072aa(9, 2);
            p2 = Func_02007248(0);
            if (p2 != 0) {
                Func_0200728a(9, *(s16 *)(p2 + 10), *(s16 *)(p2 + 18));
            }
            Func_020072b8(9);
            Func_020072ca(9, 0, 0);
            Func_02007232(0x300);
            if (Func_02007230((s32)&Value_0000092b) != 0) {
                Func_02007242((s32)&Value_00000991);
            } else if (Func_02007242((s32)&Value_0000092a) != 0) {
                Func_02007254((s32)&Value_00000918);
            } else if (Func_02007254((s32)&Value_00000929) != 0) {
                Func_02007266((s32)&Value_00000936);
            } else {
                Func_0200726e((s32)&Value_0000092d);
            }
        } else {
            u16 *q = &Data_03001ebc[236];
            *q = *q + 1;
            Func_0200564a(9);
        }
    }
    Func_020072b2();
}

typedef signed int s32;
typedef unsigned short u16;

struct Rec { u16 f00, f02, f04, f06; };

struct Rec *Func_02002c8a(s32);
void Func_02002c70(struct Rec *);
void Func_02002e10(s32);
void Func_02002d58(s32);
void Func_02002d78(s32, s32);
void Func_02002c9c(void);

void Func_02000230(void) {
    struct Rec *q = Func_02002c8a(0);
    s32 v = q->f06;
    Func_02002c70(q);
    if (v >= 0xa001 && v <= 0xdfff) {
        Func_02002e10(16);
    } else {
        Func_02002d58(0x16b3);
        Func_02002d78(16, 0);
    }
    Func_02002c9c();
}

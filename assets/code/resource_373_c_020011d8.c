typedef signed int s32;
typedef unsigned char u8;

extern u8 Value_00000808;
extern u8 Value_00000f4d;
extern u8 Value_00000366;

void Func_02007196();
void Func_020071a2();
void Func_020071d4();
void Func_020071e0();
void Func_020071f2();
s32 Func_0200717a();
void Func_02007280();
void Func_020072a2();
void Func_020072ae();
void Func_0200724e();

void Func_020011d8(void)
{
    s32 id;

    if (Func_0200717a((s32)&Value_00000808) != 0) {
        return;
    }
    Func_020071a2();
    Func_020071e0(0, 0x10000, 0x8000);
    id = (s32)&Value_00000f4d;
    Func_02007280(id);
    Func_020072a2(15, 0, 2);
    id += 2;
    Func_020072ae(16, 0, 2);
    Func_02007196(id, 1);
    Func_020071d4(6);
    Func_0200724e(0, 69, (s32)&Value_00000366);
    Func_020071f2();
}

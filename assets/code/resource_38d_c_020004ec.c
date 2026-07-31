typedef signed int s32;

extern void Func_0200296a(void);
extern s32 Func_02002ab4(s32, s32, s32);
/* 0x02002996 serves two imports: the two-argument reader that guards the
 * branch and the one-argument flag clear inside its first arm. */
extern s32 Func_02002996(s32, s32);
extern void Func_02002996_b(s32);
extern void Func_02002a1e(s32, s32);
extern void Func_0200297c(s32);
extern void Func_02002982(s32);
extern void Func_02002990(s32);
extern void Func_02002af6(s32);
extern void Func_02002a46(s32, s32);
extern void Func_02002964(s32);
extern void Func_020029c8(void);

void Func_020004ec(s32 slot, s32 subject, s32 flag)
{
    s32 record;

    Func_0200296a();

    record = Func_02002ab4(0, slot, subject);

    if (Func_02002996(subject, 0) != -1) {
        Func_02002a1e(slot, 2);
        Func_0200297c(0x84E);
        Func_02002982(flag);
        Func_02002990(0x322);
        Func_02002996_b(0x202);
    } else {
        Func_02002af6(125);
        Func_02002a46(slot, 5);
    }

    Func_02002964(record);
    Func_020029c8();
}

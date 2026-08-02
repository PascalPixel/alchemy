typedef unsigned char u8;
typedef signed int s32;

/*
 * Resource 378 scene reset at 0x020006e8 (100 bytes including its literal).
 * The prologue and the pop-{r0}/bx-r0 epilogue are unambiguous.  The literal
 * 0x116c is loaded as a value (not an in-image pointer), so it stays an
 * integer argument here.  All calls are retained in the ROM order.
 */

extern void Func_0808a018();
extern void Func_0808a208();
extern void Func_0808a220();
extern void Func_0808a218();
extern void Func_0808a010();
extern void Func_0808a148();
extern void Func_0808a110();
extern void Func_0808a170();
extern void Func_0808a180();
extern void Func_080770c8();
extern void Func_0808a020();

void Func_020006e8(void)
{
    Func_0808a018();
    Func_0808a208(0x10000, 0x2000);
    Func_0808a220(1, 1);
    Func_0808a218();
    Func_0808a010(20);
    Func_0808a148(8, 0, 0);
    Func_0808a010(10);
    Func_0808a110(8, 4);
    Func_0808a010(20);
    Func_0808a170(0x116c);
    Func_0808a180(8, 0);
    Func_080770c8(0x200);
    Func_0808a020();
}

typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;

/*
 * Resource 378 scene setup at 0x0200074c.  The data at 0x7f8--0x81b is an
 * embedded literal pool reached over by the branch at 0x7f6; the owner thus
 * runs through the return at 0x844 and the next prologue at 0x848 (252 bytes).
 * The RAM cell at 0x03001ed0 is loaded first and its pointed-to state block is
 * then updated at the literal offsets 0xe5a, 0xe5c, 0x2a00--0x2a03.
 */

extern u8 *Data_03001ed0;

extern void Func_0808a018();
extern void Func_0808a100();
extern void Func_0808a330();
extern void Func_0808a348();
extern void Func_0808a010();
extern void Func_08015210();
extern s32 Func_020027d4();
extern void Func_0808a020();
extern void Func_0808a248();

void Func_0200074c(void)
{
    u8 *state = Data_03001ed0;
    u16 *halfwords = (u16 *)state;

    Func_0808a018();
    Func_0808a100(0, 0);
    Func_0808a100(1, 0);
    Func_0808a100(11, 0);
    Func_0808a100(12, 0);
    Func_0808a100(8, 0);
    Func_0808a100(9, 0);
    Func_0808a100(10, 0);
    Func_0808a330(0x10002, 0);
    Func_0808a348(120);
    Func_0808a010(180);

    halfwords[0xe5a / 2] = 0x7c00;
    halfwords[0xe5c / 2] = 0x7c00;
    halfwords[(0xe5c + 2) / 2] = 0x7c00;
    state[0x2a00] = 0;
    state[0x2a01] = 1;
    state[0x2a02] = 1;
    state[0x2a03] = 1;

    Func_0808a010(1);
    Func_08015210(0x116d, 1, 0);
    Func_0808a330(0, 0);
    Func_0808a348(120);

    Func_0808a010(120);
    Func_0808a010(60);
    if (Func_020027d4() == 0) {
        Func_0808a020();
        Func_0808a248(20);
    } else {
        Func_0808a020();
        Func_0808a248(50);
    }
}

typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;

void Func_02001008();       /* opens a scripted sequence */
void Func_02001096();       /* queues a cue / script id */
s32 Func_020010a6();
s32 Func_02001036();        /* result is compared against 1 */
void Func_020010c2();
void Func_020010ec();
void Func_02001050();       /* closes the scripted sequence */

extern u8 *Data_03001ebc;   /* pointer cell holding the overlay workspace */

void Func_0200015c(void)
{
    u8 *workspace;

    Func_02001008();
    Func_02001096(0x18bd);
    /* r1 is set before r0; the argument order is unchanged. */
    Func_020010a6(8, 0);

    if (Func_02001036(0, 0) == 1) {
        Func_020010c2(8, 0);
    } else {
        workspace = Data_03001ebc;
        *(u16 *)(workspace + 472) = (u16)(*(u16 *)(workspace + 472) + 1);
        Func_020010ec(8, 0);
    }

    Func_02001050();
}

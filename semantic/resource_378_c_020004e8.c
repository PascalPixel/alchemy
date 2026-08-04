typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;

/*
 * Resource 378 owner at 0x020004e8 (140 bytes, through 0x02000573).
 *
 * The prologue reserves 28 bytes.  Seven words in that frame are populated
 * before the 11-argument Func_0808a1d0 call; they are written explicitly in
 * the call below in their stack order (arguments 4 through 10).  The three
 * literal words at 0x02000568--0x02000573 belong to this owner, and the
 * following push at 0x02000574 starts the next owner.
 *
 * There are ten imported calls and two branch sites.  The only persistent
 * store is the halfword increment at workspace offset +472; the seven stack
 * stores are call-argument materialization, not game-state writes.
 */

extern u8 Data_0200bc9c[];
extern u8 *Data_03001ebc;

extern void Func_02003ade(void);
extern void Func_02003af4(u8 *data);
extern void Func_02003a22(s32 frames);
extern void Func_02003ba8(s32 value);
extern void Func_02003bb8(s32 id, s32 arg1);
extern s32 Func_02003b18(s32 id, s32 arg1);
extern void Func_02003bd4(s32 id, s32 arg1);
extern void Func_02003c1a(s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32);
extern void Func_02003c12(s32 id, s32 arg1);
extern void Func_02003b56(void);

void Func_020004e8(void)
{
    Func_02003ade();
    Func_02003af4(Data_0200bc9c);
    Func_02003a22(1);
    Func_02003ba8(0x1bfd);
    Func_02003bb8(9, 0);

    if (Func_02003b18(0, 0) == 0) {
        Func_02003bd4(9, 0);
        goto finish;
    }

    (*(u16 *)(Data_03001ebc + 472))++;

    Func_02003c1a(2, 16, 1, 24,
                  1, 3, 7, 16, 1, 14, 0);
    Func_02003c12(9, 0);

finish:
    Func_02003b56();
}

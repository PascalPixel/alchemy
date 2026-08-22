#include "types.h"

/*
 * Resource 3a0 owner at 0x02000e4c (228 bytes).
 *
 * Complete owner: `push {r5, lr}` at 0x02000e4c and `pop {r5} ; pop {r0} ;
 * bx r0` at 0x02000f0e -- the popped register IS r0, so the owner is
 * **void**.  0x02000e4c + 228 = 0x02000f30, where the next owner's
 * `push {r5, r6, lr}` begins.  The span decomposes as head and dispatcher
 * (0x02000e4c-0x02000e6d), a two-byte alignment halfword at 0x02000e6e,
 * 7-entry jump table (0x02000e70-0x02000e8b, data), seven case bodies and
 * the shared tail (0x02000e8c-0x02000f13) and the literal pool
 * (0x02000f14-0x02000f2f).
 *
 * Fifth of the nine dispatch sites the tree described nowhere; adopted with
 * `--span 228`.
 *
 * A DIFFERENT selector from the other seam rows: the scene table is reached
 * through the IWRAM pointer `Data_03001ebc` rather than through
 * `Data_02000240`, the index is [182] not [225], and the guard is
 * `subs #1 ; cmp #6 ; bhi` for the range 1..7.  The pointer is loaded before
 * the first call and held in r5 across all of them, so it is a function-top
 * local.
 *
 * ONE FALLTHROUGH, and it is a jump INTO the middle of a later arm: cases 2
 * and 3 set two registers and branch to 0x02000ee4, which is inside case 6's
 * body, so all three share case 6's final call.  Written as a `goto` to a
 * label inside the case-6 arm with the two shared arguments as locals --
 * §5b6's shared-tail lever, one level further in.
 *
 * Case-arm ORDER off the ROM (§5b5); here the distinct table values ascending
 * happen to coincide with selector order, which was checked rather than
 * assumed.  Per-site RAW callee names (§5b3a).
 */

extern u8 *Data_03001ebc;

extern u8 Data_02009778[];
extern u8 Data_0200978e[];
extern u8 Data_020097a4[];
extern u8 Data_020097ba[];
extern u8 Data_020097d0[];

void Func_02002214();
void Func_02002348();
void Func_02002212();
void Func_0200235a();
void Func_02002366();
void Func_02002372();
void Func_0200223c();
void Func_02002384();
void Func_0200224e();
void Func_02002396();
void Func_02002260();
void Func_020023a8();
void Func_02002272();
void Func_02001cf0();
void Func_020022d4();

void Func_02000e4c(void)
{
    s16 *scene = (s16 *) Data_03001ebc;
    u8 *shared0;
    s32 shared1;

    Func_02002214();

    switch (scene[182]) {
    case 1:
        Func_02002348(158);
        Func_02002212(Data_02009778, 81, 18);
        break;
    case 2:
        Func_0200235a(158);
        shared0 = Data_0200978e;
        shared1 = 83;
        goto shared;
    case 3:
        Func_02002366(158);
        shared0 = Data_0200978e;
        shared1 = 86;
        goto shared;
    case 4:
        Func_02002372(158);
        Func_0200223c(Data_020097a4, 84, 24);
        break;
    case 5:
        Func_02002384(158);
        Func_0200224e(Data_020097a4, 72, 7);
        break;
    case 6:
        Func_02002396(188);
        shared0 = Data_020097ba;
        shared1 = 69;
    shared:
        Func_02002260(shared0, shared1, 11);
        break;
    case 7:
        Func_020023a8(158);
        Func_02002272(Data_020097d0, 83, 7);
        break;
    default:
        break;
    }

    Func_02001cf0(scene[182]);
    Func_020022d4();
}

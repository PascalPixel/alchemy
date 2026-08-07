/*
 * BYTE-EXACT and adopted 2026-08-07 with -fthumb-arg-before-shift-in-sheet
 * and -fthumb-call-literal-arg1-first-after-call: the two residuals were both
 * argument-setup transpositions -- `movs r1, #0' ahead of `movs r0, #9' in
 * the sheet that follows the call at 0x02000626, and `movs r0, #9' ahead of
 * `lsls r1, r1, #7' at 0x02000658.
 */

#include "types.h"
  
  
 

extern void Func_02001fd0(void);
extern void Func_0200206e(int);
extern void Func_02001d64(int,int,int);
extern void Func_02002088(int,int);
extern int Func_02002010(int,int);
extern void Func_020020b4(int,int);
extern void Func_020020d0(int,int,int);
extern void Func_02002024(void);
void Func_02000614(void)
{
    u8 *workspace;

    Func_02001fd0();
    Func_0200206e(0x1223);
    Func_02001d64(9, 0, 2);
    Func_02002088(9, 0);
    if (Func_02002010(0, 0) != 0) {
        workspace = *(u8 **)0x03001ebc;
        ++*(u16 *)(workspace + 472);
    }
    Func_020020b4(9, 0);
    Func_020020d0(9, 0x5000, 0);
    Func_02002024();
}

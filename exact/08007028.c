/*
 * VENEER AUDIT NOTE (2026-08-01) -- COMMENT ONLY, NO CODE CHANGE.
 *
 * This file is byte-exact, so nothing below is rewritten.  The note records
 * what the call sites actually are, so the next reader is not misled.
 *
 * 0x080072e4 begins the GCC `__call_via_rN` veneer bank: fifteen four-byte
 * `bx rN; nop` entries in register order r0..lr, ending at 0x08007320.  A
 * `bl` into that range is an INDIRECT CALL through the named register, not a
 * call to a function at the branch target.  The `Func_080072f*` prototype
 * this file declares is therefore a phantom, and the register load that
 * precedes each site -- which reads like dead code -- is the callee load.
 *
 * Why the file is still byte-identical while being wrong: a direct call to a
 * declared function at 0x080072f0 emits exactly the same `bl` the real
 * indirect call emits.  Converting to a function-pointer call would require
 * the compiler to choose the same register and therefore the same veneer
 * entry, which is a byte-exact source question and is deliberately NOT attempted
 * here.
 *
 * Sites in this owner, resolved with tools/veneer_resolve.ts:
 *
 *   0x08007066  __call_via_r3  ->  loaded from memory [r0, #0]
 *     CONTEXT-DEPENDENT: a function-pointer table entry, struct field or
 *     stack slot. Must be read with the surrounding code; must NOT be
 *     pattern-matched against other files.
 */
#include "types.h"

/* 0x08007c10 のフラッシュ設定表。wait は WAITCNT の SRAM 待ち値。 */
struct FlashConfig08007028 {
    u8 unknown_00[0x24];
    u16 wait;
};

void Func_08006ac0(void *);
s32 Func_080072f0(s32, u8 *, u8, s32);

/* チップ全消去。復帰用の手続きを退避してから WAITCNT の SRAM 待ちを
   設定表の値に替え、0x5555/0x2aaa へ解除列と消去命令 0x10 を書く。
   0x2aaa の番地は最初の 0xAA を書いた後に作る。 */
u16 Func_08007028(void)
{
    u8 savedCode[64];
    u16 result;
    s32 *status;
    volatile u8 *command;
    volatile u8 *unlock;

    Func_08006ac0(savedCode);

    *(volatile u16 *)0x04000204 =
        (*(volatile u16 *)0x04000204 & 0xFFFC) |
        ((struct FlashConfig08007028 *)0x08007C10)->wait;

    command = (volatile u8 *)0x0E005555;
    *command = 0xAA;
    unlock = (volatile u8 *)0x0E002AAA;
    *unlock = 0x55;
    *command = 0x80;
    *command = 0xAA;
    *unlock = 0x55;
    *command = 0x10;

    status = (s32 *)0x02004C00;
    result = Func_080072f0(3, (u8 *)0x0E000000, 255, *status);

    *(volatile u16 *)0x04000204 =
        (*(volatile u16 *)0x04000204 & 0xFFFC) | 3;

    return result;
}

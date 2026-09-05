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
 *   0x0808d888  __call_via_r1  ->  the RETURN VALUE of the call at 0x8091750
 *     The callee is whatever that call produced, not a fixed address.
 */
#include "types.h"

#define BattleAction_RunDescriptor Func_0808d828

typedef struct {
    s32 flags;
    u8 pad4[4];
    s32 val8;
    u8 padA[0x170 - 0xC];
    s16 h170;
    u8 padB[0x19C - 0x172];
    s16 h19c;
} Obj;

struct EffectDescriptor *Func_0808d48c(s32, s32);
void BattleRuntime_Reset(void);
void Func_08092b94(s32);
void Func_08092f84(s32, s32);
void Func_08091750(void);
void Func_080072e8(s32);
void Audio_PlayCue(s32);
void Func_08094354(void);
void Func_08094368(void);

s32 BattleAction_RunDescriptor(s32 arg0) {
    Obj *desc;
    Obj *desc2;
    s32 kind;
    s32 ret;
    Obj *work;

    desc = (Obj *) Func_0808d48c(2, arg0);
    ret = -1;
    work = *(Obj **)0x03001ebc;
    if ((desc != 0) && (desc->val8 != 0)) {
        if (desc->flags & 0x200) {
            work->h19c = 0;
        }
        if (desc->val8 < 0x10000) {
            BattleRuntime_Reset();
            Func_08092b94(desc->val8);
            Func_08092f84(-1, 0);
            ret = 0;
            Func_08091750();
        } else {
            Func_080072e8(arg0);
            goto block_17;
        }
    } else {
        desc2 = (Obj *) Func_0808d48c(1, arg0);
        if (desc2 != 0) {
            kind = desc2->flags & 0x30;
            switch (kind) {
            case 0: Audio_PlayCue(0x7B); break;
            case 32: Audio_PlayCue(0x80); Func_08094354(); break;
            case 48: Audio_PlayCue(0x81); Func_08094368(); break;
            }
            work->h170 = (s16) desc2->val8;
block_17:
            ret = 0;
        }
    }
    return ret;
}

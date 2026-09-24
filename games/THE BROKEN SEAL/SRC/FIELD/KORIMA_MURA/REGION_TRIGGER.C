#include "TYPES.H"

#define KorimaMura_TriggerRegionScript Func_020026cc

void Func_0200512e();
void Func_02005326();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

s32 KorimaMura_TriggerRegionScript(s32 a0)
{
    if (*(s32 *)0x0200b69c != 0) {
        if ((u32)(*(s32 *)(a0 + 8) - 0x3b0001) <= 0x51fffe && *(s32 *)(a0 + 16) > 0xd30000 && *(s32 *)(a0 + 16) <= 0x100ffff)
            goto hit;
        if ((u32)(*(s32 *)(a0 + 8) - 0x450001) <= 0x34fffe && *(s32 *)(a0 + 16) > 0xc20000 && *(s32 *)(a0 + 16) <= 0x114ffff)
            goto hit;
    } else {
        if ((u32)(*(s32 *)(a0 + 8) - 0x3b0001) <= 0x33fffe && *(s32 *)(a0 + 16) > 0xc20000 && *(s32 *)(a0 + 16) < 0xe60000)
            goto hit;
        if ((u32)(*(s32 *)(a0 + 8) - 0x6f0001) <= 0x1dfffe && *(s32 *)(a0 + 16) > 0xd80000 && *(s32 *)(a0 + 16) < 0xfa0000)
            goto hit;
        if ((u32)(*(s32 *)(a0 + 8) - 0x4e0001) <= 0x2bfffe && *(s32 *)(a0 + 16) > 0xf10000 && *(s32 *)(a0 + 16) <= 0x114ffff)
            goto hit;
    }
    return 0;
hit:
    Func_02005326(106);
    /* FAKEMATCH: the do/while loads the script address before a0. */
    do {
        Func_0200512e(a0, 0x200b5ec);
    } while (0);
    *(s32 *)0x0200b698 = 1;
    return 0;
}

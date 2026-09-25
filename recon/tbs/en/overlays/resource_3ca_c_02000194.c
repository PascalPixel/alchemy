/* NONMATCHING: 554 bytes, candidate 268, 274 differing halfwords, 210
 * halfword edits (2026-09-25). BabiFune_Func02000194, meant for
 * FIELD/BABI_FUNE/F_00194.C as a single-overlay unit binding its names at
 * their runtime addresses (an import veneer's listing offset plus 0x8000).
 * Remaining: 274 HW; decompile half-size; needs
 * Iwram_MulQ16(MathSin(phase<<9), n) wave for BG/actor bob plus
 * drifting-object spawn (update=BabiFune_UpdateDriftingObject); sibling of
 * CyclePalette/UpdateDriftingObject */
#include "TYPES.H"

/* Literal pool, read from the ROM:
 *   0x0200035c: 0x00000000  constant; Value_00000000 if the pool must hold it
 *   0x02000360: 0x03001e70  gSceneEffectWork
 *   0x02000364: 0x020097e8  overlay data at 0x020017e8
 *   0x02000368: 0x020097ec  overlay data at 0x020017ec
 *   0x0200036c: 0x03000118
 *   0x02000378: 0x020097fc  overlay data at 0x020017fc
 *   0x0200037c: 0x02009800  overlay data at 0x02001800
 *   0x0200038c: 0xffff0000
 *   0x0200039c: 0x020097f8  overlay data at 0x020017f8
 *   0x020003a0: 0x03001e40  gFrameCount
 *   0x020003a4: 0x000001f7  constant; Value_000001f7 if the pool must hold it
 *   0x020003a8: 0x02008101  Local_02000100 (overlay function)
 */

extern u8 Data_00000000[];
void Engine_MathSin();
s32 Engine_RandomNext();
s32 Engine_ObjectCreate();
void Engine_ObjectSetAnimation();
void Engine_ActorSetSpriteFlags();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ s32 Value0(s32 (*f)())
{
    return f();
}

static __inline__ s32 Value4(s32 (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    return f(a0, a1, a2, a3);
}

void BabiFune_Func02000194(void)
{
    u32 i;
    s32 p10;
    s32 p8;
    s32 p9;
    s32 rec7;
    s32 record;
    s32 value;
    s32 base5_20097ec;
    s32 slot0;
    u8 *p6;
    u8 *p5;
    u8 slot4[16];

    p9 = *(s32 *)0x03001e70;
    if (*(s32 *)0x020097e8 != 0) {
        base5_20097ec = 0x20097ec;
        Engine_MathSin((*(s32 *)base5_20097ec << 9));
    }
    if (*(s32 *)0x020097fc != 0) {
        Engine_MathSin((*(s32 *)0x02009800 << 9), 3);
    }
    if (*(s32 *)0x020097f8 != 0) {
        if ((*(s32 *)0x03001e40 & 1) != 0) {
            p6 = *(s32 *)(p9 + 228);
            p5 = *(s32 *)((p9 + 228) + 4);
            value = Value0(Engine_RandomNext);
            *(s32 *)(slot4 + 4) = 0;
            *(s32 *)(slot4) = ((s32)((s32)p6 & -0x10000) + (((value << 4) - value) << 4));
            slot0 = slot4;
            value = Value0(Engine_RandomNext);
            *(s32 *)(slot0 + 8) = ((s32)((s32)((s32)p5 & -0x10000) + (((value << 2) + value) << 5)) + 0x1e0000);
            rec7 = Value4(Engine_ObjectCreate, 0x1f7, *(s32 *)(slot0), *(s32 *)(slot0 + 4), ((s32)((s32)((s32)p5 & -0x10000) + (((value << 2) + value) << 5)) + 0x1e0000));
            if (rec7 != 0) {
                *(s32 *)(rec7 + 108) = 0x2008101;
                {
                    s32 shown = 60;
                
                    *(u16 *)(rec7 + 100) = shown;
                }
                {
                    s32 shown = 1;
                
                    *(u16 *)(rec7 + 102) = shown;
                }
                *(u8 *)(((rec7 + 102) - 17)) = (s32)Data_00000000;
                *(u8 *)(((rec7 + 100) - 65)) = 2;
                *(u8 *)(*(s32 *)(rec7 + 80) + 9) = ((-13 & *(u8 *)(*(s32 *)(rec7 + 80) + 9)) | 8);
                Engine_ActorSetSpriteFlags(rec7, 0);
                Engine_ObjectSetAnimation(rec7, 0);
            }
        }
    }
    p8 = (*(s32 *)0x03001e40 & 1);
    p10 = ((s32)((s32)p6 & -0x10000) + (((value << 4) - value) << 4));
}

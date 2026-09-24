/* NONMATCHING: 644 of 644 bytes, 3 differing halfwords (2026-09-24). Written as a
 * single-overlay unit source: Engine_* names bind at the overlay's runtime
 * import veneers (listing import offset + 0x8000), Local_* at their own
 * address + 0x8000. The actor-89 flag write is a struct field and the effect
 * call returns a value; remaining: the reference builds the 0x1c2 field
 * offset before the 0x209 store (one scheduling slot). */
#include "TYPES.H"

s32 Engine_GameFlagIsSet();
u8 * Engine_ActorSetAnimation();
void FieldScene_RunScene39e_020027ec();
void Engine_GameFlagClear();
s32 Engine_ActorGet();
void Engine_ActorSetPosition();
void Engine_MapCopyCellAttributes();
void NewEffectObject();
void FieldScene_RunRoofEnsembleSequence();
void Main_0808a5e0();
void Engine_ActorFaceDirection();


extern u8 Data_0000003d[];
extern u8 Data_02000240[];
extern s16 Data_02000240_t[][1];

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ s32 Value4(s32 (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    return f(a0, a1, a2, a3);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

struct Flags89 {
    u8 pad[89];
    u8 flags;
};

s32 Local_02003e58(s32 a0)
{
    u32 i;
    s32 rec7;
    u8 *record;

    if (Data_02000240_t[224][0] != (s32)Data_0000003d) {
    } else {
        *(s32 *)((*(s32 *)0x03001ebc + 0x1c0)) = 0x209;
        do { } while (0);
        if (Data_02000240_t[225][0] == 1) {
            if (Value1(Engine_GameFlagIsSet, 0x88f) != 0) {
                Engine_ActorSetAnimation(8, 6);
                goto L_02004092;
            }
            Engine_ActorSetAnimation(8, 5);
            if (Value1(Engine_GameFlagIsSet, 0xf14) == 0) {
                goto L_02004092;
            }
            if (Value1(Engine_GameFlagIsSet, 0x893) != 0) {
                goto L_02004092;
            }
            if (Value1(Engine_GameFlagIsSet, 0x109) != 0) {
                goto L_02004092;
            }
            FieldScene_RunScene39e_020027ec();
            goto L_02004092;
        }
        if (Data_02000240_t[225][0] != 2) {
            if (Data_02000240_t[225][0] != 4) {
                goto L_02003fd6;
            }
        }
        Call1(Engine_GameFlagClear, 0x12f);
        rec7 = Value1(Engine_GameFlagIsSet, 0x895);
        if (rec7 == 0) {
            record = Value1(Engine_ActorGet, 19);
            record[85] = rec7;
            *(s32 *)((s32)record + 12) = 0xc0000;
            *(s32 *)((s32)record + 60) = 0xc0000;
            *(s32 *)((s32)record + 24) = 0xcccc;
            *(s32 *)((s32)record + 28) = 0x8000;
            {
                s32 target = *(s32 *)((s32)record + 80);
                s32 shown = 0x8000;
            
                *(u16 *)(target + 30) = shown;
            }
            record = Value1(Engine_GameFlagIsSet, 0x89a);
            if ((s32)record == 0) {
                goto L_02003fb2;
            }
            Call3(Engine_ActorSetPosition, 18, 0xf80000, 0xd00000);
            record = Value1(Engine_GameFlagIsSet, 0x89b);
            if ((s32)record != 0) {
                goto L_02003fb2;
            }
            Call3(Engine_ActorSetPosition, 16, 0x1000000, 0xf00000);
            record = Engine_ActorGet(18);
            *(s32 *)((s32)record + 108) = 0x2008325;
            record = Value1(Engine_ActorGet, 13);
            *(s32 *)((s32)record + 108) = 0x2008325;
            record = Value1(Engine_ActorGet, 14);
            *(s32 *)((s32)record + 108) = 0x2008325;
            record = Value1(Engine_ActorGet, 15);
            *(s32 *)((s32)record + 108) = 0x2008325;
            record = Value1(Engine_ActorGet, 16);
            *(s32 *)((s32)record + 108) = 0x2008325;
        } else {
            record = Value1(Engine_ActorGet, 19);
            record[85] = 0;
            *(s32 *)((s32)record + 12) = 0xc0000;
            *(s32 *)((s32)record + 60) = 0xc0000;
            *(s32 *)((s32)record + 24) = 0xcccc;
            *(s32 *)((s32)record + 28) = 0x8000;
            {
                u8 value = *(volatile u8 *)&record[89];
            
                ((struct Flags89 *)record)->flags = (u8)(value | 8);
            }
            {
                s32 target = *(s32 *)((s32)record + 80);
                s32 shown = 0x8000;
            
                *(u16 *)(target + 30) = shown;
            }
            Call6(Engine_MapCopyCellAttributes, 14, 11, 1, 1, 14, 10);
        }
        L_02003fb2:;
        Value4((s32 (*)())NewEffectObject, 0x1300000, 0x180000, 0xe00000, 223);
        Call2((void (*)())Engine_ActorSetAnimation, 10, 5);
        Engine_ActorSetAnimation(11, 5);
        goto L_02004092;
        L_02003fd6:;
        if (Data_02000240_t[225][0] != 3) {
            goto L_02004092;
        }
        Call1(Engine_GameFlagClear, 0x12f);
        if (Value1(Engine_GameFlagIsSet, 0x895) == 0) {
            FieldScene_RunRoofEnsembleSequence();
            goto L_02004092;
        }
        record = Value1(Engine_GameFlagIsSet, 0x8b2);
        if ((s32)record != 0) {
            goto L_02004092;
        }
        Engine_ActorSetPosition(8, 0, 0);
        Engine_ActorSetPosition(9, 0, 0);
        goto L_02004092;
    }
    Main_0808a5e0(170);
    {
        u8 *record = Engine_ActorGet(9);
        ((struct Flags89 *)record)->flags |= 16;
    }
    if (Data_02000240_t[225][0] == 3) {
        if (Value1(Engine_GameFlagIsSet, 0xf14) != 0) {
            if (Value1(Engine_GameFlagIsSet, 0x894) == 0) {
                Call6(Engine_MapCopyCellAttributes, 10, 84, 1, 1, 10, 24);
            }
        }
    }
    if (Value1(Engine_GameFlagIsSet, 0x892) != 0) {
        Call3(Engine_ActorSetPosition, 9, 0x980000, 0x1880000);
        Engine_ActorFaceDirection(9, 0, 0);
        Call6(Engine_MapCopyCellAttributes, 10, 26, 1, 1, 10, 22);
    }
    L_02004092:;
    return 0;
}

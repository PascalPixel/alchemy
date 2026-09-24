#include "TYPES.H"

/* Unit bindings for scoring (declare as absolute_symbols of a unit on
 * resource_3a0:02001148):
 *   Main_0808a5e0 = 0x020094a8 (thumb)
 *   Engine_GameFlagClear = 0x020093b0 (thumb)
 *   Engine_GameFlagIsSet = 0x020093a0 (thumb)
 *   Engine_ActorFaceDirection = 0x02009480 (thumb)
 *   Engine_ActorSetPosition = 0x02009428 (thumb)
 *   Engine_ActorGet = 0x020093d0 (thumb)
 *   Engine_ActorSetSpriteFlags = 0x02009390 (thumb)
 *   Engine_ObjectSetPalette = 0x02009458 (thumb)
 *   Engine_ActorSetAnimation = 0x02009430 (thumb)
 *   Engine_MapCopyCellAttributes = 0x02009380 (thumb)
 *   Engine_TaskAddCallback = 0x02009340 (thumb)
 *   Engine_ActorSetChildValue = 0x02009450 (thumb)
 */

void Main_0808a5e0();
void Engine_GameFlagClear();
s32 Engine_GameFlagIsSet();
void Engine_ActorFaceDirection();
u8 * Engine_ActorSetPosition();
s32 Engine_ActorGet();
void Engine_ActorSetSpriteFlags();
void Engine_ObjectSetPalette();
void Engine_ActorSetAnimation();
void Engine_MapCopyCellAttributes();
void Engine_TaskAddCallback();
void Engine_ActorSetChildValue();


extern u8 Data_00000000[];
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

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

/* NONMATCHING: 466 of 488 bytes, 61 halfword edits (2026-09-24). The zero
 * at +85 of actor 21 must come from the pool into r5 as in 3af:02002618;
 * the reference also keeps the loop's second zero in r8. */
s32 Func_02001148(s32 a0, s32 a1)
{
    u32 i;
    u8 *rec7;
    u8 *record;
    s32 none;
    s32 base6_0;
    s32 v7;
    u8 *p5;

    *(s32 *)((*(s32 *)0x03001ebc + 0x1c0)) = 0x100;
    Main_0808a5e0(169);
    if (Data_02000240_t[225][0] > 9) {
        Call1(Engine_GameFlagClear, 0x12f);
    }
    if (Value1(Engine_GameFlagIsSet, 0x895) != 0) {
        Call3(Engine_ActorFaceDirection, 13, 0x8000, 0);
        Call3(Engine_ActorSetPosition, 14, 0x920000, 0x1380000);
        Engine_ActorFaceDirection(14, 0, 0);
        if (Value1(Engine_GameFlagIsSet, 0x89a) != 0) {
            Engine_ActorSetPosition(17, 0, 0);
        }
    }
    if (Value1(Engine_GameFlagIsSet, 0x8b0) != 0) {
        Engine_ActorSetPosition(17, 0, 0);
    }
    base6_0 = 0;
    none = 0;
    do {
        rec7 = Value1(Engine_ActorGet, (base6_0 + 23));
        *(u8 *)(*(s32 *)((s32)rec7 + 80) + 9) = ((*(u8 *)(*(s32 *)((s32)rec7 + 80) + 9) & -13) | 4);
        v7 = 0;
        rec7[85] = 0;
        rec7[89] = 8;
        Engine_ActorSetSpriteFlags((s32)rec7, 0);
        Engine_ObjectSetPalette((s32)rec7, 15);
        base6_0 = (base6_0 + 1);
        rec7[35] = ((254 & rec7[35]) | 2);
    } while ((u32)base6_0 <= 2);
    if (Value1(Engine_GameFlagIsSet, 0x202) != 0) {
        Call3((void (*)())Engine_ActorSetPosition, 14, 0x920000, 0x1380000);
        Engine_ActorFaceDirection(14, 0, 0);
    }
    if (Value1(Engine_GameFlagIsSet, 0x201) != 0) {
        Engine_ActorSetAnimation(20, 5);
        record = Value1(Engine_ActorGet, 20);
        p5 = *(s32 *)((s32)record + 8);
        record = Value1(Engine_ActorGet, 20);
        Call6(Engine_MapCopyCellAttributes, 3, 17, 1, 1, ((s32)p5 >> 20), (*(s32 *)((s32)record + 16) >> 20));
        Call2(Engine_TaskAddCallback, 0x2008325, 0xc80);
    }
    Engine_ActorSetChildValue(18, 2);
    record = Engine_ActorGet(18);
    *(s32 *)((s32)record + 108) = 0x2008501;
    record = Value1(Engine_ActorGet, 19);
    record[85] = v7;
    *(s32 *)((s32)record + 12) = 0x100000;
    *(s32 *)((s32)record + 60) = 0x100000;
    *(s32 *)((s32)record + 24) = 0x8ccc;
    *(s32 *)((s32)record + 28) = 0x6666;
    {
        s32 target = *(s32 *)((s32)record + 80);
        s32 shown = 0x8000;
    
        *(u16 *)(target + 30) = shown;
    }
    record = Engine_ActorGet(21);
    Engine_ActorSetSpriteFlags((s32)record, 0);
    { u8 *p = (u8 *)Engine_ActorGet(21); s32 z = (u16)(u32)Data_00000000; p[85] = z; }
    record = Value1(Engine_ActorGet, 21);
    *(s32 *)((s32)record + 12) = v7;
    record = Engine_ActorGet(21);
    *(s32 *)((s32)record + 60) = -0x80000000;
    return 0;
}

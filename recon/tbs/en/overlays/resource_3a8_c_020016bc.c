#include "TYPES.H"

/* Unit bindings for scoring (declare as absolute_symbols of a unit on
 * resource_3a8:020016bc):
 *   Engine_GameFlagIsSet = 0x0200bbd4 (thumb)
 *   Engine_GameFlagSet = 0x0200bbdc (thumb)
 *   Engine_ActorSetPosition = 0x0200bc74 (thumb)
 *   Engine_ActorSetChildValue = 0x0200bcac (thumb)
 *   Engine_ActorGet = 0x0200bc1c (thumb)
 *   Engine_HeapAllocate = 0x0200bb44 (thumb)
 *   Engine_ItemLoadIcon = 0x0200bbcc (thumb)
 *   Engine_VramLoad = 0x0200bb54 (thumb)
 *   Engine_HeapRelease = 0x0200bb4c (thumb)
 *   Engine_TaskAddCallback = 0x0200bb1c (thumb)
 */

s32 Engine_GameFlagIsSet();
s32 Engine_GameFlagSet();
void Engine_ActorSetPosition();
void Engine_ActorSetChildValue();
s32 Engine_ActorGet();
s32 Engine_HeapAllocate();
void Engine_ItemLoadIcon();
s32 Engine_VramLoad();
void Engine_HeapRelease();
void Engine_TaskAddCallback();


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

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

/* NONMATCHING: 300 of 300 bytes, 4 halfword edits (2026-09-24). The
 * reference loads the +5 flag byte before building its -33 mask; every
 * ordering of the two byte updates builds the mask first. */
void Func_020016bc(void)
{
    u32 i;
    u8 *rec;
    s32 rec2;
    s32 rec7;
    s32 record;
    u8 *p6;

    if (Value1(Engine_GameFlagIsSet, 0x941) != 0) {
        Call1(Engine_GameFlagSet, 0x321);
        Call1(Engine_GameFlagSet, 0x913);
        Call1(Engine_GameFlagSet, 0x912);
        Call1(Engine_GameFlagSet, 0x915);
    }
    if (Value1(Engine_GameFlagIsSet, 0x940) != 0) {
        Call1(Engine_GameFlagSet, 0x321);
    }
    if (Data_02000240_t[225][0] == 14) {
        Call3(Engine_ActorSetPosition, 25, 0x1a80000, 0x580000);
    }
    Engine_ActorSetChildValue(21, 2);
    rec2 = Value1(Engine_GameFlagIsSet, 0x916);
    if (rec2 != 0) {
        Engine_ActorSetPosition(26, 0, 0);
    } else {
        rec = Value1(Engine_ActorGet, 26);
        p6 = *(s32 *)((s32)rec + 80);
        {
            s32 v = p6[9];
            s32 w;

            v = (v & -13) | 4;
            w = p6[5] & -33;
            p6[5] = w;
            p6[9] = v & 15;
        }
        p6[39] = rec2;
        rec[92] = 1;
        *(u8 *)((((s32)rec + 92) - 7)) = rec2;
        *(s32 *)((s32)rec + 12) = 0xa0000;
        rec[97] = 1;
        rec7 = Value2(Engine_HeapAllocate, 17, 0x608);
        Engine_ItemLoadIcon(181);
        Engine_VramLoad(p6[28], 128, (rec7 + 0x400));
        Engine_HeapRelease(17);
        *(s32 *)((s32)rec + 48) = rec2;
        *(s32 *)((s32)rec + 56) = *(s32 *)((s32)rec + 8);
        *(s32 *)((s32)rec + 60) = *(s32 *)((s32)rec + 12);
        *(s32 *)((s32)rec + 64) = *(s32 *)((s32)rec + 16);
        Call2(Engine_TaskAddCallback, 0x200b90d, 0xc80);
    }
}

#include "TYPES.H"
#include "IO_WRITE_QUEUE.H"

extern volatile u16 Data_04000208;

/* Queue a register write for the next frame with interrupts masked, as
 * SYSTEM/IO_WRITE_QUEUE.C does; expanded in place, so the value is only
 * computed once there is room. */
#define QUEUE_IO_WRITE(address, value, delay) \
    do { \
    volatile u16 *ime; \
    struct IoWriteQueue *q; \
    u32 saved; \
    s32 count; \
     \
    ime = &Data_04000208; \
    q = &gIoWriteQueue; \
    saved = *ime; \
    *ime = (u16)(u32)ime; \
    count = q->count; \
    if (count <= 31) { \
        u32 *destination = (u32 *)((u8 *)q + count * 12 + 4); \
        *(u16 *)&q->count = count + 1; \
        *destination++ = (value); \
        *destination++ = (address); \
        *destination = (delay); \
    } \
    *ime = saved; \
    } while (0)


s32 Engine_GameFlagIsSet();
s32 Engine_GameFlagSet();
s32 Engine_ActorGet();
void Main_0808a5e0();



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

/* NONMATCHING: 328 of 324 bytes, 33 halfword edits (2026-09-24). Both
 * register writes use QUEUE_IO_WRITE expanded in place; the IME and queue
 * pointers swap r5 and r6 against the reference. */
void Func_02000f50(void)
{
    u32 i;
    s32 rec7;
    s32 record;
    s32 none;
    s32 base6_8;
    s32 v8;

    none = 0;
    v8 = none;
    if (Value1(Engine_GameFlagIsSet, 0x301) != 0) {
        Call1(Engine_GameFlagSet, 0x206);
    }
    if (Value1(Engine_GameFlagIsSet, 0x302) != 0) {
        Call1(Engine_GameFlagSet, 0x207);
    }
    if (Value1(Engine_GameFlagIsSet, 0x303) != 0) {
        Call1(Engine_GameFlagSet, 0x208);
    }
    if (Value1(Engine_GameFlagIsSet, 0x304) != 0) {
        Call1(Engine_GameFlagSet, 0x209);
    }
    if (Value1(Engine_GameFlagIsSet, 0x305) != 0) {
        Call1(Engine_GameFlagSet, 0x20a);
    }
    base6_8 = 8;
    do {
        rec7 = Value1(Engine_ActorGet, base6_8);
        if (rec7 != 0) {
            if (Value1(Engine_GameFlagIsSet, 0x109) == 0) {
                *(s32 *)(rec7 + 24) = 0x800;
                *(s32 *)(rec7 + 28) = 0x800;
            }
            *(u8 *)(*(s32 *)(rec7 + 80) + 38) = 0;
        }
        base6_8 = (base6_8 + 1);
    } while (base6_8 <= 12);
    QUEUE_IO_WRITE(0x4000050, 0x3f42, 0x20000);
    if (Value1(Engine_GameFlagIsSet, 0x340) != 0) {
        v8 = 16;
        Main_0808a5e0(244);
    }
    QUEUE_IO_WRITE(0x4000052, ((16 - v8) << 8) | v8, 0x20000);
}

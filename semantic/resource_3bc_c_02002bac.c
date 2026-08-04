typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;
typedef signed int s32;

/*
 * Resource 3bc queue/scene owner at 0x02002bac.
 *
 * The owner opens with the saved-register/high-register prologue at
 * 0x02002bac and returns through the single unwind at 0x02002df0-0x02002dfe.
 * The six directly referenced pool words at 0x02002e00-0x02002e14 follow
 * the return and the next independent prologue is 0x02002e18, establishing
 * the complete 0x26c (620-byte) span.  The pool words are the shared
 * Data_02000240 base and message ids 0x207d-0x2084.
 *
 * The first argument selects the short path (including the mode-3 message
 * exit), the second identifies the scene actor, and the third is the state
 * base used by the 0x200 flag query.  The queue count and four byte-list
 * passes are written as ordinary loops around their single assembly call
 * sites.  The message tail at 0x02002cc0 is shared by the early exits and the
 * -1 result path, so one source label preserves that one static call site.
 * All 41 resolver-confirmed calls are represented without inline assembly,
 * fixed-register bindings or translator escape constructs.
 */

extern u8 *Func_0808a080(s32 actor);
extern s32 Func_08077148(void);
extern s32 Func_080770c0(s32 flag);
extern void Func_080000c0(s32 frames);
extern void Func_0808a170(s32 message);
extern void Func_0808a178(s32 actor, s32 mode);
extern s32 Func_0808a070(s32 actor, s32 mode);
extern void Func_08077168(s32 value);
extern void Func_08077150(s32 value);
extern s32 Func_080a1018(void);
extern void Func_0808a180(s32 actor, s32 mode);
extern void Func_08015120(s32 handle, s32 mode);
extern void Func_0808a090(s32 actor, s32 x, s32 z);
extern void Func_0808a0f0(s32 actor, s32 x, s32 z);
extern void Func_0808a0d0(s32 actor, s32 x, s32 z);
extern void Func_0808a150(s32 actor, s32 target, s32 frames);
extern void Func_0808a100(s32 actor, s32 mode);
extern void Func_0808a110(s32 actor, s32 mode);
extern void Func_0808a0c8(s32 actor, s32 x, s32 z);
extern void Func_0808a588(s32 owner, s32 value);
extern void Func_0808a1b8(s32 actor, s32 speed, s32 frames);
extern void Func_080770c8(s32 flag);
extern void Func_080770e8(s32 address, s32 value);

void Func_02002bac(s32 mode, s32 actor, s32 state_base)
{
    u8 *record;
    u8 *zero_record;
    u8 queued[12];
    s32 count;
    s32 decision;
    s32 handle;
    s32 x;
    s32 z;
    s32 message;
    s32 i;

    record = Func_0808a080(actor);
    x = *(s16 *)(record + 10);
    z = *(s16 *)(record + 18);

    if (mode == 3) {
        message = 0x207e;
        goto message_tail;
    }

    count = Func_08077148();
    if (count > 0) {
        const u8 *source = (const u8 *)0x02000240 + 504;

        for (i = 0; i < count; i++)
            queued[i] = source[i];
    }

    if (count <= 1) {
        message = 0x2083;
        goto message_tail;
    }

    if (Func_080770c0(state_base + 512) != 0) {
        message = 0x2084;
        goto message_tail;
    }

    if (mode == 2) {
        Func_080000c0(6);
        decision = 0;
    } else {
        Func_0808a170(0x207d);
        Func_0808a178(actor, 0);
        decision = Func_0808a070(0, 0);
    }

    if (decision != 0)
        goto message_tail;

    if (decision < count) {
        for (i = 0; i < count; i++)
            Func_08077168((s32)(s8)queued[i]);
    }

    if (count > 0) {
        for (i = 0; i < count; i++) {
            if ((s8)queued[i] != 0)
                Func_08077150((s32)(s8)queued[i]);
        }
    }

    handle = Func_080a1018();

    if (count > 0) {
        for (i = 0; i < count; i++)
            Func_08077168((s32)(s8)queued[i]);
    }

    if (count > 0) {
        for (i = 0; i < count; i++)
            Func_08077150((s32)(s8)queued[i]);
    }

    if (handle == -1) {
        message = 0x207e;
        goto message_tail;
    }

    goto heavy_path;

message_tail:
    Func_0808a170(message);
    Func_0808a180(actor, 0);
    return;

heavy_path:
    Func_08015120(handle, 1);
    Func_0808a170(0x207f);
    Func_0808a180(actor, 0);
    Func_0808a090(0, 0x10000, 0x8000);
    Func_0808a090(handle, 0x10000, 0x8000);
    Func_0808a090(actor, 0x10000, 0x8000);

    zero_record = Func_0808a080(0);
    if (zero_record != 0)
        Func_0808a0f0(handle,
                      *(s32 *)(zero_record + 8),
                      *(s32 *)(zero_record + 16));

    Func_0808a0d0(handle, x, z + 16);
    Func_0808a0d0(0, x + 16, z + 16);
    Func_0808a150(handle, 0, 30);
    Func_0808a100(handle, 3);
    Func_0808a110(0, 3);
    Func_0808a0d0(actor, x, z - 16);
    Func_0808a0c8(actor, x + 16, z - 16);
    Func_0808a588(0, handle);
    Func_0808a0d0(handle, x, z - 16);
    Func_0808a100(actor, 1);
    Func_0808a1b8(actor, 0x8000, 0);
    Func_0808a0d0(handle, x, z - 48);
    Func_0808a0d0(actor, x, z - 16);
    Func_0808a0d0(actor, x, z);
    Func_08077168(handle);
    Func_080770c8(state_base + 512);

    record = Func_0808a080(handle);
    Func_080770e8(handle * 16 + 0xdc * 4, *(s32 *)(record + 8) >> 20);
    Func_080770e8(handle * 16 + 0xde * 4, *(s32 *)(record + 16) >> 20);
}

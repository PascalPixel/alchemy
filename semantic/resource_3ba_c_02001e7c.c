#include "types.h"

/*
 * resource_3ba owner at 0x02001e7c, 620 bytes.  This is the complete
 * per-frame scene/actor transition task: it looks up the current object,
 * validates the active roster twice, closes through one of the small state
 * ids on an early failure, and otherwise performs the actor setup sequence
 * using the object's two signed coordinates.
 *
 * The owner boundary is independently visible in the assembled overlay:
 * the saved-register prologue starts at 0x02001e7c, the epilogue is the
 * `add sp`/restore sequence at 0x020020be-0x020020ce, and the following
 * alignment halfword is at 0x020020d0.  Its six-word literal pool occupies
 * 0x020020d0-0x020020e7 (0x02000240, 0x2083, 0x2084, 0x207d, 0x207e,
 * 0x207f); the next owner's prologue begins at 0x020020e8.  Thus the
 * admitted span is 0x26c (620) bytes and includes this owner's pool.
 *
 * Three arguments are passed in r0 (mode), r1 (object handle), and r2 (an
 * output pointer).  The object fields at offsets 10 and 18 are signed
 * halfwords.  The small signed-byte roster is copied from the resident table
 * at 0x02000438.  As in the neighboring state-machine owners, the source
 * uses an eight-byte local table and an explicit bound on the copy loop; the
 * real code trusts the game invariant that the roster count never exceeds
 * the available frame storage.
 *
 * The common close block is intentionally kept in this owner body instead of
 * a helper: three control-flow paths converge on one `Func_0808a170` /
 * `Func_0808a180` pair, and retaining one lexical call site preserves the
 * assembly call order audit.  The two validation loops are likewise kept
 * separate: the first calls `Func_08077150` only for nonzero entries, while
 * the second calls it for every copied entry.
 */

extern u8 *Func_0808a080(s32 arg0);
extern s32 Func_08077148(void);
extern s32 Func_080770c0(s32 arg0);
extern void Func_080000c0(s32 arg0);
extern void Func_0808a170(s32 arg0);
extern void Func_0808a178(s32 arg0, s32 arg1);
extern s32 Func_0808a070(s32 arg0, s32 arg1);
extern void Func_08077168(s32 arg0);
extern void Func_08077150(s32 arg0);
extern s32 Func_080a1018(void);
extern void Func_08015120(s32 arg0, s32 arg1);
extern void Func_0808a180(s32 arg0, s32 arg1);
extern void Func_0808a090(s32 arg0, s32 arg1, s32 arg2);
extern void Func_0808a0f0(s32 arg0, s32 arg1, s32 arg2);
extern void Func_0808a0d0(s32 arg0, s32 arg1, s32 arg2);
extern void Func_0808a150(s32 arg0, s32 arg1, s32 arg2);
extern void Func_0808a100(s32 arg0, s32 arg1);
extern void Func_0808a110(s32 arg0, s32 arg1);
extern void Func_0808a0c8(s32 arg0, s32 arg1, s32 arg2);
extern void Func_0808a588(s32 arg0, s32 arg1);
extern void Func_0808a1b8(s32 arg0, s32 arg1, s32 arg2);
extern void Func_080770c8(s32 arg0);
extern void Func_080770e8(s32 arg0, s32 arg1);

void Func_02001e7c(s32 mode, s32 handle, u8 *outPtr)
{
    u8 *object = Func_0808a080(handle);
    s32 fieldA = *(short *)(object + 10);
    s32 fieldB = *(short *)(object + 18);
    signed char bytes[8];
    s32 count = 0;
    s32 closeId = 0;
    s32 id2 = -1;
    s32 i;
    u8 *actor;

    if (mode == 3) {
        closeId = 0x207e;
    } else {
        count = Func_08077148();
        if (count > 0) {
            for (i = 0; i < count && i < 8; i++) {
                bytes[i] = *(signed char *)(0x02000438 + i);
            }
        }

        if (count <= 1) {
            closeId = 0x2083;
        } else if (Func_080770c0((s32)outPtr + 0x200) != 0) {
            closeId = 0x2084;
        } else {
            if (mode == 2) {
                Func_080000c0(6);
            } else {
                Func_0808a170(0x207d);
                Func_0808a178(handle, 0);
                if (Func_0808a070(0, 0) != 0) {
                    closeId = 0x207e;
                }
            }

            if (closeId == 0) {
                for (i = 0; i < count; i++) {
                    Func_08077168(bytes[i]);
                }
                for (i = 0; i < count; i++) {
                    if (bytes[i] != 0) {
                        Func_08077150(bytes[i]);
                    }
                }

                id2 = Func_080a1018();

                for (i = 0; i < count; i++) {
                    Func_08077168(bytes[i]);
                }
                for (i = 0; i < count; i++) {
                    Func_08077150(bytes[i]);
                }

                if (id2 == -1) {
                    closeId = 0x207e;
                }
            }
        }
    }

    if (closeId != 0) {
        Func_0808a170(closeId);
        Func_0808a180(handle, 0);
        return;
    }

    Func_08015120(id2, 1);
    Func_0808a170(0x207f);
    Func_0808a180(handle, 0);
    Func_0808a090(0, 0x10000, 0x8000);
    Func_0808a090(id2, 0x10000, 0x8000);
    Func_0808a090(handle, 0x10000, 0x8000);

    actor = Func_0808a080(0);
    if (actor != 0) {
        Func_0808a0f0(id2, *(s32 *)(actor + 8), *(s32 *)(actor + 16));
    }
    Func_0808a0d0(id2, fieldA, fieldB + 16);
    Func_0808a0d0(0, fieldA + 16, fieldB + 16);
    Func_0808a150(id2, 0, 30);
    Func_0808a100(id2, 3);
    Func_0808a110(0, 3);
    Func_0808a0d0(handle, fieldA, fieldB - 16);
    Func_0808a0c8(handle, fieldA + 16, fieldB - 16);
    Func_0808a588(0, id2);
    Func_0808a0d0(id2, fieldA, fieldB - 16);
    Func_0808a100(handle, 1);
    Func_0808a1b8(handle, 0x8000, 0);
    Func_0808a0d0(id2, fieldA, fieldB - 48);
    Func_0808a0d0(handle, fieldA, fieldB - 16);
    Func_0808a0d0(handle, fieldA, fieldB);
    Func_08077168(id2);
    Func_080770c8((s32)outPtr + 0x200);

    actor = Func_0808a080(id2);
    Func_080770e8(id2 * 16 + 0x370, *(s32 *)(actor + 8) >> 20);
    Func_080770e8(id2 * 16 + 0x378, *(s32 *)(actor + 16) >> 20);
}

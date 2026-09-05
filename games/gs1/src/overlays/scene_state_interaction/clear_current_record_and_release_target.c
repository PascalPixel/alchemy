#include "types.h"

#define SceneState_ClearCurrentRecordAndReleaseTarget Func_02005578

/*
 * resource_39c owner at 0x02005578, 72 bytes: clear the current-rec
 * flag word and, if the rec has a linked object, reset one of its
 * fields and notify twice.
 *
 * Complete owner: `push {r5, r6, r7, lr}` at 0x02005578 through `pop
 * {r5, r6, r7} / pop {r0} / bx r0` at 0x20055ae-0x20055b2, followed by the
 * three-word literal pool 0x020055b4-0x020055bf; the next owner's prologue
 * is at 0x020055c0. No incoming arguments are read before being
 * overwritten, so `void`.
 *
 * Not found by the structural inventory walk (unindexed): reached only by
 * `bl`, resolved with `cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml --`'s `+2` rule.
 *
 * `rec = *Data_03001edc` is the same current-object global
 * resource_39c_c_02005900.c already documents (a pointer read once at
 * 0x03001edc, dereferenced once more to reach the live rec).
 *
 * Uncertainty: Func_0200b022 and Func_0200af6e are not identified; the
 * latter is called twice with the same object but very different-looking
 * second arguments (an address-taken literal, then the plain small
 * integer 7) so its real parameter meaning is unclear -- recorded as
 * compiled, not unified.
 */

extern s32 **Data_03001edc;

extern void Func_0200b022(s32 arg0);
extern void Func_0200af6e(s32 *object, s32 arg1);

void SceneState_ClearCurrentRecordAndReleaseTarget(void)
{
    s32 *rec = *Data_03001edc;
    s32 *target;

    if (rec[0] == 0) {
        return;
    }

    rec[0] = 0;
    Func_0200b022(0x161);

    target = (s32 *)rec[5];
    if (target != 0) {
        *(short *)((u8 *)target + 0x64) = 0;
        Func_0200af6e(target, (s32)0x0200de2c);
        Func_0200af6e(target, 7);
        rec[5] = 0;
    }
}

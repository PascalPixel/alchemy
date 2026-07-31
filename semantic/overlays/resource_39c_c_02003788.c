typedef unsigned char u8;
typedef int s32;

/*
 * resource_39c owner at 0x02003788, 216 bytes: one-time scene setup for
 * "self" -- if a linked object for id 265 doesn't already exist, run a
 * long linear sequence of state-clearing and positioning calls.
 *
 * Complete owner: `push {r5, r6, r7, lr}` at 0x02003788 through `pop
 * {r5, r6, r7} / pop {r0} / bx r0` at 0x02003846-0x0200384a, followed by
 * the five-word literal pool 0x0200384c-0x0200385f; the next owner's
 * prologue is at 0x02003860. No incoming arguments are read before being
 * overwritten, so `void`.
 *
 * Not found by the structural inventory walk (unindexed): reached only by
 * `bl`, resolved with `bun tools/overlay_call_targets.ts`'s `+2` rule.
 *
 * The owner-record[9] update (`(field9 & ~0xc) | 4`) is the same idiom
 * resource_39c_c_02005388.c/02005458.c use elsewhere in this overlay.
 * `self[0x55]` is written twice on the init path -- 0 early, then 3 near
 * the end -- and `self[0x6c]` likewise (a pool constant, then 0);
 * recorded exactly as compiled, not collapsed, since these are genuinely
 * two different writes in sequence rather than a repeated identical
 * store.
 *
 * Uncertainty: none of the seventeen callees are identified beyond call
 * shape; `Func_02009454` is called twice with different argument counts
 * (one, then three) and is declared old-style rather than miscredited
 * either shape, per this project's "arities vary per site" convention
 * (resource_3cb_c_02000580.c). The fixed-point conversion of `self`'s
 * fields 0x10/0x12 (signed halfwords, shifted to 16.16, one of them
 * additionally offset by -1.0 via the 0xfff00000 pool constant) is
 * recorded as read, not decoded further.
 */

extern void *Func_02009262(s32 arg0);
extern void *Func_0200921a(s32 arg0);
extern void Func_02009254();
extern void Func_0200936a(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
extern void Func_020092f2(s32 arg0, s32 arg1, s32 arg2);
extern void Func_02009342(s32 arg0, s32 arg1);
extern void Func_020092b0(s32 arg0);
extern void Func_0200920e(s32 arg0, s32 arg1);
extern void Func_020093da();
extern void Func_020093ee();
extern void Func_02009454();
extern void Func_020092da(s32 arg0, s32 arg1, s32 arg2);
extern void Func_0200937c(s32 arg0, s32 arg1);
extern void Func_020092ea(s32 arg0);
extern void Func_02009248(s32 arg0, s32 arg1);
extern void Func_02009482(s32 arg0, s32 arg1, s32 arg2);
extern void Func_0200949c();
extern void Func_02009300();

void Func_02003788(void)
{
    u8 *self = (u8 *)Func_02009262(0);
    void *existing = Func_0200921a(0x109);
    u8 *ownerRecord;

    if (existing != 0) {
        return;
    }

    Func_02009254();

    self[0x55] = 0;
    Func_0200936a(-1, -1, -1, 0);

    Func_020092f2(0, *(short *)(self + 10) << 16, (*(short *)(self + 18) << 16) - 0x10000);
    Func_02009342(0, 15);
    Func_020092b0(0);
    Func_0200920e(0, 0);
    Func_020093da();
    Func_020093ee();
    Func_02009454(228);

    *(s32 *)(self + 0x6c) = 0x02008cd1;
    Func_020092da(0, 0x6666, 0x3333);
    Func_02009454(0, 0, 8);
    Func_0200937c(0, 0);
    Func_020092ea(0);
    Func_02009248(0, 1);

    ownerRecord = *(u8 **)(self + 0x50);
    ownerRecord[9] = (ownerRecord[9] & ~0x0c) | 4;

    Func_02009482(0, 0, 10);

    self[0x55] = 3;
    *(s32 *)(self + 0x6c) = 0;

    Func_0200949c();
    Func_02009300();
}

#include "types.h"

/*
 * resource_3bc owner at 0x0200457c, 84 bytes: decompress an archived asset
 * into this overlay's other workspace, then -- unless a companion entity is
 * present -- seed five fields of the balance-snapshot block with fresh
 * values, and finally install a task at a fixed rate regardless.
 *
 * Complete owner: `push {r5, r6, lr}` at 0x0200457c through the matching
 * interworking return `pop {r5, r6} / pop {r0} / bx r0` at 0x20045b4-
 * 0x20045b8, followed by the five-word literal pool 0x20045ba-0x20045cf
 * that closes the 84-byte span. One argument (r0), void.
 *
 * Not found by the structural inventory walk (unindexed); one incoming call
 * site per overlay_unindexed.ts. This owner makes four outgoing calls, all
 * resolved with the `+2` rule against the raw image (this owner starts
 * outside a recorded row, so overlay_call_targets.ts's own table does not
 * cover it -- resolved with `bun tools/overlay_show.ts resource_3bc 457c |
 * bun tools/overlay_call_targets.ts resource_3bc 457c 45b8 --annotate`):
 *   0x2004584 -> veneer -> Func_08000290(assetId)          archived asset by id, established (resource_3bc_c_02002e54.c)
 *   0x200458c -> veneer -> Func_080001a8(source, dest)     decompress into scratch, established, same file
 *   0x2004592 -> veneer -> Func_080770c0(0x109)             story-flag query, established; 0x109 is the exact "selects a companion entity" flag documented in semantic/overlays/resource_3a8_c_02003a3c.c
 *   0x20045b0 -> veneer -> Func_080000d0(0x0200bef1, 0xc85)  install a per-frame task, established (resource_3bc_c_02003abc.c); the callback pointer is kept as the raw link-base literal per that file's own convention, not resolved to a symbolic name
 *
 * 0x03001f3c is a second overlay workspace pointer (distinct from the
 * 0x03001ebc / 0x03001e68 pair documented in other resource_3bc units); this
 * owner only touches its +224 and +240 fields, so nothing further about it
 * is established here.
 *
 * Data_02001000 is the established "balance snapshot" scalar (semantic/
 * overlays/resource_3b7_c_020009f8.c, resource_3bc_c_02003cd0.c); this
 * owner treats its address as the base of a small struct and writes five
 * consecutive u16 fields (+0, +2, +4, +6, +8), so the established scalar
 * covers only this struct's first field. Fields +6/+8 are stored from the
 * same value the companion check already read (0, since this whole block
 * is reachable only when the query returned 0).
 */

extern u8 Data_02001000[];

u8 *Func_08000290();           /* archived asset by id, established */
void Func_080001a8();          /* decompress into scratch, established */
s32 Func_080770c0();           /* story-flag query, established */
void Func_080000d0();          /* install a per-frame task, established */

void Func_0200457c(s32 assetId)
{
    u8 *workspace;
    u8 *source;
    s32 companion;

    workspace = *(u8 **)0x03001f3c;

    source = Func_08000290(assetId);
    Func_080001a8(source, workspace + 240);

    companion = Func_080770c0(0x109);
    if (companion == 0) {
        *(u16 *)(Data_02001000 + 0) = 1;
        *(u16 *)(Data_02001000 + 2) = 1;
        *(u16 *)(Data_02001000 + 4) = *(u16 *)(workspace + 224);
        *(u16 *)(Data_02001000 + 6) = (u16)companion;
        *(u16 *)(Data_02001000 + 8) = (u16)companion;
    }

    Func_080000d0(0x0200bef1, 0xc85);
}

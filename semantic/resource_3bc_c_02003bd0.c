#include "types.h"

/*
 * resource_3bc owner at 0x02003bd0, 196 bytes: when the caller's scene
 * entity exists and its kind byte reads 1, re-roll its equipment palette --
 * reserve a gradient workspace, DMA-zero 32 words into it, upload a new
 * ramp, release the old weapon handle, and merge the upload id into a
 * packed field -- otherwise do nothing.
 *
 * Complete owner: `push {r5, r6, r7, lr}` plus the r8/r9/sl saves and a
 * 4-byte frame, at 0x02003bd0, through the matching interworking return
 * `pop {r3} / mov r8,r3 / mov r9,r5 / mov sl,r6 / pop {r5, r6, r7} /
 * pop {r0} / bx r0` at 0x2003c84-0x2003c92.  The popped word is the return
 * address (r0), and no explicit value is set on either exit path, so this
 * owner is void.  Two arguments (r0, r1).
 *
 * Not found by the structural inventory walk (unindexed); two incoming call
 * sites per overlay_unindexed.ts. This owner makes seven outgoing calls
 * over six distinct targets, all resolved with the `+2` rule against the
 * raw image (this owner starts outside a recorded row, so
 * overlay_call_targets.ts's own table does not cover it -- resolved with
 * `bun tools-rs/overlay-show resource_3bc 3bd0 | bun
 * tools/lib/overlay_call_targets.ts resource_3bc 3bd0 3c92 --annotate`):
 *   0x2003bde -> veneer -> Func_0808a080(selector)   scene-record accessor, established
 *   0x2003bfc -> veneer -> Func_08000140(17, 0x608)  established, "reserve workspace on a channel" (semantic/overlays/resource_3a8_c_02003a3c.c: "reserves 0x608 (0xc1 << 3) bytes on channel 17")
 *   0x2003c1c -> veneer -> Func_08015250(arg1)       established, shared-workspace side effect, same file
 *   0x2003c26 -> veneer -> Func_080001c8(paletteIndex, 0x80, gradient)   established, upload a palette ramp, same file
 *   0x2003c2e -> veneer -> Func_08000150(17)         established, commit the channel reservation, same file
 *   0x2003c3c -> veneer -> Func_08009060(weaponHandle)   release, unestablished elsewhere
 *
 * `entity` (Func_0808a080's return) and `handle` (`*(entity + 0x50)`) are
 * the SAME SceneEntity/SceneHandle pair semantic/overlays/
 * resource_3a8_c_02003a3c.c and resource_373_c_02005b48.c document in full:
 * `handle->paletteIndex` at +0x1c, `handle->field27` at +0x27 (zeroed here,
 * exactly as that sibling does) and `handle->flags05 &= ~0x20` at +5 are
 * BYTE-IDENTICAL operations to that file's idle-presentation reset;
 * `entity->field5c` at +0x5c is likewise the same field, set here to the
 * kind byte read from `entity+0x54` -- which is always 1 on the reachable
 * path, so the effect matches the sibling's unconditional `entity->field5c
 * = 1;` exactly. `handle+8` (u16, bits merged from the upload id) and
 * `handle+0x28` (word, the released weapon handle) and `handle+0x25/0x26`
 * are NOT part of that sibling's (partial) struct and are read here as raw
 * offsets rather than guessed field names.
 *
 * The DMA3 write at 0x040000d4/d8/dc (source = the address of a single
 * local zero word, dest = the reserved gradient workspace, control
 * 0x85000020 = enable | ... | 32 words) zero-fills the workspace before the
 * ramp upload; no busy-wait follows, unlike the DMA idiom in
 * resource_3bc_c_02002e54.c, so none is added here.
 */

u8 *Func_0808a080();           /* scene-record accessor, established */
s32 Func_08000140();           /* reserve workspace on a channel, established */
s32 Func_08015250();           /* shared-workspace side effect, established */
s32 Func_080001c8();           /* upload a palette ramp, established */
void Func_08000150();          /* commit the channel reservation, established */
void Func_08009060();          /* release a handle, unestablished */

void Func_02003bd0(s32 selector, s32 arg1)
{
    u8 *entity;
    u8 *handle;
    s32 kind;
    s32 gradient;
    s32 zeroWord;
    s32 uploadId;
    u16 merged;
    u32 weaponHandle;

    entity = Func_0808a080(selector);
    if (entity == 0) {
        return;
    }

    kind = entity[0x54];
    if (kind != 1) {
        return;
    }

    handle = *(u8 **)(entity + 0x50);

    gradient = Func_08000140(17, 0xc1 << 3) + (0x80 << 3);

    *(volatile u32 *)0x040000dc = 0x85000020;
    zeroWord = 0;
    *(volatile u32 *)0x040000d8 = (u32)gradient;
    *(volatile u32 *)0x040000d4 = (u32)&zeroWord;

    Func_08015250(arg1);

    uploadId = Func_080001c8(handle[0x1c], 0x80, gradient);
    Func_08000150(17);

    entity[0x5c] = (u8)kind;

    weaponHandle = *(u32 *)(handle + 0x28);
    *(u32 *)(handle + 0x28) = 0;
    handle[0x27] = 0;
    Func_08009060(weaponHandle);

    handle[5] = (u8)(handle[5] & ~0x20);

    merged = (*(u16 *)(handle + 8) & 0xfc00) | (uploadId & 0x3ff);
    *(u16 *)(handle + 8) = merged;

    handle[0x25] = 0;
    handle[0x26] = 0;
}

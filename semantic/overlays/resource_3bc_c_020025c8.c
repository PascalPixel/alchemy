typedef signed int s32;

/*
 * resource_3bc owner at 0x020025c8, 120 bytes: place three entities (slots
 * 1, 2, 3) at positions read back from six saved story values, two per
 * entity (x, z), each scaled into Q20 world space and centered on its tile
 * (+0x80000, half a 1.0-unit tile).
 *
 * Complete owner: `push {r5, r6, lr}` at 0x020025c8 through the matching
 * interworking return `pop {r5, r6} / pop {r0} / bx r0` at
 * 0x0200263a-0x0200263e.  No literal pool -- every operand is an immediate
 * or a shifted immediate.  No arguments, void.
 *
 * Not found by the structural inventory walk (unindexed); two incoming call
 * sites per overlay_unindexed.ts.  This owner makes six outgoing calls over
 * two distinct targets, both resolved with the `+2` rule against the raw
 * image (this owner starts outside a recorded row, so
 * overlay_call_targets.ts's own table does not cover it -- resolved with
 * `bun tools/overlay_show.ts resource_3bc 25c8 | bun
 * tools/overlay_call_targets.ts resource_3bc 25c8 263e --annotate`):
 *   0x20025ce, 0x20025d8, 0x20025f6, 0x2002600, 0x200261a, 0x2002624
 *     -> veneer -> Func_080770e0(id)         "read a story value", established (resource_3ba_c_02000a3c.c), ids 896/904/912/920/928/936 (224/226/228/230/232/234 << 2, the same id<<2 addressing that file documents)
 *   0x20025ee, 0x2002612, 0x2002636
 *     -> veneer -> Func_0808a0f0(slot, x, z)  "place an entity at (x, z)", established (resource_395/37a/3c6/373)
 *
 * The six story-value ids are not otherwise cross-referenced from this
 * owner alone; they run consecutively in steps of 2 (224, 226 | 228, 230 |
 * 232, 234), consistent with three adjacent 4-id story blocks of which ids
 * 0 and 2 are used here, but the block layout beyond that is not
 * established.
 */

s32 Func_080770e0();           /* read a story value, established */
void Func_0808a0f0();          /* place an entity at (x, z), established */

void Func_020025c8(void)
{
    s32 x, z;

    x = (Func_080770e0(896) << 20) + 0x80000;
    z = (Func_080770e0(904) << 20) + 0x80000;
    Func_0808a0f0(1, x, z);

    x = (Func_080770e0(912) << 20) + 0x80000;
    z = (Func_080770e0(920) << 20) + 0x80000;
    Func_0808a0f0(2, x, z);

    x = (Func_080770e0(928) << 20) + 0x80000;
    z = (Func_080770e0(936) << 20) + 0x80000;
    Func_0808a0f0(3, x, z);
}

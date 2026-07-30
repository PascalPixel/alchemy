typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;

/*
 * resource_377 owner at 0x02000284, 92 bytes.  Complete owner: 'push {lr}'
 * prologue and the interworking return 'pop {r0} ; bx r0'.  r0 holds the popped
 * return address, so the owner returns nothing.
 *
 * One scripted beat, bracketed by the established scene pair
 * Func_0808a018 / Func_0808a020 (both argument-less; the same bracket appears in
 * semantic/overlays/resource_39f_c_02001150.c and resource_373_c_02002f14.c).
 * Nothing reads r0 on entry, so the owner takes no argument.
 *
 * Literal pool: two words at owner offsets 0x54 and 0x58 (0x000011aa and
 * 0x03001ebc), preceded by an alignment 'movs r0,r0'.  Control flow never
 * reaches them -- the epilogue at 0x020002d4 precedes them -- so they are pool,
 * derived by walking the owner from its prologue rather than from the
 * disassembler's referenced-words list.  0x000011aa is a cue id; 0x03001ebc is
 * the family workspace pointer cell.
 *
 * The 'else' arm bumps the u16 at workspace + 472 -- the skip-beat counter this
 * overlay family uses on the absent side of a scene-variant test, the same slot
 * assets/code/resource_383_c_020003bc.c increments.  'movs r3,#236 ; lsls #1'
 * is 472.
 *
 * All eight calls resolved with
 * 'bun tools/overlay_call_targets.ts resource_377 0284 --json' (8 sites,
 * 8 distinct veneer targets, so the multiset below is one call each):
 *   0x0286 -> 0x190c -> Func_0808a018()
 *   0x0290 -> 0x19a4 -> Func_0808a148(14, 0, 10)
 *   0x0296 -> 0x19c4 -> Func_0808a170(0x11aa)
 *   0x029e -> 0x19cc -> Func_0808a178(14, 0)
 *   0x02a6 -> 0x1924 -> Func_0808a070(0, 0)   result tested
 *   0x02b2 -> 0x19d4 -> Func_0808a180(14, 0)
 *   0x02cc -> 0x19e4 -> Func_0808a190(14, 0)
 *   0x02d0 -> 0x1914 -> Func_0808a020()
 * The disassembler's own 'bl' annotations are wrong, as on every overlay.
 */

extern u8 *Data_03001ebc;

void Func_0808a018();
void Func_0808a148();
void Func_0808a170();
void Func_0808a178();
s32 Func_0808a070();
void Func_0808a180();
void Func_0808a190();
void Func_0808a020();

void Func_02000284(void)
{
    Func_0808a018();
    Func_0808a148(14, 0, 10);
    Func_0808a170(0x11AA);
    Func_0808a178(14, 0);

    if (Func_0808a070(0, 0) == 0) {
        Func_0808a180(14, 0);
    } else {
        *(u16 *)(Data_03001ebc + 472) += 1;
        Func_0808a190(14, 0);
    }

    Func_0808a020();
}

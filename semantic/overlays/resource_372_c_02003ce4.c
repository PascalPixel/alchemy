typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef unsigned char u8;

/*
 * Resource 372 per-frame cutscene camera/entity update at 0x02003ce4
 * (row span 296 bytes, 6 calls).
 *
 * Complete owner: `push {r5, r6, r7, lr} / mov r7, r8 / push {r7}` at
 * 0x02003ce4, matching `pop {r3} / mov r8, r3 / pop {r5, r6, r7} / pop {r0} /
 * bx r0` at 0x02003df6.  `pop {r0} ; bx r0` — r0 is the popped return address,
 * so the owner returns nothing.
 *
 * LINK BASE, measured here: this owner contains an embedded 9-entry jump table
 * physically at file offset 0x3dac, and the dispatcher loads its base from the
 * pool word 0x0200bdac (0x02003e08).  The difference is exactly 0x8000, and
 * every table entry (0x0200bdd0, 0x0200bdd6, 0x0200bddc) then lands on a real
 * `ldr r3,[r5,#24] / ldr r2,[pc,#imm]` case body at 0x3dd0 / 0x3dd6 / 0x3ddc.
 * So **resource_372 is linked at 0x02008000**, the same as resource_3bf and
 * resource_3c4: a pool word in 0x0200_8xxx..0200_dxxx is an in-image address at
 * offset = value - 0x8000.  (`work/claude/notes/resource_372-3ce4.md` parked
 * this row for the byte-exact lane precisely because that table cannot be
 * emitted from C at the current link base.  That is a byte-exactness obstacle,
 * not a semantic one, so the switch is written out normally below.)
 *
 * The bodies at 0x3dd0/0x3dd6/0x3ddc are disassembled by hand from the
 * `.4byte` halfwords the reconstruction records for 0x02003dd0-0x02003de9;
 * `bun tools/overlay_show.ts resource_372 3dc8 -n 80` decodes them as code.
 * All three select a delta and fall into the shared tail at 0x3de0.
 *
 * Uncertainties: the two handles (19 and 27) and the meaning of the deltas
 * 0xa3d / 0x51e / -1966 are not identified; they are added to two adjacent
 * words at +24 and +28 of the second object, which reads like a 16.16 pair.
 * 0xe666 is 0.8999 in 16.16 but is only asserted here as the literal it is.
 * Note also that the row's advertised 296 bytes stop at 0x02003e0c, while the
 * owner's last pool word ends at 0x02003e17 (the three case deltas live at
 * 0x02003e0c/0x02003e10/0x02003e14); the true extent is 308 bytes.
 */

/* Imports, old-style: interfaces unknown.  Named by the address each call site
 * computes, per this overlay's established convention. */
u8 *Func_0200841a();
u8 *Func_02008422();
void Func_020083da();
void Func_020083f6();
void Func_02008412();
void Func_0200842c();

void Func_02003ce4(void)
{
    u8 *a;        /* r7 */
    u8 *b;        /* r5 */
    u8 *c;        /* r8, = *(void **)(b + 80), captured before any call */
    s16 t;
    u16 h;
    s32 sel;
    s32 delta;

    a = Func_0200841a(19);
    b = Func_02008422(27);
    c = *(u8 **)(b + 80);

    t = *(s16 *)(a + 100);
    if (t != 0) {
        h = *(u16 *)(a + 100);

        /* The first test uses the signed halfword loaded before the reloads;
         * the rest use the unsigned halfword, re-read after every call because
         * each callee may write it. */
        if (t == 60) {
            Func_020083da(0x30000, 0x30000, 0x10000);   /* 192<<10, 128<<9 */
            h = *(u16 *)(a + 100);
        }
        if (h == 40) {
            Func_020083f6(0x40000, 0x40000, 0x10000);   /* 128<<11 */
            h = *(u16 *)(a + 100);
        }
        if (h == 30) {
            Func_02008412(0x20000, 0x20000, 0x10000);   /* 128<<10 */
            h = *(u16 *)(a + 100);
        }
        if (h == 20) {
            Func_0200842c(-1, -1, 0xe666);
            h = *(u16 *)(a + 100);
        }
        *(u16 *)(a + 100) = (u16)(h - 1);
    }

    *(s32 *)(b + 8) = *(s32 *)(a + 8);
    *(s32 *)(b + 56) = *(s32 *)(a + 8);
    *(s32 *)(b + 16) = *(s32 *)(a + 16);
    c[35] = 10;

    if ((*(u32 *)0x03001e40 & 1) != 0) {
        sel = *(s16 *)(a + 102) - 1;
        if ((u32)sel <= 8) {
            /* Jump table at 0x02003dac, base 0x0200bdac:
             *   sel 0,4       -> 0x3dd0, delta 0x0a3d
             *   sel 3         -> 0x3dd6, delta 0x051e
             *   sel 1,2,5..8  -> 0x3ddc, delta 0xfffff852 (-1966)   */
            if (sel == 0 || sel == 4) {
                delta = 0xa3d;
            } else if (sel == 3) {
                delta = 0x51e;
            } else {
                delta = -1966;
            }
            *(s32 *)(b + 24) += delta;
            *(s32 *)(b + 28) += delta;
        }
        *(s32 *)(c + 24) = *(s32 *)(b + 24);
    } else {
        /* r2 is the masked flag word, which is zero on this path. */
        *(s32 *)(c + 24) = 0;
    }
}

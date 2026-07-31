typedef unsigned char u8;
typedef unsigned short u16;
typedef signed short s16;
typedef signed int s32;

/*
 * resource_3a4 owner at 0x020025c0, 256 bytes: scene-entry
 * reconciliation -- four independent story-flag checks that patch the
 * scene to match saved progress: re-place actor 8 for scene selector 2,
 * always run the marker-drop helper on actor 9, optionally mark actor
 * 9's record and pop a panel, clear a bit in the scene-state +20
 * halfword, then pick one of two closing panel/window pairs.
 *
 * Complete owner: `push {r5, lr}` at 0x020025c0, `sub sp, #8`, through
 * `add sp, #8 / pop {r5} / pop {r0} / bx r0` at 0x200269e-0x20026a4,
 * trailing literal pool at 0x20026a6-0x20026bf, immediately followed by
 * the next unindexed candidate at 0x020026c0.  No arguments, void.
 *
 * The same shared bases as the neighbouring rows: 0x02000240 is the
 * runtime scene table (+450 selector halfword, as in
 * resource_3a4_c_02002934.c), 0x03001e70 the scene-state pointer the
 * twins use; its +20 halfword is masked with 0xfdff (bit 9 cleared) --
 * numerically the same mask the exact 0x6a0 callback applies to
 * DISPCNT, here applied to state, not the register.
 *
 * All 13 bl sites resolve with the `+2` stored-displacement rule; 12
 * veneers, one local:
 *   0x20025d6 (0x109), 0x20025f6 (0x200), 0x200262e (0x907),
 *     0x2002644 (0x326) -> Func_080770c0   test story flag
 *   0x20025e8 -> Func_0808a0f0             place entity (id, x, z)
 *   0x20025ee -> image offset 0x3460       local Func_02003460(9)
 *   0x2002600 -> Func_0808a080             record accessor
 *   0x200260a -> Func_0808a100             (id, n)
 *   0x2002620, 0x200265c, 0x2002686 -> Func_080091c0   six-arg panel
 *   0x2002670, 0x200269a -> Func_08009180  six-arg window call
 */

s32 Func_080770c0();           /* test story flag, established */
u8 *Func_0808a080();           /* scene-record accessor, established */
void Func_0808a0f0();          /* place entity (id, x, z), established */
void Func_0808a100();          /* (id, n) */
void Func_080091c0();          /* six-argument panel call */
void Func_08009180();          /* six-argument window call */
void Func_02003460(s32 selector);   /* local, resource_3a4_c_02003460.c */

void Func_020025c0(void)
{
    u8 *record;

    if (*(s16 *)((u8 *)0x02000240 + 450) == 2
        && Func_080770c0(0x109) == 0) {
        Func_0808a0f0(8, 179 << 17, 208 << 15);
    }

    Func_02003460(9);

    if (Func_080770c0(0x200) != 0) {
        record = Func_0808a080(9);
        Func_0808a100(9, 5);
        Func_080091c0(45, 41, 1, 1, 43, 41);
        record[0x23] |= 2;
    }

    if (Func_080770c0(0x907) != 0) {
        *(u16 *)(*(u8 **)0x03001e70 + 20) &= 0xfdff;
    }

    if (Func_080770c0(0x326) != 0) {
        Func_080091c0(17, 93, 1, 1, 16, 92);
        Func_08009180(46, 29, 16, 28, 1, 2);
    } else {
        Func_080091c0(15, 93, 1, 1, 16, 92);
        Func_08009180(47, 29, 16, 28, 1, 2);
    }
}

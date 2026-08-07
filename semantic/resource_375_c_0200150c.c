#include "types.h"

/*
 * resource_375 owner at 0x0200150c, 596 bytes: code 0x0200150c-0x0200172b and
 * the literal pool 0x0200172c-0x0200175f.
 *
 * Conventions.  An overlay `bl` stores the target's image offset minus two, not
 * a pc-relative displacement, so the disassembler's printed targets are wrong;
 * all 46 call sites were resolved with `tools/lib/overlay_call_targets.ts`.  Except
 * for one they land in the 8-byte-per-entry import veneer table starting at
 * 0x02001a54 and are named by that entry's trailing main-image word; the
 * exception is the site at 0x02001694, which resolves to the real overlay
 * prologue at file offset 0x0964 and is therefore spelled Func_02000964.
 * resource_375 is linked at 0x02008000, so the pool words 0x02009c34 and
 * 0x02009ce0 are in-image data blocks at file offsets 0x1c34 and 0x1ce0.
 * 0x02000240 lies below the link base and is the resident table many other
 * overlays already use, declared here with the same `u16 []` spelling.
 *
 * Return type: the epilogue is `add sp,#8 / pop {r3} / mov r8,r3 /
 * pop {r5, r6} / pop {r1} / bx r1`.  The return address is popped into r1, so
 * r0 survives and IS the result -- the constant 0 loaded at 0x0200171e.  The
 * eight bytes of frame carry the fifth and sixth arguments of the six-argument
 * Func_080091c0 calls.
 *
 * Behaviour: the overlay's per-scene setup.  It fixes up two resident flags,
 * then splits on flag 0x0834.  With the flag set it builds the full scene
 * (five actor slots reset, slot 8's byte at +0x59 ORed with 8, actor 11 placed
 * and given the block at 0x02009c34, its halfword at +32 set to 12) and
 * registers six Func_080091c0 entries that differ only in their last two
 * arguments.  With the flag clear it runs the reduced variant, optionally
 * entering the sub-scene Func_02000964, and places actors 14/15/16 according to
 * flags 0x0801, 0x0808 and 0x087a.
 *
 * The guard at 0x0200151a-0x02001528 is `((u16)(Data_02000240[225] - 5)) << 16
 * <= 0x10000` compared unsigned, which selects exactly scene 5 and scene 6; it
 * is written that way below rather than as the compiler's shifted form.
 *
 * r5 carries three unrelated values across the owner (the address of
 * Data_02000240[225], a small loop constant in the first branch, and the data
 * block 0x02009ce0 in the second), so it is modelled as separate locals.  The
 * two live branches never share an r5 value.
 *
 * Uncertainties: 0x012f, 0x0242, 0x0109, 0x0834, 0x0839, 0x0801, 0x0808,
 * 0x087a, 0x0111 and 0x01090000 are identifiers or packed parameters whose
 * tables are not reconstructed, so they stay numeric.  The bit ORed into slot
 * 8's byte at +0x59 is a flag whose meaning is not established.
 */

/* 0x02000240 is below the 0x02008000 link base: a resident table, not overlay
 * data.  Entry 225 is read here as u16 once and as s16 once. */
extern u16 Data_02000240[];

/* Imports, old-style: arities are established per call site.  The queried
 * imports are typed for their return values. */
void Func_02002fa2();
s32 Func_02002f98();
void Func_02002fb2();
s32 Func_02002fa8();
void Func_020030ea();
void Func_020030f6();
void Func_02002fda();
void Func_02003044();
void Func_0200304e();
void Func_02003058();
void Func_02003062();
void Func_0200306c();
u8 *Func_0200302a();
void Func_02003088();
void Func_02003082();
void Func_020030a2();
u8 *Func_02003058_b();
void Func_02003074();
s32 Func_02003032();
void Func_020030c0();
void Func_02003050();
void Func_02003078();
void Func_0200308a();
void Func_0200309e();
void Func_020030ae();
void Func_02003142();
void Func_0200314c();
void Func_02003156();
void Func_02003160();
void Func_02003104();
s32 Func_020030ea_b();
void Func_02001ffa(void);
s32 Func_02003100();
void Func_0200318e();
void Func_02003198();
void Func_020031a2();
s32 Func_0200312a();
void Func_020031bc();
void Func_020031ca();
void Func_02003218();
void Func_02003224();
s32 Func_0200316a();
void Func_020031fc();
void Func_020031a8();

                     

                     

                     

                     
                     
/* The overlay's own scene entry, converted separately. */

s32 Func_0200150c(void)
{
    u16 *scene;
    u8 *actor;

    if ((u16)(*scene - 5) <= 1) {
        Func_02002fa2(0x12f);
    }
    if (Func_02002f98(0x109) != 0) {
        Func_02002fb2(0x242);
    }
    scene = &Data_02000240[225];

    if (Func_02002fa8(0x834) != 0) {
        Func_020030ea();
        Func_020030f6();
        Func_02002fda();
        Func_02003044(12, 0, 0);
        Func_0200304e(13, 0, 0);
        Func_02003058(14, 0, 0);
        Func_02003062(15, 0, 0);
        Func_0200306c(5, 0, 0);

        actor = Func_0200302a(8);
        *(u8 *)(actor + 0x59) = (u8)(*(u8 *)(actor + 0x59) | 8);

        Func_02003088(11, 0x00530000, 0x01090000);
        Func_02003082(11, 0x53, 0x111);
        Func_020030a2(11, 5);

        *(u16 *)(actor + 32) = 12;
        actor = Func_02003058_b(11);

        Func_02003074(11, (void *)0x02009c34);
        if (Func_02003032(0x839) != 0) {
            Func_020030c0(11, 0, 0);
        }
        Func_0200306c();

        Func_02003050(9, 24, 1, 1, 14, 21);
        Func_02003062(9, 24, 1, 1, 15, 21);
        Func_02003078(9, 24, 1, 1, 23, 19);
        Func_0200308a(9, 24, 1, 1, 24, 19);
        Func_0200309e(9, 24, 1, 1, 23, 20);
        Func_020030ae(9, 24, 1, 1, 24, 20);
        return 0;
    }

    Func_02003142(9, 0, 0);
    Func_0200314c(10, 0, 0);
    Func_02003156(1, 0, 0);
    Func_02003160(11, 0, 0);
    Func_02003104();

    if (Func_020030ea_b(0x109) == 0 && *(s16 *)scene == 10) {
        Func_02001ffa();
    }

    if (Func_02003100(0x801) != 0) {
        Func_0200318e(13, 0, 0);
        Func_02003198(14, 0, 0);
        Func_020031a2(15, 0, 0);
    } else if (Func_0200312a(0x808) != 0) {
        Func_020031bc(14, 0x01880000, 0x01780000);
        Func_020031ca(15, 0x01780000, 0x01780000);
        Func_02003218(14, 0x10000, (void *)0x02009ce0);
        Func_02003224(15, 0x10000, (void *)0x02009ce0);
    }

    if (Func_0200316a(0x87a) != 0) {
        Func_020031fc(16, 0x00840000, 0x01080000);
    }

    return 0;
    Func_020031a8();
}

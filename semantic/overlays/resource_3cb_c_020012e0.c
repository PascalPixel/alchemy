typedef unsigned char u8;
typedef signed char s8;
typedef signed short s16;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed int s32;

/*
 * Resource 3cb post-session outcome dispatcher at 0x020012e0 (1060 bytes,
 * 0x020012e0-0x02001703).
 *
 * Complete owner: `push {r5, r6, r7, lr} / sub sp, #8` prologue at 0x020012e0
 * (the `sub sp` is delayed to 0x020012fc) and the matching `add sp, #8 /
 * pop {r5, r6, r7} / pop {r1} / bx r1` interworking return at 0x020016ba.
 * The popped return address goes to r1, so r0 survives and IS the result;
 * `movs r0, #0` at 0x020016b8 is on the single exit path, so the owner always
 * returns 0.
 *
 * TWO literal pools sit INSIDE the span, both branched over, both data:
 *   0x02001368-0x0200138b, hopped by the `b.n 0x0200138c` at 0x02001366 and by
 *     the `beq.n 0x0200138c` at 0x0200135e;
 *   0x020016c4-0x02001703, after the epilogue.
 * Register state is carried straight across the first one: r6 is the loop
 * counter set at 0x02001348 and incremented at 0x0200138c on the far side, and
 * r5 (the flag id) is built at 0x0200134a from it.  Nothing in the pool is
 * decoded as an instruction here — in particular the `strh`/`ldr`/`lsls` the
 * disassembler prints at 0x02001368-0x02001388 are the four tag halfwords and
 * the four pool addresses.
 *
 * All 87 call sites were resolved with tools/overlay_call_targets.ts
 * (true_target_offset = stored_displacement + 2).  The inventory reports
 * calls=84, a lower bound.  Placed multiset: Func_02000128 x12,
 * Func_080770d0 x7, Func_080770e8 x6, Func_0808a170 x6, Func_0808a018 x5,
 * Func_0808a178 x5, Func_0808a360 x5, Func_0808a370 x5, Func_080770c8 x4,
 * Func_080770c0 x3, Func_0808a148 x3, Func_080000c0 x2, Func_080770e0 x2,
 * Func_0808a020 x2, Func_08015120 x2, Func_020011bc x2, Func_02001228 x2,
 * Func_02001294 x2, and one each of Func_0200003c, Func_02000f30,
 * Func_080000d0, Func_08000300, Func_08000378, Func_080003b8, Func_08009190,
 * Func_080091c0, Func_08077260, Func_0808a070, Func_0808a180 and
 * Func_080b5108.  Func_02000128 is reported `unknown` at all twelve sites only
 * because it has no `push` opening; it is a prologue-less leaf of this overlay
 * ending in `bx lr`, and every site is an ordinary call.
 *
 * One site is SHARED between two arms with different arguments: 0x0200152c
 * calls Func_02000128 with r0 = 0 on the fall-through from the mode-9 arm and
 * with r0 = 4 when the mode-10 arm branches to it from 0x0200158e.  It is
 * spelled as the `installTable` join below rather than duplicated, so the
 * multiset stays exact.  The same is true of the Func_0808a020 site at
 * 0x02001530, which five arms reach.
 *
 * Link base: resource_3cb is linked at 0x02008000, and this owner is another
 * witness — the pool word 0x02008149 at 0x02001700 is Func_02000148 (the
 * overlay's per-frame state machine) plus the Thumb bit, used as the argument
 * of Func_080000d0 and Func_080003b8.  Under that base 0x02009f50 and
 * 0x02009f4c are IN-IMAGE counters at file offsets 0x1f50/0x1f4c (the ones
 * Func_020002d8 and Func_02000148 advance), while 0x02000240, 0x02002224,
 * 0x03001ebc and 0x03001d08 are below the image and are ordinary RAM.
 *
 * The flag family is Func_080770c0 = test, Func_080770c8 = set,
 * Func_080770d0 = clear, Func_080770e0 = read value, Func_080770e8 = write
 * value.  The first three are forced by Func_020002d8's one-shot timer; the
 * value pair is forced here, since 0x020013d0 reads slot 1016 into a variable
 * and 0x02001442 writes a computed value back to the same slot.
 *
 * Behaviour: reset both in-image counters, park the workspace word at +448 at
 * 513, restart audio, redraw the score from the RAM record's halfword at +688,
 * open an 11x11 window, install table 4, tag 0x02002224+8 with "TALK", and
 * rebuild the eight per-member flags at 752..759 from Func_02000f30.  Then
 * dispatch on the record's signed mode halfword at +450:
 *  - 8: a completed exchange.  Bump the counters at +676 and +682, read the
 *    streak byte at slot 1016 sign-extended, form a line index 2*streak+2
 *    capped at 14, and either reset slot 1000 (bumping streak and index) or
 *    advance it.  Offer cue 0x293e+index and ask Func_0808a070: a zero answer
 *    caps the streak at 90, stores it and stops; a nonzero answer ends the
 *    session — clear 0x173, park slot 1016 at -1, release the sound handle at
 *    +682, and report either a new best (cue 0x293c, record it at +680, play
 *    Func_02001228) or no improvement (cue 0x2939).
 *  - 9: the same ending without the offer: bump +678, release the handle,
 *    report best/no-best, zero +682, clear 0x173, park slot 1016 at -1.
 *  - 10: with slot 1000 non-zero this is the abort — clear it, park the
 *    workspace phase at 2, clear flag 772, wait 20, run the interrupt-masked
 *    Func_0200003c probe and reinstall.  Otherwise bump +684 and +690, keep
 *    +688 as the running maximum, redraw it, play Func_020011bc and set flags
 *    772 and 0x305.
 *  - 11: bump +686, zero +690 and play Func_020011bc unless flag 0x173 is up;
 *    set 772, clear 0x305.
 *  - anything else: stop the transfer (Func_08000300), clear flag 370, park
 *    slot 1016 at -1, and if the record byte at +554 is set, play the
 *    farewell cue 0x2929 before clearing that byte and the IWRAM byte at
 *    0x03001d08.
 * Every arm converges on reinstalling Func_02000148 as the per-frame task and,
 * unless mode 8 left flag 0x173 up, on Func_08077260(1) and Func_080b5108.
 *
 * Uncertainties: the meanings of the individual record fields (+450, +500,
 * +554, +676, +678, +680, +682, +684, +686, +688, +690), of the workspace word
 * at +448 and its value 513, and of the flag/slot ids are not established
 * beyond their read/write pairings.  The +680 comparisons are unsigned
 * (`bcs`), the mode load is a signed halfword (`ldrsh`), and the slot-1016
 * read is sign-extended from a byte — all transcribed as such.  The four tag
 * halfwords 0x54/0x41/0x4c/0x4b spell "TALK" in ASCII, the counterpart of the
 * "EXEC" tag Func_02000860 writes at 0x02002224+0; that reading is suggestive,
 * not proven.
 */

extern u8 Data_02000240[];
extern u32 Data_02009f4c[];
extern u32 Data_02009f50[];

/* Old-style declarations: arities vary per site across this overlay. */
void Func_0200003c();
s32 Func_02000128();
s32 Func_02000f30();
void Func_020011bc();
void Func_02001228();
void Func_02001294();
void Func_080000c0();
void Func_080000d0();
void Func_08000300();
void Func_08000378();
void Func_080003b8();
void Func_08009190();
void Func_080091c0();
void Func_08015120();
s32 Func_080770c0();
void Func_080770c8();
void Func_080770d0();
s32 Func_080770e0();
void Func_080770e8();
void Func_08077260();
void Func_0808a018();
void Func_0808a020();
s32 Func_0808a070();
void Func_0808a148();
void Func_0808a170();
void Func_0808a178();
void Func_0808a180();
void Func_0808a360();
void Func_0808a370();
void Func_080b5108();

/* Used for their return values. */

s32 Func_020012e0(void)
{
    u8 *record = Data_02000240;
    u16 *workspace = *(u16 **)0x03001ebc;
    u16 *tag = (u16 *)0x02002224;
    s32 installTable;
    s32 member;
    s32 mode;
    s32 streak;
    s32 line;
    s32 slot;
    u16 handle;

    Data_02009f50[0] = 0;
    Data_02009f4c[0] = 0;
    *(u32 *)((u32)workspace + 448) = 513;

    Func_08000378(2);
    Func_02001294(*(u16 *)&record[688]);
    Func_080091c0(11, 11, 1, 1, 13, 10);
    Func_02000128(4);
    Func_080000c0(1);
    Func_08009190(5);

    tag[4] = 0x54;
    tag[5] = 0x41;
    tag[6] = 0x4c;
    tag[7] = 0x4b;

    for (member = 0; member <= 7; member++) {
        Func_080770d0(member + 752);
        if (Func_02000f30(member) != 0) {
            Func_080770c8(member + 752);
        }
    }

    mode = *(s16 *)&record[450];

    if (mode == 8) {
        Func_0808a018();
        Func_0808a360();
        Func_0808a370();
        Func_02000128(5);

        *(u16 *)&record[676] = (u16)(*(u16 *)&record[676] + 1);
        *(u16 *)&record[682] = (u16)(*(u16 *)&record[682] + 1);

        streak = (s32)(s8)Func_080770e0(1016);
        line = streak * 2 + 2;
        if (line > 14) {
            line = 14;
        }

        slot = Func_080770e0(1000);
        if (slot == 2) {
            Func_080770e8(1000, 0);
            streak++;
            line++;
        } else {
            Func_080770e8(1000, slot + 1);
        }

        Func_0808a148(8, *(u32 *)&record[500], 0);
        Func_0808a170(0x293e + line);
        Func_0808a178(8, 0);

        if (Func_0808a070(0, 0) == 0) {
            if (streak > 90) {
                streak = 90;
            }
            Func_080770e8(1016, streak);
            goto emitEnd;
        }

        Func_080770d0(0x173);
        Func_080770e8(1016, -1);

        handle = *(u16 *)&record[682];
        Func_08015120((s32)handle, 5);

        handle = *(u16 *)&record[682];
        if (*(u16 *)&record[680] < handle) {
            *(u16 *)&record[680] = handle;
            Func_0808a170(0x293c);
            Func_0808a178(8, 0);
            Func_02001228();
        } else {
            Func_0808a170(0x2939);
            Func_0808a178(8, 0);
        }

        Func_02000128(0);
        goto emitEnd;
    }

    if (mode == 9) {
        *(u16 *)&record[678] = (u16)(*(u16 *)&record[678] + 1);

        Func_0808a018();
        Func_0808a360();
        Func_0808a370();
        Func_02000128(5);
        Func_0808a148(8, *(u32 *)&record[500], 0);

        handle = *(u16 *)&record[682];
        Func_08015120((s32)handle, 5);

        handle = *(u16 *)&record[682];
        if (*(u16 *)&record[680] < handle) {
            *(u16 *)&record[680] = handle;
            Func_0808a170(0x293c);
            Func_0808a178(8, 0);
            Func_02001228();
        } else {
            Func_0808a170(0x293a);
            Func_0808a178(8, 0);
        }

        *(u16 *)&record[682] = 0;
        Func_080770d0(0x173);
        Func_080770e8(1016, -1);
        installTable = 0;
        goto finishInstall;
    }

    if (mode == 10) {
        Func_02000128();
        Func_0808a020();
        Func_0808a018();
        Func_0808a360(0);
        Func_0808a370(4);

        if (Func_02000128(1000) != 0) {
            u16 *phase = *(u16 **)0x03001ebc;

            Func_02000128(1000);
            *(u16 *)((u32)phase + 386) = 2;
            Func_080770c0(772);
            Func_080770d0(20);
            Func_080770d0();
            Func_080000c0(0);
            installTable = 4;
            goto finishInstall;
        }

        *(u16 *)&record[684] = (u16)(*(u16 *)&record[684] + 1);
        {
            u16 runs = (u16)(*(u16 *)&record[690] + 1);

            *(u16 *)&record[690] = runs;
            if (*(u16 *)&record[688] < runs) {
                *(u16 *)&record[688] = runs;
            }
        }
        Func_0200003c(*(u16 *)&record[688]);
        Func_02000128();
        Func_02001294(772);
        Func_020011bc(0x305);
        goto emitEnd;
    }

    if (mode == 11) {
        Func_080770c8();
        Func_080770c8();
        Func_0808a018();
        Func_0808a360(0);
        Func_0808a370(4);

        if (Func_02000128(0x173) == 0) {
            *(u16 *)&record[686] = (u16)(*(u16 *)&record[686] + 1);
            *(u16 *)&record[690] = 0;
            Func_02000128();
        }

        Func_080770c0(772);
        Func_020011bc(0x305);
        goto emitEnd;
    }

    Func_080770c8();
    Func_080770d0(370);
    Func_08000300(1016, -1);

    if (record[554] != 0) {
        Func_080770d0();
        Func_080770e8();
        Func_0808a018();
        Func_0808a360(8, *(u32 *)&record[500], 0);
        Func_0808a370(0x2929);
        Func_0808a148(8, 0);
        Func_0808a170();
    }

    record[554] = 0;
    *(u8 *)0x03001d08 = 0;
    Func_0808a180(0);
    Func_0808a020(4);
    goto tail;

finishInstall:
    Func_02000128(installTable);

emitEnd:
    Func_02000128();

tail:
    Func_080000d0((void *)0x02008149, 3200);
    Func_080003b8((void *)0x02008149, 1);

    if (*(s16 *)&record[450] == 8 && Func_080770c0(0x173) != 0) {
        return 0;
    }

    Func_08077260(1);
    Func_080b5108();
    return 0;
}

/*
 * resource_3c4 @ 0x0200259c, 2,636 bytes (0x0200259c-0x02002fe7).
 *
 * The overlay's chapter director: one owner that stages every actor, prop and
 * camera cue for the scene named by the pair of signed halfwords at
 * Data_02000240[224] (the chapter) and [225] (the beat).  It is reached once
 * per beat, does its staging, and returns 0.
 *
 * POOL MAP, derived by a control-flow walk from the prologue (branch targets
 * only; both `mov pc,r3` tables were read and their entries seeded).  1,100
 * bytes are reached without them, 2,408 with; the five gaps are:
 *
 *   0x02002602..0x02002604   2   alignment halfword before the table
 *   0x02002604..0x02002638  52   jump table A, 13 entries
 *   0x02002970..0x020029a8  56   literal pool (14 words)
 *   0x02002a48..0x02002a90  72   jump table B, 18 entries
 *   0x02002b8a..0x02002b8c   2   alignment halfword before the pool
 *   0x02002b8c..0x02002bac  32   literal pool (8 words)
 *   0x02002c8c..0x02002c98  12   literal pool (3 words)
 *
 * 228 pool bytes, 2,408 executable.  Every one of those gaps is confirmed
 * twice over: each word is the target of an `ldr rN,[pc,#k]` reached by the
 * walk, and each table's base is the pool word its `mov pc,r3` dispatch loads.
 * The 12-byte gap at 0x02002c8c is the interesting one — it ends *mid-row*,
 * the `b.n` at 0x02002c8a jumping over exactly three words to resume at
 * 0x02002c98 with an argument register (r1 = 0) that was set at 0x02002c88
 * and is consumed by the call at 0x02002c98, so the gap cannot be a body
 * boundary.
 *
 * LINK BASE 0x02008000, four witnesses inside this owner alone: the two jump
 * table bases 0x0200a604 and 0x0200aa48 equal the tables' own file offsets
 * 0x2604/0x2a48; the callback words 0x02008ec9 and 0x02008e21 are
 * Func_02000ec8 and Func_02000e20 plus the Thumb bit; and 0x02009a99 is
 * Func_02001a98 plus the Thumb bit, whose byte-exact source is banked in
 * assets/code.  The table entries are even, as `mov pc,rN` does not interwork.
 *
 * CALLS: 211 `bl` sites on the walk, of which five (0x020025f6, 0x0200269e,
 * 0x0200270a, 0x02002776, 0x0200278e) resolve to this owner's own epilogue at
 * 0x02002fda and are long unconditional branches, not calls.  206 real calls
 * remain, resolved with `tools/overlay_call_targets.ts` (`bl` stores
 * target_offset - 2); the printed annotations in any disassembly of this
 * region are artefacts.
 *
 * Shared tails are spelled with a label and a `goto`, never duplicated per
 * arm: 0x020026e2 is entered from both halves of the first beat with only its
 * four register arguments differing, 0x02002a1e is entered from three beats
 * with only the actor id differing, and 0x02002daa is entered both from the
 * head of its beat and by falling off the end of it.
 *
 * The epilogue is `movs r0,#0 ; add sp,#8 ; pop {r3} ; mov r8,r3 ;
 * pop {r5,r6,r7} ; pop {r1} ; bx r1` — r1, not r0, carries the return address,
 * so r0 survives and the owner returns the constant 0.
 */
typedef signed int s32;
typedef signed short s16;
typedef unsigned char u8;

typedef struct Actor_0200259c Actor_0200259c;

/* Installed at +108 and run per frame while the beat is on screen. */
typedef s32 (*Watch_0200259c)(void *actor);

struct Actor_0200259c {
    s32 pad0;                  /* +0 */
    s32 pad4;                  /* +4 */
    s32 x;                     /* +8,   12.20 fixed point */
    s32 y;                     /* +12,  12.20 fixed point */
    s32 z;                     /* +16,  12.20 fixed point */
    u8 pad20[15];              /* +20 */
    u8 flags;                  /* +35 */
    u8 pad36[49];              /* +36 */
    u8 hidden;                 /* +85 */
    u8 pad86[3];               /* +86 */
    u8 mode;                   /* +89 */
    u8 pad90[10];              /* +90 */
    s16 pose;                  /* +100 */
    u8 pad102[6];              /* +102 */
    Watch_0200259c watch;      /* +108 */
};

/* Chapter/beat script state, shared with the rest of the overlay set. */
extern s16 Data_02000240[];
/* IWRAM pointer cell; the block it names carries the beat's request word. */
extern u8 *Data_03001ebc;

/* Scene-flag query and clear, and the scene-script kick. */
s32 Func_080770c0(s32 flag);
void Func_080770c8(s32 flag);
void Func_080770d0(s32 script);

/* Placement services: four register arguments plus two stack words. */
void Func_08009180(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_080091b8(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_080091c0(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
/* Resets the actor record the first argument points at. */
void Func_080091e0(Actor_0200259c *actor, s32 mode);

/* Task installer; the first argument is a Thumb entry, not data. */
void Func_080000d0();

/* Actor services, all keyed by slot id. */
void Func_0808a010(s32 group);
Actor_0200259c *Func_0808a080(s32 slot);
void Func_0808a0f0(s32 slot, s32 x, s32 z);
void Func_0808a100(s32 slot, s32 action);
void Func_0808a158(s32 slot, s32 facing);
void Func_0808a1e0(s32 slot, s32 enable);
void Func_0808a408(s32 which);

/* Beat helpers inside this overlay. */
void Func_020023a0(void);
void Func_02002410(void);
void Func_02002480(void);
void Func_0200252c(void);
void Func_02001fdc(void);
void Func_02001abc(s32 mode);

/* Per-frame watches installed through +108 and through Func_080000d0. */
s32 Func_02000ec8(void *actor);
s32 Func_02001a98(void *actor);
void Func_02000e20(void);

s32 Func_0200259c(void)
{
    Actor_0200259c *actor;
    s32 beat;
    s32 a, b, c, d;
    s32 slot;
    s32 x, z;

    *(s32 *)(Data_03001ebc + 448) = 516;

    if (Data_02000240[224] == 0xac || Data_02000240[224] == 0xad) {
        Func_0808a408(0);
        Data_02000240[289] = 1;
        Data_02000240[288] = 0xac;
    }

    if (Data_02000240[224] != 0xac) goto other_chapter;

    beat = Data_02000240[225];
    if ((unsigned int)(beat - 1) > 12) goto done;

    switch (beat) {
    case 1:
    case 2:                                     /* 0x02002638 */
        if (Func_080770c0(0x982) != 0) {
            Func_08009180(121, 4, 74, 9, 5, 8);
            Func_08009180(18, 83, 9, 73, 3, 2);
            Func_08009180(18, 81, 9, 75, 3, 2);
            Func_08009180(18, 83, 9, 77, 3, 2);
            a = 18; b = 83; c = 9; d = 79;
            goto place_trio;
        }
        if (Func_080770c0(0x983) == 0) goto done;
        Func_08009180(121, 13, 74, 9, 5, 8);
        Func_08009180(18, 85, 11, 74, 3, 2);
        Func_08009180(18, 83, 13, 75, 3, 2);
        a = 18; b = 85; c = 11; d = 76;
    place_trio:                                 /* 0x020026e2 */
        Func_08009180(a, b, c, d, 3, 2);
        Func_08009180(18, 83, 11, 78, 3, 2);
        Func_08009180(18, 83, 13, 79, 3, 2);
        goto done;

    case 3:
    case 4:                                     /* 0x0200270e */
        Func_020023a0();
        Func_0808a080(8)->hidden = 0;
        Func_0808a080(9)->hidden = 0;
        Func_080091e0(Func_0808a080(8), 0);
        Func_080091e0(Func_0808a080(9), 0);
        Func_0808a080(8)->watch = Func_02000ec8;
        slot = 9;
        goto install_watch;

    case 5:
    case 6:
    case 7:                                     /* 0x0200274e */
        if (Func_080770c0(0x982) != 0) Func_080091b8(23, 17, 1, 2, 30, 8);
        if (Func_080770c0(0x983) == 0) goto done;
        Func_080091b8(23, 17, 1, 2, 32, 10);
        goto done;

    case 8:
    case 9:                                     /* 0x02002792 */
        Func_02002410();
        Func_0808a080(10)->hidden = 0;
        Func_0808a080(11)->hidden = 0;
        Func_080091e0(Func_0808a080(10), 0);
        Func_080091e0(Func_0808a080(11), 0);
        Func_0808a080(10)->watch = Func_02000ec8;
        slot = 11;
        goto install_watch;

    case 10:
    case 11:                                    /* 0x020027d2 */
        Func_080091e0(Func_0808a080(18), 0);
        Func_080091e0(Func_0808a080(19), 0);
        Func_0808a100(18, 2);
        Func_080091e0(Func_0808a080(20), 0);
        Func_080091e0(Func_0808a080(21), 0);
        Func_0808a158(20, 15);
        Func_0808a158(21, 15);

        if (Func_080770c0(0x971) != 0) {
            Func_08009180(59, 8, 49, 8, 1, 3);
            Func_080091c0(51, 8, 1, 1, 49, 8);
            actor = Func_0808a080(18);
            actor->flags |= 2;
            Func_0808a100(18, 3);
            Func_080091c0(45, 4, 1, 1, 46, 8);
            Func_0808a0f0(18, 0x02e80000, 0x00880000);
            Func_0808a080(18)->y = 0xfff00000;
            Func_0808a0f0(20, 0x02e80000, 0x00880000);
        }
        if (Func_080770c0(512) != 0) {
            Func_0808a158(20, 0);
            Func_0808a100(20, 5);
        }
        if (Func_080770c0(0x202) != 0) Func_0808a100(19, 2);
        if (Func_080770c0(0x972) != 0) {
            Func_08009180(59, 8, 45, 14, 1, 3);
            Func_080091c0(51, 8, 1, 1, 45, 14);
            actor = Func_0808a080(19);
            actor->flags |= 2;
            Func_0808a100(19, 3);
            Func_080091c0(45, 4, 1, 1, 48, 14);
            Func_0808a0f0(19, 0x03080000, 0x00e80000);
            Func_0808a080(19)->y = 0xfff00000;
            Func_0808a0f0(21, 0x03080000, 0x00e80000);
            Func_080770c8(0x202);
        }
        if (Func_080770c0(0x201) == 0) goto done;
        Func_0808a158(21, 0);
        Func_0808a100(21, 5);
        goto done;

    case 12:
    case 13:                                    /* 0x020029a8 */
        Func_02002480();
        Func_0808a080(12)->hidden = 0;
        Func_0808a080(13)->hidden = 0;
        Func_080091e0(Func_0808a080(15), 0);
        Func_080091e0(Func_0808a080(16), 0);
        Func_080091e0(Func_0808a080(17), 0);
        Func_080091e0(Func_0808a080(12), 0);
        Func_080091e0(Func_0808a080(13), 0);
        Func_080091e0(Func_0808a080(14), 0);
        Func_0808a080(12)->watch = Func_02000ec8;
        Func_0808a080(13)->watch = Func_02000ec8;
        slot = 14;
        goto install_watch;
    }

install_watch:                                  /* 0x02002a1e */
    Func_0808a080(slot)->watch = Func_02000ec8;
    Func_080000d0(Func_02000e20, 3200);
    goto done;

other_chapter:                                  /* 0x02002a30 */
    beat = Data_02000240[225];
    if ((unsigned int)beat > 17) goto done;

    switch (beat) {
    case 1:
    case 2:
    case 3:                                     /* 0x02002a90 */
        Data_02000240[289] = 1;
        Data_02000240[288] = 176;
        Func_080770d0(303);
        Func_0808a158(17, 6);
        Func_0808a158(18, 6);
        if (Func_080770c0(0x974) != 0) Func_0808a0f0(17, 0x02d80000, 0x01380000);
        if (Func_080770c0(0x975) != 0) Func_0808a0f0(18, 0x02e80000, 0x01380000);
        Func_0200252c();
        goto done;

    case 4:
    case 5:                                     /* 0x02002bac */
        if (Func_080770c0(0x109) != 0) goto restart_chapter;
        Func_0808a080(10)->hidden = 0;
        Func_0808a080(11)->hidden = 0;
        Func_0808a080(10)->y = 0xffd00000;
        Func_0808a080(11)->y = 0xffd00000;
        Func_0808a080(10)->flags |= 2;
        Func_0808a080(11)->flags |= 2;
        Func_0808a080(10)->mode &= 0xfe;
        Func_0808a080(11)->mode &= 0xfe;
        Func_0808a080(10)->pose = 3;
        Func_0808a080(11)->pose = 3;
        Func_0808a1e0(10, 1);
        Func_0808a1e0(11, 1);
        Func_0808a080(12)->hidden = 0;
        Func_0808a080(13)->hidden = 0;
        Func_0808a080(14)->hidden = 0;
        Func_080091e0(Func_0808a080(12), 0);
        Func_080091e0(Func_0808a080(13), 0);
        Func_080091e0(Func_0808a080(14), 0);
        Func_0808a080(12)->pose = 0;
        Func_0808a080(13)->pose = 0;
        actor = Func_0808a080(14);
        beat = Data_02000240[225];
        actor->pose = 0;
        if (beat != 5) goto done;

        Func_0808a080(10)->y = 0xffe00000;
        Func_0808a080(11)->y = 0xffc00000;
        Func_0808a080(10)->pose = 2;
        Func_0808a080(11)->pose = 4;
        Func_0808a0f0(12, 0x00c80000, 0x00980000);
        Func_0808a080(12)->pose = 11;
        Func_0808a080(12)->watch = Func_02001a98;
        Func_0808a080(12)->flags |= 2;
        Func_0808a0f0(13, 0x00c80000, 0x00980000);
        Func_0808a080(13)->pose = 12;
        Func_0808a080(13)->watch = Func_02001a98;
        Func_0808a080(13)->flags |= 2;
        Func_0808a0f0(14, 0x00880000, 0x00980000);
        Func_0808a080(14)->pose = 10;
        Func_0808a080(14)->watch = Func_02001a98;
        Func_0808a080(14)->flags |= 2;
        Func_0808a010(2);
        Func_080770c8(512);
        Func_080770c8(0x201);
        Func_080770c8(0x202);
    restart_chapter:                            /* 0x02002daa */
        Func_02001abc(0);
        goto done;

    case 6:
    case 7:                                     /* 0x02002aee */
        Func_0808a1e0(8, 1);
        Func_0808a080(8)->hidden = 0;
        Func_080091e0(Func_0808a080(8), 0);
        Func_0808a1e0(9, 1);
        Func_0808a158(9, 15);
        Func_080091e0(Func_0808a080(9), 0);
        Func_0808a080(9)->hidden = 0;
        if (Func_080770c0(516) == 0) goto done;
        Func_0808a158(9, 0);
        Func_0808a100(9, 5);
        x = Func_0808a080(9)->x;
        z = Func_0808a080(9)->z >> 20;
        Func_080091c0(26, 8, 1, 1, x >> 20, z);
        Func_0808a080(9)->watch = Func_02000ec8;
        Func_0808a080(8)->watch = Func_02000ec8;
        goto done;

    case 8:
    case 9:
    case 10:
    case 11:                                    /* 0x02002db2 */
        if (Func_080770c0(0x982) != 0) Func_080091b8(10, 30, 1, 2, 16, 30);
        if (Func_080770c0(0x983) != 0) Func_080091b8(10, 30, 1, 2, 22, 30);
        Func_080770c8(0x973);
        goto done;

    case 12:                                    /* 0x02002df6 */
        Func_080091c0(8, 49, 1, 1, 8, 113);
        Func_02001fdc();
        Func_080000d0(Func_02000e20, 3200);
        goto done;

    case 13:
    case 14:                                    /* 0x02002e1a */
        Func_0808a010(1);
        if (Func_080770c0(0x984) != 0) {
            Func_080091b8(24, 59, 1, 2, 32, 46);
            Func_0808a0f0(19, 0x01980000, 0x03180000);
            Func_0808a0f0(20, 0x01780000, 0x03180000);
            Func_0808a0f0(21, 0x01980000, 0x02f80000);
            Func_0808a0f0(22, 0x01780000, 0x02f80000);
            Func_0808a0f0(23, 0x01880000, 0x03080000);
        }
        Func_0808a080(19)->hidden &= 0xfe;
        Func_0808a080(20)->hidden &= 0xfe;
        Func_0808a080(21)->hidden &= 0xfe;
        Func_0808a080(22)->hidden &= 0xfe;
        Func_0808a080(23)->hidden &= 0xfe;
        Func_0808a158(19, 4);
        Func_0808a158(20, 1);
        Func_0808a158(21, 4);
        Func_0808a158(22, 10);
        Func_0808a158(23, 0);
        Func_0808a100(19, 2);
        Func_0808a100(23, 2);
        /* Every pin is issued against slot 20, whichever slot was measured. */
        x = Func_0808a080(19)->x;
        z = Func_0808a080(19)->z >> 20;
        Func_080091c0(20, 56, 1, 1, x >> 20, z);
        x = Func_0808a080(20)->x;
        z = Func_0808a080(20)->z >> 20;
        Func_080091c0(20, 56, 1, 1, x >> 20, z);
        x = Func_0808a080(21)->x;
        z = Func_0808a080(21)->z >> 20;
        Func_080091c0(20, 56, 1, 1, x >> 20, z);
        x = Func_0808a080(22)->x;
        z = Func_0808a080(22)->z >> 20;
        Func_080091c0(20, 56, 1, 1, x >> 20, z);
        x = Func_0808a080(23)->x;
        z = Func_0808a080(23)->z >> 20;
        Func_080091c0(20, 56, 1, 1, x >> 20, z);
        goto done;

    case 17:                                    /* 0x02002fc2 */
        Func_080091c0(49, 43, 1, 1, 49, 107);
        Func_02001fdc();
        break;

    default:
        /* Beats 0, 15 and 16 stage nothing. */
        break;
    }

done:                                           /* 0x02002fda */
    return 0;
}

# The 384-byte twin family — working draft

`tools/overlay_twins.ts` reports 32 groups of owners that are the same routine
across overlays, differing only in `bl` displacements and pool words. The largest
is **384 bytes x 11**, and no member is converted, so the first correct draft is
worth **4,224 bytes** — 384 for itself and 3,840 by transposition.

Members: `resource_373:00c4`, `389:00c4`, `392:00c4`, `393:00c4`, `39f:00c4`,
`3b2:00c4`, `3b4:00c4`, `3bf:00c4`, `3c4:00c4`, `3c5:00c4`, `3c8:0374`.

## State on 2026-07-30

The draft below is **structurally correct and not yet byte-exact**: 172 emitted
instructions, matching the reference exactly, with **22 instruction groups still
differing** — register assignment and scheduling in the first quarter, not shape.
Do not re-derive the semantics from assembly, and do not re-measure by halfwords;
start here.

Read the residual by **instruction-group equality, not halfwords** (§2): at 384
bytes every `bl` displacement is target-absolute, so one positional drift makes
every later `bl` differ. The raw count was 333/384 differing bytes, which means
almost nothing.

### What the routine does

`Func_020060b8(0)` yields `self`. The high nibble of `self`'s halfword at +6 is a
kind index into the table at `0x0200E190`; its entry supplies a masked offset
(`& 0xFFFF0000`) added to `f08` and a `<< 16` term added to `f10`, building a
three-word probe on the stack. That probe is walked through three lookups —
`Func_02000176` for the target, then `Func_020001a2` and `Func_020001ce`, each of
which aborts the routine if its hit has bit 0 of byte +89 set. A fourth call,
`Func_020060e4`, aborts on a positive result, and byte +98 of the target must be
zero. Only then does it commit: fill `0x3333` into `f30`/`f34` of both actors,
publish the probe through `Func_020060f2`/`Func_02006102`, and reset `self`'s
`f38`/`f40` to `0x80000000` and its `f08`/`f10` from the sign-extended halfwords
at +10 and +18.

### Measure it with `tools/overlay_group_diff.sh`

```sh
tools/overlay_group_diff.sh resource_373 00c4 384 <draft.c> 20
```

It compiles the draft, normalises both instruction streams and reports how many
groups differ. **Normalise properly or you will chase ghosts.** objdump renders
Thumb `mov rd,rn` as `adds rd,rn,#0`, prints `[r7, #0]` where gcc prints `[r7]`,
and gcc prints the three-operand `and r2,r2,r3` for the two-operand `ands r2,r3`.
A naive line compare called all of those differences and reported **62** on a
draft whose real residual is **22** — nearly three times over, and it hid where
the mass actually was.

### Known divergences to attack first

1. ~~The reference loads `self->f06` before stashing `self` in r8.~~ **Closed.**
   Declaring the kind index immediately after `self`, ahead of `table` and
   `mask`, drops the reload through r2 and took the residual from 164 groups to
   **22**. Declaration order is what fixes this class — not a flag.
2. **Open, and the obvious attacks are measured dead ends.** The reference
   interleaves its two pool loads around the index shift — `ldr r0,<table>`,
   `lsls r5,r3,#2`, `ldr r2,<mask>`, `ldr r1,[r0,r5]` — where gcc emits both
   loads adjacent and then the shift. Ruled out so far:
   - **No compiler mode reaches it.** `-fthumb-literal-before-index-shift` (named
     for exactly this shape), `-fthumb-low-constant-before-high-move` and
     `-fsched-high-dest-first` all leave it unchanged; `-fsched-low-dest-first`
     and `-fno-sched-depend-count` are worse. Nor does the constant's spelling:
     `~0xFFFF`, `0xFFFF0000u` and `-65536` all measure identically.
   - **Do not rewrite the lookup as `table[kind]` with an unscaled index.** It
     reads like the natural spelling and it destroys the shape: 22 groups to
     **164**, and the emitted function drops to 167 instructions because gcc
     keeps the kind in a low register and stops using `sl` at all, changing the
     prologue. Keep the pre-scaled `index` local and the
     `*(s32 *)((u8 *)table + index)` form.

   What is left to try is delaying the mask's materialisation without moving its
   declaration — it is live in `sl` across all three probe builds, so it cannot
   simply be block-scoped.
3. **All that is really left is one register swap.** Of the 22 groups, positions
   13-19 are the pool-load interleave plus the swap, 39/41/90/92 are the same
   swap propagating through the probe builds, and 143-146 are a late store
   ordering. The reference holds the mask in `sl` and the table in `r9`; we hold
   them the other way. Fix that and most of the residual goes at once.
4. **Do not swap the `table`/`mask` declaration order to chase the high
   registers.** The reference holds the mask in `sl` and the table in `r9`; we
   hold them the other way round, which looks like a one-line fix. It is not —
   declaring `mask` first sends the residual from 62 groups to **165**, because
   the table pointer then materialises before `self` is stashed and every
   subsequent lifetime shifts. Leave `table` declared first and attack the
   register identity some other way.
5. `sl` is reused: it holds `0xFFFF0000` for the first three probe builds, then
   is reloaded with the byte at +98 and carried as the zero written into `f24`
   and `f2c` of both actors. The draft models that with a `blocked` local, which
   is why those stores read `= blocked` rather than `= 0` — keep that.

### Draft

```c
typedef signed int s32;
typedef signed short s16;
typedef unsigned short u16;
typedef unsigned char u8;

struct Actor {
    s32 f00;
    s32 f04;             /* f06 is its high halfword */
    s32 f08;
    s32 f0c;
    s32 f10;
    s32 f14;
    s32 f18;
    s32 f1c;
    s32 f20;             /* f22 is a byte inside it */
    s32 f24;
    s32 f28;
    s32 f2c;
    s32 f30;
    s32 f34;
    s32 f38;
    s32 f3c;
    s32 f40;
};

extern struct Actor *Func_020060b8(s32);
extern struct Actor *Func_02000176(s32 *, struct Actor *);
extern u8 *Func_020001a2(s32 *, struct Actor *);
extern u8 *Func_020001ce(s32 *, struct Actor *);
extern s32 Func_020060e4(struct Actor *, s32 *);
extern void Func_020060ac(struct Actor *, s32);
extern void Func_0200604c(s32);
extern void Func_02006312(s32);
extern void Func_020060f2(struct Actor *, s32, s32, s32);
extern void Func_02006102(struct Actor *, s32, s32, s32);
extern void Func_02006110(struct Actor *);
extern void Func_02006334(void);
extern void Func_02006118(struct Actor *, s32);

void Func_020000c4(void)
{
    s32 probe[3];
    struct Actor *self = Func_020060b8(0);
    s32 index = (*(u16 *)((u8 *)self + 6) >> 12) * 4;
    s32 *table = (s32 *)0x0200E190;
    s32 mask = 0xFFFF0000;
    struct Actor *target;
    u8 *hit;
    s32 blocked;
    s32 fill;

    {
        s32 entry = *(s32 *)((u8 *)table + index);

        probe[0] = self->f08 + (entry & mask);
        probe[1] = self->f0c;
        probe[2] = self->f10 + (entry << 16);
    }
    target = Func_02000176(probe, self);
    if (target == 0) {
        return;
    }

    {
        s32 entry = *(s32 *)((u8 *)table + index);

        probe[0] = target->f08 + (entry & mask);
        probe[1] = target->f0c;
        probe[2] = target->f10 + (entry << 16);
    }
    hit = Func_020001a2(probe, target);
    if (hit != 0 && (hit[89] & 1) != 0) {
        return;
    }

    probe[0] = target->f08;
    probe[1] = target->f0c + 0x100000;
    probe[2] = target->f10;
    hit = Func_020001ce(probe, target);
    if (hit != 0 && (hit[89] & 1) != 0) {
        return;
    }
    *((u8 *)target + 34) = 2;

    {
        s32 entry = *(s32 *)((u8 *)table + index);

        probe[0] = target->f08 + (entry & mask);
        probe[1] = target->f0c;
        probe[2] = target->f10 + (entry << 16);
    }
    if (Func_020060e4(target, probe) > 0) {
        return;
    }
    blocked = *((u8 *)target + 98);
    if (blocked != 0) {
        return;
    }

    Func_020060ac(self, 8);
    fill = 0x3333;
    Func_0200604c(15);
    Func_02006312(185);
    target->f30 = fill;
    target->f34 = fill;
    Func_020060f2(target, probe[0], probe[1], probe[2]);
    self->f30 = fill;
    self->f34 = fill;
    Func_02006102(self, probe[0], probe[1], probe[2]);
    Func_02006110(target);
    Func_02006334();
    target->f08 = probe[0];
    target->f10 = probe[2];
    target->f24 = blocked;
    target->f2c = blocked;
    self->f38 = 0x80000000;
    self->f40 = 0x80000000;
    self->f24 = blocked;
    self->f2c = blocked;
    self->f08 = *(s16 *)((u8 *)self + 10) << 16;
    self->f10 = *(s16 *)((u8 *)self + 18) << 16;
    Func_02006118(self, 1);
}
```

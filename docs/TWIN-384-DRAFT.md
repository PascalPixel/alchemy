# The 384-byte twin family — working draft

`tools/overlay_twins.ts` reports 32 groups of owners that are the same routine
across overlays, differing only in `bl` displacements and pool words. The largest
is **384 bytes x 11**, and no member is converted, so the first correct draft is
worth **4,224 bytes** — 384 for itself and 3,840 by transposition.

Members: `resource_373:00c4`, `389:00c4`, `392:00c4`, `393:00c4`, `39f:00c4`,
`3b2:00c4`, `3b4:00c4`, `3bf:00c4`, `3c4:00c4`, `3c5:00c4`, `3c8:0374`.

## State on 2026-07-30

The draft below is **structurally correct and not yet byte-exact**: 172 emitted
instructions, matching the reference exactly, with **62 instruction groups still
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

### Known divergences to attack first

1. ~~The reference loads `self->f06` before stashing `self` in r8.~~ **Closed.**
   Declaring the kind index immediately after `self`, ahead of `table` and
   `mask`, drops the reload through r2 and took the residual from 164 groups to
   **62**. Declaration order is what fixes this class — not a flag.
2. The reference loads its two pool words apart (`0x0200E190` at position 12,
   `0xFFFF0000` at 14, with `lsl r5` between); gcc emits both together.
3. **Do not swap the `table`/`mask` declaration order to chase the high
   registers.** The reference holds the mask in `sl` and the table in `r9`; we
   hold them the other way round, which looks like a one-line fix. It is not —
   declaring `mask` first sends the residual from 62 groups to **165**, because
   the table pointer then materialises before `self` is stashed and every
   subsequent lifetime shifts. Leave `table` declared first and attack the
   register identity some other way.
4. `sl` is reused: it holds `0xFFFF0000` for the first three probe builds, then
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

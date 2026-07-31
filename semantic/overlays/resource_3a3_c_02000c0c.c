typedef signed int s32;
typedef unsigned char u8;

/*
 * Resource 3a3 actor-state initializer at 0x02000c0c (56 bytes,
 * 0x02000c0c .. 0x02000c43, of which the single word at 0x02000c40 is the
 * literal pool).
 *
 * Complete owner: `push {r5, lr}` at 0x02000c0c and the interworking return
 * `pop {r5} / pop {r0} / bx r0` at 0x02000c3a.  The popped register is r0, so
 * the owner returns nothing.
 *
 * Both call sites are placed (the inventory row reports calls=2):
 * Func_0808a160 x1, Func_080091e0 x1.  Neither return value is used.
 *
 * The record layout is read straight off the instruction offsets; the field
 * names below are descriptive only.  +0x50 holds a pointer to a second record
 * whose byte at +9 carries a two-bit mode field (masked with 0xf3, then bit 2
 * set), and +0x18/+0x1c take the same 16.16 value 0x00004ccc (0.29999).
 */

/* Imports.  Old-style declarations are mandatory in overlay sources. */
void Func_0808a160();
void Func_080091e0();

struct Actor02000c0c {
    u8 pad00[0x18];
    s32 f18;
    s32 f1c;
    u8 pad20[0x30];
    u8 *f50;            /* pointer to the attached record */
    u8 pad54[1];
    u8 f55;
};

void Func_02000c0c(struct Actor02000c0c *actor)
{
    u8 *attached;

    actor->f55 = 0;

    attached = actor->f50;
    /* `movs r3,#0 / subs r3,#13` builds the mask 0xfffffff3; only the low byte
     * is stored back, so this clears bits 2-3 and then sets bit 2. */
    attached[9] = (u8)((attached[9] & 0xf3) | 0x04);

    /* r0 still holds the incoming pointer here; it is never rewritten. */
    Func_0808a160(actor, 3);
    Func_080091e0(actor, 0);

    actor->f18 = 0x4ccc;
    actor->f1c = 0x4ccc;
}

typedef signed int s32;
typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;

/*
 * Resource 3c8 at 0x0200247c: the puzzle-statue step - walk the four statue
 * slots (10..13) and take the first of three actions that applies to one of
 * them: report it in place, drop it, or swap it with a free slot and run the
 * "settled" sequence.
 *
 * The owner is complete: `push {r5, r6, r7, lr}` plus the high-register saves
 * `mov r7, fp ; mov r6, sl ; mov r5, r9 ; push {r5, r6, r7}` and
 * `mov r7, r8 ; push {r7}` at 0x0200247c, a 132-byte frame, and the matching
 * unwind at 0x020026ce..0x020026de, followed by a one-word literal pool at
 * 0x020026e0-0x020026e3.  No live frame or register state escapes the row;
 * the next prologue is at 0x020026e4.
 *
 * `pop {r0} ; bx r0` means r0 holds the popped return address, so the owner
 * returns nothing.  Every one of the three actions ends by branching to the
 * shared 0x02007568 tail at 0x020026ca, which is also where the loop falls
 * out; the actions therefore *break* the slot loop rather than continuing it.
 *
 * The two frame slots at [sp,#12] and [sp,#16] are cleared before the loop
 * and only written in the second action, so the handles handed to 0x0200735a
 * and 0x02007360 are 0 when the height test did not fire.  That is what the
 * code does; whether the callee treats 0 as "no handle" is not decidable
 * here.
 *
 * Field offsets are this package's actor record: 0x08/0x0c/0x10 the 16.16
 * x/y/z triple, 0x14, 0x28, 0x3c, 0x44, 0x23 `flags23`, 0x55 `mode55`, 0x59
 * `state59`, 0x50 the sprite (whose byte at +9 carries the two-bit field the
 * spawner at 0x02000118 writes), 0x64 a halfword counter, 0x6c the callback
 * slot.  0x80000000 is `movs r3, #128 ; lsls r3, r3, #24`.
 *
 * `Func_02007540(target)->callback = 0x0200a2a5` installs Func_020022a4: at
 * this overlay's proven 0x02008000 link base, 0x0200a2a5 - 0x8000 - 1 =
 * 0x020022a4, this reconstruction's `s32`-returning per-frame callback that parks an
 * actor one unit above the object its kind word names.
 *
 * `Func_02004966` is called two bytes before the real prologue at
 * 0x02004968; 0x02004966 holds the alignment `movs r0, r0` that precedes it.
 * That is the ordinary "call the function two bytes later" shape, not a
 * hidden-context entry - the same routine 0x02004a2c installs by its
 * Thumb-tagged address 0x0200c969.
 *
 * IMPORT IDENTITIES INSIDE THE OWN BODY.  0x0200259e and 0x020025ae, reached
 * from 0x02002544 and 0x02002554, disassemble to the middle of this very
 * routine (a `str r3, [r6, #16]` and a `b`).  As everywhere in this package,
 * an overlay `bl`'s encoded address is a stable identity for the import it
 * reaches after load-time fixup, not a place to disassemble; see the note in
 * resource_3c8_c_020002f0.c and the proof in
 * semantic/overlays/resource_3c8_c_02002f30.c.  Neither names a local
 * continuation owned by this C file.
 *
 * Thirty-four `bl` sites reach 33 distinct targets - 0x020075c2 is called
 * twice, once with no argument register written and once with four, which is
 * why every import is declared old-style.  The full list: 0x02007328,
 * 0x02007354, 0x0200733e, 0x02002e22, 0x02007358, 0x0200733a, 0x02007394,
 * 0x02007490, 0x0200259e, 0x020025ae, 0x02002eaa, 0x0200745c, 0x0200735a,
 * 0x02007360, 0x020073f6, 0x02007430, 0x02007436, 0x02007460, 0x020074ac,
 * 0x0200759e, 0x020075c2, 0x020075ce, 0x02004a1c, 0x020074fa, 0x02007508,
 * 0x02007516, 0x02007524, 0x02007532, 0x02007540, 0x02004966, 0x02007550,
 * 0x02007534, 0x02007568.
 */

struct Sprite_0200247c {
    u8 unknown_00[9];
    u8 flags9;                  /* 0x09 */
};

struct Actor_0200247c {
    u8 unknown_00[8];
    s32 x;                      /* 0x08 */
    s32 y;                      /* 0x0c */
    s32 z;                      /* 0x10 */
    s32 state14;                /* 0x14 */
    u8 unknown_18[0x0b];
    u8 flags23;                 /* 0x23 */
    u8 unknown_24[4];
    s32 state28;                /* 0x28 */
    u8 unknown_2c[0x10];
    s32 limit3c;                /* 0x3c */
    u8 unknown_40[4];
    s32 velocity44;             /* 0x44 */
    u8 unknown_48[8];
    struct Sprite_0200247c *sprite;  /* 0x50 */
    u8 unknown_54[1];
    u8 mode55;                  /* 0x55 */
    u8 unknown_56[3];
    u8 state59;                 /* 0x59 */
    u8 unknown_5a[0x0a];
    u16 word64;                 /* 0x64 = 100 */
    u8 unknown_66[6];
    s32 callback;               /* 0x6c */
};

/* Used for their return values. */
struct Actor_0200247c *Func_02007354();
struct Actor_0200247c *Func_02007430();
struct Actor_0200247c *Func_020074ac();
struct Actor_0200247c *Func_020074fa();
struct Actor_0200247c *Func_02007508();
struct Actor_0200247c *Func_02007516();
struct Actor_0200247c *Func_02007524();
struct Actor_0200247c *Func_02007532();
struct Actor_0200247c *Func_02007540();
struct Actor_0200247c *Func_02007550();
struct Actor_0200247c *Func_020075c2();
s32 Func_0200733e();
s32 Func_02007394();
s32 Func_02007436();
s32 Func_02007460();
s32 Func_0200259e();
s32 Func_020025ae();

/* Old-style declarations: the imports' real interfaces are not known here. */
void Func_02007328();
void Func_02002e22();
void Func_02007358();
void Func_0200733a();
void Func_02007490();
void Func_02002eaa();
void Func_0200745c();
void Func_0200735a();
void Func_02007360();
void Func_020073f6();
void Func_0200759e();
void Func_020075ce();
void Func_02004a1c();
void Func_02004966();
void Func_02007534();
void Func_02007568();

void Func_0200247c(void)
{
    struct Actor_0200247c scratch;   /* the 132-byte frame's tail, at sp+20 */
    struct Actor_0200247c *actor;
    struct Actor_0200247c *other;
    struct Actor_0200247c *object;
    struct Actor_0200247c *probe;
    s32 handleA;
    s32 handleB;
    s32 slot;
    s32 index;
    s32 free;
    s32 target;
    s32 gridX;
    s32 gridZ;

    handleA = 0;
    handleB = 0;
    Func_02007328();

    index = 0;
    slot = 10;
    do {
        actor = Func_02007354(slot);
        gridX = actor->x >> 20;
        gridZ = actor->z >> 20;

        if (gridX == 13 && gridZ == 7 && Func_0200733e(512 + index) == 0) {
            Func_02002e22(actor);
            Func_02007358(512 + index);
            actor->flags23 |= 2;
            actor->state59 = 0;
            actor->mode55 = 0;
            Func_0200733a(4, 19, 1, 1, gridX, gridZ);
            break;
        }

        if ((actor->sprite->flags9 & 12) == 12
            && Func_02007394(512 + index) == 0) {
            Func_02007490(slot, 1);
            actor->velocity44 = 0;
            if ((actor->z >> 20) <= 12) {
                handleA = Func_0200259e(actor->x, 0, 0x00e00000, 253);
                handleB = Func_020025ae(actor->x, 0, 0x00f00000, 253);
            }
            Func_02002eaa(actor);
            Func_0200745c(slot, 0, 0);
            Func_0200735a(handleA);
            Func_02007360(handleB);
            Func_020073f6(512 + index);
            break;
        }

        if ((actor->z >> 20) == 19 && Func_02007436(512 + index) == 0) {
            actor->limit3c = 0x80000000;
            actor->state14 = 0;
            actor->state28 = 0;
            actor->mode55 = 0;
            actor->word64 = 0;

            for (free = 0; free < index; free++) {
                if (Func_02007460(512 + free) == 0) {
                    break;
                }
            }
            if (free < index) {
                other = Func_02007430(free + 10);
                scratch.x = actor->x;
                scratch.y = actor->y;
                scratch.z = actor->z;
                actor->x = other->x;
                actor->y = other->y;
                actor->z = other->z;
                other->x = scratch.x;
                other->y = scratch.y;
                other->z = scratch.z;
            }

            target = free + 10;
            object = Func_020074ac(target);
            object->limit3c = 0x80000000;
            object->state14 = 0;
            object->state28 = 0;
            object->mode55 = 0;
            object->word64 = 0;

            Func_0200759e(0x00030000, 0x6000);

            /* No argument register is written at this site; the returned
             * record's mode byte is cleared straight away. */
            Func_020075c2()->mode55 = 0;

            Func_020075c2(0x00880000, 0x00080000, 0x01580000, 1);
            Func_020075ce();
            Func_02004a1c(target);

            probe = Func_020074fa(target);
            if ((probe->x >> 20) == 6) {
                Func_02007508(8)->word64++;
                Func_02007516(9)->word64--;
            } else {
                Func_02007524(8)->word64--;
                Func_02007532(9)->word64++;
            }

            Func_02007540(target)->callback = 0x0200a2a5;  /* Func_020022a4|1 */
            Func_02004966(40);
            Func_02007550(target)->flags23 |= 2;
            Func_02007534(free + 512);
            break;
        }

        index++;
        slot++;
    } while (index <= 3);

    Func_02007568();
}

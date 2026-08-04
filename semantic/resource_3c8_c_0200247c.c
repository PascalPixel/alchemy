#include "types.h"

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
 * `Func_0808a080(target)->callback = 0x0200a2a5` installs Func_020022a4: at
 * this overlay's proven 0x02008000 link base, 0x0200a2a5 - 0x8000 - 1 =
 * 0x020022a4, this reconstruction's `s32`-returning per-frame callback that parks an
 * actor one unit above the object its kind word names.
 *
 * The two choreography calls after the settling effect are distinct resident
 * services: Func_0808a228 returns the record whose mode is cleared, while
 * Func_0808a210 receives the four movement arguments.
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

struct Actor_0200247c *Func_0808a080();
struct Actor_0200247c *Func_0808a228();
s32 Func_080770c0();
s32 Func_02000058();

void Func_0808a018();
void Func_0200094c();
void Func_080770c8();
void Func_080091c0();
void Func_0808a1e0();
void Func_0808a0f0();
void Func_080090d0();
void Func_0808a208();
void Func_0808a210();
void Func_0808a218();
void Func_020023d4();
void Func_020022c8();
void Func_0808a020();

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
    Func_0808a018();

    index = 0;
    slot = 10;
    do {
        actor = Func_0808a080(slot);
        gridX = actor->x >> 20;
        gridZ = actor->z >> 20;

        if (gridX == 13 && gridZ == 7 && Func_080770c0(512 + index) == 0) {
            Func_0200094c(actor);
            Func_080770c8(512 + index);
            actor->flags23 |= 2;
            actor->state59 = 0;
            actor->mode55 = 0;
            Func_080091c0(4, 19, 1, 1, gridX, gridZ);
            break;
        }

        if ((actor->sprite->flags9 & 12) == 12
            && Func_080770c0(512 + index) == 0) {
            Func_0808a1e0(slot, 1);
            actor->velocity44 = 0;
            if ((actor->z >> 20) <= 12) {
                handleA = Func_02000058(actor->x, 0, 0x00e00000, 253);
                handleB = Func_02000058(actor->x, 0, 0x00f00000, 253);
            }
            Func_0200094c(actor);
            Func_0808a0f0(slot, 0, 0);
            Func_080090d0(handleA);
            Func_080090d0(handleB);
            Func_080770c8(512 + index);
            break;
        }

        /* The swap block is laid out before the test that reaches it in the
         * original function.  Keep that shape explicit so source order also
         * reflects the machine's address order. */
        goto check_settled_action;

    swap_with_free_slot:
        other = Func_0808a080(free + 10);
        scratch.x = actor->x;
        scratch.y = actor->y;
        scratch.z = actor->z;
        actor->x = other->x;
        actor->y = other->y;
        actor->z = other->z;
        other->x = scratch.x;
        other->y = scratch.y;
        other->z = scratch.z;
        goto finish_settled_action;

    check_settled_action:
        if ((actor->z >> 20) == 19 && Func_080770c0(512 + index) == 0) {
            actor->limit3c = 0x80000000;
            actor->state14 = 0;
            actor->state28 = 0;
            actor->mode55 = 0;
            actor->word64 = 0;

            for (free = 0; free < index; free++) {
                if (Func_080770c0(512 + free) == 0) {
                    goto swap_with_free_slot;
                }
            }

    finish_settled_action:
            target = free + 10;
            object = Func_0808a080(target);
            object->limit3c = 0x80000000;
            object->state14 = 0;
            object->state28 = 0;
            object->mode55 = 0;
            object->word64 = 0;

            Func_0808a208(0x00030000, 0x6000);

            /* No argument register is written at this site; the returned
             * record's mode byte is cleared straight away. */
            Func_0808a228()->mode55 = 0;

            Func_0808a210(0x00880000, 0x00080000, 0x01580000, 1);
            Func_0808a218();
            Func_020023d4(target);

            probe = Func_0808a080(target);
            if ((probe->x >> 20) == 6) {
                Func_0808a080(8)->word64++;
                Func_0808a080(9)->word64--;
            } else {
                Func_0808a080(8)->word64--;
                Func_0808a080(9)->word64++;
            }

            Func_0808a080(target)->callback = 0x0200a2a5;  /* Func_020022a4|1 */
            Func_020022c8(40);
            Func_0808a080(target)->flags23 |= 2;
            Func_080770c8(free + 512);
            break;
        }

        index++;
        slot++;
    } while (index <= 3);

    Func_0808a020();
}

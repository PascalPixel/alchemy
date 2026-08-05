#include "types.h"

/*
 * Resource 371 owner at 0x02002768 (116 bytes, 0 calls).
 *
 * Complete owner: `push {r5, r6, r7, lr}` at 0x02002768 and the interworking
 * return `pop {r5, r6, r7} ; pop {r0} ; bx r0` at 0x020027ba.  r0 holds the
 * popped return address, so the owner returns nothing.  The single return
 * sits inside the first loop's success arm; the second loop is exited only
 * that way.  Bytes 0x020027c6-0x020027db are alignment plus the literal pool
 * (0x0200e3f4, 0x0200a859, 0x0200db84, 0x17940000, 0x0d480000).
 *
 * The owner is a table patcher, and it is the clearest single witness in this
 * overlay for the 0x02008000 link base.  Its pool words 0x0200e3f4 and
 * 0x0200db84 are in-image data at file offsets 0x63f4 and 0x5b84, and the
 * handler word it installs, 0x0200a859, is 0x02002858 + 0x8000 + the Thumb
 * bit — i.e. the overlay's own `Func_02002858`, whose `push {lr}` prologue is
 * at that offset.  The same relation holds for the handler words already in
 * the table (0x02008031 -> Func_02000030, 0x0200808d -> Func_0200008c,
 * 0x020080d5 -> Func_020000d4, 0x02009ca5 -> Func_02001ca4), and for the
 * `Data_020081c5` that the byte-exact sibling
 * `assets/code/resource_371_c_02000250.c` stores into an object's +0x6c
 * (-> Func_020001c4).  Handler slots are therefore written here as C
 * function pointers, which carry exactly that Thumb-bit value.
 *
 * Rule table (0x0200e3f4, 12 bytes per entry, terminated by kind == -1):
 * find the entry whose kind is 1 and whose id is 0x8a, and promote it to
 * kind 2 with `Func_02002858` as its handler.  The terminator test re-reads
 * the kind word after the patch, which is why it is written as a do/while.
 *
 * Placement table (0x0200db84, 24 bytes per entry): find the entry whose id
 * is 57 and overwrite its X, Z and facing.  There is no terminator test in
 * this loop — the search is assumed to succeed, and the owner returns from
 * inside it.
 *
 * UNCERTAINTY: the original also clobbers r4 without saving it (`push {r5,
 * r6, r7, lr}` only), which is the same call-clobbered-r4 convention the
 * owner at 0x0200011c shows.  Nothing observable depends on it.
 */

struct SpawnRule_02002768 {
    s32 kind;                   /* 0x00, -1 terminates the table */
    s16 id;                     /* 0x04 */
    s16 param;                  /* 0x06 */
    void (*handler)(void);      /* 0x08, only meaningful for kind 2 */
};

struct Placement_02002768 {
    s16 id;                     /* 0x00 */
    s16 kind;                   /* 0x02 */
    s32 param;                  /* 0x04 */
    s32 x;                      /* 0x08, 16.16 */
    s32 y;                      /* 0x0c, 16.16 */
    s32 z;                      /* 0x10, 16.16 */
    u16 facing;                 /* 0x14 */
    u16 flags;                  /* 0x16 */
};

extern struct SpawnRule_02002768 Data_0200e3f4[];
extern struct Placement_02002768 Data_0200db84[];

void Func_02002858(void);

void Func_02002768(void)
{
    struct SpawnRule_02002768 *rule = Data_0200e3f4;
    struct Placement_02002768 *placement = Data_0200db84;

    do {
        if (rule->kind == 1 && rule->id == 0x8a) {
            rule->kind = 2;
            rule->handler = Func_02002858;
        }
        if (rule->kind == -1) break;
        rule++;
    } while (1);

    placement->facing = 0x3000;
    while (placement->id != 57) {
        placement++;
    }
    placement->x = 0x17940000;
    placement->z = 0x0d480000;
}

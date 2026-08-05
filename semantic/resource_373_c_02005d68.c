#include "types.h"

/*
 * Resource 373: spawn the two halves of a paired scene actor.
 *
 * Complete owner: `push {r5, r6, r7, lr}` plus the r8/r9/sl/fp saves and the
 * 8-byte local frame at 0x02005d68, through the single `add sp, #8` epilogue
 * at 0x02005e6a..0x02005e7a, followed by its two-word literal pool.  The
 * spawn loop's two `continue` edges rejoin at 0x02005e34 and the tail runs
 * once; nothing is live past the return.
 *
 * Note the interior literal pools at 0x02005dbc and 0x02005e2c: the loop
 * branches over them, so they sit inside the owner's span rather than after
 * it.  The word at 0x02005dbc is 0, which the reference keeps in r8 as the
 * loop's zero constant.
 *
 * All three calls are placed.  None is an interworking `call_via rN` site.
 *
 * UNCERTAINTY 1: the called service addresses are the ones encoded in the
 * overlay image (shared 0x02000000 namespace, load-time fixups).
 * UNCERTAINTY 2: the tail dereferences both spawn slots without re-testing
 * them, so a failed spawn would fault.  That is what the reference does; it
 * is reproduced rather than guarded.
 * UNCERTAINTY 3: 0x0200dd15 and 0x0200dcc5 are odd (Thumb) addresses stored
 * into the actors' 0x6c behaviour slots; their signatures are not
 * established here.
 */

struct Resource373Sprite {
    u8 unknown_00[5];
    u8 flags05;
    u8 unknown_06[1];
    u8 flags07;
    u16 attribute08;                /* 0x08; its high byte is also flags09 */
    u8 unknown_0a[0x12];
    u8 tileGroup;                   /* 0x1c */
    u8 flags1d;
    u8 unknown_1e[8];
    u8 field26;
    u8 unknown_27[1];
    u8 *frame;                      /* 0x28 */
};

struct Resource373Actor {
    u8 unknown_00[0x14];
    s32 field14;
    u8 unknown_18[0x0b];
    u8 flags23;
    u8 unknown_24[0x2c];
    struct Resource373Sprite *sprite;   /* 0x50 */
    u8 unknown_54[1];
    u8 field55;
    u8 unknown_56[0x0e];
    u16 field64;
    u8 unknown_66[2];
    void *owner;                        /* 0x68 */
    void (*behaviour)(void);            /* 0x6c */
};

struct Resource373Spawn {
    u8 unknown_00[8];
    s32 x;
    s32 y;
    s32 z;
    s32 field14;
};

/* Scene state block; the halfword at +0x46 selects the sprite tile group. */
#define RESOURCE_373_SCENE (*(u8 *volatile *)0x03001f30)

/* Four bytes per tile group; the halfword at +2 packs the palette field. */
#define RESOURCE_373_TILE_GROUPS ((const u16 *)0x03001b10)

/*
 * The reference reads and writes sprite offset 9 as a byte while it reads and
 * writes offset 8 as a halfword, so the two overlap by construction.
 */
#define RESOURCE_373_SPRITE_FLAGS09(sprite) \
    (((u8 *)&(sprite)->attribute08)[1])

void Func_080001b8();
void Func_08009020();
struct Resource373Actor *Func_080090c8();

void Func_02005d68(struct Resource373Spawn *spawn)
{
    s32 permuted_20;
    struct Resource373Actor *halves[2];
    u8 *scene = RESOURCE_373_SCENE;
    struct Resource373Actor *first;
    struct Resource373Actor *second;
    u32 index;

    for (index = 0; index <= 1; index++) {
        struct Resource373Actor *actor =
            Func_080090c8(26, spawn->x, spawn->y, spawn->z);
        struct Resource373Sprite *sprite;
        s32 tileGroup;
        s32 packed;

        if (actor == 0) {
            continue;
        }
        halves[index] = actor;

        actor->field14 = spawn->field14;
        actor->field55 = 0;
        actor->field64 = 0;
        sprite = actor->sprite;
        if (sprite == 0) {
            continue;
        }
        actor->owner = spawn;

        Func_08009020(sprite, 0);
        sprite->field26 = 0;
        Func_080001b8(sprite->tileGroup);

        sprite->tileGroup = (u8)tileGroup;
        tileGroup  = permuted_20;
        permuted_20 = *(const u16 *)(scene + 0x46);
        sprite->flags1d = (u8)(sprite->flags1d | 1);

        /* Bits 5..14 of the group's second halfword become bits 0..9. */
        packed = RESOURCE_373_TILE_GROUPS[sprite->tileGroup * 2 + 1];
        packed = (packed << 17) >> 22;
        sprite->attribute08 =
            (u16)((sprite->attribute08 & (s32)0xfffffc00) | packed);

        sprite->flags05 = (u8)(((sprite->flags05 & ~0x20) & 63) | 0x40);
        sprite->flags07 = (u8)((sprite->flags07 & 63) | 0x80);
        sprite->frame[22] = 0;
    }

    second = halves[1];
    first = halves[0];

    first->behaviour = (void (*)(void))0x0200dd15;
    RESOURCE_373_SPRITE_FLAGS09(first->sprite) =
        (u8)((RESOURCE_373_SPRITE_FLAGS09(first->sprite) & ~0x0c) | 4);

    second->flags23 = 2;
    second->behaviour = (void (*)(void))0x0200dcc5;
    RESOURCE_373_SPRITE_FLAGS09(second->sprite) =
        (u8)((RESOURCE_373_SPRITE_FLAGS09(second->sprite) & ~0x0c) | 4);
}

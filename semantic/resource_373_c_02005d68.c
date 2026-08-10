#include "types.h"

/* Resource 373: spawn the two halves of a paired scene actor. */

struct Resource373Sprite {
    u8 unknown_00[5];
    u8 flags05;
    u8 unknown_06[1];
    u8 flags07;
    u16 attribute08;
    u8 unknown_0a[0x12];
    u8 tileGroup;
    u8 flags1d;
    u8 unknown_1e[8];
    u8 field26;
    u8 unknown_27[1];
    u8 *frame;
};

struct Resource373Actor {
    u8 unknown_00[0x14];
    s32 field14;
    u8 unknown_18[0x0b];
    u8 flags23;
    u8 unknown_24[0x2c];
    struct Resource373Sprite *sprite;
    u8 unknown_54[1];
    u8 field55;
    u8 unknown_56[0x0e];
    u16 field64;
    u8 unknown_66[2];
    void *owner;
    void (*behaviour)(void);
};

struct Resource373Spawn {
    u8 unknown_00[8];
    s32 x;
    s32 y;
    s32 z;
    s32 field14;
};

#define RESOURCE_373_SCENE (*(u8 *volatile *)0x03001f30)
#define RESOURCE_373_TILE_GROUPS ((const u16 *)0x03001b10)
#define RESOURCE_373_SPRITE_FLAGS09(sprite) \
    (((u8 *)&(sprite)->attribute08)[1])

struct Resource373Actor *Func_0200bc96(s32 kind, s32 x, s32 y, s32 z);
void Func_0200bcb4(s32 tileGroup);
void Func_0200bcb6(struct Resource373Sprite *sprite, s32 mode);

void Func_02005d68(struct Resource373Spawn *spawn)
{
    struct Resource373Actor *halves[2];
    u8 *scene = RESOURCE_373_SCENE;
    struct Resource373Actor *first;
    struct Resource373Actor *second;
    s32 index;

    for (index = 0; index <= 1; index++) {
        struct Resource373Actor *actor =
            Func_0200bc96(26, spawn->x, spawn->y, spawn->z);
        struct Resource373Sprite *sprite;
        s32 tileGroup;
        s32 packed;

        halves[index] = actor;
        if (actor == 0) {
            continue;
        }

        actor->field14 = spawn->field14;
        sprite = actor->sprite;
        actor->field55 = 0;
        actor->field64 = 0;
        actor->owner = spawn;
        if (sprite == 0) {
            continue;
        }

        Func_0200bcb6(sprite, 0);
        sprite->field26 = 0;
        Func_0200bcb4(sprite->tileGroup);

        tileGroup = *(const u16 *)(scene + 0x46);
        sprite->tileGroup = (u8)tileGroup;
        sprite->flags1d = (u8)(sprite->flags1d | 1);
        packed = RESOURCE_373_TILE_GROUPS[sprite->tileGroup * 2 + 1];
        packed = (packed << 17) >> 22;
        sprite->attribute08 =
            (u16)((sprite->attribute08 & (s32)0xfffffc00) | packed);

        sprite->flags05 = (u8)(((sprite->flags05 & ~0x20) & 63) | 0x40);
        sprite->flags07 = (u8)((sprite->flags07 & 63) | 0x80);
        sprite->frame[22] = 0;
    }

    first = halves[0];
    second = halves[1];

    first->behaviour = (void (*)(void))0x0200dd15;
    RESOURCE_373_SPRITE_FLAGS09(first->sprite) =
        (u8)((RESOURCE_373_SPRITE_FLAGS09(first->sprite) & ~0x0c) | 4);

    second->behaviour = (void (*)(void))0x0200dcc5;
    RESOURCE_373_SPRITE_FLAGS09(second->sprite) =
        (u8)((RESOURCE_373_SPRITE_FLAGS09(second->sprite) & ~0x0c) | 4);
    second->flags23 = 2;
}

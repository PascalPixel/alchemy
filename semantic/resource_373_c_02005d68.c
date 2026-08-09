#include "types.h"

/* Resource 373: spawn and configure the two halves of a paired scene actor. */

struct Resource373Sprite {
    u8 unknown_00[5];
    u8 flags05;
    u8 unknown_06;
    u8 flags07;
    s16 attribute08;
    u8 unknown_0a[0x12];
    u8 tileGroup;
    u8 flags1d;
    u8 unknown_1e[8];
    u8 field26;
    u8 unknown_27;
    u8 *frame;
};

struct Resource373Actor {
    u8 unknown_00[0x14];
    s32 field14;
    u8 unknown_18[0x0b];
    u8 flags23;
    u8 unknown_24[0x2c];
    struct Resource373Sprite *sprite;
    u8 unknown_54;
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

struct Resource373ActorState {
    u8 unknown_54;
    u8 field55;
    u8 unknown_56[0x0e];
    u16 field64;
};

struct Resource373TileGroup {
    u16 unknown_00;
    u16 packed;
};

#define RESOURCE_373_SCENE (*(u8 *volatile *)0x03001f30)
#define RESOURCE_373_TILE_GROUPS ((const struct Resource373TileGroup *)0x03001b10)

struct Resource373Actor *Func_0200bc96();
void Func_0200bcb6();
void Func_0200bcb4();

void Func_02005d68(struct Resource373Spawn *spawn)
{
    struct Resource373Actor *halves[2];
    u8 *scene = RESOURCE_373_SCENE;
    s32 index;
    u8 mask;
    s32 zero;

    index = 0;
    for (; index <= 1; index++) {
        struct Resource373Actor *actor =
            Func_0200bc96(26, spawn->x, spawn->y, spawn->z);
        struct Resource373Sprite *sprite;
        struct Resource373ActorState *state;
        u32 packed;

        mask = 63;
        halves[index] = actor;
        if (actor == 0) {
            continue;
        }

        actor->field14 = spawn->field14;
        sprite = actor->sprite;
        state = (struct Resource373ActorState *)((u8 *)actor + 0x54);
        state->field55 = 0;
        state->field64 = 0;
        zero = 0;
        actor->owner = spawn;
        if (sprite == 0) {
            continue;
        }

        Func_0200bcb6(sprite, 0);
        sprite->field26 = (u8)zero;
        Func_0200bcb4(sprite->tileGroup);

        sprite->tileGroup = (u8)*(const u16 *)(scene + 0x46);
        sprite->flags1d = (u8)(sprite->flags1d | 1);
        packed = RESOURCE_373_TILE_GROUPS[sprite->tileGroup].packed;
        packed <<= 17;
        packed >>= 22;
        sprite->attribute08 =
            (u16)((sprite->attribute08 & (s32)0xfffffc00) | packed);
        sprite->flags05 =
            (u8)(((sprite->flags05 & ~0x20) & mask) | 0x40);
        sprite->flags07 = (u8)((sprite->flags07 & mask) | 0x80);
        sprite->frame[22] = (u8)zero;
    }

    {
        struct Resource373Actor *first = halves[0];
        struct Resource373Actor *second;
        s32 flagsMask = ~0x0c;
        u8 *firstSprite;
        u8 *secondSprite;
        u8 secondFlags;

        firstSprite = (u8 *)first->sprite;
        first->behaviour = (void (*)(void))0x0200dd15;
        firstSprite[9] =
            (u8)((firstSprite[9] & flagsMask) | 4);

        second = halves[1];
        secondSprite = (u8 *)second->sprite;
        secondFlags = (u8)((secondSprite[9] & flagsMask) | 4);
        second->behaviour = (void (*)(void))0x0200dcc5;
        secondSprite[9] = secondFlags;
        second->flags23 = 2;
    }
}

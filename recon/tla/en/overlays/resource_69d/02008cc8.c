#include "types.h"
#include "field_event.h"
#include "field_sprite.h"

#define WORD_AT(base, offset) (*(s32 *)((u8 *)(base) + (offset)))

struct FieldActor *Object_GetById(s32 id);
void Func_02001e70(s32 id, s32 x, s32 z);
s32 GameFlag_Test(s32 flag);
void Animation_ApplyChildValues(struct DispatchObject *, u32);

static inline void Place(s32 id, s32 x, s32 z)
{
    /* FAKEMATCH: as in ☀️'s scene layer, the inline parameters keep each call's constants */
    Func_02001e70(id, x, z);
}

static inline s32 FlagTest(s32 flag)
{
    /* FAKEMATCH: as in ☀️'s scene layer, the inline parameter keeps each flag's constant */
    return GameFlag_Test(flag);
}

void Func_02000cc8(void)
{
    struct FieldActor *left = Object_GetById(9);
    struct FieldActor *right = Object_GetById(10);
    struct FieldActor *door = Object_GetById(23);
    struct FieldSprite *sprite = door->sprite;

    Place(24, 512 << 16, 248 << 16);
    Place(23, 512 << 16, 248 << 16);
    Place(9, 472 << 16, 256 << 16);
    Place(10, 552 << 16, 256 << 16);
    WORD_AT(Object_GetById(24), 24) = 0;
    door->motionFlags = 0;
    door->y = door->terrainHeight;
    left->motionFlags = 0;
    left->y = left->terrainHeight;
    right->motionFlags = 0;
    right->y = right->terrainHeight;
    if (FlagTest(0xa0a)) {
        left->y += 0x66640;
        door->y += 0x1eb80;
        door->x -= 0x28f40;
        sprite->rotation += 0x800;
        Animation_ApplyChildValues((struct DispatchObject *)Object_GetById(9), 4);
        Animation_ApplyChildValues((struct DispatchObject *)Object_GetById(11), 4);
    }
    if (FlagTest(0xa0b)) {
        right->y += 0x66640;
        door->y += 0x1eb80;
        door->x += 0x28f40;
        sprite->rotation -= 0x800;
        Animation_ApplyChildValues((struct DispatchObject *)Object_GetById(10), 4);
        Animation_ApplyChildValues((struct DispatchObject *)Object_GetById(12), 4);
    }
    if (FlagTest(0xa0a) && FlagTest(0xa0b)) {
        left->y += 0x199900;
        right->y += 0x199900;
        door->y += 0x1b8480;
        left->motionFlags = 4;
        right->motionFlags = 4;
        door->motionFlags = 4;
    }
}

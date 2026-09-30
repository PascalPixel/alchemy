#include "MORI.H"

s32 UpdateOrbitingSceneObject(s32 *p)
{
    s16 *q = (s16 *)p[20];
    s32 a, b;
    s32 d = Math_Sin(p[12]) * 2;
    if (d > 0)
        d = -d;
    p[2] = p[14] + Math_Cos(p[12]) * 2;
    p[3] = p[15] + d;
    q[15] = Math_Cos(p[12] + 0x8000) / 8;
    a = Random_Next();
    b = Random_Next();
    p[12] = p[12] + ((((u32)a << 9) >> 16) + (((u32)b << 9) >> 16)) + 0x400;
    return 0;
}

void InitializeOrbitingEffect(s32 id)
{
    OrbitingSceneObject *actor;
    OrbitingSceneObjectSprite *sprite;
    u8 *transfer;
    s32 zero;

    actor = (OrbitingSceneObject *)Object_GetById(id);
    sprite = actor->sprite;
    sprite->flags_09_mode = 1;
    sprite->flags_05_bit_5 = 0;
    sprite->flags_09_high = 0;

    zero = 0;
    sprite->state = zero;
    Actor_SetSpriteFlags(actor, zero);
    actor->active = zero;
    actor->mode = zero;

    if (GameFlag_IsSet(0x109) == 0)
        actor->y += 0x200000;

    actor->flags_23 &= 0xfe;
    actor->visible = 1;

    transfer = AllocateEffectTransfer(17, 0x608);
    Item_LoadIcon(ITEM_NUT);
    transfer += 0x400;
    Vram_Load(sprite->palette, 128, transfer);
    Heap_Release(17);

    actor->orbit_center_x = actor->x;
    actor->orbit_angle = zero;
    actor->orbit_center_y = actor->y;
    actor->active = 1;
    actor->callback = (u32)UpdateOrbitingSceneObject;
    actor->state = zero;
}

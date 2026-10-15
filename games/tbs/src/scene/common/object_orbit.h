#include "orbit.h"

static s32 SceneEffect_UpdateOrbitPosition(s32 *p)
{
    s16 *q = (s16 *)p[20];
    s32  a, b;
    s32  d = Engine_MathSin(p[12]) * 2;
    if (d > 0)
        d = -d;
    p[2] = p[14] + Engine_MathCos(p[12]) * 2;
    p[3] = p[15] + d;
    q[15] = Engine_MathCos(p[12] + 0x8000) / 8;
    a = Engine_RandomNext();
    b = Engine_RandomNext();
    p[12] = p[12] + ((((u32)a << 9) >> 16) + (((u32)b << 9) >> 16)) + 0x400;
    return 0;
}

void InitializeStagedActorSceneOrbitingEffect(s32 id)
{
    OrbitingSceneObject       *actor;
    OrbitingSceneObjectSprite *sprite;
    u8                        *transfer;
    s32                        zero;

    actor = (OrbitingSceneObject *)Object_GetById(id);
    sprite = actor->sprite;
    sprite->flags09Mode = 1;
    sprite->flags05Bit5 = 0;
    sprite->flags09High = 0;

    zero = 0;
    sprite->state = zero;
    Engine_ActorSetSpriteFlags(actor, zero);
    actor->active = zero;
    actor->mode = zero;

    if (Engine_GameFlagIsSet(0x109) == 0)
        actor->y += 0x200000;

    actor->flags23 &= 0xfe;
    actor->visible = 1;

    transfer = Engine_HeapAllocate(17, 0x608);
    Engine_ItemLoadIcon(ITEM_NUT);
    transfer += 0x400;
    Engine_VramLoad(sprite->palette, 128, transfer);
    Engine_HeapRelease(17);

    actor->orbitCenterX = actor->x;
    actor->orbitAngle = zero;
    actor->orbitCenterY = actor->y;
    actor->active = 1;
    actor->callback = (u32)SceneEffect_UpdateOrbitPosition;
    actor->state = zero;
}

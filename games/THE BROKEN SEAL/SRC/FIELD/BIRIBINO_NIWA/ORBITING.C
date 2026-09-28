#include "NIWA.H"

void FieldScene_OpenGate(void)
{
    Audio_PlayCue(0xBC);
    Map_AnimateCells(Niwa_GateCells, 0x34, 0xB);
    GameFlag_Set(0x200);
}

/*
 * Walks one entity around a lobe of a sine and cosine figure and advances its
 * phase by a random step. The vertical term is forced non-positive, so the
 * path is one lobe rather than a full circle. The two trig calls take
 * different arguments and the two random draws are independent and summed:
 * neither pair is a common subexpression.
 */
s32 SceneEffect_UpdateLobeOrbitEntity(struct SceneEntity_0200090c *entity)
{
    struct SceneHandle_0200090c *handle = entity->handle;
    s32 vertical;
    s32 tilt;
    s32 step;

    vertical = Math_Sin(entity->phase) * 2;
    if (vertical > 0) vertical = -vertical;

    entity->x = entity->origin_x + Math_Cos(entity->phase) * 2;
    entity->y = entity->origin_y + vertical;

    /* A quarter turn on from the position phase. */
    tilt = Math_Cos(entity->phase + 0x8000);
    /* Bias then shift: division by 8 rounded toward zero. */
    if (tilt < 0) tilt += 7;
    handle->field1e = (s16)(tilt >> 3);

    /* The shift pair extracts a field, unsigned; it is not a scale. */
    step = (s32)(((u32)Random_Next() << 9) >> 16)
         + (s32)(((u32)Random_Next() << 9) >> 16);
    entity->phase = entity->phase + step + 1024;

    return 0;
}

void InitializeOrbitingSceneEntity(s32 id)
{
    OrbitingSceneObject *actor;
    OrbitingSceneObjectSprite *sprite;
    u8 *transfer;
    s32 zero;

    actor = (OrbitingSceneObject *)Engine_ActorGet(id);
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

    transfer = (u8 *)Engine_HeapAllocate(17, 0x608);
    Item_LoadIcon(ITEM_NUT);
    transfer += 0x400;
    Vram_Load(sprite->palette, 128, transfer);
    Heap_Release(17);

    actor->orbit_center_x = actor->x;
    actor->orbit_angle = zero;
    actor->orbit_center_y = actor->y;
    actor->active = 1;
    actor->callback = (u32)SceneEffect_UpdateLobeOrbitEntity;
    actor->state = zero;
}

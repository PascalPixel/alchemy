/*
 * The orbiting nut: its per-frame orbit, and the setup that shows the
 * item's icon on actor 11 and starts it circling.
 */

#include "TYPES.H"
#include "FIELD_EVENT.H"

#include "STAGED_ACTOR.H"

enum {
    /* Message 0x182 + 181. */
    ITEM_NUT = 181
};

typedef struct { s32 lo, hi; } Pair;

typedef struct { s32 w0, w1, w2, w3; Pair tail; } Query;

struct Particle_02000c4c {
    u8 unknown_00[8];
    s32 x;                  /* +0x08 */
    s32 y;                  /* +0x0c */
    u8 unknown_10[0x20];
    s32 angle;              /* +0x30, 0x10000 to the turn */
    u8 unknown_34[4];
    s32 base_x;             /* +0x38 */
    s32 base_y;             /* +0x3c */
    u8 unknown_40[0x10];
    u16 *sprite;            /* +0x50 */
};

typedef struct OrbitingSceneObjectSprite {
    u8 padding_00[5];
    u8 flags_05_low : 5;
    u8 flags_05_bit_5 : 1;
    u8 flags_05_high : 2;
    u8 padding_06[3];
    u8 flags_09_low : 2;
    u8 flags_09_mode : 2;
    u8 flags_09_high : 4;
    u8 padding_0a[18];
    u8 pal;
    u8 padding_1d[10];
    u8 state;
} OrbitingSceneObjectSprite;

typedef struct OrbitingSceneObject {
    u8 padding_00[8];
    s32 x;
    s32 y;
    u8 padding_10[19];
    u8 flags_23;
    u8 padding_24[12];
    s32 orbit_angle;
    u8 padding_34[4];
    s32 orbit_center_x;
    s32 orbit_center_y;
    u8 padding_40[16];
    OrbitingSceneObjectSprite *sprite;
    u8 padding_54;
    u8 mode;
    u8 state;
    u8 padding_57[5];
    u8 active;
    u8 padding_5d[4];
    u8 visible;
    u8 padding_62[10];
    u32 callback;
} OrbitingSceneObject;

/* The scene's tables, laid out after the code. */
extern u8 KorimaHiroba_Scripts[];
extern u8 KorimaHiroba_Messages[];
extern u8 KorimaHiroba_Actors[];
extern u8 KorimaHiroba_Extras[];


/* Constant getter; the owner includes its own pool word. */

s32 StagedActor_FindClearPosition(struct StagedActorProbe *probe);

s32 SceneEffect_UpdateOrbitingParticle(struct Particle_02000c4c *record)
{
    u16 *sprite = record->sprite;
    s32 lift;
    s32 tilt;
    s32 jitter;

    lift = Math_Sin(record->angle) * 2;
    if (lift > 0)
        lift = -lift;

    record->x = record->base_x + Math_Cos(record->angle) * 2;
    record->y = record->base_y + lift;

    /* Signed divide by 8, spelled `if (v < 0) v += 7; v >>= 3`. */
    tilt = Math_Cos(record->angle + 0x8000);
    if (tilt < 0)
        tilt += 7;
    sprite[15] = (u16)(tilt >> 3);          /* +0x1e */

    jitter = (s32)(((u32)Random_Next() << 9) >> 16);
    jitter += (s32)(((u32)Random_Next() << 9) >> 16);
    record->angle += jitter + 1024;

    return 0;
}

void SceneEffect_InitOrbitingParticle(s32 id)
{
    OrbitingSceneObject *actor;
    OrbitingSceneObjectSprite *sprite;
    u8 *transfer;
    s32 zero;

    actor = (void *)Object_GetById(id);
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

    transfer = Heap_Allocate(17, 0x608);
    Item_LoadIcon(ITEM_NUT);
    transfer += 0x400;
    Vram_Load(sprite->pal, 128, transfer);
    Heap_Release(17);

    actor->orbit_center_x = actor->x;
    actor->orbit_angle = zero;
    actor->orbit_center_y = actor->y;
    actor->active = 1;
    actor->callback = (u32)SceneEffect_UpdateOrbitingParticle;
    actor->state = zero;
}

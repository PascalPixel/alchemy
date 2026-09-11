#include "types.h"
#include "scene.h"

#define UpdateOrbitingSceneObject Value

#include "facing_object.h"

struct SceneHandle {
    u8 unknown_00[9];
    u8 flags09;                     /* 0x09 */
};

struct SceneEntity {
    u8 unknown_00[0x23];
    u8 fp;                     /* 0x23 */
    u8 unknown_24[0x2c];
    struct SceneHandle *h;     /* 0x50 */
};

/* Entity and handle as the lobe-orbit callback reads them. */
struct SceneHandle {
    u8 unknown_00[30];
    s16 field1e;                    /* 0x1e */
};

struct SceneEntity {
    u8 unknown_00[8];
    s32 x;                          /* 0x08 */
    s32 y;                          /* 0x0c */
    u8 unknown_10[0x20];
    s32 phase;                      /* 0x30 */
    u8 unknown_34[4];
    s32 origin_x;                    /* 0x38 */
    s32 origin_y;                    /* 0x3c */
    u8 unknown_40[0x10];
    struct SceneHandle *handle;     /* 0x50 */
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
    u8 palette;
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

extern s16 gCell[];
extern u8 Value_00000022;
extern u8 gOv[];
extern u8 gOv2[];
extern u8 gOv3[];
extern u8 gOv4[];
extern u8 Value_000013c0;
extern s32 Effect_Run(void);   /* Random source. */
extern u8 Value;

struct SceneEntity *Effect_unk2_4();

OrbitingSceneObject *GetOrbitingSceneObject(void);

u8 *AllocateEffectTransfer(s32, s32);

/* gCell is the shared cross-overlay scene workspace; gWork is
 * a pointer cell holding the per-overlay workspace base, not the workspace
 * itself. The imports above are old-style because their arity varies between
 * call sites. */

s32 Actor_UpdateFacingTowardTarget(struct FacingObject *object)
{
    s32 delta;
    u16 old;
    s32 tgt;
    struct FacingObject *target;

    target = object->facing_target;
    if (target != NULL) {
        object->facing_flags = (u8)(0xFE & object->facing_flags);
        tgt = (u16)CalculateFacingAngle(target->position_z - object->position_z, target->position_x - object->position_x);
        old = object->facing;
        delta = (s16)(tgt - old);
        if (delta != 0) {
            if (delta > 0x1000) {
                delta = 0x1000;
            }
            if (delta < -0x1000) {
                delta = -0x1000;
            }
            object->facing = (u16)(old + delta);
        }
    }
    return 1;
}

/*
 * Returns the in-image table at 0x02008c08. The eight-byte owner at
 * 0x02000088 includes its one pool word, which holds that address and is
 * returned without being dereferenced.
 */
u8 *SceneData_GetTable8c08(void)
{
    return (u8 *)0x02008c08;
}

/*
 * Returns the in-image table at 0x02008c50. The eight-byte owner at
 * 0x02000094 includes its one pool word, which holds that address and is
 * returned without being dereferenced.
 */
u8 *SceneData_GetTable8C50(void)
{
    return (u8 *)0x02008c50;
}

u8 *SceneData_SelectRecordByScene22(void)
{
    if (gCell[224] == (s32)&Value_00000022) {
        if (Effect_unk2(0x84f) != 0)
            gOv[118] = 1;
        if (Effect_unk3(0x845) != 0)
            gOv[70] = 0;
        return gOv;
    }
    return gOv2;
}

void Scene_RunStepWithValueFd2(void)
{
    Effect_unk3_4();
    Effect_unk3_3(0xD, 0, 0);
    Effect_Do(0xFD2);
    Effect_Apply(0xB5, 3);
    Effect_Apply2(0xB5, 0);
    Effect_unk4_4();
}

void Scene_RunStepWithValue29de(void)
{
    Effect_unk5_4();
    Effect_Apply3(0x947, 1);
    Effect_Apply4(0x29DE, 1);
    Effect_unk6_4();
}

s32 SceneData_SelectTable8d24ByState(void)
{
    if (gCell[224] == (s32)&Value_00000022) {
        return (s32)gOv3;
    }
    return (s32)gOv4;
}

void Dialogue_RunActor9Message13c0(void)
{
    Effect_unk7_4();
    Effect_unk2_2((s32)&Value_000013c0);
    Effect_Apply5(9, 0);
    Effect_unk8_4();
}

void Dialogue_RunActor10Message13c3(void)
{
    Effect_unk9_4();
    Effect_unk3_2(0x13C3);
    Effect_Apply6(10, 0);
    Effect_unk10_4();
}

void Dialogue_RunActor11Message1751(void)
{
    Effect_unk11_4();
    Effect_unk4_2(0x1751);
    Effect_Apply7(11, 0);
    Effect_unk12_4();
}

void Scene_RunScene38e(void)
{
    extern u8 gWork[];

    u32 i;
    s32 record;

    Effect_unk13_4();
    if (Effect_Check(0x200) == 0) {
        Effect_unk14_4();
    }
    Effect_Place(0, 0x8000, 0x4000);
    *(s32 *)((*(u8 *volatile *)gWork + 0x1c0)) = 0x100;
    Effect_unk15_4(0, 2);
    Effect_unk2_3(0, 2, -16);
    Effect_unk16_4(16);
    Effect_unk17_4(2);
    Effect_unk18_4();
}

/*
 * Entry-0 root: seats scene entity 8 in its idle presentation and, when the
 * shared scene workspace says this is scene 0x22, runs the scene body.
 *
 * The handle mask is built from one register: zero is stored through fp first
 * and only then decremented by 13 to become ~0x0c, so the local carries a
 * stored value and then a mask, and must not be folded into two constants.
 */
s32 Scene_RunEntryZeroAndScene22Body(void)
{
    extern u8 *gWork;

    u8 *work = gWork;
    struct SceneEntity *ent;
    struct SceneHandle *h;
    u8 *fp;
    s32 zero;

    *(s32 *)(work + 448) = 256;            /* 128 << 1 */

    ent = Effect_unk2_4(8);
    fp = (u8 *)ent + 35;
    zero = 0;
    *fp = (u8)zero;

    h = ent->h;
    zero -= 13;
    h->flags09 = (u8)((h->flags09 & zero) | 0x04);

    if (gCell[224] == (s32)&Value_00000022) {
        Effect_unk19_4();
    }

    return 0;
}

void Scene_SetupWithDescriptor8BD4(void)
{
    Effect_unk5_2(0xBC);
    Effect_unk4_3(0x2008BD4, 0x34, 0xB);
    Effect_unk6_2(0x200);
}

/*
 * Walks one entity around a lobe of a sine and cosine figure and advances its
 * phase by a random step. The vertical term is forced non-positive, so the
 * path is one lobe rather than a full circle. The two trig calls take
 * different arguments and the two random draws are independent and summed:
 * neither pair is a common subexpression.
 */
s32 Effect_UpdateLobeOrbitEntity(struct SceneEntity *entity)
{
    struct SceneHandle *handle = entity->handle;
    s32 vertical;
    s32 tilt;
    s32 step;

    vertical = Effect_unk4(entity->phase) * 2;
    if (vertical > 0) vertical = -vertical;

    entity->x = entity->origin_x + Effect_unk5(entity->phase) * 2;
    entity->y = entity->origin_y + vertical;

    /* A quarter turn on from the position phase. */
    tilt = Effect_unk6(entity->phase + 0x8000);
    /* Bias then shift: division by 8 rounded toward zero. */
    if (tilt < 0) tilt += 7;
    handle->field1e = (s16)(tilt >> 3);

    /* The shift pair extracts a field, unsigned; it is not a scale. */
    step = (s32)(((u32)Effect_Run() << 9) >> 16)
         + (s32)(((u32)Effect_unk7() << 9) >> 16);
    entity->phase = entity->phase + step + 1024;

    return 0;
}

void InitializeOrbitingSceneEntity(void)
{
    OrbitingSceneObject *actor;
    OrbitingSceneObjectSprite *sprite;
    u8 *transfer;
    s32 zero;

    actor = GetOrbitingSceneObject();
    sprite = actor->sprite;
    sprite->flags_09_mode = 1;
    sprite->flags_05_bit_5 = 0;
    sprite->flags_09_high = 0;

    zero = 0;
    sprite->state = zero;
    NormalizeOrbitingSceneObject(actor, zero);
    actor->active = zero;
    actor->mode = zero;

    if (IsGameFlagSet(0x109) == 0)
        actor->y += 0x200000;

    actor->flags_23 &= 0xfe;
    actor->visible = 1;

    transfer = AllocateEffectTransfer(17, 0x608);
    LoadEffectResource(181);
    transfer += 0x400;
    ConfigurePaletteTransfer(sprite->palette, 128, transfer);
    ReleaseEffectTransfer(17);

    actor->orbit_center_x = actor->x;
    actor->orbit_angle = zero;
    actor->orbit_center_y = actor->y;
    actor->active = 1;
    actor->callback = (u32)&UpdateOrbitingSceneObject;
    actor->state = zero;
}

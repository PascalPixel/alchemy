#include "types.h"

#define NULL ((void *)0)
#define CalculateFacingAngle Func_02000aae
#define GetOrbitingSceneObject Func_02001474
#define NormalizeOrbitingSceneObject Func_0200144e
#define IsGameFlagSet Func_0200147c
#define AllocateEffectTransfer Func_0200146c
#define LoadEffectResource Func_020014ac
#define ConfigurePaletteTransfer Func_02001494
#define ReleaseEffectTransfer Func_02001492
#define UpdateOrbitingSceneObject Value_0200890d
#define SceneActor_UpdateFacingTowardTarget Func_02000030
#define SceneData_GetTable8c08 Func_02000088
#define SceneData_GetTable8C50 Func_02000094
#define SceneData_SelectRecordByScene22 Func_0200009c
#define FieldScene_RunStepWithValueFd2 Func_020000f8
#define FieldScene_RunStepWithValue29de Func_0200012c
#define SceneData_SelectTable8d24ByState Func_02000154
#define SceneDialogue_RunActor9Message13c0 Func_02000184
#define SceneDialogue_RunActor10Message13c3 Func_020001a4
#define SceneDialogue_RunActor11Message1751 Func_020001c4
#define FieldScene_RunScene38e_0200045c Func_0200045c
#define FieldScene_RunEntryZeroAndScene22Body Func_020004bc
#define FieldScene_SetupWithDescriptor8BD4 Func_020008e8
#define SceneEffect_UpdateLobeOrbitEntity Func_0200090c
#define InitializeOrbitingSceneEntity Func_02000974

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
struct SceneHandle_0200090c {
    u8 unknown_00[30];
    s16 field1e;                    /* 0x1e */
};

struct SceneEntity_0200090c {
    u8 unknown_00[8];
    s32 x;                          /* 0x08 */
    s32 y;                          /* 0x0c */
    u8 unknown_10[0x20];
    s32 phase;                      /* 0x30 */
    u8 unknown_34[4];
    s32 origin_x;                    /* 0x38 */
    s32 origin_y;                    /* 0x3c */
    u8 unknown_40[0x10];
    struct SceneHandle_0200090c *handle;     /* 0x50 */
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

extern s16 Data_02000240[];
extern u8 Value_00000022;
extern u8 Data_02008c7c[];
extern u8 Data_02008c64[];
extern u8 Data_02008d30[];
extern u8 Data_02008d24[];
extern u8 Value_000013c0;
extern s32 Func_020013a0(void);   /* Random source. */
extern u8 Value_0200890d;

s32 Func_02000aae(s32, s32);
s32 Func_02000b68(s32);
s32 Func_02000b7a(s32);
void Func_02000bd0(void);
s32 Func_02000c2a(s32, s32, s32);
void Func_02000bc8(s32);
void Func_02000cd8(s32, s32);
s32 Func_02000c00(s32, s32);
void Func_02000bfc(void);
void Func_02000c04(void);
void Func_02000bdc(s32, s32);
void Func_02000be4(s32, s32);
void Func_02000c20(void);
void Func_02000c5c(void);
void Func_02000ce2(s32);
s32 Func_02000d0a(s32, s32);
void Func_02000c76(void);
void Func_02000c7c(void);
void Func_02000d02(s32);
s32 Func_02000d2a(s32, s32);
void Func_02000c96(void);
void Func_02000c9c(void);
void Func_02000d22(s32);
s32 Func_02000d4a(s32, s32);
void Func_02000cb6(void);
void Func_02000d58();
s32 Func_02000f1c();
void Func_02000f34();
void Func_02000f72();
void Func_02000f7a();
void Func_02000f8c();
void Func_02000fb4();
void Func_02000fc0();
void Func_02001050();
struct SceneEntity *Func_02000fc4();
void Func_02000a00(void);
void Func_020014ba(s32);
void Func_0200138c(s32, s32, s32);
void Func_020013bc(s32);
s32 Func_0200137a(s32);
s32 Func_02001390(s32);
s32 Func_020013aa(s32);
s32 Func_020013a6(void);
OrbitingSceneObject *Func_02001474(void);
void Func_0200144e(OrbitingSceneObject *, s32);
s32 Func_0200147c(s32);
u8 *Func_0200146c(s32, s32);
void Func_020014ac(s32);
s32 Func_02001494(u8, s32, u8 *);
void Func_02001492(s32);

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

/* Data_02000240 is the shared cross-overlay scene workspace; Data_03001ebc is
 * a pointer cell holding the per-overlay workspace base, not the workspace
 * itself. The imports above are old-style because their arity varies between
 * call sites. */

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    extern u8 Data_03001ebc[];

    return f(a0);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    extern u8 Data_03001ebc[];

    f(a0, a1, a2);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    extern u8 Data_03001ebc[];

    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

s32 SceneActor_UpdateFacingTowardTarget(struct FacingObject *object)
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
    if (Data_02000240[224] == (s32)&Value_00000022) {
        if (Func_02000b68(0x84f) != 0)
            Data_02008c7c[118] = 1;
        if (Func_02000b7a(0x845) != 0)
            Data_02008c7c[70] = 0;
        return Data_02008c7c;
    }
    return Data_02008c64;
}

void FieldScene_RunStepWithValueFd2(void)
{
    Func_02000bd0();
    Func_02000c2a(0xD, 0, 0);
    Func_02000bc8(0xFD2);
    Func_02000cd8(0xB5, 3);
    Func_02000c00(0xB5, 0);
    Func_02000bfc();
}

void FieldScene_RunStepWithValue29de(void)
{
    Func_02000c04();
    Func_02000bdc(0x947, 1);
    Func_02000be4(0x29DE, 1);
    Func_02000c20();
}

s32 SceneData_SelectTable8d24ByState(void)
{
    if (Data_02000240[224] == (s32)&Value_00000022) {
        return (s32)Data_02008d30;
    }
    return (s32)Data_02008d24;
}

void SceneDialogue_RunActor9Message13c0(void)
{
    Func_02000c5c();
    Func_02000ce2((s32)&Value_000013c0);
    Func_02000d0a(9, 0);
    Func_02000c76();
}

void SceneDialogue_RunActor10Message13c3(void)
{
    Func_02000c7c();
    Func_02000d02(0x13C3);
    Func_02000d2a(10, 0);
    Func_02000c96();
}

void SceneDialogue_RunActor11Message1751(void)
{
    Func_02000c9c();
    Func_02000d22(0x1751);
    Func_02000d4a(11, 0);
    Func_02000cb6();
}

void FieldScene_RunScene38e_0200045c(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    s32 record;

    Func_02000f34();
    if (Value1(Func_02000f1c, 0x200) == 0) {
        Func_02000d58();
    }
    Call3(Func_02000f7a, 0, 0x8000, 0x4000);
    *(s32 *)((*(u8 *volatile *)Data_03001ebc + 0x1c0)) = 0x100;
    Func_02000fc0(0, 2);
    Call3(Func_02000fb4, 0, 2, -16);
    Func_02000f72(16);
    Func_02001050(2);
    Func_02000f8c();
}

/*
 * Entry-0 root: seats scene entity 8 in its idle presentation and, when the
 * shared scene workspace says this is scene 0x22, runs the scene body.
 *
 * The handle mask is built from one register: zero is stored through fp first
 * and only then decremented by 13 to become ~0x0c, so the local carries a
 * stored value and then a mask, and must not be folded into two constants.
 */
s32 FieldScene_RunEntryZeroAndScene22Body(void)
{
    extern u8 *Data_03001ebc;

    u8 *work = Data_03001ebc;
    struct SceneEntity *ent;
    struct SceneHandle *h;
    u8 *fp;
    s32 zero;

    *(s32 *)(work + 448) = 256;            /* 128 << 1 */

    ent = Func_02000fc4(8);
    fp = (u8 *)ent + 35;
    zero = 0;
    *fp = (u8)zero;

    h = ent->h;
    zero -= 13;
    h->flags09 = (u8)((h->flags09 & zero) | 0x04);

    if (Data_02000240[224] == (s32)&Value_00000022) {
        Func_02000a00();
    }

    return 0;
}

void FieldScene_SetupWithDescriptor8BD4(void)
{
    Func_020014ba(0xBC);
    Func_0200138c(0x2008BD4, 0x34, 0xB);
    Func_020013bc(0x200);
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

    vertical = Func_0200137a(entity->phase) * 2;
    if (vertical > 0) vertical = -vertical;

    entity->x = entity->origin_x + Func_02001390(entity->phase) * 2;
    entity->y = entity->origin_y + vertical;

    /* A quarter turn on from the position phase. */
    tilt = Func_020013aa(entity->phase + 0x8000);
    /* Bias then shift: division by 8 rounded toward zero. */
    if (tilt < 0) tilt += 7;
    handle->field1e = (s16)(tilt >> 3);

    /* The shift pair extracts a field, unsigned; it is not a scale. */
    step = (s32)(((u32)Func_020013a0() << 9) >> 16)
         + (s32)(((u32)Func_020013a6() << 9) >> 16);
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

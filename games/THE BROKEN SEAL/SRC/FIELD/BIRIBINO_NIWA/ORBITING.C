#include "TYPES.H"
#include "FIELD_EVENT.H"

#define NULL ((void *)0)
#define CalculateFacingAngle Func_02000aae
#define GetOrbitingSceneObject Func_02001474
#define AllocateEffectTransfer Func_0200146c
#define UpdateOrbitingSceneObject Value_0200890d

#include "FACING_OBJECT.H"

enum OrbitingMessage {
    MSG_ROBIN_PEERED_INTO = 0x947,
    MSG_TELLING_ME_IM_RESPONSIBLE_FOR = 0x13c3,
    MSG_DO_THINK_CAN_BECOME_AS = 0x1751,
    MSG_UPON_CLOSER_INSPECTION_SEEMS_DRIED = 0x29de
};


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
extern u8 Value_0200890d;

s32 Func_02000aae(s32, s32);
struct SceneEntity *Func_02000fc4();
void Func_02000a00(void);
OrbitingSceneObject *Func_02001474(void);
u8 *Func_0200146c(s32, s32);

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
    return f(a0);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
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
    if (gGameState.scene == (s32)&Value_00000022) {
        if (GameFlag_IsSet(0x84f) != 0)
            Data_02008c7c[118] = 1;
        if (GameFlag_IsSet(0x845) != 0)
            Data_02008c7c[70] = 0;
        return Data_02008c7c;
    }
    return Data_02008c64;
}

void FieldScene_RunStepWithValueFd2(void)
{
    Event_Begin();
    Actor_SetPosition(0xD, 0, 0);
    GameFlag_Set(0xFD2);
    Item_ShowFound(0xB5, 3);
    Party_GiveItem(0xB5, 0);
    Event_End();
}

void FieldScene_RunStepWithValue29de(void)
{
    Event_Begin();
    Message_ShowCentered(MSG_ROBIN_PEERED_INTO, 1);
    Message_ShowCentered(MSG_UPON_CLOSER_INSPECTION_SEEMS_DRIED, 1);
    Event_End();
}

s32 SceneData_SelectTable8d24ByState(void)
{
    if (gGameState.scene == (s32)&Value_00000022) {
        return (s32)Data_02008d30;
    }
    return (s32)Data_02008d24;
}

void SceneDialogue_RunActor9Message13c0(void)
{
    Event_Begin();
    Event_SetMessage((s32)&Value_000013c0);
    Event_AskYesNo(9, 0);
    Event_End();
}

void SceneDialogue_RunActor10Message13c3(void)
{
    Event_Begin();
    Event_SetMessage(MSG_TELLING_ME_IM_RESPONSIBLE_FOR);
    Event_AskYesNo(10, 0);
    Event_End();
}

void SceneDialogue_RunActor11Message1751(void)
{
    Event_Begin();
    Event_SetMessage(MSG_DO_THINK_CAN_BECOME_AS);
    Event_AskYesNo(11, 0);
    Event_End();
}

void FieldScene_RunScene38e_0200045c(void)
{
    Event_Begin();
    if (GameFlag_IsSet(0x200) == 0) {
        FieldScene_SetupWithDescriptor8BD4();
    }
    Actor_SetSpeed(0, 0x8000, 0x4000);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    Actor_SetAnimation(0, 2);
    Actor_CenterAndWalk(0, 2, -16);
    Event_Wait(16);
    Event_RequestExit(2);
    Event_End();
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

    if (gGameState.scene == (s32)&Value_00000022) {
        Func_02000a00();
    }

    return 0;
}

void FieldScene_SetupWithDescriptor8BD4(void)
{
    Audio_PlayCue(0xBC);
    Map_AnimateCells(0x2008BD4, 0x34, 0xB);
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
    Actor_SetSpriteFlags(actor, zero);
    actor->active = zero;
    actor->mode = zero;

    if (GameFlag_IsSet(0x109) == 0)
        actor->y += 0x200000;

    actor->flags_23 &= 0xfe;
    actor->visible = 1;

    transfer = AllocateEffectTransfer(17, 0x608);
    Item_LoadIcon(181);
    transfer += 0x400;
    Vram_Load(sprite->palette, 128, transfer);
    Heap_Release(17);

    actor->orbit_center_x = actor->x;
    actor->orbit_angle = zero;
    actor->orbit_center_y = actor->y;
    actor->active = 1;
    actor->callback = (u32)&UpdateOrbitingSceneObject;
    actor->state = zero;
}

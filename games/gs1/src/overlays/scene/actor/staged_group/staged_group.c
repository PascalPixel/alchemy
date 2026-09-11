#include "types.h"
#include "scene.h"
#include "abi/overlays/scene/actor/staged_group/staged_group.h"
#include "resource_3a8_effect.h"

/* overlays/scene/actor/staged_group/actor_presentation.c */
struct Obj {
    u8 filler00[6];
    u16 f06;
};

struct Obj *Actor_Run9(s32);

void Dialogue_RunActorTwentyThreeByLeaderHeading(void)
{
    struct Obj *o;
    u32 v;

    o = Actor_Run9(0);
    v = (o->f06 - 0x6001) << 16;
    if (v <= 0x7ffe0000) {
        Actor_Apply(23, 23);
    } else {
        Actor_Run10();
        Actor_Do12(0x1ad1);
        Actor_Apply2(23, 0);
        Actor_Run11();
    }
}

void Actor_SetActorZeroByte35Bit0(void)
{
    u8 *p;
    s32 v;

    p = (u8 *)(Actor_Check30(0) + 35);
    v = 1;
    v |= *p;
    *p = v;
}

/* overlays/scene/actor/staged_group/actor_search.c */
extern u8 *gWork;

s32 *Actor_FindAtTileXZ(s32 x, s32 z)
{
    s32 **tbl = (s32 **)(gWork + 0x14);
    u32 i;

    for (i = 8; i <= 65; i++) {
        s32 *p = tbl[i];

        if (x == (p[2] >> 20) && z == (p[4] >> 20)) {
            return p;
        }
    }
    return 0;
}

/* overlays/scene/actor/staged_group/effect.c */

struct Obj {
    u8 filler00[100];
    u16 f64;
    u16 f66;
};

extern u8 gOv[];

struct Obj *Actor_Run12(s32);

s32 Effect_UpdateRandomAction(struct Resource3a8Effect *effect)
{
    u32 action;
    u32 next_timer;

    if (effect->action_timer == 0) {
        action = (u32)(SampleEffectAction() * 8) >> 0x10;
        switch (action) {
        case 0:
            RunEffectAction3(effect, 3);
            break;
        case 1:
            RunEffectAction4(effect, 4);
            break;
        case 3:
        case 4:
            effect->unknown_06 += (u32)(SampleEffectOffset() << 0xF) >> 0x10;
            break;
        }
        next_timer = (u32)(SampleEffectTimer() * 0x50) >> 0x10;
        effect->action_timer = (s16)next_timer;
        if (next_timer != 0) {
            goto decrement_timer;
        }
    } else {
decrement_timer:
        --effect->action_timer;
    }
    return 1;
}

void Effect_SetSlotVariantAndDescriptor(s32 a)
{
    struct Obj *p;
    u32 t;

    p = Actor_Run12(a);
    p->f64 = a;
    t = Actor_Run13();
    p->f66 = (t * 5) >> 12;
    Actor_Apply3(p, (s32)gOv);
}

/* overlays/scene/actor/staged_group/facing_target_scene.c */
struct Obj {
    u8 filler00[6];
    u16 f06;
};

struct Obj *Actor_Run14(s32);

struct Obj *Actor_Run15(s32);

void State_BranchOnSlotZeroFacing(void)
{
    struct Obj *o;
    u32 v;

    o = Actor_Run14(0);
    v = (o->f06 - 0x2000) << 16;
    if (v > 0x80000000) {
        Actor_Apply4(22, 22);
    } else {
        Actor_Run16();
        Actor_Do13(0x1acf);
        Actor_Apply5(22, 0);
        Actor_Run17();
    }
}

void Scene_RunActorTwentyFourAngleDialogue(void)
{
    struct Obj *o;
    u32 v;

    o = Actor_Run15(0);
    v = (o->f06 - 0x2000) << 16;
    if (v > 0xC0000000) {
        Actor_Apply6(24, 24);
    } else {
        Actor_Run18();
        Actor_Do14(0x1ad5);
        Actor_Apply7(24, 0);
        Actor_Run19();
    }
}

/* overlays/scene/actor/staged_group/orbiting_effect.c */
#define UpdateOrbitingSceneObject Value

typedef struct RenderData {
    unsigned char pad00[0x1E];
    s16 rotation;
} RenderData;

typedef struct Effect {
    unsigned char pad00[8];
    s32 x;
    s32 y;
    unsigned char pad10[0x20];
    s32 angle;
    unsigned char pad34[4];
    s32 base_x;
    s32 base_y;
    unsigned char pad40[0x10];
    RenderData *render;
} Effect;

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

extern u8 Value;

Effect *Actor_Run20(s32);

OrbitingSceneObject *GetOrbitingSceneObject();
OrbitingSceneObject *IsGameFlagSet();

u8 *AllocateEffectTransfer(s32, s32);

void Effect_UpdateLobeOrbitEffect26(void)
{
    Effect *effect = Actor_Run20(26);
    RenderData *render = effect->render;
    s32 offset = Actor_Check31(effect->angle) * 2;
    s32 first;

    if (offset > 0) {
        offset = -offset;
    }
    effect->x = effect->base_x + Actor_Check32(effect->angle) * 2;
    effect->y = effect->base_y + offset;
    render->rotation = Actor_Check33(effect->angle + 0x8000) >> 3;
    first = Actor_Check34();
    effect->angle +=
        ((u32)(first << 9) >> 16)
        + ((u32)(Actor_Check35() << 9) >> 16)
        + 0x400;
}

s32 Effect_UpdateOrbitingEffect(Effect *effect)
{
    RenderData *render = effect->render;
    s32 ofs = Actor_Check36(effect->angle) * 2;
    s32 first;

    if (ofs > 0) {
        ofs = -ofs;
    }
    effect->x = effect->base_x + Actor_Check37(effect->angle) * 2;
    effect->y = effect->base_y + ofs;
    render->rotation = Actor_Check38(effect->angle + 0x8000) / 8;
    first = Actor_Check39();
    effect->angle +=
        ((u32)(first << 9) >> 16)
        + ((u32)(Actor_Check40() << 9) >> 16)
        + 0x400;
    return 0;
}

void InitializeOrbitingRenderEffect(void)
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

/* overlays/scene/actor/staged_group/overlay_object.c */
struct Object {
    u8 filler00[6];
    u16 x;
    u8 filler08[94];
    s16 cnt;
};

s32 OvObj_AdvanceXWhenCounterExpires(struct Object *obj)
{
    s32 cnt = *(u16 *)&obj->cnt;
    s32 loaded = obj->cnt;
    s32 amount;

    if (loaded == 0) {
        obj->x += (u32)(Actor_Check41() << 15) >> 16;
        amount = (u32)(Actor_Check42() * 80) >> 16;
        obj->cnt = amount;
        if (amount == 0) {
            goto done;
        }
        cnt = amount;
    }
    obj->cnt = cnt - 1;
done:
    return 1;
}

/* overlays/scene/actor/staged_group/run_staged_group_sequence.c */
extern u8 Value_00001a9e[];
extern u8 Value_00001aa2[];

/* Two early long branches share the scene-skip tail. Four polling loops
 * wait on signed actor fields; calls bind at loader runtime addresses. */

u8 *Actor_Run21();

extern const u32 SceneAction_GroupMotion[];
extern const u32 SceneAction_GroupOffsetMotion[];

/* Verified scene siblings use these call forms for independently evaluated
 * large constants in repeated actor operations. */

void Scene_RunStagedGroupSequence(void)
{
    s32 messageId;
    const u32 *actionDescriptor;
    s32 idleState;
    s32 actionEnabled;
    u8 *actor20PairedWait;
    u8 *actor21PairedWait;
    u8 *closingWaitRecord;
    u8 *actor14FinalFacing;
    u8 *actor20FinalFacing;
    u8 *actor21FinalFacing;
    u8 *playerFinalFacing;
    u8 *actor21OpeningWait;
    u8 *actor20BeforeFirstMove;
    u8 *actor20AfterFirstMove;
    u8 *actor20BeforeSecondMove;
    u8 *actor20AfterSecondMove;
    u8 *sceneWorkspace;

    sceneWorkspace = *(u8 **)0x03001ebc;
    Actor_Run22();
    if (Actor_Check43(2320) == 0) {
        goto skip_scene;
    }
    if (Actor_Check43(2321) != 0) {
        goto skip_scene;
    }
    Actor_Run23(33605960);
    Actor_Far(20, 16515072, 17825792);
    Actor_Far(27, 18612224, 17301504);
    Actor_Far(28, 18612224, 18350080);
    Actor_Far(29, 19660800, 17301504);
    Actor_Far(30, 19660800, 18350080);
    Actor_Far(32, 20709376, 17301504);
    Actor_Far(31, 20709376, 18350080);
    Actor_Far(33, 21757952, 17301504);
    Actor_Far(34, 21757952, 18350080);
    Actor_Far(21, 23855104, 17825792);
    Actor_Run24(17);
    Actor_Run25(20);
    Actor_Run26(6801, 1, 0);
    Actor_Run24(9);
    Actor_Run27(10);
    Actor_Run28(0, 2);
    if (*(s16 *)(sceneWorkspace + 364) == 9) {
        Actor_Far6(157286, 19660);
        Actor_Far3(0, 57344, 20);
    } else {
        Actor_Far6(78643, 9830);
        Actor_Far3(0, 0, 20);
    }
    Actor_Far2(20, 72089, 36044);
    Actor_Far2(27, 65536, 32768);
    Actor_Far2(28, 65536, 32768);
    Actor_Far2(29, 58982, 29491);
    Actor_Far2(30, 58982, 29491);
    Actor_Far2(32, 52428, 26214);
    Actor_Far2(31, 52428, 26214);
    Actor_Far2(33, 45875, 22937);
    Actor_Far2(34, 45875, 22937);
    Actor_Far2(21, 39321, 19660);
    actionDescriptor = SceneAction_GroupMotion;
    Actor_Run29(20, actionDescriptor);
    Actor_Run29(27, actionDescriptor);
    Actor_Run29(28, actionDescriptor);
    Actor_Run29(29, actionDescriptor);
    Actor_Run29(30, actionDescriptor);
    Actor_Run29(32, actionDescriptor);
    Actor_Run29(31, actionDescriptor);
    Actor_Run29(33, actionDescriptor);
    Actor_Run29(34, actionDescriptor);
    actor21OpeningWait = Actor_Run21(21);
    {
        s32 value = 0;
        *(u16 *)(actor21OpeningWait + 100) = value;
    }
    Actor_Run29(21, actionDescriptor);
    Actor_Run30(12189696, -1, 17825792, 1);
    Actor_Run31(20);
    Actor_Far3(20, 0, 0);
    do {
        Actor_Run32(1);
    } while (*(s16 *)((u8 *)actor21OpeningWait + 100) == 0);
    Actor_Run27(40);
    Actor_Run28(27, 2);
    Actor_Far3(27, 20480, 20);
    Actor_Run33(6802);
    Actor_Run34(27, 0, 10);
    Actor_Run28(28, 2);
    Actor_Far3(28, 45056, 10);
    Actor_Run35(28, 3);
    Actor_Run34(28, 0, 10);
    Actor_Far5(32, 258);
    Actor_Run27(40);
    Actor_Run34(32, 0, 10);
    Actor_Far4(31, 256, 40);
    Actor_Far3(31, 45056, 10);
    Actor_Run34(31, 0, 10);
    Actor_Far3(31, 32768, 10);
    Actor_Run28(31, 2);
    Actor_Run35(31, 4);
    Actor_Run34(31, 0, 10);
    Actor_Far3(31, 45056, 0);
    Actor_Far3(32, 20480, 20);
    Actor_Run35(31, 3);
    Actor_Run36(32, 3);
    Actor_Run28(20, 2);
    Actor_Far5(20, 258);
    Actor_Run27(40);
    Actor_Run34(20, 0, 10);
    Actor_Run28(20, 2);
    actor20BeforeFirstMove = Actor_Run21(20);
    {
        s32 flags = actor20BeforeFirstMove[90] & 0xfe;
        idleState = 0;
        actor20BeforeFirstMove[90] = flags;
    }
    Actor_Far7(20, 172, 264);
    Actor_Run27(1);
    actor20AfterFirstMove = Actor_Run21(20);
    actionEnabled = 1;
    {
        s32 flags = actor20AfterFirstMove[90];
        flags |= actionEnabled;
        actor20AfterFirstMove[90] = flags;
    }
    Actor_Far3(27, 32768, 0);
    Actor_Far3(28, 32768, 0);
    Actor_Far3(32, 32768, 0);
    Actor_Far3(31, 32768, 20);
    Actor_Run36(20, 3);
    Actor_Run27(20);
    actor20BeforeSecondMove = Actor_Run21(20);
    *(u8 *)((u8 *)(actor20BeforeSecondMove) + 90) &= 0xfe;
    Actor_Far7(20, 172, 272);
    Actor_Run27(1);
    actor20AfterSecondMove = Actor_Run21(20);
    actionEnabled |= actor20AfterSecondMove[90];
    actor20AfterSecondMove[90] = actionEnabled;
    Actor_Far7(20, 180, 272);
    Actor_Far3(20, 0, 0);
    Actor_Run34(20, 0, 10);
    Actor_Far4(34, 261, 0);
    Actor_Run28(34, 1);
    Actor_Run36(34, 3);
    Actor_Run34(34, 0, 10);
    Actor_Run28(33, 1);
    Actor_Run34(33, 0, 10);
    Actor_Run35(33, 4);
    Actor_Run34(33, 0, 10);
    Actor_Run28(21, 2);
    Actor_Far4(21, 258, 0);
    Actor_Run34(21, 0, 10);
    Actor_Run37(20, 2, 20);
    Actor_Run37(20, 4, 40);
    Actor_Run28(20, 2);
    Actor_Run34(20, 0, 10);
    Actor_Far2(21, 104857, 52428);
    Actor_Far7(21, 265, 282);
    Actor_Far7(21, 251, 284);
    Actor_Far7(21, 246, 296);
    Actor_Far3(21, 49152, 0);
    State_SetValues27Through34();
    Actor_Run27(40);
    Actor_Far2(21, 104857, 52428);
    Actor_Far7(21, 228, 296);
    Actor_Far3(21, 49152, 40);
    Actor_Far7(21, 212, 296);
    Actor_Far3(21, 49152, 40);
    Actor_Far7(21, 192, 296);
    Actor_Far3(21, 49152, 40);
    Actor_Run38(21, 2);
    Actor_Far4(21, 256, 60);
    Actor_Far3(20, 12288, 0);
    Actor_Far7(21, 184, 286);
    Actor_Far3(21, 45056, 10);
    Actor_Far5(21, 258);
    Actor_Run27(40);
    Actor_Run36(21, 4);
    Actor_Far4(20, 257, 40);
    Actor_Run36(20, 3);
    Actor_Run28(20, 2);
    Actor_Far3(20, 0, 0);
    Actor_Far3(21, 0, 60);
    Actor_Far3(20, 12288, 0);
    Actor_Far3(21, 45056, 10);
    Actor_Run36(20, 3);
    Actor_Run36(21, 3);
    Actor_Far3(21, 0, 0);
    Actor_Far2(20, 104857, 52428);
    Actor_Run39(20, 33603504);
    Actor_Far7(20, 228, 296);
    Actor_Far3(20, 49152, 40);
    Actor_Far7(20, 212, 296);
    Actor_Far3(20, 49152, 40);
    Actor_Far7(20, 192, 296);
    Actor_Far3(20, 49152, 40);
    Actor_Far3(20, 45056, 0);
    Actor_Far3(21, 12288, 10);
    Actor_Far5(20, 258);
    Actor_Run27(60);
    Actor_Run36(20, 4);
    messageId = (s32)Value_00001a9e;
    Actor_Run33(messageId);
    Actor_Run34(20, 0, 40);
    Actor_Run40();
    Actor_Far7(20, 178, 272);
    Actor_Far3(20, 0, 0);
    Actor_Run27(240);
    Actor_Run41(27);
    Actor_Run32(1);
    Actor_Far3(27, 32768, 10);
    Actor_Far4(27, 257, 60);
    Actor_Run34(27, 0, 10);
    Effect_SetSlotVariantAndDescriptor(27);
    Actor_Run27(80);
    Actor_Run41(28);
    Actor_Run32(1);
    Actor_Far3(28, 53248, 20);
    Actor_Run28(28, 2);
    Actor_Run34(28, 0, 10);
    Effect_SetSlotVariantAndDescriptor(28);
    Actor_Run27(160);
    Actor_Run41(32);
    Actor_Run32(1);
    Actor_Far3(32, 20480, 10);
    Actor_Far4(32, 257, 60);
    Actor_Run34(32, 0, 10);
    Effect_SetSlotVariantAndDescriptor(32);
    Actor_Run27(80);
    Actor_Run41(30);
    Actor_Run32(1);
    Actor_Far3(30, 45056, 10);
    Actor_Run28(30, 1);
    Actor_Run33(messageId + 6);
    Actor_Run34(30, 0, 10);
    Actor_Run42(33595157);
    Actor_Run41(20);
    Actor_Run41(21);
    Actor_Run32(1);
    actor20PairedWait = Actor_Run21(20);
    *(u16 *)(actor20PairedWait + 100) = idleState;
    actor21PairedWait = Actor_Run21(21);
    *(u16 *)(actor21PairedWait + 100) = idleState;
    Actor_Far2(20, 52428, 26214);
    Actor_Far2(21, 52428, 26214);
    Actor_Run29(20, 33603636);
    Actor_Run29(21, 33603788);
    Actor_Run41(29);
    Actor_Run32(1);
    Actor_Far3(29, 20480, 10);
    messageId += 5;
    Actor_Run28(29, 2);
    Actor_Run33(messageId);
    Actor_Run34(29, 0, 20);
    Effect_SetSlotVariantAndDescriptor(29);
    Effect_SetSlotVariantAndDescriptor(30);
    do {
        Actor_Run32(1);
    } while (*(s16 *)((u8 *)Actor_Run21(20) + 100) == 0 ||
             *(s16 *)((u8 *)Actor_Run21(21) + 100) != 1);
    Actor_Run29(20, 33603940);
    Actor_Run29(21, 33604012);
    Actor_Run41(31);
    Actor_Run32(1);
    Actor_Far3(31, 0x5000, 10);
    Actor_Run28(31, 1);
    Actor_Run36(31, 4);
    messageId = (s32)Value_00001aa2;
    Actor_Run33(messageId);
    Actor_Run34(31, 0, 10);
    Effect_SetSlotVariantAndDescriptor(31);
    Actor_Run41(34);
    Actor_Run41(33);
    Actor_Run32(1);
    Actor_Far4(34, 261, 40);
    Actor_Far4(33, 261, 60);
    Actor_Far3(34, 45056, 10);
    Actor_Far3(33, 20480, 10);
    messageId += 3;
    Actor_Run36(34, 4);
    Actor_Run33(messageId);
    Actor_Run34(34, 0, 10);
    Actor_Run28(33, 1);
    Actor_Run35(33, 4);
    Actor_Run34(33, 0, 10);
    Actor_Far4(34, 258, 60);
    Actor_Far4(20, 259, 0);
    Actor_Run28(20, 2);
    Actor_Run33(6834);
    Actor_Run34(20, 0, 10);
    Actor_Run41(27);
    Actor_Run41(28);
    Actor_Run41(29);
    Actor_Run41(30);
    Actor_Run41(32);
    Actor_Run41(31);
    Actor_Run41(33);
    Actor_Run41(34);
    Actor_Run41(20);
    Actor_Run41(21);
    Actor_Run32(1);
    Actor_Run37(27, 2, 0);
    Actor_Run37(28, 2, 0);
    Actor_Run37(29, 2, 0);
    Actor_Run37(30, 2, 0);
    Actor_Run37(32, 2, 0);
    Actor_Run37(31, 2, 0);
    Actor_Run37(33, 2, 0);
    Actor_Run37(34, 2, 0);
    Actor_Run37(21, 2, 40);
    Actor_Far3(27, 32768, 0);
    Actor_Far3(28, 32768, 0);
    Actor_Far3(29, 32768, 0);
    Actor_Far3(30, 32768, 0);
    Actor_Far3(32, 32768, 0);
    Actor_Far3(31, 32768, 0);
    Actor_Far3(33, 32768, 0);
    Actor_Far3(34, 32768, 40);
    Actor_Run37(21, 4, 40);
    Actor_Run34(21, 0, 10);
    Actor_Run28(20, 1);
    Actor_Run34(20, 0, 10);
    Actor_Run36(21, 3);
    Actor_Run34(21, 0, 10);
    Actor_Run36(20, 3);
    Actor_Run34(20, 0, 10);
    Actor_Far4(27, 258, 40);
    Actor_Run38(27, 1);
    Actor_Run34(27, 0, 10);
    Actor_Far4(28, 258, 40);
    Actor_Run34(28, 0, 10);
    Actor_Run36(21, 4);
    Actor_Run27(40);
    Actor_Run36(21, 3);
    Actor_Run34(21, 0, 20);
    Actor_Run36(20, 3);
    Actor_Run34(20, 0, 10);
    Actor_Far3(27, 20480, 0);
    Actor_Far3(28, 45056, 4);
    Actor_Far3(29, 20480, 0);
    Actor_Far3(30, 45056, 4);
    Actor_Far3(32, 20480, 0);
    Actor_Far3(31, 45056, 4);
    Actor_Far3(33, 20480, 0);
    Actor_Far3(34, 45056, 4);
    Actor_Run35(27, 3);
    Actor_Run36(28, 3);
    Actor_Run35(29, 3);
    Actor_Run36(30, 3);
    Actor_Run35(32, 3);
    Actor_Run36(31, 3);
    Actor_Run35(33, 3);
    Actor_Run36(34, 3);
    Actor_Run37(20, 2, 40);
    Actor_Run34(20, 0, 10);
    Actor_Far3(27, 32768, 0);
    Actor_Far3(28, 32768, 4);
    Actor_Far3(29, 32768, 0);
    Actor_Far3(30, 32768, 4);
    Actor_Far3(32, 32768, 0);
    Actor_Far3(31, 32768, 4);
    Actor_Far3(33, 32768, 0);
    Actor_Far3(34, 32768, 4);
    Actor_Far2(20, 72089, 36044);
    Actor_Far2(27, 68812, 34406);
    Actor_Far2(28, 68812, 34406);
    Actor_Far2(29, 65536, 32768);
    Actor_Far2(30, 65536, 32768);
    Actor_Far2(32, 0xf333, 0x7999);
    Actor_Far2(31, 0xf333, 0x7999);
    Actor_Far2(33, 58982, 29491);
    Actor_Far2(34, 58982, 29491);
    Actor_Far2(21, 55705, 27852);
    Actor_Run43(27, 1);
    Actor_Run43(28, 1);
    Actor_Run43(29, 1);
    Actor_Run43(30, 1);
    Actor_Run43(32, 1);
    Actor_Run43(31, 1);
    Actor_Run43(33, 1);
    Actor_Run43(34, 1);
    Actor_Run43(20, 1);
    Actor_Run43(21, 1);
    Actor_Run41(27);
    Actor_Run41(28);
    Actor_Run41(29);
    Actor_Run41(30);
    Actor_Run41(32);
    Actor_Run41(31);
    Actor_Run41(33);
    Actor_Run41(34);
    Actor_Run41(20);
    Actor_Run41(21);
    Actor_Run32(1);
    actionDescriptor = SceneAction_GroupOffsetMotion;
    Actor_Run29(20, actionDescriptor);
    Actor_Run29(27, actionDescriptor);
    Actor_Run29(28, actionDescriptor);
    Actor_Run29(29, actionDescriptor);
    Actor_Run29(30, actionDescriptor);
    Actor_Run29(32, actionDescriptor);
    Actor_Run29(31, actionDescriptor);
    Actor_Run29(33, actionDescriptor);
    Actor_Run29(34, actionDescriptor);
    closingWaitRecord = Actor_Run21(21);
    {
        u16 *state = (u16 *)(closingWaitRecord + 100);
        s32 value = 0;
        *state = value;
    }
    Actor_Run29(21, actionDescriptor);
    do {
        Actor_Run32(1);
    } while (*(s16 *)((u8 *)Actor_Run21(21) + 100) != 1);
    Actor_Run27(80);
    Actor_Far(14, 22282240, 17956864);
    Actor_Run32(1);
    Actor_Far2(14, 65536, 32768);
    Actor_Far7(14, 224, 274);
    Actor_Far3(14, 0, 40);
    Actor_Far3(14, 32768, 40);
    Actor_Far3(14, 49152, 40);
    Actor_Far3(14, 20480, 40);
    Actor_Far4(14, 257, 60);
    Actor_Run34(14, 0, 10);
    Actor_Far3(14, 0, 40);
    Actor_Far3(14, 49152, 40);
    Actor_Far3(14, 32768, 40);
    Actor_Far5(14, 258);
    Actor_Run37(14, 4, 40);
    Actor_Run34(14, 0, 20);
    Actor_Run28(14, 2);
    Actor_Run34(14, 0, 10);
    Actor_Run37(14, 4, 40);
    Actor_Far2(14, 78643, 39321);
    closingWaitRecord = Actor_Run21(14);
    {
        u16 *state = (u16 *)(closingWaitRecord + 100);
        s32 value = 0;
        *state = value;
    }
    Actor_Run29(14, SceneAction_GroupOffsetMotion);
    do {
        Actor_Run32(1);
    } while (*(s16 *)((u8 *)Actor_Run21(14) + 100) != 1);
    Actor_Far(14, 23527424, 20578304);
    actor14FinalFacing = Actor_Run21(14);
    {
        s32 value = 53248;
        *(u16 *)(actor14FinalFacing + 6) = value;
    }
    Actor_Far(20, 29818880, 28442624);
    actor20FinalFacing = Actor_Run21(20);
    {
        s32 value = 53248;
        *(u16 *)(actor20FinalFacing + 6) = value;
    }
    Actor_Far(21, 30408704, 27262976);
    actor21FinalFacing = Actor_Run21(21);
    {
        s32 value = 20480;
        *(u16 *)(actor21FinalFacing + 6) = value;
    }
    Actor_Run44(27);
    Actor_Run44(28);
    Actor_Run44(29);
    Actor_Run44(30);
    Actor_Run44(31);
    Actor_Run44(32);
    Actor_Run44(33);
    Actor_Run44(34);
    Actor_Run24(17);
    if (*(s16 *)(sceneWorkspace + 364) == 9) {
        Actor_Far7(0, 224, 458);
        playerFinalFacing = Actor_Run21(0);
        {
            s32 value = 49152;
            *(u16 *)(playerFinalFacing + 6) = value;
        }
    } else {
        Actor_Far7(0, 40, 248);
        playerFinalFacing = Actor_Run21(0);
        {
            s32 value = 16384;
            *(u16 *)(playerFinalFacing + 6) = value;
        }
    }
    Actor_Run45();
    Actor_Run46(0x911);
    goto finish;
skip_scene:
    Actor_Run24(123);
    Actor_Run47(*(s16 *)(sceneWorkspace + 364));
    Actor_Run48();
    Actor_Run49();
finish:
    Actor_Run50();
}

/* overlays/scene/actor/staged_group/scene_data.c */
extern s16 gCell[];
extern u8 Value_00000063;
extern u8 Value_00000066;
extern u8 Value_00000099;
extern u8 Value_0000009a;
extern u8 Value_0000009b;
extern u8 Value_0000009c;
extern u8 gOv2[];
extern u8 gOv3[];
extern u8 gOv4[];
extern u8 gOv5[];
extern u8 gOv6[];
extern u8 gOv7[];
extern u8 gOv8[];
extern u8 gOv9[];
extern u8 gOv10[];
extern u8 gOv11[];
extern u8 gOv12[];
extern u8 gOv13[];
extern u8 gOv14[];
extern u8 gOv15[];
extern u8 gOv16[];
extern u8 gOv17[];
extern u8 gOv18[];
extern u8 gOv19[];
extern u8 gOv20[];
extern u8 gOv21[];

/* Contiguous unnamed leaf-owner run for resource_3a8. */

s32 SceneData_SelectTableByWord224(void)
{
    s16 v = gCell[224];

    if (v == (s32)&Value_00000063) {
        return (s32)gOv2;
    }
    if (v == (s32)&Value_00000066) {
        return (s32)gOv3;
    }
    if (v == (s32)&Value_00000099) {
        return (s32)gOv4;
    }
    if (v == (s32)&Value_0000009a) {
        return (s32)gOv5;
    }
    if (v == (s32)&Value_0000009b) {
        return (s32)gOv6;
    }
    if (v == (s32)&Value_0000009c) {
        return (s32)gOv7;
    }
    return (s32)gOv8;
}

s32 SceneData_ReturnZero(void)
{
    return 0;
}

u8 *SceneData_GetTablec6b8(void)
{
    return gOv9;
}

s32 SceneData_SelectTableByWord224B(void)
{
    s16 v = gCell[224];

    if (v == (s32)&Value_00000063) {
        return (s32)gOv10;
    }
    if (v == (s32)&Value_00000066) {
        return (s32)gOv11;
    }
    if (v == (s32)&Value_00000099) {
        return (s32)gOv12;
    }
    if (v == (s32)&Value_0000009c) {
        return (s32)gOv13;
    }
    return (s32)gOv14;
}

s32 SceneData_SelectTableBySelector224(void)
{
    s16 v = gCell[224];

    if (v == (s32)&Value_00000063) {
        return (s32)gOv15;
    }
    if (v == (s32)&Value_00000066) {
        return (s32)gOv16;
    }
    if (v == (s32)&Value_00000099) {
        return (s32)gOv17;
    }
    if (v == (s32)&Value_0000009a) {
        return (s32)gOv18;
    }
    if (v == (s32)&Value_0000009b) {
        return (s32)gOv19;
    }
    if (v == (s32)&Value_0000009c) {
        return (s32)gOv20;
    }
    return (s32)gOv21;
}

/* overlays/scene/actor/staged_group/scene_dialogue.c */
extern u8 gOv22[];

void Dialogue_RunActorNineteenDialogue(void)
{
    Actor_Run51();
    Actor_Do15(0x1a7c);
    Actor_Apply8(19, 0);
    Actor_Run52();
}

void Dialogue_RunActorTenDialogue(void)
{
    Actor_Run53();
    Actor_Do16(0x25b3);
    Actor_Apply9(10, 0);
    Actor_Run54();
}

void Dialogue_RunActor181Line916(void)
{
    Actor_Do17((s32)gOv22);
    Actor_Do18(1);
    Actor_Place15(26, 0, 0);
    Actor_Do19(0x916);
    Actor_Apply10(181, 3);
    Actor_Apply11(181, 0);
}

/* overlays/scene/actor/staged_group/scene_motion.c */
struct Obj {
    u8 filler00[8];
    s32 f08;
    s32 f0c;
    u8 filler10[8];
    s32 f18;
    s32 f1c;
    s32 f20;
    s32 f24;
    s32 f28;
    u8 filler2c[56];
    s16 f64;
};

void Effect_UpdateMotionWithDamping(struct Obj *p)
{
    s16 *h;
    s32 v;
    s32 a;
    s32 b;

    h = &p->f64;
    v = *h;
    if (v == 0) {
        Actor_Run55();
    } else if (v == 1) {
        p->f24 = 0;
        p->f28 = 0;
        p->f08 = 0;
        p->f0c = 0;
    } else {
        p->f18 += 0x800;
        p->f1c += 0x800;
    }
    p->f08 += p->f24;
    p->f0c += p->f28;
    a = p->f24;
    b = p->f28;
    p->f24 = a - a / 256;
    p->f28 = b - b / 16;
    { s32 t = *(u16 *)h; t -= 1; *(u16 *)h = t; }
}

/* overlays/scene/actor/staged_group/scene_primary_script.c */

extern u8 Value_00000063;
extern u8 Value_00000066;
extern u8 Value_00000099;
extern u8 Value_0000009b;
extern u8 Value_0000009c;
extern u8 gWork[];
extern s16 gCell2[][1];
extern volatile s32 gOv23[];

u8 *Actor_Run56();

u8 *Actor_Run57();

u8 *Actor_Run58();

u8 *Actor_Run59();

u8 *Scene_GetRecord_1();

u8 *Scene_GetRecord_2();

u8 *Scene_GetRecord_5();
u8 *Scene_GetRecord_6();

u8 *Scene_GetRecord_12();
u8 *Scene_GetRecord_13();

u8 *Actor_Run60();
u8 *Actor_Run61();
u8 *Motion_SetSpeed_6();
u8 *Motion_SetSpeed_7();
u8 *Scene_GetRecord_10();
u8 *Scene_GetRecord_11();

/* Field at 0x1c8 of the shared scene work record. */

/* Field at 0x1c0 of the shared scene work record. */

s32 Scene_DispatchBySceneId(void)
{
    extern s16 gCell[];

    s32 v;

    Actor_Do20(0x87a);
    v = gCell[224];
    if (v == (s32)&Value_00000063) {
        Actor_Run62();
    } else if (v == (s32)&Value_00000066) {
        Actor_Run63();
    } else if (v == (s32)&Value_00000099) {
        Actor_Run64();
    } else if (v == (s32)&Value_0000009b) {
        Actor_Run65();
    } else if (v == (s32)&Value_0000009c) {
        Actor_Run66();
    }
    return 0;
}

void Scene_RunMiddleSequence(void)
{
    extern u8 gCell[];

    u32 i;
    s32 rec7;
    u8 *record;
    s32 r0;
    s32 v5;
    u8 *p5;
    u8 *volatile *base = (u8 *volatile *)gWork;

    *(s32 *)(base[0] + 0x1c0) = 0x204;
    Actor_Run67(0);
    rec7 = Actor_Check(0x109);
    if (rec7 != 0) {
        p5 = base[9];
        r0 = Actor_Check2(0x200);
        if (r0 != 0) {
            r0 = Actor_Check3(0);
        }
        *(volatile s32 *)(p5 + 24) = r0;
    } else {
        Actor_Do(0x200);
        if (gCell2[225][0] == 4) {
            *(volatile s32 *)(base[9] + 24) = rec7;
            Actor_Do2(0x200);
        }
    }
    if (Actor_Check4(0x302) != 0) {
        Actor_Place(11, 0x960000, 0x2d80000);
        if (Actor_Check5(0x201) != 0) {
            Actor_Run68(11);
            v5 = 9;
            Actor_Run69(11, 5);
            Actor_SetRect(0, 0, 1, 1, v5, 14);
            Actor_SetRect2(0, 0, 1, 1, v5, 45);
            {
                u8 *record = Actor_Run56(11);
                u8 value = *(volatile u8 *)&record[35];

                record[35] = (u8)(value | 2);
            }
        }
    }
    record = Actor_Run57(8);
    Actor_Run70((s32)record, 0);
    record = Actor_Run58(9);
    Actor_Run71((s32)record, 0);
    Actor_Run(0x200b769, 0xc80);
    if (Actor_Check6(0x915) != 0) {
        Actor_Place2(10, 0x1aa0000, 0x2da0000);
        record = Actor_Run59(10);
        {
            s32 shown = 0x5000;

            *(volatile u16 *)((s32)record + 6) = shown;
        }
        Actor_SetRect3(88, 48, 88, 45, 2, 3);
        Actor_SetRect4(24, 49, 24, 48, 2, 1);
        Actor_SetRect5(25, 42, 25, 47, 1, 1);
        Actor_SetRect6(22, 50, 2, 1, 24, 49);
    }
    if (Actor_Check7(0x302) == 0) {
    } else {
        Actor_Place3(8, 0xe80000, 0x2dc0000);
        Actor_SetRect7(7, 44, 1, 1, 0, 1);
        Actor_SetRect8(74, 58, 78, 41, 1, 5);
        Actor_SetRect9(16, 109, 13, 109, 3, 2);
        Actor_SetRect10(67, 64, 71, 44, 1, 2);
        Actor_SetRect11(67, 64, 72, 44, 1, 2);
        Actor_SetRect12(67, 68, 73, 43, 1, 2);
        Actor_SetRect13(67, 68, 74, 43, 1, 2);
        Actor_SetRect14(67, 64, 75, 44, 1, 2);
        Actor_SetRect15(67, 66, 76, 44, 1, 2);
        Actor_SetRect16(67, 64, 77, 44, 1, 2);
        Actor_SetRect17(67, 64, 78, 44, 1, 2);
        Actor_SetRect18(67, 64, 79, 44, 1, 2);
        Actor_SetRect19(67, 66, 80, 44, 1, 2);
        Actor_SetRect20(2, 0, 9, 42, 2, 2);
        Actor_SetRect21(68, 64, 71, 44, 1, 2);
        Actor_SetRect22(68, 64, 72, 44, 1, 2);
        Actor_SetRect23(68, 68, 73, 43, 1, 2);
        Actor_SetRect24(68, 68, 74, 43, 1, 2);
        Actor_SetRect25(68, 64, 75, 44, 1, 2);
        Actor_SetRect26(68, 66, 76, 44, 1, 2);
        Actor_SetRect27(68, 64, 77, 44, 1, 2);
        Actor_SetRect28(68, 64, 78, 44, 1, 2);
        Actor_SetRect29(68, 64, 79, 44, 1, 2);
        Actor_SetRect30(68, 66, 80, 44, 1, 2);
        Actor_SetRect31(4, 0, 9, 42, 2, 2);
        Actor_SetRect32(7, 11, 7, 42, 10, 8);
        Actor_SetRect33(71, 12, 71, 43, 10, 13);
        Actor_SetRect34(6, 13, 12, 12, 6, 44);
        Actor_SetRect35(0, 1, 1, 1, 7, 44);
        goto L_02001cc2;
    }
    switch (gCell2[225][0]) {
    case 1:
    case 2:
        Actor_Run72(170);
        break;
    }
    L_02001cc2:;
    if (Actor_Check8(0x303) == 0) {
    } else {
        Actor_Place4(9, 0x2b80000, 0x2dc0000);
        Actor_SetRect36(74, 58, 107, 41, 1, 5);
        Actor_SetRect37(45, 109, 42, 109, 3, 2);
        Actor_SetRect38(67, 64, 102, 44, 1, 2);
        Actor_SetRect39(67, 64, 103, 44, 1, 2);
        Actor_SetRect40(67, 64, 104, 44, 1, 2);
        Actor_SetRect41(67, 66, 105, 44, 1, 2);
        Actor_SetRect42(67, 64, 106, 44, 1, 2);
        Actor_SetRect43(67, 64, 107, 44, 1, 2);
        Actor_SetRect44(67, 64, 108, 44, 1, 2);
        Actor_SetRect45(67, 66, 109, 44, 1, 2);
        Actor_SetRect46(68, 64, 102, 44, 1, 2);
        Actor_SetRect47(68, 64, 103, 44, 1, 2);
        Actor_SetRect48(68, 64, 104, 44, 1, 2);
        Actor_SetRect49(68, 66, 105, 44, 1, 2);
        Actor_SetRect50(68, 64, 106, 44, 1, 2);
        Actor_SetRect51(68, 64, 107, 44, 1, 2);
        Actor_SetRect52(68, 64, 108, 44, 1, 2);
        Actor_SetRect53(68, 66, 109, 44, 1, 2);
        Actor_SetRect54(38, 14, 38, 44, 8, 4);
        Actor_SetRect55(102, 14, 102, 44, 8, 12);
        Actor_SetRect56(37, 13, 10, 12, 37, 43);
        goto L_02001e5a;
    }
    switch (gCell2[225][0]) {
    case 3:
    case 4:
        Actor_Run73(170);
        break;
    }
    L_02001e5a:;
}

/* Runs a scripted sequence for two actors (8 and 9): sets up their sprite
 * records, moves and animates them in lockstep through a series of timed
 * steps, then hands off to a third actor (2) and a couple of standalone
 * calls (5) before advancing the shared scene step counter and phase word. */
void Scene_RunTwoActorCutsceneSequence(void)
{
    u32 i;
    u8 *record;

    Battle_Reset_1();
    Motion_CamBounds_1(-1, -1, -1, 0);
    Actor_Run74(1);
    Motion_SetHPosTerrain_1(0, 0xc00000, 0x1560000);
    Actor_Run75(1);
    Motion_SetSpeedLim_1(0x3333, 0x666);
    Motion_CamBounds_2(0xc00000, -1, 0xfc0000, 1);
    SCENE_FIELD_1C8 = 40;
    Battle_WaitMode0_1();
    Motion_SetSpeed_1(0, 0xcccc, 0x6666);
    Motion_SetPosReset_1(0, 192, 0x116);
    Motion_SetSpeed_2(8, 0x10000, 0x8000);
    Motion_SetSpeed_3(9, 0x10000, 0x8000);
    record = Scene_GetRecord_1(8);
    {
        /* Field at +6 of the record: a visibility/state word. */
        s32 shown = 0x3000;

        *(u16 *)(record + 6) = shown;
    }
    record = Scene_GetRecord_2(9);
    {
        /* Field at +6 of the record: a visibility/state word. */
        s32 shown = 0x5000;

        *(u16 *)(record + 6) = shown;
    }
    Actor_Run76(1);
    /* Clear bit 0 of the flag byte at +90. */
    *(u8 *)(Scene_GetRecord_3(8) + 90) &= 254;
    *(u8 *)(Scene_GetRecord_4(9) + 90) &= 254;
    Motion_ResetPosMode2_1(8, 184, 232);
    Motion_SetPosReset_2(9, 198, 232);
    Object_SetModeById_1(8, 1);
    Battle_WaitMode0_2(20);
    {
        /* Set bit 0 of the flag byte at +90. */
        u8 *record = Scene_GetRecord_5(8);
        u8 flags = *(volatile u8 *)&record[90];

        record[90] = (u8)(flags | 1);
    }
    {
        u8 *record = Scene_GetRecord_6(9);
        u8 flags = *(volatile u8 *)&record[90];

        record[90] = (u8)(flags | 1);
    }
    Battle_WaitMode0_3(20);
    Motion_CallWaitAnim_1(8, 4);
    SceneWork_SetStepValue_1(0x1b05);
    Battle_RunThenWaitIfModeZero_1(8, 0, 10);
    Motion_ArmCb_1(9, 0x5000, 10);
    Motion_CallWaitAnim_2(9, 3);
    Battle_RunThenWaitIfModeZero_2(9, 0, 10);
    Battle_WaitMode0_4(0, 0x102);
    Battle_WaitMode0_5(60);
    record = Scene_GetRecord_7(0);
    if (record != 0) {
        Motion_SetHPosTerrain_2(2, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    Motion_SetSpeed_4(2, 0xcccc, 0x6666);
    Motion_SetPosReset_3(2, 212, 0x10c);
    Motion_ArmCb_2(0, 0xe000, 0);
    Motion_ArmCb_3(2, 0x6000, 20);
    Battle_RunThenWaitIfModeZero_3(0x4002, 0, 20);
    Motion_ArmCb_4(0, 0xc000, 0);
    Motion_SetPosReset_4(2, 202, 254);
    Battle_WaitMode0_6(20);
    Motion_CallWaitAnim_3(2, 3);
    Battle_WaitMode0_7(10);
    Battle_RunThenWaitIfModeZero_4(2, 0, 10);
    BattleFx_SpawnLinked_1(8, 0x101, 0);
    BattleFx_SpawnLinked_2(9, 0x101, 40);
    Motion_ArmCb_5(8, 0, 0);
    Motion_ArmCb_6(9, 0x8000, 40);
    Motion_ArmCb_7(8, 0x3000, 0);
    Motion_ArmCb_8(9, 0x3000, 20);
    BattleFx_SpawnLinked_3(2, 0x102, 60);
    Motion_SetVarCbObj_1(2, 1);
    Battle_RunThenWaitIfModeZero_5(2, 0, 10);
    Motion_SetVarCbObj_2(8, 2);
    Battle_RunThenWaitIfModeZero_6(8, 0, 10);
    BattleFx_SpawnLinked_4(8, 0x105, 60);
    Motion_SetVarCb_1(8, 2);
    Battle_RunThenWaitIfModeZero_7(8, 0, 10);
    Motion_SetVarCbObj_3(9, 2);
    Battle_RunThenWaitIfModeZero_8(9, 0, 20);
    Motion_SetVarCbObj_4(2, 1);
    Battle_WaitMode0_8(10);
    Motion_ArmCb_9(2, 0x6000, 10);
    BattleEventRuntime_ProcessAction_1(0x4002, 0);
    Battle_WaitMode0_9(10);
    Motion_SetVarCbObj_5(8, 1);
    Motion_CallWaitAnim_4(8, 4);
    Battle_RunThenWaitIfModeZero_9(8, 0, 10);
    BattleFx_SpawnLinked_5(2, 0x101, 40);
    Motion_ArmCb_10(2, 0xa000, 20);
    Motion_SetVarCbObj_6(9, 1);
    Motion_CallWaitAnim_5(9, 3);
    Motion_ArmCb_11(2, 0xc000, 0);
    Battle_RunThenWaitIfModeZero_10(9, 0, 10);
    Motion_ArmCb_12(2, 0x6000, 60);
    Motion_ArmCb_13(2, 0xc000, 10);
    Battle_RunThenWaitIfModeZero_11(2, 0, 10);
    Motion_ArmCb_14(8, 0, 0);
    Motion_ArmCb_15(9, 0x8000, 40);
    Motion_ArmCb_16(8, 0x3000, 0);
    Motion_ArmCb_17(9, 0x3000, 10);
    Motion_SetVarCbObj_7(8, 1);
    Motion_SetSpeed_5(8, 0);
    Motion_ArmCb_18(2, 0x6000, 0);
    if (UiWork_WaitThenFinalizeCapacity_1(0, 0) == 0) {
        Motion_SetVarCbObj_8(2, 2);
        Battle_RunThenWaitIfModeZero_12(0x4002, 0, 10);
        bump_step(1);
    } else {
        bump_step(1);
        Motion_CallWaitAnim_6(2, 4);
        Battle_RunThenWaitIfModeZero_13(0x4002, 0, 10);
    }
    Motion_SetVarCbObj_9(9, 2);
    Battle_RunThenWaitIfModeZero_14(9, 0, 10);
    Motion_ArmCb_19(2, 0xc000, 10);
    Motion_CallWaitAnim_7(2, 4);
    Battle_RunThenWaitIfModeZero_15(2, 0, 10);
    Battle_WaitMode0_10(8, 0x102);
    Battle_WaitMode0_11(60);
    Battle_RunThenWaitIfModeZero_16(8, 0, 10);
    Motion_ArmCb_20(2, 0xa000, 10);
    Motion_CallWaitAnim_8(2, 3);
    Battle_RunThenWaitIfModeZero_17(2, 0, 10);
    Battle_WaitMode0_12(8, 0x102);
    Battle_WaitMode0_13(9, 0x102);
    Battle_WaitMode0_14(60);
    Motion_SetVarCbObj_10(8, 2);
    Battle_RunThenWaitIfModeZero_18(8, 0, 10);
    Motion_SetVarCbObj_11(9, 2);
    Motion_ArmCb_21(9, 0x8000, 10);
    Battle_RunThenWaitIfModeZero_19(9, 0, 10);
    Motion_ArmCb_22(8, 0, 10);
    Motion_CallWaitAnim_9(8, 3);
    Motion_ArmCb_23(8, 0x3000, 10);
    BattleEv_RunWait_1(8, 0);
    Motion_ArmCb_24(9, 0x3000, 10);
    Motion_CallWaitAnim_10(2, 3);
    Motion_ArmCb_25(2, 0x6000, 10);
    BattleEventRuntime_ProcessAction_2(0x4002, 0);
    Battle_WaitMode0_15(10);
    Motion_SetVarCbObj_12(2, 1);
    Battle_RunThenWaitIfModeZero_20(0x4002, 0, 10);
    Motion_CallWaitAnim_11(0, 3);
    Motion_CallWaitAnim_12(2, 3);
    Motion_ArmCb_26(2, 0xc000, 10);
    Motion_CallWaitAnim_13(2, 3);
    Actor_Place5(8, 0xcccc, 0x6666);
    Actor_Place6(9, 0xcccc, 0x6666);
    record = Motion_SetSpeed_6(8);
    {
        /* Field at +6 of the record: a visibility/state word. */
        s32 shown = 0;

        *(u16 *)(record + 6) = shown;
    }
    record = Motion_SetSpeed_7(9);
    {
        s32 shown = 0x8000;

        *(u16 *)(record + 6) = shown;
    }
    /* Clear bit 0 of the flag byte at +90. */
    *(u8 *)(Scene_GetRecord_8(8) + 90) &= 254;
    *(u8 *)(Scene_GetRecord_9(9) + 90) &= 254;
    Motion_ResetPosMode2_2(8, 168, 232);
    Motion_SetPosReset_5(9, 212, 232);
    Object_SetModeById_2(8, 1);
    Battle_WaitMode0_16(20);
    {
        /* Set bit 0 of the flag byte at +90. */
        u8 *record = Scene_GetRecord_10(8);
        u8 flags = *(volatile u8 *)&record[90];

        record[90] = (u8)(flags | 1);
    }
    {
        u8 *record = Scene_GetRecord_11(9);
        u8 flags = *(volatile u8 *)&record[90];

        record[90] = (u8)(flags | 1);
    }
    Motion_SetPosReset_6(2, 192, 232);
    Motion_ArmCb_27(2, 0xc000, 0);
    Audio_PlayCue_1(188);
    Actor_SetRect57(36, 23, 43, 12, 2, 2);
    Actor_Run77(5);
    Actor_SetRect58(39, 23, 43, 12, 2, 2);
    Actor_Run78(5);
    Motion_SetPosReset_7(2, 192, 222);
    Motion_SetHPosTerrain_3(2, 0, 0);
    Motion_SetSpeed_8(0, 0x13333, 0x9999);
    Motion_SetPosReset_8(0, 192, 222);
    Motion_SetHPosTerrain_4(0, 0, 0);
    Object_SetModeById_3(8, 3);
    Motion_CallWaitAnim_14(9, 3);
    Motion_ResetPosMode2_3(8, 184, 232);
    Motion_SetPosReset_9(9, 198, 232);
    Motion_ResetPosMode2_4(8, 188, 212);
    Motion_SetPosReset_10(9, 194, 212);
    Motion_SetHPosTerrain_5(8, 0, 0);
    Motion_SetHPosTerrain_6(9, 0, 0);
    SCENE_FIELD_1C8 = 24;
    SCENE_FIELD_1C0 = 0x201;
    Actor_Run79();
    Motion_SetSpeed_9();
    Actor_Run80(5);
    Battle_SchedShoulder_1();
}

void Scene_RunLateSequence(void)
{
    s32 tmp[3];
    s32 record;
    s32 rec;
    s32 idx;
    s32 w;
    s32 w2;
    s32 a;
    s32 *dst;
    s32 zero;
    s32 k;

    record = Actor_Check9(0);
    idx = (s32)((u32)*(u16 *)(record + 6) >> 12);
    a = *(s16 *)(record + 10);
    w = gOv23[idx];
    rec = Actor_Check10((a + (w >> 16)) >> 4, (*(s16 *)(record + 18) + (s32)(s16)w) >> 4);
    if (rec != 0) {
        zero = 0;
        *(u8 *)(rec + 34) = 2;
        dst = tmp;
        w2 = gOv23[idx];
        dst[0] = *(volatile s32 *)(rec + 8) + (w2 & -0x10000);
        dst[1] = *(volatile s32 *)(rec + 12);
        dst[2] = *(volatile s32 *)(rec + 16) + (w2 << 16);
        if (Actor_Check11(rec, (s32)dst) <= 0) {
            Actor_Run81(record, 8);
            k = 0x3333;
            Actor_Run82(15);
            Actor_Run83(185);
            *(volatile s32 *)(rec + 48) = k;
            *(volatile s32 *)(rec + 52) = k;
            Actor_Run84(rec, dst[0], dst[1], dst[2]);
            *(volatile s32 *)(record + 48) = k;
            *(volatile s32 *)(record + 52) = k;
            Actor_Run85(record, dst[0], dst[1], dst[2]);
            Actor_Run86(rec);
            Actor_Run87();
            *(volatile s32 *)(rec + 8) = dst[0];
            *(volatile s32 *)(rec + 16) = dst[2];
            *(s32 *)(rec + 36) = zero;
            *(s32 *)(rec + 44) = zero;
            Actor_Run88(record, 1);
            Actor_Run89();
        }
    }
}

void Actor_Run90(void)
{
    s32 rec8;
    s32 rec4;
    s32 rec7;
    s32 rec2;
    u32 i;
    s32 v5;
    s32 v6;
    s32 v7;

    rec8 = Actor_Check12(8);
    rec4 = Actor_Check13(9);
    rec7 = Actor_Check14(0x302);
    if (rec7 != 0) {
    } else if ((*(volatile s32 *)(rec8 + 8) >> 19) > 29) {
    } else {
        rec2 = Actor_Check15(11);
        Actor_Run91();
        Actor_Run92(7, 44, 1, 1, rec7, 1);
        i = 67;
        v7 = 1;
        v6 = 5;
        do {
            Actor_Run93(i, 58, 78, 41, v7, v6);
            Actor_Run94(4);
            if (i == 70) {
                Actor_Run95(0x302);
            }
            i++;
        } while (i <= 74);
        v5 = 2;
        Actor_Run96(16, 109, 13, 109, 3, v5);
        Actor_Run97(40);
        *(s32 *)(rec2 + 24) = 0x1999;
        *(s32 *)(rec2 + 28) = 0x1999;
        Actor_Place7(11, 0x960000, 0x2d80000);
        Actor_Check16(11, 0x200c268);
        v6 = 1;
        Actor_Run98(67, 64, 71, 44, v6, v5);
        Actor_Run99(67, 64, 72, 44, v6, v5);
        Actor_Run100(67, 68, 73, 43, v6, v5);
        Actor_Run101(67, 68, 74, 43, v6, v5);
        Actor_Run102(67, 64, 75, 44, v6, v5);
        Actor_Run103(67, 66, 76, 44, v6, v5);
        Actor_Run104(67, 64, 77, 44, v6, v5);
        Actor_Run105(67, 64, 78, 44, v6, v5);
        Actor_Run106(67, 64, 79, 44, v6, v5);
        Actor_Run107(67, 66, 80, 44, v6, v5);
        Actor_Run108(2, 0, 9, 42, v5, v5);
        Actor_Run109(40);
        Actor_Run110(68, 64, 71, 44, v6, v5);
        Actor_Run111(68, 64, 72, 44, v6, v5);
        Actor_Run112(68, 68, 73, 43, v6, v5);
        Actor_Run113(68, 68, 74, 43, v6, v5);
        Actor_Run114(68, 64, 75, 44, v6, v5);
        Actor_Run115(68, 66, 76, 44, v6, v5);
        Actor_Run116(68, 64, 77, 44, v6, v5);
        Actor_Run117(68, 64, 78, 44, v6, v5);
        Actor_Run118(68, 64, 79, 44, v6, v5);
        Actor_Run119(68, 66, 80, 44, v6, v5);
        Actor_Run120(4, 0, 9, 42, v5, v5);
        Actor_Run121(40);
        v5 = 10;
        Actor_Run122(7, 11, 7, 42, v5, 8);
        Actor_Run123(71, 12, 71, 43, v5, 13);
        v5 = 44;
        Actor_Run124(6, 13, 12, 12, 6, v5);
        Actor_Run125(40);
        Actor_Run126();
        Actor_Run127(0, 1, 1, 1, 7, v5);
        Actor_Run128();
    }
    if (Actor_Check17(0x303) != 0) {
    } else if ((*(volatile s32 *)(rec4 + 8) >> 19) > 87) {
    } else {
        Actor_Run129();
        i = 67;
        v7 = 1;
        v6 = 5;
        do {
            Actor_Run130(i, 58, 107, 41, v7, v6);
            Actor_Run131(4);
            if (i == 70) {
                Actor_Run132(0x303);
            }
            i++;
        } while (i <= 74);
        v6 = 2;
        Actor_Run133(45, 109, 42, 109, 3, v6);
        Actor_Run134(40);
        v5 = 1;
        Actor_Run135(67, 64, 102, 44, v5, v6);
        Actor_Run136(67, 64, 103, 44, v5, v6);
        Actor_Run137(67, 64, 104, 44, v5, v6);
        Actor_Run138(67, 66, 105, 44, v5, v6);
        Actor_Run139(67, 64, 106, 44, v5, v6);
        Actor_Run140(67, 64, 107, 44, v5, v6);
        Actor_Run141(67, 64, 108, 44, v5, v6);
        Actor_Run142(67, 66, 109, 44, v5, v6);
        Actor_Run143(40);
        Actor_Run144(68, 64, 102, 44, v5, v6);
        Actor_Run145(68, 64, 103, 44, v5, v6);
        Actor_Run146(68, 64, 104, 44, v5, v6);
        Actor_Run147(68, 66, 105, 44, v5, v6);
        Actor_Run148(68, 64, 106, 44, v5, v6);
        Actor_Run149(68, 64, 107, 44, v5, v6);
        Actor_Run150(68, 64, 108, 44, v5, v6);
        Actor_Run151(68, 66, 109, 44, v5, v6);
        Actor_Run152(40);
        v5 = 8;
        Actor_Run153(38, 14, 38, 44, v5, 4);
        Actor_Run154(102, 14, 102, 44, v5, 12);
        Actor_SetRect59(37, 13, 10, 12, 37, 43);
        Actor_Run155(40);
        Actor_Run156();
        Actor_Run157();
    }
}

void Scene_RunSupplementalSequenceOne(s32 a0)
{
    s32 rec7;
    s32 rec8;
    s32 record;
    s32 mask;
    s32 c12;
    s32 old;
    u8 *rp;
    u8 *p4;

    rec7 = Actor_Check18(8);
    rec8 = Actor_Check44(9);
    if ((u32)(*(s16 *)(rec7 + 10) + -0x17d) <= 12) {
        if (*(s16 *)(rec7 + 18) <= 0x309) {
            goto L_020037ae;
        }
        record = Actor_Check19(0);
        p4 = (u8 *)*(s32 *)(rec7 + 80);
        rp = (u8 *)*(s32 *)(record + 80);
        c12 = 12 & rp[9];
        old = p4[9];
        mask = -13;
        mask &= old;
        p4[9] = (mask | c12);
    } else {
        L_020037ae:;
        if (Actor_Check20(0x302) == 0) {
            if (*(s16 *)(rec7 + 10) <= 245) {
                if ((*(volatile s32 *)0x03001e40 & 1) == 0) {
                    if (Actor_Check21(0x202) == 0) {
                        Actor_Do3(-1);
                        Actor_Run158(230);
                        Actor_Do4(0x202);
                    }
                    Actor_Run159(*(volatile s32 *)(rec7 + 8), *(volatile s32 *)(rec7 + 12), *(volatile s32 *)(rec7 + 16));
                }
            }
        }
    }
    if (Actor_Check22(0x303) == 0) {
        if (*(s16 *)(rec8 + 10) <= 0x2c5) {
            if ((*(volatile s32 *)0x03001e40 & 1) == 0) {
                if (Actor_Check23(0x203) == 0) {
                    Actor_Do5(-1);
                    Actor_Run160(230);
                    Actor_Do6(0x203);
                }
                Actor_Run161(*(volatile s32 *)(rec8 + 8), *(volatile s32 *)(rec8 + 12), *(volatile s32 *)(rec8 + 16));
            }
        }
    }
}

void Actor_Run162(s32 a0, s32 a1, s32 a2)
{
    s32 p8;
    u8 *rec7;
    s32 value;
    s32 mask;
    u8 *link;

    p8 = a2;
    value = Actor_Run2();
    rec7 = (u8 *)Actor_Check24(222, (a0 + -0x80000), (((((u32)(value << 3) >> 16) << 16) + a1) + 0x100000), p8);
    if ((s32)rec7 != 0) {
        rec7[85] = (mask = 0);
        link = (u8 *)*(volatile s32 *)((s32)rec7 + 80);
        mask -= 13;
        link[9] = ((link[9] & mask) | 8);
        Actor_Run163((s32)rec7, 9);
        Actor_Run164((s32)rec7, 0);
        value = Actor_Check45();
        *(volatile s32 *)((s32)rec7 + 36) = ((((u32)(value << 1) >> 16) - 1) << 16);
        value = Actor_Run3();
        *(volatile s32 *)((s32)rec7 + 40) = ((((u32)(((value << 1) + value) << 1) >> 16) - 3) << 16);
        {
            volatile u16 *target = (volatile u16 *)((s32)rec7 + 100);
            s32 shown = 20;

            *target = shown;
            *((u8 *)target - 3) = 1;
        }
        Actor_Run165((s32)rec7, 1);
        Actor_Run166((s32)rec7, 0x200d120);
    }
}

/* overlays/scene/actor/staged_group/scene_script.c */

struct SceneWork {
    u8 unknown_000[0x1c0];
    s32 request;
    u8 unknown_1c4[4];
    s32 setup;
    u8 unknown_1cc[12];
    u16 step;
};

extern struct SceneWork *gWork;
extern const u8 gOv24[];
extern const u8 gOv25[];

u8 *Actor_Run21();

u8 *Actor_Run167();

void Scene_RunSecondaryGroupSequence(void)
{
    u8 *fieldActor;
    u8 *object;
    struct SceneWork *work;
    u32 random;
    u32 motionPhase;
    const u8 *motionActions;
    s32 scale;
    const u8 *exitActions;

    Actor_Run22();
    Actor_Run4(-1, -1, -1, 0);
    Actor_Run32(1);
    Actor_Far(0, 0, 0);
    Actor_Place8(9, 0x1b80000, 0x20a0000);
    object = Actor_Run21(9);
    Actor_Run168(object, 0);
    Actor_Run4(0x1b80000, -1, 0x20a0000, 0);
    Actor_Run169();
    Actor_Run32(1);
    Actor_Run170();
    Actor_Run5(0x19999, 0x3333);
    Actor_Run4(0x1b80000, -1, 0x1900000, 1);
    Actor_Run24(141);
    Actor_Place9(9, 0x19999, 0xcccc);
    Actor_Place10(9, 0x1b8, 0x190);
    Actor_Run5(0xc000, 0x1800);
    Actor_Run4(0x1b80000, -1, 0x12c0000, 1);
    Actor_Place9(9, 0xcccc, 0x6666);
    Actor_Place10(9, 0x1b8, 0x12c);
    Actor_Run35(9, 0);
    Actor_Do7(0x121);
    Actor_Run171(40);
    Actor_Place9(11, 0xcccc, 0x6666);
    Actor_Place8(11, 0x1b70000, 0x1320000);
    Actor_Run37(11, 4, 0);
    Actor_Place10(11, 0x1b7, 0x138);
    Actor_Place10(11, 0x1a0, 0x138);
    Actor_Place10(11, 0x190, 0x100);
    Actor_Place11(11, 0x3000, 40);
    Actor_Run4(0x19a0000, -1, 0x1180000, 1);
    Actor_Place9(10, 0x9999, 0x4ccc);
    Actor_Place8(10, 0x1b70000, 0x1320000);
    Actor_Run37(10, 4, 0);
    Actor_Place10(10, 0x1b7, 0x138);
    Actor_Place10(10, 0x1a0, 0x138);
    Actor_Place10(10, 0x184, 0x10e);
    Actor_Place11(10, 0xd000, 10);
    Actor_Place11(11, 0x5000, 10);
    Actor_Place9(0, 0xcccc, 0x6666);
    Actor_Place8(0, 0x1b70000, 0x1320000);
    Actor_Run37(0, 4, 0);
    Actor_Place10(0, 0x1b7, 0x138);
    Actor_Place10(0, 0x1a0, 0x138);
    Actor_Place10(0, 0x184, 0x12c);
    Actor_Place11(0, 0xc000, 40);
    fieldActor = Actor_Run21(0);
    random = Actor_Run172();
    motionPhase = random * 5;
    motionActions = gOv24;
    fieldActor += 102;
    *(u16 *)fieldActor = motionPhase >> 12;
    Actor_Run29(0, motionActions);
    Actor_Run37(11, 2, 20);
    Actor_Run36(11, 3);
    Actor_Do8(0x256f);
    Actor_Run34(11, 0, 10);
    Actor_Run36(10, 3);
    object = Actor_Run21(0);
    if (object != 0) {
        Actor_Far(1, *(s32 *)(object + 8), *(s32 *)(object + 16));
    }
    object = Actor_Run21(0);
    if (object != 0) {
        Actor_Far(2, *(s32 *)(object + 8), *(s32 *)(object + 16));
    }
    object = Actor_Run21(0);
    if (object != 0) {
        Actor_Far(3, *(s32 *)(object + 8), *(s32 *)(object + 16));
    }
    Actor_Place9(1, 0xcccc, 0x6666);
    Actor_Place9(2, 0xcccc, 0x6666);
    Actor_Place9(3, 0xcccc, 0x6666);
    Actor_Place12(1, 0x17a, 0x136);
    Actor_Place12(2, 0x190, 0x120);
    Actor_Place10(3, 0x19a, 0x134);
    Actor_Run35(1, 1);
    Actor_Run35(2, 1);
    Actor_Place11(1, 0xc000, 0);
    Actor_Place11(2, 0xc000, 0);
    Actor_Place11(3, 0xc000, 40);
    fieldActor = Actor_Run21(1);
    random = Actor_Run172();
    fieldActor += 102;
    *(u16 *)fieldActor = ((random * 5) >> 12);
    fieldActor = Actor_Run21(2);
    random = Actor_Run172();
    fieldActor += 102;
    *(u16 *)fieldActor = ((random * 5) >> 12);
    fieldActor = Actor_Run21(3);
    random = Actor_Run172();
    fieldActor += 102;
    *(u16 *)fieldActor = ((random * 5) >> 12);
    Actor_Run29(1, motionActions);
    Actor_Run29(2, motionActions);
    Actor_Run29(3, motionActions);
    Actor_Place13(2, 0x101, 60);
    Actor_Place14(0x2002, 0, 10);
    Actor_Place13(10, 0x100, 40);
    Actor_Place11(10, 0x3000, 20);
    Actor_Run34(10, 0, 10);
    Actor_Run41(0);
    Actor_Run41(1);
    Actor_Run41(2);
    Actor_Run41(3);
    Actor_Run32(1);
    Actor_Place11(2, 0xc000, 10);
    Actor_Run36(2, 3);
    Actor_Place11(0, 0xc000, 0);
    Actor_Place11(1, 0xc000, 0);
    Actor_Place11(3, 0xc000, 20);
    Actor_Run36(10, 3);
    Actor_Run34(10, 0, 10);
    Actor_Place13(3, 0x101, 60);
    Actor_Place14(0x2003, 0, 10);
    Actor_Run36(10, 3);
    Actor_Run34(10, 0, 10);
    Actor_Place13(1, 0x101, 60);
    Actor_Run34(1, 0, 10);
    Actor_Place11(10, 0x5000, 0);
    Actor_Run28(11, 2);
    Actor_Run34(11, 0, 10);
    Actor_Place11(0, 0x6000, 0);
    Actor_Place11(1, 0xe000, 0);
    Actor_Place11(2, 0x2000, 0);
    Actor_Place11(3, 0xa000, 20);
    Actor_Run35(0, 3);
    Actor_Run35(1, 3);
    Actor_Run35(2, 3);
    Actor_Run36(3, 3);
    Actor_Run171(10);
    Actor_Place11(0, 0xc000, 0);
    Actor_Place11(1, 0xc000, 0);
    Actor_Place11(2, 0xc000, 0);
    Actor_Place11(3, 0xc000, 10);
    Actor_Place11(10, 0xd000, 10);
    Actor_Run36(10, 3);
    Actor_Run34(10, 0, 10);
    Actor_Run36(11, 3);
    Actor_Run6(2, 0x102);
    Actor_Run171(40);
    Actor_Run38(2, 2);
    Actor_Place14(0x2002, 0, 10);
    Actor_Place11(11, 0x3000, 10);
    Actor_Run35(11, 4);
    Actor_Run34(11, 0, 10);
    Actor_Place13(3, 0x101, 40);
    Actor_Place14(0x2003, 0, 10);
    Actor_Place11(10, 0x3000, 10);
    Actor_Place13(10, 0x108, 20);
    Actor_Run34(10, 0, 10);
    Actor_Run28(1, 2);
    Actor_Run34(1, 0, 10);
    Actor_Place11(11, 0x5000, 0);
    Actor_Place11(10, 0x5000, 10);
    Actor_Run35(11, 4);
    Actor_Run34(11, 0, 10);
    Actor_Place11(0, 0x6000, 0);
    Actor_Place11(1, 0xe000, 40);
    Actor_Place11(0, 0xc000, 0);
    Actor_Place11(1, 0xc000, 10);
    Actor_Run36(11, 4);
    Actor_Run34(11, 0, 10);
    Actor_Run36(10, 4);
    Actor_Run34(10, 0, 10);
    Actor_Place13(0, 0x105, 0);
    Actor_Place13(1, 0x105, 0);
    Actor_Place13(2, 0x105, 0);
    Actor_Place13(3, 0x105, 60);
    Actor_Place11(10, 0x3000, 10);
    Actor_Run35(10, 3);
    Actor_Check25(10, 0);
    Actor_Place11(1, 0xe000, 0);
    Actor_Place11(2, 0x6000, 0);
    Actor_Place11(3, 0xa000, 0);
    if (Actor_Check26(0, 0) == 0) {
        Actor_Run171(20);
        gWork->step += 3;
    } else {
        Actor_Run171(20);
        Actor_Run28(11, 2);
        Actor_Run34(11, 0, 40);
        Actor_Run34(11, 0, 10);
        Actor_Place13(3, 0x106, 40);
        Actor_Place11(3, 0xc000, 20);
        Actor_Place14(0x2003, 0, 10);
    }
    Actor_Place11(1, 0xc000, 0);
    Actor_Place11(2, 0xc000, 0);
    Actor_Place11(3, 0xc000, 10);
    Actor_Run35(1, 3);
    Actor_Run35(2, 3);
    Actor_Run36(3, 3);
    Actor_Run171(20);
    Actor_Place11(8, 0x3000, 0);
    Actor_Run36(10, 3);
    Actor_Run5(0x10000, 0x2000);
    Actor_Run4(0x1180000, -1, 0xc80000, 1);
    Actor_Place10(10, 0x14d, 222);
    Actor_Place10(10, 0x11c, 198);
    Actor_Place11(10, 0x8000, 10);
    Actor_Run36(10, 3);
    Actor_Far3(8, 0, 10);
    Actor_Run36(8, 3);
    Actor_Place9(8, 0xcccc, 0x6666);
    Actor_Place10(8, 0x10c, 198);
    Actor_Place11(8, 0xc000, 10);
    Actor_Run28(8, 2);
    Actor_Run24(125);
    Actor_SetRect60(71, 60, 76, 11, 2, 1);
    Actor_SetRect61(71, 60, 2, 1, 16, 11);
    Actor_Run171(20);
    Actor_Far7(8, 246, 198);
    Actor_Place11(8, 0, 20);
    *(u8 *)(Actor_Run167() + 85) = 0;
    Actor_Run5(0x9999, 0x1333);
    Actor_Run4(0xf80000, -1, 0xaa0000, 1);
    Actor_Place10(10, 0x10e, 198);
    Actor_Place10(10, 0x10e, 174);
    Actor_Far7(10, 224, 170);
    Actor_Far7(10, 210, 158);
    Actor_Far7(10, 246, 148);
    Actor_Far7(10, 246, 142);
    Actor_Far(10, 0, 0);
    gWork->request = 514;
    Actor_Run48();
    Actor_Run49();
    Actor_Place8(9, 0x1b80000, 0x1540000);
    object = Actor_Run21(9);
    scale = 0x4000;
    *(u16 *)(object + 6) = scale;
    Actor_Run4(0x17c0000, -1, 0x1180000, 0);
    Actor_Run169();
    Actor_Run32(10);
    Actor_Run170();
    Actor_Run49();
    Actor_Do9(40);
    Actor_Run36(11, 3);
    Actor_Run171(20);
    Actor_Run35(0, 3);
    Actor_Run35(1, 3);
    Actor_Run35(2, 3);
    ((void (*)())Actor_Run36)(3, 3);
    Actor_Place10(11, 0x1a4, 0x11a);
    Actor_Place10(11, 0x1a4, 0x138);
    Actor_Place10(11, 0x1b7, 0x138);
    Actor_Place10(11, 0x1b7, 0x132);
    Actor_Far(11, 0, 0);
    exitActions = gOv25;
    Actor_Run29(1, exitActions);
    Actor_Run7(2, (s32)exitActions);
    Actor_Run8(3, (s32)exitActions);
    Actor_Run4(0x19a0000, -1, 0x12c0000, 1);
    Actor_Place10(0, 0x1a0, 0x138);
    Actor_Place10(0, 0x1b7, 0x138);
    Actor_Place10(0, 0x1b7, 0x132);
    Actor_Far(0, 0, 0);
    Actor_Run24(141);
    Actor_Place9(9, 0xcccc, 0x6666);
    Actor_Run5(0x10000, 0x2000);
    Actor_Run4(0x1b80000, -1, 0x1a40000, 1);
    Actor_Place10(9, 0x1b8, 0x1a4);
    Actor_Run5(0x20000, scale);
    Actor_Run4(0x1b80000, -1, 0x2580000, 1);
    Actor_Place9(9, 0x19999, 0xcccc);
    Actor_Place10(9, 0x1b8, 0x1f4);
    Actor_Place12(9, 0x1b8, 0x258);
    Actor_Do7(0x121);
    work = gWork;
    work->setup = 24;
    work->request = 0x100;
    Actor_Run48();
    Actor_Run49();
    Actor_Run47(10);
    Actor_Run50();
}

/* overlays/scene/actor/staged_group/scene_setup.c */
struct Obj {
    u8 filler00[6];
    u16 f06;
};

typedef struct Effect {
    unsigned char pad00[0xC];
    s32 y;
    unsigned char pad10[0x13];
    s8 state23;
} Effect;

extern u8 *gWork;

struct Obj *Actor_Run173(s32);

u8 *Actor_Run174();

Effect *Actor_Run175(s32);

/* Contiguous unnamed leaf-owner run for resource_3a8. */

/* resource_3a8 owner at 0x02000504, 138 bytes. Scene arrival sequence. */

static __inline__ void SetOffset(s32 actor, s32 axis, s32 offset)
{
    Actor_Run176(actor, axis, offset);
}

void Scene_RunStepWithValue29df(void)
{
    Actor_Run177();
    Actor_Apply12(0x947, 1);
    Actor_Apply13(0x29df, 1);
    Actor_Run178();
}

void Scene_RunStepWithValueFd6(void)
{
    Actor_Run179();
    Actor_Place16(12, 0, 0);
    Actor_Do21(0xfd6);
    Actor_Apply14(181, 3);
    Actor_Apply15(181, 0);
    Actor_Run180();
}

void Scene_RunSlotZeroFacingSequence(void)
{
    struct Obj *o;
    u32 v;
    u16 *q;

    o = Actor_Run173(0);
    v = (o->f06 + 0xfffff000) << 16;
    if (v > 0x60000000) {
        Actor_Run181();
        Actor_Place17(0, 8, 0);
        Actor_Do22(10);
        Actor_Do23(0x2584);
        Actor_Apply16(8, 0);
        if (Actor_Apply17(0, 0) == 0) {
            Actor_Apply18(8, 4);
            Actor_Apply19(8, 0);
        } else {
            q = (u16 *)(gWork + 472);
            *q = *q + 1;
            Actor_Apply20(8, 3);
            Actor_Apply21(8, 0);
        }
        Actor_Run182();
    }
}

void RunSceneArrivalSetup(void)
{
    s32 two = 2;

    Actor_Run183();
    Actor_Run184(188);
    Actor_Run185(36, 23, 43, 12, two, two);
    Actor_Run186(5);
    Actor_Run187(39, 23, 43, 12, two, two);
    Actor_Run188(5);
    Actor_Run189(0, 0x8000, 0x4000);
    Actor_Run174(0)[85] = 0;
    Actor_Run190(0, 2);
    SetOffset(0, 0, -8);
    Actor_Run191(10);
    Actor_Run192(2);
    Actor_Run193();
    Actor_Run194();
    Actor_Run195();
}

void Scene_DrawTilesAndRaiseActor11(void)
{
    Effect *effect = Actor_Run175(11);

    Actor_SetRect62(0, 0, 1, 1, 9, 14);
    Actor_SetRect63(0, 0, 1, 1, 9, 45);
    if (effect != 0) {
        Actor_Apply22(effect, 0);
        effect->y -= 0x200000;
        effect->state23 = 2;
    }
    Actor_Do24(0x201);
}

/* overlays/scene/actor/staged_group/scene_state_interaction.c */
extern u8 gWork[];

void State_LinkRecordZeroWhenFlag200Clear(void)
{
    u32 i;
    s32 rec;
    u8 *work;

    if (Actor_Check27(0x200) == 0) {
        work = *(volatile s32 *)0x03001ee0;
        rec = Actor_Check28(0);
        *(volatile s32 *)((s32)work + 24) = rec;
        Actor_Do10(0x200);
    }
}

/* overlays/scene/actor/staged_group/state_update.c */

struct Obj {
    u8 filler00[6];
    u16 f06;
};

extern s16 gCell[];

struct Obj *Actor_Run196(s32);

void State_ApplyValues12And2And3(void)
{
    Actor_Place18(0xC, 2, 3);
}

void State_ClearWord24WhenFlag200(void)
{
    extern u8 gWork[];

    u32 i;
    s32 record;

    if (Actor_Check29(0x200) != 0) {
        *(volatile s32 *)(*(volatile s32 *)0x03001ee0 + 24) = 0;
        Actor_Do11(0x200);
    }
}

void State_ClearRecordZeroBit0At35(void)
{
    u8 *p;
    s32 v;

    p = (u8 *)(Actor_Check46(0) + 35);
    v = 254;
    v &= *p;
    *p = v;
}

void State_CheckFlags941And940(void)
{
    if (Actor_Check47(0x941) != 0) {
        Actor_Do25(0x321);
        Actor_Do26(0x913);
        Actor_Do27(0x912);
        Actor_Check48(0x915);
    }
    if (Actor_Check49(0x940) != 0) {
        Actor_Do28(0x321);
    }
    if (gCell[225] != 0) {
        if (Actor_Check50(0x912) == 0) {
            Actor_Run197();
        }
    }
}

void State_ApplyFlagGatedActorEightSetup(void)
{
    struct Obj *o;

    if (Actor_Check51(0xfd6) == 0) {
        Actor_Do29(12);
    }
    if (Actor_Check52(0x915) != 0) {
        o = Actor_Run196(8);
        o->f06 = 0;
    }
    if (gCell[225] == 10) {
        Actor_Run198();
    }
}

void State_SetWork448AndRunFlag915Step(void)
{
    extern u8 *gWork;

    *(s32 *)(gWork + 448) = 516;
    if (Actor_Check53(0x915) != 0) {
        s32 k = 2;
        Actor_SetRect64(58, 5, 58, 8, k, 3);
        { s32 a = 8, b = 10; Actor_SetRect65(8, 11, 2, 1, a, b); }
        Actor_SetRect66(8, 12, 8, 11, k, 1);
        Actor_Run199();
        Actor_Do30(1);
    }
    if (gCell[225] <= 3) {
        Actor_Do31(170);
    }
}

void State_SetValues27Through34(void)
{
    Actor_Do32(27);
    Actor_Do33(28);
    Actor_Do34(29);
    Actor_Do35(30);
    Actor_Do36(32);
    Actor_Do37(31);
    Actor_Do38(33);
    Actor_Do39(34);
}

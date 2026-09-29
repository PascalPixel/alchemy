#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

#define NULL ((void *)0)
#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))
#define SCENE_FIELD_1C8 (*(s32 *)(*(u8 **)Data_03001ebc + 0x1c8))
#define SCENE_FIELD_1C0 (*(s32 *)(*(u8 **)Data_03001ebc + 0x1c0))

#include "RESOURCE_3A8_EFFECT.H"
extern u8 MsgKareiCameKalayBecauseDidntLike[];
extern u8 MsgKareiDidFindNeededInWeapon[];
extern u8 MsgKareiDoWantGoCaveUp[];
extern u8 MsgKareiLadyLayanaSharedInLord[];
extern u8 MsgKareiLayanaWasVeryHardOn[];
extern u8 MsgKareiLordHammetSellsHisBest[];

enum {
    /* Message 0x182 + 181. */
    ITEM_NUT = 181
};


struct Obj {
    u8 filler00[6];
    u16 f06;
};

struct Obj_02000040 {
    u8 filler00[100];
    u16 f64;
    u16 f66;
};

struct Object {
    u8 filler00[6];
    u16 x;
    u8 filler08[94];
    s16 cnt;
};

typedef struct Effect {
    unsigned char pad00[0xC];
    s32 y;
    unsigned char pad10[0x13];
    s8 state23;
} Effect;

struct SceneWork {
    u8 unknown_000[0x1c0];
    s32 request;
    u8 unknown_1c4[4];
    s32 setup;
    u8 unknown_1cc[12];
    u16 step;
};

struct Obj_020036f8 {
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

typedef struct RenderData {
    unsigned char pad00[0x1E];
    s16 rotation;
} RenderData;

typedef struct Effect_0200390c {
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
} Effect_0200390c;

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

void SceneDialogue_RunActorNineteenDialogue(void)
{
    Event_Begin();
    Event_SetMessage((s32)MsgKareiCameKalayBecauseDidntLike);
    Event_AskYesNo(19, 0);
    Event_End();
}

void FieldScene_RunSlotZeroFacingSequence(void)
{
    extern u8 *Data_03001ebc;

    struct Obj *o;
    u32 v;
    u16 *q;

    o = Actor_Get(ACTOR_PARTY_LEADER);
    v = (o->f06 + 0xfffff000) << 16;
    if (v > 0x60000000) {
        Event_Begin();
        Actor_FaceEachOther(ACTOR_PARTY_LEADER, 8, 0);
        Event_Wait(10);
        Event_SetMessage((s32)MsgKareiDoWantGoCaveUp);
        Event_OpenMessage(8, 0);
        if (Event_ChooseYesNo(0, 0) == 0) {
            Actor_SetAnimationAndWait(8, 4);
            Event_ShowMessage(8, 0);
        } else {
            q = (u16 *)(Data_03001ebc + 472);
            *q = *q + 1;
            Actor_SetAnimationAndWait(8, 3);
            Event_ShowMessage(8, 0);
        }
        Event_End();
    }
}

void SceneDialogue_RunActorTenDialogue(void)
{
    Event_Begin();
    Event_SetMessage((s32)MsgKareiLayanaWasVeryHardOn);
    Event_AskYesNo(10, 0);
    Event_End();
}

void SceneState_BranchOnSlotZeroFacing(void)
{
    struct Obj *o;
    u32 v;

    o = Actor_Get(ACTOR_PARTY_LEADER);
    v = (o->f06 - 0x2000) << 16;
    if (v > 0x80000000) {
        Shop_Open(22, 22);
    } else {
        Event_Begin();
        Event_SetMessage((s32)MsgKareiLordHammetSellsHisBest);
        Event_ShowMessage(22, 0);
        Event_End();
    }
}

void SceneDialogue_RunActorTwentyThreeByLeaderHeading(void)
{
    struct Obj *o;
    u32 v;

    o = Actor_Get(ACTOR_PARTY_LEADER);
    v = (o->f06 - 0x6001) << 16;
    if (v <= 0x7ffe0000) {
        Shop_Open(23, 23);
    } else {
        Event_Begin();
        Event_SetMessage((s32)MsgKareiDidFindNeededInWeapon);
        Event_AskYesNo(23, 0);
        Event_End();
    }
}

void FieldScene_RunActorTwentyFourAngleDialogue(void)
{
    struct Obj *o;
    u32 v;

    o = Actor_Get(ACTOR_PARTY_LEADER);
    v = (o->f06 - 0x2000) << 16;
    if (v > 0xC0000000) {
        Shop_Open(24, 24);
    } else {
        Event_Begin();
        Event_SetMessage((s32)MsgKareiLadyLayanaSharedInLord);
        Event_ShowMessage(24, 0);
        Event_End();
    }
}

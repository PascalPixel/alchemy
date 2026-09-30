#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

#define NULL ((void *)0)
#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))
#define SCENE_FIELD_1C8 (*(s32 *)(*(u8 **)Data_03001ebc + 0x1c8))
#define SCENE_FIELD_1C0 (*(s32 *)(*(u8 **)Data_03001ebc + 0x1c0))

#include "RESOURCE_3A8_EFFECT.H"

enum {
    /* Message 0x182 + 181. */
    ITEM_NUT = 181
};

enum StagedGroupMessage {
    MSG_ROBIN_PEERED_INTO = 0x947,
    MSG_CAME_KALAY_BECAUSE_DIDNT_LIKE = 0x1a7c,
    MSG_WHY_WE_STOPPING_AT_PLACE = 0x1a92,
    MSG_THATS_WE_CANT_WAIT_ANY = 0x1ab2,
    MSG_LORD_HAMMET_SELLS_HIS_BEST = 0x1acf,
    MSG_DID_FIND_NEEDED_IN_WEAPON = 0x1ad1,
    MSG_LADY_LAYANA_SHARED_IN_LORD = 0x1ad5,
    MSG_LORD_HAMMETS_PALACE_LORD_AWAY = 0x1b05,
    MSG_WEVE_ARRIVED_HAMMET = 0x256f,
    MSG_DO_WANT_GO_CAVE_UP = 0x2584,
    MSG_LAYANA_WAS_VERY_HARD_ON = 0x25b3,
    MSG_VERY_CLEAN_MAINTAINED = 0x29df
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

extern u8 KareiMachi_Script01[];
union GameStateRows {
    s16 halves[512][1];
};
void Scheduler_AddOrUpdateCallback();
void FieldScene_RunSupplementalSequenceOne();
u8 *Object_GetByIdFar();
void BattleFx_StartFadeOverlay();
void BattleFx_SetQueuedSoundAndPlay();

void SceneState_CheckFlags941And940(void)
{
    if (GameFlag_IsSet(0x941) != 0) {
        GameFlag_Set(0x321);
        GameFlag_Set(0x913);
        GameFlag_Set(0x912);
        GameFlag_Set(0x915);
    }
    if (GameFlag_IsSet(0x940) != 0) {
        GameFlag_Set(0x321);
    }
    if (gGameState.entrance != 0) {
        if (GameFlag_IsSet(0x912) == 0) {
            FieldScene_RunTwoActorCutsceneSequence();
        }
    }
}

void SceneState_ApplyFlagGatedActorEightSetup(void)
{
    struct Obj *o;

    if (GameFlag_IsSet(0xfd6) == 0) {
        InitializeOrbitingRenderEffect(12);
    }
    if (GameFlag_IsSet(0x915) != 0) {
        o = Actor_Get(8);
        o->f06 = 0;
    }
    if (gGameState.entrance == 10) {
        FieldScene_RunSecondaryGroupSequence();
    }
}

void SceneState_SetWork448AndRunFlag915Step(void)
{
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
    if (GameFlag_IsSet(0x915) != 0) {
        s32 k = 2;
        Map_CopyCellsTo(58, 5, 58, 8, k, 3);
        { s32 a = 8, b = 10; Map_CopyCellAttributes(8, 11, 2, 1, a, b); }
        Map_CopyCellsTo(8, 12, 8, 11, k, 1);
        Map_Redraw();
        Task_Wait(1);
    }
    if (gGameState.entrance <= 3) {
        BattleFx_SetQueuedSoundAndPlay(170);
    }
}

void FieldScene_RunMiddleSequence(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    s32 rec7;
    u8 *record;
    s32 r0;
    s32 v5;
    u8 *p5;
    u8 **base = (u8 **)Data_03001ebc;

    *(s32 *)(base[0] + 0x1c0) = 0x204;
    BattleFx_StartFadeOverlay(0);
    rec7 = GameFlag_IsSet(0x109);
    if (rec7 != 0) {
        p5 = base[9];
        r0 = GameFlag_IsSet(0x200);
        if (r0 != 0) {
            r0 = Object_GetById(ACTOR_PARTY_LEADER);
        }
        *(s32 *)(p5 + 24) = r0;
    } else {
        GameFlag_Set(0x200);
        if (((union GameStateRows *)&gGameState)->halves[225][0] == 4) {
            *(s32 *)(base[9] + 24) = rec7;
            GameFlag_Clear(0x200);
        }
    }
    if (GameFlag_IsSet(0x302) != 0) {
        Actor_SetPosition(11, 0x960000, 0x2d80000);
        if (GameFlag_IsSet(0x201) != 0) {
            Actor_Get(11);
            v5 = 9;
            Actor_SetAnimation(11, 5);
            Map_CopyCellAttributes(0, 0, 1, 1, v5, 14);
            Map_CopyCellAttributes(0, 0, 1, 1, v5, 45);
            {
                u8 *p = Object_GetByIdFar(11) + 35;
                s32 v = 2;

                v |= *p;
                *p = v;
            }
        }
    }
    record = Actor_Get(8);
    Actor_SetSpriteFlags((s32)record, 0);
    record = Actor_Get(9);
    Actor_SetSpriteFlags((s32)record, 0);
    Scheduler_AddOrUpdateCallback((s32)FieldScene_RunSupplementalSequenceOne, 0xc80);
    if (GameFlag_IsSet(0x915) != 0) {
        Actor_SetPosition(10, 0x1aa0000, 0x2da0000);
        record = Actor_Get(10);
        {
            s32 shown = 0x5000;

            *(u16 *)((s32)record + 6) = shown;
        }
        Map_CopyCellsTo(88, 48, 88, 45, 2, 3);
        Map_CopyCellsTo(24, 49, 24, 48, 2, 1);
        Map_CopyCellsTo(25, 42, 25, 47, 1, 1);
        Map_CopyCellAttributes(22, 50, 2, 1, 24, 49);
    }
    if (GameFlag_IsSet(0x302) == 0) {
    } else {
        Actor_SetPosition(8, 0xe80000, 0x2dc0000);
        Map_CopyCellAttributes(7, 44, 1, 1, 0, 1);
        Map_CopyCellsTo(74, 58, 78, 41, 1, 5);
        Map_CopyCellsTo(16, 109, 13, 109, 3, 2);
        Map_CopyCellsTo(67, 64, 71, 44, 1, 2);
        Map_CopyCellsTo(67, 64, 72, 44, 1, 2);
        Map_CopyCellsTo(67, 68, 73, 43, 1, 2);
        Map_CopyCellsTo(67, 68, 74, 43, 1, 2);
        Map_CopyCellsTo(67, 64, 75, 44, 1, 2);
        Map_CopyCellsTo(67, 66, 76, 44, 1, 2);
        Map_CopyCellsTo(67, 64, 77, 44, 1, 2);
        Map_CopyCellsTo(67, 64, 78, 44, 1, 2);
        Map_CopyCellsTo(67, 64, 79, 44, 1, 2);
        Map_CopyCellsTo(67, 66, 80, 44, 1, 2);
        Map_CopyCellsTo(2, 0, 9, 42, 2, 2);
        Map_CopyCellsTo(68, 64, 71, 44, 1, 2);
        Map_CopyCellsTo(68, 64, 72, 44, 1, 2);
        Map_CopyCellsTo(68, 68, 73, 43, 1, 2);
        Map_CopyCellsTo(68, 68, 74, 43, 1, 2);
        Map_CopyCellsTo(68, 64, 75, 44, 1, 2);
        Map_CopyCellsTo(68, 66, 76, 44, 1, 2);
        Map_CopyCellsTo(68, 64, 77, 44, 1, 2);
        Map_CopyCellsTo(68, 64, 78, 44, 1, 2);
        Map_CopyCellsTo(68, 64, 79, 44, 1, 2);
        Map_CopyCellsTo(68, 66, 80, 44, 1, 2);
        Map_CopyCellsTo(4, 0, 9, 42, 2, 2);
        Map_CopyCellsTo(7, 11, 7, 42, 10, 8);
        Map_CopyCellsTo(71, 12, 71, 43, 10, 13);
        Map_CopyCellAttributes(6, 13, 12, 12, 6, 44);
        Map_CopyCellAttributes(0, 1, 1, 1, 7, 44);
        goto L_02001cc2;
    }
    switch (((union GameStateRows *)&gGameState)->halves[225][0]) {
    case 1:
    case 2:
        BattleFx_SetQueuedSoundAndPlay(170);
        break;
    }
    L_02001cc2:;
    if (GameFlag_IsSet(0x303) == 0) {
    } else {
        Actor_SetPosition(9, 0x2b80000, 0x2dc0000);
        Map_CopyCellsTo(74, 58, 107, 41, 1, 5);
        Map_CopyCellsTo(45, 109, 42, 109, 3, 2);
        Map_CopyCellsTo(67, 64, 102, 44, 1, 2);
        Map_CopyCellsTo(67, 64, 103, 44, 1, 2);
        Map_CopyCellsTo(67, 64, 104, 44, 1, 2);
        Map_CopyCellsTo(67, 66, 105, 44, 1, 2);
        Map_CopyCellsTo(67, 64, 106, 44, 1, 2);
        Map_CopyCellsTo(67, 64, 107, 44, 1, 2);
        Map_CopyCellsTo(67, 64, 108, 44, 1, 2);
        Map_CopyCellsTo(67, 66, 109, 44, 1, 2);
        Map_CopyCellsTo(68, 64, 102, 44, 1, 2);
        Map_CopyCellsTo(68, 64, 103, 44, 1, 2);
        Map_CopyCellsTo(68, 64, 104, 44, 1, 2);
        Map_CopyCellsTo(68, 66, 105, 44, 1, 2);
        Map_CopyCellsTo(68, 64, 106, 44, 1, 2);
        Map_CopyCellsTo(68, 64, 107, 44, 1, 2);
        Map_CopyCellsTo(68, 64, 108, 44, 1, 2);
        Map_CopyCellsTo(68, 66, 109, 44, 1, 2);
        Map_CopyCellsTo(38, 14, 38, 44, 8, 4);
        Map_CopyCellsTo(102, 14, 102, 44, 8, 12);
        Map_CopyCellAttributes(37, 13, 10, 12, 37, 43);
        goto L_02001e5a;
    }
    switch (((union GameStateRows *)&gGameState)->halves[225][0]) {
    case 3:
    case 4:
        BattleFx_SetQueuedSoundAndPlay(170);
        break;
    }
    L_02001e5a:;
}

void SceneState_SetValues27Through34(void)
{
    SceneEffect_SetSlotVariantAndDescriptor(27);
    SceneEffect_SetSlotVariantAndDescriptor(28);
    SceneEffect_SetSlotVariantAndDescriptor(29);
    SceneEffect_SetSlotVariantAndDescriptor(30);
    SceneEffect_SetSlotVariantAndDescriptor(32);
    SceneEffect_SetSlotVariantAndDescriptor(31);
    SceneEffect_SetSlotVariantAndDescriptor(33);
    SceneEffect_SetSlotVariantAndDescriptor(34);
}

void SceneEffect_SetSlotVariantAndDescriptor(s32 a)
{
    struct Obj_02000040 *p;
    u32 t;

    p = Actor_Get(a);
    p->f64 = a;
    t = Random_Next();
    p->f66 = (t * 5) >> 12;
    Object_SetScript(p, (s32)KareiMachi_Script01);
}

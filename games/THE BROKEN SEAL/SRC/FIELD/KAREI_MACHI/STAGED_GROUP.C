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
u8 *Object_GetByIdFar();

/* Contiguous unnamed leaf-owner run for resource_3a8. */

static __inline__ void SetOffset(s32 actor, s32 axis, s32 offset)
{
    Actor_SetDestinationOffset(actor, axis, offset);
}

void RunSceneArrivalSetup(void)
{
    s32 two = 2;

    Event_Begin();
    Audio_PlayCue(188);
    Map_CopyCellsTo(36, 23, 43, 12, two, two);
    Task_Wait(5);
    Map_CopyCellsTo(39, 23, 43, 12, two, two);
    Task_Wait(5);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x8000, 0x4000);
    Object_GetByIdFar(0)[85] = 0;
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 2);
    SetOffset(0, 0, -8);
    Event_Wait(10);
    Event_RequestExit(2);
    Event_CloseScreen();
    Event_WaitForScreen();
    Event_End();
}

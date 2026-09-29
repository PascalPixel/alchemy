#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

#include "FIELD_EFFECT.H"

extern const u16 TakaraShima_EntranceCells[];

enum {
    /* Message 0x182 + 181. */
    ITEM_NUT = 181
};

/* Integrate position, velocity, rate and sprite angle for one scene effect.
   Signed division preserves decay toward zero for negative Z velocity. */
struct Sprite {
    u8 pad00[9];
    u8 flags9;
    u8 pad0a[20];
    u16 angle;
    u8 pad20[6];
    u8 state26;
};

struct Effect {
    u8 pad00[8];
    s32 position[3];
    u8 pad14[4];
    s32 accum18;
    s32 accum1c;
    u8 pad20[3];
    u8 flatla3;
    u8 pad24[12];
    s32 rate30;
    s32 rate34;
    u8 pad38[12];
    s32 velocity[3];
    struct Sprite *sprite;
    u8 pad54;
    u8 mode55;
    u8 pad56[14];
    u16 step64;
    u8 pad66[6];
    u32 callback;
};

struct T {
    u8 pad00[30];
    u16 f1e;
};

struct S {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    u8 pad10[32];
    s32 f30;
    s32 f34;
    s32 f38;
    s32 f3c;
    u8 pad40[16];
    struct T *f50;
};

typedef struct Obj {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
    u8 f14[28];
    s32 f30;
    s32 f34;
} Obj;

struct V6 {
    s32 a;
    s32 b;
    s32 c;
    s32 d;
    s32 e;
    s32 f;
};

struct S_02000474 {
    s32 f00;
    u16 f04;
    u16 f06;
    s32 f08;
    s32 f0c;
    s32 f10;
    u8 pad14[20];
    s32 f28;
    u8 pad2c[4];
    s32 f30;
    s32 f34;
    u8 pad38[29];
    u8 f55;
};

struct V {
    s32 a;
    s32 b;
    s32 c;
};

struct Owner {
    u8 unk0[9];
    u8 unk9_0 : 2;
    u8 mode : 2;
    u8 unk9_4 : 4;
};

struct S_020009dc {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
    u8 pad14[15];
    u8 f23;
};

struct SceneObject {
    u8 filler00[8];
    s32 x;
    s32 y;
    s32 z;
};

struct Vec {
    s32 x;
    s32 y;
    s32 z;
};

struct EffectParams {
    u8 filler00[0x24];
    s32 callback;
};

typedef union {
    s32 w;
    s16 h[2];
} RecWord;

struct S_02001b14 { s32 pad[2]; s32 f08; s32 f0c; s32 f10; };

struct S_02001bbc {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

struct S_02001c84 {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

struct S_02001d2c {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

struct S_02001de0 {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

struct S_02001e5c {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

struct S_02001ef4 {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

struct S_02001f78 {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

struct S_02002004 {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

struct S_02002080 {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

struct S_0200216c {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

struct S_02002200 {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

struct S_020022c8 {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

struct S_020023c4 {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

struct S_02002450 {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
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
union GameStateRows {
    s16 halves[512][1];
};
void Object_SetPosition(Obj *, s32, s32, s32);
void Object_CommitPosition(Obj *);
void SceneActor_MoveAndRedraw(struct V6 arg0);
void Vector_AddPolarOffset(s32 arg0, s32 arg1, struct V *arg2);
s32 Object_CheckMovementCollision(struct S *arg0, struct V *arg1);
Obj *Object_GetByIdFar();
void MarkGridLeftOfSceneActor(s32 actor_mode, s32 grid_value, s32 grid_attribute, s32 unused);
void MarkGridAboveSceneActor(s32 actor_mode, s32 grid_value, s32 grid_attribute, s32 unused);
void SetMapCellCollision(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
s32 TryPushBlockingSceneActor(struct S_02000474 *actor, struct V *requested);

/*
 * Distance between two three-component 16.16 fixed-point positions.
 *
 * Each argument walks three consecutive 16.16 words in x, y, z order. The
 * per-axis deltas are taken in fixed point, shifted down to integers, squared,
 * and summed; the total is passed to the resident IWRAM integer square root.
 *
 * Expressions are preserved exactly as reconstructed: the walking-pointer form
 * is load-bearing for byte-identity and must not become struct field access.
 */

/* Advance actor 18 and its companions along the Z-axis escape route.
   Shared branches preserve the transition call sites used by multiple rows. */

/* Old-style declarations: overlay imports vary in arity between call sites.
   One import name per call site: bl displacements are per-site. */

/* This overlay's transition starter at 0x02001774, one name per site. */

/* Advance actor 9 and its companions along the Z-axis escape route.
   Shared branches preserve the transition call sites used by multiple rows. */

/* Return leg of the slot-14 transition beat: negated ids. */

/* This overlay's transition starter at 0x02001774. */

/* Outbound leg of the slot-14 transition beat. */

/* Return leg of the slot-16 transition beat: negated ids. */

/* Outbound leg of the slot-16 transition beat. */

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
s32 StagedActor_FindClearPosition(struct StagedActorProbe *probe);

void FieldScene_RunScene3b2_02001494(void)
{
    u32 i;
    s32 record;

    Event_Begin();
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x6666, 0x3333);
    Event_OpenScreen();
    Event_WaitForScreen();
    GameFlag_Set((((union GameStateRows *)&gGameState)->halves[224][0] + (0x8c8 - (s32)&SceneId_TakaraShima6)));
    Event_Wait(30);
    Map_AnimateCells(TakaraShima_EntranceCells, 44, 7);
    Actor_CenterAndWalk(ACTOR_PARTY_LEADER, 3, -16);
    Event_RequestExit(3);
    Event_End();
}

void ConfigureSceneActor11(s32 actor_id)
{
    s32 a = 0x1300000;
    s32 b = 0x1700000;
    u8 *p = Actor_Get(11);

    if (p != 0) {
        p[89] = 0;
    }
    Actor_SetSpriteFlags(Actor_Get(actor_id), 0);
    SetMapCellCollision(0, a, b, 253);
    GameFlag_Set(576);
}

void ConfigureSceneActor12(s32 actor_id)
{
    s32 a = 0x500000;
    s32 b = 0x1700000;
    u8 *p = Actor_Get(12);

    if (p != 0) {
        p[89] = 0;
    }
    Actor_SetSpriteFlags(Actor_Get(actor_id), 0);
    SetMapCellCollision(0, a, b, 253);
    GameFlag_Set(577);
}

void ConfigureSceneActor13(s32 actor_id)
{
    s32 a = 0x600000;
    s32 b = 0x1500000;
    u8 *p = Actor_Get(13);

    if (p != 0) {
        p[89] = 0;
    }
    Actor_SetSpriteFlags(Actor_Get(actor_id), 0);
    SetMapCellCollision(0, a, b, 253);
    GameFlag_Set(578);
}

void ConfigureSceneActor14(s32 actor_id)
{
    s32 a = 0x900000;
    s32 b = 0x1400000;
    s32 c = 0x2f00000;
    s32 d = 0x1400000;
    u8 *p = Actor_Get(14);

    if (p != 0) {
        p[89] = 0;
    }
    Actor_SetSpriteFlags(Actor_Get(actor_id), 0);
    SetMapCellCollision(0, a, b, 253);
    SetMapCellCollision(0, c, d, 253);
    GameFlag_Set(579);
}

void ShowForgetEverythingMessage(void)
{
    GameFlag_Set(2244);
    {
        s32 k4 = 8, k5 = 21;

        Map_CopyCellAttributes(0, 0, 1, 1, k4, k5);
    }
}

void ShowHelpYouForgetMessage(void)
{
    GameFlag_Set(2245);
}

void ShowDamagedDoorMessage(void)
{
    GameFlag_Set(2246);
}

void ShowSaveMyLifeMessage(void)
{
    GameFlag_Set(2247);
}

void FieldScene_RunScene3b2_0200167c(void)
{
    s32 record;

    if (GameFlag_IsSet(0x8c4) != 0) {
        Map_CopyCellAttributes(0, 0, 1, 1, 8, 21);
        Actor_SetPosition(15, 0x3c80000, 0x3c80000);
    } else {
        record = Actor_Get(15);
        *(s32 *)(record + 28) = 0x19999;
    }
    if (GameFlag_IsSet(0x8c5) != 0) {
        Actor_SetPosition(16, 0x3c80000, 0x3c80000);
    } else {
        record = Actor_Get(16);
        *(s32 *)(record + 28) = 0x19999;
    }
    if (GameFlag_IsSet(0x8c6) != 0) {
        Actor_SetPosition(17, 0x3c80000, 0x3c80000);
    } else {
        record = Actor_Get(17);
        *(s32 *)(record + 28) = 0x19999;
    }
    if (GameFlag_IsSet(0x8c7) != 0) {
        Actor_SetPosition(18, 0x3c80000, 0x3c80000);
    } else {
        record = Actor_Get(18);
        *(s32 *)(record + 28) = 0x19999;
    }
}

void RunSceneVectorTransition(void)
{
    struct V6 transition;

    Event_Begin();
    if (StagedActor_FindClearPosition(&transition) != 0) {
        SceneActor_MoveAndRedraw(transition);
    }
    Event_End();
}

void PositionSceneActorPair(s32 actor_id, s32 x_offset, s32 z_offset)
{
    Obj *p;
    Obj *q;
    s32 x;
    s32 y;

    p = Actor_Get(gGameState.selected_actor);
    q = Actor_Get(actor_id);
    Event_Begin();
    {
        x = ((p->f08 + (x_offset << 16)) & 0xFFF00000) + 0x80000;
        y = ((p->f10 + (z_offset << 16)) & 0xFFF00000) + 0x80000;

        p->f30 = 0x10000;
        p->f34 = 0x8000;
        Object_SetPosition(p, x, p->f0c, y);
    }
    Object_SetAnimation(p, 27);
    {
        x = ((q->f08 + (x_offset << 16)) & 0xFFF00000) + 0x80000;
        y = ((q->f10 + (z_offset << 16)) & 0xFFF00000) + 0x80000;

        q->f30 = 0x10000;
        q->f34 = 0x8000;
        Object_SetPosition(q, x, q->f0c, y);
    }
    if (x_offset < 0 || z_offset < 0) {
        Object_SetAnimation(q, 4);
    } else {
        Object_SetAnimation(q, 3);
    }
    Audio_PlayCue(226);
    Object_CommitPosition(p);
    Audio_PlayCue(288);
    Event_End();
}

void MarkGridLeftOfSceneActor(s32 actor_mode, s32 grid_value, s32 grid_attribute, s32 unused)
{
    struct S_020009dc *p = (struct S_020009dc *)Object_GetByIdFar(actor_mode);

    if (p != 0) {
        s32 v;

        Actor_SetSpritePriority(actor_mode, 3);
        v = 2;
        v |= p->f23;
        p->f23 = (u8)v;
        {
            s32 k5 = p->f10 >> 20;
            s32 k4 = (p->f08 >> 20) - 1;

            Map_CopyCellAttributes(grid_value, grid_attribute, 3, 1, k4, k5);
        }
    }
}

void MarkGridAboveSceneActor(s32 actor_mode, s32 grid_value, s32 grid_attribute, s32 unused)
{
    struct S_020009dc *p = (struct S_020009dc *)Object_GetByIdFar(actor_mode);

    if (p != 0) {
        s32 v;

        Actor_SetSpritePriority(actor_mode, 3);
        v = 2;
        v |= p->f23;
        p->f23 = (u8)v;
        {
            s32 k4 = p->f08 >> 20;
            s32 k5 = (p->f10 >> 20) - 1;

            Map_CopyCellAttributes(grid_value, grid_attribute, 1, 3, k4, k5);
        }
    }
}

void SetSceneActorModes(int actor_id)
{
    Actor_SetAnimation(actor_id, 1);
    Actor_SetAnimation(actor_id, 2);
}

void InitializeEscapeSceneActors(void)
{
    MarkGridLeftOfSceneActor(8, 0x11, 0x1E, 0x15);
    MarkGridLeftOfSceneActor(0xA, 0x11, 0x1F, 0x16);
    MarkGridAboveSceneActor(0xB, 0x14, 0x1E, 0x17);
    MarkGridAboveSceneActor(0xC, 0x15, 0x1E, 0x18);
    MarkGridAboveSceneActor(0xD, 0x16, 0x1E, 0x19);
    MarkGridAboveSceneActor(0xF, 0x17, 0x1E, 0x1A);
    MarkGridLeftOfSceneActor(0x11, 0, 0x1E, 0x1F);
    MarkGridLeftOfSceneActor(0x12, 0, 0x1F, 0x20);
    MarkGridLeftOfSceneActor(9, 0, 0x20, 0x21);
    MarkGridAboveSceneActor(0x13, 4, 0x1E, 0x22);
    MarkGridAboveSceneActor(0xE, 5, 0x1E, 0x23);
    MarkGridAboveSceneActor(0x10, 6, 0x1E, 0x24);
}

void ActivateSceneActor8(void)
{
    SetSceneActorModes(8);
}

void ActivateSceneActor10(void)
{
    SetSceneActorModes(10);
}

void ActivateSceneActor11(void)
{
    SetSceneActorModes(11);
}

void ActivateSceneActor12(void)
{
    SetSceneActorModes(12);
}

void ActivateSceneActor13(void)
{
    SetSceneActorModes(13);
}

void ActivateSceneActor15(void)
{
    SetSceneActorModes(15);
}

void ActivateSceneActor17(void)
{
    SetSceneActorModes(17);
}

void ActivateSceneActor18(void)
{
    SetSceneActorModes(18);
}

void ActivateSceneActor9(void)
{
    SetSceneActorModes(9);
}

void ActivateSceneActor19(void)
{
    SetSceneActorModes(19);
}

void ActivateSceneActor14(void)
{
    SetSceneActorModes(14);
}

void ActivateSceneActor16(void)
{
    SetSceneActorModes(16);
}

s32 TryPushBlockingSceneActor(struct S_02000474 *actor, struct V *requested)
{
    u8 *state = &actor->f55;
    s32 saved_state = *state;
    struct V destination;

    destination.a = (actor->f08 & 0xfff00000) + 0x80000;
    destination.b = actor->f0c;
    destination.c = (actor->f10 & 0xfff00000) + 0x80000;
    {
        s32 direction = (actor->f06 + 0x2000) & 0xc000;

        Vector_AddPolarOffset(0x200000, direction, &destination);
    }
    if (Object_CheckMovementCollision(actor, &destination) == 0) {
        s32 t;

        Event_Begin();
        Object_SetAnimation(actor, 6);
        Task_Wait(6);
        Audio_PlayCue(152);
        Object_SetAnimation(actor, 7);
        actor->f30 = 0x30000;
        actor->f34 = 0x20000;
        actor->f28 = 0x40000;
        t = 126;
        t &= *state;
        *state = (u8)t;
        Actor_SetSpriteFlags(actor, 0);
        {
            s16 *coordinates = (s16 *)&destination;

            Actor_MoveToAndWait(ACTOR_PARTY_LEADER, coordinates[1], coordinates[5]);
        }
        Object_SetAnimation(actor, 6);
        Actor_SetSpriteFlags(actor, 1);
        *state = (u8)saved_state;
        Event_End();
        return 1;
    }
    return 0;
}

s32 CheckActorPathSouth(void)
{
    struct S_02001b14 *actor = Actor_Get(ACTOR_PARTY_LEADER);
    struct V destination;

    destination.a = actor->f08;
    destination.b = actor->f0c;
    destination.c = actor->f10 + -0x200000;
    return TryPushBlockingSceneActor(actor, &destination);
}

s32 CheckActorPathNorth(void)
{
    struct S_02001b14 *actor = Actor_Get(ACTOR_PARTY_LEADER);
    struct V destination;

    destination.a = actor->f08;
    destination.b = actor->f0c;
    destination.c = actor->f10 + 0x200000;
    return TryPushBlockingSceneActor(actor, &destination);
}

s32 CheckActorPathWest(void)
{
    struct S_02001b14 *actor = Actor_Get(ACTOR_PARTY_LEADER);
    struct V destination;

    destination.a = actor->f08 + -0x200000;
    destination.b = actor->f0c;
    destination.c = actor->f10;
    return TryPushBlockingSceneActor(actor, &destination);
}

s32 CheckActorPathEast(void)
{
    struct S_02001b14 *actor = Actor_Get(ACTOR_PARTY_LEADER);
    struct V destination;

    destination.a = actor->f08 + 0x200000;
    destination.b = actor->f0c;
    destination.c = actor->f10;
    return TryPushBlockingSceneActor(actor, &destination);
}

void UpdateEscapeRouteForActorPositions(void)
{
    s32 actor_x = Object_GetByIdFar(8)->f08 >> 20;
    s32 actor_z = Object_GetByIdFar(8)->f10 >> 20;
    s32 actor_12_x = Object_GetByIdFar(12)->f08 >> 20;
    s32 actor_15_x = Object_GetByIdFar(15)->f08 >> 20;

    if (actor_z == 19) {
        if (actor_12_x == 24) {
            PositionSceneActorPair(8, 0, -80);
        } else if (actor_15_x == 24) {
            PositionSceneActorPair(8, 0, -112);
            PositionSceneActorPair(8, 0, -32);
        } else {
            PositionSceneActorPair(8, 0, -80);
            PositionSceneActorPair(8, 0, -112);
        }
    } else if (actor_z == 14) {
        if (actor_12_x == 24) {
            return;
        }
        if (actor_15_x == 24) {
            PositionSceneActorPair(8, 0, -64);
        } else {
            PositionSceneActorPair(8, 0, -112);
        }
    } else if (actor_z == 10) {
        if (actor_15_x == 24) {
            return;
        }
        PositionSceneActorPair(8, 0, -48);
    } else {
        CheckActorPathSouth();
        return;
    }
    Task_Wait(2);
    {
        s32 route_end_z = Object_GetByIdFar(8)->f10 >> 20;
        s32 route_x = actor_x - 1;

        Map_CopyCellAttributes(route_x, actor_z, 3, 1, route_x, route_end_z);
    }
    Map_CopyCellAttributes(0, 0, 3, 1, actor_x - 1, actor_z);
}

void UpdateActor8ReturnRoute(void)
{
    s32 x = Object_GetByIdFar(8)->f08 >> 20;
    s32 y = Object_GetByIdFar(8)->f10 >> 20;
    s32 z = Object_GetByIdFar(12)->f08 >> 20;

    if (y == 7) {
        if (z == 24) {
            PositionSceneActorPair(8, 0, 48);
        } else {
            PositionSceneActorPair(8, 0, 80);
            PositionSceneActorPair(8, 0, 112);
        }
    } else if (y == 10) {
        if (z == 24) {
            return;
        }
        PositionSceneActorPair(8, 0, 144);
    } else if (y == 14) {
        PositionSceneActorPair(8, 0, 80);
    } else {
        return;
    }
    Task_Wait(2);
    {
        s32 k = Object_GetByIdFar(8)->f10 >> 20;
        s32 m = x - 1;

        Map_CopyCellAttributes(m, y, 3, 1, m, k);
    }
    Map_CopyCellAttributes(0, 0, 3, 1, x - 1, y);
}

void UpdateActor10RetreatRoute(void)
{
    s32 x = Object_GetByIdFar(10)->f08 >> 20;
    s32 y = Object_GetByIdFar(10)->f10 >> 20;
    s32 z = Object_GetByIdFar(13)->f08 >> 20;
    s32 w = Object_GetByIdFar(15)->f08 >> 20;

    if (y == 18) {
        if (w >= 31 && w <= 33) {
            PositionSceneActorPair(10, 0, -128);
        } else if (z >= 31 && z <= 33) {
            PositionSceneActorPair(10, 0, -128);
        } else {
            PositionSceneActorPair(10, 0, -112);
            PositionSceneActorPair(10, 0, -64);
        }
    } else if (y == 10) {
        if (w >= 31 && w <= 33) {
            return;
        }
        if (z >= 31 && z <= 33) {
            return;
        }
        PositionSceneActorPair(10, 0, -48);
    } else if (y == 7) {
        return;
    }
    Task_Wait(2);
    {
        s32 k = Object_GetByIdFar(10)->f10 >> 20;
        s32 m = x - 1;

        Map_CopyCellAttributes(m, y, 3, 1, m, k);
    }
    Map_CopyCellAttributes(0, 0, 3, 1, x - 1, y);
}

void UpdateActor10AdvanceRoute(void)
{
    s32 x = Object_GetByIdFar(10)->f08 >> 20;
    s32 y = Object_GetByIdFar(10)->f10 >> 20;

    if (y != 18) {
        if (y == 10) {
            PositionSceneActorPair(10, 0, 128);
        } else {
            PositionSceneActorPair(10, 0, 112);
            PositionSceneActorPair(10, 0, 64);
        }
        Task_Wait(2);
        {
            s32 k = Object_GetByIdFar(10)->f10 >> 20;
            s32 m = x - 1;

            Map_CopyCellAttributes(m, y, 3, 1, m, k);
        }
        Map_CopyCellAttributes(0, 0, 3, 1, x - 1, y);
    }
}

void UpdateActor11WestRoute(void)
{
    s32 x = Object_GetByIdFar(11)->f08 >> 20;
    s32 y = Object_GetByIdFar(11)->f10 >> 20;

    if (x != 30) {
        if (x == 34) {
            if ((Object_GetByIdFar(10)->f10 >> 20) == 18) {
                return;
            }
            PositionSceneActorPair(11, -64, 0);
        } else if (x == 36) {
            if ((Object_GetByIdFar(10)->f10 >> 20) == 18) {
                PositionSceneActorPair(11, -32, 0);
            } else {
                PositionSceneActorPair(11, -96, 0);
            }
        }
        Task_Wait(2);
        {
            s32 k = Object_GetByIdFar(11)->f08 >> 20;
            s32 m = y - 1;

            Map_CopyCellAttributes(x, m, 1, 3, k, m);
        }
        Map_CopyCellAttributes(0, 0, 1, 3, x, y - 1);
    }
}

void UpdateActor11EastRoute(void)
{
    s32 x = Object_GetByIdFar(11)->f08 >> 20;
    s32 y = Object_GetByIdFar(11)->f10 >> 20;

    if (x != 36) {
        if (x == 30) {
            if ((Object_GetByIdFar(10)->f10 >> 20) == 18) {
                return;
            }
            PositionSceneActorPair(11, 96, 0);
        } else if (x == 34) {
            PositionSceneActorPair(11, 32, 0);
        }
        Task_Wait(2);
        {
            s32 k = Object_GetByIdFar(11)->f08 >> 20;
            s32 m = y - 1;

            Map_CopyCellAttributes(x, m, 1, 3, k, m);
        }
        Map_CopyCellAttributes(0, 0, 1, 3, x, y - 1);
    }
}

void UpdateActor12WestRoute(void)
{
    s32 x = Object_GetByIdFar(12)->f08 >> 20;
    s32 y = Object_GetByIdFar(12)->f10 >> 20;

    if (x == 36) {
        PositionSceneActorPair(12, -96, 0);
        PositionSceneActorPair(12, -96, 0);
    } else if (x == 34) {
        PositionSceneActorPair(12, -96, 0);
        PositionSceneActorPair(12, -64, 0);
    } else if (x == 24) {
        return;
    }
    Task_Wait(2);
    {
        s32 k = Object_GetByIdFar(12)->f08 >> 20;
        s32 m = y - 1;

        Map_CopyCellAttributes(x, m, 1, 3, k, m);
    }
    Map_CopyCellAttributes(0, 0, 1, 3, x, y - 1);
}

void UpdateActor12EastRoute(void)
{
    s32 x = Object_GetByIdFar(12)->f08 >> 20;
    s32 y = Object_GetByIdFar(12)->f10 >> 20;

    if (x == 24) {
        PositionSceneActorPair(12, 96, 0);
        PositionSceneActorPair(12, 96, 0);
    } else if (x == 34) {
        PositionSceneActorPair(12, 32, 0);
    } else if (x == 36) {
        return;
    }
    Task_Wait(2);
    {
        s32 k = Object_GetByIdFar(12)->f08 >> 20;
        s32 m = y - 1;

        Map_CopyCellAttributes(x, m, 1, 3, k, m);
    }
    Map_CopyCellAttributes(0, 0, 1, 3, x, y - 1);
}

void UpdateActor13WestRoute(void)
{
    s32 x = Object_GetByIdFar(13)->f08 >> 20;
    s32 y = Object_GetByIdFar(13)->f10 >> 20;
    s32 z = Object_GetByIdFar(10)->f10 >> 20;
    s32 w = Object_GetByIdFar(15)->f08 >> 20;

    if (x == 36) {
        if (w == 34) {
            PositionSceneActorPair(13, -16, 0);
        } else if (z == 7) {
            PositionSceneActorPair(13, -32, 0);
        } else if (w == 30) {
            PositionSceneActorPair(13, -80, 0);
        } else {
            PositionSceneActorPair(13, -96, 0);
            PositionSceneActorPair(13, -80, 0);
        }
    } else if (x == 35) {
        if (w == 34) {
            return;
        } else if (z == 7) {
            PositionSceneActorPair(13, -16, 0);
        } else if (w == 30) {
            PositionSceneActorPair(13, -64, 0);
        } else {
            PositionSceneActorPair(13, -80, 0);
            PositionSceneActorPair(13, -80, 0);
        }
    } else if (x == 34) {
        if (z == 7) {
            return;
        }
        if (w == 30) {
            PositionSceneActorPair(13, -48, 0);
        } else {
            PositionSceneActorPair(13, -144, 0);
        }
    } else if (x == 31) {
        if (w == 30) {
            return;
        }
        PositionSceneActorPair(13, -96, 0);
    } else if (x == 25) {
        return;
    }
    Task_Wait(2);
    {
        s32 k = Object_GetByIdFar(13)->f08 >> 20;
        s32 m = y - 1;

        Map_CopyCellAttributes(x, m, 1, 3, k, m);
    }
    Map_CopyCellAttributes(0, 0, 1, 3, x, y - 1);
}

void UpdateActor13EastRoute(void)
{
    s32 x = Object_GetByIdFar(13)->f08 >> 20;
    s32 y = Object_GetByIdFar(13)->f10 >> 20;

    Actor_Get(15);
    if (x == 25) {
        PositionSceneActorPair(13, 96, 0);
        PositionSceneActorPair(13, 80, 0);
    } else if (x == 31) {
        PositionSceneActorPair(13, 80, 0);
    } else if (x == 34) {
        PositionSceneActorPair(13, 32, 0);
    } else if (x == 35) {
        PositionSceneActorPair(13, 16, 0);
    } else if (x == 36) {
        return;
    }
    Task_Wait(2);
    {
        s32 k = Object_GetByIdFar(13)->f08 >> 20;
        s32 m = y - 1;

        Map_CopyCellAttributes(x, m, 1, 3, k, m);
    }
    Map_CopyCellAttributes(0, 0, 1, 3, x, y - 1);
}

void UpdateActor15WestRoute(void)
{
    s32 x = Object_GetByIdFar(15)->f08 >> 20;
    s32 y = Object_GetByIdFar(15)->f10 >> 20;
    s32 z = Object_GetByIdFar(8)->f10 >> 20;
    s32 w = Object_GetByIdFar(10)->f10 >> 20;

    if (x == 35) {
        if (w == 7) {
            PositionSceneActorPair(15, -16, 0);
        } else if (z == 7) {
            PositionSceneActorPair(15, -112, 0);
        } else {
            PositionSceneActorPair(15, -96, 0);
            PositionSceneActorPair(15, -80, 0);
        }
    } else if (x == 34) {
        if (w == 7) {
            return;
        }
        PositionSceneActorPair(15, -96, 0);
        PositionSceneActorPair(15, -64, 0);
    } else if (x == 33) {
        PositionSceneActorPair(15, -144, 0);
    } else if (x == 31) {
        PositionSceneActorPair(15, -80, 0);
    } else if (x == 30) {
        PositionSceneActorPair(15, -96, 0);
    } else if (x == 24) {
        return;
    }
    Task_Wait(2);
    {
        s32 k = Object_GetByIdFar(15)->f08 >> 20;
        s32 m = y - 1;

        Map_CopyCellAttributes(x, m, 1, 3, k, m);
    }
    Map_CopyCellAttributes(0, 0, 1, 3, x, y - 1);
}

void UpdateActor15EastRoute(void)
{
    s32 x = Object_GetByIdFar(15)->f08 >> 20;
    s32 y = Object_GetByIdFar(15)->f10 >> 20;
    s32 z = Object_GetByIdFar(10)->f10 >> 20;
    s32 w = Object_GetByIdFar(13)->f08 >> 20;

    if (x == 24) {
        if (z == 7 || w == 31) {
            PositionSceneActorPair(15, 96, 0);
        } else if (w == 34) {
            PositionSceneActorPair(15, 64, 0);
            PositionSceneActorPair(15, 80, 0);
        } else if (w == 35) {
            PositionSceneActorPair(15, 80, 0);
            PositionSceneActorPair(15, 80, 0);
        } else {
            PositionSceneActorPair(15, 80, 0);
            PositionSceneActorPair(15, 96, 0);
        }
    } else if (x == 30 || w == 31) {
        if (z == 7) {
            return;
        }
        if (w == 34) {
            PositionSceneActorPair(15, 48, 0);
        } else if (w == 35) {
            PositionSceneActorPair(15, 64, 0);
        } else {
            PositionSceneActorPair(15, 80, 0);
        }
    } else if (x == 33) {
        if (w == 34) {
            return;
        }
        if (w == 35) {
            PositionSceneActorPair(15, 16, 0);
        } else {
            PositionSceneActorPair(15, 32, 0);
        }
    } else if (x == 34) {
        PositionSceneActorPair(15, 16, 0);
    } else if (x == 35) {
        return;
    }
    Task_Wait(2);
    {
        s32 k = Object_GetByIdFar(15)->f08 >> 20;
        s32 m = y - 1;

        Map_CopyCellAttributes(x, m, 1, 3, k, m);
    }
    Map_CopyCellAttributes(0, 0, 1, 3, x, y - 1);
}

void UpdateActor17SouthRoute(void)
{
    s32 x = Object_GetByIdFar(17)->f08 >> 20;
    s32 y = Object_GetByIdFar(17)->f10 >> 20;
    s32 z = Object_GetByIdFar(19)->f08 >> 20;

    if (y == 19) {
        if (z >= 3 && z <= 5) {
            PositionSceneActorPair(17, 0, -16);
        } else {
            PositionSceneActorPair(17, 0, -64);
        }
    } else if (y == 18) {
        if (z >= 3 && z <= 5) {
            return;
        }
        PositionSceneActorPair(17, 0, -48);
    } else if (y == 15) {
        return;
    }
    Task_Wait(2);
    {
        s32 k = Object_GetByIdFar(17)->f10 >> 20;
        s32 m = x - 1;

        Map_CopyCellAttributes(m, y, 3, 1, m, k);
    }
    Map_CopyCellAttributes(0, 0, 3, 1, x - 1, y);
}

void UpdateActor17NorthRoute(void)
{
    s32 x = Object_GetByIdFar(17)->f08 >> 20;
    s32 y = Object_GetByIdFar(17)->f10 >> 20;

    if (y == 15) {
        PositionSceneActorPair(17, 0, 64);
    } else if (y == 18) {
        PositionSceneActorPair(17, 0, 16);
    } else if (y == 19) {
        return;
    }
    Task_Wait(2);
    {
        s32 k = Object_GetByIdFar(17)->f10 >> 20;

        s32 m = x - 1;

        Map_CopyCellAttributes(m, y, 3, 1, m, k);
    }
    Map_CopyCellAttributes(0, 0, 3, 1, x - 1, y);
}

#include "TYPES.H"
#include "MAKYURI.H"
#include "IWRAM_CALL.H"

#define FrameCounter gFrameCount
s32 FixedSqrt(s32 value);

struct FollowDetail {
    u8 unknown_00[5];
    u8 shade;
};

struct FollowSprite {
    u8 unknown_00[37];
    u8 visible;
    u8 unknown_26[2];
    struct FollowDetail *detail;
};

void AnimationObjects_SelectAnimation(struct FieldSprite *sprite, s32 mode);

/* A spawn point: an object's position and its first update value. */
struct SpawnPoint {
    u8 unknown_00[8];
    s32 x;
    s32 y;
    s32 z;
    s32 value;
};

struct EventSpawns {
    u8 unknown_00[20];
    struct SpawnPoint *points[1];
};

extern s32 MakyuriHeya_SparkScript[];
void Engine_EventBegin(void);
void MakyuriHeya_SpawnSparkPair(void);
void WaitFrames(s32 frames);
void Engine_EventEnd(void);
extern u8 Makyuri_PillarScript[];

/* The IWRAM work pointers: the event work, and 0x20 bytes on, the scene's. */
struct WorkPointers {
    u8 *event;
    u8 *unknown_04[7];
    u8 *scene;
};

extern struct WorkPointers gWork;

struct Sprite39b {
    u8 pad[9];
    u8 lo : 2;
    u8 layer : 2;
    u8 hi : 4;
};

struct Work {
    u8 pad[20];
    u8 *actors[1];
};

struct Flags85 {
    u8 pad[85];
    u8 flags;
};

struct Obj {
    u8 pad00[12];
    s32 y;
    u8 pad10[69];
    u8 f85;
    u8 pad56[18];
    u8 *owner;
};

struct PillarState {
    s32 step;
    u8 pad04[8];
    s32 x;
    s32 z;
    u8 *pillar;
};

/* Glide the actor toward its leader, faster the farther behind it is, and
 * flicker its shadow. */
s32 Makyuri_FollowLeader(struct FieldActor *actor)
{
    struct FieldActor *leader;
    struct FollowSprite *sprite;
    s32 tz;
    s32 tx;
    s32 dx;
    s32 dz;
    s32 dist;
    s32 step;

    actor->speed = 0x20000;
    leader = *(struct FieldActor **)actor->unknown_68;
    actor->acceleration = 0x10000;
    tx = leader->x.fixed;
    tz = leader->z.fixed;
    actor->target_x = ACTOR_NO_TARGET;
    actor->target_y = ACTOR_NO_TARGET;
    actor->target_z = ACTOR_NO_TARGET;
    dx = (tx - actor->x.fixed) / 0x10000;
    dz = (tz - actor->z.fixed) / 0x10000;
    dist = (u16)Iwram_Sqrt(dx * dx + dz * dz);
    dx = tx - actor->x.fixed;
    dz = tz - actor->z.fixed;
    dist <<= 16;
    if (dist < 0x400000)
        dist = FixedSqrt(Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dz, dz));
    step = dist / 8;
    if (step > actor->speed)
        step = actor->speed;
    if (dist < 0x4000) {
        actor->x.fixed = tx;
        actor->z.fixed = tz;
    } else {
        if (dist > step) {
            dx = Iwram_MulQ16(Iwram_RatioMulQ14(dist, dx), step);
            dz = Iwram_MulQ16(Iwram_RatioMulQ14(dist, dz), step);
        }
        actor->x.fixed += dx;
        actor->z.fixed += dz;
    }
    sprite = (struct FollowSprite *)actor->sprite;
    sprite->detail->shade = ((FrameCounter >> 1) & 1) * 7;
    sprite->visible = 1;
    return 1;
}

/* Spawns the two sparks at the event's spawn point, one on each sprite layer,
 * and plays their cue. */
void MakyuriHeya_SpawnSparkPair(void)
{
    struct SpawnPoint *point;
    struct FieldActor *spark;
    struct FieldSprite *sprite;

    point = (*(struct EventSpawns **)&gEventWork)->points[gGameState.selected_actor];
    spark = Engine_ObjectCreate(26, point->x, point->y, point->z);
    if (spark != NULL) {
        *(s32 *)spark->unknown_14 = point->value;
        sprite = spark->sprite;
        Engine_ObjectSetScript(spark, MakyuriHeya_SparkScript);
        spark->motion_flags = 0;
        spark->unknown_64 = 0;
        *(struct SpawnPoint **)&spark->unknown_68 = point;
        if (sprite != NULL) {
            AnimationObjects_SelectAnimation(sprite, 2);
            sprite->flags = 0;
            sprite->priority = 1;
        }
    }
    spark = Engine_ObjectCreate(26, point->x, point->y, point->z);
    if (spark != NULL) {
        *(s32 *)spark->unknown_14 = point->value;
        sprite = spark->sprite;
        Engine_ObjectSetScript(spark, MakyuriHeya_SparkScript);
        spark->motion_flags = 0;
        spark->unknown_64 = 0;
        *(struct SpawnPoint **)&spark->unknown_68 = point;
        spark->priority_flags = 2;
        if (sprite != NULL) {
            AnimationObjects_SelectAnimation(sprite, 1);
            sprite->flags = 0;
        }
    }
    Audio_PlayCue(130);
}

/* Linked into both Mercury Lighthouse overlays, each with its own copy of the
 * pillar script and of the spark routine. */
/* The saved game as words: word 125 is the selected actor. */
/* Raise the pillar under the party leader in the remaining steps, then record
 * the cell it now stands on. */
void MakyuriIriguchi_RaisePillar(void)
{
    struct PillarState *state;
    u8 *leader;
    u8 *pillar;
    u8 *obj;
    struct Sprite39b *spr;
    s32 step;

    /* FAKEMATCH: the event work pointer is read as globals - 8, so the pool
     * holds the scene pointer's address and the base is derived from it. */
    {
        u8 **globals = &gWork.scene;

        state = *(struct PillarState **)globals[0];
        leader = (*(struct Work **)(globals - 8))->actors[gGameState.selected_actor];
    }
    if ((u32)state->step > 2)
        return;
    Engine_EventBegin();
    pillar = state->pillar;
    if (pillar == 0) {
        obj = Engine_ObjectCreate(26, *(s32 *)(leader + 8), *(s32 *)(leader + 12) + 0x180000, *(s32 *)(leader + 16));
        if (obj != 0) {
            spr = *(struct Sprite39b **)(obj + 80);
            *(s32 *)(obj + 20) = *(s32 *)(leader + 20);
            Engine_ObjectSetScript(obj, (s32)Makyuri_PillarScript);
            ((struct Obj *)obj)->owner = leader;
            ((struct Obj *)obj)->f85 = 4;
            ((struct Obj *)obj)->y += -0x8000;
            if (spr != 0) {
                ((u8 *)spr)[38] = 0;
                spr->layer = 1;
            }
            obj[84] = 0;
            state->pillar = obj;
            pillar = obj;
        } else {
            pillar = state->pillar;
        }
    }
    for (step = state->step; step <= 2; step++) {
        MakyuriHeya_SpawnSparkPair();
        WaitFrames(30);
        pillar[84] = 1;
        Object_SetMode(pillar, 5 - step);
    }
    state->step = 3;
    state->x = (*(s32 *)(pillar + 8) & -0x100000) + 0x80000;
    state->z = (*(s32 *)(pillar + 16) & -0x100000) + 0x80000;
    Engine_GameFlagSet(0x161);
    Engine_EventEnd();
}

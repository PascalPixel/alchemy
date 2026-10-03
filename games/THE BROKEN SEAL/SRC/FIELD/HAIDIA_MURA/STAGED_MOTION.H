#include "RUNTIME_MEM.H"
#ifndef GUARD_STAGED_MOTION_H
#define GUARD_STAGED_MOTION_H

#include "TYPES.H"
#include "CALLBACK_SCHEDULER.H"

#define ACTOR_A 12
#define ACTOR_B 13
#define ACTOR_C 14
#define ACTOR_D 11

#include "STAGED_ACTOR.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

enum {
    /* Message 0x182 + 181. */
    ITEM_NUT = 181
};

/* Robin's mother, as in the repair-morning scene of this overlay; the
   villagers call her Dora. */
enum {
    ACTOR_DORA = 21
};

/*
 * Reset or tear down a consecutive run of scene actors -- resource_373.
 */
struct Resource373Actor {
    u8 unknown_00[8];
    s32 field08;
    s32 field0c;
    s32 field10;
    u8 unknown_14[0x41];
    u8 flag55;
};

struct SourceEntity {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

struct ParticleRecord {
    u8 f00[9];
    u8 f09;
};

struct StagedParticle {
    u8 f00[0x48];
    s32 f48;
    s32 f4c;
    struct ParticleRecord *f50;
    u8 f54;
    u8 f55;
    u8 f56[8];
    u16 f5e;
};

/* Emitter fields are named by offset; the layout is not verified. */
struct Resource373Emitter {
    u8 unknown_00[6];
    u16 field06;
    s32 x;                          /* 0x08 */
    s32 y;                          /* 0x0c */
    s32 z;                          /* 0x10 */
    u8 unknown_14[4];
    s32 field18;
    s32 field1c;
    u8 unknown_20[4];
    s32 field24;
    s32 field28;
    s32 field2c;
    u8 unknown_30[8];
    s32 field38;
    s32 field3c;
    s32 field40;
};

struct Resource373Particle {
    u8 unknown_00[0x28];
    s32 lifetime;                   /* 0x28 */
    u8 unknown_2c[4];
    s32 field30;
    s32 field34;
    u8 unknown_38[0x10];
    s32 field48;
    u8 unknown_4c[9];
    u8 field55;
};

struct StagedVerticalEffect {
    s32 pad0[2];
    s32 f8;
    s32 fc;
    s32 f10;
    s32 pad14;
    s32 f18;
    s32 f1c;
    s32 pad20[17];
    s16 f64;
    s16 pad66;
    s32 *f68;
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

extern s32 gHaidiaMuraEntrances[];
extern s32 gHaidiaMuraExits[];
extern s32 gHaidiaMuraPlacements4[];
extern s32 gHaidiaMuraPlacements3[];
extern s32 gHaidiaMuraPlacements2[];
extern s32 gHaidiaMuraPlacements[];
extern s32 gHaidiaMuraEvents3[];
extern s32 gHaidiaMuraEvents2[];
extern s32 gHaidiaMuraEvents[];
extern u8 gHaidiaMuraCellAnimA[];
extern u8 gHaidiaMuraCellAnimB[];
extern u8 gHaidiaMuraCellAnimC[];
extern u8 gHaidiaMuraCellAnimD[];
extern u8 gHaidiaMuraActor22Actions[];
extern u8 gHaidiaMuraLeaderWalkActions[];
extern u8 gHaidiaMuraActor22WalkActions[];
extern u8 gVillagerAction[];
extern u8 gLeaderHammerAction[];
extern u8 gGeraldAction[];
extern u8 gJasmineAction[];
extern s32 gHaidiaMuraPairTableA[];
extern s32 gHaidiaMuraPairTableB[];
extern s32 gHaidiaMuraPairTableC[];
extern s32 gHaidiaMuraPairTableD[];
extern s32 gDustBurstScript[];

void Object_RefreshSelectorById();
s32 Object_SetActionCallbackAndRefreshById();
void Graphics_EnableObjLayerAndCallbacks();
void ObjectDispatch_StopCallbacksAndHideLayers();
void UiText_ShowCenteredMessage();
void Audio_PlayCueFromEventWork();
void Object_SetPosition();
void HaidiaMura_RunWalkScene032B0(s32 actor, s32 animation, s32 next_animation, s32 grounded);
void HaidiaMura_OpenVillagerLane(void);
s32 SceneActor_RunStep18WhenTargetSet();
void HaidiaMura_RunWalkScene03380(s32 actor, s32 animation, s32 next_animation, s32 grounded);
void FieldScene_RunStep8C();
void SceneState_SetValue0ThenCall();
void FieldScene_RunStep17();
void SceneState_SetValue1ThenCall();
void FieldScene_RunStep9();
void Effect_ConfigureSpawnedParticle(struct SourceEntity *source);
void SceneState_SetValue24ThenCall();
void SceneState_SetValue25ThenCall();
void Effect_PlayStepSound();
void Vector_AddPolarOffset(s32, s32, s32 *);
void BattleEffect_CleanupSceneObjects(void);
s32 HaidiaMura_TestFacing(struct FieldActor *obj, struct FieldActor *target, s32 range, s32 force);
void InitializeStagedActorSceneOrbitingEffect(s32 id);
void WorldMap_CreateLinkedEffects(s32);
s32 SceneEffect_UpdateOrbitPosition(s32 *p);

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

/* Actor IDs the scene sets up: three actors share a common data pointer,
 * a fourth actor (11) is driven through its own sequence of position/speed
 * calls. */

/* Shared data used for actors A, B, C. */

void Effect_UpdateParticlePosition(s32 *particle, s32 delta_x, s32 delta_z);

/* Moves the next dialogue line on by amount messages. */
static __inline__ void bump_step(s32 amount)
{
    gEventWork->message += amount;
}

/* Newly identified engine calls: each macro below names the site's own
 * engine function from its reconstructed C source, and keeps the site's
 * own calling form. */

/*
 * Staged actor cutscene tail -- resource_373. The long scene script plus the
 * particle, sound, and vertical-motion effects it drives.
 */

#endif

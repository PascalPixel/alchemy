#include "types.h"
#include "scene.h"
#include "sound_ids.h"
#include "global_cells.h"
#include "fixed_math.h"

/* battle/effects/scene_transition/reset.c */
typedef struct {
    u8  reserved00[0x34];
    s8 field34;
} SceneTransitionContext;

typedef struct {
    u8  reserved000[0xcb8];
    s16 active;
    s16 transition_timer;
} SceneTransitionState;

typedef struct {
    u8  reserved000[0x53c];
    u8 transition_status;
    u8 transition_mode;
    u8 transition_phase;
} SceneTransitionScene;

void WaitFrames(s32);
void ScheduleCallback(void (*callback)(void));
void *FunctionHead_0808e4b4(u32 kind, u32 entry_index, s32 *size);
void BattleFx_ApplyColorToTargetBuffer(u32 battle_value, s32 enabled);
void BattleFx_ApplyColorToSourceBuffer(u32 battle_value, s32 enabled);
void BattleFx_StartBufferInterpolation(s32 battle_value);

void Audio_PlayCue(s32 no);
void Battle_unk2_4(void);

extern SceneTransitionContext *gIw;
extern s32 gCell[];

void ResetSceneTransitionEffect(void)
{
    SceneTransitionContext **cell = &gIw;
    SceneTransitionContext *ctx = *cell;
    SceneTransitionScene *scene = *(SceneTransitionScene **)((u8 *)cell - 0x64);
    SceneTransitionState *state = *(SceneTransitionState **)((u8 *)cell - 0x74);
    s16 zero;
    s32 size;
    void *resource;

    if (state->active != 0) {
        Audio_PlayCue(SOUND_SCENE_TRANSITION);
        ScheduleCallback(Battle_unk2_4);

        zero = 0;
        state->active = zero;
        state->transition_timer = zero;
        Battle_Do(0);

        BattleFx_ApplyColorToTargetBuffer(0x10000, 1);
        BattleFx_StartBufferInterpolation(1);
        BattleFx_ApplyColorToSourceBuffer(0, 0);
        BattleFx_ApplyColorToTargetBuffer(0x10000, 0);
        BattleFx_StartBufferInterpolation(30);
        WaitFrames(1);

        resource = FunctionHead_0808e4b4(0x40000005, 8, &size);
        if (resource != NULL)
            Battle_Place(resource, gCell[125], size);

        if (ctx->field34 == 0) {
            scene->transition_phase = 0;
            scene->transition_status = 1;
            scene->transition_mode = 1;
            WaitFrames(10);
        }
    }
}

/* battle/effects/burst_particles/run_main_object.c */
void Object_SetMode(s32, s32);

void BattleFx_PrepareBufferInterpolation(void);

void BattleFx_RunBurstParticleMainObject(void)
{
    u8 *object;
    u8 *flags;
    u8 battle_value;

    object = FIELD_AT_OFFSET(*(void **)ADDR_03001F30, u8 **, 0x14);
    if (object != 0) {
        FunctionHead_08098698();
        Object_SetMode((s32)object, 2);
        object[0x59] = 0;
        Battle_Apply(object, 0);
        flags = object + 0x23;
        battle_value = 2;
        battle_value |= *flags;
        *flags = battle_value;
        WaitFrames(0xAU);
        Audio_PlayCue(0x7E);
        WaitFrames(0x28U);
        BattleFx_PrepareBufferInterpolation();
    }
}

/* battle/effects/burst_particles/run.c */
struct BurstParticleVector {
    s32 values[3];
};

u32 Random16(void);
/* LCG: seed = seed * 0x41c64e6d + 0x3039, returns bits 8-23. */
#define Rand Random16
void RotateVectorByMagnitude(s32, s32, struct BurstParticleVector *);
void *Object_Spawn(s32, s32, s32, s32);
void Object_SetCallback(void *, const void *);
extern const u8 gRom[];

void BattleFx_RunBurstParticles(void)
{
    u8 *state = (u8 *)gIw;
    struct BurstParticleVector position;
    struct BurstParticleVector *p;
    s32 entry_count;

    FunctionHead_08098698();
    Audio_PlayCue(SOUND_HEAVY_IMPACT);
    p = &position;
    entry_count = 4;
    do {
        void *object;
        s32 random_value;

        p->values[0] = *(s32 *)(state + 4);
        p->values[2] = *(s32 *)(state + 12);
        random_value = (Rand() * 6) + 0x40000;
        RotateVectorByMagnitude(random_value, Rand(), p);
        p->values[1] = *(s32 *)(state + 8);
        object = Object_Spawn(
            0xD9,
            p->values[0],
            p->values[1],
            p->values[2]
        );
        if (object != 0) {
            Object_SetCallback(object, gRom);
            *((u8 *)object + 0x55) = 2;
        }
        WaitFrames((((u32)Rand() * 2) >> 16) + 2);
        entry_count--;
    } while (entry_count >= 0);
    WaitFrames(0x1E);
    BattleFx_PrepareBufferInterpolation();
}

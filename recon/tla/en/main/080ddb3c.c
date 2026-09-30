#include "TYPES.H"
#include "SCENE.H"
#include "FIXED_MATH.H"
#include "SYSTEM.H"
void ObjectDispatch_SetSingleChildField26Far(void *, s32);
extern u8 Data_03001f30[];
void ObjectDispatch_ApplyValueToKind200Children(s32 battle_value);
s32 BattleFx_RunEventAction(void *resource, s32 battle_mode, s32 size);
void BattleEffect_SpawnBurstParticleField(void);

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

void Scheduler_RemoveCallback(void (*callback)(void));
void *BattleFx_FindMatchingEvent(u32 kind, u32 entry_index, s32 *size);
void BattleFx_ApplyColorToTargetBuffer(u32 battle_value, s32 enabled);
void BattleFx_ApplyColorToSourceBuffer(u32 battle_value, s32 enabled);
void BattleFx_StartBufferInterpolation(s32 battle_value);

void Audio_PlayCue(s32 no);
void FieldEffect_WatchLeaderDistance(void);

extern SceneTransitionContext *gEffectWork;
extern s32 gGameState[];

void BattleFx_RunBurstParticles(void)
{
    u8 *state = (u8 *)gEffectWork;
    struct BurstParticleVector position;
    struct BurstParticleVector *p;
    s32 entry_count;

    BattleEffect_SpawnBurstParticleField();
    Audio_PlayCue(SOUND_HEAVY_IMPACT);
    p = &position;
    entry_count = 4;
    do {
        void *object;
        s32 random_value;

        p->values[0] = *(s32 *)(state + 4);
        p->values[2] = *(s32 *)(state + 12);
        random_value = (Random16() * 6) + 0x40000;
        Vector_AddPolarOffset(random_value, Random16(), p);
        p->values[1] = *(s32 *)(state + 8);
        object = Object_Spawn(
            0xD9,
            p->values[0],
            p->values[1],
            p->values[2]
        );
        if (object != 0) {
            ObjectDispatch_InitializeFar(object, BattleFx_BurstParticleObjectScript);
            *((u8 *)object + 0x55) = 2;
        }
        WaitFrames((((u32)Random16() * 2) >> 16) + 2);
        entry_count--;
    } while (entry_count >= 0);
    WaitFrames(0x1E);
    BattleFx_PrepareBufferInterpolation();
}

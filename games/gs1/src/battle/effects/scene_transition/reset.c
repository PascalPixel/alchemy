#include "types.h"
#include "sound_ids.h"

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

void WaitFrames(s32 frames);
void ScheduleCallback(void (*callback)(void));
void *Func_0808e4b4(u32 kind, u32 entry_index, s32 *size);
void Func_08091200(u32 battle_value, s32 enabled);
void Func_08091220(u32 battle_value, s32 enabled);
void BattleEffect_StartBufferInterpolation(s32 battle_value);
s32 Func_08096b28(void *resource, s32 battle_mode, s32 size);
void Func_08098294(s32 battle_value);
void Func_080982dc(void);
void Audio_PlayCue(s32 soundId);

extern SceneTransitionContext *Data_03001f30;
extern s32 Data_02000240[];

#define ResetSceneTransitionEffect Func_080984c0
void ResetSceneTransitionEffect(void)
{
    SceneTransitionContext **globalSlot = &Data_03001f30;
    SceneTransitionContext *context = *globalSlot;
    SceneTransitionScene *scene = *(SceneTransitionScene **)((u8 *)globalSlot - 0x64);
    SceneTransitionState *state = *(SceneTransitionState **)((u8 *)globalSlot - 0x74);
    s16 clearedValue;
    s32 resourceSize;
    void *resource;

    if (state->active != 0) {
        Audio_PlayCue(SOUND_SCENE_TRANSITION);
        ScheduleCallback(Func_080982dc);

        clearedValue = 0;
        state->active = clearedValue;
        state->transition_timer = clearedValue;
        Func_08098294(0);

        Func_08091200(0x10000, 1);
        BattleEffect_StartBufferInterpolation(1);
        Func_08091220(0, 0);
        Func_08091200(0x10000, 0);
        BattleEffect_StartBufferInterpolation(30);
        WaitFrames(1);

        resource = Func_0808e4b4(0x40000005, 8, &resourceSize);
        if (resource != NULL)
            Func_08096b28(resource, Data_02000240[125], resourceSize);

        if (context->field34 == 0) {
            scene->transition_phase = 0;
            scene->transition_status = 1;
            scene->transition_mode = 1;
            WaitFrames(10);
        }
    }
}

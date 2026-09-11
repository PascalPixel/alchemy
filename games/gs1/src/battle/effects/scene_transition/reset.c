#include "types.h"
#include "scene.h"
#include "abi/battle/effects/scene_transition/reset.h"
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
void *Battle_Run(u32 kind, u32 entry_index, s32 *size);
void BattleFx_ApplyColorToTargetBuffer(u32 battle_value, s32 enabled);
void BattleFx_ApplyColorToSourceBuffer(u32 battle_value, s32 enabled);
void BattleFx_StartBufferInterpolation(s32 battle_value);

void Audio_PlayCue(s32 no);

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
        ScheduleCallback(Battle_Run2);

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

        resource = Battle_Run(0x40000005, 8, &size);
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

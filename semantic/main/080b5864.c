#include "types.h"

struct SceneState {
    u8 reserved00[12];
    s32 position0;
    s32 position1;
    s32 position2;
    s32 position3;
    void *alternatePosition;
    s32 depth;
    u8 reserved24[16];
    s16 angle0;
    s16 angle1;
};

struct InteractionState {
    u8 reserved00[68];
    u8 active;
    u8 reserved45[11];
    u8 selector;
    u8 delay;
    u8 pending;
};

struct AngleTransition {
    s32 target;
    s32 framesRemaining;
    u8 reserved08[12];
    s32 completionState;
};

struct FrameSlots {
    struct SceneState *scene;
    u8 reserved04[124];
    struct AngleTransition *transition;
};

struct RuntimeMemory {
    struct InteractionState *interaction;
    u8 reserved04[8];
    struct FrameSlots frame;
};

struct RenderArguments {
    s32 first;
    s32 second;
    s32 depth;
};

void Func_080049ac(void);
void Func_08004cb4(void *);
void Func_08004c1c(s32);
void Func_08004bd4(s32);
void Func_080c0a24(u32, u32, s32, s32, u32);

void Func_080b5864(void)
{
    struct FrameSlots *slots = (struct FrameSlots *)0x03001e80;
    struct SceneState *scene = slots->scene;
    struct AngleTransition *transition = slots->transition;
    struct InteractionState *interaction =
        *(struct InteractionState **)(0x03001e80 - 12);
    struct RenderArguments arguments;
    void *position;
    s32 roundedDifference;
    s16 difference;
    u16 current;

    if (interaction->active != 0) {
        if ((*(volatile u16 *)0x03001f64 & 3) != 3) {
            interaction->delay++;
            if (interaction->delay > 24)
                interaction->pending = 1;
        } else {
            u32 selector = (*(volatile u32 *)0x04000128 << 26) >> 30;

            if (interaction->selector != selector)
                interaction->pending = 1;
            interaction->delay = 0;
        }
    }

    if (transition->framesRemaining != 0) {
        current = scene->angle1;
        difference = transition->target - current;
        roundedDifference = difference;
        if (difference < 0)
            roundedDifference += 15;
        scene->angle1 = current + (roundedDifference >> 4);
        transition->framesRemaining--;
    }

    position = &scene->position0;
    if (scene->alternatePosition != NULL)
        position = scene->alternatePosition;

    Func_080049ac();
    Func_08004cb4(position);
    Func_08004c1c(scene->angle1);
    Func_08004bd4(scene->angle0);

    arguments.first = 0;
    arguments.second = 0;
    arguments.depth = scene->depth;
    ((void (*)(struct RenderArguments *, struct SceneState *))0x03000250)(
        &arguments, scene);

    if (transition->completionState == 0)
        Func_080c0a24(0x780000, 0x780000, 0, 0, 0x10000);
}

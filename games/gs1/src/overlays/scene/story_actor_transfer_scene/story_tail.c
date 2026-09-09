#include "types.h"

#define StoryReward_LookupBySelection Func_02003448
#define StoryScene_UpdateSelectedActorProgress Func_02003678
#define StoryScene_SetReferenceActor Func_020037bc
#define StoryScene_ActivateSharedState Func_020037c8
#define StoryScene_CompleteActor98 Func_020037d8

/*
 * Complete selected-actor presentation/progress synchronizer.  It derives the
 * actor presentation from the shared phase word, applies the state-one scene
 * marker, conditionally backs higher progress down, then publishes the result.
 */
struct StorySelection {
    u8 reserved000[500];
    s32 actor_id;
};

struct StorySelectionActor {
    u8 reserved00[6];
    u16 presentation;
};

struct StoryProgressWork {
    u8 reserved000[386];
    u16 state_one_marker;
};

/* Complete prologue-less shared-state activation leaf through its pool. */
struct StorySharedState {
    u8 reserved00[52];
    u8 active;
};

struct StoryCompletionWork {
    u8 reserved000[386];
    s16 scene_value;
};

extern s32 Data_0200e718[];
extern volatile s32 Data_03001e40;
extern struct StorySharedState *Data_03001f30;

s32 Func_02007708(s32);
struct StorySelectionActor *Func_02007982(s32 actor);
s32 Func_0200793e(s32 state);
s32 Func_02007944(s32 flag);
void Func_02007974(s32 state, s32 value);
void Func_02006e9e(s32 reference_actor);
void *Func_02007b28(s32 actor);
void Func_02007c22(s32 actor, s32 mode);
void Func_02007c34(s32 actor, s32 mode);
void Func_02007a90(s32 flag);
void Func_02007a92(s32 flag);
void Func_02007ab2(s32 flag, s32 value);

/* Complete reference-actor-54 selected-actor setup wrapper. */
s32 StoryReward_LookupBySelection(u32 selection)
{
    s32 flag_base = 0;
    u32 offset;

    switch (selection) {
    case 0:
        flag_base = 0x92C;
        break;
    case 1:
        flag_base = 0x935;
        break;
    case 2:
        flag_base = 0x917;
        break;
    case 3:
        flag_base = 0x990;
        break;
    }
    for (offset = 0; offset < 9; offset++) {
        if (Func_02007708(flag_base + offset) != 0) return Data_0200e718[offset];
    }
    return 0;
}

void StoryScene_UpdateSelectedActorProgress(void)
{
    extern struct StorySelection Data_02000240;
    extern struct StoryProgressWork *Data_03001ebc;

    struct StorySelectionActor *actor;
    struct StoryProgressWork *scene;
    s32 progress;

    actor = Func_02007982(Data_02000240.actor_id);
    scene = Data_03001ebc;
    actor->presentation = (u16)(Data_03001e40 << 12);

    progress = Func_0200793e(0x2f8);
    if (progress != 0) {
        if (progress == 1) {
            scene->state_one_marker = 99;
        } else if (Func_02007944(0x106) == 0) {
            progress -= 1;
        }
    }
    Func_02007974(0x2f8, progress);
}

void StoryScene_SetReferenceActor(void)
{
    Func_02006e9e(54);
}

void StoryScene_ActivateSharedState(void)
{
    Data_03001f30->active = 1;
}

/* Publish the actor-98 scene state and restore its selected actor. */
void StoryScene_CompleteActor98(void)
{
    extern u8 Data_02000240[];
    extern struct StoryCompletionWork *Data_03001ebc;

    u8 *selected_actor;

    if (Data_03001ebc->scene_value == 99) {
        Data_03001ebc->scene_value = 0;
    }
    Func_02007a92(0x2f0);
    Func_02007a90(0x2f1);
    Func_02007ab2(0x2f8, 0);
    Func_02007c22(98, 5);
    Data_02000240[0x22b] = 3;
    Func_02007c34(98, 7);
    selected_actor = Func_02007b28(*(s32 *)(Data_02000240 + 500));
    selected_actor[85] = 2;
}

#include "types.h"

/*
 * Complete selected-actor presentation/progress synchronizer.  It derives the
 * actor presentation from the shared phase word, applies the state-one scene
 * marker, conditionally backs higher progress down, then publishes the result.
 */
struct Selection_02003678 {
    u8 reserved000[500];
    s32 actor_id;
};

struct Actor_02003678 {
    u8 reserved00[6];
    u16 presentation;
};

struct SceneWork_02003678 {
    u8 reserved000[386];
    u16 state_one_marker;
};

extern struct Selection_02003678 Data_02000240;
extern struct SceneWork_02003678 *Data_03001ebc;
extern volatile s32 Data_03001e40;
struct Actor_02003678 *Func_02007982(s32 actor);
s32 Func_0200793e(s32 state);
s32 Func_02007944(s32 flag);
void Func_02007974(s32 state, s32 value);

void Func_02003678(void)
{
    struct Actor_02003678 *actor;
    struct SceneWork_02003678 *scene;
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

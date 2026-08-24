#include "types.h"

/*
 * Complete selected-actor presentation/progress synchronizer.  It derives the
 * actor presentation from the shared phase word, applies the state-one scene
 * marker, conditionally backs higher progress down, then publishes the result.
 */
struct Selection_02000400 {
    u8 reserved000[500];
    s32 actor_id;
};

struct Actor_02000400 {
    u8 reserved00[6];
    u16 presentation;
};

struct SceneWork_02000400 {
    u8 reserved000[386];
    u16 state_one_marker;
};

extern struct Selection_02000400 Data_02000240;
extern struct SceneWork_02000400 *Data_03001ebc;
extern volatile s32 Data_03001e40;
struct Actor_02000400 *Func_0200169a(s32 actor);
s32 Func_02001686(s32 state);
s32 Func_0200168c(s32 flag);
void Func_020016bc(s32 state, s32 value);

void Func_02000400(void)
{
    struct Actor_02000400 *actor;
    struct SceneWork_02000400 *scene;
    s32 progress;

    actor = Func_0200169a(Data_02000240.actor_id);
    scene = Data_03001ebc;
    actor->presentation = (u16)(Data_03001e40 << 12);

    progress = Func_02001686(0x210);
    if (progress != 0) {
        if (progress == 1) {
            scene->state_one_marker = 99;
        } else if (Func_0200168c(0x106) == 0) {
            progress -= 1;
        }
    }
    Func_020016bc(0x210, progress);
}

#include "TYPES.H"

#define Actor_UpdatePresentationFlag Func_02000324
struct SceneState { u8 unk_00[14]; s16 field_0e; };
struct Actor { u8 unk_00[35]; u8 presentation_flags; };
struct SceneState *Func_020016fa(s32);
struct Actor *Func_02001708(s32);
struct Actor *Func_02001718(s32);

void Actor_UpdatePresentationFlag(void)
{
    struct SceneState *scene = Func_020016fa(0);
    if (scene->field_0e > 31) {
        Func_02001708(20)->presentation_flags |= 2;
    } else {
        Func_02001718(20)->presentation_flags &= (u8)~2;
    }
}

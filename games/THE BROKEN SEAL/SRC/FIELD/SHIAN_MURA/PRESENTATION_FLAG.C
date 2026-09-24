#include "TYPES.H"

/*
 * Shian village: toggle the presentation flag on actor 20. While the scene
 * counter has passed the threshold the flag is set; otherwise it is cleared.
 *
 * The local SceneState and Actor layouts are this overlay's own copies of the
 * scene counter record and the field actor record (presentation_flags at
 * 0x24). The offsets here are what the reference reads, so the spelling is
 * kept as-is.
 */

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

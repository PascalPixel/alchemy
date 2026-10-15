/* Near miss: differ 2 of 32 in English, 0 of 22 in Japanese. Unpinned, the object and the frame
 * counter swap r5 and r6 (differ 14 and 12); the frame counter pinned to r6 fixes both. Left in
 * English: the native copies the motion flags' address to r8 after it starts the counter (strb;
 * movs r6, #60; mov r8, r2), here before the store. Measured with the pin: the flags address
 * as a local, an empty asm after the store or after the counter, a one-pass block around the
 * counter (2 each); alchemy permute, 4 minutes: nothing closer. */
#include "types.h"
#include "object_runtime.h"
#include "party_state.h"
#include "scene_ids.h"
#include "call.h"

struct SceneEvent;

struct ObjectRuntime *Object_GetById(s32 id);
void WaitFrames(s32 frames);
void Func_02000bc0(s32 id, s32 flag);
void Func_02000e44(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_02000f24(s32 a);

extern const u8 Data_0200102c[];
extern const u8 Data_02001044[];
extern const u8 Data_02001194[];
extern const u8 Data_020012cc[];

#ifdef TLA_EDITION_JA
void Func_02000100(s32 id)
{
    struct ObjectRuntime *object = Object_GetById(id);
    register s32 frames asm("r6"); /* FAKEMATCH: pinned, the counter takes r6 and the object r5, as in the native (12 differ unpinned). */

    object->motion_flags = 3;
    for (frames = 60; frames != 0; frames--) {
        WaitFrames(1);
        if (object->velocity_y == 0)
            break;
    }
    WaitFrames(10);
    object->motion_flags = 0;
}
#else
void Func_02000100(s32 id, s32 cue)
{
    struct ObjectRuntime *object = Object_GetById(id);
    register s32 frames asm("r6"); /* FAKEMATCH: pinned, the counter takes r6 and the object r5, as in the native (14 differ unpinned). */

    object->motion_flags = 3;
    frames = 60;
    while (frames != 0) {
        WaitFrames(1);
        frames--;
        if (object->velocity_y == 0)
            break;
    }
    if (cue != 0)
        Func_02000f24(cue);
    WaitFrames(10);
    object->motion_flags = 0;
}
#endif

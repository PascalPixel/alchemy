#include "types.h"
#include "party_state.h"
#include "scene_ids.h"
#include "object_runtime.h"

struct SceneEvent;

void                  Func_02000bc0(s32 id, s32 flag);
struct ObjectRuntime *Object_GetById(s32 id);
void                  WaitFrames(s32 frames);
void                  Func_02000f24(s32 cue);

extern const u8 gData_0200102c[];
extern const u8 gData_02001044[];
extern const u8 gData_02001194[];
extern const u8 gData_020012cc[];

const struct SceneEvent *Func_02000080(void)
{
    if (gPartyState.scene == (s32)&SceneId_Ov680Room1)
        return (const struct SceneEvent *)gData_02001044;
    if (gPartyState.scene == (s32)&SceneId_Ov680Room2)
        return (const struct SceneEvent *)gData_02001194;
    if (gPartyState.scene == (s32)&SceneId_Ov680Room3)
        return (const struct SceneEvent *)gData_020012cc;
    return (const struct SceneEvent *)gData_0200102c;
}

void Func_020000d0(void)
{
    Func_02000bc0(17, 0x300);
}

void Func_020000e0(void)
{
    Func_02000bc0(18, 0x301);
}

void Func_020000f0(void)
{
    Func_02000bc0(19, 0x302);
}

// Sets the object's motion flags to 3 while it moves, for at most 60 frames
// until its vertical speed is 0, then clears them ten frames later. The
// editions after the Japanese one also run the given cue first.
#ifdef TLA_EDITION_JA
void Func_02000100(s32 id)
{
    struct ObjectRuntime *object = Object_GetById(id);
    /* FAKEMATCH: pinned, the counter takes r6 and the object r5, as in the native (differ 12 unpinned). */
    register s32 frames asm("r6");

    object->motionFlags = 3;
    for (frames = 60; frames != 0; frames--)
    {
        WaitFrames(1);
        if (object->velocityY == 0)
            break;
    }
    WaitFrames(10);
    object->motionFlags = 0;
}
#else
void Func_02000100(s32 id, s32 cue)
{
    struct ObjectRuntime *object = Object_GetById(id);
    /* FAKEMATCH: pinned, the flags' address is built in r2 with 85 in it first (movs r2, #85; adds r2, r2, r5) and the 3 takes r3, as in the native (unpinned they swap r2 and r3: differ 10). */
    register u8 *flags asm("r2");
    u8          *held;
    /* FAKEMATCH: pinned, the counter takes r6 and the object r5, as in the native (differ 14 unpinned). */
    register s32 frames asm("r6");

    flags = (u8 *)85;
    /* FAKEMATCH: the empty asm keeps 85 in a register, so it is added to the object rather than the object copied and 85 added (differ 4 without it). */
    asm("" : "+r"(flags));
    flags += (s32)object;
    *flags = 3;
    frames = 60;
    /* FAKEMATCH: a forced temporary: the address is copied to r8 for the calls after the counter starts, as in the native; kept in one variable, its reload copy comes before the store (differ 2). */
    held = flags;
    while (frames != 0)
    {
        WaitFrames(1);
        frames--;
        if (object->velocityY == 0)
            break;
    }
    if (cue != 0)
        Func_02000f24(cue);
    WaitFrames(10);
    *held = 0;
}
#endif

/* Draft, differ 94 of 155: ☀️'s Title_Run for overlay 649 with ⚓️'s two
 * extra entrances (2 and 3) and scene ids Title and Ov650 not yet named in
 * scenes.s (with ResourceId_FarCall00B in resource/directory.s). Left: the
 * native rebuilds the 3599 bound and reloads gInput in both wait loops (a for
 * loop hoists them; a goto loop keeps them but compares 3600), re-reads the
 * entrance with ldrsh before the third test and builds 0x226 from 0x225. */
#include "types.h"
#include "input.h"
#include "object_runtime.h"
#include "party_state.h"
#include "scene_ids.h"

extern char SceneId_Title;
extern char SceneId_Ov650;

extern char ResourceId_FarCall00B;

struct ObjectRuntime *Object_GetById(s32 id);
void WaitFrames(s32 frames);
void Battle_WaitMode0(s32 frames);
void Blend_SetDarkenTarget16(s32 frames);
void Blend_WaitForTransition(void);
void Func_0200031c(s32 unused);
void Func_020005bc(s32 resource);
s32 Func_020005d4(void);
void Func_020005dc(s32 a, s32 b);
void Func_020005e4(void);
void Func_020005ec(void);
void Func_02000604(s32 scene, s32 entrance);
void Func_0200061c(s32 value);
s32 Func_02000624(s32 value);
void Func_0200062c(s32 value);
void Func_02000634(s32 value);
void Func_0200063c(s32 value);
void Func_02000644(s32 cue);

s32 Func_0200005c(void)
{
    s32 wait;

    if (gPartyState.entrance == 10) {
        Object_GetById(gPartyState.current_owner)->motion_flags = 0;
        Func_02000644(75);
        Func_0200031c(0);
        WaitFrames(120);
        for (wait = 0; wait <= 3599; wait++) {
            WaitFrames(1);
            if (gInput.pressed != 0)
                break;
        }
        Func_02000604((s32)&SceneId_Title, 2);
        return 0;
    }
    if (gPartyState.entrance == 8) {
        Func_02000644(68);
        Func_0200061c(1);
        Func_02000644(78);
        Blend_SetDarkenTarget16(60);
        Blend_WaitForTransition();
        Battle_WaitMode0(240);
        Func_02000644(0);
        Func_02000604((s32)&SceneId_Title, 2);
        return 0;
    }
    Func_020005bc((s32)&ResourceId_FarCall00B);
    if (gPartyState.entrance == 1) {
        for (;;) {
            Func_02000644(0);
            Func_0200062c(0);
            Func_02000634(0);
            if (Func_020005d4() > 0) {
                Func_02000644(40);
                Func_0200063c(1);
                if (Func_02000624(1) == 0) {
                    Func_02000644(78);
                    Blend_SetDarkenTarget16(30);
                    Blend_WaitForTransition();
                    for (wait = 0; wait <= 119; wait++) {
                        WaitFrames(1);
                        if (gInput.pressed != 0)
                            break;
                    }
                    continue;
                }
            }
            break;
        }
        Func_02000604((s32)&SceneId_Ov64a, 1);
    } else {
        if (gPartyState.entrance == 2) {
            Func_02000644(40);
            Func_0200063c(0);
            Func_020005dc(gPartyState.unknown_220[5], gPartyState.unknown_220[6]);
            Func_020005e4();
            Func_02000604((s32)&SceneId_VinasuChojo, 99);
        }
        if (gPartyState.entrance == 3) {
            Func_02000644(75);
            Func_02000624(0);
            Func_020005dc(gPartyState.unknown_220[5], gPartyState.unknown_220[6]);
            Func_020005ec();
            Func_02000604((s32)&SceneId_Ov650, 13);
        }
    }
    Func_02000644(78);
    Blend_SetDarkenTarget16(30);
    Blend_WaitForTransition();
    Battle_WaitMode0(60);
    Func_02000644(0);
    return 0;
}

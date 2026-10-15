/* Near miss, differ 4 (one reordered row): the twin of The Broken Seal's
 * BattleFx_UpdatePairedArcSpawner. The listing loads the gPartyState literal
 * before it stores the advanced step; this source stores first. */
#include "types.h"
#include "party_state.h"
#include "scene_ids.h"

struct ArcEffectObject {
    u8 unk00[6];
    u16 angle;
    u8 unk08[0x5c];
    s16 phase;
    u16 step;
};

s32 __modsi3(s32, s32);
void Func_080e1024(struct ArcEffectObject *source);

void Func_080e1154(struct ArcEffectObject *obj)
{
    s16 phase;
    s16 cnt;

    phase = obj->phase;
    cnt = obj->step++;

    if (gPartyState.map == (s32)&SceneId_Ov64a) {
        if (__modsi3(cnt, 7) == 0)
            Func_080e1024(obj);
    } else if (__modsi3(cnt, 5) == 0) {
        Func_080e1024(obj);
    }

    if (phase == 1)
        obj->angle += 0xc00;
}

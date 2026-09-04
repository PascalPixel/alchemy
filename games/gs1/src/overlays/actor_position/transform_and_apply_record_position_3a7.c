#include "types.h"

#define SceneActor_TransformAndApplyRecordPosition Func_02000b54

extern void Func_02001bfc(s32, s32, s32 *);
extern void Func_02001c38(s32 *, s32, s32, s32);

void SceneActor_TransformAndApplyRecordPosition(s32 *rec, s32 v0, s32 v1)
{
    s32 pos[3];

    if (rec == 0) {
        return;
    }
    pos[0] = rec[2];
    pos[1] = rec[3];
    pos[2] = rec[4];
    Func_02001bfc(v0, v1, pos);
    Func_02001c38(rec, pos[0], pos[1], pos[2]);
}

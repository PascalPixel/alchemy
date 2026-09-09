#include "types.h"

extern s16 Data_02000240[];
extern u8 Value_000000ac;
extern u8 *Data_03001ee0;
s32 Func_02003f32(s32 id);

#define SceneState_LinkActorZeroToWork24BySubstateY Func_02000e20

void SceneState_LinkActorZeroToWork24BySubstateY(void)
{
    s32 actor;
    u8 *work;
    s32 thresh;
    s16 sub;

    actor = Func_02003f32(0);
    work = Data_03001ee0;
    thresh = 0;
    if (Data_02000240[224] == (s32)&Value_000000ac) {
        sub = Data_02000240[225];
        switch (sub) {
        case 3:
        case 4:
            thresh = 94;
            break;
        case 5:
        case 6:
        case 7:
            break;
        case 8:
        case 9:
            thresh = 74;
            break;
        case 10:
        case 11:
            break;
        case 12:
        case 13:
            thresh = 118;
            break;
        }
    } else if (Data_02000240[225] == 12) {
        thresh = 93;
    }
    if ((*(s32 *)(actor + 16) >> 19) <= thresh) {
        *(s32 *)(work + 24) = 0;
    } else {
        *(s32 *)(work + 24) = actor;
    }
}

#include "colosso_log_rolling_stage.h"


typedef struct ScaledStageObject {
    u8 pad00[12];
    s32 y;                      /* +12 */
    u8 pad10[32];
    s32 scale_x;                /* +48 */
    s32 scale_z;                /* +52 */
    u8 pad38[35];
    u8 state;                   /* +91 */
} ScaledStageObject;

ScaledStageObject *Func_020086b0();
void Func_02008464();
void Func_0200844c();
void Func_02008488();

void ColossoLogRollingStage_PositionScaledObject(s32 id, s32 x, s32 z)
{
    ScaledStageObject *object = Func_020086b0(id);
    s32 scale;

    if (object != 0) {
        scale = 0x20000;
        object->scale_x = scale;
        object->scale_z = scale / 2;
        object->state = 0;
        Func_02008464(object);
        Func_0200844c(object, 5);
        Func_02008488(object, x << 16, object->y, z << 16);
    }
}

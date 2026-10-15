//--------------------------------------------------------------------------
// motion_set_position_and_reset_motion.c
// オブジェクト：位置の設定と移動の取り消し
//--------------------------------------------------------------------------

#include "object_runtime.h"

//--------------------------------------------------------------------------
// 位置を置き、移動先を「なし」に、速度を 0 にする
// obj     : オブジェクト
// x, y, z : 位置（16.16 固定小数）
//--------------------------------------------------------------------------
void Object_SetPositionAndResetMotion(struct ObjectRuntime *obj, s32 x, s32 y, s32 z)
{
    obj->z = z;
    obj->targetX = 0x80000000; // 移動先なし
    obj->targetY = 0x80000000;
    obj->targetZ = 0x80000000;
    obj->x = x;
    obj->y = y;
    obj->velocityX = 0;
    obj->velocityY = 0;
    obj->velocityZ = 0;
}

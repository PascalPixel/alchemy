//--------------------------------------------------------------------------
// setup_transition_at_pair_midpoint.c
// バトル表示：ふたりの中間へのカメラ合わせ
//--------------------------------------------------------------------------

#include "types.h"
#include "motobj.h"
#include "battle_display.h"

//--------------------------------------------------------------------------
// ふたつのオブジェクトの中間（x, z）にカメラを合わせる
// a, b : オブジェクト番号
// zoom : 拡大率（%）
//--------------------------------------------------------------------------
void BattlePres_SetupTransitionAtPairMidpoint(s32 a, s32 b, s32 zoom)
{
    struct MotionObject *obj_a = BattleObjGet(a)->object;
    struct MotionObject *obj_b = BattleObjGet(b)->object;
    s32                  ax = obj_a->x;
    s32                  bx = obj_b->x;
    s32                  az = obj_a->z;
    s32                  bz = obj_b->z;
    s32                  x = (bx + ax) / 2;
    s32                  z = (bz + az) / 2;

    BattlePres_SetupTransitionScene(x, 0, z, zoom);
}

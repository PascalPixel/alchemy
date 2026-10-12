#include "types.h"
#include "iwram_call.h"

struct Chase {
    s32 x;
    s32 z;
    s32 unknown_08;
    s32 target_x;
    s32 target_z;
};

s32 ArcTan2(s32 y, s32 x);
s32 Trig_Sin(s32 angle);
s32 Trig_Cos(s32 angle);

// 目標へ speed だけ近づけ、通り過ぎたら目標に合わせる
void Func_080e9ee4(struct Chase *chase, s32 speed)
{
    s32 dx = chase->target_x - chase->x;
    s32 dz = chase->target_z - chase->z;
    u16 angle = ArcTan2(dz, dx);

    chase->x += Iwram_MulQ16(Trig_Cos(angle), speed);
    chase->z += Iwram_MulQ16(Trig_Sin(angle), speed);
    {
        s32 next_dx = chase->target_x - chase->x;
        s32 next_dz = chase->target_z - chase->z;

        if ((next_dx ^ dx) & 0x80000000)
            chase->x = chase->target_x;
        if ((dz ^ next_dz) & 0x80000000)
            chase->z = chase->target_z;
    }
}

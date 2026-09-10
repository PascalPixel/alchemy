#include "types.h"

#define ActorPresentation_SetSceneCellByFlag985 Func_0200246c
#define ActorPresentation_PlaceActorTwelveAtTile20And12 Func_020026b8
s32 Func_0200528c();
void Func_0200526c();
void Func_02005282();
s32 *Func_02005510();
void Func_020054cc();

void ActorPresentation_SetSceneCellByFlag985(void)
{
    if (Func_0200528c(0x985) == 0) {
        s32 k5 = 17, k6 = 78;

        Func_0200526c(36, 78, 1, 2, k5, k6);
    } else {
        s32 k5 = 17, k6 = 78;

        Func_02005282(34, 78, 1, 2, k5, k6);
    }
}

void ActorPresentation_PlaceActorTwelveAtTile20And12(void)
{
    s32 *p = Func_02005510(12);
    s32 a = p[2] >> 20;

    if (a == 20) {
        s32 b = p[4] >> 20;

        if (b == 12) {
            ((u8 *)p)[85] = 2;
            p[5] = 0x300000;
            ((u8 *)p)[35] = 2;
            {
                s32 k5 = a, k6 = b;

                Func_020054cc(38, 12, 1, 1, k5, k6);
            }
        }
    }
}

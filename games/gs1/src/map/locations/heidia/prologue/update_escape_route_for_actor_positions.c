#include "types.h"

struct S {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

extern struct S *Func_02004c4c(s32 arg0);
extern struct S *Func_02004c56(s32 arg0);
extern struct S *Func_02004c60(s32 arg0);
extern struct S *Func_02004c6a(s32 arg0);
extern struct S *Func_02004cda(s32 arg0);
extern void Func_02003376(s32 arg0, s32 arg1, s32 arg2);
extern void Func_02003386(s32 arg0, s32 arg1, s32 arg2);
extern void Func_020033b2(s32 arg0, s32 arg1, s32 arg2);
extern void Func_02003758(void);
extern void Func_02004c04(s32 arg0);
extern void Func_02004c90(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
extern void Func_02004ca0(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);

void UpdateEscapeRouteForActorPositions(void) {
    s32 actor_x = Func_02004c4c(8)->f08 >> 20;
    s32 actor_z = Func_02004c56(8)->f10 >> 20;
    s32 actor_12_x = Func_02004c60(12)->f08 >> 20;
    s32 actor_15_x = Func_02004c6a(15)->f08 >> 20;

    if (actor_z == 19) {
        if (actor_12_x == 24) {
            Func_020033b2(8, 0, -80);
        } else if (actor_15_x == 24) {
            Func_02003376(8, 0, -112);
            Func_020033b2(8, 0, -32);
        } else {
            Func_02003386(8, 0, -80);
            Func_020033b2(8, 0, -112);
        }
    } else if (actor_z == 14) {
        if (actor_12_x == 24) {
            return;
        }
        if (actor_15_x == 24) {
            Func_020033b2(8, 0, -64);
        } else {
            Func_020033b2(8, 0, -112);
        }
    } else if (actor_z == 10) {
        if (actor_15_x == 24) {
            return;
        }
        Func_020033b2(8, 0, -48);
    } else {
        Func_02003758();
        return;
    }
    Func_02004c04(2);
    {
        s32 route_end_z = Func_02004cda(8)->f10 >> 20;
        s32 route_x = actor_x - 1;

        Func_02004c90(route_x, actor_z, 3, 1, route_x, route_end_z);
    }
    Func_02004ca0(0, 0, 3, 1, actor_x - 1, actor_z);
}

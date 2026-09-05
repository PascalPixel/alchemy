#include "types.h"

struct SceneObject {
    u8 filler00[6];
    u16 angle;
    s32 x;
    s32 y;
    s32 z;
    u8 filler14[0x14];
    s32 accel;
    u8 filler2c[4];
    s32 scale_x;
    s32 scale_y;
    u8 filler38[0x1d];
    u8 state;
};

struct Vec {
    s32 x;
    s32 y;
    s32 z;
};

union VecView {
    struct Vec v;
    s16 h[6];
};

extern struct SceneObject *Func_02005b84(s32);
extern void Func_02005ae6(s32, s32, struct Vec *);
extern s32 Func_02005b5e(struct SceneObject *, struct Vec *);
extern s32 Func_0200105a(struct Vec *, struct SceneObject *);
extern void Func_02005b2c(s32, s32, struct Vec *);
extern s32 Func_02001094(struct Vec *, struct SceneObject *);
extern s32 Func_02005bb0(struct SceneObject *, struct Vec *);
extern void Func_02005c10(void);
extern void Func_02005b58(struct SceneObject *, s32);
extern void Func_02005b2e(s32);
extern void Func_02005d8c(s32);
extern void Func_02005b6c(struct SceneObject *, s32);
extern void Func_02005c02(struct SceneObject *, s32);
extern void Func_02005ca0(s32, s32, s32);
extern void Func_02005ba8(struct SceneObject *, s32);
extern void Func_02005c20(struct SceneObject *, s32);
extern void Func_02005c82(void);

s32 SceneActor_TryMoveActorZeroTwoTilesAhead(void)
{
    struct SceneObject *obj;
    struct Vec vec;
    u8 *state;
    u8 old;
    s32 m;

    obj = Func_02005b84(0);
    state = &obj->state;
    old = *state;
    vec.x = (obj->x & 0xfff00000) + 0x80000;
    vec.y = obj->y;
    vec.z = (obj->z & 0xfff00000) + 0x80000;
    m = (obj->angle + 0x2000) & 0xc000;
    Func_02005ae6(0x100000, m, &vec);
    if (Func_02005b5e(obj, &vec) != 1 && Func_0200105a(&vec, obj) == 0) {
        vec.x = (obj->x & 0xfff00000) + 0x80000;
        vec.y = obj->y;
        vec.z = (obj->z & 0xfff00000) + 0x80000;
        Func_02005b2c(0x200000, (obj->angle + 0x2000) & 0xc000, &vec);
        if (Func_02001094(&vec, obj) == 0 && Func_02005bb0(obj, &vec) == 0) {
            Func_02005c10();
            Func_02005b58(obj, 6);
            Func_02005b2e(6);
            Func_02005d8c(152);
            Func_02005b6c(obj, 7);
            obj->scale_x = 0x30000;
            obj->scale_y = 0x20000;
            obj->accel = 0x40000;
            *state &= 0x7e;
            Func_02005c02(obj, 0);
            Func_02005ca0(0, ((union VecView *)&vec)->h[1], ((union VecView *)&vec)->h[5]);
            Func_02005ba8(obj, 6);
            Func_02005c20(obj, 1);
            *state = old;
            Func_02005c82();
            return 1;
        }
    }
    return 0;
}

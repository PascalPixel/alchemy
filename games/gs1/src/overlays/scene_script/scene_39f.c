#include "types.h"

#define FieldScene_RunActorNinePlacementScene Func_02000f94

struct PlacementTail {
    s32 target_y;
    void (*callback)(void);
};

struct PlacementRequest {
    s32 movement_index;
    s32 actor_id;
    s32 target_x;
    s32 target_depth;
    struct PlacementTail tail;
};

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
s32 Func_02001416(s32 *a);
void Func_02003d3e(void);
void Func_02003d64(s32);
void Func_02003d7a();
void Func_02003d94(s32);
void Func_02003daa(s32, s32, s32);
u8 *Func_02003dd0(s32);
void Func_02003dd4(s32, s32);
void Func_02003dd6(s32, s32, s32);
void Func_02003dde(void);
void Func_02003dfc(s32, s32);
void Func_02003e4a(s32, s32);
void Func_02003eba(s32);
void Func_020015c2(struct PlacementRequest);

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

void FieldScene_RunActorNinePlacementScene(void)
{
    struct PlacementRequest request;

    Func_02003d3e();
    if (Func_02001416((s32 *)&request) != 0) {
        Func_020015c2(request);
        if (request.actor_id == 9) {
            if ((request.tail.target_y >> 20) == 26) {
                Call1(Func_02003d64, 0x310);
                Func_02003dd4(9, 3);
                Call3(Func_02003daa, 9, 0x4000, 0x8000);
                Call3(Func_02003dd6, 9, 0, -16);
                Func_02003d94(45);
                Func_02003dfc(9, 8);
                Func_02003eba(240);
                Func_02003e4a(9, 1);
                *(u8 *)(Func_02003dd0(9) + 35) = 2;
                Call6(Func_02003d7a, 38, 27, 4, 2, 31, 25);
            }
        }
    }
    Func_02003dde();
}

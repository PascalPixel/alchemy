#include "types.h"

#define FieldScene_RunActorsNineAndElevenPlacementScene Func_020019b8

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
s32 Func_02001e3c(s32 *a);
void Func_02001fe8(struct PlacementRequest);
void Func_02004762(void);
void Func_0200474e(s32, s32, s32, s32, s32, s32);
void Func_02004768(s32, s32, s32, s32, s32, s32);
void Func_02004794(s32, s32, s32, s32, s32, s32);
void Func_020047c0(s32);
void Func_020047ec(s32);
void Func_020047fc(s32, s32, s32, s32, s32, s32);
void Func_0200480c(void);
void Func_02004816(s32);
u8 *Func_0200484a(s32);
void Func_02004858(s32, s32, s32);
void Func_02004860_a(s32, s32);
void Func_02004860_b();
void Func_02004866(s32, s32);
void Func_0200487e(s32, s32);
void Func_0200488c(s32, s32);
void Func_0200493c(s32);

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

void FieldScene_RunActorsNineAndElevenPlacementScene(void)
{
    struct PlacementRequest request;
    s32 done = 0;

    Func_02004762();
    if (Func_02001e3c((s32 *)&request) != 0) {
        Func_02001fe8(request);
        if (request.actor_id != 9 && request.actor_id != 11) {
            goto actor_eight;
        }
        if (request.actor_id == 9) {
            Call6(Func_0200474e, 38, 68, 1, 4, (request.target_x >> 20), 68);
            if ((request.target_x >> 20) != 42) {
                goto after_block;
            }
            Call6(Func_02004768, 26, 20, 2, 4, (request.target_x >> 20), 23);
            Func_02004860_a(9, 1);
            done = 1;
            Call1(Func_020047c0, 0x312);
            goto after_block;
        }
        if ((request.target_x >> 20) == 40) {
            Call6(Func_02004794, 26, 20, 2, 4, (request.target_x >> 20), 32);
            Func_0200488c(11, 1);
            done = 1;
            Call1(Func_020047ec, 0x313);
        }
        after_block:;
        if (done == 0) {
            Func_0200480c();
            goto skip_final;
        }
        Func_02004866(request.actor_id, 3);
        Func_02004858(request.actor_id, 18, 6);
        Func_02004816(30);
        Func_0200487e(request.actor_id, 8);
        Func_0200493c(240);
        *(u8 *)(Func_0200484a(request.actor_id) + 35) = 2;
        goto exit;
        actor_eight:;
        if (request.actor_id == 8) {
            Call6(Func_020047fc, 42, 49, 1, 4, (request.target_x >> 20), 49);
        }
    }
    exit:;
    Func_02004860_b();
    skip_final:;
}

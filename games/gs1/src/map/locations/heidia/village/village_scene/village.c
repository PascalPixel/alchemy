#include "types.h"

#define StagedActorStepTable Data_0200ded8
#define GetStagedActor Func_02005724
#define FindNextStagedActor Func_02000176
#define FindBlockingStagedActor Func_020001a2
#define FindElevatedBlockingStagedActor Func_020001ce
#define CanStartStagedActorMove Func_02005780
#define SetStagedActorMode Func_02005760
#define SelectStagedActorSlot Func_02005748
#define StartStagedActorEffect Func_0200596e
#define StartNextStagedActorMove Func_02005796
#define StartLeadStagedActorMove Func_020057a6
#define FinishStagedActorMove Func_020057b4
#define FinishStagedActorEffect Func_02005990
#define SetStagedActorTransition Func_020057cc
#define AdvanceStagedActorPair Func_020000c4
#define FindClearActorPosition Func_02000474
#define StartDoraHouseAccidentScene Func_020010a8
#define TurnActorToSceneDirection Func_02001be4

#include "staged_actor.h"

typedef struct Obj {
    u8 pad0[8];
    s32 x;          /* 0x08 */
    s32 y;          /* 0x0c */
    s32 z;          /* 0x10 */
} Obj;

typedef struct Req {
    s32 kind;       /* 0x00 */
    s32 f4;         /* 0x04 */
    s32 x;          /* 0x08 */
    s32 y;          /* 0x0c */
    s32 z;          /* 0x10 */
    s32 f14;        /* 0x14 */
} Req;

extern s32 Data_0200df30[];
extern s32 Value_00000f2e;
extern s32 Data_0200dfa4;

struct StagedActor *Func_02005724(s32 arg0);
struct StagedActor *Func_02000176(s32 *arg0, struct StagedActor *arg1);
struct StagedActor *Func_020001a2(s32 *arg0, struct StagedActor *arg1);
struct StagedActor *Func_020001ce(s32 *arg0, struct StagedActor *arg1);
s32 Func_02005780(struct StagedActor *arg0, s32 *arg1);
void Func_02005760(struct StagedActor *arg0, s32 arg1);
void Func_02005748(s32 arg0);
void Func_0200596e(s32 arg0);
void Func_02005796(struct StagedActor *arg0, s32 arg1, s32 arg2, s32 arg3);
void Func_020057a6(struct StagedActor *arg0, s32 arg1, s32 arg2, s32 arg3);
void Func_020057b4(struct StagedActor *arg0);
void Func_02005990(void);
void Func_020057cc(struct StagedActor *arg0, s32 arg1);
Obj *Func_020007de(s32 *, s32 *, Req *);
s32 Func_02005b3c(Obj *, s32 *);
void Func_02006736(s32, s32, s32);
void Func_02006754(s32);
void Func_020067d0(s32, s32, s32);
void Func_020066f6(s32);
void Func_0200677e(s32, s32);
void Func_02006866(s32, s32);
void Func_0200678e(s32, s32);
void Func_0200672e(s32, s32);
void Func_0200670c(s32);
void Func_0200679e(s32, s32, s32);
void Func_020072be(s32, s32, s32);
void Func_020072c8(s32, s32, s32);
void Func_0200386a(s32);
void Func_020038ca(s32);
void Func_0200391e(s32);
void Func_02003996(s32);
void Func_020038e2(s32);

void AdvanceStagedActorPair(void)
{
    extern u32 Data_0200ded8[];

    s32 destination[3];
    struct StagedActor *lead_actor;
    struct StagedActor *next_actor;
    struct StagedActor *blocking_actor;
    s32 facing_index;
    u32 step;
    s32 move_rate;
    s32 zero;

    lead_actor = GetStagedActor(0);
    facing_index = lead_actor->direction_and_kind >> 12;
    step = StagedActorStepTable[facing_index];
    destination[0] = lead_actor->x.value + (step & 0xffff0000);
    destination[1] = lead_actor->y;
    step <<= 16;
    destination[2] = lead_actor->z.value + step;
    next_actor = FindNextStagedActor(destination, lead_actor);
    if (next_actor == 0) return;

    step = StagedActorStepTable[facing_index];
    destination[0] = next_actor->x.value + (step & 0xffff0000);
    destination[1] = next_actor->y;
    step <<= 16;
    destination[2] = next_actor->z.value + step;
    blocking_actor = FindBlockingStagedActor(destination, next_actor);
    if (blocking_actor != 0 && (blocking_actor->collision_flags & 1) != 0) return;

    destination[0] = next_actor->x.value;
    destination[1] = next_actor->y + 0x100000;
    destination[2] = next_actor->z.value;
    blocking_actor = FindElevatedBlockingStagedActor(destination, next_actor);
    if (blocking_actor != 0 && (blocking_actor->collision_flags & 1) != 0) return;

    next_actor->transition_mode = 2;
    step = StagedActorStepTable[facing_index];
    destination[0] = next_actor->x.value + (step & 0xffff0000);
    destination[1] = next_actor->y;
    step <<= 16;
    destination[2] = next_actor->z.value + step;
    if (CanStartStagedActorMove(next_actor, destination) > 0) return;

    zero = next_actor->transition_busy;
    if (zero != 0) return;

    SetStagedActorMode(lead_actor, 8);
    move_rate = 0x3333;
    SelectStagedActorSlot(15);
    StartStagedActorEffect(185);
    next_actor->move_rate_x = move_rate;
    next_actor->move_rate_z = move_rate;
    StartNextStagedActorMove(next_actor, destination[0], destination[1], destination[2]);
    lead_actor->move_rate_x = move_rate;
    lead_actor->move_rate_z = move_rate;
    StartLeadStagedActorMove(lead_actor, destination[0], destination[1], destination[2]);
    FinishStagedActorMove(next_actor);
    FinishStagedActorEffect();
    next_actor->x.value = destination[0];
    next_actor->z.value = destination[2];
    next_actor->unknown_24 = zero;
    next_actor->unknown_2c = zero;
    lead_actor->unknown_38 = 0x80000000;
    lead_actor->unknown_40 = 0x80000000;
    lead_actor->unknown_24 = zero;
    lead_actor->unknown_2c = zero;
    lead_actor->x.value = lead_actor->x.parts.cell << 16;
    lead_actor->z.value = lead_actor->z.parts.cell << 16;
    SetStagedActorTransition(lead_actor, 1);
}

s32 FindClearActorPosition(Req *request)
{
    extern s32 Data_0200ded8[];

    s32 out;
    s32 vec[3];
    Obj *r;
    s32 n;
    s32 nx, ny;
    u8 *flag;
    s32 i, j;
    s32 w1, w2;
    s32 off;

    request->f14 = 0;
    r = Func_020007de(&out, &request->f4, request);
    if (r == 0) {
        return 0;
    }
    flag = (u8 *)r + 0x22;
    *flag = 2;
    {
        s32 k = request->kind;

        n = 0;
        off = k * 4;
        w1 = Data_0200df30[off + 1];
        if (w1 < 0) {
            w1 = -w1;
        }
        w2 = Data_0200df30[off + 3];
        if (w2 < 0) {
            w2 = -w2;
        }
        ny = (w1 + w2) >> 4;
        w1 = Data_0200df30[off];
        if (w1 < 0) {
            w1 = -w1;
        }
        w2 = Data_0200df30[off + 2];
        if (w2 < 0) {
            w2 = -w2;
        }
        nx = (w1 + w2) >> 4;
    }
    vec[0] = r->x + (Data_0200ded8[out] & 0xffff0000);
    {
        s32 ry = r->y;

        vec[1] = ry;
        vec[2] = r->z + (Data_0200ded8[out] << 16);
        request->y = ry;
    }
    for (;;) {
        request->z = vec[2] + (Data_0200df30[request->kind * 4 + 1] << 16);
        for (j = 0; j < ny; j++) {
            request->x = vec[0] + (Data_0200df30[request->kind * 4] << 16);
            for (i = 0; i < nx; i++) {
                if (Func_02005b3c(r, &request->x) == 2) {
                    goto found;
                }
                request->x += 0x100000;
            }
            request->z += 0x100000;
        }
        n++;
        vec[0] += Data_0200ded8[out] & 0xffff0000;
        vec[2] += Data_0200ded8[out] << 16;
    }
found:
    *flag = 0;
    if (n == 0) {
        return 0;
    }
    request->x = r->x + (Data_0200ded8[out] & 0xffff0000) * n;
    request->y = r->y;
    request->z = r->z + (Data_0200ded8[out] << 16) * n;
    return 1;
}

void StartDoraHouseAccidentScene(void)
{
    Func_02006736(0, 0x108, 0x318);
    Func_02006754(0);
    Func_020067d0(0, 0x4000, 0);
    Func_020066f6(10);
    Func_0200677e(0, 1);
    Func_02006866(0xea, 3);
    Func_0200678e(0, 1);
    Func_0200672e(0xea, 0);
    Func_0200670c((s32)&Value_00000f2e);
    Func_0200679e(8, 0, 0);
}

void TurnActorToSceneDirection(s32 actor_id)
{
    Func_020072be(actor_id, 0, 0);
    Func_020072c8(0, actor_id, 0);
    switch (Data_0200dfa4 & 3) {
    case 0:
        Func_0200386a(actor_id);
        break;
    case 1:
        Func_020038ca(actor_id);
        break;
    case 2:
        Func_0200391e(actor_id);
        break;
    case 3:
        Func_02003996(actor_id);
        break;
    default:
        Func_020038e2(actor_id);
        break;
    }
}

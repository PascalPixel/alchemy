/* NONMATCHING: 288 of 288 bytes, 6 differing halfwords (2026-09-24); twin of
 * 39c:02005458. Unit source for 39b with absolute symbols Data_02000240_t
 * (0x02000240, data) plus the evconv Engine_* veneers. Remaining: the
 * reference keeps the obj+104 owner store ahead of the obj[85] store and the
 * obj+12 load (sched2 sees them as conflicting there); here the store sinks
 * below the y update. The globals - 8 spelling reproduces the reference's
 * derived 0x03001ebc (FAKEMATCH candidate). */
#include "TYPES.H"

void Engine_EventBegin(void);
u8 *Engine_ObjectCreate(s32 kind, s32 x, s32 y, s32 z);
void Engine_ObjectSetScript(u8 *obj, s32 script);
void Local_02001df8(void);
void Engine_TaskWait(s32 frames);
void Engine_ObjectSetAnimation(u8 *obj, s32 anim);
void Engine_GameFlagSet(s32 flag);
void Engine_EventEnd(void);

extern s32 Data_02000240_t[];

struct Sprite39b {
    u8 pad[9];
    u8 lo : 2;
    u8 layer : 2;
    u8 hi : 4;
};

struct Work {
    u8 pad[20];
    u8 *actors[1];
};

struct Flags85 {
    u8 pad[85];
    u8 flags;
};

struct PillarState {
    s32 step;
    u8 pad04[8];
    s32 x;
    s32 z;
    u8 *pillar;
};

/* Raise the pillar under the party leader in the remaining steps, then record
 * the cell it now stands on. */
void MakyuriIriguchi_Func02001ec8(void)
{
    struct PillarState *state;
    u8 *leader;
    u8 *pillar;
    u8 *obj;
    struct Sprite39b *spr;
    s32 step;

    {
        u8 **globals = (u8 **)0x03001edc;

        state = *(struct PillarState **)globals[0];
        leader = (*(struct Work **)(globals - 8))->actors[Data_02000240_t[125]];
    }
    if ((u32)state->step > 2)
        return;
    Engine_EventBegin();
    pillar = state->pillar;
    if (pillar == 0) {
        obj = Engine_ObjectCreate(26, *(s32 *)(leader + 8), *(s32 *)(leader + 12) + 0x180000, *(s32 *)(leader + 16));
        if (obj != 0) {
            spr = *(struct Sprite39b **)(obj + 80);
            *(s32 *)(obj + 20) = *(s32 *)(leader + 20);
            Engine_ObjectSetScript(obj, 0x200a7e8);
            *(u8 **)(obj + 104) = leader;
            ((struct Flags85 *)obj)->flags = 4;
            *(s32 *)(obj + 12) += -0x8000;
            if (spr != 0) {
                ((u8 *)spr)[38] = 0;
                spr->layer = 1;
            }
            obj[84] = 0;
            state->pillar = obj;
            pillar = obj;
        } else {
            pillar = state->pillar;
        }
    }
    for (step = state->step; step <= 2; step++) {
        Local_02001df8();
        Engine_TaskWait(30);
        pillar[84] = 1;
        Engine_ObjectSetAnimation(pillar, 5 - step);
    }
    state->step = 3;
    state->x = (*(s32 *)(pillar + 8) & -0x100000) + 0x80000;
    state->z = (*(s32 *)(pillar + 16) & -0x100000) + 0x80000;
    Engine_GameFlagSet(0x161);
    Engine_EventEnd();
}

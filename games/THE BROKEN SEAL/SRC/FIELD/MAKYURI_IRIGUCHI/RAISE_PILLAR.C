/* Instanced unit: 39b:02001ec8 and its twin 39c:02005458; the pillar script
 * and the rumble routine are per-overlay absolute symbols. */
#include "TYPES.H"

void Engine_EventBegin(void);
u8 *Engine_ObjectCreate(s32 kind, s32 x, s32 y, s32 z);
void Engine_ObjectSetScript(u8 *obj, s32 script);
void Pillar_Rumble(void);
void Engine_TaskWait(s32 frames);
void Engine_ObjectSetAnimation(u8 *obj, s32 anim);
void Engine_GameFlagSet(s32 flag);
void Engine_EventEnd(void);

extern u8 Pillar_Script[];
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

struct Obj {
    u8 pad00[12];
    s32 y;
    u8 pad10[69];
    u8 f85;
    u8 pad56[18];
    u8 *owner;
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
void MakyuriIriguchi_RaisePillar(void)
{
    struct PillarState *state;
    u8 *leader;
    u8 *pillar;
    u8 *obj;
    struct Sprite39b *spr;
    s32 step;

    /* FAKEMATCH: the work pointer is read as globals - 8 so the reference
     * derives 0x03001ebc from the loaded 0x03001edc. */
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
            Engine_ObjectSetScript(obj, (s32)Pillar_Script);
            ((struct Obj *)obj)->owner = leader;
            ((struct Obj *)obj)->f85 = 4;
            ((struct Obj *)obj)->y += -0x8000;
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
        Pillar_Rumble();
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

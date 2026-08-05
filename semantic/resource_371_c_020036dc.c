#include "types.h"

/*
 * Complete selected-actor first-time initialization scene.  It positions the
 * selected actor from an optional reference actor, installs the progress task,
 * advances the shared selector/state, and records flag 0x2f0.
 */
union Coord_020036dc {
    s32 fixed;
    struct {
        u16 fraction;
        s16 integer;
    } parts;
};

struct Actor_020036dc {
    u8 reserved00[8];
    union Coord_020036dc x;
    s32 auxiliary;
    union Coord_020036dc y;
    u8 reserved14[20];
    s32 motion_state;
    u8 reserved2c[41];
    u8 variant;
};

struct Selection_020036dc {
    u8 reserved000[498];
    u8 scene_mode;
    u8 reserved1f3;
    s32 actor_id;
};

struct SceneWork_020036dc {
    u8 reserved000[380];
    u16 scene_marker;
};

extern struct Selection_020036dc Data_02000240;
extern struct SceneWork_020036dc *Data_03001ebc;
extern struct Actor_020036dc *Func_0808a080(s32 actor);
extern s32 Func_080770c0(s32 flag);
extern void Func_0808a018(void);
extern void Func_0808a1f0(s32 actor, s32 value);
extern void Func_0808a100(s32 actor, s32 mode);
extern void Func_0808a0b8(s32 actor, s32 x, s32 y);
extern void Func_0808a0e8(s32 actor);
extern void Func_080f9010(s32 value);
extern void Func_02003678(void);
extern void Func_080000d0(void (*task)(void), s32 priority);
extern void Func_08009150(struct Actor_020036dc *actor, s32 x, s32 auxiliary, s32 y);
extern void Func_080770c8(s32 flag);
extern void Func_080770e8(s32 state, s32 value);
extern void Func_0808a020(void);

void Func_020036dc(s32 reference_actor)
{
    struct Actor_020036dc *actor;
    struct Actor_020036dc *reference;
    s32 selected_actor;

    selected_actor = Data_02000240.actor_id;
    actor = Func_0808a080(selected_actor);
    if (Func_080770c0(0x2f0) != 0) {
        return;
    }

    Func_0808a018();
    Func_0808a1f0(selected_actor, 0x101);
    Func_0808a100(selected_actor, 9);
    reference = Func_0808a080(reference_actor);
    if (reference != 0) {
        Func_0808a0b8(selected_actor,
                     reference->x.parts.integer,
                     reference->y.parts.integer);
    }
    Func_0808a0e8(selected_actor);
    Func_080f9010(244);
    Func_080000d0(Func_02003678, 3200);

    Func_08009150(actor, actor->x.fixed,
                  actor->auxiliary + 0x200000, actor->y.fixed);
    actor->variant = 0;
    actor->motion_state = 0;
    actor->variant = 4;
    Func_0808a0e8(selected_actor);
    Data_02000240.scene_mode = 2;
    Func_080770c8(0x2f0);
    Func_080770e8(0x2f8, 180);
    Func_0808a020();
    Data_03001ebc->scene_marker = 0;
}

#include "types.h"

/*
 * Complete actor-16 first-time/repeat interaction owner.  The first visit
 * stages actor 16, advances the shared branch counter when the interaction
 * reports success, and records flag 0x308.  Later visits use a short dialogue
 * wrapper while temporarily accepting input for the actor.
 */
struct Actor_020008a8 {
    u8 reserved00[91];
    u8 accepted;
};

struct SceneWork_020008a8 {
    u8 reserved000[472];
    u16 branch_counter;
};

extern struct SceneWork_020008a8 *Data_03001ebc;
extern s32 Func_080770c0(s32 flag);
extern void Func_0808a018(void);
extern struct Actor_020008a8 *Func_0808a080(s32 actor);
extern void Func_0808a100(s32 actor, s32 mode);
extern void Func_0808a138(s32 actor, s32 mode);
extern void Func_0808a010(s32 frames);
extern void Func_0808a170(s32 dialogue);
extern void Func_0808a150(s32 actor, s32 mode, s32 value);
extern void Func_0808a178(s32 actor, s32 mode);
extern s32 Func_0808a070(s32 actor, s32 mode);
extern void Func_0808a180(s32 actor, s32 mode);
extern void Func_0808a098(s32 actor, s32 behavior);
extern void Func_0808a020(void);
extern void Func_080770c8(s32 flag);
extern void Func_020004b4(s32 actor);

void Func_020008a8(void)
{
    struct Actor_020008a8 *actor;

    if (Func_080770c0(0x308) == 0) {
        Func_0808a018();
        actor = Func_0808a080(16);
        actor->accepted = 1;
        Func_0808a100(16, 1);
        Func_0808a138(16, 1);
        Func_0808a010(20);
        Func_0808a170(0x1cb5);
        Func_0808a150(16, 0, 2);
        Func_0808a178(16, 0);
        if (Func_0808a070(0, 0) != 0) {
            Data_03001ebc->branch_counter += 1;
        }
        Func_0808a180(16, 0);
        Func_0808a080(16)->accepted = 0;
        Func_0808a098(16, 2);
        Func_0808a020();
        Func_080770c8(0x308);
    } else {
        Func_0808a170(0x1cc2);
        actor = Func_0808a080(16);
        Func_020004b4(16);
        Func_0808a080(16)->accepted = 0;
        actor->accepted = 1;
    }
}

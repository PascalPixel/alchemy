#include "TYPES.H"

/*
 * Makyuri aerie: initialize the actor's motion. It computes the actor's
 * tile-aligned position from its angle, and if the motion check passes it
 * arms the motion fields, clears the active flag, and queues the start-up
 * cues before restoring the saved flags.
 */

#define SceneActor_InitializeMotion Func_02000928
struct SceneActor {
    u8 unk_00[6]; u16 angle; s32 x,y,z; u8 unk_14[20];
    s32 motion28; u8 unk_2c[4]; s32 motion30,motion34;
    u8 unk_38[29]; u8 flags55;
};
extern struct SceneActor *Func_02004024(s32);
extern void Func_02003fca(s32,s32,s32 *);
extern s32 Func_02004002(struct SceneActor *,s32 *);
extern void Func_02004046(s32);
extern void Func_0200145a(void);
extern void Func_02003ff2(struct SceneActor *,s32);
extern void Func_02003fc8(s32);
extern void Func_020041de(s32);
extern void Func_02004006(struct SceneActor *,s32);
extern void Func_02004058(struct SceneActor *,s32);
extern void Func_020040de(s32,s32,s32);
extern void Func_0200403e(struct SceneActor *,s32);
extern void Func_02004076(struct SceneActor *,s32);
void SceneActor_InitializeMotion(void)
{
    struct SceneActor *actor;
    s32 position[3];
    s32 angle;
    u8 flags;
    actor = Func_02004024(0);
    angle = (actor->angle + 0x1000) & 0xe000;
    flags = actor->flags55;
    position[0] = (actor->x & 0xfff00000) + 0x80000;
    position[1] = actor->y;
    position[2] = (actor->z & 0xfff00000) + 0x80000;
    Func_02003fca(0x200000,angle,position);
    if (Func_02004002(actor,position) == 0) {
        Func_02004046(592);
        Func_0200145a();
        Func_02003ff2(actor,6);
        Func_02003fc8(6);
        Func_020041de(152);
        Func_02004006(actor,7);
        actor->motion30 = 0x30000;
        actor->motion34 = 0x20000;
        actor->motion28 = 0x40000;
        actor->flags55 &= 0x7e;
        Func_02004058(actor,0);
        Func_020040de(0,(s16)(position[0] >> 16),(s16)(position[2] >> 16));
        Func_0200403e(actor,6);
        Func_02004076(actor,1);
        actor->flags55 = flags;
    }
}

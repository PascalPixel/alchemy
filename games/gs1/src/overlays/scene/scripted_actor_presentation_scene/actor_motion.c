#include "types.h"

#define InitializeActorZeroMotion Func_02000518
#define SceneActor_ParkRecord Func_02005688
struct SceneActor {
    u8 pad00[6];
    u16 angle;
    s32 x;
    s32 y;
    s32 z;
    u8 pad14[20];
    s32 motion28;
    u8 pad2c[4];
    s32 motion30;
    s32 motion34;
    u8 pad38[29];
    u8 flags55;
};

void Func_02000c3a();
void Func_02006184();
void Func_0200618e();
void Func_020061de();
void Func_020061ec();
s32 Func_0200620e();
void Func_0200622a();
void Func_02006264();
void *Func_02006270();
void Func_02006282();
void Func_02006292();
void Func_02006352();
void Func_0200646e();

void InitializeActorZeroMotion(void)
{
    struct SceneActor *actor;
    s32 position[3];
    s32 angle;
    u8 flags;

    actor = Func_02006270(0);
    angle = (actor->angle + 0x1000) & 0xe000;
    flags = actor->flags55;
    position[0] = (actor->x & 0xfff00000) + 0x80000;
    position[1] = actor->y;
    position[2] = (actor->z & 0xfff00000) + 0x80000;
    Func_0200618e(0x200000, angle, position);
    if (Func_0200620e(actor, position) == 0) {
        Func_02006292(592);
        Func_02000c3a();
        Func_020061de(actor, 6);
        Func_02006184(6);
        Func_020061ec(actor, 7);
        actor->motion30 = 0x30000;
        actor->motion34 = 0x20000;
        Func_0200646e(152);
        actor->motion28 = 0x40000;
        actor->flags55 &= 0x7e;
        Func_02006264(actor, 0);
        Func_02006352(0, (s16)(position[0] >> 16),
                         (s16)(position[2] >> 16));
        Func_0200622a(actor, 6);
        Func_02006282(actor, 1);
        actor->flags55 = flags;
    }
}

/*
 * Park a record: stamp the sentinel 0x80000000 into the three mirror fields at
 * +56, +60 and +64, zero the three at +36, +40 and +44, and clear the u16
 * angle at +100.  The 24-byte owner at 0x02005688 has no prologue, no stack
 * use and no literal pool.  0x80000000 is read as a sentinel because it is
 * stamped into three position-family fields at once and then tested for
 * equality; the roles of +36, +40 and +44 are not established.
 */
void SceneActor_ParkRecord(u8 *record)
{
    /* The sentinel is built as 128 shifted left by 24, not pooled. */
    *(s32 *)(record + 56) = (s32)0x80000000;
    *(s32 *)(record + 60) = (s32)0x80000000;
    *(s32 *)(record + 64) = (s32)0x80000000;

    *(s32 *)(record + 36) = 0;
    *(s32 *)(record + 40) = 0;
    *(s32 *)(record + 44) = 0;

    *(u16 *)(record + 100) = 0;
}

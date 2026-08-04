#include "types.h"

extern void Func_0808a010(s32 frames);
extern void Func_0808a018(void);
extern void Func_0808a020(void);
extern u8 *Func_0808a080(s32 actorId);
extern void Func_0808a090(s32 actorId, s32 width, s32 height);
extern void Func_0808a0b8(s32 actorId, s32 x, s32 y);
extern void Func_0808a0d0(s32 actorId, s32 x, s32 y);
extern void Func_0808a0f0(s32 actorId, s32 x, s32 y);
extern void Func_0808a100(s32 actorId, s32 direction);
extern void Func_0808a110(s32 actorId, s32 direction);
extern void Func_0808a130(s32 actorId, s32 mode);
extern void Func_0808a138(s32 actorId, s32 mode);
extern void Func_0808a148(s32 actorId, s32 distance, s32 frames);
extern void Func_0808a170(s32 messageId);
extern void Func_0808a180(s32 actorId, s32 mode);
extern s32 Func_0808a190(s32 actorId, s32 mode);
extern void Func_0808a1b8(s32 actorId, s32 distance, s32 frames);
extern void Func_0808a1e8(s32 actorId, s32 animationId, s32 frames);
extern void Func_0808a200(s32 mode, s32 value);
extern void Func_0808a208(s32 left, s32 right);
extern void Func_0808a210(s32 value, s32 direction, s32 distance, s32 enabled);
extern void Func_0808a218(void);
extern void Func_0808a360(void);
extern void Func_0808a370(void);

/* Stage the five actors around the leader, play the approach until its state
 * machine settles, then restore their ordinary positions and presentation. */
void Func_02000db8(s32 leaderId)
{
    u8 *leader = Func_0808a080(leaderId);
    s32 leaderX = *(s16 *)(leader + 10);
    s32 leaderY = *(s16 *)(leader + 18);
    u8 *actor;

    Func_0808a018();
    Func_0808a090(leaderId, 0x10000, 0x8000);
    Func_0808a090(0, 0x10000, 0x8000);
    Func_0808a090(1, 0x10000, 0x8000);
    Func_0808a090(2, 0x10000, 0x8000);
    Func_0808a090(3, 0x10000, 0x8000);

    Func_0808a0f0(0, leaderX << 16, (leaderY << 16) - 0x300000);
    Func_0808a0f0(1, (leaderX << 16) - 0x100000, (leaderY << 16) - 0x280000);
    Func_0808a0f0(2, (leaderX << 16) + 0x100000, (leaderY << 16) - 0x280000);
    Func_0808a0f0(3, leaderX << 16, (leaderY << 16) - 0x200000);
    Func_0808a0f0(leaderId, leaderX << 16, (leaderY << 16) - 0x500000);

    actor = Func_0808a080(0);
    *(u16 *)(actor + 6) = 0xc000;
    Func_0808a200(0, 0);
    Func_0808a360();
    Func_0808a370();

    Func_0808a170(0x20cb);
    Func_0808a180(leaderId, 0);
    Func_0808a1e8(3, 0x101, 60);
    Func_0808a180(3, 0);
    Func_0808a130(leaderId, 3);
    Func_0808a180(leaderId, 0);
    Func_0808a1e8(2, 0x101, 60);
    Func_0808a180(2, 0);
    Func_0808a148(leaderId, 2, 0);
    Func_0808a010(20);
    Func_0808a110(leaderId, 3);
    Func_0808a180(leaderId, 0);
    Func_0808a1e8(1, 0x101, 60);
    Func_0808a180(1, 0);
    Func_0808a1e8(3, 0x101, 60);
    Func_0808a180(3, 0);
    Func_0808a1e8(leaderId, 0x102, 60);

    if (Func_0808a190(leaderId, 0) == 0) {
        do {
            Func_0808a170(0x20d5);
            Func_0808a100(2, 3);
            Func_0808a010(2);
            Func_0808a100(1, 3);
            Func_0808a010(2);
            Func_0808a100(3, 3);
            Func_0808a010(1);
            Func_0808a110(0, 3);
            Func_0808a110(leaderId, 3);
            Func_0808a180(leaderId, 0);
            Func_0808a1b8(leaderId, 0xa000, 0);
            Func_0808a010(20);
            Func_0808a180(leaderId, 0);

            Func_0808a208(0x30000, 0x6000);
            Func_0808a210(0x1380000, -1, 0x680000, 1);
            Func_0808a218();
            Func_0808a180(leaderId, 0);
            Func_0808a208(0x18000, 0x3000);
            Func_0808a210(0x3080000, -1, 0x680000, 1);
            Func_0808a180(leaderId, 0);
            Func_0808a218();
            Func_0808a180(leaderId, 0);
            Func_0808a208(0x30000, 0x6000);
            Func_0808a210(0x4d80000, -1, 0xa80000, 1);
            Func_0808a218();
            Func_0808a148(leaderId, 0x6000, 0);
            Func_0808a180(leaderId, 0);
            Func_0808a210(0x5180000, -1, 0xa80000, 1);
            Func_0808a218();
            Func_0808a148(leaderId, 0, 0);
            Func_0808a180(leaderId, 0);
            Func_0808a180(leaderId, 0);
            Func_0808a180(leaderId, 0);
            Func_0808a200(0, 0);
            Func_0808a138(leaderId, 2);
        } while (Func_0808a190(leaderId, 0) != 0);

        Func_0808a138(leaderId, 2);
        Func_0808a170(0x20d4);
        Func_0808a180(leaderId, 0);
    }

    Func_0808a170(0x20e1);
    Func_0808a138(leaderId, 2);
    Func_0808a180(leaderId, 0);
    Func_0808a1b8(0, 0x4000, 0);
    Func_0808a1b8(1, 0, 0);
    Func_0808a1b8(2, 0x8000, 0);
    Func_0808a138(1, 2);
    Func_0808a180(1, 0);
    Func_0808a138(2, 2);
    Func_0808a180(2, 0);
    Func_0808a110(3, 3);
    Func_0808a180(3, 0);
    Func_0808a100(3, 3);
    Func_0808a010(1);
    Func_0808a100(1, 3);
    Func_0808a010(2);
    Func_0808a100(2, 3);
    Func_0808a010(1);
    Func_0808a110(0, 3);
    Func_0808a010(6);
    Func_0808a100(1, 2);

    actor = Func_0808a080(0);
    if (actor != 0)
        Func_0808a0b8(1, *(s16 *)(actor + 10), *(s16 *)(actor + 18));
    Func_0808a100(2, 2);
    actor = Func_0808a080(0);
    if (actor != 0)
        Func_0808a0b8(2, *(s16 *)(actor + 10), *(s16 *)(actor + 18));
    Func_0808a100(3, 2);
    actor = Func_0808a080(0);
    if (actor != 0)
        Func_0808a0b8(3, *(s16 *)(actor + 10), *(s16 *)(actor + 18));

    Func_0808a0d0(leaderId, leaderX - 16, leaderY - 64);
    Func_0808a0f0(1, 0, 0);
    Func_0808a0f0(2, 0, 0);
    Func_0808a0f0(3, 0, 0);
    Func_0808a0d0(leaderId, leaderX - 16, leaderY - 16);
    Func_0808a0d0(leaderId, leaderX, leaderY);
    Func_0808a1b8(leaderId, 0xc000, 10);
    Func_0808a020();
}

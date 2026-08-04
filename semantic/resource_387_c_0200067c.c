typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;

/*
 * resource_387 owner at 0x0200067c, 1672 bytes: the overlay's long two-path
 * presentation script.  The short path restores actor 1 and returns when
 * story flag 0x855 is clear.  The long path stages actors 0..2 through a
 * deliberately ordered sequence of positions, motions, waits, poses, and
 * paired transitions, then publishes panel 74 and story flag 0x865.
 *
 * This is one machine owner despite the literal islands at 0x0200074c,
 * 0x02000b68, and 0x02000cf8: branches cross the first two, and the sole
 * interworking return is at 0x02000cf0-0x02000cf4.  Its final three-word pool
 * extends through 0x02000d03; the next independent prologue is 0x02000d04.
 * Every static call remains explicit so the choreography is readable and its
 * machine order can be audited without an assembly embedding.
 */

struct SceneActor_0200067c {
    u8 reserved00[8];
    s32 x;
    u8 reserved0c[4];
    s32 y;
};

extern void Func_0808a5e8(void);
extern void Func_0808a018(void);
extern void Func_0808a010(s32 frames);
extern void Func_0808a020(void);
extern void Func_0808a048(s32 actor, s32 value);
extern struct SceneActor_0200067c *Func_0808a080(s32 actor);
extern void Func_0808a090(s32 actor, s32 x, s32 y);
extern void Func_0808a0b8(s32 actor, s32 x, s32 y);
extern void Func_0808a0c8(s32 actor, s32 x, s32 y);
extern void Func_0808a0d0(s32 actor, s32 x, s32 y);
extern void Func_0808a0e8(s32 actor);
extern void Func_0808a0f0(s32 actor, s32 x, s32 y);
extern void Func_0808a100(s32 actor, s32 mode);
extern void Func_0808a110(s32 actor, s32 pose);
extern void Func_0808a128(s32 actor, s32 pose, s32 mode);
extern void Func_0808a130(s32 actor, s32 mode);
extern void Func_0808a138(s32 actor, s32 mode);
extern void Func_0808a148(s32 actor, s32 target, s32 frames);
extern void Func_0808a150(s32 actor, s32 target, s32 frames);
extern void Func_0808a170(s32 dialogue);
extern void Func_0808a188(s32 actor, s32 value, s32 frames);
extern void Func_0808a1b8(s32 actor, s32 value, s32 frames);
extern void Func_0808a1e8(s32 actor, s32 value, s32 frames);
extern void Func_0808a1f0(s32 actor, s32 pose);
extern s32 Func_080770c0(s32 flag);
extern void Func_080770c8(s32 flag);
extern void Func_080091c0(s32, s32, s32, s32, s32, s32);
extern void Func_02000314(s32 actor, s32 frames);
extern void Func_02000d68(void);
extern void Func_02000fc8(void);

void Func_0200067c(void)
{
    struct SceneActor_0200067c *actor;

    Func_0808a5e8();
    Func_0808a018();
    Func_0808a010(30);
    Func_0808a170(0x138f);
    Func_0808a090(0, 0xcccc, 0x6666);
    Func_0808a090(1, 0xcccc, 0x6666);
    Func_0808a1b8(0, 0xe000, 20);
    actor = Func_0808a080(0);
    if (actor != 0) {
        Func_0808a0f0(1, actor->x, actor->y);
    }
    Func_0808a0d0(1, 264, 168);
    Func_0808a1b8(1, 0x6000, 20);
    Func_0808a010(20);
    Func_0808a110(1, 4);
    Func_0808a010(20);
    Func_0808a188(1, 0, 20);
    Func_0808a110(0, 3);
    Func_0808a010(20);

    if (Func_080770c0(0x855) == 0) {
        Func_0808a100(1, 2);
        actor = Func_0808a080(0);
        if (actor != 0) {
            Func_0808a0b8(1, *(s16 *)((u8 *)actor + 10),
                              *(s16 *)((u8 *)actor + 18));
        }
        Func_0808a0e8(1);
        Func_0808a0f0(1, 0, 0);
        Func_0808a020();
        return;
    }

    /* Bring actor 2 on stage and establish the paired actors. */
    Func_0808a0f0(2, 0x1680000, 0xf80000);
    Func_0808a090(2, 0xcccc, 0x6666);
    Func_0808a0d0(2, 272, 248);
    Func_0808a0d0(2, 272, 208);
    Func_0808a1b8(2, 0xa000, 20);
    Func_0808a138(2, 2);
    Func_0808a010(20);
    Func_0808a188(2, 0, 20);
    Func_0808a1b8(0, 0x2000, 0);
    Func_0808a1b8(1, 0x4000, 0);
    Func_0808a010(20);
    Func_0808a1e8(0, 256, 0);
    Func_0808a1e8(1, 256, 60);
    Func_0808a110(2, 3);
    Func_0808a0d0(2, 264, 200);
    Func_0808a0c8(0, 248, 168);
    Func_0808a0d0(2, 248, 184);
    Func_0808a0e8(0);
    Func_0808a1b8(0, 0x6000, 0);
    Func_0808a1b8(1, 0x6000, 0);
    Func_0808a0d0(2, 232, 184);
    Func_0808a010(20);
    Func_0808a1e8(2, 0x105, 60);
    Func_0808a1b8(2, 0xe000, 20);
    Func_0808a110(2, 4);
    Func_0808a010(20);
    Func_0808a188(2, 0, 20);
    Func_0808a100(0, 3);
    Func_0808a110(1, 3);
    Func_0808a010(20);

    /* First exchange: approach, recoil, and mirror the pair. */
    Func_0808a1b8(2, 0x8000, 20);
    Func_0808a188(2, 0, 120);
    Func_0808a1e8(0, 0x105, 0);
    Func_0808a1e8(1, 0x105, 60);
    Func_0808a150(0, 1, 0);
    Func_0808a010(60);
    Func_0808a1b8(0, 0x6000, 0);
    Func_0808a1b8(1, 0x6000, 0);
    Func_0808a010(60);
    Func_0808a1e8(2, 262, 0);
    Func_0808a130(2, 1);
    Func_0808a010(30);
    Func_0808a188(2, 0, 30);
    Func_0808a1b8(2, 0xe000, 20);
    Func_0808a130(0, 2);
    Func_0808a138(1, 2);
    Func_0808a010(20);
    Func_0808a188(2, 0, 20);
    Func_0808a100(0, 3);
    Func_0808a110(1, 3);
    Func_0808a010(20);
    Func_0808a110(2, 3);
    Func_0808a010(20);
    Func_0808a1b8(2, 0x8000, 20);

    Func_02000d68();
    Func_0808a100(2, 1);
    Func_0808a010(20);
    Func_0808a5e8();
    Func_0808a1e8(0, 256, 0);
    Func_0808a1e8(1, 256, 60);
    Func_0808a128(1, 2, 0);
    Func_0808a010(20);
    Func_02000314(1, 20);
    Func_0808a0d0(2, 264, 184);
    Func_0808a010(10);
    Func_0808a148(2, 1, 0);
    Func_0808a148(1, 2, 0);
    Func_0808a148(0, 2, 0);
    Func_0808a010(20);
    Func_0808a110(2, 3);
    Func_0808a010(20);
    Func_02000314(2, 60);

    /* Repeat the exchange with the 0x105 and 0x101 cue pairs. */
    Func_0808a1e8(0, 0x105, 0);
    Func_0808a1e8(1, 0x105, 60);
    Func_0808a1e8(0, 0x101, 0);
    Func_0808a1e8(1, 0x101, 0);
    Func_0808a010(60);
    Func_0808a1b8(1, 0x8000, 0);
    Func_0808a1b8(0, 0, 0);
    Func_0808a010(60);
    Func_0808a1b8(1, 0x4000, 0);
    Func_0808a1b8(0, 0x2000, 0);
    Func_0808a010(10);
    Func_02000314(1, 20);
    Func_0808a1e8(2, 0x105, 0);
    Func_0808a010(60);
    Func_0808a110(2, 4);
    Func_0808a010(20);
    Func_02000314(2, 20);
    Func_0808a148(0, 2, 0);
    Func_0808a148(1, 2, 0);
    Func_0808a1e8(0, 258, 0);
    Func_0808a1e8(1, 258, 0);
    Func_0808a010(60);
    Func_0808a138(2, 2);
    Func_0808a010(20);
    Func_02000314(2, 30);
    Func_0808a1e8(0, 0x101, 0);
    Func_0808a1e8(1, 0x101, 0);
    Func_0808a010(80);
    Func_0808a110(2, 3);
    Func_0808a010(20);
    Func_02000314(2, 20);
    Func_0808a130(0, 1);
    Func_0808a130(1, 1);
    Func_0808a1f0(0, 258);
    Func_0808a1f0(1, 258);
    Func_0808a010(60);
    Func_0808a110(2, 4);
    Func_0808a010(20);
    Func_02000314(2, 20);

    /* Final movement phrases, followed by a complete stage reset. */
    Func_0808a1b8(0, 0, 0);
    Func_0808a1b8(1, 0x8000, 0);
    Func_0808a010(80);
    Func_0808a148(0, 2, 0);
    Func_0808a148(1, 2, 0);
    Func_0808a010(30);
    Func_0808a110(2, 3);
    Func_0808a010(20);
    Func_02000314(2, 30);
    Func_0808a110(2, 4);
    Func_0808a010(20);
    Func_02000314(2, 20);
    Func_0808a130(0, 2);
    Func_0808a138(1, 2);
    Func_0808a010(20);
    Func_0808a110(2, 3);
    Func_0808a010(20);
    Func_02000314(2, 40);
    Func_0808a100(0, 3);
    Func_0808a110(1, 3);
    Func_0808a010(20);
    Func_0808a048(2, 1);
    Func_0808a010(60);
    Func_02000fc8();
    Func_0808a138(2, 1);
    Func_0808a010(20);
    Func_0808a0d0(2, 248, 184);
    Func_0808a010(20);
    Func_02000314(2, 20);
    Func_0808a1b8(0, 0x8000, 0);
    Func_0808a1b8(1, 0x8000, 0);
    Func_0808a010(120);
    Func_02000314(2, 30);
    Func_0808a148(0, 2, 0);
    Func_0808a148(1, 2, 0);
    Func_0808a148(2, 0, 0);
    Func_0808a010(20);
    Func_0808a100(0, 3);
    Func_0808a100(1, 3);
    Func_0808a110(2, 3);
    Func_0808a010(50);

    Func_0808a090(1, 0xcccc, 0x6666);
    Func_0808a090(2, 0xcccc, 0x6666);
    Func_0808a0c8(1, 248, 168);
    Func_0808a0d0(2, 248, 168);
    Func_0808a0f0(2, 0, 0);
    Func_0808a0e8(1);
    Func_0808a0f0(1, 0, 0);
    Func_080091c0(74, 11, 1, 1, 73, 11);
    Func_080770c8(0x865);
    Func_0808a020();
}

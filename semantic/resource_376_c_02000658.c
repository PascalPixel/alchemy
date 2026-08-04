typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;

/*
 * resource_376 owner at 0x02000658, 2664 bytes: the overlay's long, linear
 * scripted set piece.
 *
 * Complete owner: the five-register/28-byte-frame prologue at 0x02000658,
 * all three code islands separated by inline literal pools, the sole
 * conditional counter increment at 0x02000c50, and the interworking return at
 * 0x02001090-0x0200109b.  The final nine-word pool runs through 0x020010bf;
 * the next independent prologue is 0x020010c0.
 *
 * The scene opens, configures actors 0..3 and 8, seats the off-screen actors
 * 23..25, then performs a long ordered choreography using the engine's move,
 * turn, wait, dialogue, and animation primitives.  Inline pools split the
 * machine body at 0x02000a74 and 0x02000ed4; control branches over both, so
 * neither pool is a function boundary.  The only data-dependent branch calls
 * Func_0808a070(0, 0) and increments the scene-work u16 at +472 exactly when
 * that call returns one.
 *
 * All 259 static call sites resolve to 38 veneer imports under the overlay +2
 * branch rule.  Calls remain explicit and ordered below so this reconstruction
 * preserves the script as readable C rather than hiding it in a table or an
 * assembly embedding.
 */

struct Actor_02000658 {
    u8 reserved00[8];
    s32 x;
    u8 reserved0c[4];
    s32 y;
    u8 reserved14[0x0f];
    u8 flags_23;
    u8 reserved24[0x31];
    u8 active_55;
};

struct SceneWork_02000658 {
    u8 reserved000[456];
    s32 scene_value;
    u8 reserved1cc[12];
    u16 branch_counter;
};

extern struct SceneWork_02000658 *Data_03001ebc;
extern const u8 Data_020092fc[];
extern const u8 Data_02009400[];
extern const u8 Data_02009310[];

extern void Func_0808a018(void);
extern void Func_0808a020(void);
extern void Func_0808a010(s32 frames);
extern s32 Func_0808a070(s32 actor, s32 mode);
extern struct Actor_02000658 *Func_0808a080(s32 actor);
extern void Func_0808a090(s32 actor, s32 x, s32 y);
extern void Func_0808a098(s32 actor, const void *descriptor);
extern void Func_0808a0a8(s32 actor);
extern void Func_0808a0b0(s32 actor, const void *descriptor);
extern void Func_0808a0c8(s32 actor, s32 x, s32 y);
extern void Func_0808a0d0(s32 actor, s32 x, s32 y);
extern void Func_0808a0e8(s32 actor);
extern void Func_0808a0f0(s32 actor, s32 x, s32 y);
extern void Func_0808a100(s32 actor, s32 mode);
extern void Func_0808a110(s32 actor, s32 animation);
extern void Func_0808a128(s32 actor, s32 animation, s32 mode);
extern void Func_0808a130(s32 actor, s32 mode);
extern void Func_0808a138(s32 actor, s32 mode);
extern void Func_0808a168(s32 actor, s32 value, const void *script);
extern void Func_0808a170(s32 dialogue);
extern void Func_0808a178(s32 actor, s32 mode);
extern void Func_0808a180(s32 actor, s32 mode);
extern void Func_0808a188(s32 actor, s32 mode, s32 frames);
extern void Func_0808a1b8(s32 actor, s32 value, s32 frames);
extern void Func_0808a1d0(s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32);
extern void Func_0808a1e0(s32 actor, s32 mode);
extern void Func_0808a1e8(s32 actor, s32 value, s32 frames);
extern void Func_0808a208(s32 x, s32 y);
extern void Func_0808a210(s32 x, s32 y, s32 z, s32 mode);
extern void Func_0808a218(void);
extern struct Actor_02000658 *Func_0808a228(void);
extern void Func_0808a360(void);
extern void Func_0808a370(void);
extern void Func_080000c0(s32 frames);
extern void Func_080000d0(const void *callback, s32 value);
extern void Func_080000d8(const void *callback);
extern void Func_080091e0(struct Actor_02000658 *actor, s32 mode);
extern void Func_080770c8(s32 flag);

void Func_02000658(void)
{
    struct Actor_02000658 *actor;

    Func_0808a018();
    Func_0808a210(-1, -1, -1, 0);
    Func_080000c0(1);
    Func_0808a1e0(3, 1);

    Func_0808a090(0, 0x6666, 0x3333);
    Func_0808a090(1, 0x6666, 0x3333);
    Func_0808a090(2, 0x6666, 0x3333);
    Func_0808a090(3, 0x6666, 0x3333);
    Func_0808a100(8, 5);
    Func_0808a0c8(0, 808, 508);

    Func_080091e0(Func_0808a080(23), 0);
    Func_080091e0(Func_0808a080(24), 0);
    Func_080091e0(Func_0808a080(25), 0);
    Func_0808a080(23)->active_55 = 0;
    Func_0808a080(24)->active_55 = 0;
    Func_0808a080(25)->active_55 = 0;

    Func_080000d0((const void *)0x020090c1, 3200);
    Func_080000c0(1);
    Data_03001ebc->scene_value = 32;
    Func_0808a360();
    Func_0808a370();
    Func_0808a0e8(0);
    Func_0808a100(0, 1);

    actor = Func_0808a080(0);
    if (actor != 0) {
        Func_0808a0f0(1, actor->x, actor->y);
    }
    actor = Func_0808a080(0);
    if (actor != 0) {
        Func_0808a0f0(2, actor->x, actor->y);
    }
    actor = Func_0808a080(0);
    if (actor != 0) {
        Func_0808a0f0(3, actor->x, actor->y);
    }

    Func_0808a0c8(1, 792, 512);
    Func_0808a0c8(2, 824, 504);
    Func_0808a0d0(3, 0x332, 524);
    Func_0808a100(1, 1);
    Func_0808a100(2, 1);
    Func_0808a010(10);

    Func_0808a168(0, 0x1000a, Data_020092fc);
    Func_0808a168(1, 0x1000a, Data_020092fc);
    Func_0808a168(2, 0x1000a, Data_020092fc);
    Func_0808a168(3, 0x1000a, Data_020092fc);
    Func_0808a010(300);
    Func_0808a228()->active_55 = 0;

    Func_0808a208(0x1999, 0x333);
    Func_0808a210(0x03120000, 0, 0x01ae0000, 1);
    Func_0808a010(240);
    Func_0808a0a8(10);
    Func_0808a1e8(10, 258, 80);
    Func_0808a0d0(10, 0x333, 0x195);
    Func_0808a010(40);
    Func_0808a110(10, 4);
    Func_0808a010(40);
    Func_0808a1b8(10, 0xd000, 20);
    Func_0808a170(0x1c1e);
    Func_0808a188(0x900a, 0, 20);

    Func_0808a0a8(0);
    Func_0808a0a8(1);
    Func_0808a0a8(2);
    Func_0808a0a8(3);
    Func_0808a1e8(11, 256, 40);
    Func_0808a188(0x200b, 0, 20);
    Func_0808a138(10, 2);
    Func_0808a010(40);
    Func_0808a188(0x900a, 0, 10);
    Func_0808a1b8(11, 0x5000, 10);
    Func_0808a188(0x200b, 0, 40);
    Func_0808a138(10, 2);
    Func_0808a010(20);
    Func_0808a188(0x900a, 0, 20);
    Func_0808a1e8(0, 258, 80);
    Func_0808a1e8(11, 262, 40);
    Func_0808a188(0x200b, 0, 40);
    Func_0808a130(10, 2);
    Func_0808a1e8(10, 258, 20);
    Func_0808a100(10, 4);
    Func_0808a188(0x900a, 0, 10);
    Func_0808a130(11, 1);
    Func_0808a110(11, 3);
    Func_0808a010(20);
    Func_0808a130(10, 1);
    Func_0808a110(10, 4);
    Func_0808a130(11, 1);
    Func_0808a110(11, 3);
    Func_0808a130(10, 1);
    Func_0808a110(10, 4);

    Func_0808a1e8(9, 0x105, 0);
    Func_0808a138(9, 1);
    Func_0808a010(20);
    Func_0808a1b8(9, 0x4000, 40);
    Func_0808a138(9, 2);
    Func_0808a010(60);
    Func_0808a138(9, 3);
    Func_0808a010(40);
    Func_0808a188(0x4009, 0, 40);
    Func_0808a100(11, 0);
    Func_0808a138(11, 2);
    Func_0808a188(0x200b, 0, 10);
    Func_0808a110(9, 4);
    Func_0808a138(9, 2);
    Func_0808a188(0x4009, 0, 10);
    Func_0808a1e8(10, 256, 20);
    Func_0808a1b8(10, 0x5000, 40);
    Func_0808a110(10, 3);
    Func_0808a188(0x400a, 0, 10);
    Func_0808a110(9, 4);
    Func_0808a1b8(9, 0xd000, 10);
    Func_0808a128(9, 2, 0);
    Func_0808a100(9, 4);
    Func_0808a188(0x4009, 0, 10);
    Func_0808a1e8(11, 0x101, 0);
    Func_0808a1e8(10, 0x101, 40);
    Func_0808a1b8(10, 0xd000, 80);
    Func_0808a1b8(10, 0x5000, 60);
    Func_0808a130(10, 2);
    Func_0808a130(11, 2);

    Func_0808a1d0(10, 11, 6, 6, 6, 11, 12, 1, 7, 1, 0);
    Func_0808a010(20);
    Func_0808a208(0x19999, 0x3333);
    Func_0808a210(0x02ee0000, 0, 0x01d40000, 1);
    Func_0808a218();
    Func_0808a010(40);
    Func_0808a110(1, 3);
    Func_0808a188(0x1001, 0, 20);
    Func_0808a138(8, 2);
    Func_080000d8((const void *)0x020090c1);
    Func_0808a010(40);
    Func_0808a110(8, 6);
    Func_0808a010(20);
    Func_0808a188(0x4008, 0, 20);
    Func_0808a210(0x02ee0000, 0, 0x01c30000, 1);
    Func_0808a010(20);

    Func_0808a1b8(11, 0x5000, 0);
    Func_0808a1b8(10, 0x5000, 10);
    Func_0808a1b8(8, 0x4000, 40);
    Func_0808a1e8(8, 256, 40);
    Func_0808a1b8(8, 0x3000, 20);
    Func_0808a1b8(8, 0x4000, 20);
    Func_0808a1b8(8, 0x3000, 40);
    Func_0808a110(8, 6);
    Func_0808a010(60);
    Func_0808a128(8, 6, 0);
    Func_0808a188(0x4008, 0, 20);

    Func_0808a090(1, 0x19999, 0xcccc);
    Func_0808a0d0(1, 0x315, 0x1d9);
    Func_0808a1b8(1, 0x7000, 20);
    Func_0808a110(1, 3);
    Func_0808a188(0x4001, 0, 10);
    Func_0808a1b8(8, 0x4000, 20);
    Func_0808a110(8, 3);
    Func_0808a178(0x4008, 0);
    Func_0808a1b8(10, 0x5000, 0);
    Func_0808a1b8(9, 0x4000, 0);
    Func_0808a1b8(1, 0x3000, 0);
    Func_0808a1b8(2, 0x7000, 0);
    Func_0808a1b8(3, 0xb000, 0);

    if (Func_0808a070(0, 0) == 1) {
        Data_03001ebc->branch_counter += 1;
    }
    Func_0808a210(0x03090000, 0, 0x01ac0000, 1);
    Func_0808a010(20);
    Func_0808a138(10, 2);
    Func_0808a180(10, 0);
    Func_0808a110(11, 4);
    Func_0808a010(20);
    Func_0808a170(0x1c33);
    Func_0808a180(0x200b, 0);
    Func_0808a210(0x03090000, 0, 0x01d40000, 1);
    Func_0808a010(20);

    Func_0808a1b8(1, 0xd000, 20);
    Func_0808a110(1, 3);
    Func_0808a010(20);
    Func_0808a110(9, 4);
    Func_0808a1b8(9, 0xd000, 10);
    Func_0808a180(0x4009, 0);
    Func_0808a110(8, 3);
    Func_0808a180(0x4008, 0);
    Func_0808a1b8(1, 0x7000, 10);
    Func_0808a110(1, 3);
    Func_0808a1b8(9, 0x4000, 10);
    Func_0808a100(11, 3);
    Func_0808a100(10, 3);
    Func_0808a100(9, 3);
    Func_0808a110(8, 3);
    Func_0808a010(20);

    Func_0808a1b8(1, 0x3000, 20);
    Func_0808a1e8(1, 258, 80);
    Func_0808a1b8(1, 0x7000, 20);
    Func_0808a188(0x4001, 0, 20);
    Func_0808a1b8(1, 0x3000, 10);
    Func_0808a1b8(0, 0, 40);
    Func_0808a100(0, 3);
    Func_0808a110(1, 3);
    Func_0808a010(20);
    Func_0808a1b8(0, 0x4000, 20);
    Func_0808a110(3, 3);
    Func_0808a010(20);
    Func_0808a1b8(1, 0x4000, 0);
    Func_0808a1b8(0, 0xe000, 0);

    Func_0808a090(2, 0x10000, 0x8000);
    Func_0808a0d0(2, 0x333, 0x1e9);
    Func_0808a1b8(2, 0xb000, 40);
    Func_0808a138(2, 2);
    Func_0808a188(2, 0, 20);
    Func_0808a110(2, 3);
    Func_0808a100(8, 3);
    Func_0808a100(9, 3);
    Func_0808a100(10, 3);
    Func_0808a110(9, 3);

    actor = Func_0808a080(3);
    actor->flags_23 &= 0xfe;
    Func_0808a1e0(3, 1);
    Func_0808a090(3, 0x10000, 0x8000);
    Func_0808a0d0(3, 0x31a, 0x208);
    Func_0808a1b8(1, 0x5000, 0);
    Func_0808a1b8(0, 0xa000, 0);
    Func_0808a0d0(3, 0x310, 0x1f0);
    Func_0808a1b8(3, 0x9000, 10);
    actor = Func_0808a080(3);
    actor->flags_23 |= 1;
    Func_0808a188(3, 0, 20);
    Func_0808a100(8, 3);
    Func_0808a100(9, 3);
    Func_0808a100(10, 3);
    Func_0808a110(9, 3);
    Func_0808a010(20);

    Func_0808a210(0x03090000, 0, 0x01ac0000, 1);
    Func_0808a010(20);
    Func_0808a090(11, 0x6666, 0x3333);
    Func_0808a0d0(11, 0x343, 0x184);
    Func_0808a1b8(11, 0x5000, 0);
    Func_0808a1e8(11, 264, 40);
    Func_0808a188(0x200b, 0, 20);
    Func_0808a210(0x03090000, 0, 0x01d40000, 1);
    Func_0808a010(40);

    Func_0808a1b8(2, 0x7000, 0);
    Func_0808a1b8(3, 0xf000, 40);
    Func_0808a1b8(2, 0x9000, 0);
    Func_0808a1b8(3, 0xd000, 20);
    Func_0808a100(2, 3);
    Func_0808a110(3, 3);
    Func_0808a010(20);
    Func_0808a138(10, 1);
    Func_0808a010(20);
    Func_0808a100(10, 3);
    Func_0808a188(10, 0, 20);
    Func_0808a1b8(0, 0xc000, 0);
    Func_0808a1b8(1, 0xd000, 0);
    Func_0808a1b8(2, 0xb000, 0);
    Func_0808a1b8(3, 0xd000, 40);
    Func_0808a100(0, 3);
    Func_0808a100(1, 3);
    Func_0808a100(2, 3);
    Func_0808a110(3, 3);
    Func_0808a010(20);

    Func_0808a090(2, 0x10000, 0x8000);
    Func_0808a098(1, Data_02009400);
    Func_0808a098(2, Data_02009400);
    Func_0808a0b0(3, Data_02009400);
    Func_0808a098(10, Data_02009310);
    Func_0808a0d0(11, 0x345, 376);
    Func_0808a1b8(11, 0xd000, 20);
    Func_080770c8(0x81d);
    Func_0808a020();
}

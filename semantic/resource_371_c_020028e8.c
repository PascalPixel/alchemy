typedef unsigned char u8;
typedef unsigned short u16;
typedef signed short s16;
typedef signed int s32;

/*
 * resource_371 owner at 0x020028e8, 972 bytes: the full dialogue-0x2644
 * presentation scene.  It repairs the lead actor's presentation bounds,
 * spawns and dresses a transient effect, runs the selected actor and actor 55
 * through the ordered exchange, restores both actors, publishes the story
 * flag, and closes the scene.  The single high-register return is followed by
 * an eighteen-word literal pool through 0x02002cb3.
 */

struct SceneActor_020028e8 {
    u8 reserved00[6];
    u16 presentation;
    s32 x;
    s32 auxiliary;
    s32 y;
    u8 reserved14[72];
};

struct SceneWork_020028e8 {
    u8 reserved000[472];
    u16 scene_counter;
};

extern s32 Data_0200e79c;
extern struct SceneWork_020028e8 *Data_03001ebc;
extern void Func_0808a018(void);
extern void Func_0808a020(void);
extern void Func_0808a010(s32 frames);
extern void Func_0808a080_dummy(void);
extern struct SceneActor_020028e8 *Func_0808a080(s32 actor);
extern void Func_0808a088(s32 actor);
extern void Func_0808a090(s32 actor, s32 x, s32 y);
extern void Func_0808a0b8(s32 actor, s32 x, s32 y);
extern void Func_0808a0e8(s32 actor);
extern void Func_0808a0f0(s32 actor, s32 x, s32 y);
extern void Func_0808a100(s32 actor, s32 mode);
extern void Func_0808a110(s32 actor, s32 pose);
extern void Func_0808a128(s32 actor, s32 pose, s32 frames);
extern void Func_0808a138(s32 actor, s32 mode);
extern void Func_0808a170(s32 dialogue);
extern void Func_0808a188(s32 actor, s32 value, s32 frames);
extern void Func_0808a1b8(s32 actor, s32 value, s32 frames);
extern void Func_0808a1e8(s32 actor, s32 value, s32 frames);
extern void Func_0808a208(s32 left, s32 right);
extern void Func_0808a210(s32 x, s32 y, s32 z, s32 enabled);
extern void Func_0808a3d8(void);
extern void Func_0808a3e0(void);
extern void Func_0808a5c0(s32 value, s32 mode);
extern void Func_08009140(struct SceneActor_020028e8 *actor);
extern void Func_08009150(void *actor, s32 x, s32 auxiliary, s32 y);
extern void Func_08009158(void *actor);
extern u8 *Func_080090c8(s32 kind, s32 x, s32 auxiliary, s32 y);
extern void Func_080090d0(u8 *effect);
extern s32 Func_08000140(s32 bank, s32 item);
extern void Func_08000150(s32 bank);
extern void Func_080001c8(s32 frame, s32 size, s32 destination);
extern void Func_08015250(s32 value);
extern void Func_08015040(s32 dialogue, s32 mode);
extern void Func_080772e8(s32 cue);
extern void Func_080770c8(s32 flag);
extern void Func_080770d0(s32 flag);
extern void Func_020027dc(void);

void Func_020028e8(void)
{
    struct SceneActor_020028e8 *lead;
    struct SceneActor_020028e8 *selected;
    struct SceneActor_020028e8 *actor55;
    u8 *effect;
    u8 *effect_payload;
    s32 frame_data;

    lead = Func_0808a080(0);
    Func_0808a018();
    Func_0808a3e0();
    Func_0808a5c0(0x16666, 6);
    Func_0808a208(0x30000, 0x6000);
    Func_0808a210(0x17880000, -1, 0x0d680000, 1);
    Func_0808a090(0, 0xcccc, 0x6666);
    Func_0808a100(0, 2);
    ((u8 *)lead)[91] = 0;
    Func_08009140(lead);

    if (lead->y > 0x0d680000) {
        if (lead->x > 0x176e0000) {
            Func_08009150(lead, 0x176e0000, lead->auxiliary, 0x0d7d0000);
            Func_08009158(lead);
        }
    } else if (lead->x > 0x177a0000) {
        Func_08009150(lead, 0x177a0000, *(volatile s32 *)12, 0x0d480000);
        Func_08009158(lead);
    }
    Func_08009150(lead, 0x17690000, 0, 0x0d680000);
    Func_08009158(lead);

    Func_0808a100(0, 1);
    Func_0808a1b8(0, 0, 40);
    Func_0808a3d8();
    Func_0808a138(0, 2);
    Func_0808a010(20);
    Func_0808a100(0, 28);

    effect = Func_080090c8(22, lead->x + 0x20000, 0x260000, lead->y);
    if (effect != 0) {
        effect[85] = 0;
        effect_payload = *(u8 **)(effect + 80);
        effect_payload[38] = 0;
        effect_payload[39] = 0;
        effect_payload[5] &= (u8)~0x20;
        effect_payload[9] &= 0x0f;

        frame_data = Func_08000140(17, 0x608);
        Func_08015250(242);
        frame_data += 0x400;
        Func_080001c8(effect_payload[28], 128, frame_data);
        Func_08000150(17);
        Func_0808a010(20);
        *(s32 *)(effect + 108) = 0x0200813d;
        Func_0808a010(80);
    }

    selected = Func_0808a080(Data_0200e79c);
    selected->presentation = 0x3000;
    Func_0808a1e8(Data_0200e79c, 256, 0);
    Func_0808a138(Data_0200e79c, 2);
    Func_0808a170(0x2644);
    Func_0808a188(Data_0200e79c, 0, 80);
    if (effect != 0) {
        Func_080090d0(effect);
    }
    Func_0808a100(0, 1);
    Func_0808a010(40);
    Func_0808a128(Data_0200e79c, 6, 40);
    Func_0808a188(Data_0200e79c, 0, 20);
    Func_0808a1b8(0, 0xe000, 0);
    Func_0808a1b8(Data_0200e79c, 0xd000, 20);
    Func_0808a188(Data_0200e79c | 0x9000, 0, 40);
    Func_0808a110(Data_0200e79c, 4);
    Func_0808a188(Data_0200e79c | 0x9000, 0, 20);
    Func_0808a1b8(Data_0200e79c, 0x3000, 20);
    Func_0808a188(Data_0200e79c, 0, 10);
    Func_0808a090(Data_0200e79c, 0xcccc, 0x6666);
    Func_0808a100(Data_0200e79c, 2);

    actor55 = Func_0808a080(55);
    Func_08009150(actor55, 0x177a0000, actor55->auxiliary, 0x0d480000);
    Func_08009158(actor55);
    Func_08009150(actor55, 0x17710000, 0, 0x0d580000);
    Func_08009158(actor55);
    Func_0808a100(55, 1);
    Func_0808a1b8(Data_0200e79c, 0x5000, 10);
    Func_0808a138(Data_0200e79c, 1);
    Func_0808a188(Data_0200e79c | 0x1000, 0, 20);

    Func_0808a090(55, 0x10000, 0x8000);
    actor55 = Func_0808a080(55);
    actor55 = (struct SceneActor_020028e8 *)((u8 *)actor55);
    ((u8 *)actor55)[90] &= 0xfe;
    Func_0808a100(55, 2);
    Func_08009150(actor55, 0x176d0000, 0, 0x0d580000);
    Func_08009158(actor55);
    Func_0808a100(55, 1);
    Func_0808a010(10);
    Func_0808a100(55, 2);
    Func_08009150(actor55, 0x17710000, 0, 0x0d580000);
    Func_08009158(actor55);
    Func_0808a100(55, 1);
    Func_08015040(0x264a, 1);
    Data_03001ebc->scene_counter += 1;
    Func_080772e8(242);
    Func_0808a010(20);

    Func_0808a100(Data_0200e79c, 4);
    Func_0808a188(Data_0200e79c, 0, 10);
    Func_0808a110(0, 3);
    Func_0808a110(Data_0200e79c, 3);
    Func_0808a100(Data_0200e79c, 2);
    lead = Func_0808a080(0);
    if (lead != 0) {
        Func_0808a0b8(Data_0200e79c,
                      *(s16 *)((u8 *)lead + 10),
                      *(s16 *)((u8 *)lead + 18));
    }
    Func_0808a0e8(Data_0200e79c);
    Func_0808a0f0(Data_0200e79c, 0, 0);
    Func_0808a3e0();
    Func_0808a5c0(0x10000, 6);
    Func_0808a010(20);
    Func_020027dc();
    Func_0808a088(Data_0200e79c);
    Func_080770d0(0x234);
    Func_080770c8(0x85d);
    Func_0808a020();
}

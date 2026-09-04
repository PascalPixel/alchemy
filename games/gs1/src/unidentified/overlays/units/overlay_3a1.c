#include "types.h"

#define NULL ((void *)0)
#define CreateOverlayObject Func_020006de
#define SetOverlayObjectMode Func_02000710
#define SetOverlayObjectSlot Func_02000770
#define CreateConfiguredOverlayObject Func_020000a0

typedef struct {
    u8 pad[9];
    u8 lo:2;
    u8 field:2;
    u8 hi:4;
} Object_02000030;

union Slot {
    s32 w;
    u16 h[2];
    void *p;
};

struct Sprite {
    u8 pad00[9];
    u8 flags9;
    u8 pad0a[20];
    u16 angle;
    u8 pad20[6];
    u8 state26;
};

struct Effect {
    u8 pad00[24];
    s32 accum18;
    s32 accum1c;
    u8 pad20[3];
    u8 flags23;
    u8 pad24[12];
    s32 rate30;
    s32 rate34;
    u8 pad38[12];
    s32 velocity_x;
    s32 velocity_y;
    s32 velocity_z;
    struct Sprite *sprite;
    u8 pad54;
    u8 mode55;
    u8 pad56[14];
    u16 step64;
    u8 pad66[6];
    u32 callback;
};

struct Options {
    u8 mode_bits;
    u8 pad01[3];
    s32 mode;
    s32 accum18;
    s32 accum1c;
    s32 target30;
    s32 target34;
    s16 kind;
    u16 pad1a;
    s32 callback_arg;
    u16 angle;
    u16 step;
    u32 callback;
};

struct Descriptor {
    s32 pad00[3];
    s32 duration;
};

extern struct Descriptor *Data_0200876c[];
extern u8 Data_02008778[];
extern u8 Data_02008868[];
extern s16 Data_02000240[];
extern u8 Data_020089c8[];
extern u8 Data_02008890[];
extern u8 Value_00001a40;
extern u8 Data_02008d4c[];
extern u8 Data_02008a28[];
extern u8 *Data_03001ebc;

void Func_020006b4(void *, s32);
void Func_020006cc(void *, s32);
void Func_02000714(void *, s32);
void *Func_020006de(s32, s32, s32, s32);
void Func_02000710(void *, s32);
void Func_02000770(void *, s32);
struct Effect *Func_020007d2(s32 slot);
struct Effect *Func_020007b0(s32 kind, s32 x, s32 y, s32 z);
void Func_020007ba(struct Effect *effect, s32 mode);
void Func_020007d4(struct Effect *effect, struct Descriptor *descriptor);
void Func_020008a6(struct Effect *effect, s32 mode);
s32 Func_02000890(s32 delta, s32 duration);
s32 Func_020008a8(s32 delta, s32 duration);
s32 Func_020008b6(s32 delta, s32 duration);
void Func_020008d4(struct Effect *effect, s32 mode);
void Func_020008e4(struct Effect *effect, s32 callback_arg);
void Func_020009b0(void *);
u16 *Func_020009d6(s32);
void Func_020009c4(void);
void Func_02000a2e(s32, s32);
s32 Func_020009c6(s32);
void Func_02000a20(s32);
void Func_02000a28(s32);
void Func_02000a38(s32, s32);
void Func_02000a04(void);
u16 *Func_02000a36(s32);
void Func_02000a24(void);
void Func_02000a8e(s32, s32);
s32 Func_02000a26(s32);
void Func_02000a80(s32);
void Func_02000a88(s32);
void Func_02000a98(s32, s32);
void Func_02000a64(void);
u16 *Func_02000a96(s32);
void Func_02000a84(void);
void Func_02000aee(s32, s32);
s32 Func_02000a86(s32);
void Func_02000ae0(s32);
void Func_02000b10(s32, s32);
void Func_02000acc(void);
u16 *Func_02000afe(s32);
void Func_02000aec(void);
void Func_02000b60(s32, s32);
void Func_02000b4a(s32);
void Func_02000b52(s32);
void Func_02000b62(s32, s32);
void Func_02000b2e(void);
void Func_02000b44(void);
void Func_02000b82(s32);
void Func_02000b5e(void);
void Func_02000b64(void);
void Func_02000b7e(void);
void Func_02000b84(void);
void Func_02000bdc(s32, s32, s32);
s32 Func_02000bc6(s32, s32, s32);
void Func_02000b9c(s32);
s32 Func_02000bd6(s32, s32, s32);
void Func_02000bac(s32);
void Func_02000bfc(s32, s32);
void Func_02000bc8(void);
s32 Func_02000678(s32, s32, s32, s32);
void Func_02000c3e(s32);
void Func_02000c76(s32, s32);

/*
 * resource_3a1 configurable spawn/copy owner at 0x0200013c. The code ends
 * with the unwind at 0x02000306; its three-word pool occupies
 * 0x02000308-0x02000313. This is the same compiler-witnessed source family
 * as the exact resource_380/resource_39c/resource_3c9 owners, with this
 * overlay's own import veneers and descriptor table.
 */

void Func_02000030(void *arg0, s32 arg1)
{
    Object_02000030 *object = *(Object_02000030 **)((u8 *)arg0 + 0x50);

    object->field = arg1;
}

void *Func_02000048(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
    void *Func_02000686(s32, s32, s32, s32);

    u8 *result = Func_02000686(arg3, arg0, arg1, arg2);

    if (result != NULL) {
        u8 *object = *(u8 **)(result + 0x50);
        s32 flags;
        s32 mask = 13;

        flags = object[9];
        mask = -mask;
        mask &= flags;
        object[9] = mask;
        result[0x55] = 0;
        result[0x59] = 8;
        Func_020006b4(result, 0);
        Func_02000714(result, 14);
        Func_020006cc(result, 1);
        return result;
    }
    return NULL;
}

void *CreateConfiguredOverlayObject(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
    u8 *result = CreateOverlayObject(arg3, arg0, arg1, arg2);

    if (result != NULL) {
        u8 *object = *(u8 **)(result + 0x50);
        s32 flags;
        s32 mask = 13;

        flags = object[9];
        mask = -mask;
        mask &= flags;
        mask |= 4;
        object[9] = mask;
        result[0x55] = 0;
        result[0x59] = 8;
        SetOverlayObjectMode(result, 0);
        SetOverlayObjectSlot(result, 15);
        result[0x23] = (result[0x23] & 0xfe) | 2;
        return result;
    }
    return NULL;
}

void Func_02000104(void *arg0)
{
    union Slot *s = (union Slot *)arg0;
    union Slot *obj;

    s[2].w += s[17].w;
    s[3].w += s[18].w;
    s[4].w += s[19].w;
    s[6].w += s[12].w;
    s[7].w += s[13].w;
    obj = (union Slot *)s[20].p;
    obj[7].h[1] += s[25].h[0];
}

void Func_0200013c(s32 x, s32 y, s32 z, s32 vx, s32 vy, s32 vz,
                   u32 flags, const struct Options *options)
{
    u32 table_offset;
    struct Effect *party;
    u32 copied_bits;
    s32 flag_mask;
    u32 block_bits;
    struct Effect *effect;
    struct Sprite *block;
    struct Sprite *mode_block;
    u32 option_bits;
    u16 *tag;
    s32 duration;
    s32 first_delta;
    s32 accumulated;

    party = Func_020007d2(0);
    if ((flags & 0x100000) != 0 && options != 0) {
        effect = Func_020007b0(options->kind, x, y, z);
    } else {
        effect = Func_020007b0(222, x, y, z);
    }
    if (effect == 0)
        return;

    block = effect->sprite;
    mode_block = block;

    Func_020007ba(effect, (flags + 1) & 15);
    table_offset = (flags & 15) << 2;
    Func_020007d4(effect, Data_0200876c[table_offset >> 2]);

    effect->mode55 = 0;
    block->state26 = 0;
    effect->callback = 0x02008105;

    effect->velocity_x = vx;
    x = 3;
    effect->velocity_y = vy;
    effect->velocity_z = vz;

    copied_bits = party->sprite->flags9 & 12;
    block_bits = *(volatile u8 *)&block->flags9;
    flag_mask = ~12;
    block->flags9 = (u8)((block_bits & flag_mask) | copied_bits);

    effect->rate30 = 0;
    effect->rate34 = 0;
    effect->step64 = 0;
    tag = &effect->step64;

    if ((flags & 0xffff0000) == 0 || options == 0)
        return;

    if ((flags & 0x10000) != 0)
        Func_020008a6(effect, options->mode);

    if ((flags & 0x20000) != 0) {
        effect->flags23 &= 0xfe;
        option_bits = *(const u8 *)options & x;
        block->flags9 = (u8)((*((const u8 *)mode_block + 9) & flag_mask)
                             | (option_bits << 2));
    }

    if ((flags & 0x80000) != 0) {
        effect->accum18 = options->accum18;
        effect->accum1c = options->accum1c;
    }

    if ((flags & 0x40000) != 0) {
        const struct Descriptor *descriptor =
            Data_0200876c[table_offset >> 2];
        s32 delta;

        if ((flags & 0x80000) != 0) {
            first_delta = *(volatile const s32 *)&options->target30;
            accumulated = *(volatile const s32 *)&effect->accum18;
            first_delta -= accumulated;
            effect->rate30 = Func_02000890(first_delta,
                                           descriptor->duration);
            delta = options->target34;
            duration = descriptor->duration;
            delta -= effect->accum1c;
        } else {
            first_delta = options->target30;
            first_delta += (s32)0xffff0000;
            effect->rate30 = Func_020008a8(first_delta,
                                           descriptor->duration);
            delta = options->target34;
            duration = descriptor->duration;
            delta += (s32)0xffff0000;
        }

        effect->rate34 = Func_020008b6(delta, duration);
    }

    if ((flags & 0x200000) != 0) {
        Func_020008d4(effect, 1);
        Func_020008e4(effect, options->callback_arg);
    }

    if ((flags & 0x400000) != 0)
        block->angle = options->angle;
    if ((flags & 0x800000) != 0)
        *tag = options->step;
    if ((flags & 0x1000000) != 0)
        effect->callback = options->callback;
}

s32 Func_02000314(void)
{
    return (s32)Data_02008778;
}

s32 Func_0200031c(void)
{
    return 0;
}

s32 Func_02000320(void)
{
    return (s32)Data_02008868;
}

s32 Func_02000328(void)
{
    if (Data_02000240[225] == 8) {
        return (s32)Data_020089c8;
    }
    Func_020009b0(Data_02008890);
    return (s32)Data_02008890;
}

void Func_0200035c(void)
{
    u16 *state = Func_020009d6(0);
    u32 value = state[3];

    Func_020009c4();
    if (value >= 0xa001 && value <= 0xdfff) {
        Func_02000a2e(16, 14);
    } else {
        if (Func_020009c6(0x895) == 0) {
            Func_02000a20(0x1817);
        } else {
            Func_02000a28(0x1a46);
        }
        Func_02000a38(14, 0);
    }
    Func_02000a04();
}

void Func_020003bc(void)
{
    u16 *state = Func_02000a36(0);
    u32 value = state[3];

    Func_02000a24();
    if (value >= 0xa001 && value <= 0xdfff) {
        Func_02000a8e(17, 15);
    } else {
        if (Func_02000a26(0x895) == 0) {
            Func_02000a80(0x1819);
        } else {
            Func_02000a88(0x1a48);
        }
        Func_02000a98(15, 0);
    }
    Func_02000a64();
}

void Func_0200041c(void)
{
    void Func_02000af0();

    u16 *state = Func_02000a96(0);
    u32 value = state[3];

    Func_02000a84();
    if (value >= 0xa001 && value <= 0xdfff) {
        Func_02000aee(18, 16);
    } else if (Func_02000a86(0x895) == 0) {
        Func_02000ae0(0x181b);
        Func_02000af0(16, 0);
    } else {
        Func_02000af0(0x1a4a);
        Func_02000b10(16, 0);
    }
    Func_02000acc();
}

void Func_02000484(void)
{
    s32 Func_02000af0(s32);

    u16 *state = Func_02000afe(0);
    u32 value = state[3];

    Func_02000aec();
    if (value < 0x2000 || value > 0xe000) {
        Func_02000b60(5, 17);
    } else {
        if (Func_02000af0(0x895) == 0) {
            Func_02000b4a(0x181d);
        } else {
            Func_02000b52(0x1a4e);
        }
        Func_02000b62(17, 0);
    }
    Func_02000b2e();
}

void Func_020004e4(void)
{
    s32 Func_02000ba2(s32, s32);

    Func_02000b44();
    Func_02000b82(0x1a3a);
    Func_02000ba2(10, 0);
    Func_02000b5e();
}

void Func_02000504(void)
{
    void Func_02000ba2(s32);
    s32 Func_02000bc2(s32, s32);

    Func_02000b64();
    Func_02000ba2((s32)&Value_00001a40);
    Func_02000bc2(12, 0);
    Func_02000b7e();
}

void Func_02000524(void)
{
    void Func_02000bc2(s32);

    Func_02000b84();
    Func_02000bc2(0x1a64);
    Func_02000bdc(9, 0, 20);
    Func_02000bc6(9, 10, 0);
    Func_02000b9c(60);
    Func_02000bd6(9, 0, 0);
    Func_02000bac(20);
    Func_02000bfc(9, 0);
    Func_02000bc8();
}

s32 Func_02000570(void) {
    if (Data_02000240[225] == 8) {
        return (s32)Data_02008d4c;
    }
    return (s32)Data_02008a28;
}

s32 Func_02000598(void)
{
    void Func_02000686(s32, s32, s32, s32);

    s16 scene;

    *(s32 *)(Data_03001ebc + 448) = 521;
    scene = Data_02000240[225];
    if (scene == 4 || scene == 7) {
        Func_02000686(0x00f80000, 0, 0x01a10000, 20);
    } else if (scene == 6) {
        Func_02000678(0x01cc0000, 0, 0x02380000, 20);
        Func_02000686(0x01e40000, 0, 0x02380000, 20);
    } else if (scene == 8) {
        Func_02000c3e(0x12f);
        Func_02000c76(10, 6);
    }
    return 0;
}

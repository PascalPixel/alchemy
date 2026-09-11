#include "types.h"
#include "scene.h"
#include "abi/overlays/scene/dialogue/flagged_object.h"

typedef struct {
    u8 pad[9];
    u8 lo:2;
    u8 field:2;
    u8 hi:4;
} Object;

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
    s32 dur;
};

extern struct Descriptor *gOv[];
extern u8 gOv2[];
extern u8 gOv3[];
extern s16 gCell[];
extern u8 gOv4[];
extern u8 gOv5[];
extern u8 Value_00001a40;
extern u8 gOv6[];
extern u8 gOv7[];
extern u8 *gWork;

void *CreateOverlayObject(s32, s32, s32, s32);

struct Effect *Talk_Run(s32 slot);
struct Effect *Talk_Run2(s32 kind, s32 x, s32 y, s32 z);

u16 *Talk_Run3(s32);

u16 *Talk_Run4(s32);

u16 *Talk_Run5(s32);

u16 *Talk_Run6(s32);

/*
 * resource_3a1 configurable spawn/copy owner at 0x0200013c. The code ends
 * with the unwind at 0x02000306; its three-word pool occupies
 * 0x02000308-0x02000313. This is the same compiler-witnessed source family
 * as the exact resource_380/resource_39c/resource_3c9 owners, with this
 * overlay's own import veneers and descriptor table.
 */
void OvObj_SetEntryField(void *arg0, s32 arg1)
{
    Object *obj = *(Object **)((u8 *)arg0 + 0x50);

    obj->field = arg1;
}

void *OvObj_SpawnWithMode14(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
    void *Talk_Run7(s32, s32, s32, s32);

    u8 *ret = Talk_Run7(arg3, arg0, arg1, arg2);

    if (ret != NULL) {
        u8 *obj = *(u8 **)(ret + 0x50);
        s32 flags;
        s32 mask = 13;

        flags = obj[9];
        mask = -mask;
        mask &= flags;
        obj[9] = mask;
        ret[0x55] = 0;
        ret[0x59] = 8;
        Talk_Apply(ret, 0);
        Talk_Apply2(ret, 14);
        Talk_Apply3(ret, 1);
        return ret;
    }
    return NULL;
}

void *OvObj_CreateConfigured(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
    u8 *ret = CreateOverlayObject(arg3, arg0, arg1, arg2);

    if (ret != NULL) {
        u8 *obj = *(u8 **)(ret + 0x50);
        s32 flags;
        s32 mask = 13;

        flags = obj[9];
        mask = -mask;
        mask &= flags;
        mask |= 4;
        obj[9] = mask;
        ret[0x55] = 0;
        ret[0x59] = 8;
        SetOverlayObjectMode(ret, 0);
        SetOverlayObjectSlot(ret, 15);
        ret[0x23] = (ret[0x23] & 0xfe) | 2;
        return ret;
    }
    return NULL;
}

void OvObj_IntegrateVelocities(void *arg0)
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

void Effect_SpawnConfiguredEffect(s32 x, s32 y, s32 z, s32 vx, s32 vy, s32 vz,
                   u32 flags, const struct Options *opt)
{
    u32 off;
    struct Effect *party;
    u32 bits;
    s32 fmask;
    u32 bbits;
    struct Effect *effect;
    struct Sprite *block;
    struct Sprite *mblock;
    u32 obits;
    u16 *tag;
    s32 dur;
    s32 d1;
    s32 acc;

    party = Talk_Run(0);
    if ((flags & 0x100000) != 0 && opt != 0) {
        effect = Talk_Run2(opt->kind, x, y, z);
    } else {
        effect = Talk_Run2(222, x, y, z);
    }
    if (effect == 0)
        return;

    block = effect->sprite;
    mblock = block;

    Talk_Apply4(effect, (flags + 1) & 15);
    off = (flags & 15) << 2;
    Talk_Apply5(effect, gOv[off >> 2]);

    effect->mode55 = 0;
    block->state26 = 0;
    effect->callback = 0x02008105;

    effect->velocity_x = vx;
    x = 3;
    effect->velocity_y = vy;
    effect->velocity_z = vz;

    bits = party->sprite->flags9 & 12;
    bbits = *(volatile u8 *)&block->flags9;
    fmask = ~12;
    block->flags9 = (u8)((bbits & fmask) | bits);

    effect->rate30 = 0;
    effect->rate34 = 0;
    effect->step64 = 0;
    tag = &effect->step64;

    if ((flags & 0xffff0000) == 0 || opt == 0)
        return;

    if ((flags & 0x10000) != 0)
        Talk_Apply6(effect, opt->mode);

    if ((flags & 0x20000) != 0) {
        effect->flags23 &= 0xfe;
        obits = *(const u8 *)opt & x;
        block->flags9 = (u8)((*((const u8 *)mblock + 9) & fmask)
                             | (obits << 2));
    }

    if ((flags & 0x80000) != 0) {
        effect->accum18 = opt->accum18;
        effect->accum1c = opt->accum1c;
    }

    if ((flags & 0x40000) != 0) {
        const struct Descriptor *desc =
            gOv[off >> 2];
        s32 delta;

        if ((flags & 0x80000) != 0) {
            d1 = *(volatile const s32 *)&opt->target30;
            acc = *(volatile const s32 *)&effect->accum18;
            d1 -= acc;
            effect->rate30 = Talk_Apply7(d1,
                                           desc->dur);
            delta = opt->target34;
            dur = desc->dur;
            delta -= effect->accum1c;
        } else {
            d1 = opt->target30;
            d1 += (s32)0xffff0000;
            effect->rate30 = Talk_Apply8(d1,
                                           desc->dur);
            delta = opt->target34;
            dur = desc->dur;
            delta += (s32)0xffff0000;
        }

        effect->rate34 = Talk_Apply9(delta, dur);
    }

    if ((flags & 0x200000) != 0) {
        Talk_Apply10(effect, 1);
        Talk_Apply11(effect, opt->callback_arg);
    }

    if ((flags & 0x400000) != 0)
        block->angle = opt->angle;
    if ((flags & 0x800000) != 0)
        *tag = opt->step;
    if ((flags & 0x1000000) != 0)
        effect->callback = opt->callback;
}

s32 SceneData_GetTable8778(void)
{
    return (s32)gOv2;
}

s32 SceneData_ReturnZero(void)
{
    return 0;
}

s32 SceneData_GetTable8868(void)
{
    return (s32)gOv3;
}

s32 SceneData_SelectTable89c8Or8890(void)
{
    if (gCell[225] == 8) {
        return (s32)gOv4;
    }
    Talk_Do(gOv5);
    return (s32)gOv5;
}

void Dialogue_RunActor14FlaggedDialogue(void)
{
    u16 *state = Talk_Run3(0);
    u32 value = state[3];

    Talk_Run8();
    if (value >= 0xa001 && value <= 0xdfff) {
        Talk_Apply12(16, 14);
    } else {
        if (Talk_Check(0x895) == 0) {
            Talk_Do2(0x1817);
        } else {
            Talk_Do3(0x1a46);
        }
        Talk_Apply13(14, 0);
    }
    Talk_Run9();
}

void Dialogue_RunActor15FlaggedDialogue(void)
{
    u16 *state = Talk_Run4(0);
    u32 value = state[3];

    Talk_Run10();
    if (value >= 0xa001 && value <= 0xdfff) {
        Talk_Apply14(17, 15);
    } else {
        if (Talk_Check2(0x895) == 0) {
            Talk_Do4(0x1819);
        } else {
            Talk_Do5(0x1a48);
        }
        Talk_Apply15(15, 0);
    }
    Talk_Run11();
}

void Dialogue_RunActor16FlaggedDialogue(void)
{
    void Talk_Run12();
    void Talk_Run13(s32);

    u16 *state = Talk_Run5(0);
    u32 value = state[3];

    Talk_Run14();
    if (value >= 0xa001 && value <= 0xdfff) {
        Talk_Apply16(18, 16);
    } else if (Talk_Check3(0x895) == 0) {
        Talk_Do6(0x181b);
        Talk_Run12(16, 0);
    } else {
        Talk_Run13(0x1a4a);
        Talk_Apply17(16, 0);
    }
    Talk_Run15();
}

void Dialogue_RunActor17FlaggedDialogue(void)
{
    s32 Talk_Run16(s32);

    u16 *state = Talk_Run6(0);
    u32 value = state[3];

    Talk_Run17();
    if (value < 0x2000 || value > 0xe000) {
        Talk_Apply18(5, 17);
    } else {
        if (Talk_Run16(0x895) == 0) {
            Talk_Do7(0x181d);
        } else {
            Talk_Do8(0x1a4e);
        }
        Talk_Apply19(17, 0);
    }
    Talk_Run18();
}

void Dialogue_RunActor10Dialogue(void)
{
    s32 Talk_Run19(s32, s32);

    Talk_Run20();
    Talk_Do9(0x1a3a);
    Talk_Run19(10, 0);
    Talk_Run21();
}

void Dialogue_RunActor12Dialogue(void)
{
    void Talk_Run22(s32);
    s32 Talk_Run23(s32, s32);

    Talk_Run24();
    Talk_Run22((s32)&Value_00001a40);
    Talk_Run23(12, 0);
    Talk_Run25();
}

void Dialogue_RunActor9MotionDialogue(void)
{
    void Talk_Run26(s32);

    Talk_Run27();
    Talk_Run26(0x1a64);
    Talk_Place(9, 0, 20);
    Talk_Place2(9, 10, 0);
    Talk_Do10(60);
    Talk_Place3(9, 0, 0);
    Talk_Do11(20);
    Talk_Apply20(9, 0);
    Talk_Run28();
}

s32 SceneData_SelectTable8d4cOr8a28(void)
{
    if (gCell[225] == 8) {
        return (s32)gOv6;
    }
    return (s32)gOv7;
}

s32 State_SetRuntimeWord448To521(void)
{
    void Talk_Run29(s32, s32, s32, s32);

    s16 scene;

    *(s32 *)(gWork + 448) = 521;
    scene = gCell[225];
    if (scene == 4 || scene == 7) {
        Talk_Run29(0x00f80000, 0, 0x01a10000, 20);
    } else if (scene == 6) {
        Talk_SetMode(0x01cc0000, 0, 0x02380000, 20);
        Talk_Run29(0x01e40000, 0, 0x02380000, 20);
    } else if (scene == 8) {
        Talk_Do12(0x12f);
        Talk_Apply21(10, 6);
    }
    return 0;
}

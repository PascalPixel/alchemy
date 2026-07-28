typedef signed int s32;
typedef signed short s16;
typedef unsigned char u8;

struct Record {
    u8 filler00[0x28];
    s16 *entry;
};

struct Object {
    u8 filler00[8];
    s32 x;
    u8 filler0C[4];
    s32 z;
    u8 filler14[0x3c];
    struct Record *record;
    u8 state;
};

extern s16 Data_02000240[];

extern struct Object *Func_02007198(s32);
extern void Func_02007186(void);
extern struct Object *Func_020071a6(s32);
extern void Func_020072a0(s32);
extern void Func_020072b8(s32, s32);
extern void Func_02007210();
extern void Func_02007374(void);
extern void Func_02007380(void);
extern void Func_02007200(s32, s32);
extern void Func_0200735a(s32);
extern void Func_0200720a(s32);
extern void Func_02004e9a(s32);
extern void Func_020073ae(void);
extern void Func_020073c2(void);
extern void Func_02007274(void);

void Func_02002758(void)
{
    u8 *state;
    s16 *table;
    struct Object *target;
    struct Object *object;
    s32 *frame;
    s32 slotValue;
    s32 slot;
    s32 best;
    s32 bestSlot;
    s32 dx;
    s32 adx;
    s32 dz;
    s32 base;
    s32 z;

    state = *(u8 **)0x03001ebc;
    bestSlot = 8;
    best = 0x100000;
    table = Data_02000240;
    slotValue = *(s32 *)&table[250];
    target = Func_02007198(slotValue);
    Func_02007186();
    for (slot = 8; slot <= 66; slot++) {
        object = Func_020071a6(slot);
        if (object == 0) {
            continue;
        }
        if (object->state != 1) {
            continue;
        }
        if (*object->record->entry != 165) {
            continue;
        }
        dx = (target->x - object->x) / 65536;
        dz = (target->z - object->z) / 65536;
        if (dz > 0) {
            continue;
        }
        adx = dx;
        if (adx < 0) {
            adx = -adx;
        }
        if (dz < 0) {
            dz = -dz;
        }
        if (adx + dz < best) {
            bestSlot = slot;
            best = adx + dz;
        }
    }
    Func_020072a0(0x2085);
    Func_020072b8(bestSlot, 0);
    frame = (s32 *)(state + 448);
    *frame = 0x200;
    *(s32 *)(state + 456) = 15;
    Func_02007210(20);
    Func_02007374();
    Func_02007380();
    base = slotValue * 16;
    Func_02007200(base + 880, target->x >> 20);
    z = target->z >> 20;
    Func_02007210(base + 888, z);
    slotValue = slotValue + 1;
    if (slotValue > 3) {
        Func_0200735a(10);
        Func_0200720a(282);
    } else {
        Func_02004e9a(slotValue);
        Func_020073ae();
        Func_020073c2();
        *frame = 0;
    }
    Func_02007274();
}

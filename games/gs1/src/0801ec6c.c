#include "types.h"
#include "gs1_edition.h"

struct State_0801ec6c {
#if defined(GS1_EDITION_JA)
    u8 pad_0000[0x117c];
    u16 ids_117c[2];
    u16 values_1180[2];
#else
    u8 pad_0000[0x12ec];
    u16 ids_12ec[2];
    u16 values_12f0[2];
#endif
};

#if defined(GS1_EDITION_JA)
#define STATE_IDS ids_117c
#define STATE_VALUES values_1180
#else
#define STATE_IDS ids_12ec
#define STATE_VALUES values_12f0
#endif

struct Object_0801ec6c {
    u8 pad_00[4];
    u8 mode_04;
    u8 pad_05[20];
    u8 slot_19;
};

extern struct State_0801ec6c *Data_03001e8c;

extern s32 Func_080770c0(s32);
extern s32 Func_08019d2c(s32);
extern void Func_0801a4fc(s32, s32, s32 *, s32 *, s32, s32);
extern struct Object_0801ec6c *Func_0801eadc(
    s32, s32, s32, s32, s32);

struct Object_0801ec6c *Func_0801ec6c(
    s32 kind, s32 arg1, s32 side, s32 arg3, s32 arg4, s32 arg5)
{
    struct State_0801ec6c *state = Data_03001e8c;
    struct Object_0801ec6c *object = 0;
    s32 first;
    s32 second;
    s32 id;
    s32 slot;

    if (Func_080770c0(32) != 0) {
        if (kind == 0)
            kind = 18;
        if (kind == 1)
            kind = 19;
    }

    id = Func_08019d2c(kind);
    if (id == -1)
        return object;

    if ((u32)side > 1) {
        side = 1;
        if (state->STATE_IDS[1] != 999) {
            side = 0;
            if (state->STATE_IDS[0] != 999)
                return object;
        }
    }

    slot = 14 + side;
    Func_0801a4fc(id, arg1, &first, &second, slot, 0);
    object = Func_0801eadc(first, 0x80000000, arg3, arg4, arg5);
    if (object != 0) {
        s32 slotBits = slot << 4;

        object->slot_19 = (object->slot_19 & 15) | slotBits;
        object->mode_04 = 2;
    }

    state->STATE_IDS[side] = id;
    state->STATE_VALUES[side] = first;
    return object;
}

/* Draft, differ 29 of 49: the native shifts the random offset in r0 and builds x in r1 and z in r3 from it without a saved copy; this form copies the offset to r1 first. */
#include "../../../../games/tla/src/field/overlay_678/local.h"


struct PartyState254 {
    u8 unknown_000[0x254];
    u16 value;
};

void Func_020009c0(s32 id);
void Func_02000a94(s32 id);
void Func_020024f8(struct ObjectRuntime *object, s32 priority);
void Func_02002554(s32 *position, s32 base);

/* The accessors step the work pointer by whole entries and then take entry 0: the bytes add the base to index * 20 before the field offset. */
#define VENT_AT(work, index) ((struct VentWork *)((u8 *)(work) + (index) * sizeof(struct VentEntry)))

/* FAKEMATCH: the callers set up the arguments as for a call whose value is used (flag before index); the setter is declared void for the modules that match with that. */
#define Vent_SetActive(index, active) ((s32 (*)(s32, s32))Func_0200271c)(index, active)

static __inline__ void Blend_SetAlpha(s32 value)
{
    REG_BLDALPHA->value = value;
}

void Func_02001684(void)
{
    if ((gFrameCount & 7) == 0 && (u32)Random16Far() < 0x4000) {
        s32 offset = Random16Far() << 5;
        struct ObjectRuntime *object = Func_02002808_(0x12d, offset + 0x1800000, 0xa00000, 0x1900000 - offset);

        s32 scale = 0x547a;


        if (object != NULL) {

            object->scale_x = scale;

            object->scale_y = scale;
            object->motion_flags = 2;
            object->terrain_height = -0x400000;
            object->gravity = 0x7333;
            Func_02002800(object, Data_02002a6c);
            ObjectDispatch_SetSingleChildFlags(object, 0);
        }
    }
}

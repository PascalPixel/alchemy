/* Draft, differ 6 of 34: argument set-up order for the object creation (the native builds r1, r3, r2, then r0 and finishes r0 last). */
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

void Func_02001618(void)
{
    if ((gFrameCount & 63) == 0) {
        struct ObjectRuntime *object = Func_02002808_(0x12d, 0x1980000, 0xa00000, 0x1b80000);

        s32 scale = 0x547a;


        if (object != NULL) {

            object->scale_x = scale;

            object->scale_y = scale;
            object->gravity = 0x7333;
            Func_02002800(object, Data_02002a60);
            ObjectDispatch_SetSingleChildFlags(object, 0);
        }
    }
}

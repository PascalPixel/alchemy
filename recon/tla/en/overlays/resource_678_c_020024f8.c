/* Draft, differ 4 of 40: the native builds the priority mask 3 before it loads the sprite pointer and masks the priority before it reads the sprite byte; this form loads first. Masking the parameter in its own statement is differ 15. */
/* Trial: the sprite pointer in a FieldSprite local (sprite = object->animation; sprite->priority = priority) is differ 4 too, the same two loads ahead of the mask. */
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

void Func_020024f8(struct ObjectRuntime *object, s32 priority)
{
    object->motion_flags = 0;
    object->collision_flags = 0;
    object->priority_flags = 2;
    ((struct FieldSprite *)object->animation)->priority = priority;
    ObjectDispatch_SetSingleChildFlags(object, 0);
    Func_020027f8(object, 2);
    Func_02002800(object, Data_02002af4);
    Object_SetPartAttribute(object, 10);
    object->action = 0;
    object->speed_limit = 0;
    object->acceleration = 0;
}

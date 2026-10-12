/* Draft, differ 5 of 29: the native loads the work base into r3, the hook into r0 (a call whose value is used) and copies the base to r5 for the entries (adds r5, r3; then adds r5, #8 after the call); this form loads the base into r5 directly. */
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

void Func_020025d4(void)
{
    struct VentWork *work = VENT_WORK;
    struct VentWork *base;
    struct VentEntry *entry;
    s32 i = 15;

    base = work;
    ((s32 (*)(void))work->hook)();
    entry = base->entries;
    for (; i >= 0; i--) {
        if (entry->active != 0) {
            if (entry->timer <= 0) {
                Func_02002554(&entry->x, entry->value);
                entry->timer = 9;
            } else {
                entry->timer--;
            }
        }
        entry++;
    }
}

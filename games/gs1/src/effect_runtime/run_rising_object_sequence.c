#include "object_lookup.h"
#include "types.h"
#include "scene.h"
#include "abi/effect_runtime/run_rising_object_sequence.h"

#define FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

void Object_SetCallback(void *, s32);
void WaitFrames(u32);
void EffectRuntime_PrepareRisingObject(struct Object_0808f0d8 *object);
void Object_SetMode(void *, s32);

struct GlobalState {
    u8 padding_000[500];
    u32 selected_object;
};

extern struct GlobalState gCell;
extern const u8 gRom[];
extern u8 gRom2;

void EffectRuntime_RunRisingObjectSequence(void *object, s32 flags)
{
    void *other;

    if (object != NULL) {
        other = ObjectTable_Get(gCell.selected_object);
        if (flags & 1) {
            Sys_Apply(object, 0);
            Object_SetCallback(object, (s32)gRom);
            FIELD(object, u32, 0x28) = 0x20000;
            FIELD(object, u32, 0x48) = 0x4000;
            FIELD(object, s32, 0x6C) = (s32)&gRom2;
        }
        if (flags == 3) {
            WaitFrames(60);
        }
        if (flags & 2) {
            EffectRuntime_PrepareRisingObject(object);
        }
        if (flags == 3) {
            WaitFrames(80);
        }
        Object_SetMode(other, 1);
    }
}

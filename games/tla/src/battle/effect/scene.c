#include "types.h"
#include "ram_buffer.h"
#include "effect_slot.h"

void Field_BeginPaletteTransition(s32 frames);

void Func_080dc37c(s32 frames)
{
    Field_BeginPaletteTransition(frames);
}

void BattleFx_PrepareBufferInterpolation(void)
{
    Field_BeginPaletteTransition(8);
}

struct EffectVector {
    s32 x;
    s32 y;
    s32 z;
};

s32 Render_ProjectPoint(struct EffectVector *position, struct EffectVector *result);

void Camera_WorldToScreen(struct EffectVector *position)
{
    struct HeapSlots *slots = Ram_HeapSlots;

    if (*(s8 *)((u8 *)slots->eventWork + 0x18a) == 3) {
        struct EffectVector result;

        Render_ProjectPoint(position, &result);
        position->x = result.x << 16;
        position->z = result.y << 16;
        position->y = 0;
    } else {
        u8 *state = (u8 *)slots->mapWork;
        s32 x = *(s32 *)(state + 228) & 0xffff0000;
        s32 z = *(s32 *)(state + 232) & 0xffff0000;

        position->x -= x;
        position->z -= position->y;
        position->z -= z;
        position->y = 0;
    }
}

struct BattleFxScene {
    u8 unk00[0x50];
    struct EffectSlot slots[24];
    u8 unk710[0xe4];
};

void Func_080dc3f0(void)
{
    struct BattleFxScene *scene;
    struct EffectSlot *slot;
    s32 remaining_slots;

    scene = Ram_HeapSlots->effectWork;
    remaining_slots = 0x17;
    slot = scene->slots;
    do {
        remaining_slots -= 1;
        EffectSlot_Update(slot);
        slot++;
    } while (remaining_slots >= 0);
}

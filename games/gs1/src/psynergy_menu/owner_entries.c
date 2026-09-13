#include "types.h"

void Func_080a1bdc(s32 arg0, s32 arg1, s32 arg2);
void PsynergyMenu_RefreshOwnerEntriesDefault(void)
{
  int val;
  val = 0x28;
  Func_080a1bdc(0x6C, val, 8);
}
#include "m7_interfaces.h"
#include "global_cells.h"

void PsynergyMenu_RefreshOwnerEntries(s32 origin_x, s32 origin_y, s32 phase)
{
    s32 _v1d16;
    s32 _v1t36;
    s32 _v1d75;
    struct Object080a1c **scan;
    struct Object080a1c **slot;
    struct Object080a1c *object;
    s32 index;

    index = 0;
    _v1t36 = (*(s32 *)ADDR_03001F2C);
    slot = (struct Object080a1c **)(_v1t36 + 0x48);
    scan = slot;
    do {
        object = *scan++;
        if (object != NULL) {
            PsynergyMenu_PositionOwnerEntry(slot, index, origin_x, origin_y, phase);
        }
        _v1d75 = 0; index += 1;
        _v1d16 = 0; slot++;
    } while (index <= 0x1F);
}

#define FIELD_AT_OFFSET(base, type, offset) (*(type)((u8 *)(base) + (offset)))

s32 FixedPoint_Ratio(s32, s32);
s32 Modulo(s32, s32);

void PsynergyMenu_PositionOwnerEntry(struct Object080a1c **slot, s32 index,
    s32 origin_x, s32 origin_y,
    s32 phase) {
    struct Object080a1c *object;
    s32 wave;
    s32 stable_phase;
    s32 wrapped_index;

    wrapped_index = index;
    if (wrapped_index > 0x1F) {
        wrapped_index = 0;
    }
    stable_phase = *(volatile s32 *)&phase;
    object = *(struct Object080a1c *volatile *)slot;
    wave = FixedPoint_Ratio(wrapped_index, stable_phase);
    object->y = (s16)((wave * 0x10) + origin_y);
    object->x =
        (s16)((Modulo(wrapped_index, phase) * 0x10) + origin_x);
    Func_080a17c4(object);
}

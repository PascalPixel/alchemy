/* Draft, differ 76 of 76 by count, the structure is the native's (overlays
 * 69B's Func_02001fbc and 6A4's Func_020027d8 are the same routine). Two
 * things differ. The native reads the cell count twice before the loop
 * (ldrsh for the entry test, ldrh into r2) and sign-extends r2 at the loop's
 * end on every pass (lsls r3, r2, #16; asrs r3, r3, #16); this form extends
 * once into ip. And the native keeps the work in r6 and the index in r7,
 * the other way round here. Measured: the count as u16 with an (s16) cast,
 * and as a 16-bit signed bitfield (the same code as the s16 field). */
#include "types.h"
#include "party_state.h"
#include "field_event.h"

struct LightCell {
    s16 x;
    s16 z;
    s16 flag;
    s16 unknown_06;
};

struct LightWork {
    s16 unknown_00;
    s16 flag_base;
    u8 unknown_04[6];
    s16 count;
    s16 current;
    u8 unknown_0e[18];
    struct LightCell cells[1];
};

extern struct LightWork Data_020023a0;

struct FieldActor *Object_GetById(s32 id);
s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void GameFlag_SetByte(s32 flag, s32 value);
void Func_020014fc(struct LightWork *work, struct LightCell *cell);
void Func_0200156c(struct LightWork *work, struct LightCell *cell);

s32 Func_0200189c(void)
{
    struct FieldActor *object = Object_GetById(gPartyState.current_owner);
    struct LightWork *work = &Data_020023a0;
    s32 x = object->x >> 20;
    s32 z = object->z >> 20;
    struct LightCell *cell = work->cells;
    s32 i;

    for (i = 0; i < work->count; i++, cell++) {
        if (cell->x == x && cell->z == z) {
            if (!GameFlag_Test(cell->flag)) {
                Func_020014fc(work, cell);
                GameFlag_SetBit(cell->flag);
                work->current = i;
                break;
            }
            if (i != work->current) {
                work->current = i;
                Func_0200156c(work, cell);
                GameFlag_SetByte(work->flag_base, x);
                GameFlag_SetByte(work->flag_base + 8, z);
                return 1;
            }
            break;
        }
    }
    return 0;
}

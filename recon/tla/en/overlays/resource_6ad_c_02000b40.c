/* Near miss, differ 4 of 129, written from the listing. Left: one reordered pair in the
 * direction read: the native loads the table's address before 15 (ldr r3, =gInput; ldr r1,
 * =Data_02005f18; ldr r3, [r3]; movs r2, #15), where sched2 here ranks the mask's 15 above the
 * table's address (priorities 6 and 5). Overlay 6ad's Func_02004080 reads its table in this
 * draft's order and matches. Tried: a table pointer, a keys local, volatile gInput, gInput.held,
 * an s16 angle, an asm barrier or a one-pass block around the read, permuter 3 minutes. */
#include "../../../../games/tla/src/field/overlay_6ad/room.h"

extern u32 gInput;
extern const s16 Data_02005f18[];
extern u8 gMapCellBuffer[];

void Func_020009ec(void);
void Func_02000cb8(s32 id);

void Func_02000b40(void)
{
    struct Box box;

    if (Func_02004a8c(&box)) {
        Func_02004d10(box);
        Func_020009ec();
        if (Func_020027dc(0) != 64 && Flag_IsSet(0x221)) {
            struct ObjectRuntime *object = Object_GetById(12);

            WORD_AT(object, 108) = 0;
            Animation_ApplyChildValues(object, 0);
            Object_SetModeById(12, 6);
            Flag_Clear(0x221);
            Func_02000cb8(12);
        }
        if (Func_020027dc(2) != 64 && Flag_IsSet(0x220)) {
            struct ObjectRuntime *object = Object_GetById(11);

            WORD_AT(object, 108) = 0;
            Animation_ApplyChildValues(object, 0);
            Object_SetModeById(11, 6);
            Flag_Clear(0x220);
            Func_02000cb8(11);
        }
    } else {
        s32 angle;

        Object_GetById(gPartyState.current_owner);
        angle = Data_02005f18[(gInput >> 4) & 15];
        if (angle != -1) {
            struct MapLayers *map = (struct MapLayers *)Ram_HeapSlots->map_work;
            struct ObjectRuntime *leader = Object_GetById(gPartyState.current_owner);
            s32 position[3];
            s32 x;
            s32 z;
            u8 *cells;

            position[0] = (leader->x & 0xfff00000) + 0x80000;
            position[1] = leader->y;
            position[2] = (leader->z & 0xfff00000) + 0x80000;
            Vector_AddPolarOffsetFar(0x100000, angle, position);
            x = position[0] >> 20;
            z = position[2] >> 20;
            cells = gMapCellBuffer;
            if (map != NULL)
                cells = map->layers[2].cells;
            cells += (x + (z << 7)) << 2;
            if (cells[2] != 255)
                Func_02005600();
        }
    }
}

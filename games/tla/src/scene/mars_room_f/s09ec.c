#include "room.h"
#include "input.h"

extern s16 gData_02005f18[];

void Func_02000cb8(s32 id);

void Func_020009ec(void)
{
    Func_020027ac(2, 65);
    Func_020027ac(1, 65);
    Func_020027ac(0, 65);
    Flag_Clear(0x350);
    if (Object_GetById(10)->z >> 20 != 15 && Object_GetById(9)->z >> 20 != 15) {
        Func_020027ac(1, 33);
        return;
    }
    if (Object_GetById(9)->z >> 20 != 13) {
        Func_020027ac(0, 17);
        return;
    }
    if (Object_GetById(8)->x >> 20 != 37) {
        Func_020027ac(2, 49);
        return;
    }
    Flag_Set(0x350);
    if (!Flag_IsSet(0x351)) {
        Func_020055d0(0x20000, 0x4000);
        Camera_SetBounds(0x2680000, -1, 0x180000, 1);
        Battle_WaitMode0(60);
        Func_02005660();
        Func_020055f0(9);
    }
}

void Func_02000aac(void)
{
    Func_020027ac(3, 3);
    Func_020055d0(0x10000, 0x2000);
    Camera_SetBounds(0xb80000, -1, 0x2300000, 1);
}

void Func_02000ad8(void)
{
    struct SceneWork *work = Ram_HeapSlots->eventWork;

    Flag_Set(0x106);
    Func_02005560();
    Func_02005648(0);
    work->unk166 = 0;
    Flag_Clear(0x106);
    Flag_Clear(0x104);
    Battle_WaitMode0(60);
    Func_020055d0(0x30000, 0x6000);
    Camera_SetBounds(0xb80000, -1, 0x2e80000, 1);
    Func_020055e0();
    Battle_WaitMode0(60);
    Func_02005568();
}

void Func_02000b40(void)
{
    struct Box box;

    if (Func_02004a8c(&box)) {
        Func_02004d10(box);
        Func_020009ec();
        if (Func_020027dc(0) != 64 && Flag_IsSet(0x221)) {
            struct ObjectRuntime *object = Object_GetById(12);

            object->update = NULL;
            Animation_ApplyChildValues(object, 0);
            Object_SetModeById(12, 6);
            Flag_Clear(0x221);
            Func_02000cb8(12);
        }
        if (Func_020027dc(2) != 64 && Flag_IsSet(0x220)) {
            struct ObjectRuntime *object = Object_GetById(11);

            object->update = NULL;
            Animation_ApplyChildValues(object, 0);
            Object_SetModeById(11, 6);
            Flag_Clear(0x220);
            Func_02000cb8(11);
        }
    } else {
        s32 angle;
        const s16 *table;
        u32 *keys;

        Object_GetById(gPartyState.currentOwner);
        keys = &gInput.held;
        table = gData_02005f18;
        /* FAKEMATCH: the barrier loads the table's address before the direction mask's 15, where plainly sched2 builds the 15 first (differ 4); reading through a byte offset keeps the table as the base register (indexed plainly, differ 2). */
        asm volatile("" : "+l"(table));
        angle = *(s16 *)((u8 *)table + (((*keys >> 4) & 15) << 1));
        if (angle != -1) {
            struct MapLayers *map = (struct MapLayers *)Ram_HeapSlots->mapWork;
            struct ObjectRuntime *leader = Object_GetById(gPartyState.currentOwner);
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
            cells = Ram_MapCellBuffer;
            if (map != NULL)
                cells = map->layers[2].cells;
            cells += (x + (z << 7)) << 2;
            if (cells[2] != 255)
                Func_02005600();
        }
    }
}

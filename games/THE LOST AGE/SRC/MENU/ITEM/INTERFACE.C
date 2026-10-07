#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "LIB.H"
#include "MENU.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "TYPES.H"
#include "RAM_BUFFER.H"
#include "RENDER_INPUT.H"

#define RenderOutput_CreateFromResourceFar ((void * (*)(s32,s32,s32,s32,s32))RenderOutput_CreateFromResourceFar)

struct IconMenuState {
    u8 padding_000[36];
    s32 handle;
    u8 padding_028[36];
    void *entries[32];
};


void Menu_SpawnIconEntries(struct IconMenuState *state, s32 arg1)
{
    void **output0;
    s32 index0;
    s32 fifth0;
    void **output1;
    s32 index1;
    s32 fifth1;
    void **output2;
    s32 index2;
    s32 fifth2;

    index0 = 0;
    fifth0 = 168;
    output0 = &state->entries[0];
    do {
        *output0++ = RenderOutput_CreateFromResourceFar(2, index0, arg1, 248, fifth0);
        index0++;
    } while (index0 <= 7);
    index1 = 8;
    fifth1 = 168;
    output1 = &state->entries[8];
    do {
        *output1++ = RenderOutput_CreateFromResourceFar(2, index1, arg1, 256, fifth1);
        index1++;
    } while (index1 <= 15);
    index2 = 16;
    fifth2 = 168;
    output2 = &state->entries[16];
    do {
        *output2++ = RenderOutput_CreateFromResourceFar(2, index2, arg1, 256, fifth2);
        index2++;
    } while (index2 <= 31);
}

void ItemMenu_HideAllIcons(void)
{
    s32 hidden_state = 13;
    struct RenderOutput **icons = (struct RenderOutput **)((struct IconMenuState *)Ram_HeapSlots->menu_runtime)->entries;
    s32 slot;

    for (slot = 31; slot >= 0; slot--) {
        struct RenderOutput *icon = *icons++;
        if (icon != 0) {
            icon->active = hidden_state;
        }
    }
}

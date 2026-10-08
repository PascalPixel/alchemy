#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "callback_scheduler.h"
#include "psynergy_menu.h"
#include "animspr.h"
#include "field_sprite.h"
#include "resource.h"
#include "scene.h"
#include "m7_interfaces.h"
#include "global_cells.h"

void Object_ApplyProjectedPlacementFar(void *, s32 *, s32 *, s32);

struct MenuCursorAttributes {
    u16 y : 8;
    u16 affine_mode : 2;
    u16 object_mode : 2;
    u16 mosaic : 1;
    u16 palette_256 : 1;
    u16 shape : 2;
    u16 x : 9;
    u16 matrix : 5;
    u16 size : 2;
};

struct MenuCursorSprite {
    u8 reserved_00[6];
    u16 x;
    u16 y;
    u8 reserved_0a[10];
    struct MenuCursorAttributes attributes;
};

void WaitFrames(s32 frames);

extern volatile u32 gFrameCount;

s32 UiMenu_CreateCursor(void *source)
{
    struct PsynergyMenuState *work = source;
    s32 handle;
    s32 zero = 0;
    s32 state;
    struct RenderOutput *object;

    work->auxiliary_window = zero;
    UiWindow_UpdateOrCreate(&work->auxiliary_window, zero, zero, 13, 5, 2);
    handle = work->auxiliary_window;
    object = (struct RenderOutput *)UiIcon_CreateWithResourceVariant((struct RenderInput *)handle, -8, 11);
    object->active = 13;
    work->tab_index[0] = 255;
    work->tab_index[1] = zero;
    work->pane_icon[0] = object;
    state = 254;
    object->sentinel = state;
    state -= 255;
    work->pane_icon[1]->sentinel = state;
    return handle;
}

void PsynergyMenu_InitializeEntryObjects(void *source, s32 origin_x, s32 origin_y, s32 spacing)
{
    u16 entry_ids[14];
    struct PsynergyMenuState *entry_state = gMenuWork;
    struct RenderInput *window = source;
    s32 entry_count = (u16)Party_ListActiveOwnersFar(entry_ids);
    s32 i;

    entry_state->tab_counts[0] = entry_count;
    for (i = 0; i < entry_count; i++) {
        struct AnimationObject *entry_object = (struct AnimationObject *)ResourceObject_CreateFar(Party_RemapCharacterIdByFlagsFar(entry_ids[i]));
        if (entry_object != 0) {
            s32 entry_x;
            s32 source_x;
            s32 position_x;

            entry_state->tab_objects[i] = entry_object;
            source_x = window->x;
            entry_x = spacing + 16;
            entry_x *= i;
            position_x = origin_x + source_x;
            entry_state->tab_x[i] = position_x * 8 + entry_x;
            entry_state->row_positions[i] =
                (origin_y + window->y) * 8 + 16;
            entry_state->owner_scale[i] = 0x10000;
            ((struct FieldSprite *)entry_object)->priority = 0;
            entry_object->flags = 0;
            AnimationObjects_SelectAnimationFar(entry_object, 1);
        }
    }
    for (; i < 8; i++) {
        entry_state->tab_objects[i] = 0;
    }
    {
        s32 delay_frames = 200;
        delay_frames <<= 4;
        Scheduler_AddOrUpdateCallback(Menu_UpdateEntryObjectTransforms, delay_frames);
    }
}

void Menu_ReleaseEntryObjects(void)
{
    u32 buf[7];
    struct PsynergyMenuState *base = gMenuWork;
    s32 count;
    void **p;
    s32 i;

    count = (u16)Party_ListActiveOwnersFar((s16 *)buf);
    if (count != 0) {
        p = (void **)base->tab_objects;
        i = count;
        do {
            void *entry = *p++;

            if (entry != 0) {
                ResourceObject_ReleaseFar(entry);
            }
        } while (--i != 0);
    }
    Scheduler_RemoveCallback((void (*)(void))Menu_UpdateEntryObjectTransforms);
}

void Menu_UpdateEntryObjectTransforms(void)
{
    /* FAKEMATCH: signed sprite-byte writes and the byte cursor preserve loop registers and write order. */
    u8 *p;
    s32 pos[2];
    s32 trans[4];
    s32 *pp;
    s32 *tp;
    s16 *hp;
    s32 i;
    s32 cnt;

    p = (u8 *)gMenuWork;
    cnt = (u16)Party_CountActiveOwnersFar();
    i = 0;
    if (i < cnt) {
        pp = pos;
        tp = trans;
        hp = (s16 *)(p + 308);
        p += 276;
        do {
            void *obj;
            s32 top;

            top = 0x01e20000 - (hp[8] << 16);
            obj = *(void **)p;
            if (obj != 0) {
                *((s8 *)obj + 9) &= -13;
                pos[0] = *(s32 *)(p + 64);
                pp[1] = *(s32 *)(p + 64);
                tp[1] = top;
                tp[0] = hp[0] << 16;
                /* FAKEMATCH: needed to match: a one-pass block keeps the transform stores in order */
                do {
                    tp[2] = (hp[8] << 16) + top;
                } while (0);
                tp[3] = 0;
                Object_ApplyProjectedPlacementFar(obj, tp, pp, 0x4000);
            }
            i++;
            hp++;
            p += 4;
        } while (i < cnt);
    }
}

void UiMenu_PositionCursor(s32 x_offset, s32 y_offset)
{
    /* FAKEMATCH: the unsigned position/OAM view and member lifetimes preserve load/register order changed by earlier caching. */
    struct PsynergyMenuState *work = gMenuWork;

    ((struct MenuCursorSprite *)work->pane_icon[0])->attributes.x =
        ((struct MenuCursorSprite *)work->pane_icon[0])->x =
        UiMenu_CursorBobX[(gFrameCount >> 1) & 7] + x_offset
        + ((struct RenderInput *)work->auxiliary_window)->x * 8 + 8;
    ((struct MenuCursorSprite *)work->pane_icon[0])->attributes.y =
        ((struct MenuCursorSprite *)work->pane_icon[0])->y =
        UiMenu_CursorBobY[(gFrameCount >> 1) & 7] + y_offset
        + ((struct RenderInput *)work->auxiliary_window)->y * 8 + 8;
}

void UiMenu_SlideCursor(s32 x, s32 y)
{
    /* FAKEMATCH: the unsigned cursor/OAM view preserves scheduling changed by signed coordinates and packed-word casts. */
    struct PsynergyMenuState *work = gMenuWork;
    struct MenuCursorSprite *cursor;
    s32 steps;
    s32 start_x;
    s32 start_y;
    s32 px;
    s32 py;
    s32 dx;
    s32 dy;

    steps = 2;
    if (work->skip_slide != 0) {
        work->skip_slide = 0;
        return;
    }
    cursor = (struct MenuCursorSprite *)work->pane_icon[0];
    {
        s32 sprite_x = cursor->attributes.x + 64;
        s32 sprite_y = cursor->attributes.y + 64;

        cursor->x = sprite_x;
        cursor->y = sprite_y;
    }
    x += 64;
    y += 64;
    if (cursor->x - 8 > 0)
        cursor->x -= 8;
    py = cursor->y;
    if (py - 8 > 0) {
        cursor->y -= 8;
        py = cursor->y;
    }
    start_x = cursor->x << 4;
    dx = ((x << 4) - start_x + 1) / steps;
    start_y = py << 4;
    dy = ((y << 4) - start_y + 1) / steps;
    px = start_x;
    py = start_y;
    do {
        px += dx;
        cursor->attributes.x = cursor->x =
            (px >> 4) + (((struct RenderInput *)work->auxiliary_window)->x << 3) - 56;
        py += dy;
        cursor->attributes.y = cursor->y =
            (py >> 4) + (((struct RenderInput *)work->auxiliary_window)->y << 3) - 56;
        steps--;
        if (steps != 0)
            WaitFrames(1);
    } while (steps != 0);
}

void PsynergyMenu_ReservedNoOp(void)
{
}

void PsynergyMenu_RefreshOwnerEntriesDefault(void)
{
    PsynergyMenu_RefreshOwnerEntries(0x6C, 0x28, 8);
}

void PsynergyMenu_RefreshOwnerEntries(s32 origin_x, s32 origin_y, s32 columns)
{
    struct RenderOutput **slot;
    struct RenderOutput **scan;
    s32 index;

    index = 0;
    slot = ((struct PsynergyMenuState *)gMenuWork)->entry_icons;
    scan = slot;
    do {
        if (*scan++ != NULL) {
            PsynergyMenu_PositionOwnerEntry(slot, index, origin_x, origin_y, columns);
        }
        index++;
        slot++;
    } while (index <= 0x1F);
}

void PsynergyMenu_PositionOwnerEntry(struct RenderOutput **slot, s32 index,
    s32 origin_x, s32 origin_y,
    s32 columns) {
    struct RenderOutput *object;

    if (index > 0x1F) {
        index = 0;
    }
    object = *slot;
    object->y = (index / columns) * 0x10 + origin_y;
    object->x = (index % columns) * 0x10 + origin_x;
    UiIcon_PrepareObject(object);
}

#include "TYPES.H"
#include "CALLBACK_SCHEDULER.H"
#include "PSYNERGY_MENU.H"
#include "ANIMSPR.H"
#include "FIELD_SPRITE.H"

s32 UiWindow_UpdateOrCreate(s32 *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
s32 UiIcon_CreateWithResourceVariant(s32 arg0, s32 arg1, s32 arg2);


/* Byte 9 of the object holds a packed pair of two-bit fields; this routine
   clears the upper one, which is what produces the ~12 mask. Spelling it as a
   bitfield store rather than a hand-written mask/and is what emits the
   reference's `movs #13 / ldrb / negs / adds rN,rM,#0 / ands` shape: every
   hand-written mask local (s8, u8 or s32, split or inline, with or without a
   copy round trip) either loses the mask copy or turns the `ldrb` into
   `movs #9 / ldrsb`. */
/* The word slot at 0x154 must be written through a union view, not a plain
   `u32 *`/`s32 *` cast and not a single-member struct: the union's alias set
   keeps the slot store ordered against the object's byte-9 read-modify-write,
   where a scalar or struct view lets the mask materialisation float one slot
   ahead of the store. Measured: union 0, struct 4, `u32 *` 4. */
union EntrySlot {
    s32 w;
    u16 h[2];
    void *p;
};

#define ENTRY_SLOT(base, offset) ((union EntrySlot *)((u8 *)(base) + (offset)))
s32 Party_ListActiveOwnersFar(u16 *out);
s32 Party_RemapCharacterIdByFlagsFar(u16 value);
void *ResourceObject_CreateFar(s32 value);
s32 AnimationObjects_SelectAnimationFar(struct AnimationObject *object, s32 value);
void Menu_UpdateEntryObjectTransforms(void);

void ResourceObject_ReleaseFar(void *);
s32 Party_CountActiveOwnersFar(void);
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

void WaitFrames(s32 frames);

extern u8 UiMenu_CursorBobX[];
extern u8 UiMenu_CursorBobY[];
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
    object = (struct RenderOutput *)UiIcon_CreateWithResourceVariant(handle, -8, 11);
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
        Scheduler_AddOrUpdateCallback((s32)(Menu_UpdateEntryObjectTransforms), delay_frames);
    }
}

void Menu_ReleaseEntryObjects(void)
{
    u32 buf[7];
    struct PsynergyMenuState *base = gMenuWork;
    s32 count;
    void **p;
    s32 i;

    count = (u16)Party_ListActiveOwnersFar(buf);
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
    Scheduler_RemoveCallback((u32)((void (*)(void))Menu_UpdateEntryObjectTransforms));
}

void Menu_UpdateEntryObjectTransforms(void)
{
    struct PsynergyMenuState *work;
    struct AnimationObject **objects;
    s32 pos[2];
    s32 trans[4];
    s32 *pp;
    s32 *tp;
    s16 *hp;
    s32 i;
    s32 cnt;

    work = gMenuWork;
    cnt = (u16)Party_CountActiveOwnersFar();
    i = 0;
    if (i < cnt) {
        pp = pos;
        tp = trans;
        hp = (s16 *)work->tab_x;
        objects = work->tab_objects;
        do {
            struct AnimationObject *obj;
            s32 top;

            top = 0x01e20000 - (hp[8] << 16);
            obj = *objects;
            if (obj != 0) {
                ((struct FieldSprite *)obj)->priority = 0;
                pos[0] = work->owner_scale[i];
                pp[1] = work->owner_scale[i];
                tp[1] = top;
                tp[0] = hp[0] << 16;
                tp[2] = (hp[8] << 16) + top;
                tp[3] = 0;
                Object_ApplyProjectedPlacementFar(obj, (s32 *)tp, pp, 0x4000);
            }
            i++;
            hp++;
            objects++;
        } while (i < cnt);
    }
}

/* Places the menu cursor at the given pixel offset from the top-left tile of
 * the menu window, plus eight pixels and a small diagonal bob that follows
 * the frame counter. */
void UiMenu_PositionCursor(s32 x_offset, s32 y_offset)
{
    struct PsynergyMenuState *work = gMenuWork;

    ((struct MenuCursorAttributes *)&work->pane_icon[0]->packed)->x = *(u16 *)&work->pane_icon[0]->x =
        UiMenu_CursorBobX[(gFrameCount >> 1) & 7] + x_offset
        + ((struct RenderInput *)work->auxiliary_window)->x * 8 + 8;
    ((struct MenuCursorAttributes *)&work->pane_icon[0]->packed)->y = *(u16 *)&work->pane_icon[0]->y =
        UiMenu_CursorBobY[(gFrameCount >> 1) & 7] + y_offset
        + ((struct RenderInput *)work->auxiliary_window)->y * 8 + 8;
}

/* Slides the menu cursor to the given pixel offset in two steps, a frame
 * apart, from where its sprite stands, pulled eight pixels back on each
 * axis. A set skip flag is cleared instead and the cursor stays. */
void UiMenu_SlideCursor(s32 x, s32 y)
{
    struct PsynergyMenuState *work = gMenuWork;
    struct RenderOutput *cursor;
    s32 steps;
    s32 start_x;
    s32 start_y;
    s32 px;
    s32 py;
    s32 dx;
    s32 dy;

    steps = 2;
    if (work->flags != 0) {
        work->flags = 0;
        return;
    }
    cursor = work->pane_icon[0];
    {
        s32 sprite_x = ((struct MenuCursorAttributes *)&cursor->packed)->x + 64;
        s32 sprite_y = ((struct MenuCursorAttributes *)&cursor->packed)->y + 64;

        *(u16 *)&cursor->x = sprite_x;
        *(u16 *)&cursor->y = sprite_y;
    }
    x += 64;
    y += 64;
    if ((u16)cursor->x - 8 > 0)
        cursor->x -= 8;
    py = (u16)cursor->y;
    if (py - 8 > 0) {
        cursor->y -= 8;
        py = (u16)cursor->y;
    }
    start_x = (u16)cursor->x << 4;
    dx = ((x << 4) - start_x + 1) / steps;
    start_y = py << 4;
    dy = ((y << 4) - start_y + 1) / steps;
    px = start_x;
    py = start_y;
    do {
        px += dx;
        ((struct MenuCursorAttributes *)&cursor->packed)->x = *(u16 *)&cursor->x =
            (px >> 4) + (((struct RenderInput *)work->auxiliary_window)->x << 3) - 56;
        py += dy;
        ((struct MenuCursorAttributes *)&cursor->packed)->y = *(u16 *)&cursor->y =
            (py >> 4) + (((struct RenderInput *)work->auxiliary_window)->y << 3) - 56;
        steps--;
        if (steps != 0)
            WaitFrames(1);
    } while (steps != 0);
}

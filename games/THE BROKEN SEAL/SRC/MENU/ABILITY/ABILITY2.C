#include "TYPES.H"
#include "CALLBACK_SCHEDULER.H"
#include "PSYNERGY_MENU.H"
#include "ANIMSPR.H"
#include "FIELD_SPRITE.H"

s32 UiWindow_UpdateOrCreate(s32 *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
s32 UiIcon_CreateWithResourceVariant(struct RenderInput *window, s32 x, s32 y);


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

struct MenuCursorSprite {
    u8 reserved_00[6];
    u16 x;
    u16 y;
    u8 reserved_0a[10];
    struct MenuCursorAttributes attributes;
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
    /* FAKEMATCH: retain the existing volatile transform writes, signed sprite-byte update and byte cursor. Typed arrays/priority fields change the native loop registers and write order. */
    u8 *p;
    s32 pos[2];
    s32 trans[4];
    s32 *pp;
    volatile s32 *tp;
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
                tp[2] = (hp[8] << 16) + top;
                tp[3] = 0;
                Object_ApplyProjectedPlacementFar(obj, (s32 *)tp, pp, 0x4000);
            }
            i++;
            hp++;
            p += 4;
        } while (i < cnt);
    }
}

/* Places the menu cursor at the given pixel offset from the top-left tile of
 * the menu window, plus eight pixels and a small diagonal bob that follows
 * the frame counter. */
void UiMenu_PositionCursor(s32 x_offset, s32 y_offset)
{
    /* FAKEMATCH: keep the existing unsigned-position/OAM sprite view; direct RenderOutput member access removes the native halfword narrowing and changes load/store order. */
    struct PsynergyMenuState *work = gMenuWork;
    struct RenderInput *window = (struct RenderInput *)work->auxiliary_window;
    struct MenuCursorSprite *cursor = (struct MenuCursorSprite *)work->pane_icon[0];

    cursor->attributes.x = cursor->x =
        UiMenu_CursorBobX[(gFrameCount >> 1) & 7] + x_offset
        + window->x * 8 + 8;
    cursor->attributes.y = cursor->y =
        UiMenu_CursorBobY[(gFrameCount >> 1) & 7] + y_offset
        + window->y * 8 + 8;
}

/* Slides the menu cursor to the given pixel offset in two steps, a frame
 * apart, from where its sprite stands, pulled eight pixels back on each
 * axis. A set skip flag is cleared instead and the cursor stays. */
void UiMenu_SlideCursor(s32 x, s32 y)
{
    /* FAKEMATCH: keep the existing unsigned cursor/OAM sprite view; canonical signed RenderOutput coordinates and packed-word casts change native scheduling. The skip flag is the original halfword at 0x222. */
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
            (px >> 4) + (work->window->x << 3) - 56;
        py += dy;
        cursor->attributes.y = cursor->y =
            (py >> 4) + (work->window->y << 3) - 56;
        steps--;
        if (steps != 0)
            WaitFrames(1);
    } while (steps != 0);
}

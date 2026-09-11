#include "types.h"
#include "scene.h"

/* menu/psynergy_menu/call_icon_routine_with_value.c */
void PsynergyMenu_CallIconRoutineWithValue(s32 arg0, s32 arg1)
{
    Sys_Place(0, arg1, 0);
}

/* ui/menu/create_cursor.c */
s32 UiIcon_CreateWithResourceVariant(s32 arg0, s32 arg1, s32 arg2);

s32 UiMenu_CreateCursor(void *work)
{
    s32 handle;
    s32 zero = 0;
    s32 state;
    void *object;

    FIELD_AT_OFFSET(work, s32 *, 0x10) = zero;
    Menu_SetRect((s32 *)((u8 *)work + 0x10), zero, zero, 13, 5, 2);
    handle = FIELD_AT_OFFSET(work, s32 *, 0x10);
    object = (void *)UiIcon_CreateWithResourceVariant(handle, -8, 11);
    FIELD_AT_OFFSET(object, u8 *, 5) = 13;
    FIELD_AT_OFFSET(work, u8 *, 0x1C) = 255;
    FIELD_AT_OFFSET(work, u8 *, 0x1D) = zero;
    FIELD_AT_OFFSET(work, void **, 0x14) = object;
    state = 254;
    FIELD_AT_OFFSET(object, u8 *, 0x0F) = state;
    state -= 255;
    FIELD_AT_OFFSET(FIELD_AT_OFFSET(work, void **, 0x18), s8 *, 0x0F) = state;
    return handle;
}

/* menu/psynergy_menu/initialize_entry_objects.c */
#define FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

/* Byte 9 of the object holds a packed pair of two-bit fields; this routine
   clears the upper one, which is what produces the ~12 mask. Spelling it as a
   bitfield store rather than a hand-written mask/and is what emits the
   reference's `movs #13 / ldrb / negs / adds rN,rM,#0 / ands` shape: every
   hand-written mask local (s8, u8 or s32, split or inline, with or without a
   copy round trip) either loses the mask copy or turns the `ldrb` into
   `movs #9 / ldrsb`. */
struct EntryObject {
    u8 pad00[9];
    u8 f09_a : 2;
    u8 f09_b : 2;
    u8 f09_c : 4;
    u8 pad0a[28];
    u8 f26;
};

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

extern u8 *gIw;

void *Sys_Run(s32 value);
void Object_InitializeMode(void *object, s32 value);
void ScheduleCallbackAfterFrames(void (*callback)(void), s32 value);

void InitializeEntryObjects(void *source, s32 origin_x, s32 origin_y, s32 spacing)
{
    u16 entry_ids[14];
    u8 *entry_state = gIw;
    s32 entry_count = (u16)Sys_Check(entry_ids);
    s32 i;

    entry_state[0x1e] = entry_count;
    for (i = 0; i < entry_count; i++) {
        void *entry_object = Sys_Run(Sys_unk2(entry_ids[i]));
        if (entry_object != 0) {
            s32 entry_x;
            s32 source_x;
            s32 position_x;

            FIELD(entry_state, void **, 0x114 + i * 4) = entry_object;
            source_x = FIELD(source, u16 *, 0xc);
            entry_x = spacing + 16;
            entry_x *= i;
            position_x = origin_x + source_x;
            FIELD(entry_state, u16 *, 0x134 + i * 2) = position_x * 8 + entry_x;
            FIELD(entry_state, u16 *, 0x144 + i * 2) =
                (origin_y + FIELD(source, u16 *, 0xe)) * 8 + 16;
            ENTRY_SLOT(entry_state, 0x154 + i * 4)->w = 0x10000;
            ((struct EntryObject *)entry_object)->f09_b = 0;
            FIELD(entry_object, u8 *, 38) = 0;
            Object_InitializeMode(entry_object, 1);
        }
    }
    for (; i < 8; i++) {
        FIELD(entry_state, void **, 0x114 + i * 4) = 0;
    }
    {
        s32 delay_frames = 200;
        delay_frames <<= 4;
        ScheduleCallbackAfterFrames(Menu_UpdateEntryObjectTransforms, delay_frames);
    }
}

/* menu/entry/release_entry_objects.c */
/* menu/entry/release_entry_objects.c */
extern u8 *gIw;

void ScheduleCallback(void (*callback)(void));
extern u8 gRom;

void Menu_ReleaseEntryObjects(void)
{
    u32 buf[7];
    u8 *base = gIw;
    s32 count;
    void **p;
    s32 i;

    count = (u16)Menu_Check(buf);
    if (count != 0) {
        p = (void **)(base + 276);
        i = count;
        do {
            void *entry = *p++;

            if (entry != 0) {
                Menu_Do(entry);
            }
        } while (--i != 0);
    }
    ScheduleCallback((void (*)(void))&gRom);
}

/* menu/entry/update_entry_object_transforms.c */
extern u8 *gIw;

void Menu_UpdateEntryObjectTransforms(void)
{
    u8 *p;
    s32 pos[2];
    s32 trans[4];
    s32 *pp;
    volatile s32 *tp;
    s16 *hp;
    s32 i;
    s32 cnt;

    p = gIw;
    cnt = (u16)Menu_Check();
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
                Menu_SetMode(obj, (s32 *)tp, pp, 0x4000);
            }
            i++;
            hp++;
            p += 4;
        } while (i < cnt);
    }
}

#include "types.h"

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

extern u8 *Data_03001f2c;

s32 Func_08077158(u16 *out);
s32 Func_0808a288(u16 value);
void *Func_08009030(s32 value);
void Func_08009020(void *object, s32 value);
void ScheduleCallbackAfterFrames(void (*callback)(void), s32 value);
void Func_080a19a0(void);

#define InitializeEntryObjects Func_080a1870
void Func_080a1870(void *source, s32 origin_x, s32 origin_y, s32 spacing)
{
    u16 entry_ids[14];
    u8 *entry_state = Data_03001f2c;
    s32 entry_count = (u16)Func_08077158(entry_ids);
    s32 i;

    entry_state[0x1e] = entry_count;
    for (i = 0; i < entry_count; i++) {
        void *entry_object = Func_08009030(Func_0808a288(entry_ids[i]));
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
            Func_08009020(entry_object, 1);
        }
    }
    for (; i < 8; i++) {
        FIELD(entry_state, void **, 0x114 + i * 4) = 0;
    }
    {
        s32 delay_frames = 200;
        delay_frames <<= 4;
        ScheduleCallbackAfterFrames(Func_080a19a0, delay_frames);
    }
}

#include "types.h"

#define FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

/* Byte 9 of the object holds a packed pair of two-bit fields; this routine
   clears the upper one, which is what produces the ~12 mask. Spelling it as a
   bitfield store rather than a hand-written mask/and is what emits the
   reference's `movs #13 / ldrb / negs / adds rN,rM,#0 / ands` shape: every
   hand-written mask local (s8, u8 or s32, split or inline, with or without a
   copy round trip) either loses the mask copy or turns the `ldrb` into
   `movs #9 / ldrsb`. */
struct Obj {
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
union Slot {
    s32 w;
    u16 h[2];
    void *p;
};

#define SLOT(base, offset) ((union Slot *)((u8 *)(base) + (offset)))

extern u8 *Data_03001f2c;

s32 Func_08077158(u16 *out);
s32 Func_0808a288(u16 value);
void *Func_08009030(s32 value);
void Func_08009020(void *object, s32 value);
void Func_080041d8(void (*callback)(void), s32 value);
void Func_080a19a0(void);

void Func_080a1870(void *arg0, s32 arg1, s32 arg2, s32 arg3)
{
    u16 values[14];
    u8 *state = Data_03001f2c;
    s32 count = (u16)Func_08077158(values);
    s32 i;

    state[0x1e] = count;
    for (i = 0; i < count; i++) {
        void *object = Func_08009030(Func_0808a288(values[i]));
        if (object != 0) {
            s32 x;
            s32 base;
            s32 sum;

            FIELD(state, void **, 0x114 + i * 4) = object;
            base = FIELD(arg0, u16 *, 0xc);
            x = arg3;
            x += 16;
            x *= i;
            sum = arg1 + base;
            FIELD(state, u16 *, 0x134 + i * 2) = sum * 8 + x;
            FIELD(state, u16 *, 0x144 + i * 2) =
                (arg2 + FIELD(arg0, u16 *, 0xe)) * 8 + 16;
            SLOT(state, 0x154 + i * 4)->w = 0x10000;
            ((struct Obj *)object)->f09_b = 0;
            FIELD(object, u8 *, 38) = 0;
            Func_08009020(object, 1);
        }
    }
    for (; i < 8; i++) {
        FIELD(state, void **, 0x114 + i * 4) = 0;
    }
    {
        s32 delay = 200;
        delay <<= 4;
        Func_080041d8(Func_080a19a0, delay);
    }
}

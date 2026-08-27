#include "types.h"
#include "global_cells.h"

typedef s32 (*WordCopyFn)(void *dest, const void *src, s32 count);

void *Func_08077008(s32 unit_id);
void *Func_08077018(s32 item_id);
s32 Func_08077028(s32 unit_id, s32 item_id);
void Func_080a112c(s32 target, s32 actor, s32 slot, s32 flags);
s32 Func_080a40ac(s32 unit_id);
void *Func_08004938(s32 size);
void Func_08002df0(void *buffer);

void Func_080a3ef0(s32 holder, s32 slot, s32 mode, s32 actor)
{
    void *state;
    s32 item_id;
    s32 flags = 0;
    u8 type;

    state = *(void **)ADDR_03001F2C;
    item_id = *(u16 *)((u8 *)Func_08077008(holder) + 216 + slot * 2);
    if (mode == 1) {
        flags = 0x100;
    }

    type = *((u8 *)Func_08077018(item_id & 0x1ff) + 2);

    switch (type) {
    case 0:
        Func_080a112c(*(s32 *)((u8 *)state + 36), actor, slot, flags);
        break;

    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 7:
    case 8:
    case 9:
        if (holder == actor) {
            flags |= 2;
            Func_080a112c(*(s32 *)((u8 *)state + 36), actor, slot, flags);
        } else {
            void *other_window;
            void *buf;
            s32 check;

            other_window = Func_08077008(actor);
            buf = Func_08004938(0x14c);
            ((WordCopyFn)0x03001388)(buf, other_window, 0x14c);
            check = Func_080a40ac(actor);
            if (check == 0) {
                Func_080a112c(*(s32 *)((u8 *)state + 36), actor, slot, flags);
            } else {
                s32 result;

                item_id &= 0xfffffdff;
                result = Func_08077028(actor, item_id);
                if (result == -1) {
                    Func_080a112c(*(s32 *)((u8 *)state + 36), actor, slot, flags);
                } else {
                    flags |= 2;
                    Func_080a112c(*(s32 *)((u8 *)state + 36), actor, result, flags);
                }
            }
            ((WordCopyFn)0x03001388)(other_window, buf, 0x14c);
            Func_08002df0(buf);
        }
        break;

    case 6:
        if (actor == holder) {
            flags |= 4;
            Func_080a112c(*(s32 *)((u8 *)state + 36), actor, slot, flags);
        } else {
            void *other_window;
            void *buf;
            s32 check;

            other_window = Func_08077008(actor);
            buf = Func_08004938(0x14c);
            ((WordCopyFn)0x03001388)(buf, other_window, 0x14c);
            check = Func_080a40ac(actor);
            if (check == 0) {
                Func_080a112c(*(s32 *)((u8 *)state + 36), actor, slot, flags);
            } else {
                s32 result;

                result = Func_08077028(actor, item_id);
                if (result == -1) {
                    Func_080a112c(*(s32 *)((u8 *)state + 36), actor, slot, flags);
                } else {
                    flags |= 4;
                    Func_080a112c(*(s32 *)((u8 *)state + 36), actor, result, flags);
                }
            }
            ((WordCopyFn)0x03001388)(other_window, buf, 0x14c);
            Func_08002df0(buf);
        }
        break;
    }
}

#include "types.h"

#define FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

typedef struct ActionResource {
    u8 unknown00[4];
    u16 flags;
} ActionResource;

void Func_08015040(s32, s32);
void Func_08015120(s32, s32);
void Func_08015140(void);
u8 *Func_08077008(s32);
u8 *Func_08077080(s32);
s32 Func_080770c0(s32);
void Func_080770c8(s32);
void Func_080770d0(s32);
void Func_08077120(s32, s32);
void Func_0808b8e8(void);
void Func_0808b98c(void);
void Func_0808ba1c(s32);
u32 Func_0808df1c(u32, u8);
ActionResource *Func_0808e4b4(u32, u16, s32 *);
s32 Func_0808e5d8(u32);
void Func_08091660(void);
s32 Func_08091d84(s32);
void Func_08096810(void);
void Func_08096960(void);
void Func_08096ab0(void);
void Func_08096b28(ActionResource *, s32, s32);
void Func_08096fb0(s32, s32);
void Func_080970f8(u32, s32);
void Func_08097174(void);
void Func_08097194(void);
void Func_0809728c(void);

/*
 * Resolve and present one packed action request.  The low ten bits select the
 * action, bits 10..13 select its actor, and bit 13 selects the special handler.
 */
s32 Func_0808e680(u32 packed_action)
{
    u8 *runtime = *(u8 **)0x03001ebc;
    s32 action = packed_action & 0x3ff;
    s32 actor = (packed_action >> 10) & 0xf;
    u8 action_type;
    s32 selected_object = 0;
    s32 restored_special_state = 0;
    ActionResource *primary;
    ActionResource *middle;
    ActionResource *secondary;

    action_type = FIELD(Func_08077080(action), u8, 0x0c);
    Func_0808ba1c(*(s32 *)0x02000434);
    Func_08091660();
    Func_080770d0(0x145);

    if (actor == 0xf)
        actor = 0;

    if (Func_080770c0(0x17e) != 0) {
        Func_08015120(actor, 1);
        Func_08015120(action, 4);
        Func_08015040(0x91f, 1);
        return 0;
    }

    if (FIELD(runtime, s16, 0x19e) == 3 && action == 0x90) {
        Func_08015120(actor, 1);
        Func_08015120(0x90, 4);
        Func_08015040(0x91f, 1);
        return 0;
    }

    if (action == 0x95) {
        if (Func_080770c0(0x144) != 0) {
            Func_08015120(actor, 1);
            Func_08015120(0x95, 4);
            Func_08015040(0x921, 1);
            return 0;
        }

        Func_08015120(0x95, 4);
        Func_08015040(0x920, 0x0d);
        restored_special_state = Func_08091d84(1);
        Func_08015140();
        if (restored_special_state != 0)
            return 0;

        *(u16 *)0x02000400 = *(u16 *)0x02000480;
        *(u16 *)0x02000402 = *(u16 *)0x02000482;
        FIELD(runtime, s16, 0x170) = 0x3e7;
        restored_special_state = 1;
    }

    if ((packed_action & 0x2000) != 0)
        return Func_0808e5d8(packed_action);

    if (actor <= 7) {
        u8 cost = FIELD(Func_08077080(action), u8, 9);

        if (FIELD(Func_08077008(actor), s16, 0x3a) < cost) {
            Func_08015120(actor, 1);
            Func_08015120(action, 4);
            Func_08015040(0x91e, 1);
            if (restored_special_state != 0)
                FIELD(runtime, s16, 0x170) = 0;
            return 0;
        }
        Func_08077120(actor, -(s32)cost);
    }

    primary = Func_0808e4b4(0x10000005, action_type, &selected_object);
    middle = Func_0808e4b4(5, action_type, &selected_object);
    secondary = Func_0808e4b4(0x50000005, action_type, &selected_object);

    selected_object = -1;
    Func_080770c8(0x140);
    Func_080770c8(0x141);
    if (primary != 0 || middle != 0 || secondary != 0) {
        selected_object = (s32)Func_0808df1c(*(u32 *)0x02000434, action_type);
        if (middle != 0 && (middle->flags & 0x0400) != 0) {
            Func_080770d0(0x140);
            Func_080770d0(0x141);
        }
    } else {
        Func_080770d0(0x141);
    }

    if (FIELD(runtime, s16, 0x19e) == 3)
        Func_0808b8e8();

    Func_08096fb0(action, 0);
    FIELD(runtime, u8, 0x0cc6) = 1;
    Func_080970f8(*(u32 *)0x02000434, selected_object);
    Func_0809728c();

    Func_08096b28(primary, actor, selected_object);
    if (Func_080770c0(0x140) != 0) {
        if (Func_080770c0(0x141) != 0)
            Func_08096960();
        else
            Func_08096810();
    }
    Func_08097174();

    Func_08096b28(middle, actor, selected_object);
    if (Func_080770c0(0x140) != 0)
        Func_08096ab0();

    Func_080770d0(0x140);
    Func_080770d0(0x141);
    FIELD(runtime, u8, 0x0cc6) = 0;
    Func_08097194();

    if (FIELD(runtime, s16, 0x19e) == 3)
        Func_0808b98c();

    return 0;
}

/*
 * Correctness fix, veneer audit (mars, 2026-08-01).
 *
 * 0x080072e4 begins the GCC `__call_via_rN` veneer bank -- fifteen four-byte
 * `bx rN; nop` entries, r0..lr, ending at 0x08007320.  A `bl` into that range
 * is an indirect call through the named register.
 *
 * Three sites here, all __call_via_r0, and all THREE DISPATCH DIFFERENT
 * CALLEES: the values at 0x02008024 (site 0x0808c5be), 0x0200801c
 * (0x0808c5cc) and 0x02008004 (0x0808c6cc).  One phantom prototype was
 * standing in for three distinct function pointers.
 *
 * __call_via_r0 means the callee occupies r0, so each call takes NO
 * arguments -- the draft's single argument WAS the callee, appearing in the
 * slot matching the veneer's register index.  That regularity holds at every
 * site in this batch and is checked per site, not assumed.
 *
 * The three globals are adjacent members of one 0x0200800x block, so a shared
 * typedef is used.  What any of them does is not established and none is
 * named.
 */
#include "types.h"

typedef s32 (*Callee_0808c4f8)(void);

#define S16(base, off) (*(volatile s16 *)((u8 *)(base) + (off)))
#define U16(base, off) (*(volatile u16 *)((u8 *)(base) + (off)))
#define U8(base, off)  (*(volatile u8 *)((u8 *)(base) + (off)))
#define S32(base, off) (*(volatile s32 *)((u8 *)(base) + (off)))
#define PTR(base, off) (*(void * volatile *)((u8 *)(base) + (off)))

/*
 * This owner is the map field's main event loop.  The workspace returned by
 * Func_080048f4 holds the active actor table, pending field actions, transition
 * request, and the two values used by the display fade.
 */
extern void *Func_080048f4(s32, s32);
extern void *Func_08077008(s32);
extern s32 Func_080022ec(s32, s32);
extern s32 Func_080770c0(s32);
extern s32 Func_0808bc9c(void);
extern s32 Func_0808ce74(void);
extern s32 Func_0808d5a4(s32);
extern s32 Func_0808ddec(s32);
extern s32 Func_080919d8(u16);
extern s32 Func_08094428(void);
extern s16 Func_080f9070(void);

extern void Func_08002dd8(s32);
extern void Func_080030f8(u32);
extern void Func_080040e8(void);
extern void Func_080072e4_void(s32);
extern void Func_08009078(s16);
extern void Func_08009080(void *, s32);
extern void Func_080090a0(void *);
extern void Func_080090a8(void *);
extern void Func_080090b0(void *);
extern void Func_080090b8(void *);
extern void Func_08009110(void);
extern void Func_08009118(void);
extern void Func_08009128(void);
extern void Func_08009130(void);
extern void Func_08009138(s32, s32);
extern void Func_08009140(void *);
extern void Func_08015000(void);
extern void Func_08015040(const void *, s32);
extern void Func_08015120(s32, s32);
extern void Func_080151e8(void);
extern void Func_08015200(s16);
extern void Func_08015208(void);
extern void Func_08015288(void);
extern void Func_08015290(void);
extern void Func_08015370(s32);
extern void Func_080770c8(s32);
extern void Func_080770d0(s32);
extern void Func_080772f0(void);
extern void Func_0808ace0(s32);
extern void Func_0808b1d8(void);
extern void Func_0808b674(s32);
extern void Func_0808ba38(void);
extern void Func_0808bb2c(void);
extern void Func_0808bc44(void);
extern void Func_0808bec0(u8, s32, s32, s32);
extern void Func_0808c44c(void);
extern void Func_0808c4c0(void);
extern void Func_0808cf78(void);
extern void Func_0808d0c8(void);
extern void Func_0808d5dc(s32);
extern void Func_0808d7d8(s32);
extern void Func_0808d828(s32);
extern void Func_0808d8f0(void);
extern void Func_0808d9a4(s16);
extern void Func_0808e118(void);
extern void Func_0808e23c(s16, s16);
extern void Func_0808e680(s16);
extern void Func_0808e9c0(void);
extern void Func_0808fefc(s32, s32);
extern void Func_080901c0(s32, s32);
extern void Func_08091174(void);
extern void Func_08091200(s32, s32);
extern void Func_08091660(void);
extern void Func_08091a58(u16, s32);
extern void Func_080941e0(s16);
extern void Func_08095680(void);
extern void Func_08095778(s16, s32);
extern void Func_08099810(void);
extern void Func_0809c138(s32);
extern void Func_0809c3a4(void);
extern void Func_080f9010(s32);

static void close_field_action(void)
{
    Func_0808c4c0();
}

s32 Func_0808c4f8(void)
{
    volatile u8 *const state = (volatile u8 *)0x02000400;
    volatile s32 *const current_actor = (volatile s32 *)0x02000434;
    volatile u8 *const startup = (volatile u8 *)0x03001f54;
    volatile s32 *const input = (volatile s32 *)0x03001ae8;
    volatile u16 *const palette = (volatile u16 *)0x05000000;
    u8 *work = Func_080048f4(27, 0xccc);
    s32 zero = 0;
    s16 result;
    s16 mode;
    void *actor;

    *(volatile const void **)0x040000d4 = &zero;
    *(void * volatile *)0x040000d8 = work;
    *(volatile u32 *)0x040000dc = 0x85000333;
    Func_080770d0(0x103);
    U16(state, 8) = U16(state, 0);
    U16(state, 10) = U16(state, 2);
    S16(state, 14) = -1;
    S16(state, 16) = -1;
    S16(state, 18) = -1;
    S16(state, 20) = -1;
    Func_080040e8();
    Func_08095778(0, 0);

    if (S16(state, 0x1a) == 1) {
        Func_08009118();
        U8(state, 0x64) = 1;
        mode = 3;
        Func_0808ace0(0);
    } else {
        Func_08009110();
        mode = 2;
        Func_0808ace0(1);
    }
    S16(work, 0x19e) = mode;
    Func_08009078(mode);
    Func_08015000();
    Func_0808bc44();
    S32(work, 0x10) = ((Callee_0808c4f8)*(volatile s32 *)0x02008024)();
    Func_0808cf78();
    Func_0808b674(((Callee_0808c4f8)*(volatile s32 *)0x0200801c)());
    if (Func_080770c0(0x109))
        Func_0808bb2c();
    if (S16(state, 0x74))
        Func_08095680();
    if (mode == 3) {
        Func_08009130();
        Func_08009138(S32(state, 0x1c), S32(state, 0x24));
    } else {
        Func_08009128();
    }
    Func_0808e9c0();
    Func_08091174();
    Func_08091200(0x10000, 0);

    S32(work, 0x1c0) = 0x100;
    S32(work, 0x1c8) = 0x10;
    U16(work, 0x1c6) = 0;
    S32(work, 0x1b0) = 0x199;
    S32(work, 0x1b4) = 0;
    if (S16(state, 0x7e) == 2) {
        S32(work, 0x1b0) = 0;
        Func_080770c8(0x144);
    }
    S32(work, 0x1bc) = 0x02010000;
    if (!Func_080770c0(0x109)) {
        Func_0808b1d8();
        S16(state, 0x6c) = 0x10;
        S16(state, 0x6e) = 0;
        S16(state, 0x70) = 1;
        S16(state, 0x8a) = -1;
        S16(state, 0x8c) = 0;
    }
    Func_08099810();
    S16(work, 0xcc8) = -1;
    (void)((Callee_0808c4f8)*(volatile s32 *)0x02008004)();

    result = S16(work, 0x170);
    if (result != 0) {
        S16(work, 0x170) = 0;
        goto finish;
    }

    Func_080770d0(0x109);
    if (!Func_0808bc9c()) {
        if (U16(work, 0x1c6) == 0) {
            Func_0808fefc(S32(work, 0x1c0), S32(work, 0x1c8));
            U16(work, 0x1c6) = 1;
            *palette = 0;
            if (!Func_08094428()) {
                s32 fade = S32(work, 0x1c8) + 1;
                Func_080030f8((u32)((fade + ((u32)fade >> 31)) >> 1));
            }
        }
        if (Func_080770c0(0x12f)) {
            Func_080770d0(0x12f);
            Func_08015200(S16(state, 0));
        }
        if (S16(state, 0x74)) {
            Func_08095778(S16(state, 0x74), 1);
            S16(state, 0x74) = 0;
        }
        if (U16(state, 0x7c)) {
            Func_0808c44c();
            if (!Func_080919d8(U16(state, 0x7c)))
                Func_08091a58(U16(state, 0x7c), 0);
            close_field_action();
            U16(state, 0x7c) = 0;
        }
    }

reload_actor:
    Func_080770c8(0x104);
    actor = PTR(work, 0x14 + *current_actor * 4);
    Func_08009140(actor);
    S32(state, 0x1c) = S32(actor, 8);
    S32(state, 0x20) = 0;
    S32(state, 0x24) = S32(actor, 0x10);
    S32(state, 0x28) = U16(actor, 6);
    S16(state, 0x2c) = U8(actor, 0x22);

event_loop:
    if (!Func_0808bc9c()) {
        Func_080770d0(0x104);
        actor = PTR(work, 0x14 + *current_actor * 4);
        if (actor != 0) {
            if (U8(state, 0x32) == 2)
                Func_080090b8(actor);
            else if (U8(state, 0x32) == 1)
                Func_080090b0(actor);
            else if (S16(work, 0x19e) == 3)
                Func_080090a8(actor);
            else
                Func_080090a0(actor);
        }
        do {
            Func_080030f8(1);
            actor = PTR(work, 0x14 + *current_actor * 4);
            if (*startup == 0 || !Func_080770c0(0x163))
                Func_0808bec0(U8(actor, 0x22), S32(actor, 8),
                              S32(actor, 0xc), S32(actor, 0x10));
        } while (!Func_0808bc9c());
        goto reload_actor;
    }

    if (S16(work, 0x170)) {
        result = S16(work, 0x170);
        S16(work, 0x170) = 0;
        goto finish;
    }

    if (S16(work, 0x182)) {
        s16 command = S16(work, 0x182);
        S16(work, 0xcb6) = 1;
        if (command == -1) {
            u32 i;
            Func_08091660();
            Func_0808c44c();
            for (i = 0; i < (u16)S16(work, 0x184); i++) {
                Func_08009080(actor, 22);
                Func_08015120(S16(work, 0x188 + i * 2), 1);
                Func_08015040((const void *)0x91a, 1);
            }
            if (S16(work, 0x186)) {
                close_field_action();
                goto clear_command;
            }
            Func_08009080(actor, *current_actor == 0
                ? (Func_080770c0(0x20) ? 21 : 37) : 19);
            Func_080f9010(59);
            Func_08015040((const void *)0x91b, 1);
            actor = Func_08077008(*current_actor);
            S16(actor, 0x38) = 1;
            {
                s32 value = Func_080022ec(0x4000, S16(actor, 0x34));
                if (value > 0x4000) value = 0x4000;
                else if (value < 0) value = 0;
                S16(actor, 0x14) = value;
                if (value == 0 && S16(actor, 0x38) != 0) S16(actor, 0x14) = 1;
                value = Func_080022ec(S16(actor, 0x3a) << 14, S16(actor, 0x36));
                if (value > 0x4000) value = 0x4000;
                else if (value < 0) value = 0;
                S16(actor, 0x16) = value;
                if (value == 0 && S16(actor, 0x3a) != 0) S16(actor, 0x16) = 1;
            }
            U16(state, 0) = U16(state, 4);
            U16(state, 2) = U16(state, 6);
            close_field_action();
            result = 0x3e7;
            goto finish;
        } else if (command == -888) {
            Func_08091660();
            Func_0809c138(27);
        } else if (command == -889) {
            Func_08091660();
            Func_0809c3a4();
        } else {
            Func_0808d7d8(command);
        }
clear_command:
        S16(work, 0xcb6) = 0;
        S16(work, 0x182) = 0;
        goto event_loop;
    }

    if (S16(work, 0x17c)) {
        Func_08091660();
        Func_0808ba38();
        S16(state, 0x5e) = -1;
        U16(state, 0) = 0x1fe;
        U16(state, 2) = U16(work, 0x17c);
        S16(work, 0x170) = 0x3e7;
        Func_080941e0(S16(work, 0x17c));
        S32(work, 0x1a8) = 0;
        S16(work, 0x17c) = 0;
        *(volatile s32 *)0x02000478 = 0;
        goto event_loop;
    }
    if (S16(work, 0x16c)) {
        S16(work, 0xcb6) = 1;
        Func_0808d828(S16(work, 0x16c));
        S16(work, 0xcb6) = 0;
        S16(work, 0x16c) = 0;
        goto event_loop;
    }
    if (S16(work, 0x16e)) {
        Func_0808d8f0();
        S16(work, 0x16e) = 0;
        goto event_loop;
    }
    if (S16(work, 0x174)) {
        s32 selected;
        Func_08015208();
        selected = Func_0808ddec(*current_actor);
        if (selected != -1 && Func_0808d5a4(selected)) {
            S16(work, 0x178) = selected | 0x1000;
            S16(work, 0x172) = 0;
        } else {
            s16 alternate = Func_0808ce74();
            if (alternate) {
                S16(work, 0x17a) = alternate;
                S16(work, 0x172) = 0;
            } else {
                S16(work, 0x172) = 1;
            }
        }
        S16(work, 0x174) = 0;
        goto event_loop;
    }
    if (S16(work, 0x172)) {
        Func_08015208();
        Func_08091660();
        Func_080f9010(111);
        Func_0808c44c();
        Func_080770c8(0x106);
        if (*startup && ((*input & 6) == 6)) {
            Func_08015290();
        } else if (Func_080770c0(0x107)) {
            S16(work, 0x182) = 250;
        } else {
            S16(work, 0xcc0) = 0;
            Func_080151e8();
            S16(work, 0xcc0) = 1;
        }
        close_field_action();
        Func_080770d0(0x106);
        Func_080772f0();
        S16(work, 0x172) = 0;
        goto event_loop;
    }
    if (S16(work, 0x178)) {
        Func_0808c44c();
        Func_0808d5dc(U16(work, 0x178) & 0xfff);
        close_field_action();
        S16(work, 0x178) = 0;
        goto event_loop;
    }
    if (S16(work, 0x17a)) {
        Func_0808c44c();
        Func_0808d9a4(S16(work, 0x17a));
        close_field_action();
        S16(work, 0x17a) = 0;
        goto event_loop;
    }
    if (S16(work, 0x17e)) {
        Func_08015208();
        Func_0808c44c();
        Func_0808e680(S16(work, 0x17e));
        close_field_action();
        S16(work, 0x17e) = 0;
        goto event_loop;
    }
    if (S16(work, 0x180)) {
        Func_0808c44c();
        Func_0808e23c(S16(work, 0x180), S16(work, 0x19a));
        close_field_action();
        S16(work, 0x180) = 0;
        goto event_loop;
    }
    if (S16(work, 0x176)) {
        Func_080f9010(111);
        Func_08091660();
        Func_0808c44c();
        Func_080770c8(0x106);
        if (*startup && (*input & 2)) {
            Func_08015288();
        } else if (*startup && (*input & 0x200)) {
            Func_0808d0c8();
        } else if (Func_080770c0(0x107)) {
            S16(work, 0x182) = 250;
        } else {
            volatile u8 *scan;
            s32 empty = 0;
            s32 i;
            volatile u8 *save;
            Func_0808e118();
            Func_08015208();
            S16(state, 0x5e) = Func_080f9070();
            if (!Func_080770c0(0x17e)) {
                save = *(volatile u8 **)0x03001e68;
                scan = (volatile u8 *)0x03001810;
                for (i = 0; i < 0x200; i++)
                    if (scan[i] == 0xff) empty++;
                if (empty < 0x88) {
                    S16(save, 4) = 1;
                    Func_080030f8(1);
                }
                Func_08015370(0);
                S16(save, 4) = 0;
            } else {
                Func_08015040((const void *)0xc2f, 1);
            }
        }
        close_field_action();
        Func_080770d0(0x106);
        S16(work, 0x176) = 0;
    }
    goto event_loop;

finish:
    if (U16(work, 0x1c6)) {
        Func_080901c0(S32(work, 0x1c0), S32(work, 0x1c8));
        U16(work, 0x1c6) = 0;
        Func_080030f8(S32(work, 0x1c8));
    }
    Func_08002dd8(27);
    return result;
}

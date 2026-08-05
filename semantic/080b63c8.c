/*
 * Correctness fix, veneer audit (2026-08-01).
 * 0x080072e4 begins the GCC `__call_via_rN` veneer bank -- fifteen four-byte
 * `bx rN; nop` entries, r0..lr, ending at 0x08007320 -- so a `bl` into that
 * range is an indirect call through the named register, not a call to a
 * function at the branch target.  Resolved with tools/veneer_resolve.ts.
 *
 * UNCERTAINTY, and it is deliberate.  What 0x03000164 DOES is not
 * established.  semantic/main/080c1ffc.c calls it a resident two-argument
 * owner initializer; across the tree it is reached with two arguments at
 * some sites and three at others, and where a third is passed it is almost
 * always zero.  It also sits four bytes -- one ARM instruction -- from the
 * fill at 0x03000168, the way the sin/cos pair at 0x0800231c/0x08002322
 * does.  That is suggestive of two entry points into one routine and it is
 * NOT asserted here: the evidence is recorded so the exact reconstruction can settle
 * it, and the type below says only what this call site proves.
 */
#include "types.h"

typedef void (*Resident_03000164)(void *destination, u32 size, u32 value);

typedef struct Transfer_080b63c8 {
    const void *source;
    void *destination;
    u32 control;
} Transfer_080b63c8;

typedef struct Scene_080b63c8 {
    u8 unknown_000[60];
    u16 selected_03c;
    u16 outcome_03e;
    u8 unknown_040;
    u8 display_mode_041;
    u8 transfer_complete_042;
    u8 unknown_043;
    u8 link_active_044;
    u8 presentation_045;
    u8 unknown_046[10];
    u8 link_side_050;
    u8 unknown_051;
    u8 link_failed_052;
    u8 unknown_053;
    s32 graphics_054;
    u8 unknown_058[660];
    s16 entries_2ec[294];
    s32 result_538;
    u8 unknown_53c[268];
    u16 resource_648;
} Scene_080b63c8;

typedef struct Fade_080b63c8 {
    s32 target;
    s32 step;
    u8 unknown_08[12];
    s32 enabled;
    s32 unknown_18;
    s32 unknown_1c;
} Fade_080b63c8;

void Func_080030f8(s32);
void Func_080037d4(s32);
void Func_08003b70(s32);
void Func_08003ce0(void);
void Func_08003f3c(s32);
s32 Func_080040b4(s32);
void Func_080040e8(void);
void Func_080041d8(const void *, u32);
void Func_08004278(s32);
void Func_080042c8(s32);
void Func_0800488c(void);
void Func_080048a0(void);
void *Func_080048f4(s32, u32);
void Func_080049ac(void);
void Func_08009078(s32);
void Func_08015008(s32);
void Func_08015018(s32, s32);
s32 Func_08015038(s32, s32, s32, s32);
s32 Func_08015048(void);
void Func_08015118(void);
void Func_08015120(s32, s32);
void Func_08015128(s32);
void Func_08015130(s32);
void Func_080151c8(s32);
u32 *Func_08077000(s32);
s32 Func_080770c0(s32);
void Func_080770c8(s32);
s32 Func_080770e0(s32);
void Func_08077140(s32, s32, s32);
u32 Func_080771a0(void);
s32 Func_0808a4a0(void);
void Func_080b5a0c(void);
void Func_080b5b14(s32);
void Func_080b5b18(void);
void Func_080b5c08(void);
void Func_080b5d3c(void);
s32 Func_080b6148(void);
void Func_080b6378(void);
void Func_080b63b0(void);
s32 Func_080b6a60(s32);
s32 Func_080b6b40(s32, u16 *);
void Func_080b6c90(void);
void Func_080b75dc(void);
void Func_080b7e7c(void);
void Func_080b7f9c(void);
s32 Func_080b8574(void *);
s32 Func_080b874c(void *);
s32 Func_080b9934(void *);
void Func_080b9b2c(void);
s32 Func_080b9b30(s16 *, s32);
void Func_080bb65c(void);
void Func_080bb7c0(s32, s32);
void Func_080bf5a8(void);
void Func_080bf674(void);
void Func_080bf678(void);
void Func_080c02a4(s32, s32);
void Func_080c08a8(void);
void Func_080c08e0(void);
void Func_080c08ec(s32, s32, s32);
void Func_080c0a24(u32, u32, s32, s32, u32);
void Func_080c0cec(s32, s32, s32, s32);
s32 Func_080c1ffc(s32);
void Func_080c24b0(void);
void Func_080c2724(void);
void Func_080f9010(s32);

static void StartTransfer_080b63c8(
    const void *source,
    void *destination,
    u32 control)
{
    volatile Transfer_080b63c8 *transfer =
        (volatile Transfer_080b63c8 *)0x040000d4;

    transfer->source = source;
    transfer->destination = destination;
    transfer->control = control;
}

s32 Func_080b63c8(s32 mode)
{
    Scene_080b63c8 *scene;
    Fade_080b63c8 *fade;
    void *graphics;
    s32 resource;
    s32 result;
    s32 count;
    s32 index;
    s32 panel;
    u32 fill;

    fade = Func_080048f4(12, 76);
    scene = Func_080048f4(9, 0x82c);
    graphics = Func_080048f4(54, 0x7c8);
    {
        Fade_080b63c8 *window = Func_080048f4(44, 32);

        Func_080048f4(11, 640);
        ((Resident_03000164)0x03000164)(graphics, 0x7c8, 0);
        Func_080040e8();

        window->target = 0x2000;
        window->step = 0;
        window->enabled = 1;
        window->unknown_18 = 0;
        window->unknown_1c = 0;
        *(volatile u16 *)0x04000000 = 1;

        Func_080770c8(0x103);
        Func_080770c8(0x169);
        Func_080049ac();

        fill = 0;
        StartTransfer_080b63c8(&fill, fade, 0x85000013);
        fill = 0;
        StartTransfer_080b63c8(&fill, scene, 0x8500020b);
        scene->graphics_054 = -1;
        *(s32 *)scene = mode;

        StartTransfer_080b63c8(
            &fill,
            Func_080048f4(37, 12),
            0x85000003);

        scene->resource_648 = Func_0808a4a0();
        Func_080048f4(4, 0x0e00);
        Func_080048f4(3, 0x0600);
        Func_08009078(4);
        if (Func_080770c0(0x16e) != 0)
            Func_08015008(1);
        else
            Func_08015008(0);

        *(s32 *)((u8 *)fade + 12) = 0;
        *(s32 *)((u8 *)fade + 16) = 0x00400000;
        *(s32 *)((u8 *)fade + 20) = 0;
        *(s32 *)((u8 *)fade + 4) = 0x00b40000;
        *(s32 *)((u8 *)fade + 8) = 0x00400000;
        *(s32 *)((u8 *)fade + 0) = 0;
        *(u16 *)((u8 *)fade + 54) = 0x2800;
        *(u16 *)((u8 *)fade + 52) = 0x5000;
        *(s32 *)((u8 *)fade + 32) = 0x01000000;

        resource = Func_080c1ffc(*(s32 *)scene);
        if (Func_080770c0(0x16c) != 0) {
            scene->link_active_044 = 1;
            *(volatile u8 *)0x0200046b = 4;
        }

        if (scene->link_active_044 != 0) {
            volatile u16 *link_status = (volatile u16 *)0x03001f64;
            s32 timeout = 0;

            *(volatile u32 *)0x020023a8 = 0;
            while ((*link_status & 3) != 3) {
                Func_080030f8(1);
                if (++timeout > 24) {
                    scene->link_failed_052 = 1;
                    break;
                }
            }

            scene->link_side_050 =
                (*(volatile u32 *)0x04000128 << 26) >> 30;
            {
                u8 *destination = *(u8 **)0x03001f28;
                const u8 *source = (const u8 *)0x080b5865;

                for (index = 0; index <= 0x7c7; index++)
                    destination[index] = source[index];
            }

            resource = Func_080770e0(0x3f0);
            Func_080b6378();
            scene->transfer_complete_042 = 0;
        }

        Func_080041d8((const void *)0x080b5865, 0x0c7f);
        if (*(volatile s16 *)0x03001f58 != 0) {
            Func_080f9010(*(volatile s16 *)0x03001f58);
            if (Func_080770c0(0x16c) != 0) {
                Func_080f9010(55);
                Func_080037d4(4);
            }
        } else {
            Func_080f9010(51);
            Func_080f9010(76);
        }

        Func_080b5a0c();
        Func_080b75dc();
        Func_080b5c08();
        Func_080b5d3c();
        scene->display_mode_041 = *Func_08077000(0) != 0 ? 3 : 1;

        Func_08015128(9);
        Func_080b7f9c();
        Func_080b6c90();
        Func_080c08a8();
        Func_080c08ec(1, scene->resource_648, 0);
        Func_080c0a24(0x00a00000, 0x00500000, 0, 0, 0x00010000);
        Func_080c0cec(0, 0, 0, 190);
        Func_080b5b14(1);
        *(volatile u16 *)0x04000050 = 0;
        Func_080c24b0();

        scene->graphics_054 = Func_080040b4(128);
        scene->presentation_045 = 0;
        if (Func_080770c0(0x16e) != 0) {
            scene->presentation_045 = 1;
        } else if (*(volatile u8 *)0x0200046b == 0) {
            if ((Func_080771a0() & 15) == 0)
                scene->presentation_045 = 1;
            else if ((Func_080771a0() & 31) == 0)
                scene->presentation_045 = 2;
        }

        Func_080c02a4(resource, mode);
        window->enabled = 0;
    }

    *(volatile u8 *)0x03001f58 = 0;
    Func_080041d8((const void *)0x080b7739, 0x0c80);

restart:
    Func_080b9b2c();
    Func_080b5d3c();
    scene->display_mode_041 = *Func_08077000(0) != 0 ? 3 : 1;
    fade->target = 0x2800;
    fade->step = 60;
    Func_08015130(scene->display_mode_041);
    ((Resident_03000164)0x03000164)(scene->entries_2ec, 320, 0);
    Func_08003f3c(scene->graphics_054);

    if (Func_080770c0(0x16a) == 0) {
        Func_0800488c();
        Func_080048a0();
        count = Func_080b9934(scene->entries_2ec);
        Func_0800488c();
        Func_080048a0();
    } else {
        count = Func_080b8574(scene->entries_2ec);
    }

    scene->graphics_054 = Func_080040b4(128);
    Func_08015130(scene->display_mode_041);
    if (count < 0)
        goto link_failure;

    for (index = 0; index < count; index++) {
        s16 *entry = &scene->entries_2ec[index * 8];
        s32 owner = *entry;
        s32 accepted;

        Func_0800488c();
        Func_080048a0();
        if (Func_080770c0(0x16a) == 0)
            accepted = Func_080b9b30(entry, index == 0 ? 10 : 0);
        else
            accepted = Func_080b874c(entry);

        if (accepted == 1)
            goto cancelled;

        Func_0800488c();
        Func_080048a0();
        if (Func_080b6b40(1, NULL) == 0)
            goto no_first_group;

        if (Func_080b6b40(2, NULL) != 0) {
            if (Func_080b6148() < 0)
                goto link_failure;
            continue;
        }

        if ((u32)owner <= 7 && scene->result_538 == 1)
            scene->outcome_03e = 3;

        Func_080b63b0();
        if (Func_080770c0(0x16e) == 0) {
            if (scene->link_active_044 != 0)
                Func_080f9010(58);
            if (scene->result_538 != 0) {
                Func_080f9010(58);
                if (scene->outcome_03e <= 1) {
                    Func_08077140(
                        128,
                        *(u16 *)((u8 *)scene + 16 +
                            scene->selected_03c * 2),
                        26);
                    Func_08015118();
                    Func_08015120(128, 1);
                    Func_080151c8(0x838 + scene->outcome_03e);
                    Func_080bb65c();
                }
            }
            Func_080c2724();
        }

        Func_080f9010(17);
        Func_08003b70(30);
        Func_08003ce0();
        result = scene->result_538;
        goto teardown;
    }

    scene->presentation_045 = 0;
    Func_080bf674();
    Func_080bf678();
    Func_080b7e7c();
    if (scene->link_active_044 != 0) {
        if (Func_080b6148() < 0)
            goto link_failure;
    } else {
        Func_080030f8(20);
    }

    if (Func_080770c0(0x16e) == 0)
        goto restart;

    panel = Func_08015038(0x0c47, 0, 4, 1);
    while (Func_08015048() == 0)
        Func_080030f8(1);
    Func_08015018(panel, 1);
    Func_080030f8(1);
    panel = Func_08015038(0x0c48, 10, 4, 1);
    Func_080bb7c0(92, 24);
    Func_08015018(panel, 1);
    Func_080030f8(1);
    goto restart;

no_first_group:
    Func_080b63b0();
    Func_080f9010(59);
    Func_08015118();
    Func_08015120(*(volatile u8 *)0x02000438, 1);
    if (Func_080b6a60(0) == 1)
        Func_080151c8(0x083d);
    else
        Func_080151c8(0x0837);
    Func_080bb65c();
    Func_080f9010(17);
    result = -1;
    Func_08003b70(30);
    Func_08003ce0();
    goto teardown;

cancelled:
    Func_080f9010(17);
    Func_08003b70(30);
    Func_08003ce0();
    result = 0x3e7;
    goto teardown;

link_failure:
    Func_080b63b0();
    Func_080042c8(0);
    *(volatile u16 *)0x04000000 = 1;
    result = scene->result_538;
    Func_080770c8(1000);

teardown:
    Func_080b5b18();
    Func_080bf674();
    Func_080bf5a8();
    *(volatile u8 *)0x0200046b = 0;
    Func_08004278(0x080b7739);
    Func_080c08e0();
    return result;
}

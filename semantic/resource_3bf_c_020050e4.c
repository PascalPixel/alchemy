typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;

typedef void (*Task_020050e4)(void);

/*
 * Mode-dispatch scene owner at 0x020050e4, 576 bytes through its sole return
 * and eleven-word pool before 0x02005324.  The linked jump table physically
 * embedded at 0x02005170-0x020051eb maps modes 1..31 to the labels below;
 * source labels retain machine-layout order, including shared tails, so all
 * 32 call sites remain auditable without assembly embeddings.
 */

extern s16 Data_02000240[];

extern void Func_02001150(void);
extern void Func_0200138c(void);
extern void Func_02001a44(void);
extern void Func_0200206c(void);
extern void Func_020054b0(void);
extern void Func_080000c0(s32 frames);
extern void Func_080000d0(Task_020050e4 task, s32 priority);
extern void Func_08009128(void);
extern void Func_080091e0(void *record, s32 mode);
extern void Func_08009258(s32 value);
extern s32 Func_080770c0(s32 flag);
extern u8 *Func_0808a080(s32 actor);
extern void Func_0808a0f0(s32 actor, s32 x, s32 z);
extern void Func_0808a158(s32 actor, s32 enabled);
extern void Func_0808a1e0(s32 actor, s32 mode);

void Func_020050e4(void)
{
    u8 *state;
    u8 *record;
    Task_020050e4 task;
    s32 mode;

    Func_020054b0();
    Func_0808a158(9, 1);
    Func_0808a158(10, 1);
    Func_0808a158(17, 1);

    if (Func_080770c0(0x94c) != 0)
        Func_0808a0f0(15, 0, 0);
    if (Func_080770c0(0x949) != 0)
        Func_0808a0f0(11, 0, 0);
    if (Func_080770c0(0x94b) != 0)
        Func_0808a0f0(16, 0, 0);
    if (Func_080770c0(0xf2e) != 0)
        Func_0808a0f0(8, 0, 0);

    state = *(u8 **)0x03001ebc;
    mode = Data_02000240[225];
    switch (mode) {
    case 1:
    case 2:
    case 3:
        goto mode_1_to_3;
    case 10:
    case 13:
    case 20:
    case 23:
    case 24:
        goto mode_10_group;
    case 21:
    case 22:
        goto mode_21_to_22;
    case 11:
    case 12:
        goto mode_11_to_12;
    case 31:
        goto mode_31;
    case 14:
    case 15:
    case 16:
        goto mode_14_to_16;
    default:
        goto default_mode;
    }

mode_1_to_3:
    *(s32 *)(state + 0x1c0) = 0x200;
    Func_08009258(224 << 4);
    task = Func_02001150;
    goto schedule_task;

mode_10_group:
    *(s32 *)(state + 0x1c0) = 0x209;
    Func_08009258(192 << 4);
    record = Func_0808a080(24);
    Func_080091e0(record, 0);
    if (Func_080770c0(0x314) != 0)
        Func_0808a0f0(25, 0x03680000, 0x00780000);
    goto finish;

mode_21_to_22:
    *(s32 *)(state + 0x1c0) = 0x200;
    Func_08009258(224 << 4);
    task = Func_0200138c;

schedule_task:
    Func_080000d0(task, 200 << 4);
    Func_080000c0(1);
    Func_08009128();
    Func_080000c0(1);
    goto finish;

mode_11_to_12:
    *(s32 *)(state + 0x1c0) = 0x200;
    if (Func_080770c0(0x94a) != 0)
        Func_0200206c();
    goto finish;

mode_31:
    *(s32 *)(state + 0x1c0) = 0x200;
    Func_0200206c();
    goto finish;

mode_14_to_16:
    Func_080000d0(Func_02001a44, 200 << 4);
    goto finish;

default_mode:
    *(s32 *)(state + 0x1c0) = 0x200;
    Func_08009258(224 << 4);

finish:
    record = Func_0808a080(8);
    Func_080091e0(Func_0808a080(8), 0);
    Func_0808a1e0(8, 1);
    *(s32 *)(record + 0x18) = 0xc000;
    *(s32 *)(record + 0x1c) = 0xc000;
}

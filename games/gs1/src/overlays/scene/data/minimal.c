#include "types.h"
#include "scene.h"

/* Signed halfwords in the shared work area; index 225 selects the mode. */
extern s16 gCell[];
extern s32 gIw;
extern s32 gIw2;
extern u8 Value_00000000;
extern u8 Value_00000001;
extern u8 Value_00000004;
extern u8 Value_0000000b;

u8 *SceneData_Run(void *);

/*
 * The eight-byte owner at 0x02000030 includes its one pool word, which holds
 * the returned table address 0x020085f8.
 */
u8 *FunctionHead_02000030(void)
{
    return (u8 *)0x020085f8;
}

s32 FunctionHead_02000030(void)
{
    return 0;
}

/*
 * The eight-byte owner at 0x0200003c includes its one pool word, which holds
 * the returned table address 0x02008628.
 */
u8 *FunctionHead_02000030(void)
{
    return (u8 *)0x02008628;
}

/*
 * The eight-byte owner at 0x02000044 includes its one pool word, which holds
 * the returned table address 0x0200862c.
 */
u8 *FunctionHead_02000030(void)
{
    return (u8 *)0x0200862c;
}

/*
 * The eight-byte owner at 0x0200004c includes its one pool word, which holds
 * the returned table address 0x02008644.
 */
u8 *FunctionHead_02000030(void)
{
    return (u8 *)0x02008644;
}

s32 FunctionHead_02000030(void)
{
    s32 wait;
    s16 mode = gCell[225];

    if (mode == 10) {
        u8 *object = SceneData_Run(*(void **)&gCell[250]);

        object[85] = 0;
        SceneData_Do(75);
        SceneData_unk2_2(0);
        SceneData_unk3_2(120);
        /*
         * The wait is a guarded do-while, not a plain while: a plain while
         * leaves the test at the top and ends the loop with an unconditional
         * jump back rather than the conditional back-edge the reference has.
         */
        wait = 0;
        if (gIw == 0) {
            do {
                SceneData_unk4_2(1);
                if (++wait > 3599) {
                    break;
                }
            } while (gIw == 0);
        }
        SceneData_Apply((s32)(u32)&Value_00000000, 2);
        return 0;
    }
    if (mode == 9) {
        SceneData_unk5_2(67);
        SceneData_unk6(0);
        SceneData_unk7(17);
        SceneData_unk8(60);
        SceneData_unk8_3();
        SceneData_unk9(240);
        SceneData_unk10(19);
        SceneData_Apply2((s32)(u32)&Value_00000001, 2);
        return 0;
    }
    SceneData_unk11((s32)(u32)&Value_0000000b);
    if (gCell[225] == 2) {
        for (;;) {
            SceneData_unk12(19);
            SceneData_unk13(0);
            SceneData_unk14(0);
            if (SceneData_Check() <= 0) {
                goto stop;
            }
            SceneData_unk15(70);
            if (SceneData_unk2(1) != 0) {
                goto stop;
            }
            SceneData_unk16(17);
            SceneData_unk17(30);
            SceneData_unk9_3();
            wait = 0;
            if (gIw2 == 0) {
                do {
                    SceneData_unk18(1);
                    if (++wait > 119) {
                        break;
                    }
                } while (gIw2 == 0);
            }
        }
stop:
        SceneData_Apply3((s32)(u32)&Value_00000001, 1);
    } else {
        SceneData_unk19(64);
        SceneData_unk20(0);
        SceneData_unk10_3();
        SceneData_Apply4((s32)(u32)&Value_00000004, 16);
        SceneData_unk21(17);
    }
    SceneData_unk22(17);
    SceneData_unk23(30);
    SceneData_unk11_3();
    SceneData_unk24(60);
    SceneData_unk25(19);
    return 0;
}

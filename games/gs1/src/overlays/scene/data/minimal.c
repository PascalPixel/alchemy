#include "types.h"
#include "scene.h"
#include "abi/overlays/scene/data/minimal.h"

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
u8 *SceneData_Run2(void)
{
    return (u8 *)0x020085f8;
}

s32 SceneData_Run3(void)
{
    return 0;
}

/*
 * The eight-byte owner at 0x0200003c includes its one pool word, which holds
 * the returned table address 0x02008628.
 */
u8 *SceneData_Run4(void)
{
    return (u8 *)0x02008628;
}

/*
 * The eight-byte owner at 0x02000044 includes its one pool word, which holds
 * the returned table address 0x0200862c.
 */
u8 *SceneData_Run5(void)
{
    return (u8 *)0x0200862c;
}

/*
 * The eight-byte owner at 0x0200004c includes its one pool word, which holds
 * the returned table address 0x02008644.
 */
u8 *SceneData_Run6(void)
{
    return (u8 *)0x02008644;
}

s32 SceneData_Run7(void)
{
    s32 wait;
    s16 mode = gCell[225];

    if (mode == 10) {
        u8 *object = SceneData_Run(*(void **)&gCell[250]);

        object[85] = 0;
        SceneData_Do(75);
        SceneData_Do2(0);
        SceneData_Do3(120);
        /*
         * The wait is a guarded do-while, not a plain while: a plain while
         * leaves the test at the top and ends the loop with an unconditional
         * jump back rather than the conditional back-edge the reference has.
         */
        wait = 0;
        if (gIw == 0) {
            do {
                SceneData_Do4(1);
                if (++wait > 3599) {
                    break;
                }
            } while (gIw == 0);
        }
        SceneData_Apply((s32)(u32)&Value_00000000, 2);
        return 0;
    }
    if (mode == 9) {
        SceneData_Do5(67);
        SceneData_Do6(0);
        SceneData_Do7(17);
        SceneData_Do8(60);
        SceneData_Run8();
        SceneData_Do9(240);
        SceneData_Do10(19);
        SceneData_Apply2((s32)(u32)&Value_00000001, 2);
        return 0;
    }
    SceneData_Do11((s32)(u32)&Value_0000000b);
    if (gCell[225] == 2) {
        for (;;) {
            SceneData_Do12(19);
            SceneData_Do13(0);
            SceneData_Do14(0);
            if (SceneData_Check() <= 0) {
                goto stop;
            }
            SceneData_Do15(70);
            if (SceneData_Check2(1) != 0) {
                goto stop;
            }
            SceneData_Do16(17);
            SceneData_Do17(30);
            SceneData_Run9();
            wait = 0;
            if (gIw2 == 0) {
                do {
                    SceneData_Do18(1);
                    if (++wait > 119) {
                        break;
                    }
                } while (gIw2 == 0);
            }
        }
stop:
        SceneData_Apply3((s32)(u32)&Value_00000001, 1);
    } else {
        SceneData_Do19(64);
        SceneData_Do20(0);
        SceneData_Run10();
        SceneData_Apply4((s32)(u32)&Value_00000004, 16);
        SceneData_Do21(17);
    }
    SceneData_Do22(17);
    SceneData_Do23(30);
    SceneData_Run11();
    SceneData_Do24(60);
    SceneData_Do25(19);
    return 0;
}

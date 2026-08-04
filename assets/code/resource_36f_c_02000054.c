#include "types.h"

/* Signed halfwords in the shared work area; index 225 is the same slot src/0808b1d8.c reads. */
extern s16 Data_02000240[];

extern s32 Data_03001c94;
extern s32 Data_03001ae8;

extern u8 Value_00000000;
extern u8 Value_00000001;
extern u8 Value_00000004;
extern u8 Value_0000000b;

u8 *Func_02000620(void *);
void Func_0200036a(s32);
void Func_020005c0(s32);
void Func_020005d2(s32);
void Func_02000656(s32);
void Func_02000662(void);
void Func_02000668(s32, s32);
void Func_0200066c(s32);
void Func_0200066e(s32);
void Func_0200067c(s32);
void Func_02000680(s32);
void Func_02000692(s32);
void Func_0200069e(s32, s32);
s32 Func_020006a4(void);
void Func_020006ac(s32);
void Func_020006b4(s32);
void Func_020006b8(s32);
void Func_020006c0(void);
void Func_020006ce(s32);
void Func_020006e2(s32);
void Func_020006ec(s32);
void Func_020006f0(s32);
s32 Func_020006f4(s32);
void Func_02000706(s32);
void Func_0200070a(void);
void Func_0200070c(s32);
void Func_02000710(s32, s32);
void Func_02000716(s32);
void Func_02000718(void);
void Func_0200072a(s32, s32);
void Func_02000736(s32);
void Func_0200073e(s32);
void Func_02000750(s32);
void Func_02000768(s32);
void Func_0200076e(s32);
void Func_02000784(s32);

s32 Func_02000054(void)
{
    s32 wait;
    s16 mode = Data_02000240[225];

    if (mode == 10) {
        u8 *object = Func_02000620(*(void **)&Data_02000240[250]);

        object[85] = 0;
        Func_0200066c(75);
        Func_0200036a(0);
        Func_020005c0(120);
        /* The wait is a guarded do-while, not a plain while. A plain while
           leaves the test at the top, and the bottom of the loop is then an
           unconditional jump back rather than the conditional back-edge the
           reference has. */
        wait = 0;
        if (Data_03001c94 == 0) {
            do {
                Func_020005d2(1);
                if (++wait > 3599) {
                    break;
                }
            } while (Data_03001c94 == 0);
        }
        Func_02000668((s32) (u32) &Value_00000000, 2);
        return 0;
    }
    if (mode == 9) {
        Func_020006ac(67);
        Func_02000692(0);
        Func_020006b8(17);
        Func_02000656(60);
        Func_02000662();
        Func_02000680(240);
        Func_020006ce(19);
        Func_0200069e((s32) (u32) &Value_00000001, 2);
        return 0;
    }
    Func_0200066e((s32) (u32) &Value_0000000b);
    if (Data_02000240[225] == 2) {
        for (;;) {
            Func_020006ec(19);
            Func_020006e2(0);
            Func_020006f0(0);
            if (Func_020006a4() <= 0) {
                goto stop;
            }
            Func_02000706(70);
            if (Func_020006f4(1) != 0) {
                goto stop;
            }
            Func_02000716(17);
            Func_020006b4(30);
            Func_020006c0();
            wait = 0;
            if (Data_03001ae8 == 0) {
                do {
                    Func_0200067c(1);
                    if (++wait > 119) {
                        break;
                    }
                } while (Data_03001ae8 == 0);
            }
        }
stop:
        Func_02000710((s32) (u32) &Value_00000001, 1);
    } else {
        Func_02000750(64);
        Func_0200073e(0);
        Func_0200070a();
        Func_0200072a((s32) (u32) &Value_00000004, 16);
        Func_02000768(17);
    }
    Func_0200076e(17);
    Func_0200070c(30);
    Func_02000718();
    Func_02000736(60);
    Func_02000784(19);
    return 0;
}

#include "types.h"

extern u8 *Data_03001f1c;
extern s16 Data_02002004;

s32 Func_080056cc(void);
void Func_08005c68(void);
void Func_08005cf8(void);
s32 Func_08005920(s16 resource, void *destination);
void Func_080030f8(s32 frames);
void Func_08006358(void);
void Func_0800651c(void);
s32 Func_08017364(void);
void Func_0801776c(s32 object, s32 options);
void Func_08019a54(s32 value);
void Func_0801f818(void);
s16 Func_08020244(s16 value, s32 mode);
s32 Func_08028df4(s32, s32, s32, s32);
void Func_0808a5b0(void);
void Func_080f9010(s32 sound);

s16 Func_080207c4(void)
{
    s16 result = 0;

    if (Func_080056cc() != 0) {
        Func_0801776c(10, 1);
        result = -9;
    } else {
        s16 selection;

        Func_08005c68();
        selection = Func_08020244(Data_02002004, 0);
        if (selection == -1) {
            result = selection;
        } else if (Data_03001f1c[0x105c + selection * 0x40] != 0) {
            s32 action;

            Func_0801776c(20, 13);
            while (Func_08017364() == 0)
                Func_080030f8(1);

            action = Func_08028df4(1, 0, 0, 1);
            Func_08019a54(action);
            if (action == 0)
                goto load_resources;
        } else {
        load_resources:
            Data_02002004 = selection;
            Func_080f9010(0x55);
            Func_0801776c(26, 13);
            while (Func_08017364() == 0)
                Func_080030f8(1);

            Func_0801f818();
            Func_0808a5b0();
            {
                s32 first =
                    Func_08005920(selection, (void *)0x02000000);
                s32 second =
                    Func_08005920(selection + 3, (void *)0x02001000);

                Func_08019a54(second);
                if ((first | second) != 0) {
                    Func_0801776c(11, 1);
                    result = -3;
                } else {
                    Func_0801776c(23, 9);
                }
            }
        }
    }

    Func_08005cf8();
    return result;
}

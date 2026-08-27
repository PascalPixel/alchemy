#include "types.h"

extern u8 Value_0000000a[];
extern u8 Value_0000000b[];
extern u8 Value_00000014[];
extern u8 Value_00000017[];
extern u8 Value_0000001a[];
extern s16 Data_02002004;
extern u8 *Data_02000000;

s32 Func_080056cc(void);
void Func_08005c68(void);
s32 Func_08020244(s16 a, s32 b);
void Func_0801776c(s32 msg, s32 mode);
s32 Func_08017364(void);
void WaitFrames(s32 frames);
s32 Func_08028df4(s32 a, s32 b, s32 c, s32 d);
void Func_08019a54(void);
void Func_080f9010(u8 mode, s32 a);
void Func_0801f818(void);
void Func_0808a5b0(void);
s32 Func_08005920(s32 a, void *b);
void Func_08005cf8(void);

s32 Func_080207c4(void)
{
    s32 result;
    s32 found;
    s32 id;
    s32 flag;

    result = 0;
    found = Func_080056cc();
    if (found != 0) {
        Func_0801776c((s32)Value_0000000a, 1);
        result = -9;
    } else {
        Func_08005c68();
        id = Data_02002004;
        found = Func_08020244(id, 0);
        if (found == -1) {
            result = found;
        } else {
            if (*(u8 *)((u8 *)Data_02000000 + (found << 6) + 0x105c) != 0) {
                Func_0801776c((s32)Value_00000014, 13);
                while (Func_08017364() == 0) {
                    WaitFrames(1);
                }
                if (Func_08028df4(1, 0, 0, 1) != 0) {
                    Func_08019a54();
                    goto skip;
                }
                Func_08019a54();
            }
            Func_080f9010(85, found);
            Func_0801776c((s32)Value_00000014, 13);
            while (Func_08017364() == 0) {
                WaitFrames(1);
            }
            Func_0801f818();
            Func_0808a5b0();
            flag = Func_08005920(found, &Data_02000000[0x1000]);
            flag |= Func_08005920(found + 3, &Data_02000000[0x1000 + 0x1000]);
            Func_08019a54();
            if (flag != 0) {
                Func_0801776c((s32)Value_0000000b, 1);
                result = -3;
            } else {
                Func_0801776c((s32)Value_00000017, 9);
            }
        }
    }
skip:
    Func_08005cf8();
    return result;
}

#include "types.h"
#include "object_runtime.h"

/* FAKEMATCH: the native reloads this counter for every use and masks it in the register it was loaded into, as it does a volatile word; declared plain, the mask is built in the constant's register (differ 12). */
extern volatile s32 Data_02005ee4;
extern const u8 Data_02005d6c[];
extern const u8 Data_02005e28[];

void Animation_ApplyChildValues(struct ObjectRuntime *object, s32 value);
void Func_02004dac(struct ObjectRuntime *object, const u8 *script);
void Func_02005014(s32 cue);

void Func_02000a60(struct ObjectRuntime *object)
{
    s32 value = 0;

    if ((Data_02005ee4 & 7) <= 0)
        value = 7;
    Animation_ApplyChildValues(object, value & 7);
    if ((Data_02005ee4 & 31) == 0) {
        object->scale_x = 0x10000;
        Func_02004dac(object, Data_02005e28);
        Func_02005014(106);
    }
    Data_02005ee4++;
}

void Func_02000aac(struct ObjectRuntime *object)
{
    s32 value = 0;

    if ((Data_02005ee4 & 7) <= 0)
        value = 7;
    Animation_ApplyChildValues(object, value & 7);
    if ((Data_02005ee4 & 31) == 0) {
        object->scale_y = 0x10000;
        Func_02004dac(object, Data_02005d6c);
        Func_02005014(106);
    }
    Data_02005ee4++;
}

/* Near miss (8 instructions differ): the native masks the counter in the
 * register it loaded it into (ldr r3; movs r2, #31) and copies the object
 * into r0 before it loads the script; the draft masks into the constant's
 * register. Func_02000aac is the same function over scale_y and
 * Data_02005d6c. */
#include "types.h"
#include "object_runtime.h"

extern s32 Data_02005ee4;
extern const u8 Data_02005d6c[];
extern const u8 Data_02005e28[];

void Animation_ApplyChildValues(struct ObjectRuntime *object, s32 value);
void Func_02004dac(struct ObjectRuntime *object, const u8 *script);
void Func_02005014(s32 cue);

void Func_02000a60(struct ObjectRuntime *object)
{
    s32 *counter = &Data_02005ee4;
    s32 value = 0;

    if ((*counter & 7) <= 0)
        value = 7;
    Animation_ApplyChildValues(object, value & 7);
    if ((*counter & 31) == 0) {
        object->scale_x = 0x10000;
        Func_02004dac(object, Data_02005e28);
        Func_02005014(106);
    }
    (*counter)++;
}

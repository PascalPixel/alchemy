#include "types.h"

struct Object_080a9e48 {
    u8 padding_000[0xd8];
    u16 effects[15];
};

struct EffectDefinition_080a9e48 {
    u8 padding_00[0x0c];
    u8 kind;
    u8 padding_0d[0x1b];
    u16 target_id;
};

struct Runtime_080a9e48 {
    u8 padding_000[0x1c8];
    u16 entries[32];
    u8 padding_208[0x10];
    u8 entry_count;
};

extern struct Runtime_080a9e48 *Data_03001f2c;

struct Object_080a9e48 *Func_08077008(s32);
struct EffectDefinition_080a9e48 *Func_08077018(s32);
u8 Func_08077058(s32, s32);
u32 Func_080a3ddc(struct Object_080a9e48 *, u16 *, s32);
s32 Func_080a9f10(s32, s32, s32, s32);

s32 Func_080a9e48(s32 arg0, s32 arg1, s32 arg2)
{
    struct Runtime_080a9e48 *runtime;
    s32 result;
    s32 value;
    struct Object_080a9e48 *object;
    struct EffectDefinition_080a9e48 *definition;

    object = Func_08077008(arg1);
    runtime = Data_03001f2c;
    value = 0x1ff & object->effects[arg0];
    definition = Func_08077018(value);
    result = Func_080a9f10(
        0x3fff & definition->target_id,
        arg1,
        arg2,
        1);
    if (result != -1) {
        definition = Func_08077018(object->effects[arg0]);
        if (definition->kind == 1) {
            Func_08077058(arg1, arg0);
            runtime->entry_count =
                Func_080a3ddc(object, runtime->entries, 0);
        }
        if (definition->kind == 4) {
            if (value == 0xb8)
                value = 0xb9;
            object->effects[arg0] = value;
        }
        result = 0;
    }
    return result;
}

s32 Func_080a9f0c(void)
{
    return 1;
}

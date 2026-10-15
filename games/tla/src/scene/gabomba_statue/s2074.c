#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"

struct EventWork1cc {
    u8 unk000[0x1cc];
    struct ObjectRuntime *object;
};

struct IoReg16 {
    u16 value;
};

#define REG_BLDALPHA ((struct IoReg16 *)0x04000052)

// 奥へ進むほど濃くなる半透明
void Func_02002074(void)
{
    s32 blend;
    struct ObjectRuntime *object = ((struct EventWork1cc *)Ram_HeapSlots->eventWork)->object;
    s32 level = object->z;

    level = (level - 0xe80000) >> 19;
    if (level < 0)
        level = 0;
    if (level > 16)
        level = 16;
    blend = ((16 - level) << 8) + level;
    REG_BLDALPHA->value = blend;
}

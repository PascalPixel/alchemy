#include "types.h"
#include "m7_interfaces.h"

struct Fields_080a4e68 {
    u8 filler[0x10c];
    struct Bounds080a23f4 *object;
};

extern struct Fields_080a4e68 *Data_03001f2c;

void Func_080a4e68(void) {
    Func_080a23f4(Data_03001f2c->object, 13, 0, 17, 5);
}

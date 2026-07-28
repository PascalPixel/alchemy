#include "object_dispatch.h"

extern u32 Data_03001e40;
extern u8 Data_0809ed80[];

void Func_08092980(struct DispatchObject *object)
{

    if ((object->kind & 0xf) == 1) {
        u8 val;
        u8 *obj;
        u8 count;

        val = Data_0809ed80[(Data_03001e40 >> 1) & 3];
        obj = object->target.child;
        count = *(obj + 0x27);
        if (count != 0) {
            u8 **arr = (u8 **)(obj + 0x28);
            s32 i = count;
            do {
                u8 *elem = *arr++;
                if (elem != 0 && *(u32 *)(elem + 0x10) != 0) {
                    *(elem + 5) = val;
                }
                i--;
            } while (i != 0);
        }
        *(obj + 0x25) = 1;
    }
}

#include "types.h"

extern u8 *Data_03001ebc;
/* 生成順維持のため戻り値を整数で受け、直後にオブジェクト番地として扱う。 */
s32 ObjectTable_Get(u32 object_id);
/* Object table: 192 pointers at Data_03001ebc + 0x14 (object/table/get.c). */
void Object_Destroy(void *);

void ObjectTable_DestroyById(s32 index)
{
    void *object = (void *)ObjectTable_Get(index);
    u8 *base = Data_03001ebc;
    s32 offset;

    if (object != 0) {
        Object_Destroy(object);
        offset = index * 4;
        offset += 20;
        *(s32 *)(base + offset) = 0;
    }
}

void Func_0809294c(void)
{
}

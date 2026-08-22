#include "types.h"

#define RESOURCE_ID_MASK_0801C7FC 0x3FFF

struct ObjectResource_0801c7fc {
    u16 id;
    u16 padding_02;
};

struct Object_0801c7fc {
    u8 padding_00[0x58];
    struct ObjectResource_0801c7fc resources[32];
};

struct ResourcePair_0801c7fc {
    u16 object_id;
    u16 resource_id;
};

s32 Func_08077158(u16 *objects);
struct Object_0801c7fc *Func_08077008(s32 object_id);
void *Func_08077080(s32 resource_id);

s32 Func_0801c7fc(struct ResourcePair_0801c7fc *output)
{
    u16 object_ids[14];
    s32 output_count = 0;
    s32 object_count = Func_08077158(object_ids);

    if (output_count < object_count) {
        u16 *object_id = object_ids;
        s32 remaining = object_count;

        do {
            struct Object_0801c7fc *object;
            struct ObjectResource_0801c7fc *resource;
            u32 id;
            s32 index;
            u32 resource_id;
            s32 resource_offset;

            id = *object_id;
            object_id++;
            object = Func_08077008(id);
            index = 0;
            resource_offset = sizeof(object->padding_00);
            resource_id =
                *(u16 *)((u8 *)object + resource_offset)
                & RESOURCE_ID_MASK_0801C7FC;

            if (resource_id != 0) {
                struct ResourcePair_0801c7fc *pair;

                resource = object->resources;
                pair = (struct ResourcePair_0801c7fc *)
                    (output_count * sizeof(*pair) + (s32)output);
                do {
                    Func_08077080(resource_id);
                    pair->object_id = id;
                    pair->resource_id = resource_id;
                    output_count++;
                    pair++;

                    if (++index >= (s32)(sizeof(object->resources)
                                      / sizeof(object->resources[0]))) {
                        break;
                    }

                    resource++;
                    resource_id =
                        resource->id & RESOURCE_ID_MASK_0801C7FC;
                } while (resource_id != 0);
            }
        } while (--remaining != 0);
    }

    return output_count;
}

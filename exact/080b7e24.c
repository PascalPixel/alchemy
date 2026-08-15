#include "types.h"
#include "motion_object.h"

void Func_080b7e04(void *record_group);

void Func_080b7e24(struct MotionObject *object) {
    s32 storage_kind;
    s32 remaining;
    void **record_groups;
    void *record_group;

    if (object != NULL) {
        storage_kind = object->record_storage_kind & 0xF;
        switch (storage_kind) {
        case 1:
            Func_080b7e04(object->records);
            return;
        case 2:
            record_groups = object->records;
            remaining = 3;
            do {
                record_group = *record_groups++;
                if (record_group != NULL) {
                    Func_080b7e04(record_group);
                }
                remaining -= 1;
            } while (remaining >= 0);
            break;
        }
    }
}

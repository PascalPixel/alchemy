#include "object_lookup.h"
#include "types.h"
#include "object_effect.h"
#include "global_cells.h"

#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

void Func_08009128(void);
void *Runtime_AllocateBlock(s32 id, s32 flag);
void WaitFrames(s32);

void Object_AttachWorkTargetToObject(s32 id, s32 flag) {
    s32 obj;
    void *target;
    void *work;
    s32 *p;

    obj = ObjectTable_Get(id);
    work = Runtime_AllocateBlock(0x1B, 0xCCC);
    target = FIELD_AT_OFFSET(work, void **, 0x1E0);
    p = *(s32 **)ADDR_03001E70;
    if (obj != 0) {
        *p = (s32)((u8 *)target + 8);
        Func_080090e0(target, (void *)obj);
        if (flag == 0) {
            FIELD_AT_OFFSET(target, s32 *, 8) = (s32) FIELD_AT_OFFSET(obj, s32 *, 8);
            FIELD_AT_OFFSET(target, s32 *, 0xC) = (s32) FIELD_AT_OFFSET(obj, s32 *, 0xC);
            FIELD_AT_OFFSET(target, s32 *, 0x10) = (s32) FIELD_AT_OFFSET(obj, s32 *, 0x10);
            WaitFrames(1);
            if (FIELD_AT_OFFSET(work, s16 *, 0x19E) != 3) {
                Func_08009128();
            }
        }
    }
}

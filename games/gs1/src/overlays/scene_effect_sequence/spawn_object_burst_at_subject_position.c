#include "types.h"

#define FIELD_AT_OFFSET(base, type, offset) (*(type)((u8 *)(base) + (offset)))

s32 Func_02001c28(void);
s32 Func_02001c3a(void);
void Func_0200171a(s32, s32, s32);              /* SceneActor_TransformAndApplyRecordPosition */
s32 Func_02001c76(s32, s32);                    /* ObjectDispatch_InitializeFar */
s32 Func_02001c90(s32, s32, s32, s32);          /* Object_CreateFar */

void Func_02000b8c(s32 subject)
{
    s32 i;
    s32 rec;
    s32 tmp;

    for (i = 0; i <= 3; i++) {
        rec = Func_02001c90(240,
            FIELD_AT_OFFSET(subject, s32 *, 8),
            FIELD_AT_OFFSET(subject, s32 *, 12),
            FIELD_AT_OFFSET(subject, s32 *, 16));
        if (rec == 0) {
            break;
        }

        FIELD_AT_OFFSET(rec, s32 *, 28) = 0x8ccc;
        FIELD_AT_OFFSET(rec, s32 *, 24) = 0x8ccc;
        FIELD_AT_OFFSET(rec, u8 *, 85) = 2;
        FIELD_AT_OFFSET(rec, s32 *, 40) = -0x10000;

        FIELD_AT_OFFSET(rec, s32 *, 48) = Func_02001c28() + 0xcccc;
        FIELD_AT_OFFSET(rec, u8 *, 89) = 1;

        tmp = Func_02001c3a();
        Func_0200171a(rec, 0x200000, tmp);

        {
            u16 *p = (u16 *)((u8 *)rec + 94);
            s32 shown = 8;
            *p = shown;
        }

        Func_02001c76(rec, 0x02009884);
    }
}

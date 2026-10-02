/*
 * Draft: ScriptObject_CheckOverlap retains an unproven TBS-shaped body.
 * On 2026-10-02 the original saved draft failed ordinary TLA compilation:
 * ScriptObjectEntry was incomplete at its member accesses and increment.
 * This LOCAL DRAFT view copies the current TBS 112-byte record only to
 * preserve that attempted body. Its flags += 0x70 and entry++ are not a
 * proved TLA 128-byte stride; no native match or credit is claimed.
 * Under this same visible local view, the SCRIPT.H include migration
 * preserves every whole object in all six TLA editions, metadata included.
 * No matching shape was changed or compiler device introduced.
 */
#include "SCRIPT.H"
/* LOCAL DRAFT: current TBS view; not a shared TLA layout. */
struct ScriptObjectEntry {
    void *data;
    u8 unknown_04[4];
    s32 values_08[6];
    u16 value_20;
    u8 unknown_22[0x37];
    u8 flags_59;
    u8 unknown_5a[0x16];
};

s32 Runtime_CheckRadiusOverlap(s32 *, s32, s32 *, s32);

extern u8 gObjectSlots[];

s32 GameFlag_TestFar(s32);
s32 GameFlag_SetBitFar(s32);
void GameFlag_ClearBitFar(s32);
void ObjectDispatch_ApplyArgumentToChildren(void *, s32);
void ObjectDispatch_Release(void);
s32 Audio_PlayCue(s32);

s32 ScriptObject_CheckOverlap(struct ScriptObjectEntry *object, s32 *values)
{
    s32 tmp;
    s32 index;
    u8 *flags;
    struct ScriptObjectEntry *entry;

    entry = *(struct ScriptObjectEntry **)((u32)&gObjectSlots);
    index = 0;
    flags = &entry->flags_59;
loop_1:
    if (entry->data != NULL && (1 & *flags) && entry != object) {
        tmp = index;
        if (Runtime_CheckRadiusOverlap(entry->values_08, entry->value_20 - 2,
                          values, object->value_20 - 2) >= 0) {
            return -1;
        }
    }
    index += 1;
    flags += 0x70;
    entry++;
    if (index > 0x3F) {
        return 0;
    }
    goto loop_1;
}

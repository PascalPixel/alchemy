/*
 * Draft of overlay 6af Func_02000760. 29 instructions differ, all from one
 * cause: the listing loads the party state for each of the four scene tests,
 * where this draft keeps the third test's address in r7 for the fourth (gcse
 * makes the address one register and cse2 puts the pool load back at the
 * second and third tests only). Tried: the third test as one merged
 * condition, an early return for the fourth, an empty asm barrier and a
 * one-pass block before the fourth: no change.
 * Scene 0x12a is named SceneId_Ov6afRoom1 for it.
 */
#include "types.h"
#include "party_state.h"
#include "scene_ids.h"
#include "field_event.h"

struct FieldActor *Object_GetById();
void GameFlag_SetBit();
void Func_02000728();

void Func_02000760(s32 unused, s32 id)
{
    struct FieldActor *object = Object_GetById(id);

    if (gPartyState.scene == (s32)&SceneId_Ov6afRoom1) {
        if (id == 8 && object->x >> 20 == 16)
            GameFlag_SetBit(0x300);
        if (id == 9 && object->x >> 20 == 20)
            GameFlag_SetBit(0x301);
        if (id == 10 && object->x >> 20 == 27)
            GameFlag_SetBit(0x302);
    }
    if (gPartyState.scene == (s32)&SceneId_Ov6afRoom2) {
        if (id == 10 && object->x >> 19 == 33)
            GameFlag_SetBit(0x308);
        if (id == 11 && object->x >> 19 == 85 && object->z >> 19 == 83)
            Func_02000728();
    }
    if (gPartyState.scene == (s32)&SceneId_Ov6afRoom3) {
        if (id == 10 && object->x >> 19 == 83)
            GameFlag_SetBit(0x309);
    }
    if (gPartyState.scene == (s32)&SceneId_Ov6afRoom4) {
        if (id == 8 && object->x >> 19 == 87)
            GameFlag_SetBit(0x30d);
    }
}

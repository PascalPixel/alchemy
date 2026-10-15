#include "scene.h"
#include "object_runtime.h"

void Func_02002d40(void);

// A halfword of the party state that has no field name yet.
#define PARTY_STATE_U16(offset) (*(u16 *)((u8 *)&gPartyState + (offset)))

void Func_020035b8(void)
{
    Func_02004180(0);
    if (GameFlag_Test(0x302) == 0)
        Object_SetWideSprite(64, 0);
}

static inline void Scene_StoreU16(u16 *field, s32 value)
{
    /* FAKEMATCH: through the inline the value is a word constant built after the field's address (movs), where a halfword constant stored directly loads from the literal pool (differ 24). */
    *field = value;
}

// Room 8's setup. Entered at entrance 4 (not in the Japanese edition), it
// sets the party state's pair at 0x260 to the room whose flag is set.
void Func_020035d8(void)
{
    struct ObjectRuntime *object = (struct ObjectRuntime *)Object_GetById(64);
    s32                   scene;

    Func_020041c8(137);
    Func_020041d0();
    Scene_Call41d8(0, 524, 8, 9);
    if (object != NULL)
        Func_02004090(64, 0, 0);
    object = (struct ObjectRuntime *)Object_GetById(10);
    object->scaleX = 0xb333;
    object->scaleY = 0xb333;
    object->radius = 8;
    Func_02002d40();
    Scheduler_AddOrUpdateCallback(Func_02001e58, 0x480);
    if (gPartyState.entrance == 2)
    {
        GameFlag_ClearBit(0xa29);
        GameFlag_SetBit(0xa2a);
        GameFlag_ClearBit(0xa2b);
    }
#if !defined(TLA_EDITION_JA)
    else if (gPartyState.entrance == 4)
    {
        if (GameFlag_Test(0xa29))
        {
            /* FAKEMATCH: each one-pass block ends a scheduling block after the scene id's load, which the native places before the field offset (plainly the offset comes first, differ 19 with the tails cross-jumped differently). */
            do
            {
                scene = (s32)&SceneId_Ov661Room1;
            } while (0);
            PARTY_STATE_U16(0x260) = scene;
            Scene_StoreU16(&PARTY_STATE_U16(0x262), 1);
        }
        else if (GameFlag_Test(0xa2a))
        {
            do
            {
                scene = (s32)&SceneId_Ov661Room8;
            } while (0);
            PARTY_STATE_U16(0x260) = scene;
            Scene_StoreU16(&PARTY_STATE_U16(0x262), 2);
        }
        else if (GameFlag_Test(0xa2b))
        {
            do
            {
                scene = (s32)&SceneId_Ov661Room9;
            } while (0);
            PARTY_STATE_U16(0x260) = scene;
            Scene_StoreU16(&PARTY_STATE_U16(0x262), 2);
        }
    }
#endif
}

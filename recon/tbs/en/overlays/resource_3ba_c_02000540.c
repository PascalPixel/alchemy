/* NONMATCHING: 752 bytes, candidate 772, 305 differing halfwords, 155
 * halfword edits (2026-09-25). KorosseoKawa_Func02000540, meant for
 * FIELD/KOROSSEO_KAWA/F_00540.C as a single-overlay unit binding its names
 * at their runtime addresses (an import veneer's listing offset plus
 * 0x8000). Remaining: Fresh typed actor and map sequence reconstructed from
 * disassembly; dimensions placed beside their first map calls.
 * WALL: Twenty-byte overlength and actor/map-call register lifetimes. */
#include "TYPES.H"
#include "FIELD_EVENT.H"

extern u32 Data_02000240[];
extern s32 Engine_AllocateBlock(s32 slot, s32 size);
extern void Engine_ObjectCommitPosition(struct FieldActor *object);
extern void ObjectDispatch_InitFromTable4WithArgumentFar(s32 *table, struct FieldActor *object);
extern void Engine_MapCopyCellAttributes(s32, s32, s32, s32, s32, s32);
extern void Engine_MapCopyCells(s32, s32, s32, s32, s32, s32);

void KorosseoKawa_Func02000540(void)
{
    s32 selector;
    s32 result;
    s32 width;
    s32 height;
    s32 x;
    struct FieldActor *actor;
    struct FieldActor *opponent;
    struct FieldActor *object;

    selector = Data_02000240[125];
    actor = Engine_ActorGet(selector);
    opponent = Engine_ActorGet(12);
    Engine_GameFlagSet(0x302);
    Engine_EventBegin();
    Engine_ActorSetAnimation(selector, 8);
    Engine_EventWait(6);
    opponent->speed = 0x8000;
    opponent->acceleration = 0x3333;
    Engine_AudioPlayCue(239);
    Engine_ObjectSetAnimation(opponent, 2);
    Engine_ObjectSetPosition(opponent, opponent->x.fixed - 0x300000, 0, opponent->z.fixed);
    Engine_EventWait(6);
    Engine_ActorSetAnimation(selector, 2);
    result = Engine_AllocateBlock(27, 0xccc);
    ObjectDispatch_InitFromTable4WithArgumentFar((s32 *)(result + 0x1e0), opponent);
    Engine_ActorSetSpeed(selector, 0x4ccc, 0x3333);
    Engine_ObjectSetPosition(actor, actor->x.fixed - 0x180000, 0, actor->z.fixed);
    Engine_ActorWaitForMove(selector);
    Engine_ActorSetAnimation(selector, 1);
    Engine_ObjectCommitPosition(opponent);
    Engine_ObjectSetAnimation(opponent, 1);
    Engine_AudioPlayCue(0x120);
    Engine_AudioPlayCue(213);
    Engine_EventWait(15);
    Engine_EventEnd();
    height = 34;
    width = 7;
    Engine_MapCopyCellAttributes(37, 7, 1, 4, height, width);
    Engine_MapCopyCellAttributes(36, 7, 1, 4, 37, width);
    result = Engine_GameFlagIsSet(0x301);
    if (result != 0) {
        Engine_EventBegin();
        Engine_CameraSetSpeed(0x20000, 0x4000);
        Engine_CameraMoveTo(0x2280000, -1, 0xc80000, 1);
        width = 38;
        Engine_CameraWaitForMove();
        Engine_MapCopyCells(96, 29, 1, 3, height, width);
        Engine_EventWait(3);
        Engine_MapCopyCells(97, 29, 1, 3, height, width);
        Engine_EventWait(3);
        Engine_MapCopyCells(98, 29, 1, 3, height, width);
        Engine_EventWait(3);
        Engine_MapCopyCells(99, 29, 1, 3, height, width);
        Engine_EventWait(3);
        Engine_MapCopyCells(100, 29, 1, 3, height, width);
        Engine_EventWait(15);
        Engine_EventEnd();
    } else {
        Engine_GameFlagSet(0x301);
        Engine_EventBegin();
        Engine_CameraSetSpeed(0x20000, 0x4000);
        Engine_CameraMoveTo(0x2580000, -1, 0xc80000, 1);
        Engine_CameraWaitForMove();
        object = Engine_ActorGet(13);
        object->motion_flags = (u8)result;
        object->speed = 0xcccc;
        object->acceleration = 0x6666;
        Engine_ObjectSetPosition(object, object->x.fixed, 0x80000, object->z.fixed);
        width = 38;
        Engine_ObjectSetAnimation(object, 3);
        Engine_MapCopyCells(96, 29, 1, 3, height, width);
        Engine_EventWait(3);
        Engine_MapCopyCells(97, 29, 1, 3, height, width);
        Engine_EventWait(3);
        Engine_MapCopyCells(98, 29, 1, 3, height, width);
        Engine_EventWait(3);
        Engine_MapCopyCells(99, 29, 1, 3, height, width);
        Engine_EventWait(3);
        Engine_MapCopyCells(100, 29, 1, 3, height, width);
        object = Engine_ActorGet(14);
        object->motion_flags = (u8)result;
        object->speed = 0xcccc;
        object->acceleration = 0x6666;
        Engine_ObjectSetPosition(object, object->x.fixed, 0x200000, object->z.fixed);
        Engine_ObjectCommitPosition(object);
        Engine_EventWait(15);
        Engine_EventEnd();
        Engine_MapCopyCellAttributes(43, 12, 1, 1, 41, 12);
    }
}

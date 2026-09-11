#include "types.h"
#include "scene.h"
#include "abi/overlays/scene/field/entry_cue.h"

extern u8 Value_000025b8;
extern u8 Value_000025dc;
extern s16 gOv[];
extern s16 gCell[];

/*
 * Scene script for overlay resource_3c1: data table getters, two actor cue
 * branches, the indexed effect setups, and the entry state.
 */

static __inline__ void SetOffset(s32 actor, s32 axis, s32 offset)
{
    Field_Place(actor, axis, offset);
}

static __inline__ void SetOffset2(s32 actor, s32 axis, s32 offset)
{
    Field_Place2(actor, axis, offset);
}

/*
 * Returns the in-image table address 0x020082f0, loaded and returned
 * without being dereferenced. The eight-byte owner includes its one pool
 * word, which sits past the bx lr.
 */
u8 *SceneData_GetScriptTable(void)
{
    return (u8 *)0x020082f0;
}

s32 SceneData_ReturnZero(void)
{
    return 0;
}

/*
 * Returns the in-image table address 0x020083c8, loaded and returned
 * without being dereferenced. The eight-byte owner includes its one pool
 * word, which sits past the bx lr.
 */
u8 *SceneData_GetMessageTable(void)
{
    return (u8 *)0x020083c8;
}

s32 SceneData_SelectActorTableByFlag96f(void)
{
    if (Field_Check(0x96f) != 0) {
        return 0x020084e0;
    }
    return 0x020083f0;
}

void Scene_RunActorCue25b8Branch(s32 obj)
{
    s32 cue = (s32)&Value_000025b8;
    Field_Check2(cue);
    Field_Apply(obj, 0);
    if (Field_Apply2(0, 0) == 0) {
        Field_Do(10);
        Field_Do2(cue + 1);
    } else {
        Field_Do3(cue + 2);
    }
    Field_Apply3(obj, 0);
}

void Scene_RunActorCue25dcBranch(s32 obj)
{
    s32 cue = (s32)&Value_000025dc;
    Field_Check3(cue);
    Field_Apply4(obj, 0);
    if (Field_Apply5(0, 0) == 0) {
        Field_Do4(10);
        Field_Do5(cue + 1);
    } else {
        Field_Do6(cue + 2);
    }
    Field_Apply6(obj, 0);
}

void Scene_RunLayoutStepThenSet201(void)
{
    s32 width = 4;
    s32 height = 9;

    Field_SetRect(25, 9, 1, 1, width, height);
    Field_Do7(0x201);
}

void Effect_ConfigureIndexedEffect85e8(void)
{
    u8 *work = *(u8 **)0x03001ebc;
    s32 no = *(s16 *)(work + 364);
    u16 x = gOv[no * 2];
    u16 y = gOv[no * 2 + 1];

    Field_Run(158);
    Field_Run2(0x020085e8, x, y);
    SetOffset(0, 0, -16);
    *(s32 *)(*(u8 **)0x03001ebc + 456) = 16;
    Field_Run3(no);
}

void Effect_ConfigureIndexedEffect85fe(void)
{
    u8 *work = *(u8 **)0x03001ebc;
    s32 no = *(s16 *)(work + 364);
    u16 x = gOv[no * 2];
    u16 y = gOv[no * 2 + 1];

    Field_Run4(158);
    Field_Run5(0x020085fe, x, y);
    SetOffset2(0, 0, -16);
    *(s32 *)(*(u8 **)0x03001ebc + 456) = 16;
    Field_Run6(no);
}

s32 SceneData_SelectTable8614ByFlag96f(void)
{
    if (Field_Check4(0x96F) != 0) {
        return 0x02008758;
    }
    return 0x02008614;
}

s32 State_InitEntryWorkspaceAndFlag96f(void)
{
    u8 *work;

    /* Record arrival on map 90, then publish the initial scene phase/timer. */
    if (gCell[225] == 90) {
        Field_Run7(0x96f);
    }

    work = *(u8 **)0x03001ebc;
    *(s32 *)(work + 448) = 256;
    *(s32 *)(work + 456) = 24;

    /* The dressing sequence and cue are unlocked by the shared event flag. */
    if (Field_Check5(0x201) != 0) {
        Field_Run8();
        Field_Run9(16, 4);
    }
    return 0;
}

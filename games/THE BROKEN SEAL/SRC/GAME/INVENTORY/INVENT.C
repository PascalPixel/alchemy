#include "TYPES.H"
#include "OBJECT_LOOKUP.H"
#include "SCENE.H"
#include "SYSTEM.H"
#include "UI.H"

struct Child_08091c44 {
    u8 padding[36];
    u8 value;
};

struct Object_08091c44 {
    u8 padding[80];
    struct Child_08091c44 *child;
    u8 active;
};

void UiWork_FinalizeEntityMatchingLocalizedIdFar(s32);
void Object_SetModeById(s32, s32);
void Object_WaitUntilChildValueDiffers(s32, s32);

struct Entry_08091c7c {
    u8 unknown_00[10];
    u16 value_0a;
    u8 unknown_0c[2];
    u16 value_0e;
};

struct Runtime_08091c7c {
    u8 unknown_000[0x1f4];
    s32 first_1f4;
    struct Entry_08091c7c *second_1f8;
    struct Entry_08091c7c *third_1fc;
    u8 unknown_200[0xac2];
    s16 value_cc2;
    s16 value_cc4;
};

extern struct Runtime_08091c7c *gWork;
extern volatile s32 gKeyState;
s16 *BattleAction_FindDescriptor(s32);
s32 Inventory_RequestMode(s32, s32, s32, s32);
void UiWork_FinalizePendingCoreFar(void);

s32 Menu_RunConfirmSelectionFar(s32 value, s32 x, s32 y, s32 z);

/* An empty routine after the inventory give-and-report code; nothing in
   the image calls it. */
void Inventory_ReservedNoOp(void)
{
}

void Object_WaitUntilChildValueDiffers(s32 object_id, s32 value)
{
    struct Object_08091c44 *obj = ObjectTable_Get(object_id);

    if (obj != 0 && obj->active == 1) {
        s32 cnt = 0;
        u8 *p = &obj->child->value;

        while (cnt <= 89) {
            WaitFrames(1);
            if (value != *p) {
                break;
            }
            cnt++;
        }
    }
}

s32 Inventory_PromptAndSetObjectMode(s32 id, s32 force)
{
    struct Runtime_08091c7c *rt = gWork;
    s32 v = *BattleAction_FindDescriptor(rt->first_1f4);
    struct Entry_08091c7c *ent0 = rt->second_1f8;
    struct Entry_08091c7c *ent1 = rt->third_1fc;
    s32 flag = 1;
    s32 ret;

    while (gKeyState != 0)
        WaitFrames(1);

    while (UiWork_IsCompleteFar() == 0)
        WaitFrames(1);

    WaitFrames(3);

    if (force == 0) {
        s32 sum = ent0->value_0e + ent0->value_0a;

        if (ent1 != 0) {
            s32 sum2 = ent1->value_0e + ent1->value_0a;
            if (sum < sum2)
                sum = sum2;
        }

        if (sum > 15)
            flag = 0;
    }

    ret = Inventory_RequestMode(flag, rt->value_cc2, rt->value_cc4, 0);
    if (ret != 0) {
        Object_SetModeById(id, 4);
        UiWork_FinalizeEntityMatchingLocalizedIdFar(v);
        UiWork_FinalizePendingCoreFar();
        Object_WaitUntilChildValueDiffers(id, 4);
    } else {
        Object_SetModeById(id, 3);
        UiWork_FinalizeEntityMatchingLocalizedIdFar(v);
        UiWork_FinalizePendingCoreFar();
        Object_WaitUntilChildValueDiffers(id, 3);
    }

    return ret;
}

s32 Object_CallSpawnRoutineAtOrigin(s32 value)
{
    return Menu_RunConfirmSelectionFar(value, 0, 0, 0);
}

#include "OBJECT_LOOKUP.H"
#include "TYPES.H"
#include "GLOBAL_CELLS.H"

s32 Object_GetById(u32);

extern s32 Data_02000240[];
#define PARTY_STATE Data_02000240
extern u8 Data_03001ebc[];

s32 WaitFrames(s32);
s32 Func_0800430c(void);
#define Scheduler_EnableUnmaskedOverlayCallbacks Func_0800430c
s32 Func_080043e0(void);
#define Scheduler_DisableOverlayCallbacks Func_080043e0
void *Runtime_AllocateBlock(s32 arg0, s32 arg1);
void Func_08091660(void);
#define Battle_InitializeRenderObject Func_08091660
void Func_080936a0(s32, s32);
#define BattleFx_ScheduleRatioTransition Func_080936a0

void BattleFx_UpdateObjectVisibilityBounds(void)
{
    s32 object;
    s32 x;
    s32 y;
    s32 left;
    s32 right;
    s32 top;
    s32 bottom;
    u32 id;

    object = PARTY_STATE[125];
    Object_GetById(object);
    object = *(s32 *)(*(u8 **)Data_03001ebc + 480);
    x = *(s32 *)(object + 8);
    left = x + 0xFEC00000;
    right = x + 0x01400000;
    y = *(s32 *)(object + 16);
    top = y + 0xFDA80000;
    bottom = y + 0x01900000;

    id = 8;
    do {
        object = ObjectTable_Get(id);

        if (object != 0) {
            s32 ox = *(s32 *)(object + 8);
            s32 oy = *(s32 *)(object + 16);

            if (ox < left || ox > right ||
                oy < top || oy > bottom) {
                *(u8 *)(object + 84) = 0;
            } else {
                *(u8 *)(object + 84) = 1;
            }
        }
        id++;
    } while (id <= 65);
}

void BattleFx_RunVisibilityTransition(void)
{
  if ((*(s16 *)(((u8 *)Runtime_AllocateBlock(0x1B, 0xCCC)) + 0x19E)) == 3)
  {
    Scheduler_EnableUnmaskedOverlayCallbacks();
    BattleFx_UpdateObjectVisibilityBounds();
    Battle_InitializeRenderObject();
    BattleFx_ScheduleRatioTransition(0x9D89, 6);
    if ((*((volatile u32 *) ADDR_03001AE8)) & 0x200)
    {
      do
      {
        WaitFrames(1);
      }
      while ((*((volatile u32 *) ADDR_03001AE8)) & 0x200);
    }
    BattleFx_ScheduleRatioTransition(0x10000, 6);
    Scheduler_DisableOverlayCallbacks();
  }
}

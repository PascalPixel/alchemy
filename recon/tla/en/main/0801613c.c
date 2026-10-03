#include "RUNTIME_MEM.H"
/*
 * Draft: SaveState_ReleaseWorkspace does not yet match; it does not compile against ⚓️'s headers yet.
 * Links as recon/tla/raw/08016054.s.
 */
#include "TYPES.H"
#include "SCENE.H"
#include "FIXED_MATH.H"
#include "GLOBAL_CELLS.H"
#include "RUNTIME_INTERFACES.H"
#include "DMA.H"
#include "FLASH.H"
#include "SAVE_STATE.H"

struct Work_08005868 {
    u8 unknown_00[64];
    s32 data;
};

u32 SaveState_WriteWorkspaceSlot(code)
{
    s32 *param = (s32 *)Flash_Handler0;
    s32 result;
    struct Work_08005868 *work;
    s32 value;

    work = *(struct Work_08005868 **)((u32)&Data_03001f1c);
    value = code & 0xFFFF;
    if ((_call_via_r3(value, (s32)&work->data,
                       (s32)param, *param) << 0x10) != 0) {
        return 1U;
    }
    result = Flash_VerifySector(value, (s32)&work->data);
    return (u32)((0 - result) | result) >> 0x1F;
}

typedef u16 (*Callback_08005904)(u16);

#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

typedef void (*InterruptHandler)(void);

void Runtime_SetIrqHandler(s32, s32, InterruptHandler);

u32 SaveState_ReleaseWorkspace(void)
{
  int fn;
  long long id;
  long long tmp;
  int arg;
  unsigned int no;
  fn = 0;
  id = (tmp = (no = 0x33));
  arg = 0;
  Runtime_SetIrqHandler(5, arg, (InterruptHandler)fn);
 return Runtime_ReleaseHeapBlock(id);
}

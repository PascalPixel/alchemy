#include "TYPES.H"
#include "GLOBAL_CELLS.H"
#include "TBS_EDITION.H"
extern u8 Data_03001e8c[];
extern u8 Data_03001ae8[];
extern u8 Data_03001c94[];
extern u8 gKeysPressedLatch[];

s32 AudioCommand_GetStateByteFar();
s32 UiWork_CheckCancelByInput(void *obj)
{
  int zero;
  s32 flag;
  flag = 0;
  if (((*((u8 *)(((u8 *)(*((void **)((u32)&Data_03001e8c)))) + RENDER_BUSY_OFS))) != 0) && (AudioCommand_GetStateByteFar() == 0))
  {
    flag = 1;
  }
  zero = 0;
  if ((*((s32 *)((u32)&Data_03001ae8))) & 0x303)
  {
    flag = 1;
  }
  if (flag != zero)
  {
    *((s16 *)(((u8 *)obj) + 0x14)) = zero;
    return 1;
  }
  return zero;
}

s32 UiWork_CheckCancelByModeInput(void *obj)
{
  void *p;
  s32 tmp;
  unsigned char zero;
  s32 key;
  s32 flag;
  void *work;
  p = *((void **)((u32)&Data_03001e8c));
  work = p;
  flag = 0;
  if (((*((u8 *)(((u8 *)work) + RENDER_BUSY_OFS))) != 0) && (AudioCommand_GetStateByteFar() == 0))
  {
    flag = 1;
  }
  key = (tmp = *((s32 *)((u32)&Data_03001c94)));
  zero = 0;
  if ((*((u8 *)(work + RENDER_MODE_OFS))) != zero)
  {
    key = *((s32 *)((u32)&gKeysPressedLatch));
  }
  if (0x303 & key)
  {
    flag = 1;
  }
  if (flag != 0)
  {
    *((s16 *)(((u8 *)obj) + 0x14)) = zero;
    return 1;
  }
  return 0;
}

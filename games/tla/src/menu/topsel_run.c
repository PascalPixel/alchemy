#include "types.h"
#include "system.h"
#include "ram_buffer.h"

void PartyStatus_CreateWindow(s32 flags);
s32 Menu_SelectTopEntry(s32);
s32 Window_CloseAndRelease(void);
s32 DjinnMenu_Open(void);
s32 Object_GetTriggerTileAheadOfCurrentFar(void);
s32 PsynergyMenu_RunFar(void);
s32 Func_080f8000(void);
s32 Func_080f8010(void);
void PartyStatus_OpenAndWait(void);

s32 Menu_RunTopSelection(void) {
  s32 ret;
  s32 selection;
  u8 *state;

  state = Ram_HeapSlots->eventWork;
  selection = 0;

loop:
  PartyStatus_OpenAndWait();
  selection = Menu_SelectTopEntry(selection);
  ret = Window_CloseAndRelease();

  switch (selection) {
  case 0:
    ret = Object_GetTriggerTileAheadOfCurrentFar();
    if (ret == 0)
      ret = 0xff;
    *(u16 *)(state + 0x162) = ret;
    break;
  case 1:
    ret = PsynergyMenu_RunFar();
    if (ret == -1)
      goto loop;
    break;
  case 2:
    ret = DjinnMenu_Open();
    if (ret != 0)
      goto loop;
    break;
  case 3:
    ret = Func_080f8000();
    if (ret == -1)
      goto loop;
    break;
  case 4:
    ret = Func_080f8010();
    if (ret == -1)
      goto loop;
    break;
  default:
    break;
  }

  return ret;
}

void PartyStatus_OpenAndWait(void) {
  PartyStatus_CreateWindow(1);
  WaitFrames(1);
}

#include "types.h"
#include "object_runtime.h"
#include "adventure.h"
#include "ram_buffer.h"
#include "flag_call.h"
#include "text/msg_ids.h"

extern u8 gData_02006338[];
extern u8 gData_02006500[];
extern u8 gData_02006598[];
extern u8 gData_02006700[];
extern u8 gData_02006790[];

struct ObjectRuntime *Object_GetById(s32 id);
void FieldMessage_Show(s32 message, s32 position);
void Message_CloseFinishedWindows(void);
void Func_02003bf0(void);
void Func_020049bc(void);
void Func_02004b28(void);
void Func_02004ba8(s32 a, s32 b);
void Func_02004da8(void);
void Func_02004db0(void);
s32 Func_02004e10(void);

u8 *Func_020003f8(void) {
  switch (gAdventure.entrance) {
  case 64:
    return gData_02006500;
  case 65:
    return gData_02006598;
  case 68:
    if (!GameFlag_Test(0x8de))
      return gData_02006700;
  case 77:
    return gData_02006790;
  default:
    return gData_02006338;
  }
}

/// The world map's render mode 8 start: with flag 0x147 a blank message is
/// shown first, the event work's counters are cleared.
void Func_02000458(void) {
  u8 *work = Ram_HeapSlots->eventWork;

  gAdventure.renderMode = 8;
  if (Flag_IsSet(0x147)) {
    Func_02004da8();
    Func_02004ba8(153, 4);
    FieldMessage_Show((s32)MsgBlank2, 4);
  }
  if (Object_GetById(Func_02004e10()) != NULL)
    Func_02004b28();
  Func_02003bf0();
  if (Flag_IsSet(0x147))
    work[3387] = 0;
  else
    work[3387] = 1;
  if (Flag_IsSet(0x147)) {
    Message_CloseFinishedWindows();
    Func_02004db0();
  }
  Func_020049bc();
  {
    s32 zero;

    /* FAKEMATCH: the halfword counters take an s32 zero assigned inside the
     * first store; a halfword constant loads from the pool, and a zero set
     * first keeps 346 out of r3 (differ 27 to 4). */
    *(u16 *)(work + 346) = zero = 0;
    *(u16 *)(work + 350) = zero;
    *(u16 *)(work + 348) = zero;
    *(u16 *)(work + 356) = zero;
    *(u16 *)(work + 358) = zero;
  }
}

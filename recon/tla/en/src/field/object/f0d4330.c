// Sets the message voice from the object `key` names (The Broken Seal's
// Message_SetVoiceFromActor): the voice sound from its value and the
// countdown from the event mode's table; 0x80000000 clears both.
#include "types.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "window.h"

s32 ObjectTable_ReadActiveValue(s32 key);
s32 Func_080d1eac(s32 id);
extern const u8 gData_080f330c[];

void Func_080d4330(s32 key) {
  struct WindowWork *work = (struct WindowWork *)Ram_HeapSlots->windowTiles;
  u8 value;
  s32 id;

  if (key == 0x80000000) {
    *(s16 *)&work->speakerVoiceSound = (value = 0);
    *(s16 *)&work->speakerVoiceCountdown = value;
  } else {
    id = Func_080d1eac(ObjectTable_ReadActiveValue(key));
    value = gData_080f330c[gMainRoutineWork.eventMode];
    *(s16 *)&work->speakerVoiceSound = id;
    *(s16 *)&work->speakerVoiceCountdown = value;
  }
}

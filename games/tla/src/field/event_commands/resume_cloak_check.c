#include "types.h"
#include "task.h"

extern u8 gMainRoutineWork[];
void FieldEffect_CheckCloakTile(void);

void FieldEffect_ResumeCloakCheck(void) {
  u8 *state;
  s8 *flag;

  state = gMainRoutineWork;
  flag = (s8 *)state + 0x26d;
  if (*flag != 0)
    Task_Add(FieldEffect_CheckCloakTile, 0x480);
}

#include "types.h"
#include "adventure.h"

extern const u8 gData_0200082c[];
extern const u8 gData_020008a4[];
extern const u8 gData_02000a0c[];
extern const u8 gData_02000b74[];
extern const u8 gData_02000cf4[];

const u8 *Func_0200004c(void) {
  if (gAdventure.entrance == 99)
    return gData_020008a4;
  if (gAdventure.entrance == 98)
    return gData_02000a0c;
  if (gAdventure.entrance == 97)
    return gData_02000b74;
  if (gAdventure.entrance == 96)
    return gData_02000cf4;
  return gData_0200082c;
}

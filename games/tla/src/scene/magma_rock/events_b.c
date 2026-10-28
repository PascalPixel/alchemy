#include "types.h"
#include "adventure.h"
#include "scene_ids.h"

struct Trigger;

extern const u8 gData_0200460c[];
extern const u8 gData_02004720[];
extern const u8 gData_020047e0[];
extern const u8 gData_0200487c[];
extern const u8 gData_02004930[];
extern const u8 gData_02004a5c[];

const struct Trigger *Func_020021e8(void) {
  if (gAdventure.scene == (s32)&SceneId_Ov691Room2) {
    if (gAdventure.entrance <= 3)
      return (const struct Trigger *)gData_02004720;
    if (gAdventure.entrance <= 8)
      return (const struct Trigger *)gData_020047e0;
    if (gAdventure.entrance <= 12)
      return (const struct Trigger *)gData_0200487c;
    return (const struct Trigger *)gData_02004930;
  }
  if (gAdventure.scene == (s32)&SceneId_Ov691Room3)
    return (const struct Trigger *)gData_02004a5c;
  return (const struct Trigger *)gData_0200460c;
}

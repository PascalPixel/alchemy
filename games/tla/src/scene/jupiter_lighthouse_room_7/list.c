#include "types.h"
#include "iwram_call.h"
#include "actor.h"
#include "event_inline.h"

extern const u8 gData_020031c4[];
extern s32 gData_02003594;
extern s16 gData_020035fe;
extern struct Actor *gData_02003600[];
extern u16 gData_02003702;
void Func_02000060(s32 value);
void Func_020000b4(void);
void Func_02000100(void);
void Func_02002b54(s32 x, s32 z, s32 a, s32 b);
void Func_02002c54(s32 x, s32 z, s32 a, s32 b);
void Func_02001a48(const u8 *data, s32 a, s32 b);
void Func_02002bec(void);

void Func_02001eb8(s32 id, s32 value) {
  struct Actor *object = Actor_Get(id);

  if (object != NULL) {
    object->action = value;
    gData_02003600[gData_020035fe] = object;
    gData_020035fe++;
  }
}

void Func_02001ee8(u16 value) { gData_02003702 = value; }

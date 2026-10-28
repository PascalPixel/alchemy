#include "types.h"
#include "actor.h"
#include "adventure.h"

struct Actor *Actor_GetById(s32 id);
void Actor_StartScriptFar(struct Actor *object,
                                  const void *script);

extern const u8 gData_080f34e4[];
extern const u8 gData_080f3428[];
extern const u8 gData_080f350c[];
extern const u8 gData_080f35c8[];
extern const u8 gData_080f373c[];
extern const u8 gData_080f3750[];
extern const u8 gData_080f34e8[];
extern const u8 gData_080f3614[];
extern const u8 gData_080f3764[];
extern const u8 gData_080f3500[];
extern const u8 gData_080f34f4[];

void Actor_SetActionCallback(struct Actor *object, s32 kind) {
  switch (kind) {
  case 1:
    kind = (s32)gData_080f34e4;
    break;
  case 2:
    kind = (s32)gData_080f3428;
    break;
  case 3:
    kind = (s32)gData_080f350c;
    break;
  case 4:
    kind = (s32)gData_080f35c8;
    break;
  case 5:
    kind = (s32)gData_080f373c;
    break;
  case 6:
    object->watchedObject = Actor_GetById(gAdventure.currentOwner);
    kind = (s32)gData_080f3750;
    break;
  case 7:
    kind = (s32)gData_080f34e8;
    break;
  case 8:
    kind = (s32)gData_080f3614;
    break;
  case 9:
    kind = (s32)gData_080f3764;
    break;
  case 10:
    kind = (s32)gData_080f3500;
    break;
  case 11:
    kind = (s32)gData_080f34f4;
    break;
  }
  Actor_StartScriptFar(object, (const void *)kind);
}

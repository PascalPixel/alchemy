#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "event_inline.h"

extern const u8 gData_020017d8[];
extern const u8 gData_020017a4[];
extern const u8 gData_0200171c[];
void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
void Func_020013f0(s32 a, s32 b, s32 c);
void Func_02001440(s32 a, s32 b);
void Func_020012e0(void);
void Func_02000f38(void);

void Func_02000d40(void) {
  s32 count = 8;

  /* FAKEMATCH: ⚓️ sets the second argument first */
  asm("" : "+l"(count));
  Func_02001440(13, count);
}

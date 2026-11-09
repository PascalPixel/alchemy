#include "types.h"
#include "ram_buffer.h"
#include "text/msg_ids.h"
#include "actor.h"
#include "event_inline.h"

extern const u8 gData_02000318[];
extern const u8 gData_02000270[];
void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
void Func_020001b4(void);
void Func_020001bc(void);
void Func_0200020c(void);

const u8 *Func_0200004c(void) {
  if (GameFlag_Test(2219))
    return gData_02000318;
  return gData_02000270;
}

#include "trial_road.h"

#include "iwram_call.h"
#include "io_reg.h"

extern s32 gData_02007e20;
extern s32 gData_02007e24;
extern u8 gData_02007e28[];

void Func_02001f08(void);

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static __inline__ void Words_Copy(void *destination, const void *source,
                                  s32 size) {
  Iwram_CopyWords(destination, source, size);
}

void Func_02001fc4(s32 mode) {
  gData_02007e24 = 1;
  if (mode != 2)
    Words_Copy(gData_02007e28, (void *)&BG_PLTT_COLOR(5, 0), 32);
  if (mode == 1) {
    /* FAKEMATCH(temporary): a local holding the zero builds it in r2 and the
     * counter address in r3, as overlay 699's twin does; as
     * gData_02007e20 = 0, the instructions differ. */
    s32 zero = 0;

    gData_02007e20 = zero;
    Task_Add(Func_02001f08, 0x480);
  }
}

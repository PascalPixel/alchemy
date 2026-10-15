#include "types.h"
#include "iwram_call.h"
#include "serial_runtime.h"

void BattlePres_ClearLinkPayload(void) {
  Iwram_ClearWords(gSerialTransfer.payload, 16);
}

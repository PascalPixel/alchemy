#include "types.h"
#include "iwram_call.h"
#include "serial.h"

void BattleStaging_ClearLinkPayload(void) {
  Iwram_ClearWords(gSerialTransfer.payload, 16);
}

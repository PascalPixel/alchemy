#include "types.h"
#include "actor_script_view.h"

s32 Field_CheckConfiguredKeys(void);

/// SC_CMD_25: checks the configured keys (Field_CheckConfiguredKeys) and steps
/// the script on. Returns 1.
s32 Field_CheckConfiguredKeysAndCount(struct ActorScriptView *interpreter) {
  Field_CheckConfiguredKeys();
  interpreter->scriptCursor++;
  return 1;
}

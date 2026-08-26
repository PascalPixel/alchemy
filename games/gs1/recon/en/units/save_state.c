#include "../main/save_state_family.h"

/* Ordered save-state translation unit: 080056cc through 08005c68. */

#include "../main/080056cc.c"

#include "../../../src/save/state/select_write_slot.c"
#include "../../../src/save/state/write_workspace_slot.c"

#include "../main/080058ac.c"

#include "../../../src/save/state/erase_slot_sector.c"

#include "../main/08005920.c"
#include "../main/08005a78.c"

#include "../../../src/save/state/delete_record.c"
#include "../../../src/save/state/checksum_workspace.c"
#include "../../../src/save/state/find_latest_slot.c"

#include "../main/08005b64.c"

#include "../../../src/save/state/compare_bytes.c"
#include "../../../src/save/state/get_latest_sequence.c"

#include "../main/08005c68.c"

#include "../main/io_write_queue_family.h"
#include "../../../src/input/init_key_irq.c"

#include "../main/0800383c.c"
#include "../main/0800387c.c"
#include "../main/080038bc.c"
#include "../main/080038fc.c"
#include "../main/0800393c.c"
#include "../main/0800397c.c"
#include "../main/080039bc.c"
#include "../main/080039fc.c"
#include "../main/08003a3c.c"

#include "../main/08003a7c.c"

#include "../../../src/display/blend/update_transition.c"

#include "../../../src/display/blend/set_darken_target_16.c"
#include "../../../src/display/blend/set_darken_target_0.c"
#include "../../../src/display/blend/set_brighten_target_16.c"
#include "../../../src/display/blend/set_brighten_target_0.c"
#include "../../../src/display/blend/configure_transition.c"

#include "../../../src/display/blend/wait_for_transition.c"
#include "../../../src/graphics/reset_frame_state.c"
#include "../../../src/graphics/affine/build_matrix.c"
#include "../../../src/runtime/slot/push_entry.c"

#include "../main/08003e10.c"
#include "../main/08003e58.c"

#include "../main/serial_runtime_family.h"

/* Ordered serial/runtime translation unit: 08005d10 through 0800651c. */

#include "../main/08005d10.c"

#include "../../../src/link/serial/enable_transfer_timer.c"
#include "../../../src/link/serial/disable_serial_transfer.c"

#include "../main/08005ee0.c"
#include "../main/08005fcc.c"
#include "../main/080060e8.c"
#include "../main/0800615c.c"

#include "../../../src/link/serial/handle_transfer_interrupt.c"
#include "../../../src/link/serial/remove_serial_irq_handlers.c"
#include "../../../src/link/serial/wait_for_status_mask.c"

#include "../main/080063bc.c"
#include "../main/08006408.c"

#include "../../../src/link/serial/wait_for_transfer_a.c"
#include "../../../src/link/serial/wait_for_transfer_b.c"
#include "../../../src/link/serial/wait_for_transfers.c"
#include "../../../src/link/serial/get_active_transfers.c"

#include "../main/0800651c.c"

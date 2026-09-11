#include "types.h"
#include "scene.h"

/* runtime/idle_forever.c */
extern volatile u32 gIw;
void WaitFrames(u32);

void Runtime_IdleForever(void)
{
    for (;;) {
        (void)gIw;
        WaitFrames(1);
    }
}

/* runtime/dispatch/reserved_no_op_a.c */
/* Reserved ROM-to-IWRAM dispatch target; its complete body is `bx lr`. */
void RuntimeDispatch_ReservedNoOpA(void)
{
}

/* runtime/dispatch/reserved_no_op_b.c */
/* Reserved ROM-to-IWRAM dispatch target; its complete body is `bx lr`. */
void RuntimeDispatch_ReservedNoOpB(void)
{
}

/* runtime/dispatch/reserved_no_op_c.c */
/* Reserved ROM-to-IWRAM dispatch target; its complete body is `bx lr`. */
void RuntimeDispatch_ReservedNoOpC(void)
{
}

/* runtime/dispatch/reserved_no_op_d.c */
/* Reserved ROM-to-IWRAM dispatch target; its complete body is `bx lr`. */
void RuntimeDispatch_ReservedNoOpD(void)
{
}

/* runtime/dispatch/reserved_no_op_e.c */
/* Reserved ROM-to-IWRAM dispatch target; its complete body is `bx lr`. */
void RuntimeDispatch_ReservedNoOpE(void)
{
}

/* runtime/dispatch/reserved_status_zero.c */
/* Reserved ROM-to-IWRAM dispatch target with a constant success/status value. */
int RuntimeDispatch_ReservedStatusZero(void)
{
    return 0;
}

/* runtime/dispatch/no_op_hook.c */
/* Optional dispatch hook retained as a real two-byte no-op owner. */
void RuntimeDispatch_NoOpHook(void)
{
}

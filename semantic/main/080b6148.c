#include "types.h"

void Func_080030f8(s32 mode);

/*
 * Exchange a five-stage eight-halfword handshake with the other link buffer.
 * Each stage waits for both transfer-ready bits, retries mismatched payloads,
 * and fails after 25 consecutive frames without readiness.
 */
s32 Func_080b6148(void)
{
    s32 timeout;
    u8 *state;
    volatile u16 *peer;
    volatile u16 *local = (volatile u16 *)0x02002224;
    volatile u16 *linkStatus = (volatile u16 *)0x03001f64;

    state = *(u8 **)0x03001e74;
    timeout = 0;
    if (state[0x44] == 0) {
        goto block_57;
    }
    peer = (volatile u16 *)(unsigned long)
        (0x02002024 + (1 ^ state[0x50]) * 0x18);
    if (state[0x52] != 0) {
        goto block_54;
    }
    local[0] = 'e';
    local[1] = 'x';
    local[4] = 'T';
    local[5] = 'U';
    Func_080030f8(1);
loop_6:
    if ((3 & *linkStatus) != 3) {
        timeout += 1;
        if (timeout > 0x18) {
            goto block_54;
        }
        goto block_5;
    }
    timeout = 0;
    if (local[2] != peer[2]) {
        goto block_54;
    }
    if (local[3] != peer[3]) {
        goto block_54;
    }
    if ((local[0] == peer[0]) && (local[1] == peer[1]) && (local[4] == peer[4]) && (local[5] == peer[5])) {
        local[6] = 'r';
        local[7] = 'n';
loop_19:
        if ((3 & *linkStatus) != 3) {
            timeout += 1;
            if (timeout > 0x18) {
                goto block_54;
            }
            goto block_18;
        }
        timeout = 0;
        if (local[4] != peer[4]) {
            goto block_54;
        }
        if (local[5] != peer[5]) {
            goto block_54;
        }
        if ((local[6] == peer[6]) && (local[7] == peer[7])) {
            local[0] = 'E';
            local[2] = 'E';
            local[1] = 'X';
            local[3] = 'C';
loop_30:
            if ((3 & *linkStatus) != 3) {
                timeout += 1;
                if (timeout > 0x18) {
                    goto block_54;
                }
                goto block_29;
            }
            timeout = 0;
            if ((local[6] == peer[6]) && (local[7] == peer[7])) {
                if ((local[0] == peer[0]) && (local[1] == peer[1]) && (local[2] == peer[2]) && (local[3] == peer[3])) {
                    local[4] = 't';
                    local[5] = 'u';
loop_41:
                    if ((3 & *linkStatus) != 3) {
                        timeout += 1;
                        if (timeout > 0x18) {
                            goto block_54;
                        }
                        goto block_40;
                    }
                    timeout = 0;
                    if ((local[0] == peer[0]) && (local[1] == peer[1]) && (local[2] == peer[2]) && (local[3] == peer[3])) {
                        if ((local[4] == peer[4]) && (local[5] == peer[5])) {
                            local[6] = 'R';
                            local[7] = 'N';
loop_52:
                            if ((3 & *linkStatus) != 3) {
                                timeout += 1;
                                if (timeout > 0x18) {
                                    goto block_54;
                                }
                                goto block_51;
                            }
                            timeout = 0;
                            if ((peer[6] != 'r') || (peer[7] != 'n')) {
block_57:
                                return 0;
                            }
block_51:
                            Func_080030f8(1);
                            goto loop_52;
                        }
block_40:
                        Func_080030f8(1);
                        goto loop_41;
                    }
                    goto block_54;
                }
block_29:
                Func_080030f8(1);
                goto loop_30;
            }
block_54:
            return -1;
        }
block_18:
        Func_080030f8(1);
        goto loop_19;
    }
block_5:
    Func_080030f8(1);
    goto loop_6;
}

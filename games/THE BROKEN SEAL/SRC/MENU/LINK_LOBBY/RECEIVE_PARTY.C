#include "TYPES.H"
#include "SERIAL_RUNTIME.H"

s32 Main_08000170(u32 size);
void Main_08000178(s32 heap);
s32 Main_08000388(void);
s32 Main_080003a8(void);
u8 *Engine_OwnerGetState(s32 owner);
void Engine_TaskWait(s32 frames);
void Main_08015020(s32 id, u16 *buf);
void Main_08077000(s32 mode);

static __inline__ void Call2(void (*f)(s32, u16 *), s32 a0, u16 *a1)
{
    f(a0, a1);
}

#define LINK_STAT (*(volatile u16 *)0x03001f64)

/* Receives the linked player's three party records (0x154-byte transfers)
 * and then one 0x140-byte block, waiting on each transfer for at most 900
 * frames in total and 24 unready polls. Each party name gets the peer's
 * prefix (up to five characters) shifted in. Returns how many received
 * records have the byte at 0x12a set, or -1 when the link fails. */
s32 LinkLobby_ReceivePartyRecords(void)
{
    u16 buf[24];
    u32 size;
    s32 heap;
    s32 result;
    s32 slot;
    s32 timeout;
    s32 tries;
    u8 *rec;
    s32 ret;
    s32 n;
    s32 i;

    size = 0x154;
    heap = Main_08000170(size);
    result = 0;
    timeout = 900;
    slot = 0;
    goto next;
wait1:
    if (SERIAL_VALUE_B > size) {
        result = -1;
        goto done;
    }
    Engine_TaskWait(1);
    if (--timeout < 0 || (LINK_STAT & 3) != 3) {
        if (++tries > 24) {
                result = -1;
                goto done;
        }
    }
test1:
    if (Main_080003a8() != 0) {
        goto wait1;
    }
    if (SERIAL_VALUE_B != size) {
        result = -1;
        goto done;
    }
    if (rec[0x12a] != 0) {
        result++;
    }
    Engine_TaskWait(2);
    Call2(Main_08015020, 0x80c, buf);
    i = 0;
    if (buf[i] != 0) {
        do {
            i++;
            if (i > 4) {
                break;
            }
        } while (buf[i] != 0);
    }
    n = i;
    for (i = 14; i >= n; i--) {
        rec[i] = rec[i - n];
    }
    for (i = 0; i < n; i++) {
        rec[i] = buf[i];
    }
    rec[14] = 0;
    slot++;
next:
    if (slot > 2) {
        goto second;
    }
    rec = Engine_OwnerGetState(slot + 128);
    tries = 0;
    if ((ret = Main_08000388()) == -1) {
        goto failed;
    }
    goto test1;
wait2:
    ret = SERIAL_VALUE_B;
    if (ret > 0x140) {
        result = -1;
        goto done;
    }
    Engine_TaskWait(1);
    if (--timeout < 0 || (LINK_STAT & 3) != 3) {
        if (++tries > 24) {
            result = -1;
        goto done;
        }
    }
test2:
    if (Main_080003a8() != 0) {
        goto wait2;
    }
    if (SERIAL_VALUE_B != 0x140) {
        result = -1;
    } else {
        Engine_TaskWait(2);
    }
    goto done;
second:
    Main_08000178(heap);
    size = 0x140;
    heap = Main_08000170(size);
    Main_08077000(1);
    tries = 0;
    if ((ret = Main_08000388()) != -1) {
        goto test2;
    }
failed:
    result = ret;
done:
    Main_08000178(heap);
    return result;
}

/// @file
/// Battle synchronization signals over the link cable.

#include "types.h"
#include "edition.h"
#include "ram_buffer.h"
#include "serial_runtime.h"
#include "battle_work.h"

extern u16 gLinkPeerSignatures[][12];

/// In a link battle, exchanges the "EXEC" signal with the other console.
/// Returns zero if both agree within 30 frames, or -1 on timeout or disconnect.
/// Returns zero immediately outside a link battle.
s32 BattlePres_WaitSync(void) {
  struct BattleWork *work = Ram_HeapSlots->battleWork;
  u16 *peer;
  u16 *sig;
  s32 lost = 0;
  s32 i;

  if (work->twoSided != 0) {
    u32 other = work->linkSide ^ 1;

    peer = (u16 *)(gSerialPeerPayloads[other] + 4);
    sig = (u16 *)gSerialTransfer.payload;
    if (work->linkPaused == 0) {
      sig[0] = 'E';
      sig[1] = 'X';
      sig[2] = 'E';
      sig[3] = 'C';

      for (i = 0; i <= 29; i++) {
        if ((gLinkStatus & 3) != 3) // The two consoles lost their connection.
        {
          lost++;
          if (lost > 24)
            return -1;
        } else {
          lost = 0;
          if (sig[2] == peer[2] && sig[3] == peer[3])
            return 0;
        }
        WaitFrames(1);
      }
    }
    return -1;
  }
  return 0;
}

/// In a link battle, synchronizes successive signals after each action and
/// after the end-of-round processing.
/// "ex" "TU" → "rn" → "EXEC" → "tu" → "RN"
/// Returns -1 if disconnected, if an earlier signal disagrees with the peer,
/// or after 25 consecutive frames without a connection. Returns zero on
/// agreement, or outside a link battle.
s32 BattlePres_SyncTurn(void) {
  struct BattleWork *work = Ram_HeapSlots->battleWork;
  u16 *peer;
  u16 *sig;
  s32 lost = 0;
  /* FAKEMATCH: unused local, the native stack frame's twenty bytes that no
   * instruction touches. Without it differ 16 on tla-ja, tla-en and tla-de: no
   * sub sp, #20 and its add back, the movs r7, #0 scheduled earlier. */
  u16 spare[10];

  if (work->twoSided != 0) {
    peer = gLinkPeerSignatures[work->linkSide ^ 1];
    sig = (u16 *)gSerialTransfer.payload;
    if (work->linkPaused != 0)
      return -1;

    // "ex" "TU"
    sig[0] = 'e';
    sig[1] = 'x';
    sig[4] = 'T';
    sig[5] = 'U';
    WaitFrames(1);
    for (;;) {
      if ((gLinkStatus & 3) != 3) {
        if (++lost > 24)
          return -1;
      } else {
        lost = 0;
        if (sig[2] != peer[2] || sig[3] != peer[3])
          return -1;
        if (sig[0] == peer[0] && sig[1] == peer[1] && sig[4] == peer[4] &&
            sig[5] == peer[5])
          break;
      }
      WaitFrames(1);
    }

    // "rn"
    sig[6] = 'r';
    sig[7] = 'n';
    for (;;) {
      if ((gLinkStatus & 3) != 3) {
        if (++lost > 24)
          return -1;
      } else {
        lost = 0;
        if (sig[4] != peer[4] || sig[5] != peer[5])
          return -1;
        if (sig[6] == peer[6] && sig[7] == peer[7])
          break;
      }
      WaitFrames(1);
    }

    // "EXEC"
    sig[0] = 'E';
    sig[1] = 'X';
    sig[2] = 'E';
    sig[3] = 'C';
    for (;;) {
      if ((gLinkStatus & 3) != 3) {
        if (++lost > 24)
          return -1;
      } else {
        lost = 0;
        if (sig[6] != peer[6] || sig[7] != peer[7])
          return -1;
        if (sig[0] == peer[0] && sig[1] == peer[1] && sig[2] == peer[2] &&
            sig[3] == peer[3])
          break;
      }
      WaitFrames(1);
    }

    // "tu"
    sig[4] = 't';
    sig[5] = 'u';
    for (;;) {
      if ((gLinkStatus & 3) != 3) {
        if (++lost > 24)
          return -1;
      } else {
        lost = 0;
        if (sig[0] != peer[0] || sig[1] != peer[1] || sig[2] != peer[2] ||
            sig[3] != peer[3])
          return -1;
        if (sig[4] == peer[4] && sig[5] == peer[5])
          break;
      }
      WaitFrames(1);
    }

    // "RN": wait for the peer to move on from "rn".
    sig[6] = 'R';
    sig[7] = 'N';
    for (;;) {
      if ((gLinkStatus & 3) != 3) {
        if (++lost > 24)
          return -1;
      } else {
        lost = 0;
        if (peer[6] != 'r' || peer[7] != 'n')
          break;
      }
      WaitFrames(1);
    }
  }
  return 0;
}

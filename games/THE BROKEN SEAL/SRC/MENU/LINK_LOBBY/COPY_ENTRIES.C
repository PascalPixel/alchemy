/* The link lobby: the party members copied for the exchange. */
#include "LOBBY.H"

s32 SceneData_CopyUpToThreeEntries(u16 *dest)
{
    s32 cnt = Party_CountActiveOwners();
    if (cnt > 3) cnt = 3;
    if (cnt > 0) {
        s16 *p = gGameState;
        const u8 *src;
        s32 n;
        p += 252;
        src = (const u8 *)p;
        n = cnt;
        do {
            u8 c = *src++;
            if (dest != 0) { *dest = (u16)c; dest++; }
            n--;
        } while (n != 0);
    }
    if (dest != 0) *dest = 0x00ff;
    return cnt;
}

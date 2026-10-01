/* NONMATCHING: 2026-10-01 brief Wave2 CopyWords plain-source attempt.
 * Removing this one source device changes Graphics_ConvertBackgroundToBlueRamp.
 * First remaining difference: Graphics_ConvertBackgroundToBlueRamp: ldr	r3, .L0+12 => ldr	r0, .L0+12 (211/211 assembly lines).
 * Measured with the existing TBS agscc option set, EN edition; no option changes.
 * This reduced draft preserves the affected function and its declarations.
 * Production retains the measured device with its FAKEMATCH reason.
 */
#include "TYPES.H"
#include "IWRAM_CALL.H"
#include "BATTLE_EFX.H"
#include "CALLBACK_SCHEDULER.H"
#include "SYSTEM.H"
#include "FIXED_MATH.H"
#include "RESOURCE_IDS.H"
#include "RAM_BUFFER.H"

extern u8 gMapCellBuffer[];
extern u8 gBattleFxWork[];
void BattleFx_ArmBg2AffineHBlankDma(void);
void BattlePresentation_ProcessPendingGraphicsTransfer(void);
extern u8 *gBattleWork;

/* The two seven-byte-stride rectangle tables are plain, non-const arrays:
 * their element loads are ordered against the outgoing-argument stores at
 * each blit call site, which a const spelling would let float away. */
extern u8 CounterReveal_PanelX[];
extern u8 CounterReveal_PanelY[];
s32 BattleFx_BeginTiledCanvas(s32 mode);
void *Resource_GetTableEntry(s32 id);
u32 Resource_DecodeType01(const void *source, void *destination);
s32 BattleFx_EndCanvasLayer(void);
void **GetBattleObjectSlotFar(s32 member_id);
void EffectPosition_ApplyStepAndYOffset(s32 member_id, void *out);
void ObjectGroup_UpdateMembers(s32 member_id, s32 b, s32 c, s32 d, s32 e);
void Audio_PlayCue(s32 cue);
void Object_SetPosition(void *object, s32 a, s32 b, s32 c);
void BattleBackground_LoadFar(s32 a, s32 b, s32 c);

typedef s32 (*WordCopy)(void *, const void *, s32);


extern u8 gWorkSlot[];
#define PIXEL_BUFFER Ram_MapCellBuffer

/*
 * Battle-presentation sub-effect at 0x080cfef4, structurally related to the
 * "0x03001eec battle work" family already recovered in
 * games/THE BROKEN SEAL/SRC/BATTLE/EFFECT/MEMBER_ORBIT.C (owner 080ce85c) and
 * recon/tbs/en/main/080e7404.c: same heap_cache/work/canvas prologue,
 * the same raw-offset field-access spelling, and the same
 * BG2-affine + rectangle-blit (BattleEffect_LoadWork heap kinds 46/47) setup.
 *
 * Unlike the member-orbit effect this owner drives a single fixed party
 * member (its slot index lives at object+0x24, not an iterated array from
 * +0x24), runs a fixed 132-frame count rather than member_count*16+48, loads
 * a second graphics resource (ResourceId_RedCrescentSheetA) straight into OBJ VRAM at
 * 0x02010000, and drives BG2PC (0x04000052) directly rather than the BG2
 * reference-point registers.  Frames 88-99 additionally reveal a run of
 * glyph-style rectangles (width/height pairs 57x98, 99x69, 128x91, 128x59,
 * 122x29, 76x25) read from two seven-byte-stride tables at 0x080ee10c and
 * 0x080ee11a, selected by the same object+4 "kind" field used for the
 * rectangle-routine pick -- most plausibly a spinning number/counter reveal
 * rather than the orbiting-member sprite loop of the sibling effect.
 *
 * `status` and `_call_via_r3`/`Func_080072f4` follow the established
 * sibling reading: both addresses are `_call_via_rN` thunk slots
 * (the container-built bank at 0x080072e4) -- r3 for _call_via_r3, r4 for Func_080072f4
 * -- so each call is a genuine indirect call through a traced function
 * pointer, not a call to a real symbol at that address.
 */
void BattleFx_RunCounterReveal(void *object)
;

/* Convert the tiled background through a blue palette ramp. Each screen
 * column begins after its random delay; the row budget grows each frame.
 * The second palette bank preserves the converted pixels after the sweep.
 */
void Graphics_ConvertBackgroundToBlueRamp(void)
{
    s32 i, x, y, row, end, speed;
    s32 red, green, blue;
    u16 color;
    u8 *delays, *p, *pixel;
    u16 *palette;
    u32 delay_mask;
    palette = (u16 *)0x05000040;
    i = 0;
    do {
        s32 half;
        half = i / 2;
        *palette++ = (i << 10) | (half << 5) | half;
        i++;
    } while (i != 32);
    delays = *(u8 **)(gWorkSlot + 41 * 4);
    end = 0;
    speed = 16;
    Iwram_CopyWords(PIXEL_BUFFER, (void *)0x06008000, 0x7800);
    delay_mask = 63;
    p = delays;
    do {
        *p++ = Random16() & delay_mask;
    } while (p != delays + 256);
    row = 0;
    i = 0;
    do {
        end += speed / 4;
        speed++;
        while (row != end) {
            x = 0;
            do {
                y = row - delays[x];
                if (y < 0) goto next_pixel;
                if (y > 119) goto next_pixel;
                {
                    pixel = PIXEL_BUFFER + ((x & 7) + ((x / 8) << 6) + ((y & 7) << 3) + ((y / 8) << 11));
                    color = ((s16 *)0x05000000)[*pixel];
                    red = color & 31;
                    green = (color >> 5) & 31;
                    blue = (color >> 10) & 31;
                    if (red < green) red = green;
                    if (red < blue) red = blue;
                    *pixel = 63 - red;
                }
next_pixel:
                x++;
            } while (x != 256);
            row++;
        }
        Iwram_CopyWords((void *)0x06008000, PIXEL_BUFFER, 0x7800);
        WaitFrames(1);
        if (end > 248) break;
        i++;
    } while (i != 27);
    palette = (u16 *)0x050000c0;
    i = 0;
    do {
        s32 half;
        half = i / 2;
        *palette++ = (i << 10) | (half << 5) | half;
        i++;
    } while (i != 32);
    i = 0;
    do {
        PIXEL_BUFFER[i] += 64;
        i++;
    } while (i != 0x7800);
    Iwram_CopyWords((void *)0x06008000, PIXEL_BUFFER, 0x7800);
    WaitFrames(1);
}

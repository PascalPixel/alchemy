typedef unsigned char u8;
typedef unsigned short u16;
typedef int s32;
typedef unsigned int u32;

/*
 * resource_39e owner at 0x02001494, 2236 bytes: the overlay's second
 * ensemble beat -- 233 calls, one inline literal pool skipped by the
 * forward branch at 0x0200188e (the a010(20) call at the skip target
 * completes the instruction pair split around the pool), and TWO
 * Func_0808a070(0, 0) gates near the end. The gates settle this
 * overlay's story-flag chain:
 *   - opening sets flag 0x89a (Func_080770c8);
 *   - gate 1 ==0 arm joins with id 16 and sets flag 0x898 -- the flag
 *     the dispatcher at 0x020012e0 tests and 0x02001dbc later clears;
 *   - gate 1 else arm bumps the workspace+472 skip-beat counter
 *     (0x03001ebc) and runs gate 2: its ==0 arm joins with id 18 and
 *     also sets 0x898, its else arm bumps the counter again and sets
 *     flag 0x899 instead.
 * Both join paths share the machine-level tail at 0x02001cb4
 * (a188(id, 0, 20) with id carried in r0 from either arm -- spelled
 * out per-arm here). Midway: record 16's byte +90 gets bit 0 cleared
 * then set around a Func_0808a0d0 reposition, sound 158/159 with
 * Func_08009178(0x0200c77a/0x0200c790, 78, 13) text calls, the
 * slot-19 sequence (a0f0(19/20, 232<<16, 168<<16), record +12 =
 * 0xc0000, +60 = 0x80000000, +24 = 0xcccc, BAM +30 of the +80
 * sub-object = 0x8000, sound 124) identical to the 0x1160/0x1dbc
 * closers, and a Func_0808a098(12, 0x0200c638) matching 0x1dbc's.
 * Called 1x within this overlay.
 *
 * Complete owner: `push {r5, lr}` at 0x02001494 through `pop {r5} /
 * pop {r0} / bx r0` at 0x02001d42-0x02001d46, two inline pools plus
 * two trailing pool words ending 0x02001d4f; the next owner
 * (0x02001d50, `push {lr}`) follows. Not found by the structural
 * inventory walk (unindexed): reached only by bl
 * (bun tools/overlay_call_targets.ts resource_39e 1494 1d46, +2 rule).
 *
 * Uncertainty: callees and argument roles by shape; ids, dialogue ids
 * and constants transcribed. 0x0200c77a/0x0200c790/0x0200c638 are
 * overlay-image data pointers, targets not identified.
 */

extern void Func_0808a010(s32 frames);
extern s32 Func_0808a070(s32 arg0, s32 arg1);
extern u8 *Func_0808a080(s32 id);
extern void Func_0808a088(s32 id);
extern void Func_0808a090(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a098(s32 id, s32 arg1);
extern void Func_0808a0c8(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a0d0(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a0e8(s32 id);
extern void Func_0808a0f0(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a100(s32 id, s32 arg1);
extern void Func_0808a110(s32 id, s32 arg1);
extern void Func_0808a130(s32 id, s32 arg1);
extern void Func_0808a138(s32 id, s32 arg1);
extern void Func_0808a148(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a170(s32 dialogue_id);
extern void Func_0808a178(s32 id, s32 arg1);
extern void Func_0808a188(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a190(s32 id, s32 arg1);
extern void Func_0808a1b8(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a1e8(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a200(s32 id, s32 arg1);
extern void Func_08009178(s32 arg0, s32 arg1, s32 arg2);
extern void Func_080770c8(s32 flagId);
extern void Func_080f9010(s32 sound_id);
extern u8 *Data_03001ebc;

void Func_02001494(void)
{
    u8 *record;
    u8 *workspace;
    s32 poseActor;

    Func_080770c8(0x89a);
    Func_0808a010(30);
    Func_0808a148(13, 0, 0);
    Func_0808a148(15, 0, 0);
    Func_0808a148(16, 0, 0);
    Func_0808a010(20);
    Func_0808a1e8(13, 0x100, 0);
    Func_0808a1e8(15, 0x100, 0);
    Func_0808a1e8(16, 0x100, 0);
    Func_0808a010(60);
    Func_0808a170(0x183b);
    Func_0808a188(13, 0, 20);
    Func_0808a148(0, 13, 0);
    Func_0808a138(15, 1);
    Func_0808a010(20);
    Func_0808a188(15, 0, 20);
    Func_0808a148(0, 15, 0);
    Func_0808a138(16, 2);
    Func_0808a010(20);
    Func_0808a148(0, 16, 0);
    Func_0808a190(16, 0);
    Func_0808a010(50);
    Func_0808a200(16, 1);
    Func_0808a090(16, 0xcccc, 0x6666);
    Func_0808a0d0(16, 176, 248);
    Func_0808a0d0(16, 154 << 1, 248);
    Func_0808a1b8(0, 128 << 6, 0);
    Func_0808a1b8(16, 192 << 8, 20);
    Func_080f9010(158);
    Func_08009178(0x0200c77a, 78, 13);
    Func_0808a138(16, 2);
    Func_0808a010(20);
    Func_0808a090(16, 192 << 9, 192 << 8);
    record = Func_0808a080(16);
    record[90] &= 0xfe;
    Func_0808a0d0(16, 154 << 1, 136 << 1);
    Func_0808a010(1);
    record = Func_0808a080(16);
    record[90] |= 1;
    Func_0808a188(16, 0, 50);
    Func_0808a0f0(17, 152 << 17, 216 << 16);
    Func_0808a0d0(17, 152 << 1, 248);
    Func_0808a148(9, 17, 0);
    Func_0808a148(10, 17, 0);
    Func_0808a148(11, 17, 0);
    Func_0808a148(12, 17, 0);
    Func_0808a148(13, 17, 0);
    Func_0808a148(14, 17, 0);
    Func_0808a148(15, 17, 0);
    Func_0808a148(16, 17, 0);
    Func_0808a148(0, 17, 0);
    Func_0808a010(10);
    Func_0808a130(9, 2);
    Func_0808a130(10, 2);
    Func_0808a130(11, 2);
    Func_0808a130(12, 2);
    Func_0808a130(13, 2);
    Func_0808a130(14, 2);
    Func_0808a130(15, 2);
    Func_0808a138(16, 2);
    Func_0808a1e8(17, 0x103, 60);
    Func_0808a0f0(18, 152 << 17, 216 << 16);
    Func_0808a0c8(18, 152 << 1, 248);
    Func_0808a0c8(17, 140 << 1, 132 << 1);
    Func_0808a0e8(18);
    Func_0808a1b8(18, 160 << 7, 0);
    Func_0808a0e8(17);
    Func_080f9010(159);
    Func_08009178(0x0200c790, 78, 13);
    Func_0808a188(18, 0, 20);
    Func_0808a148(9, 17, 0);
    Func_0808a148(10, 17, 0);
    Func_0808a148(11, 17, 0);
    Func_0808a148(12, 17, 0);
    Func_0808a148(13, 17, 0);
    Func_0808a148(14, 17, 0);
    Func_0808a148(15, 17, 0);
    Func_0808a148(16, 17, 0);
    Func_0808a148(0, 17, 0);
    Func_0808a010(10);
    Func_0808a138(17, 2);
    Func_0808a010(20);
    Func_0808a110(18, 4);
    Func_0808a010(20);
    Func_0808a188(18, 0, 20);
    Func_0808a110(17, 3);
    Func_0808a010(20);
    Func_0808a138(18, 1);
    Func_0808a010(20);
    Func_0808a188(18, 0, 20);
    Func_0808a1b8(17, 208 << 8, 20);
    Func_0808a188(17, 0, 20);
    Func_0808a1e8(18, 0x102, 60);
    Func_0808a188(18, 0, 20);
    Func_0808a110(17, 3);
    Func_0808a010(20);
    Func_0808a188(17, 0, 20);
    Func_0808a110(18, 3);
    Func_0808a010(20);
    Func_0808a188(18, 0, 20);
    Func_0808a110(17, 3);
    Func_0808a010(20);
    Func_0808a110(18, 4);
    Func_0808a010(20);
    Func_0808a188(18, 0, 20);
    Func_0808a138(17, 2);
    Func_0808a010(20);
    Func_0808a1b8(17, 0, 20);
    Func_0808a188(17, 0, 20);
    Func_0808a1b8(16, 128 << 8, 20);
    Func_0808a110(16, 3);
    Func_0808a010(20);
    Func_0808a110(17, 3);
    Func_0808a010(20);
    Func_0808a188(17, 0, 20);
    Func_0808a110(16, 3);
    Func_0808a010(20);
    Func_0808a188(16, 0, 20);
    Func_0808a1b8(17, 128 << 8, 20);
    Func_0808a188(17, 0, 20);
    Func_0808a110(9, 3);
    Func_0808a010(20);
    Func_0808a188(9, 0, 20);
    Func_0808a1b8(17, 208 << 8, 20);
    Func_0808a138(17, 1);
    Func_0808a010(20);
    Func_0808a188(17, 0, 20);
    Func_0808a1e8(18, 0x102, 60);
    Func_0808a188(18, 0, 20);
    Func_0808a1e8(17, 0x101, 60);
    Func_0808a188(17, 0, 20);
    Func_0808a110(18, 3);
    Func_0808a010(20);
    Func_0808a188(18, 0, 20);
    Func_0808a1e8(17, 0x100, 60);
    Func_0808a188(17, 0, 20);
    Func_0808a110(18, 4);
    Func_0808a010(20);
    Func_0808a1e8(17, 0x103, 60);
    Func_0808a188(17, 0, 20);
    Func_0808a1e8(18, 0x100, 60);
    Func_0808a188(18, 0, 20);
    Func_0808a110(17, 4);
    Func_0808a010(20);
    Func_0808a188(17, 0, 20);
    Func_0808a138(18, 2);
    Func_0808a010(20);
    Func_0808a188(18, 0, 20);
    Func_0808a138(17, 2);
    Func_0808a010(10);
    Func_0808a0d0(17, 128 << 1, 140 << 1);
    Func_0808a1b8(17, 128 << 7, 20);
    Func_0808a0f0(17, 0, 0);
    Func_0808a088(17);
    Func_0808a010(30);
    Func_0808a138(9, 2);
    Func_0808a010(20);
    Func_0808a188(9, 0, 20);
    Func_0808a138(15, 2);
    Func_0808a010(20);
    Func_0808a188(15, 0, 20);
    Func_0808a148(16, 18, 0);
    Func_0808a010(20);
    Func_0808a138(16, 2);
    Func_0808a010(20);
    Func_0808a188(16, 0, 20);
    Func_0808a148(18, 16, 0);
    Func_0808a010(20);
    Func_0808a188(18, 0, 20);
    Func_0808a110(18, 4);
    Func_0808a010(20);
    Func_0808a188(18, 0, 20);
    Func_0808a110(18, 3);
    Func_0808a010(20);
    Func_0808a188(18, 0, 20);
    Func_0808a090(18, 0xcccc, 0x6666);
    Func_0808a0d0(18, 128 << 1, 248);
    Func_0808a1b8(18, 192 << 8, 20);
    Func_0808a130(18, 1);
    Func_0808a1e8(18, 0x100, 60);
    Func_0808a0d0(18, 240, 184);
    Func_0808a138(18, 2);
    Func_0808a010(20);
    Func_0808a0f0(19, 232 << 16, 168 << 16);
    Func_0808a0f0(20, 232 << 16, 168 << 16);
    record = Func_0808a080(19);
    *(s32 *)(record + 12) = 192 << 12;
    record = Func_0808a080(19);
    *(u32 *)(record + 60) = 0x80000000;
    record = Func_0808a080(19);
    *(s32 *)(record + 24) = 0xcccc;
    record = Func_0808a080(19);
    *(u16 *)(*(u8 **)(record + 80) + 30) = 128 << 8;
    Func_080f9010(124);
    Func_0808a188(18, 0, 20);
    Func_0808a1b8(0, 192 << 8, 20);
    Func_0808a0d0(16, 128 << 1, 240);
    Func_0808a1b8(16, 176 << 8, 20);
    Func_0808a138(16, 1);
    Func_0808a188(16, 0, 20);
    Func_0808a148(9, 0, 0);
    Func_0808a148(10, 0, 0);
    Func_0808a148(11, 0, 0);
    Func_0808a148(12, 0, 0);
    Func_0808a148(13, 0, 0);
    Func_0808a148(14, 0, 0);
    Func_0808a148(15, 0, 0);
    Func_0808a148(16, 0, 0);
    Func_0808a138(18, 2);
    Func_0808a010(20);
    Func_0808a1b8(18, 160 << 7, 20);
    Func_0808a0d0(18, 248, 208);
    Func_0808a1b8(18, 160 << 7, 20);
    Func_0808a178(18, 0);
    if (Func_0808a070(0, 0) == 0) {
        Func_0808a138(16, 1);
        Func_0808a010(20);
        poseActor = 16;
        goto emitSkippedPose;
    }

    workspace = Data_03001ebc;
    *(u16 *)(workspace + 472) += 1;
    Func_0808a010(20);
    Func_0808a1e8(18, 0x105, 60);
    Func_0808a1b8(18, 128 << 7, 20);
    Func_0808a138(16, 2);
    Func_0808a010(20);
    Func_0808a178(16, 0);
    if (Func_0808a070(0, 0) == 0) {
        Func_0808a110(16, 3);
        Func_0808a010(20);
        Func_0808a1b8(18, 176 << 8, 20);
        poseActor = 18;
        goto emitSkippedPose;
    }
    goto secondSkip;

emitSkippedPose:
    /* The outer and inner zero-result paths share these physical sites. */
    Func_0808a188(poseActor, 0, 20);
    Func_080770c8(0x898);
    goto skipComplete;

secondSkip:
    *(u16 *)(workspace + 472) += 1;
    Func_0808a010(20);
    Func_0808a110(18, 4);
    Func_0808a010(20);
    Func_0808a188(18, 0, 20);
    Func_080770c8(0x899);

skipComplete:
    Func_0808a1b8(10, 128 << 8, 0);
    Func_0808a1b8(11, 128 << 8, 20);
    Func_0808a100(10, 5);
    Func_0808a100(11, 5);
    Func_0808a098(12, 0x0200c638);
}

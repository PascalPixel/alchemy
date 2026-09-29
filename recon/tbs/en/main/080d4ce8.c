#include "TYPES.H"
#include "RESOURCE_IDS.H"
#include "BATTLE_EFX.H"

typedef s32 (*CopyWords)(void *, const void *, s32);
typedef struct Effect {
    s32 kind, side, actor, unk0c, unk10, count, variant, unk1c, unk20;
    s16 actors[8];
} Effect;
typedef struct Particle {
    s32 x, y, vx, unused0c, vy, unused14, life;
} Particle;
typedef struct Anchor {
    s32 x, y, z, unused0c, unused10, unused14, unused18;
} Anchor;
extern void *gWorkSlot[];
extern u8 Data_080ee29a[], Data_080ee2a9[];
extern s8 Data_080ee29d[];
extern u16 ParticleStreams_CellOffsets[];
void BattleFx_BeginCanvasLayer(s32);
void *Resource_GetTableEntry(s32);
void **GetBattleObjectSlotFar(s32);
s32 Random16(void);
s32 Scheduler_AddOrUpdateCallback(s32, s32);
void Scheduler_RemoveCallback(s32);
void Render_ResetTransformState(void);
void Graphics_PrepareTransferInIwramWork(void *, void *);
void BattleEventRuntime_BeginPhaseFar(s32);
void EffectPosition_ApplyBaseAndYOffset(void *, s32 *);
void Audio_PlayCue(s32);
s32 Math_Mod(s32, s32);
s32 Trig_Sin(s32);
s32 Trig_Cos(s32);
void ObjectGroup_UpdateMembers(s32, s32, s32, s32, s32);
void BattleMotion_ApplyVariantMotionFar(s32, s32);
void Camera_ApplyShake(s32, s32);
void ObjectGroup_TickMemberTimers(void);
void WaitFrames(s32);
void Runtime_ReleaseHeapBlock(s32);
s32 BattleFx_EndCanvasLayer(void);
#define EFFECT (*(Effect **)(work + 0x7828))
#define PARTICLES ((Particle *)0x02010000)

/* Four projected columns emit particles into a shared pool. The complete
 * owner ends at 080d5258, including both internal and final literal pools. */
void Region_080d4ce8(Effect *effect)
{
    void **cache;
    void **entry;
    u8 *work, *extra;
    void *destination;
    void *camera;
    void *actor;
    void *palette;
    DrawRectangle draw[2];
    DrawRectangle *draws;
    DrawRectangle second;
    Particle *particle;
    Anchor *anchor;
    s32 screen[3];
    s32 frame, i, slot, status;
    s32 alternate;
    u8 *trigger;
    u8 *overlay;

    cache = &gWorkSlot[39];
    entry = cache;
    work = *entry++;
    destination = *entry;
    extra = cache[2];
    EFFECT = effect;
    BattleFx_BeginCanvasLayer(1);
    *(u16 *)0x04000052 = 0x1010;
    Resource_LoadAndDecompress((s32)&ResourceId_FirePillarSheetA, work, 1, 1);
    Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, extra, 0, 0);
    if (EFFECT->variant == 0) {
        palette = Resource_GetTableEntry((s32)&ResourceId_RedPaletteB);
        status = ((CopyWords)0x03001388)((void *)0x05000000, palette, 128);
    } else if (EFFECT->variant == 2) {
        palette = Resource_GetTableEntry((s32)&ResourceId_OrangePaletteB);
        status = ((CopyWords)0x03001388)((void *)0x05000000, palette, 128);
    }
    status = BattleEffect_LoadWork(46, 7, 7, 3, 2);
    draw[0] = (DrawRectangle)gWorkSlot[46];
    status = BattleEffect_LoadWork(47, 7, 7, 7, 2);
    second = (DrawRectangle)gWorkSlot[47];
    draws = draw;
    draws[1] = second;
    i = 0;
    particle = PARTICLES;
    do {
        i++;
        particle->life = 0;
        particle++;
    } while (i != 1024);
    actor = *GetBattleObjectSlotFar(EFFECT->actors[0]);
    anchor = (Anchor *)(work + 0x7080);
    i = 0;
    do {
        s32 x;
        x = ((Random16() & 15) + 72) << 16;
        anchor->y = 0;
        anchor->x = x;
        anchor->z = Data_080ee29d[EFFECT->variant * 4 + i] << 16;
        if (*(s32 *)((u8 *)actor + 8) < 0)
            anchor->x = -x;
        i++;
        anchor++;
    } while (i != 4);
    *(s32 *)(work + 0x7780) = 2;
    *(s32 *)(work + 0x7784) = 50;
    Scheduler_AddOrUpdateCallback(0x080cd261, 0x480);
    frame = 0;
    do {
        camera = gWorkSlot[12];
        if (EFFECT->variant == 2 && frame <= 63) {
            if (EFFECT->side == 0)
                *(u16 *)((u8 *)camera + 54) += 192;
            else
                *(u16 *)((u8 *)camera + 54) -= 192;
        }
        if (frame == 16)
            BattleEventRuntime_BeginPhaseFar(134);
        Render_ResetTransformState();
        Graphics_PrepareTransferInIwramWork(camera, (u8 *)camera + 12);
        if (frame <= 63) {
            slot = 0;
            if (Data_080ee29a[EFFECT->variant] != 0) {
                alternate = frame & 1;
                anchor = (Anchor *)(work + 0x7080);
                overlay = work + 0xdd0;
                trigger = Data_080ee2a9;
                do {
                    EffectPosition_ApplyBaseAndYOffset(anchor, screen);
                    screen[0] /= 2;
                    screen[1] -= 8;
                    if (frame == *trigger)
                        Audio_PlayCue(145);
                    if (frame >= *trigger + 4) {
                        s32 spin;
                        spin = Math_Mod(slot * 25 + (frame << 4), 104);
                        draws[slot & 1](destination, work, screen[0] - 17,
                            screen[1] - spin - 104, 34, 104);
                        draws[slot & 1](destination, work, screen[0] - 17,
                            screen[1] - spin, 34, spin);
                        if (alternate != 0) {
                            draw[0](destination, overlay, screen[0] - 20, screen[1] - 24, 20, 37);
                            draws[1](destination, overlay, screen[0], screen[1] - 24, 20, 37);
                        } else {
                            draw[0](destination, work + 0x10b4, screen[0] - 20, screen[1] - 24, 20, 37);
                            draws[1](destination, work + 0x10b4, screen[0], screen[1] - 24, 20, 37);
                        }
                    }
                    if (frame == *trigger || frame >= *trigger + 16) {
                        s32 spawned;
                        spawned = 0;
                        i = 0;
                        particle = PARTICLES;
                        do {
                            if (particle->life == 0) {
                                s32 magnitude, angle;
                                magnitude = Random16() & 0x3ff;
                                angle = Random16();
                                particle->x = screen[0] << 8;
                                particle->y = (screen[1] << 8) + 4096;
                                angle = (angle & 0x7fff) - 0x4000;
                                particle->vx = ((magnitude + 32) * Trig_Sin(angle)) >> 15;
                                magnitude += 32;
                                particle->vy = -((magnitude * Trig_Cos(angle)) << 1) >> 15;
                                spawned++;
                                if (frame == *trigger) {
                                    particle->life = (Random16() & 7) + 48;
                                    if (spawned == 200) break;
                                } else {
                                    particle->life = (Random16() & 7) + 24;
                                    if (spawned == 4) break;
                                }
                            }
                            i++;
                            particle++;
                        } while (i != 1024);
                    }
                    if (frame == Data_080ee2a9[slot]) {
                        *(s32 *)(work + 0x77a8) = 2;
                        i = 0;
                        while (i != EFFECT->count) {
                            ObjectGroup_UpdateMembers(EFFECT->actors[i], 10, 5, i, 8);
                            BattleMotion_ApplyVariantMotionFar(EFFECT->actors[i], 1);
                            i++;
                        }
                    }
                    slot++;
                    anchor++;
                    trigger++;
                } while (slot != Data_080ee29a[EFFECT->variant]);
            }
        }
        i = 0;
        particle = PARTICLES;
        do {
            s32 life;
            life = particle->life;
            if (life > 0) {
                s32 x, y, px, py, size;
                particle->life = life - 1;
                x = particle->x + particle->vx;
                y = particle->y + particle->vy;
                particle->x = x;
                particle->y = y;
                particle->vx = (particle->vx * 60) / 64;
                particle->vy = (particle->vy * 60) / 64 - 16;
                py = y / 256;
                if (py > 120) {
                    particle->vy = -particle->vy / 2;
                } else if (x >= 0) {
                    px = x >> 8;
                    if (px <= 126 && y >= 0) {
                        size = (life - 17) / 8;
                        if (size <= 0) size = 1;
                        draws[i & 1](destination, extra + ParticleStreams_CellOffsets[size - 1],
                            px - size / 2, py - size, size, size * 2);
                    }
                }
            }
            i++;
            particle++;
        } while (i != 1024);
        Camera_ApplyShake(16, 16);
        ObjectGroup_TickMemberTimers();
        *(s32 *)(work + 0x7824) = 1;
        WaitFrames(1);
        frame++;
    } while (frame != 96);
    Runtime_ReleaseHeapBlock(47);
    Runtime_ReleaseHeapBlock(46);
    Scheduler_RemoveCallback(0x080cd261);
    BattleFx_EndCanvasLayer();
}

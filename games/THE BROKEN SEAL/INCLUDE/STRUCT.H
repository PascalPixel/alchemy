#ifndef STRUCT_H
#define STRUCT_H

#include "TYPES.H"

struct EffectRecord;
struct Sub;

struct Actor_02000640 {
    u8 reserved00[6];
    u16 facing;
    u8 reserved08[92];
    u16 state_flags;
};

struct Actor_02000754 {
    u8 reserved00[91];
    u8 accepted;
};

struct Actor_020007d4 {
    u8 reserved00[91];
    u8 accepted;
};

struct CameraWork {
    u8 unknown_00[0x36];
    u16 scroll;
};

struct Cell {
    u8 unk0;
    u8 unk1;
    u8 kind;
    u8 type;
};

struct Cells03001ad0 {
    u16 unk00;
    u16 unk02;
    u16 unk04;
    u16 unk06;
};

struct EffectVector {
    s32 x;
    s32 y;
    s32 z;
};

struct EffectWork {
    u8 pad[80];
    struct EffectRecord *record;
};

struct Flags35 {
    u8 pad[35];
    u8 flags;
};

struct Flags85 {
    u8 pad[85];
    u8 flags;
};

struct Flags9 {
    u8 pad[9];
    u8 low : 2;
    u8 mode : 2;
};

struct FlashChipType {
    u32 handlers[5];
    u32 size;
    u16 sector_size;
    u16 unknown_1a;
    u8 sector_shift;
};

struct HeightTrackedObject {
    u8 pad00[12];
    s32 height;
};

struct Obj_02002608 {
    u8 pad00[0x18];
    s32 f18;
    u8 pad1c[7];
    u8 f23;
    u8 pad24[12];
    s32 f30;
    s32 f34;
    u8 pad38[24];
    struct Sub *f50;
    u8 pad54[1];
    u8 f55;
};

struct OverlayActorPosition {
    u8 pad00[8];
    s32 depth_fixed;
};

struct OverlayActorState {
    u8 pad00[35];
    u8 flags;
};

struct OverlayEffectMotion {
    u8 pad00[8];
    s32 x;
    s32 y;
    s32 z;
    u8 pad14[28];
    s32 horizontal_rate;
    s32 vertical_rate;
    s32 shadow_x;
    s32 shadow_y;
    s32 shadow_z;
    u8 pad44[32];
    s16 mode;
};

struct Particle_02000c4c {
    u8 unknown_00[8];
    s32 x;
    s32 y;
    u8 unknown_10[0x20];
    s32 angle;
    u8 unknown_34[4];
    s32 base_x;
    s32 base_y;
    u8 unknown_40[0x10];
    u16 *sprite;
};

struct PathRecorder {
    s16 mode;
    s16 mirror;
    s16 actor;
    u16 pos;
    s16 still;
};

struct Presentation {
    u8 reserved_00[9];
    u8 flags;
};

struct Presentation_02000c1c {
    u8 unk_00[9];
    u8 flags;
};

struct Ring {
    s32 x;
    s32 y;
    s32 vel_x;
    s32 vel_y;
    s32 age;
};

struct S {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    u8 pad10[32];
    s32 f30;
    s32 f34;
    s32 f38;
    s32 f3c;
    u8 pad40[16];
    struct T *f50;
};

struct S_020009dc {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
    u8 pad14[15];
    u8 f23;
};

struct S_02001bbc {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

struct S_02001c84 {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

struct S_02001d2c {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

struct S_02001de0 {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

struct S_02001e5c {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

struct S_02001ef4 {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

struct S_02001f78 {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

struct S_02002004 {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

struct S_02002080 {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

struct S_0200216c {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

struct S_02002200 {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

struct S_020022c8 {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

struct S_020023c4 {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

struct S_02002450 {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

struct ScatterPosition {
    s32 x;
    s32 y;
    s32 z;
};

struct SceneActor_020004b4 {
    u8 reserved_00[35];
    u8 state_23;
    u8 reserved_24[44];
    struct Presentation *presentation;
};

struct SceneActor_02000c1c {
    u8 unk_00[35];
    u8 state_23;
    u8 unk_24[44];
    struct Presentation_02000c1c *presentation;
};

struct SceneActor_0200113c {
    u8 reserved_00[35];
    u8 state_23;
    u8 reserved_24[44];
    struct Presentation *presentation;
};

struct SceneWork_020006b4 {
    u8 reserved000[472];
    u16 branch_counter;
};

struct SceneWork_02000e90 {
    u8 reserved000[370];
    u16 actor18_marker;
};

struct ScrollLayer {
    u8 unknown_00[8];
    s32 x;
};

struct ScrollPair {
    u16 x;
    u16 y;
};

struct Slot {
    u16 f00;
    u16 f02;
    u16 f04;
    u16 f06;
};

struct SwitchCell {
    u32 x;
    u32 z;
};

struct SwitchEffect {
    s32 active;
    u8 unknown_04[0x5f];
    u8 finished;
};

struct T {
    u8 pad00[30];
    u16 f1e;
};

struct TileMask {
    u32 word0;
    u32 word1;
};

struct TileRun {
    s16 id;
    s16 x;
    s16 y;
    s16 vertical;
    s16 unused08;
    s16 unused0a;
};

struct V6 {
    s32 a;
    s32 b;
    s32 c;
    s32 d;
    s32 e;
    s32 f;
};

struct Vector3 {
    s32 x;
    s32 y;
    s32 z;
};

struct WorldMapVramBlock {
    u16 base;
    u16 offset;
};

#endif

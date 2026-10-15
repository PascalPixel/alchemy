#ifndef ALCHEMY_RAM_BUFFER_H
#define ALCHEMY_RAM_BUFFER_H

#include "types.h"

#define Ram_MapCellBuffer FIXED_ADDRESS((u8 *), 0x02010000, gMapCellBuffer)
#define Ram_MapCollision FIXED_ADDRESS((u8 *), 0x0202c000, gMapCollision)

#define Ram_SoundMixer FIXED_ADDRESS((u8 *), 0x03006000, SoundDriver_Mixer)

#define Ram_IwramHeapEnd FIXED_ADDRESS((u8 *), 0x03006fc0, IwramSoundMixWorkspace)

// overlay 693 の Func_02000c14 はフレームカウンタを定数アドレスで読む
// (2 回のテストがそれぞれアドレスを読み直す。シンボルでは 1 回にまとまる)
#define Ram_FrameCount FIXED_ADDRESS((u32 *), 0x0300122c, gFrameCount)

struct BattleWork;
struct BattlePresentationTransition;
struct MapWork;
struct ObjectSystemWork;
struct DisplayTransitionState;

struct HeapSlots
{
    void                                *nextEwram;
    void                                *nextIwram;
    u8                                  *transformStack;
    void                                *unk0c[2];
    void                                *scriptObjects;
    struct ObjectSystemWork             *objectSystem;
    void                                *unk1c[1];
    struct MapWork                      *mapWork;
    struct BattleWork                   *battleWork;
    void                                *unk28[2];
    void                                *cameraWork;
    void                                *unk34[2];
    u8                                  *windowTiles;
    void                                *windowLayout;
    void                                *glyphWork;
    void                                *menuWork;
    void                                *unk4c[4];
    u8                                  *battleFxWork[2];
    void                                *unk64[2];
    void                                *eventWork;
    void                                *unk70[3];
    struct DisplayTransitionState       *displayTransition;
    void                                *block32;
    void                                *unk84[11];
    struct BattlePresentationTransition *transitionWork;
    u16                                 *canvasMap;
    void                                *unkb8[5];
    void                                *serialWork;
    void                                *unkd0[3];
    void                                *menuRuntime;
    void                                *effectWork;
    void                                *unke4[1];
    void                                *menuSelectWork;
    void                                *unkec[3];
    void                                *block62;
    void                                *unkfc[1];
};

#define Ram_HeapSlots FIXED_ADDRESS((struct HeapSlots *), 0x03000000, gHeapSlots)

#endif

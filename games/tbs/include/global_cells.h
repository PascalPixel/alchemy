#ifndef ALCHEMY_GLOBAL_CELLS_H
#define ALCHEMY_GLOBAL_CELLS_H

#include "types.h"
#include "effect_step.h"

struct DisplayTransitionState;

extern void *gMapWork[];
extern void *gMenuWork;

// ワークのスロット（gWorkSlot の並び）を自分の名前で読む
extern void *gFieldBlendWork[];                           // スロット 24 から
extern struct DisplayTransitionState *gDisplayTransition; // スロット 31
extern s32 *gDisp;        // スロット 42：切り替えのタイマー
extern void *gEffectWork; // スロット 56：場面ごとに別の構造体を指す

// EWRAM の置き場
extern struct EffectStep
    gMapCellBuffer[]; // マップのセルの置き場。戦闘の演出は粒の並びとして使う
extern u8 gDecodeBuffer[];
extern u8 gActorSpriteSlots[];

// 通信
extern u16 gLinkPeerSignatures[][12]; // 相手ごとに 24 バイト

#endif

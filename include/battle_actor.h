#ifndef ALCHEMY_BATTLE_ACTOR_H
#define ALCHEMY_BATTLE_ACTOR_H

#include "types.h"

/* 戦闘表示オブジェクトとスロット更新。 */
void *Func_080b7dd0(s32 unit); /* 表示オブジェクトを引く */
void Func_080b6f44(void *obj, s32 unit, s32 x, s32 y); /* マスへ配置 */
void Func_080b6c90(void); /* 配置の確定 */
s32 Func_080b6ae0(s16 *entries); /* 表示スロット一覧。個数を返す */
void Func_080b8000(s32 slot); /* スロットの再描画 */

#define Actor_GetObject Func_080b7dd0
#define Actor_Place Func_080b6f44
#define Actor_Commit Func_080b6c90
#define Actor_ListSlots Func_080b6ae0
#define Actor_RefreshSlot Func_080b8000

#endif

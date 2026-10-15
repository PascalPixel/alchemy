#ifndef ALCHEMY_UNIT_FIELD_OBJECT_SCRIPT_H
#define ALCHEMY_UNIT_FIELD_OBJECT_SCRIPT_H

#include "types.h"

// 命令の処理は物体を受けて、そのフレームのスクリプトを続けるなら 0 以外、
// 終えるなら 0 を返す。
// 処理ごとに物体の見方 (構造体) が違うので、引数の型は決めない。
typedef s32 (*ScriptCommandFn)();

extern const ScriptCommandFn ScriptObject_CommandTable[47];

#endif

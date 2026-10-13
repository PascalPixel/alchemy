#ifndef ALCHEMY_ACTOR_H
#define ALCHEMY_ACTOR_H

// 場面が物体を持つ型。
// Scene commands for actors, kept apart until field_event.h takes them.

#include "field_effect.h"

// 物体を番号から union FieldObject として得る。人物 (actor) と効果
// (effect) の両方の見方で読めるので、この型を通した読み書きは他の型の
// 書き込みと重なりうるものとして順序が保たれる (a union access aliases
// every store under GCC's type-based alias analysis).
#define OBJ_GET(no) ((union FieldObject *)Object_GetById(no))

#endif

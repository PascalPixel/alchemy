#ifndef ALCHEMY_OBJECT_LOOKUP_H
#define ALCHEMY_OBJECT_LOOKUP_H

/* 隠しr0引数の呼出しを保つため、引数型は確定まで省略する。 */

/* Object indices run through 191; field mode allocates 66 actor slots. */

void *ObjectTable_Get();

#endif

#ifndef ALCHEMY_OBJECT_LOOKUP_H
#define ALCHEMY_OBJECT_LOOKUP_H

/* 隠しr0引数の呼出しを保つため、引数型は確定まで省略する。 */

/* Generic lookup reads one address word at +0x14 and rejects indices
   above 0xbf. The bound does not describe the allocation of every mode;
   field mode has 66 actor slots. Existing zero-argument callers keep this
   unspecified-argument boundary until their incoming value is recovered. */

void *ObjectTable_Get();

#endif

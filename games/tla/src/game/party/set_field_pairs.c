#include "types.h"
#include "sequence_parent_work.h"

void Party_SetFields1eeAnd1f0(u16 first, u16 second) {
  gSequenceParentWork.pair1ee[0] = first;
  gSequenceParentWork.pair1ee[1] = second;
}

void Party_SetFields1f2And1f4(u16 first, u16 second) {
  gSequenceParentWork.pair1f2[0] = first;
  gSequenceParentWork.pair1f2[1] = second;
}

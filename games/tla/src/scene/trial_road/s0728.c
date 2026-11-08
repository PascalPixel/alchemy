#include "trial_road.h"

/// While the party's render mode is 0, clears the map cell flag under the
/// object.
void Func_02000728(s32 a, s32 id) {
  struct MainRoutineWork *party = &gMainRoutineWork;
  u8 mode;


  mode = party->renderMode;

  if (mode == 0)
    Func_02000660(id);
}

/// While the party's render mode is 0, sets the map cell flag under the object.
void Func_02000748(s32 a, s32 id) {
  struct MainRoutineWork *party = &gMainRoutineWork;
  u8 mode;


  mode = party->renderMode;

  if (mode == 0)
    Func_0200062c(id);
}

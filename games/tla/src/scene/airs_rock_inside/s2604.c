#include "airs_rock_inside.h"

extern u32 gData_0200654c[];
extern u32 gData_02006564[];
extern u32 gData_0200663c[];
extern u32 gData_020066b4[];
extern u32 gData_0200675c[];
extern u32 gData_020068ac[];
extern u32 gData_020068f4[];
extern u32 gData_020069e4[];
extern u32 gData_02006aec[];
extern u32 gData_02006bac[];

s32 Math_Cosine(s32 angle);
void Actor_SetAnimationSpeed(struct FieldActor *object, s32 value);
void Map_EnableUpdateCallback(void);
void Map_DisableUpdateCallback(void);
void Func_020004d4(struct FieldActor *object);

u32 *Func_02002604(void) {
  if (gAdventure.scene == (s32)&SceneId_Ov664Room1)
    return gData_02006564;
  if (gAdventure.scene == (s32)&SceneId_Ov664Room2)
    return gData_0200663c;
  if (gAdventure.scene == (s32)&SceneId_Ov664Room3)
    return gData_020066b4;
  if (gAdventure.scene == (s32)&SceneId_Ov664Room4)
    return gData_0200675c;
  if (gAdventure.scene == (s32)&SceneId_Ov664Room5)
    return gData_020068ac;
  if (gAdventure.scene == (s32)&SceneId_Ov664Room6)
    return gData_020068f4;
  if (gAdventure.scene == (s32)&SceneId_Ov664Room7)
    return gData_020069e4;
  if (gAdventure.scene == (s32)&SceneId_Ov664Room8)
    return gData_02006aec;
  if (gAdventure.scene == (s32)&SceneId_Ov664Room9)
    return gData_02006bac;
  return gData_0200654c;
}

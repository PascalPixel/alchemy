/* Near miss: Func_0200096c differs 10. The listing reloads x after the first branch; this draft keeps it in r2
 * and skips the reload when the branch is not taken. */
#include "../../../../games/tla/src/field/overlay_6aa/room.h"

extern char SceneId_Ov6aaRoom1;
extern char SceneId_Ov6aaRoom2;
extern char SceneId_Ov6aaRoom3;
extern char SceneId_Ov6aaRoom4;
extern char SceneId_Ov6aaRoom5;
extern char SceneId_Ov6aaRoom6;
extern char SceneId_Ov6aaRoom7;
extern const u8 Data_02005af4[];
extern const u8 Data_02005b0c[];
extern const u8 Data_02005b54[];
extern const u8 Data_02005bcc[];
extern const u8 Data_02005d4c[];
extern const u8 Data_02005eb4[];
extern const u8 Data_02005f74[];
extern const u8 Data_020060c4[];

void Func_02000414(s32 id);
void Func_02000798(void);
void Func_020019c4(void);
void Func_020046b4(void);
void Func_02004718(void);
void Func_02004bf8(struct ObjectRuntime *object, s32 x, s32 y, s32 z);
void Func_02004d58(s32 a0, s32 a1);
void Func_02004d60(s32 frames);
void Func_02004dc8(void);
void Func_02004dd0(void);
void BattleFx_PrepareBufferInterpolation(void);

void Func_0200096c(s32 unused, s32 id)
{
    struct ObjectRuntime *object = Object_GetById(id);
    s32 x;

    x = object->x >> 20;
    if (x == 11) {
        object->speed_limit = 0x4000;
        object->acceleration = 0x2000;
        Func_02004bf8(object, 0x980000, object->y, 0x3180000);
    }
    x = object->x >> 20;
    if (x == 10) {
        object->speed_limit = 0x4000;
        object->acceleration = 0x2000;
        Func_02004bf8(object, 0xc80000, object->y, 0x3180000);
    }
}


#include "types.h"
#include "party_state.h"
#include "object_runtime.h"
#include "text/msg_ids.h"

struct ObjectRuntime *Object_GetById(s32 id);
void Func_020010e0(void);
void Func_02001180(s32 a0);
void Func_02001138(s32 a0);
void Func_02001148(s32 a0, s32 a1);
void Func_020010e8(void);
void Func_02001190(s32 slot, s32 actor);

#ifdef V9
static __inline__ u16 Angle_Quadrant(u16 angle)
{
    return (angle + 0x2000) & ~0x3fff;
}
#define TEST if (Angle_Quadrant(Object_GetById(gPartyState.current_owner)->angle) == 0xc000)
#endif
#ifdef V10
static __inline__ s32 Angle_Quadrant(s32 angle)
{
    return (u16)((angle + 0x2000) & ~0x3fff);
}
#define TEST if (Angle_Quadrant(Object_GetById(gPartyState.current_owner)->angle) == 0xc000)
#endif
#ifdef V11
static __inline__ s32 Angle_Is(u16 angle, s32 quadrant)
{
    return (u16)((angle + 0x2000) & ~0x3fff) == quadrant;
}
#define TEST if (Angle_Is(Object_GetById(gPartyState.current_owner)->angle, 0xc000))
#endif
#ifdef V12
static __inline__ u16 Angle_Quadrant(u16 angle)
{
    u16 turned = angle + 0x2000;
    return turned & ~0x3fff;
}
#define TEST if (Angle_Quadrant(Object_GetById(gPartyState.current_owner)->angle) == 0xc000)
#endif
#ifdef V1
#define TEST s32 facing = Object_GetById(gPartyState.current_owner)->angle; facing += 0x2000; facing &= ~0x3fff; if ((u16)facing == 0xc000)
#endif
#ifdef V2
#define TEST u16 quadrant = (Object_GetById(gPartyState.current_owner)->angle + 0x2000) & ~0x3fff; if (quadrant == 0xc000)
#endif
#ifdef V3
#define TEST if ((u16)((Object_GetById(gPartyState.current_owner)->angle + 0x2000) & ~0x3fff) == 0xc000)
#endif
#ifdef V4
static __inline__ u16 Facing(s32 id)
{
    return Object_GetById(id)->angle;
}
#define TEST if ((u16)((Facing(gPartyState.current_owner) + 0x2000) & ~0x3fff) == 0xc000)
#endif
#ifdef V5
static __inline__ s32 Quadrant(s32 id)
{
    return (u16)((Object_GetById(id)->angle + 0x2000) & ~0x3fff);
}
#define TEST if (Quadrant(gPartyState.current_owner) == 0xc000)
#endif
#ifdef V6
#define TEST struct ObjectRuntime *leader = Object_GetById(gPartyState.current_owner); u16 facing = leader->angle + 0x2000; if ((u16)(facing & ~0x3fff) == 0xc000)
#endif
#ifdef V7
#define TEST u16 facing = Object_GetById(gPartyState.current_owner)->angle; facing += 0x2000; if ((u16)(facing & ~0x3fff) == 0xc000)
#endif
#ifdef V8
#define TEST s32 facing = (u16)(Object_GetById(gPartyState.current_owner)->angle + 0x2000); if ((facing & 0xc000) == 0xc000)
#endif

void Func_020000c4(s32 actor)
{
    TEST {
        Func_02001190(14, actor);
    } else {
        Func_020010e0();
        Func_02001180(0);
        Func_02001138((s32)MsgIfYouAre);
        Func_02001148(actor, 0);
        Func_020010e8();
    }
}

#include "TYPES.H"
#include "CALL.H"
extern u8 MsgMakyuriCatchUpLostOpportunity[];
extern u8 MsgMakyuriStoreClosedUntilWell[];

s32 Object_GetById();
extern u8 ImiruMura_TurnScript[];
s32 Engine_GameFlagIsSet();
void Engine_ShopOpen();
void Engine_EventBegin();
void Engine_ActorFaceActor();
void Engine_EventWait();
void Engine_EventSetMessage();
void Engine_EventShowMessage();
void Engine_ActorFaceDirection();
void Engine_EventEnd();
void Engine_CameraSetSpeed();
void Engine_CameraMoveTo();
void Engine_CameraWaitForMove();
void Object_SetActionCallbackAndRefreshById();

/* Imil weapon shop: facing the counter with the village cured opens shop 10;
 * otherwise the keeper talks about being shut, or after recovery about catching
 * up on lost business. */
void ImiruMura_RunWeaponShop(void)
{
    s32 dir;

    dir = *(s16 *)(Object_GetById(0) + 6);
    if (Engine_GameFlagIsSet(0x881) != 0) {
        if ((u32)((dir << 16) + 0x5fff0000) <= 0x3ffe0000) {
            Engine_ShopOpen(10, 12);
            return;
        }
        Engine_EventBegin();
        Engine_ActorFaceActor(12, 0, 0);
        Engine_EventWait(10);
        Engine_EventSetMessage((s32)MsgMakyuriCatchUpLostOpportunity);
        Engine_EventShowMessage(12, 0);
        Call3(Engine_ActorFaceDirection, 12, 0x4000, 10);
        Engine_EventEnd();
    } else {
        if ((u32)((dir << 16) + 0x5fff0000) <= 0x3ffe0000) {
            Engine_EventBegin();
            Engine_CameraSetSpeed(0x60000, 0xc000);
            Call4(Engine_CameraMoveTo, 0x1aa0000, -1, 0x1ec0000, 1);
            Engine_CameraWaitForMove();
            Engine_EventWait(20);
            Object_SetActionCallbackAndRefreshById(12, (s32)ImiruMura_TurnScript);
            Engine_EventSetMessage((s32)MsgMakyuriStoreClosedUntilWell);
            Engine_EventShowMessage(12, 0);
            Engine_CameraMoveTo(0x1aa0000, -1, 0x2680000, 1);
            Engine_CameraWaitForMove();
            Engine_EventEnd();
        }
    }
}

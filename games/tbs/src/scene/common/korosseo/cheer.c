#include "types.h"
#include "fieldobj.h"
#include "game_state.h"
#include "field_event.h"

void Korosseo_ClearSavedActorPositions(void)
{
	GameFlag_SetByte(896, 0);
	GameFlag_SetByte(904, 0);
	GameFlag_SetByte(912, 0);
	GameFlag_SetByte(920, 0);
	GameFlag_SetByte(928, 0);
	GameFlag_SetByte(936, 0);
}

s32 SceneDialogue_RunFlagGatedPromptInteraction(s32 speaker, s32 competitor)
{
	s32 scene;
	s32 message;
	s32 choice;

	Battle_ResetEffectCounter();
	UiWork_PushValueSlot(competitor, 5);
	scene = gGameState.scene;
	if (scene == (s32)&SceneId_KorosseoKawa)
	{
		message = (s32)MsgKorosseoStageFirstFinalsMatch;
	}
	else if (scene == (s32)&SceneId_KorosseoKabe)
	{
		message = (s32)MsgKorosseoStageSecondFinalsMatch;
	}
	else
	{
		message = (s32)MsgKorosseoStageThirdFinalsMatch;
	}
	Engine_EventSetMessage(message);
	Engine_EventShowMessage(speaker, 0);
	if (Engine_GameFlagIsSet(competitor + 512) != 0)
	{
		return 2;
	}
	if (Engine_GameFlagIsSet(competitor + 520) != 0)
	{
		choice = PartyTalkMenu_Choose(0);
		if (choice == 1)
		{
			return 2;
		}
		if (choice == 2 || choice == -1)
		{
			return 3;
		}
		return choice;
	}
	Engine_GameFlagSet(competitor + 520);
	Engine_EventSetMessage((s32)MsgKorosseoWouldYouLikeHearDescription);
	Engine_EventOpenMessage(speaker, 0);
	return Engine_EventChooseYesNo(0, 0);
}

void Korosseo_ShowFollowUpPrompt(s32 speaker, s32 competitor)
{
	s32 scene;
	s32 message;

	UiWork_PushValueSlot(competitor, 5);
	scene = gGameState.scene;
	if (scene == (s32)&SceneId_KorosseoKawa)
	{
		message = (s32)MsgKorosseoStageFirstFinalsMatch;
	}
	else if (scene == (s32)&SceneId_KorosseoKabe)
	{
		message = (s32)MsgKorosseoStageSecondFinalsMatch;
	}
	else
	{
		message = (s32)MsgKorosseoStageThirdFinalsMatch;
	}
	Engine_EventSetMessage(message + 1);
	Engine_EventShowMessage(speaker, 0);
}

void FieldScene_RunMiddleSequence(s32 mode, s32 owner, s32 base)
{
	struct FieldActor *rec;
	struct FieldActor *record;
	s32				   p9;
	s32				   p11;
	s32				   count;
	s32				   state;
	s32				   obj;
	s32				   hi;
	s32				   lo;
	s32				   tail;
	s32				   sx;
	s32				   sy;
	s32				   i;
	u8				   buf[8];

	rec = Object_GetById(owner);
	p9 = rec->x.part.pixel;
	p11 = rec->z.part.pixel;
	if (mode != 3)
	{
		count = GetPartyMemberCount();
		for (i = 0; i < count; i++)
		{
			buf[i] = gGameState.active_owners[i];
		}
		if (count <= 1)
		{
			Engine_EventSetMessage((s32)MsgKorosseoDoYourBest);
			Event_ShowMessage(owner, 0);
			return;
		}
		if (GameFlag_IsSet(base + 512) != 0)
		{
			Engine_EventSetMessage((s32)MsgKorosseoUnfortunatelyWeHaveFullHouse);
			Event_ShowMessage(owner, 0);
			return;
		}
		if (mode == 2)
		{
			state = 0;
			Engine_TaskWait(6);
		}
		else
		{
			Engine_EventSetMessage((s32)MsgKorosseoWouldLikeFriendCheerFor);
			Event_OpenMessage(owner, 0);
			state = Engine_EventChooseYesNo(0, 0);
		}
		if (state == 0)
		{
			if (state < count)
			{
				for (i = 0; i < count; i++)
				{
					Party_RemoveActiveOwner((s8)buf[i]);
				}
			}
			for (i = 0; i < count; i++)
			{
				if ((s8)buf[i] != 0)
				{
					Party_AddActiveOwner((s8)buf[i]);
				}
			}
			obj = Menu_OpenCharacterSelector();
			for (i = 0; i < count; i++)
			{
				Party_RemoveActiveOwner((s8)buf[i]);
			}
			for (i = 0; i < count; i++)
			{
				Party_AddActiveOwner((s8)buf[i]);
			}
			if (obj != -1)
			{
				goto L_main;
			}
		}
	}
	Engine_EventSetMessage((s32)MsgKorosseoIfKnowWhoWantCheer);
	Event_ShowMessage(owner, 0);
	return;
L_main:
	UiWork_PushValueSlot(obj, 1);
	Engine_EventSetMessage((s32)MsgKorosseoRobinWillCheerForWay);
	Event_ShowMessage(owner, 0);
	Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
	Actor_SetSpeed(obj, 0x10000, 0x8000);
	Actor_SetSpeed(owner, 0x10000, 0x8000);
	record = Object_GetById(0);
	if (record != 0)
	{
		Actor_SetPosition(obj, record->x.fixed, record->z.fixed);
	}
	hi = p11 + 16;
	Actor_WalkToAndWait(obj, p9, hi);
	lo = p9 + 16;
	Engine_ActorWalkToAndWait(0, lo, hi);
	Engine_ActorFaceEachOther(obj, ACTOR_PARTY_LEADER, 30);
	Engine_ActorSetAnimation(obj, 3);
	tail = hi - 32;
	Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
	Actor_WalkToAndWait(owner, p9, tail);
	Engine_ActorWalkTo(owner, lo, tail);
	Object_LinkObjectAndSetCallback(0, obj);
	Actor_WalkToAndWait(obj, p9, tail);
	Engine_ActorSetAnimation(owner, 1);
	Actor_FaceDirection(owner, 0x8000, 0);
	Actor_WalkToAndWait(obj, p9, p11 - 48);
	Actor_WalkToAndWait(owner, p9, tail);
	Actor_WalkToAndWait(owner, p9, p11);
	Party_RemoveActiveOwner(obj);
	GameFlag_Set(base + 512);
	rec = Object_GetById(obj);
	sx = rec->x.fixed >> 20;
	GameFlag_SetByte((obj << 4) + 880, sx);
	sy = rec->z.fixed >> 20;
	GameFlag_SetByte((obj << 4) + 888, sy);
}

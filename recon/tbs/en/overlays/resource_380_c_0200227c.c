/* NONMATCHING: complete 388-byte owner; typed candidate 400 bytes,
 * 121 differing halfwords / 53 aligned edits (2026-09-27).
 * The exact UpdateOverlayObjectAngle consumer in PARTY_INTRO.C establishes
 * linked_object at +0x68 and the orientation flag at +0x5a.
 * Baseline is 388 bytes / 15 differing halfwords / 14 aligned edits.
 * H1 transfers that owned layout and FIELD_EVENT.H call interfaces,
 * including void EventEnd, while naming the existing shared script.
 * First leader-pointer store now uses the correct r1. Remaining: the script
 * takes r6 and the flag mask r8, opposite the reference; repeated mask
 * copies add 12 bytes. Actor 9/10 leader reloads, script/zero setup, actor
 * 14 motion/zero ordering, and actor 13 final OR still differ. No DONE. */
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "OVERLAY_OBJECT.H"

union LinkedActor {
    struct FieldActor actor;
    struct OverlayObject link;
};

extern const s32 gLinkedActorScript[];

/* FAKEMATCH: preserve the exact neighbour's coordinate argument construction. */
static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

void Local_0200227c(void)
{
    struct OverlayObject *leader;
    union LinkedActor *special;
    union LinkedActor *actor;
    s32 none;

    leader = (struct OverlayObject *)Engine_ActorGet(0);
    Engine_EventBegin();
    Engine_ActorEnableActionCallback(5, 1);
    Engine_ActorEnableActionCallback(9, 1);
    Engine_ActorEnableActionCallback(11, 1);
    Engine_ActorEnableActionCallback(10, 1);
    Engine_ActorEnableActionCallback(14, 1);
    Engine_ActorEnableActionCallback(13, 1);
    Call3(Engine_ActorSetPosition, 5, 0x1db0000, 0x14c0000);
    Call3(Engine_ActorSetPosition, 9, 0x1eb0000, 0x14c0000);
    Call3(Engine_ActorSetPosition, 11, 0x1cb0000, 0x15c0000);
    Call3(Engine_ActorSetPosition, 10, 0x1fb0000, 0x15c0000);
    Call3(Engine_ActorSetPosition, 14, 0x1cc0000, 0x1680000);
    Call3(Engine_ActorSetPosition, 13, 0x1d70000, 0x1320000);
    actor = (union LinkedActor *)Engine_ActorGet(5);
    actor->link.linked_object = leader;
    actor->link.unknown_5a |= 1;
    none = 0;
    Engine_ObjectSetScript(&actor->actor, gLinkedActorScript);
    actor = (union LinkedActor *)Engine_ActorGet(9);
    actor->link.linked_object = leader;
    actor->link.unknown_5a |= 1;
    Engine_ObjectSetScript(&actor->actor, gLinkedActorScript);
    actor = (union LinkedActor *)Engine_ActorGet(11);
    actor->link.linked_object = leader;
    actor->link.unknown_5a |= 1;
    Engine_ObjectSetScript(&actor->actor, gLinkedActorScript);
    actor = (union LinkedActor *)Engine_ActorGet(10);
    actor->link.linked_object = leader;
    actor->link.unknown_5a |= 1;
    Engine_ObjectSetScript(&actor->actor, gLinkedActorScript);
    special = (union LinkedActor *)Engine_ActorGet(14);
    special->link.linked_object = leader;
    special->link.unknown_5a |= 1;
    special->actor.scale_x = 0x10000;
    special->actor.scale_y = 0x10000;
    special->actor.motion_flags = Engine_ActorGet(11)->motion_flags;
    special->actor.y.fixed = none;
    Engine_ObjectSetScript(&special->actor, gLinkedActorScript);
    actor = (union LinkedActor *)Engine_ActorGet(13);
    actor->link.linked_object = leader;
    {
        u8 value = *(volatile u8 *)&actor->link.unknown_5a;
    
        actor->link.unknown_5a = (u8)(value | 1);
    }
    Engine_ObjectSetScript(&actor->actor, gLinkedActorScript);
    Engine_EventEnd();
}

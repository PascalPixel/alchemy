/* NONMATCHING: 740 of 740 bytes, 2 halfword edits (2026-09-24). Hand-written from the
 * resolved jump-table disassembly as a single-overlay unit binding Engine_* at
 * their import veneers. Remaining: 2 halfwords: inside the list branch the reference copies the hoisted -1 (r5) into r8 before setting p = list (r6); this draft sets p first. The blend writes now match: each is wrapped in a do/while (FAKEMATCH when closed) so the value loads before the register address. */
#include "TYPES.H"
#include "FIELD_EVENT.H"

void SceneState_InitFourActorRecordsAndInstallTask(void);
void Main_08015120(s32 value, s32 digits);
void Main_080b0060(void);
void Local_020008f8(s32 mode);
s32 SceneDialogue_PickTopicVariantId(s32 topic);

union GameStateRows {
    u8 bytes[512][2];
    s16 halves[512][1];
    s32 words[256];
};

extern union GameStateRows Data_02000240_t;
extern u8 Data_000000bd[];
extern u8 TorebiIzumi_AndYouWonMessage;

static __inline__ void Io_SetBlendControl(s32 value)
{
    *(volatile u16 *)0x04000050 = value;
}

static __inline__ void Io_SetBlendAlpha(s32 value)
{
    *(volatile u16 *)0x04000052 = value;
}

s32 Local_020001d8(void)
{
    s32 diff;
    s8 *list;
    s8 *p;
    s32 item;

    if (Data_02000240_t.halves[224][0] == (s32)Data_000000bd) {
        gEventWork->start_transition = 0x100;
        do { Io_SetBlendControl(0x3f42); } while (0);
        do { Io_SetBlendAlpha(0x80c); } while (0);
        Engine_ActorSetAnimation(24, 2);
        Engine_ActorSetAnimation(25, 2);
        Engine_ActorGet(24)->scale_x = -0x10000;
        Engine_ActorGet(25)->scale_x = -0x10000;
        Engine_ActorGet(24)->priority_flags = 2;
        Engine_ActorGet(25)->priority_flags = 2;
        Engine_EventOpenScreen();
        if (Data_02000240_t.halves[225][0] == 1) {
            SceneState_InitFourActorRecordsAndInstallTask();
            if (Engine_GameFlagIsSet(0x200)) {
                Io_SetBlendControl(0x3f42);
                Io_SetBlendAlpha(0x1000);
            }
        }
    } else {
        if (Engine_GameFlagIsSet(0x950)) {
            Engine_ActorSetPosition(17, 0, 0);
        }
        *(u8 *)0x03001d18 = 1;
        gEventWork->start_transition = 0x209;
        if (Data_02000240_t.halves[225][0] == 10) {
            Engine_ActorSetChildValue(8, 1);
            Engine_ActorSetChildValue(9, 2);
        }
        if (Data_02000240_t.halves[225][0] == 13 && !Engine_GameFlagIsSet(0x109)) {
            Engine_EventBegin();
            Engine_ActorSetChildValue(8, 1);
            Engine_ActorSetChildValue(9, 2);
            Engine_EventOpenScreen();
            Engine_EventWaitForScreen();
            Engine_EventWait(10);
            Engine_ActorWalkToAndWait(0, 120, 112);
            Engine_EventWait(20);
            diff = Data_02000240_t.words[4] - *(s32 *)0x02001000;
            if (diff > 0) {
                if (diff > 19999) {
                    Engine_AudioPlayCue(93);
                } else if (diff > 4999) {
                    Engine_AudioPlayCue(92);
                } else {
                    Engine_AudioPlayCue(91);
                }
                Engine_EventWait(20);
                Engine_EventSetMessage(0xe13);
                Main_08015120(diff, 5);
                Engine_EventShowMessage(9, 0);
                Main_080b0060();
            } else if (diff < 0) {
                Engine_EventSetMessage(0xe14);
                Main_08015120(-diff, 5);
                Engine_EventShowMessage(9, 0);
            }
            Engine_EventEnd();
        }
        if (Data_02000240_t.halves[225][0] == 12 && !Engine_GameFlagIsSet(0x109)) {
            list = (s8 *)Data_02000240_t.bytes[150];
            Engine_EventBegin();
            Engine_EventOpenScreen();
            Engine_EventWaitForScreen();
            Engine_EventWait(10);
            if (list[0] == -1) {
                Local_020008f8(1);
            } else if (list[0] != -2) {
                Engine_EventSetMessage(0xe2e);
                Engine_EventShowMessage(8, 0);
                if (*list != -1) {
                    for (p = list; *p != -1; p++) {
                        if (p == list) {
                            Engine_EventSetMessage(0xe2f);
                        } else {
                            Engine_EventSetMessage((s32)&TorebiIzumi_AndYouWonMessage);
                        }
                        item = SceneDialogue_PickTopicVariantId(*p);
                        Main_08015120(item, 2);
                        Engine_EventShowMessage(8, 0);
                        Engine_ItemShowFound(item, 3);
                        Engine_PartyGiveItem(item, 0);
                        Engine_EventWait(10);
                        Engine_ActorFaceDirection(0, 0xc000, 0);
                        Engine_EventWait(30);
                    }
                }
                Data_02000240_t.bytes[150][0] = 0xfe;
                Engine_EventSetMessage(0xe31);
                Engine_EventShowMessage(8, 0);
            }
            Engine_EventEnd();
        }
    }
    return 0;
}

#include "TYPES.H"
#define gEventWork Data_03001ebc
#define gGameState Data_02000240

/* main:08091a58 PartyInventory_GiveItem - hand-written draft, 89 of 226
   halfwords differ (436 of 452 bytes). Residual: the ROM keeps three
   separate stores of the saved message position and a separate call in
   the leader branch of the discard path, where this C is cross-jumped; the
   position pointer and loop message share r7 in the ROM.

   Gives the party one item. When no member has room, the player picks an
   item to throw away (or gives up the new one), and the text box position
   the event had is restored afterwards. Returns the member who received the
   item, or -1. */

extern u8 Value_0000096a[];
extern u8 Value_00000977[];
extern u8 Value_00000978[];
#define MSG_RECEIVED ((s32)Value_0000096a)
#define MSG_BAGS_FULL ((s32)Value_00000977)
#define MSG_CHOOSE_DISCARD ((s32)Value_00000978)

struct ItemData {
    u8 unknown_00[3];
    u8 flags;
};

struct EventWork {
    u8 unknown_000[472];
    s16 message_position;
};

extern struct EventWork *gEventWork;
extern s32 gGameState[];

s32 Party_FindRoomForItem(s32 item);
struct ItemData *Item_Get(s32 item);
void *Owner_GetStateFar(s32 owner);
s32 Func_08077020(s32 owner, s32 slot);
void Func_080772b0(s32 owner, s32 slot);
void Func_08077240(s32 item, s32 delta);
void UiWork_PushValueSlotFar(s32 value, s32 slot);
void UiText_ShowPositionedMessageAndWaitFar(s32 message, s32 position);
void UiWork_FinalizePendingCoreFar(void);
s32 Func_080b0058(s32 *owner, s32 *slot);
s32 Object_CallSpawnRoutineAtOrigin(s32 mode);
void Audio_PlayCue(s32 cue);

#define Party_FindRoomForItem Func_08077030

s32 PartyInventory_GiveItem(s32 item)
{
    struct EventWork *work;
    s16 *position;
    s16 saved;
    s32 owner;
    s32 message;
    s32 text;
    s32 result;
    s32 count;
    s32 i;
    s32 member;
    s32 slot;

    work = gEventWork;
    position = &work->message_position;
    saved = *position;
    owner = Party_FindRoomForItem(item);
    if (owner == -1) {
        UiWork_PushValueSlotFar(item, 2);
        UiText_ShowPositionedMessageAndWaitFar(MSG_RECEIVED, 1);
        UiText_ShowPositionedMessageAndWaitFar(MSG_BAGS_FULL, 1);
    retry:
        do {
            message = MSG_CHOOSE_DISCARD;
            UiText_ShowPositionedMessageAndWaitFar(message, 1);
            result = Func_080b0058(&member, &slot);
            if (result == -1) {
                if (Item_Get(item)->flags & 8) {
                    UiWork_PushValueSlotFar(item, 2);
                    UiText_ShowPositionedMessageAndWaitFar(message + 4, 1);
                    goto retry;
                }
                UiWork_PushValueSlotFar(item, 2);
                UiText_ShowPositionedMessageAndWaitFar(message + 1, 5);
                result = Object_CallSpawnRoutineAtOrigin(1);
                UiWork_FinalizePendingCoreFar();
                if (result != 0)
                    goto retry;
                Func_08077240(item, 1);
                UiWork_PushValueSlotFar(item, 2);
                UiText_ShowPositionedMessageAndWaitFar(message + 2, 1);
                work->message_position = saved;
            } else {
                Owner_GetStateFar(member);
                count = Func_08077020(member, slot);
                for (i = 0; i < count; i++)
                    Func_080772b0(member, slot);
                owner = Party_FindRoomForItem(item);
                Audio_PlayCue(83);
                if (owner == gGameState[125]) {
                    UiWork_PushValueSlotFar(item, 2);
                    UiText_ShowPositionedMessageAndWaitFar(MSG_RECEIVED, 3);
                } else {
                    UiWork_PushValueSlotFar(item, 2);
                    UiWork_PushValueSlotFar(owner, 1);
                    UiText_ShowPositionedMessageAndWaitFar(MSG_RECEIVED + 1, 3);
                }
                work->message_position = saved;
                return owner;
            }
        } while (0);
    } else {
        Audio_PlayCue(83);
        UiWork_PushValueSlotFar(item, 2);
        text = MSG_RECEIVED;
        UiText_ShowPositionedMessageAndWaitFar(text, 3);
        if (owner != gGameState[125]) {
            UiWork_PushValueSlotFar(item, 2);
            UiWork_PushValueSlotFar(owner, 1);
            UiText_ShowPositionedMessageAndWaitFar(text + 1, 3);
        }
        *position = saved;
    }
    return owner;
}

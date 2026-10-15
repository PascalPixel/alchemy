/* Near miss: 080d41fc differs by the second message increment's
 * scheduling around the stacked window position, 080d4330 by the
 * interleaving of the portrait load with the window offset. */
#include "types.h"
#include "ram_buffer.h"
#include "party_state.h"

struct InputState {
    u32 held;
    u32 pressed;
    u32 released;
    u32 repeat;
};

struct TalkWork {
    u8 unk000[0x1c4];
    u16 message;
};

struct TalkWindowWork {
    u8 unk0000[0x1384];
    u16 speaker;
    u16 portrait;
};

struct PartyTalk {
    u8 unk000[0x22c];
    u8 language;
};

extern volatile struct InputState gInput;
extern const u8 gData_080f330c[];

s32 ObjectTable_ReadActiveValue(s32 id);
s32 Func_080d1eac(s32 value);
s32 UiText_OpenMessageWindowFar(s32 message, s32 x, s32 y, s32 packed);
void Func_080380f8(s32 value, s32 zero, s32 x, s32 y);
s32 UiWork_IsCompleteFar(void);
s32 UiWork_IsIdleFar(s32 window);
void UiWork_FinalizeEntityMatchingLocalizedIdFar(s32 value);
void UiWork_FinalizePendingCoreFar(void);
void WaitFrames(s32 frames);

void Func_080d41fc(s32 first, s32 x1, s32 y1, s32 face_x1, s32 face_y1, s32 second, s32 x2,
                   s32 y2, s32 face_x2, s32 face_y2, s32 delay)
{
    struct TalkWork *work = Ram_HeapSlots->eventWork;
    s32 one = ObjectTable_ReadActiveValue(first);
    s32 two = ObjectTable_ReadActiveValue(second);
    s32 window1;
    s32 window2;

    window1 = UiText_OpenMessageWindowFar((s16)work->message++, x1, y1, Func_080d1eac(one) << 16);
    Func_080380f8(one, 0, face_x1, face_y1);
    if (delay != 0) {
        while (!UiWork_IsCompleteFar())
            WaitFrames(delay);
    }
    window2 = UiText_OpenMessageWindowFar((s16)work->message++, x2, y2, Func_080d1eac(two) << 16);
    Func_080380f8(two, 0, face_x2, face_y2);
    while (!UiWork_IsCompleteFar())
        WaitFrames(1);
    WaitFrames(1);
    while (!(gInput.pressed & 0x303))
        WaitFrames(1);
    WaitFrames(1);
    UiWork_FinalizeEntityMatchingLocalizedIdFar(one);
    UiWork_FinalizeEntityMatchingLocalizedIdFar(two);
    UiWork_FinalizePendingCoreFar();
    WaitFrames(1);
    while (!UiWork_IsIdleFar(window1))
        WaitFrames(1);
    while (!UiWork_IsIdleFar(window2))
        WaitFrames(1);
    WaitFrames(1);
}

void Func_080d4330(s32 id)
{
    struct TalkWindowWork *work = Ram_HeapSlots->windowTiles;

    if (id == (s32)0x80000000) {
        work->speaker = 0;
        work->portrait = 0;
    } else {
        s32 speaker = Func_080d1eac(ObjectTable_ReadActiveValue(id));
        s32 portrait = gData_080f330c[((struct PartyTalk *)&gPartyState)->language];

        /* FAKEMATCH: loads the portrait before it builds the window offset. */
        __asm__("");
        work->speaker = speaker;
        work->portrait = portrait;
    }
}

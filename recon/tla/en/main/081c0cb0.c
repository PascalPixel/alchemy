/* AudioCommand_Play / AudioCommand_GetDefaultPreset: private TLA sound-command
 * reconstruction, 2026-10-02. Intended canonical draft: 081c0cb0.c.
 *
 * The native command extent is 384 bytes including its two literal pools and
 * anonymous trailing zero halfword. The adjacent preset helper is four bytes.
 * Fresh ordinary-compiler source links in JA/EN/DE/ES/FR/IT all have the
 * same measured result. Every actual BL operand and symbolic pool word
 * agrees with that edition's native command. The current raw 388-byte bank
 * independently links exactly in all six; no source adoption is claimed.
 * This candidate emits 4 + 382 bytes; the command has fourteen differing
 * instruction bytes in two ordering sites (+0x10..13, +0xce..d7) and does not
 * emit the trailing two alignment bytes. It is a draft and carries no credit.
 *
 * The initial plain range tree emitted 382/264 differing bytes in EN.
 * Eight ordinary range/type/lifetime forms emitted 378..394/324..359.
 * Five capture/argument forms emitted 378..382/16..329; the retained r3 tied
 * handoff is best. Seven further mask/argument dependencies and one memory
 * boundary kept 382/16. Two short-r7 forms incorrectly collapsed to 24 bytes
 * and were rejected. These finite measurements include length differences;
 * they do not prove those rejected shapes correct or claim source adoption.
 *
 * A second bounded cohort of sixteen forms, baseline included, left the
 * retained result unchanged. Five ordinary mask/zero/high-half/comma forms
 * measured 382/16,34,258; simple tied mask/high-argument boundaries stayed
 * 382/16. A short r7 capture with explicit tied inputs measured 382/48;
 * field-dependent argument boundaries grew to 386..390/171..184. None of
 * those follow-up devices is retained here. Complete EN follow-up trials
 * (command bytes / whole 388-byte bank differences, length included):
 *   baseline: 382/16.
 *   ordinary-mask-after-capture: 382/258.
 *   ordinary-mask-after-guard: 382/34.
 *   ordinary-jingle-zero-local: 382/16.
 *   ordinary-jingle-high-half: 382/16.
 *   ordinary-jingle-comma: 382/16.
 *   mask-tied-after-handoff: 382/16.
 *   jingle-high-tied-target: 382/16.
 *   jingle-high-r0-handoff: 382/16.
 *   jingle-two-field-boundaries: 386/173.
 *   jingle-r0-two-field-boundaries: 386/173.
 *   mask-short-r7-explicit-ties: 382/48.
 *   jingle-high-r0-earlyclobber: 382/16.
 *   jingle-lower-input-field-boundary: 386/171.
 *   jingle-two-input-only-boundaries: 386/174.
 *   jingle-r0-two-input-only-boundaries: 390/184.
 * The actual final source body is proved in all six. Current raw listings
 * retain placeholder function names, so ordinary drafts reports this
 * semantic-name proposal as unlisted; complete private links are separate.
 *
 * One saved shared-owner source form was freshly compiled in all twelve
 * editions on2026-10-02. The complete TBS358-byte command remains exact
 * (7 calls,16 symbolic words each). TLA preset4 + command382 still emits
 * 386/native388 and differs16 bytes including the missing zero2 tail,
 * with10 calls and16 symbolic words each. Five editions also lack the
 * proposed physical preset label. The namespace mapping is judgment-only;
 * no shared owner, name, padding, linker row or C credit was adopted.
 * This single form did not improve the retained canonical near miss.
 *
 * SongEntry, PlayerSlot and SoundPlayer are the current shared typed owners.
 * The busy capture is released to an ordinary local before every later call.
 */
#include "AUDIO_ENGINE.H"

struct AudioFrameState {
    u8 unknown_00[3];
    u8 busy;
};

extern const struct SongEntry Sound_SongTable[];
extern const struct PlayerSlot Sound_PlayerSlots[];
extern struct AudioFrameState Data_03001138;
extern u8 Data_02005814;
extern u16 Data_02005820[8];
extern u16 Data_02005830;
extern u16 Data_02005810;
extern u16 gMusicVolume;
extern u16 gMusicVolumeTarget;
extern u8 gMusicRestoreDelay;
extern struct SoundPlayer gMusicPlayerBgm;
extern struct SoundPlayer Data_02006b60;
extern struct SoundPlayer Data_02006990;

void Func_081c11ac(s32 cue);
void MusicPlayer_FadeOut(struct SoundPlayer *player, u16 speed);
void MusicPlayer_StartSong(struct SoundPlayer *player,
    const struct SequenceHeader *header);
void MusicPlayer_SetVolume(struct SoundPlayer *player, u16 mask, u16 volume);
void Audio_PlaySound(u16 cue);
void Sound_LoadPresetParameters(s32 preset);

/* The native neighbor returns preset 3 and ignores the incoming cue argument.
 * Keep the argument because the command passes it in the native ABI. */
s32 AudioCommand_GetDefaultPreset(s32 cue)
{
    return 3;
}

void AudioCommand_Play(s32 cue)
{
    s32 flags;
    s32 slot;
    u32 busy;

    Func_081c11ac(cue);
    flags = cue & 0xf000;
    {
        /* FAKEMATCH: eight ordinary type/lifetime forms load the busy byte directly into r7; the native sequence first captures r3, then hands it to a normal local before any call. */
        register u32 read __asm__("r3");
        read = Data_03001138.busy;
        /* FAKEMATCH: this empty boundary preserves the measured r3 capture and handoff; it changes no value and ends before the branch's calls. */
        __asm__ volatile("" : "=r"(busy) : "0"(read));
    }
    if (busy != 0)
        return;
    cue &= 0xfff;

    if (cue == 78) {
        if (Data_02005814 != 0)
            return;
        MusicPlayer_FadeOut(&gMusicPlayerBgm, 7);
        Data_02005814++;
        Data_02005830 = busy;
    } else if (cue == 390) {
        Data_02005820[3] = busy;
        MusicPlayer_FadeOut(&Data_02006b60, 3);
        Data_02005820[5] = busy;
        MusicPlayer_FadeOut(&Data_02006990, 3);
    } else if (cue == 395) {
        Data_02005820[5] = busy;
        MusicPlayer_FadeOut(&Data_02006990, 3);
    } else {
        if (cue >= 700)
            goto background;
        if (cue > 99) {
            slot = Sound_SongTable[cue].slot;
            if (slot == 7) {
            next:
                if (((u8 *)&Sound_PlayerSlots[slot].player->status)[0] != 0) {
                    slot--;
                    if (slot > 3)
                        goto next;
                    slot = 7;
                }
            }
            MusicPlayer_StartSong(Sound_PlayerSlots[slot].player,
                Sound_SongTable[cue].header);
            Data_02005820[slot] = cue;
        } else if (cue > 79) {
            MusicPlayer_SetVolume(&gMusicPlayerBgm, 255, 0);
            gMusicVolumeTarget = busy;
            gMusicVolume = busy;
            Audio_PlaySound(cue);
            gMusicRestoreDelay = 10;
        } else {
            if (cue == 79)
                return;
        background:
            if (cue == Data_02005830)
                return;
            Data_02005830 = cue;
            Sound_LoadPresetParameters(AudioCommand_GetDefaultPreset(cue));
            Audio_PlaySound(cue);
            if (flags & 0x1000)
                *(s16 *)&gMusicVolume = 0;
            else
                *(s16 *)&gMusicVolume = 0x100;
            *(s16 *)&gMusicVolumeTarget = 0x100;
            *(s16 *)&Data_02005810 = 4;
            Data_02005814 = 0;
        }
    }
}

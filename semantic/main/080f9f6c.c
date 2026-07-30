#include "types.h"

/*
 * 発音開始 (note on) の処理。二つの実行範囲をまとめて再構築する:
 *
 *   Func_080f9f3c (0x080f9f3c, 48バイト) — 左右音量の再計算
 *   Func_080f9f6c (0x080f9f6c, 504バイト実行部) — 発音チャンネルの確保と設定
 *
 * 併記の理由: Func_080f9f3c は push/pop を持たない葉の補助関数で、引数を
 * 呼び出し元の r4 (チャンネル) と r5 (トラック) から直接受け取る。
 * これは呼び出し規約というより「呼び出し元のレジスタ二本を暗黙の引数に
 * した内部補助関数」で、明示的な二引数として綴れば忠実になる。
 *
 * 註記: Func_080f9f3c は 0x080f9c90 からも同じ r4/r5 規約で呼ばれる。
 * その所有者を変換する際も同じ二引数の綴りを使うこと。
 *
 * ordinary-blockers.json は Func_080fa678 が call-via-r1 スランクに届く
 * ことも挙げていたが、それは 080fa678 自身の内部事情で、こちらからは
 * 通常の一引数呼び出しである (src/080fa678.c が既にバイト一致で存在する)。
 */

struct Voice_080f9f6c {
    u8 type;        /* 下位3ビットが音源種別、0x40 でキー分割、0x80 で分割音色 */
    u8 key;
    u8 length;
    u8 pan_sweep;
    void *wave;     /* 分割音色では下位音色表 */
    u32 envelope;   /* キー分割では分割表のアドレス */
};

struct Channel_080f9f6c {
    u8 status;              /* 0 */
    u8 type;                /* 1 */
    u8 right_volume;        /* 2 */
    u8 left_volume;         /* 3 */
    u32 envelope;           /* 4 */
    u8 key;                 /* 8 */
    u8 unknown_09[3];       /* 9 */
    u16 pseudo_echo;        /* 12 */
    u16 unknown_0e;         /* 14 */
    u8 gate_time;           /* 16 */
    u8 key_copy;            /* 17 */
    u8 velocity;            /* 18 */
    u8 priority;            /* 19 */
    s8 pan;                 /* 20 */
    u8 unknown_15[9];       /* 21 */
    u8 length;              /* 30 */
    u8 sweep;               /* 31 */
    u32 frequency;          /* 32 */
    u32 wave;               /* 36 */
    u8 unknown_28[4];       /* 40 */
    struct Track_080f9f6c *owner;      /* 44 */
    struct Channel_080f9f6c *previous; /* 48 */
    struct Channel_080f9f6c *next;     /* 52 */
    u8 unknown_38[8];       /* 56 */
};

struct Track_080f9f6c {
    u8 flags;               /* 0 */
    u8 unknown_01[3];       /* 1 */
    u8 gate_time;           /* 4 */
    u8 key;                 /* 5 */
    u8 velocity;            /* 6 */
    u8 unknown_07;          /* 7 */
    s8 key_shift;           /* 8 */
    u8 pitch;               /* 9 */
    u8 unknown_0a[6];       /* 10 */
    u8 volume_right;        /* 16 */
    u8 volume_left;         /* 17 */
    u8 unknown_12[9];       /* 18 */
    u8 modulation;          /* 27 */
    u8 modulation_active;   /* 28 */
    u8 velocity_offset;     /* 29 */
    u16 pseudo_echo;        /* 30 */
    struct Channel_080f9f6c *chain; /* 32 */
    struct Voice_080f9f6c voice;    /* 36 */
    u8 unknown_30[16];      /* 48 */
    u8 *command;            /* 64 */
};

struct Player_080f9f6c {
    u8 unknown_00[9];
    u8 velocity_offset;     /* 9 */
};

struct SoundInfo_080f9f6c {
    u8 unknown_00[6];
    u8 channel_count;                    /* 6 */
    u8 unknown_07[21];
    struct Channel_080f9f6c *cgb_channels; /* 28 */
    u8 unknown_20[16];
    u32 pcm_rate;                        /* 48 */
    u8 unknown_34[28];
    struct Channel_080f9f6c channels[1]; /* 80 */
};

/* 0x080fba14 の表は演奏コマンド番号を発音長に変換する。 */
extern const u8 Data_080fba14[];

void Func_080fa678(struct Channel_080f9f6c *channel);
void Func_080fa1ac(u32 depth, struct Track_080f9f6c *track);
void Func_080fac44(struct Player_080f9f6c *player,
                   struct Track_080f9f6c *track);
u32 Func_080f9ee8(u32 type, s32 key, u8 pitch, u32 rate);
u32 Func_080fa1fc(void *wave, u8 key, u8 pitch);

/*
 * 引数は元のコードでは r4 (チャンネル) と r5 (トラック)。push/pop を
 * 持たないため、呼び出し元のレジスタをそのまま使う内部補助関数である。
 */
void Func_080f9f3c(struct Channel_080f9f6c *channel,
                   struct Track_080f9f6c *track)
{
    s32 velocity = channel->velocity;
    s32 pan = channel->pan;
    s32 scaled;

    scaled = ((128 + pan) * velocity) * track->volume_right;
    scaled >>= 14;
    if ((u32)scaled > 255)
        scaled = 255;
    channel->right_volume = (u8)scaled;

    scaled = ((127 - pan) * velocity) * track->volume_left;
    scaled >>= 14;
    if ((u32)scaled > 255)
        scaled = 255;
    channel->left_volume = (u8)scaled;
}

void Func_080f9f6c(u32 command, struct Player_080f9f6c *player,
                   struct Track_080f9f6c *track)
{
    struct SoundInfo_080f9f6c *sound_info =
        *(struct SoundInfo_080f9f6c **)0x03007ff0;
    struct Voice_080f9f6c *voice;
    struct Channel_080f9f6c *channel;
    s32 pan = 0;
    s32 key;
    s32 priority;
    u32 type;
    s32 pitch;

    track->gate_time = Data_080fba14[command];

    /* 続く 0x80 未満のコマンドバイトは音高・強さ・長さの追加指定。 */
    {
        u8 *command_stream = track->command;

        if (*command_stream < 128) {
            track->key = *command_stream++;
            if (*command_stream < 128) {
                track->velocity = *command_stream++;
                if (*command_stream < 128) {
                    track->gate_time += *command_stream++;
                }
            }
            track->command = command_stream;
        }
    }

    if ((track->voice.type & 0xc0) != 0) {
        u32 index;

        key = track->key;
        if ((track->voice.type & 0x40) != 0)
            index = ((const u8 *)track->voice.envelope)[key];
        else
            index = (u32)key;

        voice = (struct Voice_080f9f6c *)track->voice.wave + index;
        if ((voice->type & 0xc0) != 0)
            return;

        if ((track->voice.type & 0x80) != 0) {
            if ((voice->pan_sweep & 0x80) != 0)
                pan = ((s32)voice->pan_sweep - 192) * 2;
            key = voice->key;
        }
    } else {
        voice = &track->voice;
        key = track->key;
    }

    priority = track->velocity_offset + player->velocity_offset;
    if (priority > 255)
        priority = 255;

    type = voice->type & 7;
    if (type != 0) {
        /* CGB 音源は種別ごとに一本しかない。 */
        channel = sound_info->cgb_channels;
        if (channel == 0)
            return;
        channel = (struct Channel_080f9f6c *)((u8 *)channel + (type - 1) * 64);

        if ((channel->status & 0xc7) != 0 && (channel->status & 0x40) == 0) {
            if (channel->priority > (u32)priority)
                return;
            if (channel->priority == (u32)priority &&
                (u32)channel->owner < (u32)track) {
                return;
            }
        }
    } else {
        /* PCM は空きを探し、無ければ優先度の低いものを奪う。 */
        struct Channel_080f9f6c *candidate = sound_info->channels;
        struct Channel_080f9f6c *chosen = 0;
        struct Track_080f9f6c *best_owner = track;
        s32 best_priority = priority;
        s32 stopping_seen = 0;
        s32 remaining = sound_info->channel_count;

        do {
            if ((candidate->status & 0xc7) == 0) {
                chosen = candidate;
                break;
            }
            if ((candidate->status & 0x40) != 0 && stopping_seen == 0) {
                /* 停止中のチャンネルが見つかった時点で候補を作り直す。 */
                stopping_seen = 1;
                best_priority = candidate->priority;
                best_owner = candidate->owner;
                chosen = candidate;
            } else if ((candidate->status & 0x40) != 0 || stopping_seen == 0) {
                if (candidate->priority < (u32)best_priority) {
                    best_priority = candidate->priority;
                    best_owner = candidate->owner;
                    chosen = candidate;
                } else if (candidate->priority == (u32)best_priority) {
                    if ((u32)candidate->owner > (u32)best_owner) {
                        best_owner = candidate->owner;
                        chosen = candidate;
                    } else if ((u32)candidate->owner == (u32)best_owner) {
                        chosen = candidate;
                    }
                }
            }
            candidate = (struct Channel_080f9f6c *)((u8 *)candidate + 64);
            remaining--;
        } while (remaining > 0);

        if (chosen == 0)
            return;
        channel = chosen;
    }

    Func_080fa678(channel);

    channel->previous = 0;
    channel->next = track->chain;
    if (track->chain != 0)
        track->chain->previous = channel;
    track->chain = channel;
    channel->owner = track;

    track->modulation_active = track->modulation;
    if (track->modulation != 0)
        Func_080fa1ac(track->modulation, track);

    Func_080fac44(player, track);

    /* 元のコードは gate_time/key/velocity の三バイトを 32 ビットで写す。 */
    *(u32 *)&channel->gate_time = *(u32 *)&track->gate_time;

    channel->priority = (u8)priority;
    channel->key = (u8)key;
    channel->pan = (s8)pan;
    channel->type = voice->type;
    channel->wave = (u32)voice->wave;
    channel->envelope = voice->envelope;
    channel->pseudo_echo = track->pseudo_echo;

    Func_080f9f3c(channel, track);

    pitch = channel->key + track->key_shift;
    if (pitch < 0)
        pitch = 0;

    if (type != 0) {
        u8 sweep = voice->pan_sweep;

        channel->length = voice->length;
        if ((sweep & 0x80) != 0 || (sweep & 0x70) == 0)
            sweep = 8;
        channel->sweep = sweep;
        channel->frequency =
            Func_080f9ee8(type, pitch, track->pitch, sound_info->pcm_rate);
    } else {
        channel->frequency =
            Func_080fa1fc(voice->wave, (u8)pitch, track->pitch);
    }

    channel->status = 128;
    track->flags &= 0xf0;
}

// Approved reference provenance: adapted from pret/pokeemerald
// src/m4a.c:FadeOutBody at commit 83df84e40623b79281f2397faa611cbf044170bd.
// Names and layout were checked against this ROM region and exact compiler output.
typedef signed short s16;
typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

struct MusicTrack_080fab7c {
    u8 flags;
    u8 pad01[18];
    u8 volumeScale;
    u8 pad14[60];
};

struct MusicPlayer_080fab7c {
    void *songHeader;
    u32 status;
    u8 trackCount;
    u8 priority;
    u8 command;
    u8 config;
    u32 clock;
    u8 gap10[8];
    u8 *memory;
    u16 tempoD;
    u16 tempoU;
    u16 tempoI;
    u16 tempoC;
    u16 fadeInterval;
    u16 fadeCounter;
    u16 fadeVolume;
    struct MusicTrack_080fab7c *tracks;
};

void Func_080f9ef8(
    struct MusicPlayer_080fab7c *player,
    struct MusicTrack_080fab7c *track);

void Func_080fab7c(struct MusicPlayer_080fab7c *player)
{
    s32 count;
    struct MusicTrack_080fab7c *track;
    u16 fadeVolume;

    if (player->fadeInterval == 0)
        return;
    if (--player->fadeCounter != 0)
        return;

    player->fadeCounter = player->fadeInterval;

    if (player->fadeVolume & 2) {
        if ((u16)(player->fadeVolume += (4 << 2)) >= (64 << 2)) {
            player->fadeVolume = (64 << 2);
            player->fadeInterval = 0;
        }
    } else {
        if ((s16)(player->fadeVolume -= (4 << 2)) <= 0) {
            count = player->trackCount;
            track = player->tracks;

            while (count > 0) {
                u32 temporary;

                Func_080f9ef8(player, track);

                temporary = 1;
                fadeVolume = player->fadeVolume;
                temporary &= fadeVolume;

                if (!temporary)
                    track->flags = 0;

                count--;
                track++;
            }

            if (player->fadeVolume & 1)
                player->status |= 0x80000000;
            else
                player->status = 0x80000000;

            player->fadeInterval = 0;
            return;
        }
    }

    count = player->trackCount;
    track = player->tracks;

    while (count > 0) {
        if (track->flags & 0x80) {
            fadeVolume = player->fadeVolume;

            track->volumeScale = (fadeVolume >> 2);
            track->flags |= 3;
        }

        count--;
        track++;
    }
}

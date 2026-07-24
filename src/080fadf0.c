typedef unsigned char u8;
typedef signed char s8;
typedef unsigned int u32;

struct CgbChannel
{
    u8 pad0[2];
    u8 rightVolume;
    u8 leftVolume;
    u8 pad4[2];
    u8 su;
    u8 pad7[3];
    u8 eg;
    u8 padB[14];
    s8 sg;
    u8 pad1A;
    u8 pan;
    u8 panMask;
};

void Func_080fadf0(struct CgbChannel *chan)
{
    u32 r = chan->rightVolume;
    u32 l = chan->leftVolume;

    if ((u8) r >= (u8) l)
    {
        if (((u8) r >> 1) >= (u8) l)
        {
            chan->pan = 0x0F;
            goto clamp;
        }
    }
    else
    {
        if (((u8) l >> 1) >= (u8) r)
        {
            chan->pan = 0xF0;
            goto clamp;
        }
    }

    chan->pan = 0xFF;
    chan->eg = (u32) (chan->rightVolume + chan->leftVolume) >> 4;
    goto done;

clamp:
    chan->eg = (u32) (chan->rightVolume + chan->leftVolume) >> 4;
    if (chan->eg > 15)
        chan->eg = 15;

done:
    chan->sg = (chan->eg * chan->su + 15) >> 4;
    chan->pan &= chan->panMask;
}

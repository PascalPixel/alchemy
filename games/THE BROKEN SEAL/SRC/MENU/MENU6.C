#include "TYPES.H"

extern u8 *gMenuWork;
s32 Party_ListActiveOwnersFar(void *);
void ResourceObject_ReleaseFar(void *);
void Scheduler_RemoveCallback(void (*callback)(void));
void Menu_UpdateEntryObjectTransforms(void);

s32 Party_CountActiveOwnersFar(void);
void Object_ApplyProjectedPlacementFar(void *, s32 *, s32 *, s32);

void Menu_ReleaseEntryObjects(void)
{
    u32 buf[7];
    u8 *base = gMenuWork;
    s32 count;
    void **p;
    s32 i;

    count = (u16)Party_ListActiveOwnersFar(buf);
    if (count != 0) {
        p = (void **)(base + 276);
        i = count;
        do {
            void *entry = *p++;

            if (entry != 0) {
                ResourceObject_ReleaseFar(entry);
            }
        } while (--i != 0);
    }
    Scheduler_RemoveCallback((void (*)(void))Menu_UpdateEntryObjectTransforms);
}

void Menu_UpdateEntryObjectTransforms(void)
{
    u8 *p;
    s32 pos[2];
    s32 trans[4];
    s32 *pp;
    volatile s32 *tp;
    s16 *hp;
    s32 i;
    s32 cnt;

    p = gMenuWork;
    cnt = (u16)Party_CountActiveOwnersFar();
    i = 0;
    if (i < cnt) {
        pp = pos;
        tp = trans;
        hp = (s16 *)(p + 308);
        p += 276;
        do {
            void *obj;
            s32 top;

            top = 0x01e20000 - (hp[8] << 16);
            obj = *(void **)p;
            if (obj != 0) {
                *((s8 *)obj + 9) &= -13;
                pos[0] = *(s32 *)(p + 64);
                pp[1] = *(s32 *)(p + 64);
                tp[1] = top;
                tp[0] = hp[0] << 16;
                tp[2] = (hp[8] << 16) + top;
                tp[3] = 0;
                Object_ApplyProjectedPlacementFar(obj, (s32 *)tp, pp, 0x4000);
            }
            i++;
            hp++;
            p += 4;
        } while (i < cnt);
    }
}

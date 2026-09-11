#include "types.h"
#include "scene.h"

/* menu/entry/release_entry_objects.c */
extern u8 *gIw;

void ScheduleCallback(void (*callback)(void));
extern u8 gRom;

void Menu_ReleaseEntryObjects(void)
{
    u32 buf[7];
    u8 *base = gIw;
    s32 count;
    void **p;
    s32 i;

    count = (u16)Menu_Check(buf);
    if (count != 0) {
        p = (void **)(base + 276);
        i = count;
        do {
            void *entry = *p++;

            if (entry != 0) {
                Menu_Do(entry);
            }
        } while (--i != 0);
    }
    ScheduleCallback((void (*)(void))&gRom);
}

/* menu/entry/update_entry_object_transforms.c */
extern u8 *gIw;

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

    p = gIw;
    cnt = (u16)Menu_Check();
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
                Menu_SetMode(obj, (s32 *)tp, pp, 0x4000);
            }
            i++;
            hp++;
            p += 4;
        } while (i < cnt);
    }
}

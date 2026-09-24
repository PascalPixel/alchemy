#include "TYPES.H"

extern u8 *gMenuWork;
s32 Party_ListActiveOwnersFar(void *);
void ResourceObject_ReleaseFar(void *);
void Scheduler_RemoveCallback(void (*callback)(void));
void Menu_UpdateEntryObjectTransforms(void);

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

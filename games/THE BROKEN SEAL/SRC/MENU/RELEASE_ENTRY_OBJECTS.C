#include "TYPES.H"

extern u8 *Data_03001f2c;
s32 Party_ListActiveOwnersFar(void *);
void ResourceObject_ReleaseFar(void *);
void ScheduleCallback(void (*callback)(void));
extern u8 Data_080a19a1;

void Menu_ReleaseEntryObjects(void)
{
    u32 buf[7];
    u8 *base = Data_03001f2c;
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
    ScheduleCallback((void (*)(void))&Data_080a19a1);
}

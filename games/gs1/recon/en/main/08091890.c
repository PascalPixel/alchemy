#include "types.h"

struct Rec {
    u8 pad0[20];
    s16 f20;
    s16 f22;
    u8 pad1[28];
    s16 f52;
    s16 f54;
    s16 f56;
    s16 f58;
    u8 pad2[0x131 - 60];
    u8 b131;
};

extern s32 Data_02000240[];

s32 Func_080022ec(s32, s32);
struct Rec *Func_08077008(s32);
s32 Func_08077148(void);
void Func_08077168(void);
void Func_08091858(void);

void Func_08091890(s32 id)
{
    struct Rec *rec;
    s32 v;
    s32 i;
    s32 n;
    s32 count;

    Func_08077168();
    Func_08091858();
    rec = Func_08077008(id);
    rec->f56 = rec->f52;
    rec->f58 = rec->f54;
    v = Func_080022ec(rec->f56 << 14, rec->f52);
    {
        s32 w;
    
        w = 0x4000;
        if (v <= 0x4000) {
            w = 0;
            if (v >= 0) {
                w = v;
            }
        }
        rec->f20 = w;
    }
    if (rec->f20 == 0 && rec->f56 != 0) {
        rec->f20 = 1;
    }
    v = Func_080022ec(rec->f58 << 14, rec->f54);
    {
        s32 w;
    
        w = 0x4000;
        if (v <= 0x4000) {
            w = 0;
            if (v >= 0) {
                w = v;
            }
        }
        rec->f22 = w;
    }
    if (rec->f22 == 0 && rec->f58 != 0) {
        rec->f22 = 1;
    }
    rec->b131 = 0;
    count = 0;
    n = Func_08077148();
    for (i = 0; i < n; i++) {
        u8 *members = (u8 *)Data_02000240;

        rec = Func_08077008(members[0x1f8 + i]);
        if (rec->f56 != 0) {
            count++;
        }
    }
    if (count == 0) {
        rec = Func_08077008(Data_02000240[125]);
        rec->f56 = 1;
        v = Func_080022ec(rec->f56 << 14, rec->f52);
        {
            s32 w;
        
            w = 0x4000;
            if (v <= 0x4000) {
                w = 0;
                if (v >= 0) {
                    w = v;
                }
            }
            rec->f20 = w;
        }
        if (rec->f20 == 0 && rec->f56 != 0) {
            rec->f20 = 1;
        }
        v = Func_080022ec(rec->f58 << 14, rec->f54);
        {
            s32 w;
        
            w = 0x4000;
            if (v <= 0x4000) {
                w = 0;
                if (v >= 0) {
                    w = v;
                }
            }
            rec->f22 = w;
        }
        if (rec->f22 == 0 && rec->f58 != 0) {
            rec->f22 = 1;
        }
    }
}

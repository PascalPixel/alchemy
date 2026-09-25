/* NONMATCHING: 300 bytes, candidate 300, 112 differing halfwords, 82 halfword edits.
 * Fresh reconstruction from the complete listing (2026-09-25).
 * WALL: The polling loop and cancel branch have different block order; size and volatile input reads now agree.
 */
#include "TYPES.H"
struct MenuSelectionState {
    u8 padding000[0x78];
    void *work;
    u8 padding07c[8];
    u8 resource_ids[8];
    s16 selection;
    s16 item_count;
    s16 field090;
    s16 resource_base;
};
extern struct MenuSelectionState *Data_03001f38;
extern u8 Value_0000001f;
extern volatile u32 Data_03001c94, Data_03001b04;
void Func_08016478(void *);
void Func_0801e7c0(s32, void *, s32, s32);
void Func_080030f8(s32);
void Func_080f9010(s32);
s32 Func_08028574(s32 initial)
{
    struct MenuSelectionState *work = Data_03001f38;
    s32 resource;
    work->selection=initial;
    for (;;) {
        Func_08016478(work->work);
        if(work->resource_base != 0)
            resource=work->resource_base + work->selection;
        else
            resource=work->resource_ids[work->selection] + (s32)&Value_0000001f;
        Func_0801e7c0(resource,work->work,0,0);
        for(;;) {
            Func_080030f8(1);
            if(Data_03001c94 & 1) goto accept;
            if(Data_03001c94 & 2) goto cancel;
            if(Data_03001c94 & 8) goto cancel;
            if((Data_03001b04 & 32) || (Data_03001b04 & 64)) {
                Func_080f9010(111);
                work->selection--;
                if(work->selection<0) work->selection=work->item_count-1;
                break;
            }
            if((Data_03001b04 & 16) || (Data_03001b04 & 128)) {
                Func_080f9010(111);
                work->selection++;
                if(work->selection>=work->item_count) work->selection=0;
                break;
            }
        }
    }
cancel:
    Func_080f9010(113);
    return -1;
accept:
    Func_080f9010(112);
    return work->selection;
}

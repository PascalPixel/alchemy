#include "types.h"

#define FIELD(p,type,off) (*(type *)((u8 *)(p)+(off)))

typedef void (*CopyBufferFn)(void *,s32,const void *);
typedef void (*ExpandFn)(const void *,void *,s32);

extern u8 Data_08032224[];
extern u8 Data_08031e24[];

s32 Func_080178b0(s32 packed_window, u32 *output, const void *source)
{
    u8 buffer[384];
    u8 *runtime=*(u8 **)0x03001e8c;
    ExpandFn expand;
    const u8 *input;
    u8 *destination;
    u8 *digits;
    s32 first_length;
    s32 run_length;
    s32 second_length;
    s32 transparent;
    s32 first_index;
    s32 second_index;
    s32 group;
    s32 row;
    s32 column;
    s32 nibble;
    u32 value;

    ((CopyBufferFn)0x03000164)(buffer,384,source);
    if(FIELD(runtime,u8,0xea4)!=0) {
        run_length=8;
        transparent=0;
    } else {
        run_length=FIELD(runtime,u16,0xeae);
        transparent=1;
    }

    first_index=((u8)packed_window-32)<<5;
    input=Data_08032224+first_index;
    first_length=*(u16 *)input;
    input += 2;
    expand=(ExpandFn)0x03000214;
    if(FIELD(runtime,u16,0xeac)==1) {
        expand(input,buffer+53,transparent);
        expand(input,buffer+54,transparent);
        expand(input,buffer+36,run_length);
        expand(input,buffer+37,run_length);
        first_length++;
    } else {
        expand(input,buffer+53,transparent);
        expand(input,buffer+36,run_length);
    }

    second_index=(s32)(packed_window<<8)>>16;
    if((u16)second_index!=0) {
        input=Data_08031e24+((u16)second_index<<5);
        second_length=*(s16 *)input;
        input+=2;
        destination=buffer+first_length;
        if(FIELD(runtime,u16,0xeac)==1) {
            expand(input,destination+49,transparent);
            expand(input,destination+50,transparent);
            expand(input,destination+32,run_length);
            expand(input,destination+33,run_length);
            second_length++;
        } else {
            expand=(ExpandFn)0x03000164;
            expand(input,destination+49,transparent);
            expand(input,destination+32,run_length);
        }
        first_length+=(u16)second_length;
    }

    digits=buffer+11;
    group=0;
    do {
        row=0;
        do {
            column=0;
            do {
                value=0;
                nibble=7;
                do {
                    value=(value<<4)+*digits--;
                    nibble--;
                } while(nibble>=0);
                *output++=value;
                column++;
                digits+=24;
            } while(column<=7);
            row++;
            digits-=120;
        } while(row<=1);
        group++;
        digits+=112;
    } while(group<=1);
    return first_length;
}

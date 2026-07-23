typedef unsigned char u8;
typedef signed int s32;
typedef unsigned int u32;

s32 Func_08092054();
s32 Func_0808ba1c();

extern s32 Data_02000240[];
extern u8 Data_03001ebc[];

void Func_0809c314(void)
{
    s32 temp_r0;
    s32 x;
    s32 y;
    s32 left;
    s32 right;
    s32 top;
    s32 bottom;
    u32 id;

    temp_r0 = Data_02000240[125];
    Func_08092054(temp_r0);
    temp_r0 = *(s32 *)(*(u8 **)Data_03001ebc + 480);
    x = *(s32 *)(temp_r0 + 8);
    left = x + 0xFEC00000;
    right = x + 0x01400000;
    y = *(s32 *)(temp_r0 + 16);
    top = y + 0xFDA80000;
    bottom = y + 0x01900000;

    id = 8;
    do {
        temp_r0 = Func_0808ba1c(id);

        if (temp_r0 != 0) {
            s32 objectX = *(s32 *)(temp_r0 + 8);
            s32 objectY = *(s32 *)(temp_r0 + 16);

            if (objectX < left || objectX > right ||
                objectY < top || objectY > bottom) {
                *(u8 *)(temp_r0 + 84) = 0;
            } else {
                *(u8 *)(temp_r0 + 84) = 1;
            }
        }
        id++;
    } while (id <= 65);
}

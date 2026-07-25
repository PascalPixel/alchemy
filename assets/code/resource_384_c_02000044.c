typedef unsigned char u8;
typedef signed short s16;

/* 共有作業域の符号付き半語。添字225は src/0808b1d8.c と同じ位置を読む。 */
extern s16 Data_02000240[];

extern u8 Data_0200835c[];
extern u8 Data_020083bc[];

/* 半語の値で二つの常駐表のどちらかを返す。分岐順が定数プールの並びを決める。 */
u8 *Func_02000044(void)
{
    if (Data_02000240[225] == 10) {
        return Data_020083bc;
    }
    return Data_0200835c;
}

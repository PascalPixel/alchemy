typedef unsigned char u8;
typedef signed int s32;

struct Fields_0808b64c {
    u8 filler[20];
    void *objects[1];
};

extern struct Fields_0808b64c *Data_03001ebc;

void Func_080090d0(void *);
void *Func_0808ba1c(s32);

void Func_0808b64c(s32 arg0) {
    struct Fields_0808b64c *state = Data_03001ebc;
    void *object = Func_0808ba1c(arg0);
    if (object != 0) {
        Func_080090d0(object);
        state->objects[arg0] = 0;
    }
}

typedef unsigned char u8;

u8 *Func_0808a080();
extern u8 *Data_03001ee0;

/* Complete scene/entity linker through return and its sole pool word. */
void Func_02002080(void)
{
    u8 *entity = Func_0808a080(0);
    *(u8 **)(Data_03001ee0 + 24) = entity;
    entity[98] = 1;
}

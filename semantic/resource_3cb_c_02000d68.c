typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

/*
 * resource_3cb owner at 0x02000d68, 168 bytes: open an interaction for the
 * caller's selector, choose one of two shared halfword slots from actor 0's
 * heading, optionally service a nonzero slot, display the corresponding
 * message, and close the interaction.
 *
 * Complete owner: saved-register prologue at 0x02000d68 through the sole
 * interworking return at 0x02000de6-0x02000dee, followed by all eight
 * referenced pool words through 0x02000e0f.  Nine static calls across seven
 * targets match independently.  The first range's zero case and both nonzero
 * cases converge on the call pair at 0x02000dca; only the second range's zero
 * case uses the separate pair at 0x02000dda, preserved as two lexical sites.
 */

extern u8 Data_02000240[];

u8 *Func_0808a080();
void Func_0808a018();
void Func_0808a148();
void Func_08015118();
void Func_08015120();
void Func_0808a170();
void Func_0808a180();

void Func_02000d68(s32 selector)
{
    u8 *actor;
    u16 heading;
    u16 *slot;
    s32 firstRange;
    s32 message;

    actor = Func_0808a080(0);
    heading = *(u16 *)(actor + 6);

    Func_0808a018();
    Func_0808a148(selector, *(s32 *)&Data_02000240[500], 0);

    firstRange = (u32)(heading - 0xa001) <= 0x3ffe;
    if (firstRange) {
        slot = (u16 *)&Data_02000240[0x2ac];
        message = 0x297b;
    } else {
        slot = (u16 *)&Data_02000240[0x2b2];
        message = 0x297d;
    }

    if (firstRange || *slot != 0) {
        if (*slot != 0) {
            Func_08015118();
            Func_08015120(*slot, 5);
            message++;
        } else {
            message = 0x2988;
        }

        Func_0808a170(message);
        Func_0808a180(selector, 0);
    } else {
        Func_0808a170(0x2989);
        Func_0808a180(selector, 0);
    }
}

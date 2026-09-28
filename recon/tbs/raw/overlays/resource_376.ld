/* resource_376: its listing with maintained code linked in place, as pret
   links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/resource_376_overlay.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_HEYA/VILLAGER.o"(.text)
        "*/resource_376_overlay.o"(.text.x02008298)
        "*/games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_HEYA/TIMED_EVENTS.o"(.text)
        "*/resource_376_overlay.o"(.text.x02009144)
        "*/resource_376_overlay.o"(.rodata)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
    /* The compiler calls its remainder helper by its ABI name; here it is the
       overlay's veneer into the resident IWRAM divider, as in MAIN.LD. */
    __umodsi3 = IwramUnsignedRemainderEntry;
}

/* resource_376: its entry veneers, its listing with maintained code linked
   in place, as pret links objects in ROM order. Its import veneers follow
   its code. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_HEYA/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_HEYA/VILLAGER.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_HEYA/HOPE_TALK.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_HEYA/TIMED_EVENTS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_HEYA/IMPORT.o"(.text)
        "*/resource_376_overlay.o"(.rodata)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
    /* The compiler calls its remainder helper by its ABI name; here it is the
       overlay's veneer into the resident IWRAM divider, as in MAIN.LD. */
    __umodsi3 = IwramUnsignedRemainderEntry;
}

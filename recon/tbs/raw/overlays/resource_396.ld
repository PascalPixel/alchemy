/* resource_396: its entry veneers, its listing and its import veneers
   linked in place, as pret links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/TORETO_HEYA/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TORETO_HEYA/HEYA.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/EFFECT/MOVE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/EFFECT/SPAWN.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TORETO_HEYA/HEYA2.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TORETO_HEYA/IMPORT.o"(.text)
        "*/resource_396_overlay.o"(.rodata.x02009c00)
    }
    .bss : { "*/resource_396_overlay.o"(.bss) }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
    /* The compiler calls its division helpers by their ABI names; here they
       are the overlay's veneers into the resident IWRAM dividers, as in
       MAIN.LD. */
    __divsi3 = Engine_MathDivide;
    __udivsi3 = IwramUnsignedDivideEntry;
}

/* resource_3c0: its entry veneers, its listing with maintained code linked
   in place, as pret links objects in ROM order. Its import veneers follow
   its code. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/SUHARA_SABAKU/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SUHARA_SABAKU/SELECTED_PRESENTATION.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/EFFECT/MOVE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/EFFECT/SPAWN.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SUHARA_SABAKU/SABAKU.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SUHARA_SABAKU/IMPORT.o"(.text)
        "*/resource_3c0_overlay.o"(.rodata)
    }
    .bss (NOLOAD) : { "*/resource_3c0_overlay.o"(.bss) }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
    /* The compiler calls its division helper by its ABI name; here it is the
       overlay's veneer into the resident IWRAM divider, as in MAIN.LD. */
    __divsi3 = IwramSignedDivideEntry;
}

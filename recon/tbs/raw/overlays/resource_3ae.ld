/* resource_3ae: its entry veneers, its listing with maintained code linked
   in place, as pret links objects in ROM order. Its import veneers follow
   its code. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/KAREI_TOREBI/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/KAREI_TOREBI/TOREBI.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/KAREI_TOREBI/IMPORT.o"(.text)
        "*/resource_3ae_overlay.o"(.rodata.part1)
    }
    .bss : { "*/resource_3ae_overlay.o"(.bss) }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}

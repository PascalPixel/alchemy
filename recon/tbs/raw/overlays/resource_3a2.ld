/* resource_3a2: its entry veneers, its listing and its import veneers
   linked in place, as pret links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/YAMA_RAMA/ENTRY.o"(.text)
        "*/resource_3a2_overlay.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/YAMA_RAMA/IMPORT.o"(.text)
        "*/resource_3a2_overlay.o"(.rodata)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}

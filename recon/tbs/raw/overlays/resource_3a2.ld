/* resource_3a2: its entry veneers, its listing with maintained code linked
   in place, as pret links objects in ROM order. Its import veneers follow
   its code. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/YAMA_RAMA/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/YAMA_RAMA/RAMA.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/YAMA_RAMA/IMPORT.o"(.text)
        "*/resource_3a2_overlay.o"(.rodata.x02009620)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}

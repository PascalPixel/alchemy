/* resource_377: its entry veneers, its listing and its import veneers
   linked in place, as pret links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/HAIDIA_BABI/ENTRY.o"(.text)
        "*/resource_377_overlay.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/HAIDIA_BABI/IMPORT.o"(.text)
        "*/resource_377_overlay.o"(.rodata)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}

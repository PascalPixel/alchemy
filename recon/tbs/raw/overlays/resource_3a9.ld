/* resource_3a9: its entry veneers, its listing with maintained code linked
   in place, as pret links objects in ROM order. Its import veneers follow
   its code. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/KAREI_HEYA/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KAREI_HEYA/HEYA.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KAREI_HEYA/IMPORT.o"(.text)
        "*/resource_3a9_overlay.o"(.rodata.x020084a0)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}

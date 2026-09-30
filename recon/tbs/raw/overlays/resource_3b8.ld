/* resource_3b8: its entry veneers, its listing with maintained code linked
   in place, as pret links objects in ROM order. Its import veneers follow
   its code. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/TOREBI_KYUDEN/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TOREBI_KYUDEN/KYUDEN.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TOREBI_KYUDEN/IMPORT.o"(.text)
        "*/resource_3b8_overlay.o"(.rodata.x0200c550)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}

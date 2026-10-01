/* resource_3c3: its entry veneers, its listing with maintained code linked
   in place, as pret links objects in ROM order. Its import veneers follow
   its code. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/SUHARA_GATE/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SUHARA_GATE/GATE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SUHARA_GATE/IMPORT.o"(.text)
        "*/resource_3c3_overlay.o"(.rodata.x02008974)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}

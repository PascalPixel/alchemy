/* resource_37a: its entry veneers, its listing with maintained code linked
   in place, as pret links objects in ROM order. Its import veneers follow
   its code. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/SORU_NICHIGETSU/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SORU_NICHIGETSU/NICHIGETSU.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SORU_NICHIGETSU/IMPORT.o"(.text)
        "*/resource_37a_overlay.o"(.rodata.x0200aafc)
    }
    .bss : { "*/resource_37a_overlay.o"(.bss) }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}

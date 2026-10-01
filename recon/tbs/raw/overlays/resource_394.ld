/* resource_394: its entry veneers, its listing with maintained code linked
   in place, as pret links objects in ROM order. Its import veneers follow
   its code. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/KORIMA_MAGARI/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KORIMA_MAGARI/MAGARI.o"(.text)
        "*/resource_394_overlay.o"(.text.x02008096)
        "*/resource_394_overlay.o"(.text.x02008098)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KORIMA_MAGARI/MAGARI2.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/KORIMA/PALETTE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KORIMA_MAGARI/IMPORT.o"(.text)
        "*/resource_394_overlay.o"(.rodata.x0200911c)
    }
    .bss : { "*/resource_394_overlay.o"(.bss) }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}

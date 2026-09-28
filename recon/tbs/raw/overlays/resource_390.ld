/* resource_390: its listing with maintained code linked in place, as pret
   links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/resource_390_overlay.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KORIMA_HEYA/ENTRY_LAYOUT.o"(.text)
        "*/resource_390_overlay.o"(.text.x020082b8)
        "*/resource_390_overlay.o"(.rodata)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}

/* resource_381: its entry veneers, its listing with maintained code linked
   in place, as pret links objects in ROM order. Its import veneers follow
   its code. The overlay's own work follows the image. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/SORU_FUNKA/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SORU_FUNKA/FUNKA.o"(.text)
        "*/resource_381_overlay.o"(.text.x0200b01c)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SORU_STAR/RING_SETUP.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SORU_FUNKA/IMPORT.o"(.text)
        "*/resource_381_overlay.o"(.rodata)
    }
    .bss (NOLOAD) :
    {
        "*/resource_381_overlay.o"(.bss)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}

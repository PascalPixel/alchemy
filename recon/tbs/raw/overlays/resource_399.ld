/* resource_399: its entry veneers, its listing with maintained code linked
   in place, as pret links objects in ROM order. Its import veneers follow
   its code. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/IMIRU_MURA/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/IMIRU_MURA/MURA.o"(.text)
        "*/resource_399_overlay.o"(.text.x020095b4)
        "*/games/THE BROKEN SEAL/SRC/FIELD/IMIRU_MURA/CHOREOGRAPHY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/IMIRU_MURA/IMPORT.o"(.text)
        "*/resource_399_overlay.o"(.rodata)
    }
    .bss : { "*/resource_399_overlay.o"(.bss) }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}

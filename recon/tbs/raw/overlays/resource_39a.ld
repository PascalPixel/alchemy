/* resource_39a: its entry veneers, its listing with maintained code linked
   in place, as pret links objects in ROM order. Its import veneers follow
   its code. The overlay's own work follows the image. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/IMIRU_FUCHIN/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/IMIRU_FUCHIN/FUCHIN.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/IMIRU_FUCHIN/TRACKING.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/IMIRU_FUCHIN/FUCHIN2.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/IMIRU_FUCHIN/IMPORT.o"(.text)
        "*/resource_39a_overlay.o"(.rodata)
    }
    .bss (NOLOAD) :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/IMIRU_FUCHIN/TRACKING.o"(.bss COMMON)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}

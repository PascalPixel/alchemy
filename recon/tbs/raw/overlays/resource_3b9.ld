/* resource_3b9: its entry veneers, its listing and its import veneers
   linked in place, as pret links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/KORASHIAMU_IRIGUCHI/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KORASHIAMU_IRIGUCHI/IRIGUCHI.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KORASHIAMU_IRIGUCHI/IRIGUCHI2.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KORASHIAMU_IRIGUCHI/IRIGUCHI3.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KORASHIAMU_IRIGUCHI/IMPORT.o"(.text)
        "*/resource_3b9_overlay.o"(.rodata)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}

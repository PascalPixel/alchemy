/* resource_388: its listing with maintained code
   linked in place, as pret links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/resource_388_overlay.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/GOMA_IKE/GATE_PLACEMENT.o"(.text)
        "*/resource_388_overlay.o"(.text.x020080d8)
        "*/resource_388_overlay.o"(.rodata)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}

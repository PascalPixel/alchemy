/* resource_379: its listing with maintained code
   linked in place, as pret links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/resource_379_overlay.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_FUNKA/CONDITIONAL_SETUP.o"(.text)
        "*/resource_379_overlay.o"(.text.x02008ab8)
        "*/resource_379_overlay.o"(.rodata)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}

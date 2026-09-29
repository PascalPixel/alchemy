/* resource_3ad: its entry veneers, its listing with maintained code linked
   in place, as pret links objects in ROM order. Its import veneers follow
   its code, and the cave's scene tables follow them. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/RUNPA_DOU/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/RUNPA_DOU/SCENE.o"(.text)
        "*/resource_3ad_overlay.o"(.text.x02008044)
        "*/games/THE BROKEN SEAL/SRC/FIELD/RUNPA_DOU/GATE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/RUNPA_DOU/REUNION.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/RUNPA_DOU/WAGON.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/RUNPA_DOU/WAIT.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/RUNPA_DOU/THINGS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/RUNPA_DOU/PARTING.o"(.text)
        "*/resource_3ad_overlay.o"(.text.x02009a0c)
        "*/games/THE BROKEN SEAL/SRC/FIELD/RUNPA_DOU/IMPORT.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/RUNPA_DOU/DATA.o"(.rodata)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}

/* resource_374: its entry veneers, its listing with maintained code linked
   in place, as pret links objects in ROM order. Its import veneers follow
   its code. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_IE/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_IE/HAIDIA.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_IE/SHOW_OFF.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_IE/STONE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_IE/HELP_ADVICE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_IE/RESTORE.o"(.text)
        "*/resource_374_overlay.o"(.text.x02009274)
        "*/games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_IE/SCENES.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_IE/IMPORT.o"(.text)
        "*/resource_374_overlay.o"(.rodata)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}

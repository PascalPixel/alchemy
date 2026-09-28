/* resource_374: its listing with maintained code linked in place, as pret
   links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/resource_374_overlay.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_IE/HAIDIA.o"(.text)
        "*/resource_374_overlay.o"(.text.x02008248)
        "*/games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_IE/STONE.o"(.text)
        "*/resource_374_overlay.o"(.text.x020085e8)
        "*/games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_IE/RESTORE.o"(.text)
        "*/resource_374_overlay.o"(.text.x02009274)
        "*/games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_IE/SCENES.o"(.text)
        "*/resource_374_overlay.o"(.text.x0200a5e4)
        "*/resource_374_overlay.o"(.rodata)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}

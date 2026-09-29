/* resource_37a: its entry veneers, its listing with maintained code linked
   in place, as pret links objects in ROM order. Its import veneers follow
   its code. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/SORU_NICHIGETSU/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SORU_NICHIGETSU/SANCTUM.o"(.text)
        "*/resource_37a_overlay.o"(.text.x02008488)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SORU_NICHIGETSU/LUNA_SOL.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SORU_NICHIGETSU/RISE_AND_SHRINK.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SORU_NICHIGETSU/ENTRY_STATE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SORU_NICHIGETSU/STATUE_LIGHTS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SORU_NICHIGETSU/LIGHT_SCENE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SORU_NICHIGETSU/SUKURETA.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SORU_NICHIGETSU/IMPORT.o"(.text)
        "*/resource_37a_overlay.o"(.rodata)
    }
    .bss : { "*/resource_37a_overlay.o"(.bss) }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}

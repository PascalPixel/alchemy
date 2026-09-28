/* resource_37a: its entry veneers, its listing with maintained code linked
   in place, as pret links objects in ROM order. Its import veneers follow
   its code. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/SORU_NICHIGETSU/ENTRY.o"(.text)
        "*/resource_37a_overlay.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SORU_NICHIGETSU/RISE_AND_SHRINK.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SORU_NICHIGETSU/ENTRY_STATE.o"(.text)
        "*/resource_37a_overlay.o"(.text.x0200a5b0)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SORU_NICHIGETSU/LIGHT_SCENE.o"(.text)
        "*/resource_37a_overlay.o"(.text.x0200a924)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SORU_NICHIGETSU/IMPORT.o"(.text)
        "*/resource_37a_overlay.o"(.rodata)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}

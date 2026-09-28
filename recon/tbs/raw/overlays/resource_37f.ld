/* resource_37f: its entry veneers, its listing with maintained code linked
   in place, as pret links objects in ROM order. Its import veneers follow
   its code. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/SORU_IRIGUCHI/ENTRY.o"(.text)
        "*/resource_37f_overlay.o"(.text.x02008030)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SORU_IRIGUCHI/SCENE_EXITS.o"(.text)
        "*/resource_37f_overlay.o"(.text.x0200807c)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SORU_IRIGUCHI/SCENARIO_DISPATCH.o"(.text)
        "*/resource_37f_overlay.o"(.text.x02008420)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SORU_IRIGUCHI/ACTOR_FLAGS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SORU_IRIGUCHI/PUSH_BLOCK.o"(.text)
        "*/resource_37f_overlay.o"(.text.x020088f4)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SORU_IRIGUCHI/ENTRY_STATE.o"(.text)
        "*/resource_37f_overlay.o"(.text.x02008a24)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SORU_IRIGUCHI/SANCTUM.o"(.text)
        "*/resource_37f_overlay.o"(.text.x02008f8c)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SORU_IRIGUCHI/FORMATION.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SORU_IRIGUCHI/IMPORT.o"(.text)
        "*/resource_37f_overlay.o"(.rodata.part1)
    }
    /* The overlay's own work, past its loaded image. */
    .bss (NOLOAD) :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/SORU_IRIGUCHI/ENTRY_STATE.o"(.bss)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}

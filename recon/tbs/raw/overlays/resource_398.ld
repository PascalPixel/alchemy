/* resource_398: its entry veneers, its listing with maintained code linked
   in place, as pret links objects in ROM order. Its import veneers follow
   its code. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/BIRIBINO_DOU/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/BIRIBINO_DOU/HIDE_ACTOR.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/BIRIBINO_DOU/GET_ENTRANCES.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/BIRIBINO_DOU/EVENT_TABLE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/BIRIBINO_DOU/SCENE_TABLES.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/BIRIBINO_DOU/REGION.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/BIRIBINO_DOU/SCENE_INIT.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/BIRIBINO_DOU/SETUP.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/BIRIBINO_DOU/IMPORT.o"(.text)
        "*/resource_398_overlay.o"(.rodata)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}

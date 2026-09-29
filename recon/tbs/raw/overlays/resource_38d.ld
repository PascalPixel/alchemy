/* resource_38d: its entry veneers, its listing with maintained code linked
   in place, as pret links objects in ROM order. Its import veneers follow
   its code. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/BIRIBINO_KYUDEN/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/BIRIBINO_KYUDEN/FACING.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/BIRIBINO_KYUDEN/GET_TABLES.o"(.text)
        "*/resource_38d_overlay.o"(.text.x02008130)
        "*/games/THE BROKEN SEAL/SRC/FIELD/BIRIBINO_KYUDEN/DIALOGUE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/BIRIBINO_KYUDEN/DOOR_EXIT.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/BIRIBINO_KYUDEN/CHEST.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/BIRIBINO_KYUDEN/ENTRY_STATE.o"(.text)
        "*/resource_38d_overlay.o"(.text.x020088c0)
        "*/games/THE BROKEN SEAL/SRC/FIELD/BIRIBINO_KYUDEN/AUDIENCE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/BIRIBINO_KYUDEN/ACTOR_ROW_SCENE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/BIRIBINO_KYUDEN/IMPORT.o"(.text)
        "*/resource_38d_overlay.o"(.rodata)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}

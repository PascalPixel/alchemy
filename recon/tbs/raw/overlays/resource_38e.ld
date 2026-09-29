/* resource_38e: its entry veneers, its listing with maintained code linked
   in place, as pret links objects in ROM order. Its import veneers follow
   its code. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/BIRIBINO_NIWA/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/BIRIBINO_NIWA/TABLES.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/BIRIBINO_NIWA/GET_PLACEMENTS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/BIRIBINO_NIWA/STEPS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/BIRIBINO_NIWA/GET_EVENTS.o"(.text)
        "*/resource_38e_overlay.o"(.text.x02008184)
        "*/games/THE BROKEN SEAL/SRC/FIELD/BIRIBINO_NIWA/DIALOGUE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/BIRIBINO_NIWA/GARDEN_EVENT.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/BIRIBINO_NIWA/GATE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/BIRIBINO_NIWA/SCENE_INIT.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/BIRIBINO_NIWA/ENTRY_STATE.o"(.text)
        "*/resource_38e_overlay.o"(.text.x020085dc)
        "*/games/THE BROKEN SEAL/SRC/FIELD/BIRIBINO_NIWA/ORBITING.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/BIRIBINO_NIWA/IMPORT.o"(.text)
        "*/resource_38e_overlay.o"(.rodata)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}

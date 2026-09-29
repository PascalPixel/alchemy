/* resource_38e: its entry veneers, its listing with maintained code linked
   in place, as pret links objects in ROM order. Its import veneers follow
   its code. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/BIRIBINO_NIWA/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/BIRIBINO_NIWA/TABLES.o"(.text)
        "*/resource_38e_overlay.o"(.text.x0200809c)
        "*/games/THE BROKEN SEAL/SRC/FIELD/BIRIBINO_NIWA/STEPS.o"(.text)
        "*/resource_38e_overlay.o"(.text.x02008154)
        "*/games/THE BROKEN SEAL/SRC/FIELD/BIRIBINO_NIWA/DIALOGUE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/BIRIBINO_NIWA/GARDEN_EVENT.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/BIRIBINO_NIWA/GATE.o"(.text)
        "*/resource_38e_overlay.o"(.text.x020084bc)
        "*/games/THE BROKEN SEAL/SRC/FIELD/BIRIBINO_NIWA/ENTRY_STATE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/BIRIBINO_NIWA/ARREST.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/BIRIBINO_NIWA/ORBITING.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/BIRIBINO_NIWA/IMPORT.o"(.text)
        "*/resource_38e_overlay.o"(.rodata)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}

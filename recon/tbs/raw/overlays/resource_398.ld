/* resource_398: its entry veneers, its listing with maintained code linked
   in place, as pret links objects in ROM order. Its import veneers follow
   its code. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/BIRIBINO_DOU/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/BIRIBINO_DOU/HIDE_ACTOR.o"(.text)
        "*/resource_398_overlay.o"(.text.x02008040)
        "*/games/THE BROKEN SEAL/SRC/FIELD/BIRIBINO_DOU/EVENT_TABLE.o"(.text)
        "*/resource_398_overlay.o"(.text.x020080a0)
        "*/games/THE BROKEN SEAL/SRC/FIELD/BIRIBINO_DOU/REGION.o"(.text)
        "*/resource_398_overlay.o"(.text.x0200846c)
        "*/games/THE BROKEN SEAL/SRC/FIELD/BIRIBINO_DOU/SETUP.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/BIRIBINO_DOU/IMPORT.o"(.text)
        "*/resource_398_overlay.o"(.rodata)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}

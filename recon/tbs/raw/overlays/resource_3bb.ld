/* resource_3bb: its listing with maintained code
   linked in place, as pret links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/resource_3bb_overlay.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KOROSSEO_KABE/SPIN_ACTOR.o"(.text)
        "*/resource_3bb_overlay.o"(.text.x02008258)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KOROSSEO_KABE/PUSH_WALL.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KOROSSEO_KABE/ROLL_LOG.o"(.text)
        "*/resource_3bb_overlay.o"(.text.x0200862c)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KOROSSEO_KABE/STAGE_INTRO.o"(.text)
        "*/resource_3bb_overlay.o"(.text.x02009424)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KOROSSEO_KABE/LIFTED_ACTOR_COORDINATOR.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KOROSSEO_KABE/PUSH_BLOCK.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KOROSSEO_KABE/GUIDE_TALK.o"(.text)
        "*/resource_3bb_overlay.o"(.text.x02009b30)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/KOROSSEO/PATH_RIVAL.o"(.text)
        "*/resource_3bb_overlay.o"(.text.x0200b638)
        "*/resource_3bb_overlay.o"(.rodata)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}

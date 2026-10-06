SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_6B4/ENTRY.o"(.text)
        "*/games/THE LOST AGE/SRC/FIELD/COMMON/OBJECT/SET_SPRITE_PRIORITY.o"(.text)
        "*/resource_6b4_overlay.o"(.text.x0200807e)
        "*/games/THE LOST AGE/SRC/FIELD/COMMON/EFFECT/MOVE.o"(.text)
        "*/resource_6b4_overlay.o"(.text.x020080b6)
        "*/resource_6b4_overlay.o"(.text.x020080b8)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_6B4/SCENE_TABLES.o"(.text)
        "*/resource_6b4_overlay.o"(.text.x020082a4)
        "*/resource_6b4_overlay.o"(.text.x0200832c)
        "*/resource_6b4_overlay.o"(.text.x02008394)
        "*/games/THE LOST AGE/SRC/FIELD/COMMON/EFFECT/ADVANCE_MOTION.o"(.text)
        "*/resource_6b4_overlay.o"(.text.x02008476)
        "*/resource_6b4_overlay.o"(.text.x02008478)
        "*/resource_6b4_overlay.o"(.text.x02008564)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_6B4/PREPARE.o"(.text)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_6B4/IMPORT.o"(.text)
        "*/resource_6b4_overlay.o"(.rodata.x0200959c)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}

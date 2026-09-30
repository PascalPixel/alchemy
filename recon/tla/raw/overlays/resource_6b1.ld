/* resource_6b1: its entry veneers, its listing and its import veneers
   linked in place, as pret links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_6B1/ENTRY.o"(.text)
        "*/games/THE LOST AGE/SRC/FIELD/COMMON/OBJECT/SET_SPRITE_PRIORITY.o"(.text)
        "*/resource_6b1_overlay.o"(.text.x0200807e)
        "*/resource_6b1_overlay.o"(.text.x02008080)
        "*/resource_6b1_overlay.o"(.text.x020080d0)
        "*/resource_6b1_overlay.o"(.text.x02008124)
        "*/resource_6b1_overlay.o"(.text.x0200815c)
        "*/resource_6b1_overlay.o"(.text.x0200833c)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_6B1/SCENE_TABLES.o"(.text)
        "*/resource_6b1_overlay.o"(.text.x0200835c)
        "*/resource_6b1_overlay.o"(.text.x02008388)
        "*/resource_6b1_overlay.o"(.text.x02008408)
        "*/resource_6b1_overlay.o"(.text.x02008494)
        "*/resource_6b1_overlay.o"(.text.x020084b4)
        "*/resource_6b1_overlay.o"(.text.x020084e4)
        "*/resource_6b1_overlay.o"(.text.x02008534)
        "*/resource_6b1_overlay.o"(.text.x02008588)
        "*/resource_6b1_overlay.o"(.text.x02008714)
        "*/resource_6b1_overlay.o"(.text.x02008740)
        "*/resource_6b1_overlay.o"(.text.x02008854)
        "*/resource_6b1_overlay.o"(.text.x02008858)
        "*/resource_6b1_overlay.o"(.text.x02008a98)
        "*/resource_6b1_overlay.o"(.text.x02008b18)
        "*/resource_6b1_overlay.o"(.text.x02008ba4)
        "*/resource_6b1_overlay.o"(.text.x02008bd4)
        "*/resource_6b1_overlay.o"(.text.x02008be0)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_6B1/IMPORT.o"(.text)
        "*/resource_6b1_overlay.o"(.rodata.x02008d6c)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}

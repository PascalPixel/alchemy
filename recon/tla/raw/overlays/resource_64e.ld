/* resource_64e: its entry veneers, its listing and its import veneers
   linked in place, as pret links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE LOST AGE/SRC/FIELD/VINASU_CHOJO/ENTRY.o"(.text)
        "*/games/THE LOST AGE/SRC/FIELD/COMMON/OBJECT/SET_SPRITE_PRIORITY.o"(.text)
        "*/games/THE LOST AGE/SRC/FIELD/VINASU_CHOJO/OBJECTS.o"(.text)
        "*/games/THE LOST AGE/SRC/FIELD/COMMON/EFFECT/MOVE.o"(.text)
        "*/resource_64e_overlay.o"(.text.x0200814e)
        "*/resource_64e_overlay.o"(.text.x02008150)
        "*/games/THE LOST AGE/SRC/FIELD/VINASU_CHOJO/DISTANCE.o"(.text)
        "*/resource_64e_overlay.o"(.text.x0200834c)
        "*/resource_64e_overlay.o"(.text.x0200838c)
        "*/resource_64e_overlay.o"(.text.x02008510)
        "*/games/THE LOST AGE/SRC/FIELD/VINASU_CHOJO/FLICKER.o"(.text)
        "*/resource_64e_overlay.o"(.text.x020085a8)
        "*/games/THE LOST AGE/SRC/FIELD/VINASU_CHOJO/SCENE_TABLES.o"(.text)
        "*/resource_64e_overlay.o"(.text.x020086ac)
        "*/resource_64e_overlay.o"(.text.x0200878c)
        "*/games/THE LOST AGE/SRC/FIELD/VINASU_CHOJO/PUSH.o"(.text)
        "*/games/THE LOST AGE/SRC/FIELD/VINASU_CHOJO/STATUE.o"(.text)
        "*/games/THE LOST AGE/SRC/FIELD/VINASU_CHOJO/SCENE_EVENTS.o"(.text)
        "*/resource_64e_overlay.o"(.text.x02008a54)
        "*/games/THE LOST AGE/SRC/FIELD/VINASU_CHOJO/PREPARE.o"(.text)
        "*/resource_64e_overlay.o"(.text.x02008b2c)
        "*/resource_64e_overlay.o"(.text.x02008d30)
        "*/games/THE LOST AGE/SRC/FIELD/VINASU_CHOJO/IMPORT.o"(.text)
        "*/resource_64e_overlay.o"(.rodata.x02009ce4)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}

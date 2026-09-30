/* resource_64e: its entry veneers, its listing and its import veneers
   linked in place, as pret links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE LOST AGE/SRC/FIELD/VINASU_CHOJO/ENTRY.o"(.text)
        "*/games/THE LOST AGE/SRC/FIELD/COMMON/OBJECT/SET_SPRITE_PRIORITY.o"(.text)
        "*/resource_64e_overlay.o"(.text.x0200807e)
        "*/resource_64e_overlay.o"(.text.x02008080)
        "*/resource_64e_overlay.o"(.text.x020080a4)
        "*/resource_64e_overlay.o"(.text.x020080dc)
        "*/resource_64e_overlay.o"(.text.x02008118)
        "*/resource_64e_overlay.o"(.text.x02008150)
        "*/resource_64e_overlay.o"(.text.x02008314)
        "*/resource_64e_overlay.o"(.text.x0200834c)
        "*/resource_64e_overlay.o"(.text.x0200838c)
        "*/resource_64e_overlay.o"(.text.x02008510)
        "*/resource_64e_overlay.o"(.text.x02008570)
        "*/resource_64e_overlay.o"(.text.x020085a8)
        "*/games/THE LOST AGE/SRC/FIELD/VINASU_CHOJO/SCENE_TABLES.o"(.text)
        "*/resource_64e_overlay.o"(.text.x02008664)
        "*/resource_64e_overlay.o"(.text.x02008678)
        "*/resource_64e_overlay.o"(.text.x02008684)
        "*/resource_64e_overlay.o"(.text.x020086ac)
        "*/resource_64e_overlay.o"(.text.x0200878c)
        "*/games/THE LOST AGE/SRC/FIELD/VINASU_CHOJO/PUSH.o"(.text)
        "*/games/THE LOST AGE/SRC/FIELD/VINASU_CHOJO/STATUE.o"(.text)
        "*/games/THE LOST AGE/SRC/FIELD/VINASU_CHOJO/SCENE_EVENTS.o"(.text)
        "*/resource_64e_overlay.o"(.text.x02008a14)
        "*/resource_64e_overlay.o"(.text.x02008a18)
        "*/resource_64e_overlay.o"(.text.x02008a54)
        "*/games/THE LOST AGE/SRC/FIELD/VINASU_CHOJO/PREPARE.o"(.text)
        "*/resource_64e_overlay.o"(.text.x02008b2c)
        "*/resource_64e_overlay.o"(.text.x02008d30)
        "*/games/THE LOST AGE/SRC/FIELD/VINASU_CHOJO/IMPORT.o"(.text)
        "*/resource_64e_overlay.o"(.rodata.x02009ce4)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}

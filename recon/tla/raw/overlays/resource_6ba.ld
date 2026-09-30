/* resource_6ba: its entry veneers, its listing and its import veneers
   linked in place, as pret links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_6BA/ENTRY.o"(.text)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_6BA/SCENE_TABLES.o"(.text)
        "*/resource_6ba_overlay.o"(.text.x02008054)
        "*/resource_6ba_overlay.o"(.text.x0200812c)
        "*/resource_6ba_overlay.o"(.text.x02008144)
        "*/resource_6ba_overlay.o"(.text.x0200815c)
        "*/resource_6ba_overlay.o"(.text.x02008178)
        "*/resource_6ba_overlay.o"(.text.x02008190)
        "*/resource_6ba_overlay.o"(.text.x0200819c)
        "*/resource_6ba_overlay.o"(.text.x020081a8)
        "*/resource_6ba_overlay.o"(.text.x020081b4)
        "*/resource_6ba_overlay.o"(.text.x020081c0)
        "*/resource_6ba_overlay.o"(.text.x020081cc)
        "*/resource_6ba_overlay.o"(.text.x020081d4)
        "*/resource_6ba_overlay.o"(.text.x020082a8)
        "*/resource_6ba_overlay.o"(.text.x02008394)
        "*/resource_6ba_overlay.o"(.text.x020083a4)
        "*/resource_6ba_overlay.o"(.text.x020083b8)
        "*/resource_6ba_overlay.o"(.text.x02008520)
        "*/resource_6ba_overlay.o"(.text.x020085fc)
        "*/resource_6ba_overlay.o"(.text.x02008638)
        "*/resource_6ba_overlay.o"(.text.x02008668)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_6BA/SCENE_EVENTS.o"(.text)
        "*/resource_6ba_overlay.o"(.text.x02008688)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_6BA/PREPARE.o"(.text)
        "*/resource_6ba_overlay.o"(.text.x02008704)
        "*/resource_6ba_overlay.o"(.text.x02008748)
        "*/resource_6ba_overlay.o"(.text.x02008848)
        "*/resource_6ba_overlay.o"(.text.x02008aac)
        "*/resource_6ba_overlay.o"(.text.x02008ab4)
        "*/resource_6ba_overlay.o"(.text.x02008abc)
        "*/resource_6ba_overlay.o"(.text.x02008ae0)
        "*/resource_6ba_overlay.o"(.text.x02008b50)
        "*/resource_6ba_overlay.o"(.text.x02008ba0)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_6BA/IMPORT.o"(.text)
        "*/resource_6ba_overlay.o"(.rodata.x02008e80)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}

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
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_6BA/MENU_CALLS.o"(.text)
        "*/resource_6ba_overlay.o"(.text.x020081d4)
        "*/resource_6ba_overlay.o"(.text.x020082a8)
        "*/resource_6ba_overlay.o"(.text.x02008394)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_6BA/LOOP_CALL.o"(.text)
        "*/resource_6ba_overlay.o"(.text.x020083b8)
        "*/resource_6ba_overlay.o"(.text.x02008520)
        "*/resource_6ba_overlay.o"(.text.x020085fc)
        "*/resource_6ba_overlay.o"(.text.x02008638)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_6BA/GIVE.o"(.text)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_6BA/SCENE_EVENTS.o"(.text)
        "*/resource_6ba_overlay.o"(.text.x02008688)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_6BA/PREPARE.o"(.text)
        "*/resource_6ba_overlay.o"(.text.x02008704)
        "*/resource_6ba_overlay.o"(.text.x02008748)
        "*/resource_6ba_overlay.o"(.text.x02008848)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_6BA/FORWARD.o"(.text)
        "*/resource_6ba_overlay.o"(.text.x02008ae0)
        "*/resource_6ba_overlay.o"(.text.x02008b50)
        "*/resource_6ba_overlay.o"(.text.x02008ba0)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_6BA/IMPORT.o"(.text)
        "*/resource_6ba_overlay.o"(.rodata.x02008e80)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}

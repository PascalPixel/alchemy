SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/field/overlay_659/entry.o"(.text)
        "*/games/tla/src/field/overlay_659/scene_tables.o"(.text)
        "*/games/tla/src/field/overlay_659/s004c.o"(.text)
        "*/resource_659_overlay.o"(.text.x020080fc)
        "*/games/tla/src/field/overlay_659/s01b8.o"(.text)
        "*/resource_659_overlay.o"(.text.x02008570)
        "*/resource_659_overlay.o"(.text.x02008624)
        "*/games/tla/src/field/overlay_659/prepare.o"(.text)
        "*/resource_659_overlay.o"(.text.x02009008)
        "*/games/tla/src/field/overlay_659/events.o"(.text)
        "*/games/tla/src/field/overlay_659/import.o"(.text)
        "*/resource_659_overlay.o"(.rodata.x02009f18)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}

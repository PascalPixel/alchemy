SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/field/overlay_659/entry.o"(.text)
        "*/games/tla/src/field/overlay_659/scene_tables.o"(.text)
        "*/resource_659_overlay.o"(.text.x0200804c)
        "*/resource_659_overlay.o"(.text.x020080a4)
        "*/resource_659_overlay.o"(.text.x020080fc)
        "*/resource_659_overlay.o"(.text.x020081b8)
        "*/resource_659_overlay.o"(.text.x0200823c)
        "*/resource_659_overlay.o"(.text.x0200828c)
        "*/resource_659_overlay.o"(.text.x02008308)
        "*/resource_659_overlay.o"(.text.x02008368)
        "*/resource_659_overlay.o"(.text.x020083c8)
        "*/resource_659_overlay.o"(.text.x02008428)
        "*/resource_659_overlay.o"(.text.x020084c4)
        "*/resource_659_overlay.o"(.text.x02008534)
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

SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tla/src/field/overlay_684/entry.o"(.text)
        "*/games/tla/src/field/overlay_684/scene_tables.o"(.text)
        "*/games/tla/src/field/overlay_684/script0.o"(.text)
        "*/games/tla/src/field/overlay_684/scene_events.o"(.text)
        "*/resource_684_overlay.o"(.text.x02008064)
        "*/games/tla/src/field/overlay_684/prepare.o"(.text)
        "*/games/tla/src/field/overlay_684/import.o"(.text)
        "*/resource_684_overlay.o"(.rodata.x020080c0)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}

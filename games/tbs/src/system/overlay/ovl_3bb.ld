SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/field/korosseo_kabe/entry.o"(.text)
        "*/games/tbs/src/field/korosseo_kabe/kabe.o"(.text)
        "*/games/tbs/src/field/common/korosseo/solo_competitor.o"(.text)
        "*/games/tbs/src/field/korosseo_kabe/nearest.o"(.text)
        "*/games/tbs/src/field/common/korosseo/solo_round.o"(.text)
        "*/games/tbs/src/field/korosseo_kabe/interact.o"(.text)
        "*/games/tbs/src/field/common/korosseo/korosseo.o"(.text)
        "*/games/tbs/src/field/korosseo_kabe/transition.o"(.text)
        "*/games/tbs/src/field/common/korosseo/compete.o"(.text)
        "*/games/tbs/src/field/korosseo_kabe/motion_reset.o"(.text)
        "*/games/tbs/src/field/common/korosseo/marker.o"(.text)
        "*/games/tbs/src/field/korosseo_kabe/marker.o"(.text)
        "*/games/tbs/src/field/common/korosseo/item_icon.o"(.text)
        "*/games/tbs/src/field/korosseo_kabe/effect.o"(.text)
        "*/games/tbs/src/field/common/korosseo/path_rival.o"(.text)
        "*/games/tbs/src/field/korosseo_kabe/control.o"(.text)
        "*/games/tbs/src/field/common/korosseo/scene_state.o"(.text)
        "*/games/tbs/src/field/korosseo_kabe/push_ahead.o"(.text)
        "*/games/tbs/src/field/korosseo_kabe/import.o"(.text)
        "*/games/tbs/src/field/korosseo_kabe/data.o"(.data)
        "*/games/tbs/src/graphics/korosseo/gauge.o"(.rodata)
        "*/games/tbs/src/field/korosseo_kabe/data2.o"(.data)
    }
    .bss : { "*/resource_3bb_overlay.o"(.bss) }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}

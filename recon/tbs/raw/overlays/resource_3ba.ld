/* resource_3ba: its entry veneers, its listing with maintained code and
   the compiler library linked in place, as pret links objects and libgcc
   members in ROM order. Its import veneers follow its code. */
EXTERN(_call_via_r0)
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/KOROSSEO_KAWA/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KOROSSEO_KAWA/KAWA.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/KOROSSEO/SOLO_COMPETITOR.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KOROSSEO_KAWA/SET_STATE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/KOROSSEO/SOLO_ROUND.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KOROSSEO_KAWA/KAWA2.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/KOROSSEO/KOROSSEO.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KOROSSEO_KAWA/KAWA3.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/KOROSSEO/KOROSSEO2.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KOROSSEO_KAWA/RESET_MOTION.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/KOROSSEO/MARKER.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KOROSSEO_KAWA/KAWA4.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/KOROSSEO/ITEM_ICON.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KOROSSEO_KAWA/KAWA5.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/KOROSSEO/PATH_RIVAL.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KOROSSEO_KAWA/KAWA6.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/KOROSSEO/SCENE_STATE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KOROSSEO_KAWA/FIND_SLOT.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KOROSSEO_KAWA/IMPORT.o"(.text)
        *libgcc.a:_call_via_rX.o(.text)
        "*/resource_3ba_overlay.o"(.rodata.x0200be44)
    }
    .bss : { "*/resource_3ba_overlay.o"(.bss) }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}

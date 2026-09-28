/* resource_396: its entry veneers, its listing and its import veneers
   linked in place, as pret links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/TORETO_HEYA/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TORETO_HEYA/OBJECT_SETUP.o"(.text)
        "*/resource_396_overlay.o"(.text.x020080a0)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/EFFECT/MOVE.o"(.text)
        "*/resource_396_overlay.o"(.text.x0200813c)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TORETO_HEYA/TABLES.o"(.text)
        "*/resource_396_overlay.o"(.text.x02008334)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TORETO_HEYA/STEPS.o"(.text)
        "*/resource_396_overlay.o"(.text.x0200869c)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TORETO_HEYA/ENCOUNTER.o"(.text)
        "*/resource_396_overlay.o"(.text.x02009004)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TORETO_HEYA/PALETTE.o"(.text)
        "*/resource_396_overlay.o"(.text.x02009244)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TORETO_HEYA/GESTURE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TORETO_HEYA/SWIRL_SHRINK.o"(.text)
        "*/resource_396_overlay.o"(.text.x0200962c)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TORETO_HEYA/MAP_PATCH.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TORETO_HEYA/PALETTE_FADE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TORETO_HEYA/LANDING_DUST.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TORETO_HEYA/IMPORT.o"(.text)
        "*/resource_396_overlay.o"(.rodata)
    }
    .bss : { "*/resource_396_overlay.o"(.bss) }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}

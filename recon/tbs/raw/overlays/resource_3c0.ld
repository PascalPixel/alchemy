/* resource_3c0: its entry veneers, its listing with maintained code linked
   in place, as pret links objects in ROM order. Its import veneers follow
   its code. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/SUHARA_SABAKU/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SUHARA_SABAKU/SELECTED_PRESENTATION.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/EFFECT/MOVE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/EFFECT/SPAWN.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SUHARA_SABAKU/REQUEST.o"(.text)
        "*/resource_3c0_overlay.o"(.text.x0200834c)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SUHARA_SABAKU/EXITS.o"(.text)
        "*/resource_3c0_overlay.o"(.text.x020083ac)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SUHARA_SABAKU/MIDDLE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SUHARA_SABAKU/RESTORE_ACTOR_SCALE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SUHARA_SABAKU/LATE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SUHARA_SABAKU/FOLLOW_SPARK.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SUHARA_SABAKU/ACTOR12.o"(.text)
        "*/resource_3c0_overlay.o"(.text.x02008b24)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SUHARA_SABAKU/PALETTE.o"(.text)
        "*/resource_3c0_overlay.o"(.text.x02008d24)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SUHARA_SABAKU/VISIT_BLEND.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SUHARA_SABAKU/ALTAR_BLEND.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/SUHARA_SABAKU/IMPORT.o"(.text)
        "*/resource_3c0_overlay.o"(.rodata)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
    /* The compiler calls its division helper by its ABI name; here it is the
       overlay's veneer into the resident IWRAM divider, as in MAIN.LD. */
    __divsi3 = IwramSignedDivideEntry;
}

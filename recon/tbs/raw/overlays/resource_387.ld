/* resource_387: its entry veneers, its listing with maintained code linked
   in place, as pret links objects in ROM order. Its import veneers follow
   its code. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/GOMA_IRIGUCHI/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/GOMA_IRIGUCHI/EFFOBJ.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/EFFECT/MOVE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/EFFECT/SPAWN.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/GOMA_IRIGUCHI/SCENES.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/GOMA_IRIGUCHI/ENTRY_STATE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/GOMA_IRIGUCHI/DEPTH.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/GOMA_IRIGUCHI/SET_ENTRANCE_FLAG.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/GOMA_IRIGUCHI/OBJWAIT.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/GOMA_IRIGUCHI/SPINNING_LEAP.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/GOMA_IRIGUCHI/SLOTS.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/GOMA_IRIGUCHI/GIVE_SHAMANS_ROD.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/GOMA_IRIGUCHI/IMPORT.o"(.text)
        "*/resource_387_overlay.o"(.rodata.part1)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
    /* The compiler calls its division helper by its ABI name; here it is the
       overlay's veneer into the resident IWRAM divider, as in MAIN.LD. */
    __divsi3 = IwramSignedDivideEntry;
}

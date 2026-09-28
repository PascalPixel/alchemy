/* resource_3ab: its listing with maintained code linked in place, as pret
   links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/resource_3ab_overlay.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/EFFECT/MOVE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/EFFECT/SPAWN.o"(.text)
        "*/resource_3ab_overlay.o"(.text.x02008314)
        "*/resource_3ab_overlay.o"(.rodata)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
    /* The compiler calls its division helper by its ABI name; here it is the
       overlay's veneer into the resident IWRAM divider, as in MAIN.LD. */
    __divsi3 = IwramSignedDivideEntry;
}

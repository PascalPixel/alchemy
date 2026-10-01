/* resource_3b3: its entry veneers, its listing with maintained code and
   the compiler library linked in place, as pret links objects and libgcc
   members in ROM order. Its import veneers follow its code. */
EXTERN(_call_via_r0)
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/TAKARA_HASHIRA/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TAKARA_HASHIRA/HASHIRA.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/EFFECT/MOVE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/EFFECT/SPAWN.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/OBJECT/STAGED_ACTOR.o"(.text)
        "*/resource_3b3_overlay.o"(.text.x02008cc0)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TAKARA_HASHIRA/HASHIRA2.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TAKARA_HASHIRA/HASHIRA4.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TAKARA_HASHIRA/HASHIRA5.o"(.text)
        "*/resource_3b3_overlay.o"(.text.x02009d84)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TAKARA_HASHIRA/HASHIRA3.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/TAKARA_HASHIRA/IMPORT.o"(.text)
        *libgcc.a:_call_via_rX.o(.text)
        "*/resource_3b3_overlay.o"(.rodata.x0200abf8)
    }
    .bss : { "*/resource_3b3_overlay.o"(.bss) }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
    /* The compiler calls its division helper by its ABI name; here it is the
       overlay's veneer into the resident IWRAM divider, as in MAIN.LD. */
    __divsi3 = Engine_MathDivide;
}

/* resource_3bd: its entry veneers, its listing with maintained code and
   the compiler library linked in place, as pret links objects and libgcc
   members in ROM order. Its import veneers follow its code. */
EXTERN(_call_via_r0)
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/ARUTAMIRA_DOU/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/OBJECT/STAGED_ACTOR.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/ARUTAMIRA_DOU/DOU.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/ARUTAMIRA_DOU/ROCK.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/ARUTAMIRA_DOU/DOU2.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/ARUTAMIRA_DOU/IMPORT.o"(.text)
        *libgcc.a:_call_via_rX.o(.text)
        "*/resource_3bd_overlay.o"(.rodata.x0200beb4)
    }
    .bss : { "*/games/THE BROKEN SEAL/SRC/FIELD/ARUTAMIRA_DOU/DOU.o"(.bss COMMON) }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}

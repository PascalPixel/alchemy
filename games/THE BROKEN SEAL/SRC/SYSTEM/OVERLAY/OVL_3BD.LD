EXTERN(_call_via_r0)
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/ARUTAMIRA_DOU/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/COMMON/OBJECT/STAGED_ACTOR.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/ARUTAMIRA_DOU/SCENE.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/ARUTAMIRA_DOU/IMPORT.o"(.text)
        *libgcc.a:_call_via_rX.o(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/ARUTAMIRA_DOU/DATA.o"(.data)
    }
    .bss : { "*/games/THE BROKEN SEAL/SRC/FIELD/ARUTAMIRA_DOU/SCENE.o"(.bss COMMON) }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}

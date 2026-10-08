EXTERN(_call_via_r0)
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/field/torebi_machi/entry.o"(.text)
        "*/games/tbs/src/field/torebi_machi/machi.o"(.text)
        "*/games/tbs/src/field/torebi_machi/import.o"(.text)
        *libgcc.a:_call_via_rX.o(.text)
        "*/games/tbs/src/field/torebi_machi/data.o"(.data)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}

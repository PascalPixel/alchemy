EXTERN(_call_via_r0)
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/field/runpa_jo/entry.o"(.text)
        "*/games/tbs/src/field/common/object/staged_actor.o"(.text)
        "*/games/tbs/src/field/runpa_jo/runpa_jo.o"(.text)
        "*/games/tbs/src/field/runpa_jo/import.o"(.text)
        *libgcc.a:_call_via_rX.o(.text)
        *libgcc.a:_addsub_df.o(.text)
        *libgcc.a:_si_to_df.o(.text)
        *libgcc.a:_df_to_si.o(.text)
        *libgcc.a:_lshrdi3.o(.text)
        *libgcc.a:_pack_df.o(.text)
        *libgcc.a:_unpack_df.o(.text)
        "*/games/tbs/src/field/runpa_jo/data.o"(.data)
        *libgcc.a:_thenan_df.o(.rodata)
        "*/games/tbs/src/field/runpa_jo/data2.o"(.data)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}

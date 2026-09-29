/// Address-ordered instruction text from GNU objdump's tab-separated output.
/// Addresses are 32-bit integers; headers and malformed rows are ignored.
pub fn objdump_rows(output: &str) -> std::collections::BTreeMap<u32, String> {
    output
        .lines()
        .filter_map(|line| {
            let (address, rest) = line.trim_start().split_once(":\t")?;
            let (_, instruction) = rest.split_once('\t')?;
            Some((
                u32::from_str_radix(address, 16).ok()?,
                instruction.trim_end().into(),
            ))
        })
        .collect()
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn objdump_addresses_are_integer_ordered_and_text_is_preserved() {
        let rows = objdump_rows("header\n  8000004:\t4770      \tbx\tlr  \n  8000000:\tf000 f800 \tbl\t8000004 <entry>\n bad-address:\t0000\tbad\n 100000000:\t0000\tbad\n");
        assert_eq!(
            rows.into_iter().collect::<Vec<_>>(),
            vec![
                (0x08000000, "bl\t8000004 <entry>".into()),
                (0x08000004, "bx\tlr".into()),
            ]
        );
        assert!(objdump_rows("\nfile format elf32-littlearm\n").is_empty());
    }
}

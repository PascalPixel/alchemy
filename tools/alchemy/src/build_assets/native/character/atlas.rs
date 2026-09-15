use super::*;

pub(super) fn extract(root: &Path, inputs: &Value, rom: &[u8]) -> Result<(), String> {
    let ctx = Context::new(root);
    let mut sheets: BTreeMap<String, (usize, usize, Vec<u8>, Vec<bool>)> = BTreeMap::new();
    for input in inputs.as_array().ok_or("private input list missing")? {
        if !matches!(
            input["kind"].as_str(),
            Some(
                "frame-atlas"
                    | "sprite"
                    | "sprite-atlas"
                    | "archive-atlas"
                    | "still-atlas"
                    | "tile-atlas"
                    | "portrait-atlas"
            )
        ) {
            continue;
        }
        let decoded = if input["kind"] == "frame-atlas" {
            super::super::frame::pixels(&ctx, input, rom)?
        } else if input["kind"] == "portrait-atlas" {
            super::super::portrait::pixels(input, rom)?
        } else if input["kind"] == "tile-atlas" {
            super::super::tile::pixels(input, rom)?
        } else if input["kind"] == "still-atlas" {
            super::super::still::pixels(input, rom)?
        } else if input["kind"] == "archive-atlas" {
            super::super::graphics::pixels(&ctx, input, rom)?
        } else {
            pixels(&ctx, input, rom)?
        };
        let source = json_string(&input["source"], "sprite source")?;
        let w = address(&input["width"])?;
        let h = address(&input["height"])?;
        let (x, y, width, height) = if let Some(rect) = input.get("source_rect") {
            (
                address(&rect["x"])?,
                address(&rect["y"])?,
                address(&input["atlas_width"])?,
                address(&input["atlas_height"])?,
            )
        } else {
            (0, 0, w, h)
        };
        if x.checked_add(w).is_none_or(|v| v > width) || y.checked_add(h).is_none_or(|v| v > height)
        {
            return Err("sprite atlas extent differs".into());
        }
        let length = width.checked_mul(height).ok_or("sprite atlas overflows")?;
        let sheet = sheets
            .entry(source.into())
            .or_insert_with(|| (width, height, vec![0; length], vec![false; length]));
        if sheet.0 != width || sheet.1 != height {
            return Err("sprite atlas dimensions disagree".into());
        }
        for row in 0..h {
            for col in 0..w {
                let dst = (y + row) * width + x + col;
                let pixel = decoded[row * w + col];
                if sheet.3[dst] && sheet.2[dst] != pixel {
                    return Err("shared sprite sections disagree".into());
                }
                sheet.2[dst] = pixel;
                sheet.3[dst] = true;
            }
        }
    }
    for (source, (width, height, pixels, _)) in sheets {
        if source == "games/tbs/SRC/GRAPHICS/COMMON/TILE_BANK.PNG" {
            write(
                root,
                &source,
                &super::super::tile::image(&pixels, width as u32, height as u32)?,
            )?;
            continue;
        }
        write(root, &source, &image(&pixels, width as u32, height as u32)?)?;
    }
    Ok(())
}

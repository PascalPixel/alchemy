use super::*;

const REVIEW: &str = "games/THE BROKEN SEAL/SRC/GRAPHICS/REVIEW.JSON";

/// Export derived review sheets from private native inputs, without reading scratch or ROM files.
pub(crate) fn export(root: &Path, output: &Path) -> Result<(), String> {
    let output = if output.is_absolute() {
        output.to_path_buf()
    } else {
        root.join(output)
    };
    if !output.starts_with(root.join("out")) {
        return Err("review images must remain in the ignored repository out/ directory".into());
    }
    if output
        .components()
        .any(|part| matches!(part, std::path::Component::ParentDir))
    {
        return Err("review output cannot contain parent traversal".into());
    }
    let mut plan: Value =
        serde_json::from_slice(&fs::read(root.join(REVIEW)).map_err(|e| e.to_string())?)
            .map_err(|e| e.to_string())?;
    if plan["format"] != 3 {
        return Err("unsupported graphics review format".into());
    }
    super::review_defaults::expand(root, &mut plan)?;
    let colors: Value =
        serde_json::from_slice(&fs::read(root.join(COLORS)).map_err(|e| e.to_string())?)
            .map_err(|e| e.to_string())?;
    let mut palettes = Vec::new();
    let mut owners = BTreeMap::<String, Value>::new();
    for palette in plan["palettes"]
        .as_array()
        .ok_or("missing review palettes")?
    {
        let mut rgb = Vec::new();
        for run in palette.as_array().ok_or("invalid review palette")? {
            for index in 0..address(&run["count"])? {
                let mut reference = run["reference"].clone();
                let position = match reference[0].as_str() {
                    Some("bank" | "table" | "image") => Some(2),
                    Some("owner") => Some(4),
                    Some("gray") => Some(1),
                    Some("gray-scale") => Some(2),
                    _ => None,
                };
                if let Some(position) = position {
                    reference[position] = json!(address(&reference[position])?
                        .checked_add(
                            index
                                .checked_mul(address(&run["step"])?)
                                .ok_or("palette range overflows")?
                        )
                        .ok_or("palette range overflows")?);
                }
                if reference[0] == "diagnostic-rgb" {
                    for channel in 1..4 {
                        rgb.push(
                            u8::try_from(address(&reference[channel])?)
                                .map_err(|_| "invalid diagnostic color")?,
                        );
                    }
                    continue;
                }
                if reference[0] == "image" {
                    let path = root_path(root, json_string(&reference[1], "palette image")?)?;
                    let image = psynergy::assets::image::indexed_png(
                        &fs::read(path).map_err(|e| e.to_string())?,
                    )
                    .map_err(|e| e.to_string())?;
                    rgb.extend_from_slice(
                        image
                            .palette
                            .get(address(&reference[2])?)
                            .ok_or("palette image index exceeds source")?,
                    );
                    continue;
                }
                if reference[0] == "gray" || reference[0] == "gray-scale" {
                    let value = if reference[0] == "gray-scale" {
                        address(&reference[2])? * 255 / address(&reference[1])?
                    } else {
                        address(&reference[1])?
                    };
                    let gray = u8::try_from(value).map_err(|_| "invalid gray")?;
                    rgb.extend_from_slice(&[gray; 3]);
                    continue;
                }
                let word = if reference[0] == "owner" {
                    let name = json_string(&reference[1], "palette owner")?;
                    if !owners.contains_key(name) {
                        let owner: Value = serde_json::from_slice(
                            &fs::read(root_path(root, name)?).map_err(|e| e.to_string())?,
                        )
                        .map_err(|e| e.to_string())?;
                        owners.insert(name.to_string(), owner);
                    }
                    let owner = &owners[name];
                    address(
                        &owner[json_string(&reference[2], "palette field")?]
                            [json_string(&reference[3], "palette key")?][address(&reference[4])?],
                    )?
                } else if reference[0] == "bank" {
                    address(&colors["banks"][address(&reference[1])?][address(&reference[2])?])?
                } else if reference[0] == "table" {
                    address(
                        &colors["tables"][json_string(&reference[1], "palette table")?]
                            [address(&reference[2])?],
                    )?
                } else {
                    return Err("unknown review palette reference".into());
                };
                for shift in [0, 5, 10] {
                    let channel = (word >> shift) & 31;
                    rgb.push(
                        if reference
                            .as_array()
                            .and_then(|v| v.last())
                            .is_some_and(|v| v == "scale")
                        {
                            (channel * 255 / 31) as u8
                        } else {
                            ((channel << 3) | (channel >> 2)) as u8
                        },
                    );
                }
            }
        }
        palettes.push(rgb);
    }
    let images = plan["images"].as_array().ok_or("missing review images")?;
    let mut sources = BTreeMap::new();
    let mut generated = Vec::new();
    for image in images {
        let raster = render(root, image, &mut sources)?;
        let width = raster.width;
        let height = raster.height;
        let pixels = raster.pixels;

        let palette = palettes
            .get(address(&image["palette"])?)
            .ok_or("review palette exceeds registry")?;
        if pixels
            .iter()
            .any(|&index| index as usize >= palette.len() / 3)
        {
            return Err("review pixel exceeds palette".into());
        }
        let name = json_string(&image["file"], "review filename")?;
        if Path::new(name).components().count() != 1 || !name.ends_with(".PNG") {
            return Err("review filename must be a flat PNG name".into());
        }
        let mut bytes = Vec::new();
        {
            let mut encoder = png::Encoder::new(
                &mut bytes,
                width.try_into().map_err(|_| "width exceeds PNG")?,
                height.try_into().map_err(|_| "height exceeds PNG")?,
            );
            encoder.set_color(png::ColorType::Indexed);
            encoder.set_depth(png::BitDepth::Eight);
            encoder.set_palette(palette.clone());
            if image["transparent"] == true {
                encoder.set_trns(vec![0]);
            }
            encoder
                .write_header()
                .map_err(|e| e.to_string())?
                .write_image_data(&pixels)
                .map_err(|e| e.to_string())?;
        }
        generated.push((name.to_string(), bytes));
    }
    // Validate the complete batch before writing any output.
    fs::create_dir_all(&output).map_err(|e| e.to_string())?;
    for (name, bytes) in generated {
        fs::write(output.join(name), bytes).map_err(|e| e.to_string())?;
    }
    fs::write(
        output.join("INDEX.JSON"),
        format!("{}\n", canonical_json(&plan)),
    )
    .map_err(|e| e.to_string())?;
    println!("review images={} source={REVIEW}", images.len());
    Ok(())
}

#[derive(Clone)]
struct Raster {
    width: usize,
    height: usize,
    pixels: Vec<u8>,
}
impl Raster {
    fn blank(width: usize, height: usize) -> Result<Self, String> {
        if width == 0 || height == 0 {
            return Err("empty review raster".into());
        }
        Ok(Self {
            width,
            height,
            pixels: vec![0; width.checked_mul(height).ok_or("review raster overflows")?],
        })
    }
    fn cut(&self, x: usize, y: usize, w: usize, h: usize) -> Result<Self, String> {
        if x.checked_add(w).ok_or("crop overflows")? > self.width
            || y.checked_add(h).ok_or("crop overflows")? > self.height
        {
            return Err("review crop exceeds source".into());
        }
        let mut out = Self::blank(w, h)?;
        for row in 0..h {
            out.pixels[row * w..(row + 1) * w].copy_from_slice(
                &self.pixels[(y + row) * self.width + x..(y + row) * self.width + x + w],
            );
        }
        Ok(out)
    }
    fn paste(&mut self, source: &Self, piece: &Value) -> Result<(), String> {
        let x = address(&piece["at"][0])?;
        let y = address(&piece["at"][1])?;
        let w = piece
            .get("display_size")
            .map(|v| address(&v[0]))
            .transpose()?
            .unwrap_or(source.width);
        let h = piece
            .get("display_size")
            .map(|v| address(&v[1]))
            .transpose()?
            .unwrap_or(source.height);
        if w == 0
            || h == 0
            || x.checked_add(w).ok_or("piece overflows")? > self.width
            || y.checked_add(h).ok_or("piece overflows")? > self.height
        {
            return Err("review piece exceeds output".into());
        }
        let offset = piece
            .get("palette_offset")
            .map(address)
            .transpose()?
            .unwrap_or(0);
        for row in 0..h {
            for col in 0..w {
                let mut sx = col * source.width / w;
                let mut sy = row * source.height / h;
                if piece["flip_x"] == true {
                    sx = source.width - 1 - sx;
                }
                if piece["flip_y"] == true {
                    sy = source.height - 1 - sy;
                }
                let value = source.pixels[sy * source.width + sx];
                self.pixels[(y + row) * self.width + x + col] = if value == 0 {
                    0
                } else {
                    u8::try_from(value as usize + offset)
                        .map_err(|_| "review palette offset exceeds 8bpp")?
                };
            }
        }
        Ok(())
    }
    fn bytes(&self, bpp: usize) -> Result<Vec<u8>, String> {
        if self.width % 8 != 0 || self.height % 8 != 0 || ![4, 8].contains(&bpp) {
            return Err("review input needs complete 4bpp or 8bpp tiles".into());
        }
        let mut bytes = Vec::new();
        for ty in (0..self.height).step_by(8) {
            for tx in (0..self.width).step_by(8) {
                for row in 0..8 {
                    for col in (0..8).step_by(if bpp == 4 { 2 } else { 1 }) {
                        let at = (ty + row) * self.width + tx + col;
                        let v = self.pixels[at];
                        bytes.push(if bpp == 4 {
                            let hi = self.pixels[at + 1];
                            if v > 15 || hi > 15 {
                                return Err("review input exceeds 4bpp".into());
                            }
                            v | hi << 4
                        } else {
                            v
                        });
                    }
                }
            }
        }
        Ok(bytes)
    }
    fn tiles(bytes: &[u8], width: usize, height: usize, bpp: usize) -> Result<Self, String> {
        let mut out = Self::blank(width, height)?;
        if width % 8 != 0 || height % 8 != 0 || ![4, 8].contains(&bpp) {
            return Err("review tile shape differs".into());
        }
        let n = if bpp == 4 { 32 } else { 64 };
        if bytes.len() != width * height / (if bpp == 4 { 2 } else { 1 }) {
            return Err("review tile byte extent differs".into());
        }
        for (i, tile) in bytes.chunks_exact(n).enumerate() {
            for p in 0..64 {
                let value = if bpp == 4 {
                    (tile[p / 2] >> ((p % 2) * 4)) & 15
                } else {
                    tile[p]
                };
                let x = (i % (width / 8)) * 8 + p % 8;
                let y = (i / (width / 8)) * 8 + p / 8;
                out.pixels[y * width + x] = value;
            }
        }
        Ok(out)
    }
}
fn crop(
    root: &Path,
    input: &Value,
    sources: &mut BTreeMap<String, Raster>,
) -> Result<Raster, String> {
    let name = json_string(&input["source"], "review source")?;
    if !sources.contains_key(name) {
        let path = root_path(root, name)?;
        let im = psynergy::assets::image::indexed_png(&fs::read(&path).map_err(|e| {
            format!(
                "{}: {e}; restore private inputs with --extract-missing-sources ROM",
                path.display()
            )
        })?)
        .map_err(|e| e.to_string())?;
        let pixels = im
            .pixels
            .iter()
            .map(|&v| u8::try_from(v).map_err(|_| "source exceeds 8bpp".to_string()))
            .collect::<Result<Vec<_>, _>>()?;
        sources.insert(
            name.to_string(),
            Raster {
                width: im.width as usize,
                height: im.height as usize,
                pixels,
            },
        );
    }
    let rect = input["rectangle"]
        .as_array()
        .ok_or("missing review rectangle")?;
    if rect.len() != 4 {
        return Err("review rectangle needs four coordinates".into());
    }
    sources[name].cut(
        address(&rect[0])?,
        address(&rect[1])?,
        address(&rect[2])?,
        address(&rect[3])?,
    )
}
fn render(
    root: &Path,
    image: &Value,
    sources: &mut BTreeMap<String, Raster>,
) -> Result<Raster, String> {
    let mut out = Raster::blank(address(&image["size"][0])?, address(&image["size"][1])?)?;
    let recipe = &image["recipe"];
    match recipe["kind"]
        .as_str()
        .ok_or("missing review layout kind")?
    {
        "pieces" => {
            for piece in recipe["pieces"].as_array().ok_or("missing pieces")? {
                out.paste(&crop(root, piece, sources)?, piece)?;
            }
        }
        "grid" => {
            let source = crop(root, &recipe["input"], sources)?;
            let fw = address(&recipe["frame_size"][0])?;
            let fh = address(&recipe["frame_size"][1])?;
            let cols = address(&recipe["source_columns"])?;
            if cols == 0 || fw == 0 || fh == 0 {
                return Err("empty review grid".into());
            }
            let cells = if let Some(cells) = recipe["cells"].as_array() {
                cells.clone()
            } else {
                let count = address(&recipe["frames"])?;
                let destcols = address(&recipe["columns"])?;
                if destcols == 0 {
                    return Err("empty destination grid".into());
                }
                (0..count)
                    .map(|i| {
                        if recipe["order"] == "front-back-wrapped" {
                            json!([
                                i,
                                ((i / 2) % destcols) * fw,
                                ((i / 2) / destcols * 2 + i % 2) * fh
                            ])
                        } else if recipe["order"] == "front-back" {
                            json!([i, (i / 2) * fw, (i % 2) * fh])
                        } else {
                            json!([i, (i % destcols) * fw, (i / destcols) * fh])
                        }
                    })
                    .collect()
            };
            for cell in cells {
                let i = address(&cell[0])?;
                let tile = source.cut(i % cols * fw, i / cols * fh, fw, fh)?;
                out.paste(
                    &tile,
                    &json!({"at":[address(&cell[1])?,address(&cell[2])?]}),
                )?;
            }
        }
        "linear-bytes" => {
            let bytes =
                crop(root, &recipe["input"], sources)?.bytes(address(&recipe["native_bpp"])?)?;
            let len = address(&recipe["decoded_length"])?;
            out.pixels
                .get_mut(..len)
                .ok_or("linear review exceeds output")?
                .copy_from_slice(bytes.get(..len).ok_or("linear review exceeds source")?);
        }
        "tile-frames" => {
            let source = crop(root, &recipe["input"], sources)?;
            let fw = address(&recipe["frame_size"][0])?;
            let fh = address(&recipe["frame_size"][1])?;
            let cols = address(&recipe["columns"])?;
            if cols == 0 || fw == 0 || fh == 0 {
                return Err("empty tile frame grid".into());
            }
            for i in 0..address(&recipe["frames"])? {
                let frame = source.cut(i % cols * fw, i / cols * fh, fw, fh)?;
                let tile = Raster::tiles(&frame.pixels, fw, fh, 8)?;
                out.paste(&tile, &json!({"at":[i%cols*fw,i/cols*fh]}))?;
            }
        }
        "byte-pieces" => {
            let mut bytes = Vec::new();
            let bpp = address(&recipe["native_bpp"])?;
            for input in recipe["inputs"].as_array().ok_or("missing byte inputs")? {
                let source = crop(root, input, sources)?;
                let fw = recipe
                    .get("source_frame_size")
                    .map(|v| address(&v[0]))
                    .transpose()?
                    .unwrap_or(source.width);
                let fh = recipe
                    .get("source_frame_size")
                    .map(|v| address(&v[1]))
                    .transpose()?
                    .unwrap_or(source.height);
                if fw == 0 || fh == 0 {
                    return Err("empty source frame".into());
                }
                for y in (0..source.height).step_by(fh) {
                    for x in (0..source.width).step_by(fw) {
                        bytes.extend(source.cut(x, y, fw, fh)?.bytes(bpp)?);
                    }
                }
            }
            let bpp = address(&recipe["output_bpp"])?;
            if ![4, 8].contains(&bpp) {
                return Err("invalid review depth".into());
            }
            for piece in recipe["pieces"].as_array().ok_or("missing byte pieces")? {
                let w = address(&piece["size"][0])?;
                let h = address(&piece["size"][1])?;
                let offset = address(&piece["offset"])?;
                let n = w.checked_mul(h).ok_or("byte piece overflows")?
                    / (if bpp == 4 { 2 } else { 1 });
                let end = offset.checked_add(n).ok_or("byte piece overflows")?;
                let tile = Raster::tiles(
                    bytes.get(offset..end).ok_or("byte piece exceeds source")?,
                    w,
                    h,
                    bpp,
                )?;
                out.paste(&tile, piece)?;
            }
        }
        "camelot" => {
            let base = crop(root, &recipe["base"], sources)?.bytes(8)?;
            let animation = crop(root, &recipe["animation"], sources)?.bytes(8)?;
            let owner: Value = serde_json::from_slice(
                &fs::read(root_path(
                    root,
                    json_string(&recipe["tilemap_source"], "tilemap source")?,
                )?)
                .map_err(|e| e.to_string())?,
            )
            .map_err(|e| e.to_string())?;
            let words = owner
                .pointer(json_string(&recipe["tilemap_pointer"], "tilemap pointer")?)
                .and_then(Value::as_array)
                .ok_or("missing opening tilemap")?;
            for frame in 0..address(&recipe["frames"])? {
                let mut bytes = base.clone();
                let start = address(&recipe["replace_tile"])? * 64;
                let n = address(&recipe["replace_bytes"])?;
                let src = frame * address(&recipe["stride"])?;
                bytes
                    .get_mut(start..start + n)
                    .ok_or("opening replacement exceeds base")?
                    .copy_from_slice(
                        animation
                            .get(src..src + n)
                            .ok_or("opening replacement exceeds animation")?,
                    );
                for y in 0..20 {
                    for x in 0..30 {
                        let word =
                            address(words.get(y * 32 + x).ok_or("opening tilemap truncated")?)?;
                        let at = (word & 1023) * 64;
                        let tile = Raster::tiles(
                            bytes.get(at..at + 64).ok_or("opening tile exceeds bank")?,
                            8,
                            8,
                            8,
                        )?;
                        out.paste(&tile,&json!({"at":[frame%2*240+x*8,frame/2*160+y*8],"flip_x":word&1024!=0,"flip_y":word&2048!=0}))?;
                    }
                }
            }
        }
        _ => return Err("unknown review layout kind".into()),
    }
    Ok(out)
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn tile_roundtrip_preserves_rows_across_tile_boundaries() {
        for bpp in [4, 8] {
            let mut source = Raster::blank(24, 16).unwrap();
            for (i, pixel) in source.pixels.iter_mut().enumerate() {
                *pixel = ((i * 7 + i / 24) % (1 << bpp)) as u8;
            }
            let decoded = Raster::tiles(&source.bytes(bpp).unwrap(), 24, 16, bpp).unwrap();
            assert_eq!(decoded.pixels, source.pixels);
        }
    }

    #[test]
    fn placement_checks_bounds_and_preserves_transparent_palette_zero() {
        let source = Raster {
            width: 2,
            height: 2,
            pixels: vec![0, 1, 2, 3],
        };
        let mut out = Raster::blank(4, 4).unwrap();
        out.paste(
            &source,
            &json!({"at":[1,1],"flip_x":true,"palette_offset":16}),
        )
        .unwrap();
        assert_eq!(&out.pixels[5..7], &[17, 0]);
        assert_eq!(&out.pixels[9..11], &[19, 18]);
        assert!(out.paste(&source, &json!({"at":[3,3]})).is_err());
        assert!(Raster::tiles(&[0; 31], 8, 8, 4).is_err());
        assert!(source.cut(1, 1, 2, 2).is_err());
    }
}

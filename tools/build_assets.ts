#!/usr/bin/env bun
import { mkdirSync, readFileSync, writeFileSync } from "node:fs";
import { basename, dirname, isAbsolute, join, relative, resolve } from "node:path";
import { encode_general, encode_general_prefill, encode_palette } from "./extract_resource.ts";
import { gba_graphics, gba_palette_rgba, indexed_png, rgba_png } from "./import_asset.ts";
import { build_archive as build_offset_archive } from "./archive_asset.ts";
import { import_tilemap } from "./tilemap.ts";
import { import_words } from "./wordstream.ts";
import { import_pairs } from "./pairtable.ts";
import { build_archive as build_f0_archive } from "./f0_archive.ts";
import { prune_files, unused_tracked_images } from "./generated_files.ts";
import { build_archive as build_skip_sprite_archive } from "./skip_sprite_archive.ts";
import { build_bank as build_object_bank } from "./tile_objects.ts";
import { encode_kind2 } from "./kind2_resource.ts";
import { build_grid } from "./kind1_map_grid.ts";
import { assemble_overlay } from "./overlay_disasm.ts";
import {
  build_blend_animation,
  build_descriptors,
  build_header,
  build_metatiles,
  build_queues,
  build_sparse,
} from "./map_container_components.ts";
import { build_table as build_map_load_table } from "./map_load_table.ts";
import { build_sound_table } from "./music.ts";
import { build_sequence as build_sound_sequence } from "./music_sequence.ts";
import { buildWaveRecord } from "./audio_wave.ts";
import { build_still } from "./indexed_still.ts";
import { build_static_sprite_series } from "./static_sprite_series.ts";

const ROOT = dirname(dirname(Bun.fileURLToPath(import.meta.url)));
const ROM_BASE = 0x08000000;

type Json = Record<string, any>;

interface Options {
  rom: string;
  manifest: string;
  output: string;
}

function number(value: string | number): number {
  return typeof value === "string" ? Number(value) : Math.trunc(value);
}

function sourcePath(name: string): string {
  const source = resolve(ROOT, name);
  const position = relative(ROOT, source);
  if (position === ".." || position.startsWith("../") || position.startsWith("..\\") || isAbsolute(position)) {
    throw new Error("asset source must stay inside the repository");
  }
  return source;
}

function buildComponent(entry: Json): [Buffer, Json] {
  const kind = String(entry.kind);
  const source = sourcePath(String(entry.source));
  let data: Buffer;
  let details: Json;
  if (kind === "gba-4bpp-object-bank") {
    const [built, nestedSources, report] = build_object_bank(source);
    data = built;
    details = report;
    details._sources = nestedSources.map((path) => relative(ROOT, resolve(path)));
  } else if (kind === "gba-4bpp-tiles" || kind === "gba-8bpp-tiles") {
    const bpp = kind === "gba-4bpp-tiles" ? 4 : 8;
    const [built, , report] = gba_graphics(readFileSync(source), bpp);
    data = built;
    details = report;
  } else if (kind === "gba-palette") {
    const [, built, report] = gba_graphics(readFileSync(source), 8);
    data = built;
    details = report;
  } else if (kind === "gba-palette-rgba") {
    [data, details] = gba_palette_rgba(readFileSync(source));
  } else if (kind === "gba-tilemap16") {
    data = import_tilemap(readFileSync(source, "utf8"));
    details = { entries: data.length / 2 };
  } else if (kind === "indexed-bytes") {
    const [width, height, pixels] = indexed_png(readFileSync(source));
    data = Buffer.from(pixels);
    details = { width, height };
  } else if (kind === "raw-lz-bytes") {
    const [width, height, pixels] = indexed_png(readFileSync(source));
    data = Buffer.from(pixels).subarray(0, number(entry.size));
    details = { width, height, bytes: data.length };
  } else if (kind === "rgba-bytes") {
    const [width, height, built] = rgba_png(readFileSync(source));
    data = built;
    details = { width, height, pixels: data.length / 4 };
  } else if (kind === "little-u16-text") {
    data = import_words(readFileSync(source, "utf8"));
    details = { words: data.length / 2 };
  } else if (kind === "little-u16-pairs") {
    data = import_pairs(readFileSync(source, "utf8"));
    details = { pairs: data.length / 4 };
  } else if (kind === "zero-skip-sprite-archive") {
    const planPath = sourcePath(String(entry.plan));
    const palettePath = sourcePath(String(entry.palette));
    const plan = JSON.parse(readFileSync(planPath, "utf8"));
    data = build_skip_sprite_archive(plan, source, palettePath);
    details = {
      images: number(plan.images), width: number(plan.width), height: number(plan.height),
    };
  } else if (kind === "golden-sun-thumb-overlay") {
    const base = number(entry.base);
    data = assemble_overlay(source, base);
    details = { base: `0x${base.toString(16).padStart(8, "0")}`, bytes: data.length };
  } else {
    throw new Error(`unsupported asset component: ${kind}`);
  }
  const size = number(entry.size);
  if (data.length !== size) {
    throw new Error(`${entry.source}: built 0x${data.length.toString(16)}, expected 0x${size.toString(16)}`);
  }
  return [data, details];
}

function expandSeries(manifest: Json, entries: Json[]): void {
  const gridAddresses: Record<string, number> = {};
  for (const series of manifest.series ?? []) {
    if (series.kind !== "golden-sun-map-grid-series") continue;
    for (const grid of series.grids) gridAddresses[String(grid.id).toLowerCase()] = number(grid.address);
  }
  for (const series of manifest.series ?? []) {
    if (series.kind === "golden-sun-delta7-still-series") {
      if (number(series.width) !== 256 || number(series.height) !== 120 ||
          number(series.palette_entries) !== 128) {
        throw new Error("unsupported delta7-still layout");
      }
      for (const resource of series.resources) {
        const name = String(resource.id).toLowerCase();
        entries.push({
          address: resource.address,
          size: resource.size,
          kind: "golden-sun-delta7-still",
          source: `assets/graphics/resource_${name}/ichimaie.8bpp.png`,
        });
      }
    } else if (series.kind === "golden-sun-zero-skip-sprite-series") {
      for (const resource of series.resources) {
        const name = String(resource.id).toLowerCase();
        const directory = `assets/graphics/resource_${name}`;
        entries.push({
          address: resource.address,
          size: resource.size,
          kind: "golden-sun-general-lz",
          plan: `${directory}/stream.lz.json`,
          components: [{
            kind: "zero-skip-sprite-archive",
            size: resource.decoded_size,
            source: `${directory}/images`,
            plan: `${directory}/archive.json`,
            palette: series.palette,
          }],
        });
      }
    } else if (series.kind === "golden-sun-map-charblock-series") {
      const charblockLayout = series.charblock_layout;
      const animationLayout = series.animation_layout;
      for (const family of series.families) {
        const name = String(family.id).toLowerCase();
        const directory = `assets/graphics/resource_${name}`;
        const palette = family.palette;
        entries.push({
          address: palette.address,
          size: palette.size,
          kind: "golden-sun-general-lz",
          plan: `${directory}/palette.lz.json`,
          components: [{
            kind: "gba-palette", size: "0x1c0", source: `${directory}/palette.224.png`,
          }],
        });
        family.charblocks.forEach((resource: Json, index: number) => {
          const bank = index + 1;
          const objectSource = resource.source;
          entries.push({
            address: resource.address,
            size: resource.size,
            kind: "golden-sun-kind2-lz",
            plan: `${directory}/charblock${bank}.kind2.json`,
            layout: charblockLayout,
            components: [{
              kind: objectSource ? "gba-4bpp-object-bank" : "gba-4bpp-tiles",
              size: "0x4000",
              source: objectSource ?? `${directory}/charblock${bank}.4bpp.png`,
            }],
          });
        });
        const animation = family.animation_source;
        if (animation !== undefined && animation !== null) {
          entries.push({
            address: animation.address,
            size: animation.size,
            kind: "golden-sun-kind2-lz",
            plan: `${directory}/animation_source.kind2.json`,
            layout: animationLayout,
            components: [{
              kind: "gba-4bpp-tiles", size: "0x4000",
              source: `${directory}/animation_source.4bpp.png`,
            }],
          });
        }
      }
    } else if (series.kind === "golden-sun-standalone-palette-series") {
      for (const palette of series.palettes) {
        const name = String(palette.id).toLowerCase();
        const directory = `assets/graphics/resource_${name}`;
        entries.push({
          address: palette.address,
          size: palette.size,
          kind: "golden-sun-general-lz",
          plan: `${directory}/palette.lz.json`,
          components: [{
            kind: "gba-palette", size: "0x1c0", source: `${directory}/palette.224.png`,
          }],
        });
      }
    } else if (series.kind === "golden-sun-color-table-series") {
      for (const resource of series.resources) {
        const name = String(resource.id).toLowerCase();
        const directory = `assets/graphics/resource_${name}`;
        entries.push({
          address: resource.address, size: resource.size,
          kind: "gba-palette-rgba", source: `${directory}/color_table.rgba.png`,
        });
      }
    } else if (series.kind === "golden-sun-standalone-tile-series") {
      for (const resource of series.resources) {
        const name = String(resource.id).toLowerCase();
        const directory = `assets/graphics/resource_${name}`;
        entries.push({
          address: resource.address,
          size: resource.size,
          kind: "golden-sun-kind2-lz",
          plan: `${directory}/tiles.kind2.json`,
          components: [{
            kind: "gba-4bpp-tiles", size: "0x4000", source: `${directory}/tiles.4bpp.png`,
          }],
        });
      }
    } else if (series.kind === "golden-sun-prefill-lz-series") {
      for (const resource of series.resources) {
        const name = String(resource.id).toLowerCase();
        const directory = `assets/data/resource_${name}`;
        entries.push({
          address: resource.address,
          size: resource.size,
          kind: "golden-sun-general-lz",
          plan: `${directory}/stream.lz.json`,
          components: [{
            kind: "raw-lz-bytes", size: resource.decoded_size, source: `${directory}/content.png`,
          }],
        });
      }
    } else if (series.kind === "golden-sun-thumb-overlay-series") {
      for (const resource of series.resources) {
        const name = String(resource.id).toLowerCase();
        const directory = `assets/code/resource_${name}`;
        entries.push({
          address: resource.address,
          size: resource.size,
          kind: "golden-sun-general-lz",
          plan: `${directory}/stream.lz.json`,
          components: [{
            kind: "golden-sun-thumb-overlay",
            size: resource.decoded_size,
            source: `${directory}/overlay.s`,
            base: series.base,
          }],
        });
      }
    } else if (series.kind === "golden-sun-map-grid-series") {
      for (const grid of series.grids) {
        const name = String(grid.id).toLowerCase();
        const directory = `assets/maps/resource_${name}/grid`;
        entries.push({
          address: grid.address,
          size: grid.size,
          kind: "golden-sun-kind1-grid",
          source: directory,
          plan: `${directory}/grid.kind1.json`,
        });
      }
    } else if (series.kind === "golden-sun-map-component-series") {
      for (const family of series.families) {
        const name = String(family.id).toLowerCase();
        const directory = `assets/maps/resource_${name}/components`;
        const header = family.header;
        const container = number(header.address);
        const offsetsCheck: Record<number, number> = {};
        for (const component of family.components) {
          offsetsCheck[number(component.slot)] = number(component.address) - container;
        }
        offsetsCheck[2] = gridAddresses[name] - container;
        entries.push({
          address: header.address,
          size: header.size,
          kind: "golden-sun-map-container-header",
          source: `${directory}/header.json`,
          offsets_check: offsetsCheck,
        });
        const sources: Record<number, [string, string, string | null]> = {
          0: ["golden-sun-map-metatiles", `${directory}/metatiles.tilemap`, `${directory}/metatiles.lz.json`],
          1: ["golden-sun-map-descriptors", `${directory}/descriptors.json`, `${directory}/descriptors.lz.json`],
          3: ["golden-sun-map-animation-queues", `${directory}/animation_queues.json`, `${directory}/animation_queues.lz.json`],
          4: ["golden-sun-map-blend-animation", `${directory}/blend_animation.json`, `${directory}/blend_animation.lz.json`],
          5: ["golden-sun-map-sparse-cells", `${directory}/sparse_cells.json`, null],
        };
        for (const component of family.components) {
          const slot = number(component.slot);
          if (!(slot in sources)) throw new Error("unsupported map component slot");
          const [kind, source, plan] = sources[slot];
          const entry: Json = {
            address: component.address, size: component.size, kind, source,
          };
          if (plan !== null) entry.plan = plan;
          entries.push(entry);
        }
      }
    } else if (series.kind === "golden-sun-sound-sequence-series") {
      const index = JSON.parse(readFileSync(sourcePath(String(series.index)), "utf8"));
      if (index.format !== 1 || index.engine !== "smsh-sequence-series" ||
          !Array.isArray(index.sequences)) {
        throw new Error("unsupported sound-sequence series");
      }
      const directory = dirname(String(series.index));
      for (const sequence of index.sequences) {
        entries.push({
          address: sequence.address,
          size: sequence.size,
          kind: "golden-sun-sound-sequence",
          source: join(directory, String(sequence.source)),
        });
      }
    } else if (series.kind === "golden-sun-pcm-wave-series") {
      const index = JSON.parse(readFileSync(sourcePath(String(series.index)), "utf8"));
      if (index.format !== 1 || index.engine !== "smsh-pcm-wave-series" ||
          !Array.isArray(index.waves)) {
        throw new Error("unsupported PCM-wave series");
      }
      const directory = dirname(String(series.index));
      for (const wave of index.waves) {
        entries.push({
          ...wave,
          kind: "golden-sun-pcm-wave",
          source: join(directory, String(wave.source)),
        });
      }
    } else {
      throw new Error("unsupported asset series");
    }
  }
}

function buildEntry(entry: Json): [Buffer, string[], Json] {
  const kind = String(entry.kind);
  if (["gba-4bpp-tiles", "gba-8bpp-tiles", "gba-palette", "gba-palette-rgba"].includes(kind)) {
    const [built, report] = buildComponent(entry);
    return [built, [String(entry.source)], report];
  }
  if (kind === "golden-sun-general-lz") {
    const parts: Buffer[] = [];
    const componentReports: Json[] = [];
    const sources: string[] = [];
    for (const component of entry.components) {
      const [data, details] = buildComponent(component);
      parts.push(data);
      if (component.kind === "zero-skip-sprite-archive") {
        const archivePlan = JSON.parse(readFileSync(sourcePath(component.plan), "utf8"));
        for (let index = 0; index < number(archivePlan.images); index++) {
          sources.push(join(String(component.source), `frame_${String(index).padStart(2, "0")}.png`));
        }
        sources.push(String(component.plan), String(component.palette));
      } else {
        sources.push(String(component.source));
      }
      componentReports.push({ kind: component.kind, source: component.source, details });
    }
    const decoded = Buffer.concat(parts);
    const plan = JSON.parse(readFileSync(sourcePath(String(entry.plan)), "utf8"));
    if (plan.format !== 1 || ![
      "golden-sun-general-lz",
      "golden-sun-palette-lz",
      "golden-sun-tagged-palette-lz",
      "golden-sun-general-lz-prefill",
    ].includes(plan.codec)) {
      throw new Error("unsupported custom-LZ plan");
    }
    if (decoded.length !== number(plan.decoded_size)) {
      throw new Error("decoded components do not match plan size");
    }
    let built: Buffer;
    if (plan.codec === "golden-sun-general-lz-prefill") {
      built = encode_general_prefill(
        decoded, plan.tokens, number(plan.prefill), number(plan.header ?? 1),
      );
    } else if (plan.codec === "golden-sun-general-lz") {
      built = encode_general(decoded, plan.tokens);
    } else {
      built = encode_palette(decoded, plan.tokens);
      if (plan.codec === "golden-sun-tagged-palette-lz") {
        if (number(plan.tag ?? -1) !== 1) {
          throw new Error("tagged palette-LZ plan is missing tag 1");
        }
        built = Buffer.concat([Buffer.from([1]), built]);
      }
    }
    built = Buffer.concat([built, Buffer.from(String(plan.lookahead ?? ""), "hex")]);
    sources.push(String(entry.plan));
    return [built, sources, {
      decoded_size: decoded.length,
      tokens: plan.tokens.length,
      components: componentReports,
    }];
  }
  if (kind === "golden-sun-kind2-lz") {
    const parts: Buffer[] = [];
    const componentReports: Json[] = [];
    const sources: string[] = [];
    for (const component of entry.components) {
      const [data, originalDetails] = buildComponent(component);
      const details = { ...originalDetails };
      parts.push(data);
      if (details._sources !== undefined) {
        sources.push(...details._sources.map(String));
        delete details._sources;
      } else {
        sources.push(String(component.source));
      }
      componentReports.push({ kind: component.kind, source: component.source, details });
    }
    const decoded = Buffer.concat(parts);
    const plan = JSON.parse(readFileSync(sourcePath(String(entry.plan)), "utf8"));
    if (entry.layout !== undefined && !Bun.deepEquals(plan.layout, entry.layout, true)) {
      throw new Error("tag-2 plan layout differs from manifest");
    }
    if (decoded.length !== number(plan.decoded_size)) {
      throw new Error("decoded tag-2 components do not match plan");
    }
    const built = encode_kind2(decoded, plan);
    sources.push(String(entry.plan));
    return [built, sources, {
      decoded_size: decoded.length,
      tokens: plan.tokens.length,
      layout: plan.layout ?? null,
      components: componentReports,
    }];
  }
  if (kind === "golden-sun-kind1-grid") {
    const directory = sourcePath(String(entry.source));
    const plan = JSON.parse(readFileSync(sourcePath(String(entry.plan)), "utf8"));
    const built = build_grid(plan, directory);
    const sources = [String(entry.plan), ...[
      "value_low.png", "value_high.png", "attribute_a.png",
      "attribute_b.png", "sentinels.png",
    ].map((name) => join(String(entry.source), name))];
    return [built, sources, {
      decoded_size: number(plan.decoded_size), tokens: plan.tokens.length, planes: 4,
    }];
  }
  if ([
    "golden-sun-map-metatiles",
    "golden-sun-map-descriptors",
    "golden-sun-map-animation-queues",
    "golden-sun-map-blend-animation",
  ].includes(kind)) {
    const source = sourcePath(String(entry.source));
    const planPath = sourcePath(String(entry.plan));
    const builders: Record<string, (source: string, plan: string) => Buffer> = {
      "golden-sun-map-metatiles": build_metatiles,
      "golden-sun-map-descriptors": build_descriptors,
      "golden-sun-map-animation-queues": build_queues,
      "golden-sun-map-blend-animation": build_blend_animation,
    };
    const built = builders[kind](source, planPath);
    const plan = JSON.parse(readFileSync(planPath, "utf8"));
    return [built, [String(entry.source), String(entry.plan)], {
      decoded_size: number(plan.decoded_size),
      tokens: plan.tokens.length,
      component: plan.component,
    }];
  }
  if (kind === "golden-sun-map-container-header") {
    const source = sourcePath(String(entry.source));
    const built = build_header(source, entry.offsets_check);
    const document = JSON.parse(readFileSync(source, "utf8"));
    return [built, [String(entry.source)], {
      records: document.records.length, component_offsets: document.component_offsets,
    }];
  }
  if (kind === "golden-sun-map-sparse-cells") {
    const source = sourcePath(String(entry.source));
    const built = build_sparse(source);
    const document = JSON.parse(readFileSync(source, "utf8"));
    return [built, [String(entry.source)], {
      records: document.records.length, alignment_zeros: document.alignment_zeros,
    }];
  }
  if (kind === "golden-sun-map-load-table") {
    const source = sourcePath(String(entry.source));
    const built = build_map_load_table(source);
    const document = JSON.parse(readFileSync(source, "utf8"));
    return [built, [String(entry.source)], { records: document.records.length, record_size: 12 }];
  }
  if (kind === "golden-sun-sound-table") {
    const source = sourcePath(String(entry.source));
    const document = JSON.parse(readFileSync(source, "utf8"));
    const [built, report] = build_sound_table(document);
    return [built, [String(entry.source)], report];
  }
  if (kind === "golden-sun-sound-sequence") {
    const source = sourcePath(String(entry.source));
    const document = JSON.parse(readFileSync(source, "utf8"));
    if (number(document.base) !== number(entry.address)) throw new Error("sound-sequence base differs from manifest");
    const [built, report] = build_sound_sequence(document);
    return [built, [String(entry.source)], report];
  }
  if (kind === "golden-sun-pcm-wave") {
    const source = sourcePath(String(entry.source));
    const [built, report] = buildWaveRecord(entry, readFileSync(source));
    return [built, [String(entry.source)], report];
  }
  if (kind === "golden-sun-delta7-still") {
    const source = sourcePath(String(entry.source));
    const [built, report] = build_still(readFileSync(source));
    return [built, [String(entry.source)], report];
  }
  if (kind === "golden-sun-static-sprite-series") {
    const indexPath = sourcePath(String(entry.source));
    const palettePath = sourcePath(String(entry.palette));
    const index = JSON.parse(readFileSync(indexPath, "utf8"));
    if (number(index.address) !== number(entry.address) || number(index.size) !== number(entry.size)) {
      throw new Error("static-sprite manifest extent differs from its index");
    }
    const [built, report] = build_static_sprite_series(indexPath, palettePath);
    const directory = dirname(String(entry.source));
    const sources = [String(entry.source), String(entry.palette)];
    for (const item of index.packages ?? []) {
      sources.push(join(directory, String(item.source)), join(directory, String(item.plan)));
    }
    return [built, sources, report];
  }
  if (kind === "golden-sun-offset-palette-lz") {
    const planPath = sourcePath(String(entry.plan));
    const atlasPath = sourcePath(String(entry.source));
    const plan = JSON.parse(readFileSync(planPath, "utf8"));
    if (plan.format !== 1 || plan.codec !== kind) throw new Error("unsupported archive plan");
    const built = build_offset_archive(readFileSync(atlasPath), plan);
    return [built, [String(entry.source), String(entry.plan)], {
      streams: plan.streams.length,
      chunk_width: plan.chunk_width,
      chunk_height: plan.chunk_height,
    }];
  }
  if (kind === "golden-sun-mtf4-archive") {
    const planPath = sourcePath(String(entry.plan));
    const directory = sourcePath(String(entry.source));
    const plan = JSON.parse(readFileSync(planPath, "utf8"));
    if (plan.format !== 1 || plan.codec !== kind) throw new Error("unsupported F0 archive plan");
    const built = build_f0_archive(plan, directory);
    const imageSources = Array.from({ length: number(plan.images) }, (_, index) =>
      join(String(entry.source), `image_${String(index).padStart(2, "0")}.png`));
    return [built, [String(entry.plan), ...imageSources], {
      entries: plan.entries.length, images: number(plan.images),
    }];
  }
  throw new Error(`unsupported asset kind: ${kind}`);
}

function usage(): void {
  console.log("usage: build_assets.ts [-h] [--manifest MANIFEST] [-o OUTPUT] [rom]");
}

function parseArgs(argv: string[]): Options {
  const options: Options = {
    rom: "gs1-en.gba", manifest: join(ROOT, "assets/manifest.json"), output: join(ROOT, "out/assets"),
  };
  let positional = false;
  for (let index = 0; index < argv.length; index++) {
    const argument = argv[index];
    if (argument === "-h" || argument === "--help") {
      usage();
      process.exit(0);
    }
    const equal = argument.indexOf("=");
    const option = equal >= 0 ? argument.slice(0, equal) : argument;
    if (option === "--manifest" || option === "-o" || option === "--output") {
      const value = equal >= 0 ? argument.slice(equal + 1) : argv[++index];
      if (value === undefined) throw new Error(`${option} requires a value`);
      if (option === "--manifest") options.manifest = value;
      else options.output = value;
    } else if (!argument.startsWith("-") && !positional) {
      options.rom = argument;
      positional = true;
    } else {
      throw new Error(`unrecognized argument: ${argument}`);
    }
  }
  return options;
}

function main(): void {
  const args = parseArgs(Bun.argv.slice(2));
  const rom = readFileSync(resolve(process.cwd(), args.rom));
  const manifestPath = isAbsolute(args.manifest) ? args.manifest : resolve(process.cwd(), args.manifest);
  const manifest = JSON.parse(readFileSync(manifestPath, "utf8"));
  if (manifest.format !== 1) throw new Error("unsupported asset manifest format");
  const output = isAbsolute(args.output) ? args.output : resolve(ROOT, args.output);
  mkdirSync(output, { recursive: true });
  const entries: Json[] = [...(manifest.regions ?? [])];
  expandSeries(manifest, entries);
  const regions: Json[] = [];
  let previousEnd = ROM_BASE;
  entries.sort((left, right) => number(left.address) - number(right.address));
  for (const entry of entries) {
    const address = number(entry.address);
    const size = number(entry.size);
    if (address < previousEnd) {
      throw new Error(`overlapping asset region at 0x${address.toString(16).padStart(8, "0")}`);
    }
    previousEnd = address + size;
    if (!(ROM_BASE <= address && address < previousEnd && previousEnd <= ROM_BASE + rom.length)) {
      throw new Error(`asset region outside ROM at 0x${address.toString(16).padStart(8, "0")}`);
    }
    const [builtData, sources, report] = buildEntry(entry);
    if (builtData.length !== size) {
      throw new Error(
        `asset at 0x${address.toString(16).padStart(8, "0")}: ` +
        `built 0x${builtData.length.toString(16)}, expected 0x${size.toString(16)}`,
      );
    }
    const expected = rom.subarray(address - ROM_BASE, address - ROM_BASE + size);
    if (!builtData.equals(expected)) {
      throw new Error(`asset at 0x${address.toString(16).padStart(8, "0")}: encoded bytes differ`);
    }
    const built = join(output, `${address.toString(16).padStart(8, "0")}.bin`);
    writeFileSync(built, builtData);
    regions.push({
      address, size, end: address + size, kind: entry.kind,
      sources, output: built, details: report,
    });
  }
  prune_files(output, "*.bin", regions.map((region) => basename(String(region.output))));
  const unusedImages = unused_tracked_images(ROOT, regions, ["assets/readme/"]);
  if (unusedImages.length !== 0) {
    let listing = unusedImages.slice(0, 20).map((name) => `  ${name}`).join("\n");
    if (unusedImages.length > 20) listing += `\n  ... and ${unusedImages.length - 20} more`;
    throw new Error(`tracked images are not byte-verified asset sources:\n${listing}`);
  }
  const assetBytes = regions.reduce((sum, item) => sum + number(item.size), 0);
  writeFileSync(join(output, "manifest.json"), JSON.stringify({
    format: 1,
    rom_base: ROM_BASE,
    rom_size: rom.length,
    asset_bytes: assetBytes,
    regions,
  }, null, 2) + "\n");
  console.log(`assets=${regions.length} bytes=${assetBytes}`);
}

if (import.meta.main) main();

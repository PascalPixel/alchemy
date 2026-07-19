#!/usr/bin/env bun
import { existsSync, mkdirSync, readFileSync, writeFileSync } from "node:fs";
import { basename, dirname, isAbsolute, join, relative, resolve } from "node:path";
import { encode_general, encode_general_prefill, encode_palette } from "./extract_resource.ts";
import { characterBankPath, resourceGraphicsDir } from "./asset_paths.ts";
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
import { build_static_sprite_series, static_sprite_frame_name } from "./static_sprite_series.ts";
import { build_resource_directory } from "./resource_directory.ts";
import { build_title_resource } from "./title_resources.ts";
import { build_simple_resource } from "./simple_resources.ts";
import { build_character_catalog } from "./character_catalog.ts";
import { build_message_archive } from "./message_archive.ts";
import { build_resource_5 } from "./resource_5.ts";
import { build_localization_font } from "./localization_font.ts";
import { build_localization_tables } from "./localization_tables.ts";
import { build_battle_effect_data } from "./battle_effect_data.ts";
import {
  build_sentou_gamen_data,
  SENTOU_GAMEN_ADDRESS,
  SENTOU_GAMEN_SIZE,
} from "./sentou_gamen_data.ts";
import { build_sentou_hyouji } from "./sentou_hyouji.ts";
import { build_sentou_kouka_runtime } from "./sentou_kouka_runtime.ts";
import {
  build_sentou_menu_data,
  SENTOU_MENU_ADDRESS,
  SENTOU_MENU_SIZE,
} from "./sentou_menu_data.ts";
import { build_sentou_resource, build_sentou_series } from "./sentou_resources.ts";
import { build_kind2_resource, build_kind2_series } from "./kind2_resource_series.ts";
import { build_encounter_regions } from "./encounter_data.ts";
import { build_namae_nyuuryoku } from "./namae_nyuuryoku.ts";
import { build_tokushu_map_series } from "./tokushu_map_resources.ts";
import { build_resource_3ce } from "./resource_3ce.ts";
import { build_chiiki_map_series } from "./chiiki_map_resources.ts";
import {
  build_resource_01c,
  RESOURCE_ADDRESS as RESOURCE_01C_ADDRESS,
  RESOURCE_SIZE as RESOURCE_01C_SIZE,
} from "./resource_01c.ts";
import { build_music_residuals } from "./music_residuals.ts";
import { build_resource_d1_d3 } from "./resource_d1_d3.ts";
import {
  AUDIO_ENGINE_ADDRESS,
  AUDIO_ENGINE_SIZE,
  build_audio_engine_data,
} from "./audio_engine_data.ts";
import {
  build_runtime_support_component,
  parse_runtime_support_source,
} from "./runtime_support_data.ts";
import {
  BYTE_HENKAN_ADDRESS,
  BYTE_HENKAN_SIZE,
  build_byte_henkan_tables,
} from "./byte_henkan.ts";
import {
  build_staff_roll,
  STAFF_ROLL_ADDRESS,
  STAFF_ROLL_SIZE,
} from "./staff_roll.ts";
import { buildGbaHeaderComponent, parseGbaHeaderSource } from "./gba_header.ts";
import { build_early_runtime_data } from "./early_runtime_data.ts";
import { buildLateRuntimeResidual } from "./late_runtime_residual.ts";
import { buildResourceByteCanvases } from "./resource_byte_canvases.ts";
import { buildByteValueRegions } from "./byte_value_regions.ts";
import { buildExecutableGapData } from "./executable_gap_sources.ts";

const ROOT = dirname(dirname(Bun.fileURLToPath(import.meta.url)));
const ROM_BASE = 0x08000000;
const ROM_SIZE = 0x00800000;

type Json = Record<string, any>;

interface Options {
  rom: string;
  manifest: string;
  output: string;
  sourceOnly: boolean;
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

function canonicalJsonSource(name: string, label: string): Json {
  const path = sourcePath(name);
  const text = readFileSync(path, "utf8");
  const value = JSON.parse(text);
  if (typeof value !== "object" || value === null || Array.isArray(value) ||
      text !== `${JSON.stringify(value, null, 2)}\n`) {
    throw new Error(`${label} is not canonical JSON`);
  }
  return value;
}

function exactKeys(value: Json, keys: string[], label: string): void {
  const actual = Object.keys(value).sort();
  const expected = [...keys].sort();
  if (actual.length !== expected.length || actual.some((key, index) => key !== expected[index])) {
    throw new Error(`${label} fields differ`);
  }
}

function closureCoverage(items: Json[], label: string): Array<{ address: number; end: number }> {
  const regions = items.map((item, index) => {
    if (typeof item !== "object" || item === null || Array.isArray(item)) {
      throw new Error(`${label} region ${index} differs`);
    }
    const address = number(item.address);
    const size = number(item.size);
    if (!Number.isSafeInteger(address) || !Number.isSafeInteger(size) || size <= 0 ||
        address < ROM_BASE || address + size > ROM_BASE + ROM_SIZE) {
      throw new Error(`${label} region ${index} extent differs`);
    }
    return { address, end: address + size };
  }).sort((left, right) => left.address - right.address);
  const merged: Array<{ address: number; end: number }> = [];
  for (const region of regions) {
    const previous = merged.at(-1);
    if (previous !== undefined && region.address < previous.end) throw new Error(`${label} regions overlap`);
    if (previous !== undefined && region.address === previous.end) previous.end = region.end;
    else merged.push(region);
  }
  return merged;
}

function expandClosurePackages(manifest: Json, entries: Json[]): void {
  const supported = new Set([
    "golden-sun-asset-fragment",
    "golden-sun-kind2-resource-series",
    "golden-sun-pcm-wave-series",
  ]);
  for (const [position, raw] of (manifest.closure_packages ?? []).entries()) {
    if (typeof raw !== "object" || raw === null || Array.isArray(raw)) {
      throw new Error(`closure package ${position} differs`);
    }
    const packageSpec = raw as Json;
    exactKeys(packageSpec, ["id", "kind", "availability", "index", "expected_ranges"], `closure package ${position}`);
    if (typeof packageSpec.id !== "string" || !/^[a-z][a-z0-9_]*$/.test(packageSpec.id) ||
        !supported.has(String(packageSpec.kind)) ||
        (packageSpec.availability !== "pending" && packageSpec.availability !== "required") ||
        typeof packageSpec.index !== "string" || !Array.isArray(packageSpec.expected_ranges) ||
        packageSpec.expected_ranges.length === 0) {
      throw new Error(`closure package ${position} identity differs`);
    }
    const indexName = String(packageSpec.index);
    const indexPath = sourcePath(indexName);
    if (!existsSync(indexPath)) {
      if (packageSpec.availability === "pending") continue;
      throw new Error(`${packageSpec.id}: required closure package is missing`);
    }
    const document = canonicalJsonSource(indexName, `${packageSpec.id} closure package`);
    const generated: Json[] = [];
    if (packageSpec.kind === "golden-sun-asset-fragment") {
      exactKeys(document, ["format", "kind", "id", "regions", "series"], `${packageSpec.id} closure package`);
      if (document.format !== 1 || document.kind !== "golden-sun-asset-fragment" ||
          document.id !== packageSpec.id || !Array.isArray(document.regions) || !Array.isArray(document.series)) {
        throw new Error(`${packageSpec.id}: closure package identity differs`);
      }
      generated.push(...document.regions);
      expandSeries({ series: document.series }, generated);
    } else {
      expandSeries({ series: [{ kind: packageSpec.kind, index: indexName }] }, generated);
    }
    const expected = closureCoverage(packageSpec.expected_ranges, `${packageSpec.id} expected coverage`);
    const actual = closureCoverage(generated, `${packageSpec.id} generated coverage`);
    if (expected.length !== actual.length || expected.some((region, index) =>
      region.address !== actual[index].address || region.end !== actual[index].end)) {
      throw new Error(`${packageSpec.id}: generated coverage differs from its declared extent`);
    }
    for (const entry of generated) {
      if (entry._closure_package !== undefined) throw new Error(`${packageSpec.id}: nested closure provenance differs`);
      entry._closure_package = indexName;
      entries.push(entry);
    }
  }
}

function closureSources(entry: Json, sources: string[]): string[] {
  if (entry._closure_package === undefined) return sources;
  const packageSource = String(entry._closure_package);
  sourcePath(packageSource);
  return sources.includes(packageSource) ? sources : [packageSource, ...sources];
}

function closurePackageSelfTest(): void {
  const missing = "assets/data/closure/__self_test_missing__/index.json";
  if (existsSync(sourcePath(missing))) throw new Error("closure package self-test path exists");
  const optional = {
    closure_packages: [{
      id: "missing_optional",
      kind: "golden-sun-asset-fragment",
      availability: "pending",
      index: missing,
      expected_ranges: [{ address: "0x08001000", size: "0x10" }],
    }],
  };
  const absent: Json[] = [];
  expandClosurePackages(optional, absent);
  if (absent.length !== 0) throw new Error("missing optional closure package emitted regions");
  let rejected = false;
  try {
    expandClosurePackages({
      closure_packages: [{ ...optional.closure_packages[0], availability: "required" }],
    }, []);
  } catch { rejected = true; }
  if (!rejected) throw new Error("missing required closure package was accepted");
  const pcmIndex = "assets/audio/waves/index.json";
  const pcm = canonicalJsonSource(pcmIndex, "PCM self-test index");
  if (!Array.isArray(pcm.waves) || pcm.waves.length === 0) throw new Error("PCM self-test index differs");
  const expectedRanges = pcm.waves.map((wave: Json) => ({ address: wave.address, size: wave.size }));
  const entries: Json[] = [];
  expandClosurePackages({
    closure_packages: [{
      id: "existing_pcm",
      kind: "golden-sun-pcm-wave-series",
      availability: "required",
      index: pcmIndex,
      expected_ranges: expectedRanges,
    }],
  }, entries);
  if (entries.length !== pcm.waves.length || entries.some((entry) => entry._closure_package !== pcmIndex)) {
    throw new Error("present closure package provenance differs");
  }
  const sources = closureSources(entries[0], [String(entries[0].source)]);
  if (sources[0] !== pcmIndex || sources.length !== 2) throw new Error("closure package source provenance differs");
  rejected = false;
  try {
    closureCoverage([
      { address: "0x08001000", size: "0x10" },
      { address: "0x08001008", size: "0x10" },
    ], "overlap self-test");
  } catch { rejected = true; }
  if (!rejected) throw new Error("overlapping closure coverage was accepted");
  console.log(`self-test=ok optional=skipped present_regions=${entries.length} provenance=verified`);
}

const tokushuMapCache = new Map<string, ReturnType<typeof build_tokushu_map_series>>();
const resource3ceCache = new Map<string, ReturnType<typeof build_resource_3ce>>();
const chiikiMapCache = new Map<string, ReturnType<typeof build_chiiki_map_series>>();
const musicResidualCache = new Map<string, ReturnType<typeof build_music_residuals>>();
const resourceD1D3Cache = new Map<string, ReturnType<typeof build_resource_d1_d3>>();

function tokushuMaps(indexName: string): ReturnType<typeof build_tokushu_map_series> {
  const indexPath = sourcePath(indexName);
  let built = tokushuMapCache.get(indexPath);
  if (built === undefined) {
    built = build_tokushu_map_series(indexPath);
    tokushuMapCache.set(indexPath, built);
  }
  return built;
}

function resource3ce(layoutName: string): ReturnType<typeof build_resource_3ce> {
  const layoutPath = sourcePath(layoutName);
  let built = resource3ceCache.get(layoutPath);
  if (built === undefined) {
    built = build_resource_3ce(layoutPath);
    resource3ceCache.set(layoutPath, built);
  }
  return built;
}

function chiikiMaps(indexName: string): ReturnType<typeof build_chiiki_map_series> {
  const indexPath = sourcePath(indexName);
  let built = chiikiMapCache.get(indexPath);
  if (built === undefined) {
    built = build_chiiki_map_series(indexPath);
    chiikiMapCache.set(indexPath, built);
  }
  return built;
}

function musicResiduals(indexName: string): ReturnType<typeof build_music_residuals> {
  const indexPath = sourcePath(indexName);
  let built = musicResidualCache.get(indexPath);
  if (built === undefined) {
    built = build_music_residuals(indexPath);
    musicResidualCache.set(indexPath, built);
  }
  return built;
}

function resourcesD1D3(indexName: string): ReturnType<typeof build_resource_d1_d3> {
  const indexPath = sourcePath(indexName);
  let built = resourceD1D3Cache.get(indexPath);
  if (built === undefined) {
    built = build_resource_d1_d3(indexPath);
    resourceD1D3Cache.set(indexPath, built);
  }
  return built;
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
          source: `${resourceGraphicsDir(name)}/ichimaie.8bpp.png`,
        });
      }
    } else if (series.kind === "golden-sun-zero-skip-sprite-series") {
      for (const resource of series.resources) {
        const name = String(resource.id).toLowerCase();
        const directory = resourceGraphicsDir(name);
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
        const directory = resourceGraphicsDir(name);
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
        const directory = resourceGraphicsDir(name);
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
        const directory = resourceGraphicsDir(name);
        entries.push({
          address: resource.address, size: resource.size,
          kind: "gba-palette-rgba", source: `${directory}/color_table.rgba.png`,
        });
      }
    } else if (series.kind === "golden-sun-standalone-tile-series") {
      for (const resource of series.resources) {
        const name = String(resource.id).toLowerCase();
        const directory = resourceGraphicsDir(name);
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
    } else if (series.kind === "golden-sun-sentou-resource-series") {
      const indexFile = sourcePath(String(series.index));
      const resources = build_sentou_series(indexFile);
      for (const resource of resources) {
        const source = relative(ROOT, resolve(resource.sources[1]));
        sourcePath(source);
        entries.push({
          address: resource.address,
          size: resource.data.length,
          kind: "golden-sun-sentou-resource",
          source,
          index: String(series.index),
        });
      }
    } else if (series.kind === "golden-sun-kind2-resource-series") {
      const indexFile = sourcePath(String(series.index));
      const resources = build_kind2_series(indexFile);
      for (const resource of resources) {
        const source = relative(ROOT, resolve(resource.sources[1]));
        sourcePath(source);
        entries.push({
          address: resource.address,
          size: resource.data.length,
          kind: "golden-sun-kind2-resource",
          source,
          index: String(series.index),
        });
      }
    } else if (series.kind === "golden-sun-tokushu-map-series") {
      const indexName = String(series.index);
      for (const resource of tokushuMaps(indexName)) {
        entries.push({
          address: resource.address,
          size: resource.data.length,
          kind: "golden-sun-tokushu-map",
          source: indexName,
          resource_id: resource.id,
        });
      }
    } else if (series.kind === "golden-sun-chiiki-map-series") {
      const indexName = String(series.index);
      for (const resource of chiikiMaps(indexName)) {
        entries.push({
          address: resource.address,
          size: resource.data.length,
          kind: "golden-sun-chiiki-map",
          source: indexName,
          resource_id: resource.id,
        });
      }
    } else if (series.kind === "golden-sun-final-battle-overlay-series") {
      const source = String(series.source);
      const built = resource3ce(source);
      entries.push({
        address: built.stream_address,
        size: built.stream.length,
        kind: "golden-sun-final-battle-overlay",
        source,
        component: "stream",
      });
      entries.push({
        address: built.fill_address,
        size: built.fill.length,
        kind: "golden-sun-final-battle-overlay",
        source,
        component: "fill",
      });
    } else if (series.kind === "golden-sun-encounter-data-series") {
      const directoryName = String(series.directory);
      const directory = sourcePath(directoryName);
      for (const region of build_encounter_regions(directory)) {
        entries.push({
          address: region.address,
          size: region.size,
          kind: "golden-sun-encounter-data",
          source: join(directoryName, region.source),
        });
      }
    } else if (series.kind === "golden-sun-music-residuals") {
      const indexName = String(series.index);
      for (const region of musicResiduals(indexName)) {
        entries.push({
          address: region.address,
          size: region.data.length,
          kind: "golden-sun-music-residual",
          source: indexName,
        });
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
          index: String(series.index),
        });
      }
    } else {
      throw new Error("unsupported asset series");
    }
  }
}

function buildEntry(entry: Json): [Buffer, string[], Json] {
  const kind = String(entry.kind);
  if (kind === "gba-cartridge-header-standard-fields") {
    const sourceName = String(entry.source);
    const document = parseGbaHeaderSource(JSON.parse(readFileSync(sourcePath(sourceName), "utf8")));
    const logoName = document.standard.logo.source;
    const built = buildGbaHeaderComponent(
      document, readFileSync(sourcePath(logoName)), number(entry.address), number(entry.size),
    );
    return [built, [sourceName, logoName], { standard_header_bytes: built.length }];
  }
  if (kind === "golden-sun-early-runtime-data") {
    const sourceName = String(entry.source);
    const built = build_early_runtime_data(sourcePath(sourceName));
    const address = number(entry.address);
    const region = built.regions.get(address);
    if (region === undefined) throw new Error("early-runtime asset address is not a produced region");
    return [region, [sourceName, join(dirname(sourceName), "display.4bpp.png")], {
      source_bytes: built.source_bytes,
      region_address: `0x${address.toString(16).padStart(8, "0")}`,
    }];
  }
  if (kind === "golden-sun-late-runtime-residual") {
    const sourceName = String(entry.source);
    const built = buildLateRuntimeResidual(sourcePath(sourceName));
    const address = number(entry.address), region = built.regions.get(address);
    if (region === undefined) throw new Error("late-runtime asset address is not a produced region");
    return [region, [sourceName], { source_bytes: built.sourceBytes, region_address: `0x${address.toString(16).padStart(8, "0")}` }];
  }
  if (kind === "golden-sun-resource-byte-canvas") {
    const sourceName = String(entry.source), resource = String(entry.resource_id).toLowerCase();
    const built = buildResourceByteCanvases(sourcePath(sourceName)).find((item) => item.id === resource);
    if (built === undefined) throw new Error(`resource byte canvas ${resource} is absent`);
    return [built.data, [sourceName, join(dirname(sourceName), built.source)], { resource_id: `0x${resource}`, representation: "provisional-neutral-byte-canvas" }];
  }
  if (kind === "golden-sun-byte-value-regions") {
    const sourceName = String(entry.source), address = number(entry.address);
    const region = buildByteValueRegions(sourcePath(sourceName)).find((item) => item.address === address);
    if (region === undefined || region.data.length !== number(entry.size)) throw new Error("byte-value region differs from manifest");
    return [region.data, [sourceName], { representation: "structured byte values", region_address: `0x${address.toString(16).padStart(8, "0")}` }];
  }
  if (kind === "golden-sun-executable-gap-data") {
    const sourceName = String(entry.source), address = number(entry.address);
    const built = buildExecutableGapData(sourcePath(sourceName)).find((item) => item.address === address);
    if (built === undefined) throw new Error("executable gap data address is not a produced region");
    return [built.data, [sourceName], { representation: "typed mixed-region table", region_address: `0x${address.toString(16).padStart(8, "0")}` }];
  }
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
    const sources = [String(entry.source)];
    if (entry.index !== undefined) {
      sourcePath(String(entry.index));
      sources.unshift(String(entry.index));
    }
    return [built, sources, report];
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
      // 分類済みの battle/field 配下も追跡対象に含める。
      const planSource = relative(ROOT, characterBankPath(resolve(ROOT, directory), String(item.plan)));
      sources.push(planSource);
      const plan = JSON.parse(readFileSync(sourcePath(planSource), "utf8"));
      if (Array.isArray(plan.atlases)) {
        for (const atlas of plan.atlases) sources.push(join(dirname(planSource), String(atlas.source)));
      } else if (plan.atlas_columns !== undefined) {
        sources.push(join(directory, String(item.source)));
      } else {
        for (let frame = 0; frame < plan.frames.length; frame++) {
          sources.push(join(dirname(planSource), static_sprite_frame_name(frame)));
        }
      }
    }
    return [built, sources, report];
  }
  if (kind === "golden-sun-resource-directory") {
    const source = sourcePath(String(entry.source));
    const document = JSON.parse(readFileSync(source, "utf8"));
    if (number(document.address) !== number(entry.address)) {
      throw new Error("resource-directory address differs from manifest");
    }
    const built = build_resource_directory(document);
    return [built, [String(entry.source)], { slots: built.length / 4 }];
  }
  if (kind === "golden-sun-runtime-support-data") {
    const source = sourcePath(String(entry.source));
    const document = parse_runtime_support_source(readFileSync(source, "utf8"));
    const built = build_runtime_support_component(document, number(entry.address), number(entry.size));
    return [built, [String(entry.source)], { component_address: entry.address, bytes: built.length }];
  }
  if (kind === "golden-sun-byte-henkan-tables") {
    const sourceName = String(entry.source);
    const built = build_byte_henkan_tables(sourcePath(sourceName));
    if (number(entry.address) !== BYTE_HENKAN_ADDRESS || number(entry.size) !== BYTE_HENKAN_SIZE ||
        built.length !== BYTE_HENKAN_SIZE) {
      throw new Error("byte-conversion tables differ from canonical manifest extent");
    }
    return [built, [sourceName], {
      source_bytes: built.length,
      tables: 9,
      derived_zero_bytes: 288,
    }];
  }
  if (kind === "golden-sun-character-catalog") {
    const source = sourcePath(String(entry.source));
    const document = JSON.parse(readFileSync(source, "utf8"));
    if (number(document.address) !== number(entry.address) || number(document.size) !== number(entry.size)) {
      throw new Error("character-catalog extent differs from manifest");
    }
    const built = build_character_catalog(document);
    return [built, [String(entry.source)], {
      descriptors: Object.keys(document.descriptors).length,
      animation_groups: document.animation_groups.length,
      frame_directories: document.frame_directories.length,
    }];
  }
  if (kind === "golden-sun-message-archive") {
    const source = sourcePath(String(entry.source));
    const document = JSON.parse(readFileSync(source, "utf8"));
    if (number(document.address) !== number(entry.address) || number(document.size) !== number(entry.size)) {
      throw new Error("message-archive extent differs from manifest");
    }
    const built = build_message_archive(document);
    return [built, [String(entry.source)], {
      banks: document.banks.length,
      messages: document.banks.reduce((sum: number, bank: Json[]) => sum + bank.length, 0),
    }];
  }
  if (kind === "golden-sun-localization-font") {
    const source = sourcePath(String(entry.source));
    const document = JSON.parse(readFileSync(source, "utf8"));
    if (number(document.address) !== number(entry.address) || number(document.size) !== number(entry.size)) {
      throw new Error("localization-font extent differs from manifest");
    }
    const nested = [
      ...document.direct_tiles.map((item: Json) => `assets/${String(item.source)}`),
      ...document.mtf_banks.map((item: Json) => `assets/${String(item.source)}`),
      `assets/${String(document.packed_images.source)}`,
      `assets/${String(document.font.source)}`,
    ];
    nested.forEach(sourcePath);
    const built = build_localization_font(document, join(ROOT, "assets"));
    return [built, [String(entry.source), ...nested], {
      mtf_images: document.mtf_banks.reduce((sum: number, bank: Json) => sum + number(bank.images), 0),
      packed_images: document.packed_images.images,
      font_glyphs: document.font.glyphs,
      article_entries: document.articles.entries.length,
    }];
  }
  if (kind === "golden-sun-localization-tables") {
    const source = sourcePath(String(entry.source));
    const document = JSON.parse(readFileSync(source, "utf8"));
    if (number(document.address) !== number(entry.address) || number(document.size) !== number(entry.size)) {
      throw new Error("localization-table extent differs from manifest");
    }
    const built = build_localization_tables(document);
    return [built, [String(entry.source)], { segments: document.segments.length }];
  }
  if (kind === "golden-sun-battle-effect-data") {
    const source = sourcePath(String(entry.source));
    const document = JSON.parse(readFileSync(source, "utf8"));
    if (number(document.address) !== number(entry.address) ||
        number(document.end) - number(document.address) !== number(entry.size))
      throw new Error("battle-effect extent differs from manifest");
    const nested = [
      ...document.direct_graphics.map((item: Json) => `assets/${String(item.source)}`),
      `assets/${String(document.halfword_graphic.source)}`,
      ...document.palette_graphics.map((item: Json) => `assets/${String(item.source)}`),
    ];
    nested.forEach(sourcePath);
    const built = build_battle_effect_data(document, join(ROOT, "assets"));
    return [built, [String(entry.source), ...nested], {
      graphics: document.direct_graphics.length + 1 + document.palette_graphics.length,
      weighted_records: document.weighted_records.records.length,
      typed_tables: document.typed_tables.length,
    }];
  }
  if (kind === "golden-sun-sentou-gamen-data") {
    const sourceName = String(entry.source);
    const [built, nestedPaths] = build_sentou_gamen_data(sourcePath(sourceName));
    if (number(entry.address) !== SENTOU_GAMEN_ADDRESS || number(entry.size) !== SENTOU_GAMEN_SIZE ||
        built.length !== SENTOU_GAMEN_SIZE) {
      throw new Error("battle-screen package differs from canonical manifest extent");
    }
    const nested = nestedPaths.map((name) => relative(ROOT, resolve(name)));
    nested.forEach(sourcePath);
    return [built, [...new Set([sourceName, ...nested])], {
      source_bytes: built.length,
      graphics: 5,
      display_glyph_cells: 14,
      derived_zero_bytes: 3308,
    }];
  }
  if (kind === "golden-sun-sentou-hyouji") {
    const sourceName = String(entry.source);
    const source = sourcePath(sourceName);
    const document = JSON.parse(readFileSync(source, "utf8"));
    if (number(document.address) !== number(entry.address) || number(document.size) !== number(entry.size))
      throw new Error("battle-display extent differs from manifest");
    const directory = dirname(sourceName);
    const nested = [
      sourceName,
      join(directory, String(document.sources.kihon)),
      join(directory, String(document.sources.koma.source)),
      join(directory, String(document.sources.haichi)),
      join(directory, String(document.sources.hosei)),
      join(directory, String(document.sources.gauge.source)),
    ];
    nested.forEach(sourcePath);
    const built = build_sentou_hyouji(source);
    return [built, nested, { source_bytes: built.length, typed_tables: 3, atlases: 2 }];
  }
  if (kind === "golden-sun-sentou-kouka-runtime") {
    const sourceName = String(entry.source);
    const source = sourcePath(sourceName);
    const built = build_sentou_kouka_runtime(source);
    if (number(entry.address) !== 0x080eda78 || built.length !== number(entry.size))
      throw new Error("battle-effect runtime differs from manifest");
    const document = JSON.parse(readFileSync(source, "utf8"));
    const directory = dirname(sourceName);
    const nested = [sourceName, ...Object.values(document.sources).map((name) => join(directory, String(name)))];
    nested.forEach(sourcePath);
    return [built, nested, {
      source_bytes: built.length,
      callback_slots: 407,
      derived_zero_bytes: 4012,
    }];
  }
  if (kind === "golden-sun-sentou-menu-data") {
    const sourceName = String(entry.source);
    const [built, nestedPaths] = build_sentou_menu_data(sourcePath(sourceName));
    if (number(entry.address) !== SENTOU_MENU_ADDRESS || number(entry.size) !== SENTOU_MENU_SIZE ||
        built.length !== SENTOU_MENU_SIZE) {
      throw new Error("battle-menu package differs from canonical manifest extent");
    }
    const nested = nestedPaths.map((name) => relative(ROOT, resolve(name)));
    nested.forEach(sourcePath);
    return [built, [...new Set(nested)], {
      source_bytes: built.length,
      atlases: 5,
      loadout_records: 35,
    }];
  }
  if (kind === "golden-sun-staff-roll") {
    const sourceName = String(entry.source);
    const [built, nestedPaths] = build_staff_roll(sourcePath(sourceName));
    if (number(entry.address) !== STAFF_ROLL_ADDRESS || number(entry.size) !== STAFF_ROLL_SIZE ||
        built.length !== STAFF_ROLL_SIZE) {
      throw new Error("staff-roll package differs from canonical manifest extent");
    }
    const nested = nestedPaths.map((name) => relative(ROOT, resolve(name)));
    nested.forEach(sourcePath);
    return [built, [...new Set([sourceName, ...nested])], {
      source_bytes: built.length,
      preload_slots: 33,
      strings: 110,
      line_entries: 339,
      font_glyphs: 96,
    }];
  }
  if (kind === "golden-sun-sentou-resource") {
    const source = sourcePath(String(entry.source));
    const [built, nested] = build_sentou_resource(source);
    const sources = [String(entry.index), ...nested.map((name) => relative(ROOT, resolve(name)))];
    sources.forEach(sourcePath);
    return [built, [...new Set(sources)], { source_bytes: built.length }];
  }
  if (kind === "golden-sun-kind2-resource") {
    const source = sourcePath(String(entry.source));
    const resource = build_kind2_resource(source);
    const sources = [String(entry.index), ...resource.sources.map((name) => relative(ROOT, resolve(name)))];
    sources.forEach(sourcePath);
    return [resource.data, [...new Set(sources)], {
      resource_id: `0x${resource.id.toString(16).padStart(3, "0")}`,
      source_bytes: resource.data.length,
    }];
  }
  if (kind === "golden-sun-tokushu-map") {
    const indexName = String(entry.source);
    const id = number(entry.resource_id);
    const resource = tokushuMaps(indexName).find((item) => item.id === id);
    if (resource === undefined || resource.address !== number(entry.address) ||
        resource.data.length !== number(entry.size)) {
      throw new Error("special-map resource differs from manifest series");
    }
    const nested = resource.sources.map((name) => relative(ROOT, resolve(name)));
    nested.forEach(sourcePath);
    return [resource.data, [...new Set([indexName, ...nested])], {
      resource_id: `0x${id.toString(16).padStart(3, "0")}`,
      source_bytes: resource.data.length,
    }];
  }
  if (kind === "golden-sun-chiiki-map") {
    const indexName = String(entry.source);
    const id = number(entry.resource_id);
    const resource = chiikiMaps(indexName).find((item) => item.id === id);
    if (resource === undefined || resource.address !== number(entry.address) ||
        resource.data.length !== number(entry.size)) {
      throw new Error("regional-map resource differs from manifest series");
    }
    const nested = resource.sources.map((name) => relative(ROOT, resolve(name)));
    nested.forEach(sourcePath);
    return [resource.data, [...new Set([indexName, ...nested])], {
      resource_id: `0x${id.toString(16).padStart(3, "0")}`,
      source_bytes: resource.data.length,
    }];
  }
  if (kind === "golden-sun-kana-glyph-bank") {
    const sourceName = String(entry.source);
    const source = sourcePath(sourceName);
    const [built, sources] = build_resource_01c(source);
    if (number(entry.address) !== RESOURCE_01C_ADDRESS || number(entry.size) !== RESOURCE_01C_SIZE ||
        built.length !== RESOURCE_01C_SIZE) {
      throw new Error("kana glyph bank differs from canonical manifest extent");
    }
    const nested = sources.map((name) => relative(ROOT, resolve(name)));
    nested.forEach(sourcePath);
    return [built, [...new Set([sourceName, ...nested])], {
      glyphs: 36,
      source_bytes: built.length,
    }];
  }
  if (kind === "golden-sun-music-residual") {
    const indexName = String(entry.source);
    const address = number(entry.address);
    const region = musicResiduals(indexName).find((item) => item.address === address);
    if (region === undefined || region.data.length !== number(entry.size)) {
      throw new Error("music residual differs from manifest series");
    }
    const nested = region.sources.map((name) => relative(ROOT, resolve(name)));
    nested.forEach(sourcePath);
    return [region.data, [...new Set([indexName, ...nested])], {
      source_bytes: region.data.length,
    }];
  }
  if (kind === "golden-sun-audio-engine-data") {
    const sourceName = String(entry.source);
    const built = build_audio_engine_data(sourcePath(sourceName));
    if (number(entry.address) !== AUDIO_ENGINE_ADDRESS || number(entry.size) !== AUDIO_ENGINE_SIZE ||
        built.address !== AUDIO_ENGINE_ADDRESS || built.data.length !== AUDIO_ENGINE_SIZE) {
      throw new Error("audio-engine data differs from canonical manifest extent");
    }
    const nested = built.sources.map((name) => relative(ROOT, resolve(name)));
    nested.forEach(sourcePath);
    return [built.data, [...new Set(nested)], {
      source_bytes: built.data.length,
      tone_records: 225,
      waveforms: 18,
      players: 8,
      derived_alignment_bytes: 2,
    }];
  }
  if (kind === "golden-sun-d1-d3-resource") {
    const indexName = String(entry.source);
    const id = number(entry.resource_id);
    const resource = resourcesD1D3(indexName).find((item) => item.id === id);
    if (resource === undefined || resource.address !== number(entry.address) ||
        resource.data.length !== number(entry.size)) {
      throw new Error("D1-D3 resource differs from canonical manifest extent");
    }
    const nested = resource.sources.map((name) => relative(ROOT, resolve(name)));
    nested.forEach(sourcePath);
    return [resource.data, [...new Set([indexName, ...nested])], {
      resource_id: `0x${id.toString(16).padStart(3, "0")}`,
      source_bytes: resource.data.length,
      boundary_bytes: resource.boundarySize,
      suffix_fallback: resource.boundarySize - resource.data.length,
    }];
  }
  if (kind === "golden-sun-final-battle-overlay") {
    const source = String(entry.source);
    const built = resource3ce(source);
    const component = String(entry.component);
    const address = component === "stream" ? built.stream_address : built.fill_address;
    const data = component === "stream" ? built.stream : built.fill;
    if (!["stream", "fill"].includes(component) || address !== number(entry.address) ||
        data.length !== number(entry.size)) {
      throw new Error("final battle overlay component differs from manifest series");
    }
    const directory = dirname(source);
    const nested = [source, join(directory, "overlay.s"), join(directory, "stream.lz.json")];
    nested.forEach(sourcePath);
    return [data, nested, {
      component,
      source_bytes: data.length,
      ...(component === "stream" ? { fallback_bytes: built.fallback_size } : {}),
    }];
  }
  if (kind === "golden-sun-encounter-data") {
    const source = sourcePath(String(entry.source));
    const address = number(entry.address);
    const region = build_encounter_regions(dirname(source)).find((item) =>
      item.address === address && item.source === basename(source));
    if (region === undefined || region.size !== number(entry.size))
      throw new Error("encounter-data region differs from manifest");
    return [region.data, [String(entry.source)], { source_bytes: region.data.length }];
  }
  if (kind === "golden-sun-namae-nyuuryoku") {
    const source = sourcePath(String(entry.source));
    const document = JSON.parse(readFileSync(source, "utf8"));
    if (number(document.address) !== number(entry.address) ||
        number(document.end) - number(document.address) !== number(entry.size))
      throw new Error("name-entry extent differs from manifest");
    const built = build_namae_nyuuryoku(document);
    return [built, [String(entry.source)], {
      resource_ids: document.resource_ids.length,
      tilemap_entries: document.tilemap.tiles.reduce((sum: number, row: Json[]) => sum + row.length, 0),
    }];
  }
  if (kind === "golden-sun-gameplay-databases") {
    const source = sourcePath(String(entry.source));
    const document = JSON.parse(readFileSync(source, "utf8"));
    if (number(document.address) !== number(entry.address) || number(document.size) !== number(entry.size)) {
      throw new Error("gameplay-database extent differs from manifest");
    }
    const built = build_resource_5(document);
    return [built, [String(entry.source)], {
      items: document.items.length,
      abilities: document.abilities.length,
      combatants: document.combatants.length,
      classes: document.classes.length,
      djinn: document.djinn.length,
      alignment_bytes: document.alignment_bytes,
    }];
  }
  if (kind === "golden-sun-simple-resource") {
    const id = number(entry.resource_id);
    const sourceLists: Record<number, string[]> = {
      2: ["assets/data/resource_2/build_stamp.txt", "assets/data/resource_2/layout.json"],
      0x13: ["assets/graphics/resource_13/font.4bpp.png"],
      0x14: ["assets/graphics/resource_14/words.rgba.png"],
      0x18: ["assets/graphics/resource_18/screen.8bpp.png", "assets/graphics/resource_18/screen.lz.json"],
    };
    const sources = sourceLists[id];
    if (sources === undefined) throw new Error("unsupported simple resource");
    sources.forEach(sourcePath);
    const built = build_simple_resource(id as 2 | 0x13 | 0x14 | 0x18, join(ROOT, "assets"));
    return [built, sources, { resource_id: id }];
  }
  if (kind === "golden-sun-title-lz") {
    const source = sourcePath(String(entry.source));
    const document = JSON.parse(readFileSync(source, "utf8"));
    if (number(document.address) !== number(entry.address)) {
      throw new Error("title-resource address differs from manifest");
    }
    const directory = dirname(String(entry.source));
    const sources = [String(entry.source), ...document.components.map((component: Json) => {
      const relativeSource = join(directory, String(component.source));
      sourcePath(relativeSource);
      return relativeSource;
    })];
    const built = build_title_resource(source);
    return [built, sources, {
      resource_id: number(document.resource_id),
      decoded_size: number(document.decoded_size),
      components: document.components.length,
      fallback_tail: document.tail.policy === "fallback" ? number(document.tail.size) : 0,
    }];
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
  console.log("usage: build_assets.ts [-h] [--source-only] [--manifest MANIFEST] [-o OUTPUT] [rom] | --self-test");
}

function parseArgs(argv: string[]): Options {
  const options: Options = {
    rom: "gs1-en.gba", manifest: join(ROOT, "assets/manifest.json"), output: join(ROOT, "out/assets"),
    sourceOnly: false,
  };
  let positional = false;
  for (let index = 0; index < argv.length; index++) {
    const argument = argv[index];
    if (argument === "-h" || argument === "--help") {
      usage();
      process.exit(0);
    } else if (argument === "--source-only") {
      options.sourceOnly = true;
      continue;
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
  if (options.sourceOnly && positional) throw new Error("--source-only does not accept a ROM");
  return options;
}

function main(): void {
  if (Bun.argv.length === 3 && Bun.argv[2] === "--self-test") {
    closurePackageSelfTest();
    return;
  }
  const args = parseArgs(Bun.argv.slice(2));
  const rom = args.sourceOnly ? null : readFileSync(resolve(process.cwd(), args.rom));
  const romSize = rom?.length ?? ROM_SIZE;
  const manifestPath = isAbsolute(args.manifest) ? args.manifest : resolve(process.cwd(), args.manifest);
  const manifest = JSON.parse(readFileSync(manifestPath, "utf8"));
  if (manifest.format !== 1) throw new Error("unsupported asset manifest format");
  const output = isAbsolute(args.output) ? args.output : resolve(ROOT, args.output);
  mkdirSync(output, { recursive: true });
  const entries: Json[] = [...(manifest.regions ?? [])];
  expandClosurePackages(manifest, entries);
  expandSeries(manifest, entries);
  for (const packageEntry of [...entries]) {
    if (packageEntry.kind !== "golden-sun-byte-value-region-package") continue;
    const source = String(packageEntry.source), document = JSON.parse(readFileSync(sourcePath(source), "utf8"));
    if (document.kind !== "golden-sun-byte-value-regions" || !Array.isArray(document.regions)) {
      throw new Error("byte-value package source differs");
    }
    const index = entries.indexOf(packageEntry); entries.splice(index, 1, ...document.regions.map((region: Json) => ({
      address: region.address, size: Array.isArray(region.values) ? region.values.length : 0,
      kind: "golden-sun-byte-value-regions", source,
    })));
  }
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
    if (!(ROM_BASE <= address && address < previousEnd && previousEnd <= ROM_BASE + romSize)) {
      throw new Error(`asset region outside ROM at 0x${address.toString(16).padStart(8, "0")}`);
    }
    const [builtData, builtSources, report] = buildEntry(entry);
    const sources = closureSources(entry, builtSources);
    if (builtData.length !== size) {
      throw new Error(
        `asset at 0x${address.toString(16).padStart(8, "0")}: ` +
        `built 0x${builtData.length.toString(16)}, expected 0x${size.toString(16)}`,
      );
    }
    if (rom !== null) {
      const expected = rom.subarray(address - ROM_BASE, address - ROM_BASE + size);
      if (!builtData.equals(expected)) {
        throw new Error(`asset at 0x${address.toString(16).padStart(8, "0")}: encoded bytes differ`);
      }
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
    rom_size: romSize,
    verification: args.sourceOnly ? "source_only" : "rom",
    asset_bytes: assetBytes,
    regions,
  }, null, 2) + "\n");
  console.log(`assets=${regions.length} bytes=${assetBytes}`);
}

if (import.meta.main) main();

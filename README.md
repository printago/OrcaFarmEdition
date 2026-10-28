<div align="center">

<img alt="Orca Farm Edition" src="resources/images/OrcaSlicer_192px.png" width="120">

# Orca Farm Edition

**[OrcaSlicer](https://github.com/OrcaSlicer/OrcaSlicer) with the [Printago](https://printago.io) print-farm integration built in.**

[![Build all](https://img.shields.io/github/actions/workflow/status/printago/OrcaFarmEdition/build_all.yml?branch=main&label=Build%20all)](https://github.com/printago/OrcaFarmEdition/actions/workflows/build_all.yml)
[![Nightly](https://img.shields.io/badge/download-nightly-1262c9)](https://github.com/printago/OrcaFarmEdition/releases/tag/nightly)
[![License: AGPL-3.0](https://img.shields.io/badge/license-AGPL--3.0-blue)](LICENSE.txt)

</div>

Orca Farm Edition is a downstream fork of OrcaSlicer maintained by Printago. It tracks upstream
`main` closely and adds one thing: an embedded, opt-out Printago tab plus a host/page bridge that
lets a farm service drive **Send to farm**, **Edit in slicer → Replace**, and **Preview sliced
G-code** round trips. The same integration is proposed upstream as
[OrcaSlicer/OrcaSlicer#15126](https://github.com/OrcaSlicer/OrcaSlicer/pull/15126); this fork
exists so farms can use it today, and so we can keep it current against upstream while the PR is reviewed.

Everything else is OrcaSlicer: slicing engine, profiles, calibration, printer support. Slicing
output is unchanged. If you don't use Printago, you want stock OrcaSlicer.

## Download

- **Nightly builds** (every push to `main`, all platforms):
  <https://github.com/printago/OrcaFarmEdition/releases/tag/nightly>
- **Tagged releases**: <https://github.com/printago/OrcaFarmEdition/releases>

Windows (x64, arm64, installer and portable), macOS (universal DMG), Linux (AppImage x86_64 and
aarch64, Flatpak bundles).

Builds are not yet code-signed. On macOS, right-click → Open the first time, or run
`xattr -dr com.apple.quarantine "/Applications/OrcaSlicer.app"`. On Windows, use "More info → Run anyway".

## What differs from OrcaSlicer

| Area | Farm Edition |
|---|---|
| Printago integration | Included and **on by default** (Preferences → Printago to turn it off). Adds a Printago tab, Send / Send & Queue, Replace-on-save for cloud parts, and G-code preview from the farm. |
| Branding | Printago icon, splash and About artwork; app name "Orca Farm Edition". |
| Install side by side | Own config directory (`OrcaFarmEdition` instead of `OrcaSlicer`), own macOS bundle id (`io.printago.OrcaFarmEdition`), own Windows install dir and registry key. Stock OrcaSlicer is untouched. |
| Update check | Points at this repository's releases instead of OrcaSlicer's. |
| G-code / 3MF | Identical to upstream. Headers and metadata still identify as `OrcaSlicer`, so printers and firmware that check the producer keep working. |

Profiles, the profile updater and the network plugin are upstream's.

## Building

Same as upstream. See the [OrcaSlicer build docs](https://github.com/OrcaSlicer/OrcaSlicer/wiki/How-to-build)
and `build_linux.sh`, `build_win.bat`, `build_release_macos.sh`. Note: the dependency build does not
tolerate spaces in the checkout path.

## Keeping up with upstream

```sh
git remote add upstream https://github.com/OrcaSlicer/OrcaSlicer.git
git fetch upstream
git merge upstream/main
```

The fork keeps upstream file names (icons are replaced in place, build scripts and workflows are
untouched apart from two path fixes), so merges are usually conflict-free. The fork-specific
pieces are:

- `src/slic3r/GUI/Printago*.{cpp,hpp}`, `src/slic3r/Utils/Printago.{cpp,hpp}` and the wiring in
  `GUI_App`, `MainFrame`, `Plater`, `Preferences`, `PrinterWebView`, `Widgets/WebView`.
- `version.inc` → `SLIC3R_APP_DATA_DIR_NAME`, `src/libslic3r/libslic3r.h` → `SLIC3R_APP_FULL_NAME`,
  `src/libslic3r/AppConfig.cpp` → `enable_printago` default and `VERSION_CHECK_URL`.
- Bundle / installer identity in `CMakeLists.txt`, `src/CMakeLists.txt`, `Info.plist.in`,
  `OrcaSlicer.rc.in`, the `.desktop` file.
- `resources/images/*` brand assets and `resources/Icon.icns`.
- `.github/workflows/farm_edition_release.yml` (nightly and tagged releases).

## License

Orca Farm Edition is licensed under the **GNU Affero General Public License, version 3**, exactly
like OrcaSlicer, which is in turn based on Bambu Studio, PrusaSlicer and Slic3r. See
[LICENSE.txt](LICENSE.txt). The Printago name and logo are trademarks of Printago and are not
covered by the AGPL.

OrcaSlicer is copyright its contributors; see the upstream repository for the full history and
[the upstream README](https://github.com/OrcaSlicer/OrcaSlicer#readme) for its documentation,
community links and credits.

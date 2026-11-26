# Medieval Conquest Modernization Patch

An unofficial quality-of-life patch for the 2004 Windows game Medieval Conquest. It improves the interface and controls for modern displays while preserving the original game.

## Features

- Widescreen field-of-view and aspect-ratio correction (AlphaYellow’s bundled FOV Fix).
- Resolution-aware UI scaling and sharper fonts
- Customizable hotkeys, better keyboard camera movement, middle-mouse dragging and mouse-wheel zoom.
- Four camera bookmarks, each customizable in-game
- Current research indicator and research queuing
- `givemoney` cheat (+10,000 gold after enabling cheats with `iamacheater`)

## Installation

1. Back up your game and saves. A compatible 32-bit `MC.exe` is required; the patch checks native hook signatures before installing.
2. Set the game's resolution separately using [Tycoon Widescreen Patch](https://github.com/Mykola1453/Tycoon-Widescreen-Patch) by Mykola1453. This release does not include or integrate that tool.
3. Install a compatible 32-bit ASI loader, such as [Ultimate ASI Loader](https://github.com/ThirteenAG/Ultimate-ASI-Loader/releases) by ThirteenAG. Use the proxy DLL name that works with your game installation; do not overwrite an existing game DLL without backing it up. If an ASI loader is already installed, reuse it. The `d3d8.dll` proxy worked for the author.
4. Extract the player release beside `MC.exe`. Both ASIs and both INIs belong in the game directory. Remove older prototype ASIs and any duplicate FOV Fix first.
5. In `MedievalConquestFOVFix.ini`, set Width and Height to the resolution selected in step 2 (or leave both at 0 if it matches your desktop). The FOV fix does not set the game resolution. Launch the game. Edit the INIs with the game closed.

2560×1440 is the tested resolution (150% default scaling). 1920×1080 and 3840×2160 presets are experimental. Other resolutions are not supported. Compatibility with other executable revisions or combined executable mods is not guaranteed.

## Configuration

See [MC_Modernization.ini](MC_Modernization.ini) for hotkeys and other settings. Bookmarks last for the current level/session; the HUD Home button recalls slot 1.

## Research queues

Left-click another upgrade while research is active to queue it. Numbered blue badges show queue order; a gold dot marks active research. Click a queued upgrade again to remove it and refund its reservation. Right-click the active upgrade icon to cancel and refund it; the next queued upgrade starts automatically.

## Known limitations

Some native dialogs and text remain unscaled; high-resolution text can fall back to native raster scaling. The exact release package, including simultaneous operation of both ASIs, needs a final Windows smoke test. Report problems with resolution, executable version and `MC_Modernization.log`.

## Source

See [BUILD.md](BUILD.md) for compilation and optional executable compatibility checks. No original game files, resolution-patcher code or ASI loader binaries are distributed here. The FOV Fix is bundled unchanged as a separate ASI. Medieval Conquest belongs to its respective rights holders. This is an unofficial project.

## Credits and license

- Modernization Patch: MIT, see [LICENSE](LICENSE).
- [AlphaYellow](https://github.com/alphayellow1): bundled [Medieval Conquest FOV Fix](https://github.com/alphayellow1/AlphaYellowWidescreenFixes/releases/tag/medievalconquest), copyright (c) 2024 AlphaYellow, MIT; see [LICENSE-AlphaYellow.txt](LICENSE-AlphaYellow.txt). [Upstream source](https://github.com/alphayellow1/AlphaYellowWidescreenFixes/blob/main/source/fixes/MedievalConquestFOVFix/dllmain.cpp).
- Mykola1453: separate Tycoon Widescreen Patch. ThirteenAG: separate Ultimate ASI Loader.

See [THIRD-PARTY.md](THIRD-PARTY.md) for binary provenance and dependency license review.

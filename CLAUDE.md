# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

OP-Z-Cube is an Arduino-based hardware project that creates interactive LED lighting controlled by a Teenage Engineering OP-Z synthesizer via DMX protocol. It enables users to map individual LEDs to OP-Z's 16 DMX channels through custom configuration arrays.

## Build & Upload

This is an Arduino project - no npm/make build system. Use Arduino IDE:

1. Install required libraries via `Sketch > Include Library > Manage Libraries`:
   - **DMXUSB** (https://github.com/DaAwesomeP/dmxusb/)
   - **elapsedMillis** (DMXUSB dependency)
   - **FastLED** (https://github.com/FastLED/FastLED)

2. Open `200516_OPZ_cube.ino` in Arduino IDE
3. Select `Tools > Board > Arduino Nano`
4. Select `Tools > Port > (your Arduino's serial port)`
5. Click Upload

**Hardware note**: Arduino Nano must use FTDI FT232 USB chip (not CH340) for OP-Z compatibility.

## Code Architecture

### Main Sketch: `200516_OPZ_cube.ino`

DMX-controlled LED mapping for OP-Z:

- **Lines 13-15**: Configuration constants (`DATA_PIN`, `BRIGHTNESS`, `MAXLED`)
- **Lines 19-36**: `map_array[16][15]` - The core LED mapping table stored in PROGMEM. Each row corresponds to one OP-Z channel (0-15), containing LED IDs to illuminate for that channel. LED IDs are 1-indexed (0 means empty slot).
- **Lines 48-60**: `showLEDs()` callback - Processes incoming DMX data, iterates through channels and mapping table, writes RGB values to LED array. Note: Color order is GRB (buffer indices: G=index*3, R=index*3+1, B=index*3+2).
- **Lines 62-72**: DMXUSB initialization with callback registration

### Alternative Sketch: `Prismatik`

Adalight/Boblight-compatible firmware for PC-based ambient lighting (Prismatik software). Uses "Ada" magic word protocol for synchronization. Not OP-Z related.

## Key Configuration

When customizing for different LED arrangements, modify:

```cpp
#define DATA_PIN 3      // Arduino GPIO pin for LED data
#define BRIGHTNESS 164  // 0-255
#define MAXLED 15       // Max LEDs in any single channel row

const byte map_array[16][15] PROGMEM = {
  // Each row = one OP-Z channel, values = LED IDs (1-indexed)
  // Empty positions use 0, which are skipped in showLEDs()
};
```

The `map_array` allows non-sequential LED assignments (e.g., diagonal patterns across a cube).

## Testing

No automated tests. Validation is manual: compile, upload to Arduino, connect to OP-Z, observe LED behavior.

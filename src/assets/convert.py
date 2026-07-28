from PIL import Image
from pathlib import Path

INPUT = "ShadowTrainLogo.png"
OUTPUT = "ShadowTrainLogo.h"

img = Image.open(INPUT).convert("RGBA")

width, height = img.size
pixels = img.load()

with open(OUTPUT, "w") as f:
    f.write("#pragma once\n\n")
    f.write("#include <stdint.h>\n\n")

    f.write(f"constexpr int ShadowTrainLogoWidth = {width};\n")
    f.write(f"constexpr int ShadowTrainLogoHeight = {height};\n\n")

    f.write("const uint16_t ShadowTrainLogo[] = {\n")

    count = 0

    for y in range(height):
        for x in range(width):

            r, g, b, a = pixels[x, y]

            # Transparent pixels become black for now
            if a == 0:
                rgb565 = 0x0000
            else:
                rgb565 = (
                    ((r & 0xF8) << 8) |
                    ((g & 0xFC) << 3) |
                    (b >> 3)
                )

            f.write(f"0x{rgb565:04X},")

            count += 1
            if count % 12 == 0:
                f.write("\n")

    f.write("\n};\n")

print(f"Wrote {OUTPUT}")
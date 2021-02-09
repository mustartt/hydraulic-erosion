from PIL import Image

img = Image.new('RGB', (128, 128), color='red')

with open('output.txt', 'r') as file:
    for y in range(128):
        lines = file.readline().split()
        for x in range(128):
            val = int(lines[x])
            img.putpixel((x, y), (val, val, val))

img.save("output.png", "PNG")

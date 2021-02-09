from PIL import Image

img = Image.open('Heightmap_2.png')
grayimg = img.convert('L')

with open('input.txt', 'w') as file:
    for y in range(img.height):
        for x in range(img.width):
            val = grayimg.getpixel((x,y))
            file.write(f'{val} ')
        file.write('\n')

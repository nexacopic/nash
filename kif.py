import sys
from PIL import Image

def save_custom_image_from_jpg(input_image_path, output_text_file):
    with Image.open(input_image_path) as img:
        width, height = img.size
        pixel_data = list(img.getdata())

        with open(output_text_file, 'w') as file:
            file.write(f"{width} {height}\n")
            for pixel in pixel_data:
                file.write(f"{pixel[0]} {pixel[1]} {pixel[2]}\n")

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: python main.py <input file> <output file>")
    else:
        input_image_path = sys.argv[1]
        output_text_file = sys.argv[2]
        save_custom_image_from_jpg(input_image_path, output_text_file)
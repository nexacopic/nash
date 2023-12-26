import random

def save_custom_image(filename, width, height):
    with open(filename, 'w') as file:
        file.write(f"{width} {height}\n")
        for _ in range(height):
            row = []
            for _ in range(width):
                r = random.randint(0, 255)
                g = random.randint(0, 255)
                b = random.randint(0, 255)
                file.write(f"{r} {g} {b}\n")

# Example usage:
width = 100
height = 100

save_custom_image('test.kif', width, height)

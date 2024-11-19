import random

def genInt():
    return random.randint(1,1000001)

with open("input.txt", "w") as f:
    for i in range(100000):
        f.write(str(genInt()) + '\n')

    f.write("0")

print("Todo piola")

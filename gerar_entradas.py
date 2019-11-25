import random

for i in range(10):

    mat = []

    for j in range(1000):
        num = random.randint(100000, 999999)
        while num in mat:
            num = random.randint(100000, 999999)
        mat.append(num)

    arq = open(f'entrada{i+1}.txt', 'a')

    for i in mat:
        arq.write(f'{i}\n')


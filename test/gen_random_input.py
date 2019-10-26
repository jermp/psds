import random, sys

n = int(sys.argv[1])
print n
for i in range(0, n):
    print random.randint(-100, +100)
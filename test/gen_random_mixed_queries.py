import random, sys
from random import shuffle

n = int(sys.argv[1])
print n

queries = []

# sum
for i in range(0, n / 2):
    index = random.randint(0, 255)
    queries.append([0, index])

# update
vals = [-1, +1]
for i in range(0, n / 2):
    index = random.randint(0, 255)
    v = random.randint(0, 1)
    queries.append([1, index, vals[v]])

shuffle(queries)

for q in queries:
    print " ".join([str(x) for x in q])
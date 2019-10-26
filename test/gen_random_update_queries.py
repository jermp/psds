import random, sys

num_queries = int(sys.argv[1])
array_size = int(sys.argv[2])

print num_queries
vals = [-1, +1]
for i in range(0, num_queries):
    index = random.randint(0, array_size - 1)
    v = random.randint(0, 1)
    print index, vals[v]
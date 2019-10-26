import random, sys

num_queries = int(sys.argv[1])
array_size = int(sys.argv[2])
lower_bound = int(sys.argv[3])

print num_queries
for i in range(0, num_queries):
    print random.randint(lower_bound, array_size - 1)
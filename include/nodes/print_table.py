import sys

n = int(sys.argv[1])

# T_j
for row in range(n):
    for col in range(row + 1):
        print("0,", end='')
    for col in range(n - row - 1):
        print("all1,", end='')
    print("")

print("")

# T_k
for row in range(n):
    for col in range(row):
        print("0,", end='')
    for col in range(n - row):
        print("all1,", end='')
    print("")
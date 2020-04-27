import sys

n = int(sys.argv[1])
for row in range(n+1):
    for col in range(row):
        print("0,", end='')
    for col in range(n - row):
        print("all1,", end='')
    print("")
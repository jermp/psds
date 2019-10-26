import sys, os

sum_results_filename = sys.argv[1]
update_results_filename = sys.argv[2]

types = ["stv", "ft", "tree_epi32s", "tree_epi32u", "tree64"]
exes = [
"perf_sum",
"perf_update"
]

for t in types:
    for i in range(0, 5):
        os.system("./" + exes[0] + " " + t + " 2>> " + sum_results_filename)

for t in types:
    for i in range(0, 5):
        os.system("./" + exes[1] + " " + t + " 2>> " + update_results_filename)
#!/bin/python

import sys, os, argparse

parser = argparse.ArgumentParser()
parser.add_argument('sum_results', type = str, help = "Results filename for SUM.")
parser.add_argument('update_results', type = str, help = "Results filename for UPDATE.")

args = parser.parse_args()

types = ["st", "ft", "tree256u", "tree256s", "tree64u", "tree64s"]

def run(exe, results):
    for type in types:
        for i in range(0, 1):
            os.system("./" + exe + " " + type + " 2>> " + results)

run("perf_sum", args.sum_results)
run("perf_update", args.update_results)
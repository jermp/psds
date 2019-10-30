#!/bin/python

import sys, os, argparse

parser = argparse.ArgumentParser()
parser.add_argument('sum_results', type = str, help = "Results filename for SUM.")
parser.add_argument('update_results', type = str, help = "Results filename for UPDATE.")
args = parser.parse_args()

types = ["st", "ft",
"tree256u", "tft256u", "bft256u",
"tree64u", "tft64u", "bft64u",
"tree256s", "tft256s", "bft256s",
"tree64s", "tft64s", "bft64s"]

run = 1
def run(exe, results):
    for type in types:
        for i in range(0, runs):
            os.system("./" + exe + " " + type + " 2>> " + results)

run("perf_sum", args.sum_results)
run("perf_update", args.update_results)
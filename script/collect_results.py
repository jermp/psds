#!/bin/python

import sys, os, argparse

parser = argparse.ArgumentParser()
parser.add_argument('directory', type = str, help = "Directory with executables. Path must be relative to project parent directory.")
parser.add_argument('sum_results', type = str, help = "Results filename for SUM.")
parser.add_argument('update_results', type = str, help = "Results filename for UPDATE.")
args = parser.parse_args()

types = [

    # "st",

    # "sts_64",
    # "sts_256",
    # "sts_64_restricted",
    # "sts_256_restricted",

    # "ft",

    "ftb_64",
    "ftb_256",
    "ftb_64_restricted",
    "ftb_256_restricted",

    "ftt_64",
    "ftt_256",
    "ftt_64_restricted",
    "ftt_256_restricted"
]

def run(operation, results):
    for type in types:
        os.system("../" + args.directory + "/perf " + type + " " + operation +  " 2>> " + results)

run("sum", args.sum_results)
run("update", args.update_results)
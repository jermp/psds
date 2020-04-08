#!/bin/python

import sys, os, argparse

parser = argparse.ArgumentParser()
parser.add_argument('directory', type = str, help = "Directory with executables. Path must be relative to project parent directory.")
parser.add_argument('sum_results', type = str, help = "Results filename for SUM.")
parser.add_argument('update_results', type = str, help = "Results filename for UPDATE.")
args = parser.parse_args()

types = [

    # "st",

    # "sts_64u",
    # "sts_256u",
    # "sts_64u_restricted",
    # "sts_256u_restricted",

    # "ft",

    "ftb_64u",
    "ftb_256u",
    "ftb_64u_restricted",
    "ftb_256u_restricted",

    "ftt_64u",
    "ftt_256u",
    "ftt_64u_restricted",
    "ftt_256u_restricted"
]

def run(operation, results):
    for type in types:
        os.system("../" + args.directory + "/perf " + type + " " + operation +  " 2>> " + results)

run("sum", args.sum_results)
run("update", args.update_results)
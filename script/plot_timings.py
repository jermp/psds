#!/bin/python

import sys, json, argparse
import numpy as np
import matplotlib.pyplot as plt
plt.style.use('ggplot')
plt.rcParams['axes.facecolor'] = '#f7f7f7'

from matplotlib.backends.backend_pdf import PdfPages
from matplotlib.ticker import FuncFormatter

fig, ax = plt.subplots(figsize = (13,7))
plt.margins(0.01, 0.01)

ax.set_ylabel('ns/query', fontsize = 10)
ax.set_xlabel('n', fontsize = 10)
ax.set_xscale('log', basex = 2)
ax.set_yscale('log', basey = 2)

parser = argparse.ArgumentParser()
parser.add_argument('input_filename', type = str, help = "Input json filename.")
parser.add_argument('output_filename', type = str, help = "Output filename.")
parser.add_argument('min_log2', type = int, help = "Minimum value of log2(n). Must be at least 8.")
parser.add_argument('types', metavar='types', type = str, nargs='+',
                    help = 'types to plot')

args = parser.parse_args()

if args.min_log2 < 8:
    print("min_log2 must be at least 8")
    exit(1)

print(args.types)

y = {}

offset = args.min_log2 - 8
with open(args.input_filename) as f:
    for line in f:
        parsed_line = json.loads(line)
        type = parsed_line["type"]
        if type in args.types:
            y[type] = parsed_line["timings"][offset:]

x = []
n = pow(2, args.min_log2)
for i in range(offset, len(y[args.types[0]]) + offset):
    x.append(n)
    n *= 2

m = 5

info = {
    "segment_tree":                     ["#252525",'o'],

    "segment_tree_simd_node16u":        ["#a50f15",'X'],
    "segment_tree_simd_node64u":        ["#de2d26",'s'],
    "segment_tree_simd_node256u":       ["#fb6a4a",'d'],

    "fenwick_tree":                     ["#252525",'o'],

    "fenwick_tree_blocked_node16u":     ["#08519c",'X'],
    "fenwick_tree_blocked_node64u":     ["#3182bd",'s'],
    "fenwick_tree_blocked_node256u":    ["#6baed6",'d'],

    "fenwick_tree_truncated_node16u":   ["#006d2c",'X'],
    "fenwick_tree_truncated_node64u":   ["#31a354",'s'],
    "fenwick_tree_truncated_node256u":  ["#74c476",'d']
}


gray = '#696969'


lines = []
for i in range(0, len(y)):
    lines.append(ax.plot(x, y[args.types[i]], zorder = 1, color = info[args.types[i]][0], alpha = 1, linewidth = 1.5, marker = info[args.types[i]][1], markersize = m))

ax.legend(
    ([args.types[i] for i in range(0,len(args.types))]),
      fontsize = 9,
      loc = "best",
      borderaxespad = 0.)

pp = PdfPages(args.output_filename + '.pdf')
pp.savefig(bbox_inches = 'tight')
pp.close()

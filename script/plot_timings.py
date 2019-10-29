#!/bin/python

import sys, json, argparse
import numpy as np
import matplotlib.pyplot as plt
plt.style.use('ggplot')
plt.rcParams['axes.facecolor'] = '#f7f7f7'

from matplotlib.backends.backend_pdf import PdfPages
from matplotlib.ticker import FuncFormatter

fig, ax = plt.subplots(figsize = (15,10))
plt.margins(0.01, 0.01)

ax.set_ylabel('ns/query', fontsize = 10)
ax.set_xlabel('n', fontsize = 10)
ax.set_xscale('log', basex = 2)
ax.set_yscale('log', basey = 2)

parser = argparse.ArgumentParser()
parser.add_argument('input_filename', type = str, help = "Results filename.")
parser.add_argument('output_filename', type = str, help = "Output filename.")
parser.add_argument('min_log2', type = int, help = "Minimum value of log2(n). Must be at least 8.")
args = parser.parse_args()

if args.min_log2 < 8:
  print("min_log2 must be at least 8")
  exit(1)

y = {}
types = []
offset = args.min_log2 - 8
with open(args.input_filename) as f:
    for line in f:
        parsed_line = json.loads(line)
        type = parsed_line["type"]
        types.append(type)
        y[type] = parsed_line["timings"][offset:]

x = []
n = pow(2, args.min_log2)
for i in range(offset, len(y[types[0]]) + offset):
    x.append(n)
    n *= 2

m = 5
colors = ['#9e0142','#d53e4f','#f46d43','#fdae61','#fee08b','#e6f598','#abdda4','#66c2a5','#3288bd','#5e4fa2'] #['#d53e4f','#fc8d59','#fee08b','#e6f598','#99d594','#3288bd'] #['#ccece6','#99d8c9','#66c2a4','#2ca25f','#006d2c'] #['#bae4bc','#7bccc4','#43a2ca','#0868ac']
gray = '#696969'
markers = ['o', 'X', 's', 'd', 'v', '^', 'P', '*', 'D', 'p']

lines = []
for i in range(0,len(y)):
    lines.append(ax.plot(x, y[types[i]], zorder = 1, color = colors[i], alpha = 1, linewidth = 1.5, marker = markers[i], markersize = m))

ax.legend(
    ([types[i] for i in range(0,len(types))]),
      fontsize = 9,
      bbox_to_anchor=(0.005, 0.87),
      loc=3, ncol=2,
      borderaxespad = 0.)

pp = PdfPages(args.output_filename + '.pdf')
pp.savefig(bbox_inches = 'tight')
pp.close()

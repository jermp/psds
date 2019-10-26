import sys, json
import numpy as np
import matplotlib.pyplot as plt
plt.style.use('ggplot')
plt.rcParams['axes.facecolor'] = '#f7f7f7'

from matplotlib.backends.backend_pdf import PdfPages
from matplotlib.ticker import FuncFormatter

fig, ax = plt.subplots(figsize = (10,5))
plt.margins(0.01, 0.01)

ax.set_ylabel('ns/query', fontsize = 10)
ax.set_xlabel('n', fontsize = 10)
ax.set_xscale('log', basex = 2)
ax.set_yscale('log', basey = 2)

input_filename = sys.argv[1]
output_filename = sys.argv[2]

y = {}
types = []
with open(input_filename) as f:
    for line in f:
        parsed_line = json.loads(line)
        type = parsed_line["type"]
        types.append(type)
        y[type] = parsed_line["timings"]

x = []
n = 256
for i in range(0, len(y[types[0]])):
    x.append(n)
    n *= 2

m = 7
colors = ['#d53e4f','#fc8d59','#fee08b','#e6f598','#99d594','#3288bd'] #['#ccece6','#99d8c9','#66c2a4','#2ca25f','#006d2c'] #['#bae4bc','#7bccc4','#43a2ca','#0868ac']
gray = '#696969'
markers = ['o', 'X', 's', 'd', 'v', '^', 'P', '*', 'D', 'p']

lines = []
for i in range(0,len(y)):
    lines.append(ax.plot(x, y[types[i]], zorder = 1, color = colors[i], alpha = 1, linewidth = 1.7, marker = markers[i], markersize = m))

ax.legend(
    ([types[i] for i in range(0,len(types))]),
      fontsize = 9,
      bbox_to_anchor=(0.005, 0.77),
      loc=3, ncol=1,
      borderaxespad = 0.)

pp = PdfPages(output_filename + '.pdf')
pp.savefig(bbox_inches = 'tight')
pp.close()

import networkx
import matplotlib.pyplot as plt
import sys
import struct

data_path = sys.argv[1]
# data_path = "graph.data"
with open(data_path, "rb") as graph_file:
    vertices_count, edges_count = struct.unpack("<iq", graph_file.read(4 + 8))
    g = networkx.Graph()
    data = graph_file.read(3 * 4)
    while data:
        src_id, dst_id, weight = struct.unpack("<iif", data)
        print src_id, dst_id, weight
        g.add_edge(src_id, dst_id, weight=weight)
        data = graph_file.read(3 * 4)

networkx.draw_circular(g, with_labels=True)
networkx.draw_networkx_edge_labels(g, pos=networkx.circular_layout(g))
plt.show()


# with open("../data/tmpfile.weights", "rb") as f:
with open("../data/weights.data", "rb") as f:
    print "-----", f.name
    data = f.read(4)
    i = 0
    while data:
        i += 1
        src_id,  = struct.unpack("<f", data)
        print src_id, "#%s" % i
        data = f.read(4)


# with open("../data/tmpfile", "rb") as f:
with open("../data/edges.data", "rb") as f:
    print "-----", f.name
    data = f.read(2 * 8)
    i = 0
    while data:
        i += 1
        src_id, dst_id = struct.unpack("<qq", data)
        print src_id, dst_id, "#%s" % i
        data = f.read(2 * 8)

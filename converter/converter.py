import sys
import struct


with open(sys.argv[1], "rb") as graph_file:
    print "from file %s" % graph_file.name
    with open(sys.argv[2], "wb") as edges_file:
        print "to edges file %s" % edges_file.name
        with open(sys.argv[3], "wb") as weights_file:
            print "to weights file %s" % weights_file.name
            vertices, edges = struct.unpack("<iq", graph_file.read(4 + 8))
            data = graph_file.read(3 * 4)
            while data:
                src_id, dst_id, weight = struct.unpack("<iif", data)
                # print "edge", src_id, dst_id, weight
                edges_file.write(struct.pack("<qq", src_id, dst_id))
                weights_file.write(struct.pack("<f", weight))
                data = graph_file.read(3 * 4)

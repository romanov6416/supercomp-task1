import os

SCRATCH_PATH = "/mnt/data/users/dm4/vol12/romanov6416_1945/_scratch/"
DATA_PATH = SCRATCH_PATH + "data/"
OUTPUT_PATH = SCRATCH_PATH + "final_result.txt"


def main():
    with open(OUTPUT_PATH, "w") as output_file:
        files = os.listdir(DATA_PATH)
        i = 0
        for file_name in files:
            print "%.3f%% process '%s'" % (i * 100.0 / len(files), file_name)
            i += 1
            name_extension = file_name.split(".")
            if len(name_extension) != 2 or name_extension[1] != "txt":
                continue
            name_parts = name_extension[0].split("_")
            if len(name_parts) != 4:
                continue
            prefix, graph_type, vertex_degree, thread_number = name_parts
            if prefix != "output":
                continue
            # compute average time of algorithm
            sum_time = 0.0
            launch_count = 0
            with open(DATA_PATH + file_name) as f:
                for line in f:
                    words = line.split()
                    if not words or words[0] != "mytime":
                        continue
                    launch_count += 1
                    sum_time += float(words[1])
            out_line = "%s %s %s %s\n" % (
                graph_type, vertex_degree, thread_number, sum_time / launch_count,
            )
            output_file.write(out_line)


if __name__ == '__main__':
    main()

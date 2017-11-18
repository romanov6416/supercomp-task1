import sys
import os
import time
import subprocess


def run_command(cmd):
    print "run command: '%s'" % cmd
    os.system(cmd)


def is_busy():
    cmd = 'squeue | grep $(echo $USER | cut -c1-7)'
    if sys.version >= "2.7":
        p = subprocess.Popen(cmd, stdout=subprocess.PIPE)
        output = p.stdout.read() == ""
    else:
        output = os.popen(cmd).read()
    return output != ""


def compute(graph_type, vertex_degree, thread_number):
    home_path = "/mnt/data/users/dm4/vol12/romanov6416_1945/"
    prefix = home_path + "_scratch/data/"
    postfix = "%s_%s" % (graph_type, vertex_degree)
    graph_file_path = prefix + "graph_%s.data" % postfix
    edges_file_path = prefix + "converted_%s" % postfix
    # weights_file_path = prefix + "weights_%s.data" % postfix
    weights_file_path = edges_file_path + ".weights"
    output_file_path = prefix + "output_%s_%s_%s.txt" % (graph_type, vertex_degree, thread_number)
    if not os.path.exists(graph_file_path):
        # generate graph
        run_command("~/_scratch/generator/generator "
                    "-s %s -e 32 -undirected -weighted -type %s -file %s -t14"
                    % (vertex_degree, graph_type, graph_file_path))
    if not os.path.exists(edges_file_path) or not os.path.exists(weights_file_path):
        # convert graph
        run_command(
            "python ~/_scratch/converter/converter.py %s %s %s"
            % (graph_file_path, edges_file_path, weights_file_path)
        )
    if not os.path.exists(output_file_path) or os.path.getsize(output_file_path) < 10:
        # run algorithm
        # edges_count = os.path.getsize(edges_file_path) / 4
        for i in range(2 if graph_type == "SSCA2" else 1):
            print "run algorithm"
            os.environ.update({
                "REUSEFILE": "1",
                "TMPFILE": edges_file_path,
                "SKIP_BFS": "1",
                "SKIP_VALIDATE": "1",
            })
            run_command(
                "sbatch -n %s -p test -o %s "
                "ompi %s/_scratch/graph500-graph500-3.0.0/src/graph500_my_sssp %s %s"
                # " | grep \"pred\\|mytime\" | awk ' { print $2,$3 } ' > %s"
                # % (thread_number, output_file_path, home_path, vertex_degree, 32, output_file_path)
                % (thread_number, output_file_path, home_path, vertex_degree, 32)
            )
            # wait until output file is generated
            while is_busy():
                time.sleep(1)
    # output_file = open(output_file_path)
    # run_time = float(output_file.readline())


GRAPH_TYPES = ["RMAT", "SSCA2"]
VERTEX_DEGREES = [i for i in range(17, 23)]
THREAD_NUMBERS = [2**i for i in range(8)]
STEPS_COUNT = len(GRAPH_TYPES) * len(VERTEX_DEGREES) * len(THREAD_NUMBERS)


def main():
    i = 0
    for graph_type in GRAPH_TYPES:
        for vertex_degree in VERTEX_DEGREES:
            for thread_number in THREAD_NUMBERS:
                print("[%.3f%%] compute for (%s, %s, %s)"
                       % (float(i) / STEPS_COUNT * 100, graph_type,
                          vertex_degree, thread_number))
                compute(graph_type, vertex_degree, thread_number)
                i += 1


if __name__ == '__main__':
    main()


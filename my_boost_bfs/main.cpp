#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <limits>


struct Edge {
	int a;
	int b;
	float w;

	Edge() {}
	Edge(const int a, const int b, const float w): a(a), b(b), w(w) {}
};



void solve(int vertices_count, std::vector<Edge> edges, const char * result_path) {
	std::cout << "vertices_count " << vertices_count << std::endl;
	std::cout << "edges_count " << edges.size() << std::endl;
	const float INF = std::numeric_limits<float>::max();
	int n = vertices_count;
	int m = (int) edges.size();
	int v = 0;
	std::vector<float> d((size_t) vertices_count);
	std::vector<int> p((size_t) vertices_count, -1);
	// init distance array
	for (int i = 0; i < n; ++i)
		d[i] = INF;
	d[v] = 0.0;
	std::cout << "run algorithm" << std::endl;
	for (;;) {
		bool any = false;
		for (int j=0; j<m; ++j) {
			Edge &e = edges[j];
			if (d[e.a] < INF)
				if (d[e.b] > d[e.a] + e.w) {
					d[e.b] = d[e.a] + e.w;
					p[e.b] = e.a;
					any = true;
				}
			if (d[e.b] < INF)
				if (d[e.a] > d[e.b] + e.w) {
					d[e.a] = d[e.b] + e.w;
					p[e.a] = e.b;
					any = true;
				}
		}
		if (!any)
			break;
	}
	std::cout << "write result to file '" << result_path << "'" << std::endl;
	std::ofstream of(result_path, std::ios::out);
	for (int i = 0; i < n; ++i) {
		of << i << " " << p[i];
		float dist = 0.0;
//		of << " | ";
		int j = i;
		for (; j > 0; j = p[j]) {
			dist += d[j];
//			of << j << " <- " << p[j];
		}
		of << " | " << dist;
		of << std::endl;
	}
	of.close();
}



int main(int argc, char * argv[])
{
	if (argc != 3) {
		std::cout << "Usage: <program> graph_path result_path" << std::endl;
		return 1;
	}
	const char * graph_path = argv[1];
	const char * result_path = argv[2];
	std::ifstream graph_file(graph_path);
	if (not graph_file.is_open()) {
		std::cout << "fail to open file '" << graph_path << "'" << std::endl;
		return 1;
	}
	// read vertices and edges count
	int vertices_count;
	graph_file.read((char*) &vertices_count, sizeof(vertices_count));
	long long int edges_count;
	graph_file.read((char*)&edges_count, sizeof(edges_count));

	// read all edges and weights
	std::vector<Edge> edges((unsigned) edges_count);
	for (int i = 0; i < edges.size() and not graph_file.eof(); ++i) {
		Edge & e = edges[i];
		graph_file.read((char*)&e.a, sizeof(int));
		graph_file.read((char*)&e.b, sizeof(int));
		graph_file.read((char*)&e.w, sizeof(float));
	}
	solve(vertices_count, edges, result_path);
	return EXIT_SUCCESS;
}


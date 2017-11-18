#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/bellman_ford_shortest_paths.hpp>
#include <boost/graph/isomorphism.hpp>
#include <boost/graph/graph_utility.hpp>


//using namespace std;
using namespace boost;

typedef adjacency_list<vecS, vecS, undirectedS, no_property, property<edge_weight_t, float> > Graph;

typedef adjacency_list<vecS, vecS, undirectedS, property<vertex_index_t, int>, no_property> ResGraph;
//typedef adjacency_list<vecS, listS, undirectedS, property<vertex_index_t, int>, no_property> ResGraph;

int main(int argc, char **argv)
{
	if(argc < 3)
	{
		std::cout << "Give a filename with graph and an output filename. Optionally - filename with MPI result." << std::endl;
		return -1;
	}

	std::string file_name(argv[1]);
	std::string outfile(argv[2]);
	// open file
	std::fstream file(file_name, std::ios::in | std::ios::binary);

	int vertices_count = 0;
	long long edges_count = 0;

	// read header
	file.read((char*)(&vertices_count), sizeof(int));
	file.read((char*)(&edges_count), sizeof(long long));

	// print graph
	Graph g(vertices_count);

	// get & print graph data for WEIGHTED graph
	for(long long i = 0; i < edges_count; i++)
	{
		int src_id = 0, dst_id = 0;
		float weight = 0;

		// read i-th edge data
		file.read((char*)(&src_id), sizeof(int));
		file.read((char*)(&dst_id), sizeof(int));
		file.read((char*)(&weight), sizeof(float)); // remove it for unweighed graph

		//print edge data
		add_edge(vertex(src_id, g), vertex(dst_id, g), weight, g);
	}

	file.close();

	std::vector<int> parent(vertices_count);
	std::vector<float> distance(vertices_count, (std::numeric_limits < short >::max)());

	for (int i = 0; i < vertices_count; ++i)
		parent[i] = i;
	distance[0] = 0;

	bool res = bellman_ford_shortest_paths
			(g, vertices_count, weight_map(get(edge_weight, g)).distance_map(&distance[0]).predecessor_map(&parent[0]));

	if(argc == 3)
	{
		std::fstream fout(outfile, std::ios::out);
		for(int i=0; i < vertices_count; i++)
		{
			fout << "pred[" << i << "] = " << parent[i] <<  std::endl;
		}

		fout.close();
	}
	else
	{
		std::fstream fin(std::string(argv[3]), std::ios::in);

		ResGraph mpi_g(vertices_count), boost_g(vertices_count);

		for(int i=0; i<vertices_count; i++)
		{
			std::string s;
			int src_id = 0, dst_id = 0;
			fin >> s;
			sscanf(s.c_str(), "pred[%d] = %d\n", &dst_id, &src_id);

			if(src_id != dst_id)
				add_edge(vertex(src_id, mpi_g), vertex(dst_id, mpi_g), mpi_g);
			if(parent[i] != i)
				add_edge(vertex(parent[i], boost_g), vertex(i, boost_g), boost_g);
		}


		std::vector<graph_traits<ResGraph>::vertex_descriptor> f(vertices_count);
		property_map<ResGraph, vertex_index_t>::type v1_index_map = get(vertex_index, mpi_g);


		bool ret = isomorphism(mpi_g, boost_g, isomorphism_map(make_iterator_property_map(f.begin(), v1_index_map, f[0])));
		if(res)
			std::cout << "Pong" << std::endl;
	}
}

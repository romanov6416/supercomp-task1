//
// Created by odissey on 14.11.17.
//
#include <boost/config.hpp>
#include <iostream>
#include <fstream>

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/bellman_ford_shortest_paths.hpp>
#include <boost/property_map/property_map.hpp>

using namespace boost;

struct EdgeProperties {
    float weight;
};

typedef std::pair < int64_t, int64_t >E;
typedef adjacency_list < vecS, vecS, undirectedS,
no_property, EdgeProperties> Graph;

class EdgesReader{
public:
    int edgeCount;
    int verticeCount;
    E* edge_array;
    int64_t* vertice_array;

    EdgesReader(const char* path_to_edges){
        std::vector<E> edges;
        char* buf = new char[8];
        size_t counter = 0;
        std::ifstream ifs(path_to_edges, std::ios::binary);
        ifs.seekg (0, std::ios::beg);
        std::vector<int64_t> vertices;
        size_t ver_counter = 0;
        while(!ifs.eof()){
            counter++;
            int64_t first, second;
            ifs.read(buf, 8);
            first = *((int64_t*)(buf));
            if(find(vertices.begin(),  vertices.end(), first) == vertices.end()){
                vertices.push_back(first);
                ver_counter++;
            }
            ifs.read(buf, 8);
            second = *((int64_t*)(buf));
            if(find(vertices.begin(),  vertices.end(), second) == vertices.end()){
                vertices.push_back(second);
                ver_counter++;
            }
            edges.push_back(std::make_pair(first, second));
        }
        delete buf;
        edgeCount = counter;
        verticeCount = ver_counter;
        edge_array = new E[edgeCount* sizeof(E)];
        vertice_array = new int64_t[verticeCount* sizeof(int64_t)];
//        size_t

        for (size_t i = 0; i < edges.size(); ++i){
            edge_array[i] = edges[i];
        }

        for (size_t i = 0; i < vertices.size(); ++i){
            vertice_array[i] = vertices[i];
        }

        ifs.close();
    }

    ~EdgesReader(){
        delete edge_array;
        delete vertice_array;
    }
};

class WeightReader{
public:
    float* weights;

    WeightReader(const char* weight_file){
        char* buf;
        std::vector<float> weight;
        std::ifstream ifs(weight_file, std::ios::binary);
        ifs.seekg (0, std::ios::beg);
        buf = new char[4];
        while(!ifs.eof()){
            float first;
            ifs.read(buf, 4);
            first = *((float*)(buf));
            weight.push_back(first);
        }
        delete buf;
        ifs.close();
        weights = new float[weight.size()* sizeof(float)];
        for (size_t i = 0; i < weight.size(); ++i){
            weights[i] = weight[i];
        }
    }

    ~WeightReader(){
        delete weights;
    }
};

template < typename Graph, typename ParentMap >
struct edge_writer
{
    edge_writer(const Graph & g, const ParentMap & p)
            : m_g(g), m_parent(p)
    {
    }

    template < typename Edge >
    void operator() (std::ostream & out, const Edge & e) const
    {
        out << "[label=\"" << get(edge_weight, m_g, e) << "\"";
        typename graph_traits < Graph >::vertex_descriptor
                u = source(e, m_g), v = target(e, m_g);
        if (m_parent[v] == u)
            out << ", color=\"black\"";
        else
            out << ", color=\"grey\"";
        out << "]";
    }
    const Graph & m_g;
    ParentMap m_parent;
};

template < typename Graph, typename Parent >
edge_writer < Graph, Parent >
make_edge_writer(const Graph & g, const Parent & p)
{
    return edge_writer < Graph, Parent > (g, p);
}



int main(int argc, char * argv[]) {
    if (argc != 4) {
        std::cerr << "USAGE: <program> edges weights result" << std::endl;
        return 1;
    }
    std::cout << "read edges" << std::endl;
    EdgesReader er(argv[1]);
    std::cout << "read weights" << std::endl;
    WeightReader wr(argv[2]);

    std::cout<<"wtf";

    int n_edges = er.edgeCount;
    E* edge_array = er.edge_array;
    float* weight = wr.weights;
    int N = er.verticeCount;

    Graph g(edge_array, edge_array + n_edges, N);

    graph_traits < Graph >::edge_iterator ei, ei_end;
    property_map<Graph, float EdgeProperties::*>::type
            weight_pmap = get(&EdgeProperties::weight, g);
    int i = 0;
    for (boost::tie(ei, ei_end) = edges(g); ei != ei_end; ++ei, ++i)
        weight_pmap[*ei] = weight[i];

    std::vector<float> distance(N, (std::numeric_limits < float >::max)());
    std::vector<std::size_t> parent(N);
    for (i = 0; i < N; ++i)
        parent[i] = i;
    distance[0] = 0;

    bool r = bellman_ford_shortest_paths
            (g, int (N), weight_map(weight_pmap).distance_map(&distance[0]).
                    predecessor_map(&parent[0]));

    if (r ) {
        std::ofstream output_file(argv[3]);
	if (not output_file.is_open()) {
		std::cout << "can not open file " << argv[3] << std::endl;
		return 2;
	}

        for (i = 0; i < N; ++i)
		output_file << i << " " << parent[i] << std::endl;
    } else
        std::cout << "negative cycle" << std::endl;

    return EXIT_SUCCESS;
}

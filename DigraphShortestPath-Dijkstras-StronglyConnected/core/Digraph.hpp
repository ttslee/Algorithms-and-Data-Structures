// Digraph.hpp
//
// ICS 46 Winter 2019
// Project #4: Rock and Roll Stops the Traffic
//
// This header file declares a class template called Digraph, which is
// intended to implement a generic directed graph. The implementation
// uses the adjacency lists technique, so each vertex stores a linked
// list of its outgoing edges.
//
// Along with the Digraph class template is a class DigraphException
// and a couple of utility structs that aren't generally useful outside
// of this header file.
//
// In general, directed graphs are all the same, except in the sense
// that they store different kinds of information about each vertex and
// about each edge; these two types are the type parameters to the
// Digraph class template.

#ifndef DIGRAPH_HPP
#define DIGRAPH_HPP

#include <exception>
#include <functional>
#include <list>
#include <map>
#include <utility>
#include <vector>
#include <iostream>
#include <queue>

// DigraphExceptions are thrown from some of the member functions in the
// Digraph class template, so that exception is declared here, so it
// will be available to any code that includes this header file.

class DigraphException : public std::runtime_error
{
public:
    DigraphException(const std::string& reason);
};


inline DigraphException::DigraphException(const std::string& reason)
    : std::runtime_error{reason}
{
}


// A DigraphEdge lists a "from vertex" (the number of the vertex from which
// the edge points), a "to vertex" (the number of the vertex to which the
// edge points), and an EdgeInfo object.  Because different kinds of Digraphs
// store different kinds of edge information, DigraphEdge is a struct template.

template <typename EdgeInfo>
struct DigraphEdge
{
    int fromVertex;
    int toVertex;
    EdgeInfo einfo;
};



// A DigraphVertex includes two things: a VertexInfo object and a list of
// its outgoing edges.  Because different kinds of Digraphs store different
// kinds of vertex and edge information, DigraphVertex is a struct template.

template <typename VertexInfo, typename EdgeInfo>
struct DigraphVertex
{
    VertexInfo vinfo;
    std::list<DigraphEdge<EdgeInfo>> edges;
};

enum class Kind
{
    OK, INF, NONE, UNK
};



// Digraph is a class template that represents a directed graph implemented
// using adjacency lists.  It takes two type parameters:
//
// * VertexInfo, which specifies the kind of object stored for each vertex
// * EdgeInfo, which specifies the kind of object stored for each edge
//
// You'll need to implement the member functions declared here; each has a
// comment detailing how it is intended to work.
//
// Each vertex in a Digraph is identified uniquely by a "vertex number".
// Vertex numbers are not necessarily sequential and they are not necessarily
// zero- or one-based.

template <typename VertexInfo, typename EdgeInfo>
class Digraph
{
public:
    // The default constructor initializes a new, empty Digraph so that
    // contains no vertices and no edges.
    Digraph();

    // The copy constructor initializes a new Digraph to be a deep copy
    // of another one (i.e., any change to the copy will not affect the
    // original).
    Digraph(const Digraph& d);

    // The move constructor initializes a new Digraph from an expiring one.
    Digraph(Digraph&& d) noexcept;

    // The destructor deallocates any memory associated with the Digraph.
    ~Digraph() noexcept;

    // The assignment operator assigns the contents of the given Digraph
    // into "this" Digraph, with "this" Digraph becoming a separate, deep
    // copy of the contents of the given one (i.e., any change made to
    // "this" Digraph afterward will not affect the other).
    Digraph& operator=(const Digraph& d);

    // The move assignment operator assigns the contents of an expiring
    // Digraph into "this" Digraph.
    Digraph& operator=(Digraph&& d) noexcept;

    // vertices() returns a std::vector containing the vertex numbers of
    // every vertex in this Digraph.
    std::vector<int> vertices() const;

    // edges() returns a std::vector of std::pairs, in which each pair
    // contains the "from" and "to" vertex numbers of an edge in this
    // Digraph.  All edges are included in the std::vector.
    std::vector<std::pair<int, int>> edges() const;

    // This overload of edges() returns a std::vector of std::pairs, in
    // which each pair contains the "from" and "to" vertex numbers of an
    // edge in this Digraph.  Only edges outgoing from the given vertex
    // number are included in the std::vector.  If the given vertex does
    // not exist, a DigraphException is thrown instead.
    std::vector<std::pair<int, int>> edges(int vertex) const;

    // vertexInfo() returns the VertexInfo object belonging to the vertex
    // with the given vertex number.  If that vertex does not exist, a
    // DigraphException is thrown instead.
    VertexInfo vertexInfo(int vertex) const;

    // edgeInfo() returns the EdgeInfo object belonging to the edge
    // with the given "from" and "to" vertex numbers.  If either of those
    // vertices does not exist *or* if the edge does not exist, a
    // DigraphException is thrown instead.
    EdgeInfo edgeInfo(int fromVertex, int toVertex) const;

    // addVertex() adds a vertex to the Digraph with the given vertex
    // number and VertexInfo object.  If there is already a vertex in
    // the graph with the given vertex number, a DigraphException is
    // thrown instead.
    void addVertex(int vertex, const VertexInfo& vinfo);

    // addEdge() adds an edge to the Digraph pointing from the given
    // "from" vertex number to the given "to" vertex number, and
    // associates with the given EdgeInfo object with it.  If one
    // of the vertices does not exist *or* if the same edge is already
    // present in the graph, a DigraphException is thrown instead.
    void addEdge(int fromVertex, int toVertex, const EdgeInfo& einfo);

    // removeVertex() removes the vertex (and all of its incoming
    // and outgoing edges) with the given vertex number from the
    // Digraph.  If the vertex does not exist already, a DigraphException
    // is thrown instead.
    void removeVertex(int vertex);

    // removeEdge() removes the edge pointing from the given "from"
    // vertex number to the given "to" vertex number from the Digraph.
    // If either of these vertices does not exist *or* if the edge
    // is not already present in the graph, a DigraphException is
    // thrown instead.
    void removeEdge(int fromVertex, int toVertex);

    // vertexCount() returns the number of vertices in the graph.
    int vertexCount() const noexcept;

    // edgeCount() returns the total number of edges in the graph,
    // counting edges outgoing from all vertices.
    int edgeCount() const noexcept;

    // This overload of edgeCount() returns the number of edges in
    // the graph that are outgoing from the given vertex number.
    // If the given vertex does not exist, a DigraphException is
    // thrown instead.
    int edgeCount(int vertex) const;

    // isStronglyConnected() returns true if the Digraph is strongly
    // connected (i.e., every vertex is reachable from every other),
    // false otherwise.
    bool isStronglyConnected() const;

    // findShortestPaths() takes a start vertex number and a function
    // that takes an EdgeInfo object and determines an edge weight.
    // It uses Dijkstra's Shortest Path Algorithm to determine the
    // shortest paths from the start vertex to every other vertex
    // in the graph.  The result is returned as a std::map<int, int>
    // where the keys are vertex numbers and the value associated
    // with each key k is the precedessor of that vertex chosen by
    // the algorithm.  For any vertex without a predecessor (e.g.,
    // a vertex that was never reached, or the start vertex itself),
    // the value is simply a copy of the key.
    std::map<int, int> findShortestPaths(
        int startVertex,
        std::function<double(const EdgeInfo&)> edgeWeightFunc) const;
private:
    class Flag
    {
    public:
        Flag() : vertex{}, hasPred{Kind::UNK}, lKind{Kind::INF}, pred{-1}, length{0.0}, shortest{false}
        {
        }

        Flag(int vert) : vertex{vert}, hasPred{Kind::UNK}, lKind{Kind::INF}, pred{vert}, length{0.0}, shortest{false}
        {
        }

        Flag(int vert, Kind p, Kind lk) : vertex{vert}, hasPred{p}, lKind{lk}, pred{vert}, length{0.0}, shortest{false}
        {
        }

        Flag(const Flag & f) : vertex{f.vertex}, hasPred{f.hasPred}, lKind{f.lKind}, pred{f.pred}, length{f.length}, shortest{f.shortest}
        {
        }

        Flag(Flag && f)
        {
            std::swap(vertex, f.vertex);
            std::swap(hasPred, f.hasPred);
            std::swap(lKind, f.lKind);
            std::swap(pred, f.pred);
            std::swap(length, f.length);
            std::swap(shortest, f.shortest);
        }

        Flag& operator=(const Flag & f)
        {
            if(this != &f)
            {
                vertex = f.vertex;
                hasPred = f.hasPred;
                lKind = f.lKind;
                pred = f.pred;
                length = f.length;
                shortest = f.shortest;
            }
            return *this;
        }

        Flag& operator=(Flag && f)
        {
            if(this != &f)
            {
                std::swap(vertex, f.vertex);
                std::swap(hasPred, f.hasPred);
                std::swap(lKind, f.lKind);
                std::swap(pred, f.pred);
                std::swap(length, f.length);
                std::swap(shortest, f.shortest);
            }
            return *this;
        }
        bool operator() (Flag & lh, Flag & rh)
        {
            if(lh.lKind == Kind::INF)
                return true;
            return lh.length > rh.length;
        }
        void setPred(int p) { pred = p; }
        void setLength(double l) { length = l; }
        void setShortest(bool s) { shortest = s; }
        void setlKind(Kind k) { lKind = k; }
        void setHasPred(Kind k) { hasPred = k; }
        int getVertex() { return vertex; }
        int getPred() { return pred; }
        double getLength() { return length; }
        bool getShortest() { return shortest; }
        Kind hasPredecessor() { return hasPred; }
        Kind lengthKind() { return lKind; }
    private:
        int vertex;
        Kind hasPred;
        Kind lKind;
        int pred;
        double length;
        bool shortest;
        
    };

private:
    std::map<int, DigraphVertex<VertexInfo, EdgeInfo>> graph;

    // mutable std::map<int, Flag> priorityMap;
    
    // Add whatever member variables you think you need here.  One
    // possibility is a std::map where the keys are vertex numbers
    // and the values are DigraphVertex<VertexInfo, EdgeInfo> objects.

    bool DFTr(std::map<int, bool>& visitedMap, int fromVertex, int toVertex) const;
    // You can also feel free to add any additional member functions
    // you'd like (public or private), so long as you don't remove or
    // change the signatures of the ones that already exist.
};



// You'll need to implement the member functions below.  There's enough
// code in place to make them compile, but they'll all need to do the
// correct thing instead.

template <typename VertexInfo, typename EdgeInfo>
Digraph<VertexInfo, EdgeInfo>::Digraph() : graph{}
{
}


template <typename VertexInfo, typename EdgeInfo>
Digraph<VertexInfo, EdgeInfo>::Digraph(const Digraph& d) : graph{d.graph}
{
}


template <typename VertexInfo, typename EdgeInfo>
Digraph<VertexInfo, EdgeInfo>::Digraph(Digraph&& d) noexcept
{
    std::swap(graph, d.graph);
}


template <typename VertexInfo, typename EdgeInfo>
Digraph<VertexInfo, EdgeInfo>::~Digraph() noexcept
{
}


template <typename VertexInfo, typename EdgeInfo>
Digraph<VertexInfo, EdgeInfo>& Digraph<VertexInfo, EdgeInfo>::operator=(const Digraph& d)
{
    if(this != &d)
    {
        graph = d.graph;
    }
    return *this;
}


template <typename VertexInfo, typename EdgeInfo>
Digraph<VertexInfo, EdgeInfo>& Digraph<VertexInfo, EdgeInfo>::operator=(Digraph&& d) noexcept
{
    if(this != &d)
        std::swap(graph, d.graph);
    return *this;
}


template <typename VertexInfo, typename EdgeInfo>
std::vector<int> Digraph<VertexInfo, EdgeInfo>::vertices() const
{
    std::vector<int> vertices;
    for(auto & v: graph)
        vertices.push_back(v.first);
    return vertices;
}


template <typename VertexInfo, typename EdgeInfo>
std::vector<std::pair<int, int>> Digraph<VertexInfo, EdgeInfo>::edges() const
{
    std::vector<std::pair<int, int>> vec;
    for(auto it = graph.begin(); it != graph.end(); it++)
    {
        for(auto & e: it->second.edges)
        {
            vec.push_back(std::pair<int, int>(e.fromVertex, e.toVertex));
        }
    }
    return vec;
}


template <typename VertexInfo, typename EdgeInfo>
std::vector<std::pair<int, int>> Digraph<VertexInfo, EdgeInfo>::edges(int vertex) const
{
    std::vector<std::pair<int, int>> vec;
    auto v = graph.find(vertex);
    if(v == graph.end())
        throw DigraphException("Vertex does not exist");
    for(auto & e: v->second.edges)
    {
        vec.push_back(std::pair<int, int>(vertex, e.toVertex));
    }

    return vec;
}


template <typename VertexInfo, typename EdgeInfo>
VertexInfo Digraph<VertexInfo, EdgeInfo>::vertexInfo(int vertex) const
{
    auto v = graph.find(vertex);
    if(v == graph.end())
        throw DigraphException("Vertex does not exist");
    return v->second.vinfo;
}


template <typename VertexInfo, typename EdgeInfo>
EdgeInfo Digraph<VertexInfo, EdgeInfo>::edgeInfo(int fromVertex, int toVertex) const
{
    auto v1 = graph.find(fromVertex);
    auto v2 = graph.find(toVertex);
    if(v1 == graph.end() || v2 == graph.end())
        throw DigraphException("Vertex does not exist");
    for(auto & e: v1->second.edges)
    {
        if(e.toVertex == toVertex)
            return e.einfo;
    }
    throw DigraphException("Edge does not exist");
}


template <typename VertexInfo, typename EdgeInfo>
void Digraph<VertexInfo, EdgeInfo>::addVertex(int vertex, const VertexInfo& vinfo)
{
    auto v = graph.find(vertex);
    if(v != graph.end())
        throw DigraphException("Vertex already exists");
    graph[vertex] = {vinfo, std::list<DigraphEdge<EdgeInfo>>{}};
}


template <typename VertexInfo, typename EdgeInfo>
void Digraph<VertexInfo, EdgeInfo>::addEdge(int fromVertex, int toVertex, const EdgeInfo& einfo)
{
    auto v1 = graph.find(fromVertex);
    auto v2 = graph.find(toVertex);
    if(v1 == graph.end() || v2 == graph.end())
        throw DigraphException("Vertex does not exist");
    for(auto & e: v1->second.edges)
    {
        if(e.toVertex == toVertex)
            throw DigraphException("Edge Already Exists");
    }
    v1->second.edges.insert(v1->second.edges.begin(), {fromVertex, toVertex, einfo});
}


template <typename VertexInfo, typename EdgeInfo>
void Digraph<VertexInfo, EdgeInfo>::removeVertex(int vertex)
{
    auto v = graph.find(vertex);
    if(v == graph.end())
        throw DigraphException("Vertex does not exist");
    for(auto & vert: graph)
    {
        for(auto it = vert.second.edges.begin(); it != vert.second.edges.end(); it++)
        {
            if(it->toVertex == vertex)
                vert.second.edges.erase(it);
        }
    }
    graph.erase(v);
}


template <typename VertexInfo, typename EdgeInfo>
void Digraph<VertexInfo, EdgeInfo>::removeEdge(int fromVertex, int toVertex)
{
    auto v1 = graph.find(fromVertex);
    auto v2 = graph.find(toVertex);
    if(v1 == graph.end() || v2 == graph.end())
        throw DigraphException("Vertex does not exist");
    for(auto it = v1->second.edges.begin(); it != v1->second.edges.end(); it++)
    {
        if(it->toVertex == toVertex)
        {
            v1->second.edges.erase(it);
            return;
        }
    }
    throw DigraphException("Edge does not exist");
}


template <typename VertexInfo, typename EdgeInfo>
int Digraph<VertexInfo, EdgeInfo>::vertexCount() const noexcept
{
    return graph.size();
}


template <typename VertexInfo, typename EdgeInfo>
int Digraph<VertexInfo, EdgeInfo>::edgeCount() const noexcept
{
    int count = 0;
    for(auto & v: graph)
    {
        count += v.second.edges.size();
    }
    return count;
}


template <typename VertexInfo, typename EdgeInfo>
int Digraph<VertexInfo, EdgeInfo>::edgeCount(int vertex) const
{
    auto v = graph.find(vertex);
    if(v == graph.end())
        throw DigraphException("Vertex does not exist");
    return v->second.edges.size();
}

template <typename VertexInfo, typename EdgeInfo>
bool Digraph<VertexInfo, EdgeInfo>::DFTr(std::map<int, bool>& visitedMap, int fromVertex, int toVertex) const
{
    if(fromVertex == toVertex)
        return true;
    visitedMap[fromVertex] = true;
    auto v = graph.find(fromVertex);
    for(auto & e: v->second.edges)
    {
        if(!visitedMap[e.toVertex])
        {
            bool found =  DFTr(visitedMap, e.toVertex, toVertex);
            if(found)
            {
                visitedMap[fromVertex] = false;
                return true;
            }
        }
    }
    visitedMap[fromVertex] = false;
    return false;
}

template <typename VertexInfo, typename EdgeInfo>
bool Digraph<VertexInfo, EdgeInfo>::isStronglyConnected() const
{
    std::map<int, bool> visitedMap;
    for(auto & v: graph)
    {
        if(v.second.edges.empty())
            return false;
        visitedMap[v.first] = false;
    }
    std::vector<int> vert = vertices();
    
    int count = -1;
    for(auto & v1: vert)
    {
        count++;
        for(int c = count; c < vert.size(); c++)
        {
            if(v1 == vert[c])
                continue;
            bool first = DFTr(visitedMap, v1, vert[c]);
            if(!first)
                return false;
            bool second = DFTr(visitedMap, vert[c], v1);
            if(!second)
            {
                return false;
            }
        }
    }
    return true;
}


template <typename VertexInfo, typename EdgeInfo>
std::map<int, int> Digraph<VertexInfo, EdgeInfo>::findShortestPaths(
    int startVertex,
    std::function<double(const EdgeInfo&)> edgeWeightFunc) const
{
    auto check = graph.find(startVertex);
    if(check == graph.end())
        throw DigraphException("Vertex does not exist");
    std::map<int, int> ret;
    std::map<int, Flag> priorityMap;
    for(auto & v: graph)
    {
        if(startVertex == v.first)
        {
            priorityMap[v.first] = {v.first, Kind::NONE, Kind::OK};
        }
        else
            priorityMap[v.first] = {v.first};
    }
    std::priority_queue<Flag, std::vector<Flag>, Flag> pq;
    pq.push(priorityMap[startVertex]);

    while(!pq.empty())
    {
        Flag vert = pq.top();
        pq.pop();
        if(!vert.getShortest())
        {
            priorityMap[vert.getVertex()].setShortest(true);
            for(auto & v: graph.find(vert.getVertex())->second.edges)
            {
                if(priorityMap[v.toVertex].lengthKind() == Kind::INF)
                {
                    priorityMap[v.toVertex].setlKind(Kind::OK);
                    priorityMap[v.toVertex].setHasPred(Kind::OK);
                    priorityMap[v.toVertex].setPred(vert.getVertex());
                    priorityMap[v.toVertex].setLength(vert.getLength() + edgeWeightFunc(v.einfo));
                    pq.emplace(priorityMap[v.toVertex]);
                }
                else if(priorityMap[v.toVertex].getLength() > (vert.getLength() + edgeWeightFunc(v.einfo)))
                {
                    priorityMap[v.toVertex].setLength(vert.getLength() + edgeWeightFunc(v.einfo));
                    priorityMap[v.toVertex].setPred(vert.getVertex()); 
                    pq.emplace(priorityMap[v.toVertex]);
                }
            }
        }
    }
    for(auto & v: priorityMap)
    {
        ret[v.first] = v.second.getPred();
    }
    return ret;
}



#endif // DIGRAPH_HPP


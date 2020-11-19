/*************************************************
Author: Yuting Xie
Date:2020-11-16
Description: 253P HW6.1 Implement Clustering
**************************************************/

#include <iostream>
#include <fstream>
#include <vector>
#include <cassert>
#include <algorithm>
#include <set>
#include <map>

using std::cout;
using std::cin;
using std::fstream;
using std::vector;
using std::sort;
using std::map;
using std::set;

struct Coord {
    int x;
    int y;
    Coord() :x(0), y(0) {};
    Coord(int _x, int _y) :x(_x), y(_y) {};
    void operator=(Coord& point) {
        x = point.x;
        y = point.y;
    }
    int distanceTo(Coord& point) {
        return (x - point.x) * (x - point.x) + (y - point.y) * (y - point.y);
    }
};

struct Edge {
    int p1;
    int p2;
    int dist;
    Edge(int _p1, int _p2, int _dist) :p1(_p1), p2(_p2), dist(_dist) {}
};

void readInput(vector<Coord>& city, vector<int>& K) {
    fstream fin;
    fin.open("hw6_sample_input.txt");

    int numPoints, k;
    Coord coord;
    char c;

    city.clear();
    K.clear();

    fin >> numPoints;
    for (int i = 0; i < numPoints; i++) {
        fin >> c;
        fin >> coord.x >> coord.y;
        city.push_back(coord);
    }

    for (int i = 0; i < 3; i++) {
        fin >> k;
        K.push_back(k);
    }

}


//the union-find set, storing the father of each node, at first fathers are themselves
int ufs[1024];
int find(int node) {
    if (ufs[node] != node)
        ufs[node] = find(ufs[node]);

    return ufs[node];
}
void unite(int node1, int node2) {
    ufs[find(node2)] = find(node1);
}

//cluster cities into K classes
vector<vector<char>> cluster(vector<Coord>& city, int k) {

    //get the eage list 
    vector<Edge> edges;

    for (int i = 0; i < city.size(); i++) {
        for (int j = i + 1; j < city.size(); j++) {
            edges.push_back(Edge(i, j, city[i].distanceTo(city[j])));
        }
    }

    //should be n*(n-1)/2 edges for n nodes
    assert(edges.size() == city.size()*(city.size()-1)/2);

    //sort the edges
    auto myLess = [](Edge e1, Edge e2) {
        return e1.dist < e2.dist;
    };
    sort(edges.begin(), edges.end(), myLess);

    //run Kruskal's algo for n-k times.
    //this equals to removing the k-1 longest-dist edges

    for (int i = city.size()-1 ; i >=0  ; i--)
        ufs[i] = i;

    int edgeInTree = 0;
    for (int i = 0; i < edges.size(); i++) {
        if (edgeInTree == city.size() - k)
            break;

        //this edge introduces a cycle, skip it
        if (find(edges[i].p1) == find(edges[i].p2))
            continue;

        //other wise, unite the two nodes, adding one edge
        edgeInTree++;
        unite(edges[i].p1, edges[i].p2);

        //do find for all city to make sure they are same father
        for (int i = 0; i < city.size(); i++)
            find(i);

    }

    //collect all cities from ufs:
    map<int, set<int> > m;
    for (int i = 0; i < city.size(); i++) {
        auto it = m.find(ufs[i]);
        if (it == m.end()) {
            set<int> tmp;
            tmp.insert(i);
            m[ufs[i]] = tmp;
        }
        else {
            it->second.insert(i);
        }
    }

    //convert mapping into result vector
    vector<vector<char>> res;
    for (auto& it : m) {
        vector<char> t;
        res.push_back(t);
        for (int city : it.second)
            res[res.size() - 1].push_back(city + 65);
    }

    return res;

}


int main() {
    vector<Coord> city;
    vector<int> K;
    readInput(city, K);

    for (int k : K) {
        auto res = cluster(city, k);
        cout << "k = " << k << std::endl;
        int count = 1;
        for (auto clu : res) {
            cout << "Cluster " << count++ << " : ";
            for (char city : clu)
                cout << city << " ";
            cout << std::endl;
        }
        cout << std::endl;
    }

    return 0;

}
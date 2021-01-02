/************************************************* 
Author: Yuting Xie
Date:2020-11-21
Description: 253P QUIZ6 : Count connected components.
**************************************************/ 

#include<cstdio>
#include<vector>

using std::vector;

//dfs from a specific node in a graph
void dfs(vector<vector<int>>& adj_list, vector<bool>& visited, int node){
    visited[node] = true;
    for(int next : adj_list[node]){
        if(!visited[next])
            dfs(adj_list, visited, next);
    }
}

//return the number of connected components in a graph
int count_connected_components(vector<vector<int>>& adj_list, int N){
    vector<bool> visited(N, false);

    int count = 0;
    for(int i=0; i<N; i++){
        if(!visited[i]){
            count++;
            dfs(adj_list, visited, i);
        }
    }

    return count;
}

int main(){
    vector<int> v0;
    v0.push_back(1);
    v0.push_back(2);

    vector<int> v1;
    v1.push_back(0);

    vector<int> v2;
    v2.push_back(0);

    vector<int> v3;
    v3.push_back(4);

    vector<int> v4;
    v4.push_back(3);

    vector<vector<int>> adj;
    adj.push_back(v0);
    adj.push_back(v1);
    adj.push_back(v2);
    adj.push_back(v3);
    adj.push_back(v4);

    printf("result: %d\n", count_connected_components(adj, 5));

    return 0;

}

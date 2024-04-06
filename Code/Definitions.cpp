#include <bits/stdc++.h>
using namespace std;

int B;
int S;
int C;
int J;

int Latest_Deadline = 0;


typedef class Chunk
{
    public:
    int id;
    Chunk(int id)
    {
        this->id = id;
    }
} Chunk;

typedef class Job
{
    public:
    int id;
    vector<pair<Chunk*, int>> chunk_set;
    int processing_time;
    int deadline;

    Job(int id, vector<pair<Chunk*, int>> chunk_set, int processing_time, int deadline)
    {
        this->id = id;
        this->chunk_set = chunk_set;
        this->processing_time = processing_time;
        this->deadline = deadline;
    }
} Job;

typedef class Node
{
    public:
    set<int> node_chunk;
    vector<vector<pair<int, int>>> vm_schedule;

    Node()
    {
        this->vm_schedule.resize(S, vector<pair<int, int>>(Latest_Deadline, {-1, -1}));
    }

} Node;

vector<Chunk*> cloud_dfs;
vector<Job*> job_set;
vector<Node *> active_nodes;
set<int> deadlines;
map<int, vector<Node *>> nodes_created;

void Print()
{
    for (int t = 0; t < Latest_Deadline; t++)
    {
        printf("Time %d %5c", t,' ');
        for (int n = 0; n < active_nodes.size(); n++)
        {
            printf("Node %d %5c ", n,' ');
            for (int vm = 0; vm < S; vm++)
            {
                cout << "{" << active_nodes[n]->vm_schedule[vm][t].first << active_nodes[n]->vm_schedule[vm][t].second << "} ";
            }
        }
        cout << endl;
    }
}
// vector <int,int> to_schedule;

// a, b;

// job_set[a].chunk_set[b].second

#include <bits/stdc++.h>
using namespace std;

typedef struct Chunk
{
    int id;
    Chunk(int id)
    {
        this->id = id;
    }
} Chunk;

typedef struct Job
{
    int id;
    vector<pair<Chunk, int>> chunk_set;
    int processing_time;
    int deadline;

    Job(int id, vector<pair<Chunk, int>> chunk_set, int processing_time, int deadline)
    {
        this->id = id;
        this->chunk_set = chunk_set;
        this->processing_time = processing_time;
        this->deadline = deadline;
    }
} Job;

typedef struct Node
{
    set<Chunk> node_chunk;
    vector<vector<pair<int, int>>> vm_schedule;

    Node()
    {
        this->vm_schedule.resize(S, vector<pair<int, int>>(Latest_Deadline, {-1, -1}));
    }

} Node;

int B;
int S;
int C;
int J;

int Latest_Deadline = 0;
vector<Chunk> cloud_dfs;
vector<Job> job_set;
vector<Node *> active_nodes;
set <int> deadlines;
map <int,vector<Node *>> nodes_created;
// vector <int,int> to_schedule;

// a, b;

// job_set[a].chunk_set[b].second

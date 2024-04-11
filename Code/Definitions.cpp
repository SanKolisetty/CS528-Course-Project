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
    vector<set<int>> vm_timeslot_chunks;

    Node()
    {
        this->vm_schedule.resize(S, vector<pair<int, int>>(Latest_Deadline, {-1, -1}));
        this->vm_timeslot_chunks.resize(Latest_Deadline);
    }

} Node;

vector<Chunk*> cloud_dfs;
vector<Job*> job_set;
vector<Job*> job_set_ffa;
vector<Node *> active_nodes;
vector<Node *> active_nodes_ffa;
set<int> deadlines;
map<int, vector<Node *>> nodes_created;

void Print(vector<Node *> active_nodes)
{
    bool scheduled = false;
    FILE *fp = fopen("Output.txt", "w");
    for (int t = 0; t < Latest_Deadline; t++)
    {
        for (int n = 0; n < active_nodes.size(); n++)
        {
            for (int vm = 0; vm < S; vm++)
            {
                if(active_nodes[n]->vm_schedule[vm][t].first != -1)
                {
                    scheduled = true;
                    break;
                }
            }
        }

        if(scheduled)
        {
            fprintf(fp, "Time %d %3c\n", t,' ');
            for (int n = 0; n < active_nodes.size(); n++)
            {
                fprintf(fp, "Node %d %3c ", n,' ');
                for (int vm = 0; vm < S; vm++)
                {
                    if(active_nodes[n]->vm_schedule[vm][t].first != -1)
                        fprintf(fp, "Job %d : Chunk %d   |   ", active_nodes[n]->vm_schedule[vm][t].first, job_set[active_nodes[n]->vm_schedule[vm][t].first]->chunk_set[active_nodes[n]->vm_schedule[vm][t].second].first->id);
                        // cout << "Job " << active_nodes[n]->vm_schedule[vm][t].first << " : Chunk " << job_set[active_nodes[n]->vm_schedule[vm][t].first]->chunk_set[active_nodes[n]->vm_schedule[vm][t].second].first->id << "   |   " ;
                    else
                        fprintf(fp, "Job N : Chunk N   |   ");
                }
                fprintf(fp, "\n");
            }
        }

        scheduled = false;
    }
}
// vector <int,int> to_schedule;

// a, b;

// job_set[a].chunk_set[b].second

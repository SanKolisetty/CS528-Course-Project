#include <bits/stdc++.h>
#ifndef DEFS_H
#define DEFS_H
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

    // Copy constructor
    Chunk(const Chunk& other) : id(other.id) {}

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

    // Copy constructor
    Job(const Job& other)
        : id(other.id), chunk_set(other.chunk_set), processing_time(other.processing_time), deadline(other.deadline) {}

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

    // Copy constructor
    Node(Node* other)
        : node_chunk(other->node_chunk), vm_schedule(other->vm_schedule), vm_timeslot_chunks(other->vm_timeslot_chunks) {}

} Node;

vector<Chunk*> cloud_dfs;
vector<Job*> job_set;
vector<Job*> job_set_sjf;
vector<Job*> job_set_ffa;
vector<Job*> job_set_sda;
vector<Node *> active_nodes;
vector<Node *> active_nodes_sjf;
vector<Node *> active_nodes_ffa;
vector<Node *> active_nodes_sda;

set<int> deadlines;
map<int, vector<Node *>> nodes_created;

// void Print(vector<Node *> active_nodes, FILE * fp)
// {
//     bool scheduled = false;
//     // FILE *fp = fopen("Output.txt", "w");
//     for (int t = 0; t < Latest_Deadline; t++)
//     {
//         for (int n = 0; n < active_nodes.size(); n++)
//         {
//             for (int vm = 0; vm < S; vm++)
//             {
//                 if(active_nodes[n]->vm_schedule[vm][t].first != -1)
//                 {
//                     scheduled = true;
//                     break;
//                 }
//             }
//         }

//         if(scheduled)
//         {
//             fprintf(fp, "Time %d %3c\n", t,' ');
//             for (int n = 0; n < active_nodes.size(); n++)
//             {
//                 fprintf(fp, "Node %d %3c ", n,' ');
//                 for (int vm = 0; vm < S; vm++)
//                 {
//                     if(active_nodes[n]->vm_schedule[vm][t].first != -1)
//                         fprintf(fp, "Job %d : Chunk %d   |   ", active_nodes[n]->vm_schedule[vm][t].first, job_set[active_nodes[n]->vm_schedule[vm][t].first]->chunk_set[active_nodes[n]->vm_schedule[vm][t].second].first->id);
//                         // cout << "Job " << active_nodes[n]->vm_schedule[vm][t].first << " : Chunk " << job_set[active_nodes[n]->vm_schedule[vm][t].first]->chunk_set[active_nodes[n]->vm_schedule[vm][t].second].first->id << "   |   " ;
//                     else
//                         fprintf(fp, "Job N : Chunk N   |   ");
//                 }
//                 fprintf(fp, "\n");
//             }
//         }

//         scheduled = false;
//     }
// }

void Print(vector<Node *> active_nodes, string output)
{
    string path = "./console/Output_"+output+".txt";
    ofstream fp(path);
    bool scheduled = false;
    // FILE *fp = fopen("Output.txt", "w");
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
            // fprintf(fp, "Time %d %3c\n", t,' ');
            fp << "Time "<< t <<"   "<<endl;
            for (int n = 0; n < active_nodes.size(); n++)
            {
                // fprintf(fp, "Node %d %3c ", n,' ');
                fp << "Node "<<n<<"   ";
                for (int vm = 0; vm < S; vm++)
                {
                    if(active_nodes[n]->vm_schedule[vm][t].first != -1)
                    {
                        // fprintf(fp, "Job %d : Chunk %d   |   ", active_nodes[n]->vm_schedule[vm][t].first, job_set[active_nodes[n]->vm_schedule[vm][t].first]->chunk_set[active_nodes[n]->vm_schedule[vm][t].second].first->id);
                        // cout << "Job " << active_nodes[n]->vm_schedule[vm][t].first << " : Chunk " << job_set[active_nodes[n]->vm_schedule[vm][t].first]->chunk_set[active_nodes[n]->vm_schedule[vm][t].second].first->id << "   |   " ;
                        int job = active_nodes[n]->vm_schedule[vm][t].first;
                        int chunk = job_set[active_nodes[n]->vm_schedule[vm][t].first]->chunk_set[active_nodes[n]->vm_schedule[vm][t].second].first->id;
                        fp << "Job "<<job<<" : Chunk "<<chunk<<"   |   ";
                    }
                    else
                    {
                        // fprintf(fp, "Job N : Chunk N   |   ");
                        fp << "Job N : Chunk N   |   ";
                    }
                }
                // fprintf(fp, "\n");
                fp << endl;
            }
        }

        scheduled = false;
    }
    fp.close();
}
// vector <int,int> to_schedule;

// a, b;

// job_set[a].chunk_set[b].second

#endif // DEFS_H
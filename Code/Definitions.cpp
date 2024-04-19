#include <bits/stdc++.h>
#ifndef DEFS_H
#define DEFS_H
using namespace std;

// Global variables
int B;  // Number of chunks a node can handle
int S;  // Number of virtual machines
int C;  // Number of chunks
int J;  // Number of jobs

int Latest_Deadline = 0;  // Tracks the latest deadline among jobs

// Class definition for Chunk
typedef class Chunk
{
public:
    int id;  // Unique identifier for the chunk

    // Constructor
    Chunk(int id) {
        this->id = id;
    }

    // Copy constructor
    Chunk(const Chunk& other) : id(other.id) {}

} Chunk;

// Class definition for Job
typedef class Job
{
public:
    int id;  // Unique identifier for the job
    vector<pair<Chunk*, int>> chunk_set;  // Collection of chunks associated with the job, along with their processing times
    int processing_time;  // Total processing time required for the job
    int deadline;  // Deadline by which the job must be completed

    // Constructor
    Job(int id, vector<pair<Chunk*, int>> chunk_set, int processing_time, int deadline) {
        this->id = id;
        this->chunk_set = chunk_set;
        this->processing_time = processing_time;
        this->deadline = deadline;
    }

    // Copy constructor
    Job(const Job& other) : id(other.id), chunk_set(other.chunk_set), processing_time(other.processing_time), deadline(other.deadline) {}

} Job;

// Class definition for Node
typedef class Node
{
public:
    set<int> node_chunk;  // Set of chunk IDs currently assigned to this node
    vector<vector<pair<int, int>>> vm_schedule;  // Matrix representing the schedule of virtual machines, where each entry is a pair of job ID and chunk ID
    vector<set<int>> vm_timeslot_chunks;  // List of sets, each representing chunks scheduled for a specific timeslot in the virtual machines

    // Constructor
    Node() {
        this->vm_schedule.resize(S, vector<pair<int, int>>(Latest_Deadline, {-1, -1}));
        this->vm_timeslot_chunks.resize(Latest_Deadline);
    }

    // Copy constructor
    Node(Node* other) : node_chunk(other->node_chunk), vm_schedule(other->vm_schedule), vm_timeslot_chunks(other->vm_timeslot_chunks) {}

} Node;

// Global variables and data structures
vector<Chunk*> cloud_dfs;  // List of chunks available in the cloud
vector<Job*> job_set;  // List of jobs
vector<Job*> job_set_greedy;  // List of jobs 
vector<Job*> job_set_ffa;  // List of jobs 
vector<Job*> job_set_sda;  // List of jobs 
vector<Node *> active_nodes;  // List of active nodes
vector<Node *> active_nodes_greedy;  // List of active nodes for SJF
vector<Node *> active_nodes_ffa;  // List of active nodes for FFA
vector<Node *> active_nodes_sda;  // List of active nodes for SDA

set<int> deadlines;  // Set of unique deadlines among jobs
map<int, vector<Node *>> nodes_created;  // Mapping of deadlines to the nodes created for each deadline

void take_user_input()
{
    cout << "Enter S,B" << endl;
    cin >> S >> B;

    cout << "Total Number of Chunks" << endl;
    cin >> C;
    cloud_dfs.resize(C);
    for (int i = 0; i < C; i++)
    {
        Chunk*c =  new Chunk (i);
        cloud_dfs[i] = c;
    }

    cout << "Total Number of Jobs" << endl;
    cin >> J;
    job_set.resize(J);
    int processing_time;
    int deadline;
    vector<pair<Chunk*, int>> chunk_set;
    int chunk_set_size;
    int chunk_id;

    for (int i = 0; i < J; i++)
    {
        chunk_set.clear();
        cout << "Enter processing time and deadline" << endl;
        cin >> processing_time >> deadline;

        deadlines.insert(deadline);
        Latest_Deadline = max(Latest_Deadline, deadline);

        cout << "Enter total number of chunks required"<<endl;
        cin >> chunk_set_size;

        chunk_set.resize(chunk_set_size);

        for (int j = 0; j < chunk_set_size; j++)
        {
            cout << "Enter Chunk id required : ";
            cin >> chunk_id;
            chunk_set[j] = {cloud_dfs[chunk_id], processing_time};
        }

        Job * job =  new Job(i, chunk_set, processing_time, deadline);

    
        job_set[i] = job;
        // cout << i <<endl;
        // job_set_ffa[i] = job;
        // cout << i <<endl;
        // job_set_sda[i] = job;
        // cout << i <<endl;
        // job_set_greedy[i] = job;
        // cout << i <<endl;
    }
    job_set_ffa = job_set;
    job_set_sda = job_set;
    job_set_greedy = job_set;
}


void take_file_input()
{
    ifstream fin("./console/input.txt");

    fin >> S >> B;
    fin >> C;
    cloud_dfs.resize(C);
    for (int i = 0; i < C; i++)
    {
        Chunk*c =  new Chunk (i);
        cloud_dfs[i] = c;
    }

    fin >> J;
    job_set.resize(J);
    job_set_ffa.resize(J);
    job_set_sda.resize(J);
    job_set_greedy.resize(J);

    int processing_time;
    int deadline;
    vector<pair<Chunk*, int>> chunk_set;
    int chunk_set_size;
    int chunk_id;

    for (int i = 0; i < J; i++)
    {
        chunk_set.clear();
        fin >> processing_time >> deadline;

        deadlines.insert(deadline);
        Latest_Deadline = max(Latest_Deadline, deadline);

        fin >> chunk_set_size;
        

        chunk_set.resize(chunk_set_size);

        for (int j = 0; j < chunk_set_size; j++)
        {
            fin >> chunk_id;
            chunk_set[j] = {cloud_dfs[chunk_id], processing_time};
        }

        Job*job =  new Job(i, chunk_set, processing_time, deadline);
        job_set[i] = job;
        job_set_ffa[i] = job;
        job_set_sda[i] = job;
        job_set_greedy[i] = job;
    }
}

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
            fp << "Time "<< t <<"   "<<endl;
            for (int n = 0; n < active_nodes.size(); n++)
            {
                fp << "Node "<<n<<"   ";
                for (int vm = 0; vm < S; vm++)
                {
                    if(active_nodes[n]->vm_schedule[vm][t].first != -1)
                    {
                        int job = active_nodes[n]->vm_schedule[vm][t].first;
                        int chunk = job_set[active_nodes[n]->vm_schedule[vm][t].first]->chunk_set[active_nodes[n]->vm_schedule[vm][t].second].first->id;
                        fp << "Job "<<job<<" : Chunk "<<chunk<<"   |   ";
                    }
                    else
                    {
                        fp << "Job N : Chunk N   |   ";
                    }
                }
                fp << endl;
            }
        }

        scheduled = false;
    }
    fp.close();
}


#endif // DEFS_H
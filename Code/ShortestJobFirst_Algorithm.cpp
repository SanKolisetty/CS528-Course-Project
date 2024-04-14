#include "Definitions.cpp"
using namespace std;

vector <int> scheduled;

vector<Job*> copyJobSetSJF(const vector<Job*>& original) {
    vector<Job*> copiedJobSetSJF;
    
    // Iterate through the original vector and copy each Job object
    for (const auto& jobPtr : original) {
        // Create a new Job object with the same attributes
        Job* copiedJob = new Job(*jobPtr);
        
        // Iterate through the chunk_set of the original Job and copy each Chunk object
        vector<pair<Chunk*, int>> copiedChunkSet;
        for (const auto& chunkPair : jobPtr->chunk_set) {
            // Create a new Chunk object with the same id
            Chunk* copiedChunk = new Chunk(*chunkPair.first);
            copiedChunkSet.push_back({copiedChunk, chunkPair.second});
        }
        
        // Set the copied chunk_set to the copied Job
        copiedJob->chunk_set = copiedChunkSet;
        
        // Add the copied Job to the copied vector
        copiedJobSetSJF.push_back(copiedJob);
    }
    
    return copiedJobSetSJF;
}

void vm_scheduling_sjf(Node *node, pair<int, int> chunk, int time_req, int d, vector <Job*> &job_set_sjf_copy)
{
    cout << job_set_sjf[chunk.first]->chunk_set[chunk.second].second << ": time dekho ab1" << endl;

    int current_chunk = job_set_sjf_copy[chunk.first]->chunk_set[chunk.second].first->id;
    // time_req = job_set_sjf_copy[chunk.first]->chunk_set[chunk.second].second;
    cout<<"DEAD" <<d<<endl;
    cout << job_set_sjf[chunk.first]->chunk_set[chunk.second].second << ": time dekho ab2" << endl;

    for (int t = 0; t < d; t++)
    {
        cout << job_set_sjf[chunk.first]->chunk_set[chunk.second].second << ": time dekho ab3" << endl;

        cout << t <<" " <<d <<endl;
        if (time_req == 0)
        {
            cout << "Time khatam" << endl;
            break;

        }

        if (node->vm_timeslot_chunks[t].count(current_chunk))
        {
            cout << "Yahi problem hai" << endl;

            continue;
        }
        
        for (int m = 0; m < S; m++)
        {
            if (node->vm_schedule[m][t] != make_pair(-1, -1))
            {
                cout << "-1 -1" << endl;
                continue;
            }
            if (time_req == 0)
            {
                cout << "Time khatam" << endl;
                break;

            }
            node->vm_timeslot_chunks[t].insert(current_chunk);
            node->vm_schedule[m][t] = chunk;
            node->node_chunk.insert(current_chunk);
            time_req--;
            cout << "DECREASED TIME:" << time_req << endl;
            break;
        }
    }
    cout << job_set_sjf[chunk.first]->chunk_set[chunk.second].second << ": time dekho ab1" << endl;

    job_set_sjf_copy[chunk.first]->chunk_set[chunk.second].second = time_req;

    cout << job_set_sjf[chunk.first]->chunk_set[chunk.second].second << ": time dekho ab1" << endl;

    cout << time_req << " time required left" << endl;

    cout << job_set_sjf[chunk.first]->chunk_set[chunk.second].second << ": time dekho ab1" << endl;

}

int schedule(vector<pair<int, int>> to_schedule, int d)
{
    vector <Node *> active_nodes_sjf_copy;
    // int cur_tot_nodes = active_nodes_sjf.size();
    int extra_nodes = 0;
    cout << "Start" << endl;
    vector<Job *> job_set_sjf_copy = copyJobSetSJF(job_set_sjf);
    for (int i = 0; i < to_schedule.size(); i++)
    {
        cout << "Yes" << endl;
        cout << to_schedule[i].first << " " << to_schedule[i].second << endl;
        int current_chunk = job_set_sjf_copy[to_schedule[i].first]->chunk_set[to_schedule[i].second].first->id;
        cout << "Current chunk: " << i << endl;
        for (int n = 0; n < active_nodes_sjf_copy.size(); n++)
        {
            cout << "Inside old node: " << n << endl;
            Node node_new = new Node(active_nodes_sjf_copy[n]);
            Node *node = &node_new;
            int time_slot_req = job_set_sjf_copy[to_schedule[i].first]->chunk_set[to_schedule[i].second].second;
            if (node->node_chunk.count(current_chunk) || node->node_chunk.size() < B)
            {
                vm_scheduling_sjf(node, to_schedule[i], time_slot_req, d, job_set_sjf_copy);
            }
        }
        int time_slot_req = job_set_sjf_copy[to_schedule[i].first]->chunk_set[to_schedule[i].second].second;
        while (time_slot_req > 0)
        {
            Node *node = new Node();
            cout << "Created new node" << endl;
            cout << job_set_sjf[to_schedule[i].first]->chunk_set[to_schedule[i].second].second << ": time dekho ab" << endl;
            vm_scheduling_sjf(node, to_schedule[i], time_slot_req, d, job_set_sjf_copy);
            cout << "Scheduled as much as possible" << endl;
            cout << job_set_sjf[to_schedule[i].first]->chunk_set[to_schedule[i].second].second << ": time dekho ab" << endl;
            time_slot_req = job_set_sjf_copy[to_schedule[i].first]->chunk_set[to_schedule[i].second].second;
            active_nodes_sjf_copy.push_back(node);
            extra_nodes++;
        }
    }
    return extra_nodes;
}

void vm_scheduling_sjf_real(Node *node, pair<int, int> chunk, int time_req, int d)
{
    int current_chunk = job_set_sjf[chunk.first]->chunk_set[chunk.second].first->id;

    for (int t = 0; t < d; t++)
    {
        if (time_req == 0)
            break;

        if (node->vm_timeslot_chunks[t].count(current_chunk))
            continue;
        for (int m = 0; m < S; m++)
        {
            if (node->vm_schedule[m][t] != make_pair(-1, -1))
                continue;
            if (time_req == 0)
                break;
            node->vm_timeslot_chunks[t].insert(current_chunk);
            node->vm_schedule[m][t] = chunk;
            node->node_chunk.insert(current_chunk);
            time_req--;
            break;
        }
    }
    job_set_sjf[chunk.first]->chunk_set[chunk.second].second = time_req;
}

void schedule_real(vector<pair<int, int>> to_schedule, int d)
{
    int cur_tot_nodes = active_nodes_sjf.size();
    cout << cur_tot_nodes << "total nodes" << endl;
    for (int i = 0; i < to_schedule.size(); i++)
    {
        int current_chunk = job_set_sjf[to_schedule[i].first]->chunk_set[to_schedule[i].second].first->id;
        cout << "CUrrent chunk id" << current_chunk << endl;
        for (int n = 0; n < active_nodes_sjf.size(); n++)
        {
            Node *node = active_nodes_sjf[n];
            int time_slot_req = job_set_sjf[to_schedule[i].first]->chunk_set[to_schedule[i].second].second;
            if (node->node_chunk.count(current_chunk) || node->node_chunk.size() < B)
            {
                vm_scheduling_sjf_real(node, to_schedule[i], time_slot_req, d);
            }
        }
        int time_slot_req = job_set_sjf[to_schedule[i].first]->chunk_set[to_schedule[i].second].second;
        cout << "time jitna chahiye: " << time_slot_req << endl;
        while (time_slot_req > 0)
        {
            cout << time_slot_req << "timeslo" << endl;
            Node *node = new Node();
            cout << "naya created" << endl;
            vm_scheduling_sjf_real(node, to_schedule[i], time_slot_req, d);
            cout << "REAL schedule" << endl;
            time_slot_req = job_set_sjf[to_schedule[i].first]->chunk_set[to_schedule[i].second].second;
            active_nodes_sjf.push_back(node);
        }
    }
}

void print_jobs()
{
    cout << "********************************" << endl;
    for(int i = 0; i < job_set_sjf.size(); i++)
    {
        cout << job_set_sjf[i]->id << " " << job_set_sjf[i]->chunk_set[0].second << endl;
    }
    cout << "********************************" << endl;
}

vector<pair<int, int>> find_chunk_set(Job *job)
{
    vector<pair<int, int>> chunk_set;
    int chunks_len = job->chunk_set.size();
    int job_id = job->id;
    for (int i = 0; i < chunks_len; i++)
    {
        chunk_set.push_back({job_id, i});
    }

    return chunk_set;
}

vector<pair<int, int>> extra_nodes_required(vector<Job *> to_schedule)
{
    int extra = 0;
    vector<pair<int, int>> extra_nodes;
    for (int i = 0; i < to_schedule.size(); i++)
    {
        if(scheduled[i] == 0)
        {
            vector<pair<int, int>> chunk_set = find_chunk_set(to_schedule[i]);
            cout << "Found chunks" << i << endl;
            extra = schedule(chunk_set, to_schedule[i]->deadline);
            extra_nodes.push_back({extra, i});
        }
        else
        {
            extra_nodes.push_back({1e9, i});
        }
    }
    return extra_nodes;
}

int count_not_scheduled()
{
    int count = 0;
    for(int i = 0; i < scheduled.size(); i++)
    {
        if(scheduled[i] == 0) count++;
    }
    return count;
}

void sjf()
{
    for(int i = 0; i < job_set_sjf.size(); i++)
    {
        scheduled.push_back(0);
    }
    cout << "Pahuche" << endl;
    while (count_not_scheduled() > 0)
    {
        // print_jobs();

        vector<pair<int, int>> extra_nodes = extra_nodes_required(job_set_sjf);

        // print_jobs();

        for (int i = 0; i < extra_nodes.size(); i++)
        {
            cout << extra_nodes[i].first << " " << extra_nodes[i].second << endl;
        }

        sort(extra_nodes.begin(), extra_nodes.end());

        // print_jobs();

        cout << "Sprted" << endl;
        vector <pair<int,int>> chunks_milgaye = find_chunk_set(job_set_sjf[extra_nodes[0].second]);

        // print_jobs();

        schedule_real(chunks_milgaye, job_set_sjf[extra_nodes[0].second]->deadline);

        // print_jobs();

        cout << "pURA SCHEDULE DONE" << endl;
        // job_set_sjf.erase(job_set_sjf.begin() + extra_nodes[0].second);

        scheduled[extra_nodes[0].second] = 1;
        // print_jobs();

    }
    cout << "Out of the loop!!!!!!" << endl;
}

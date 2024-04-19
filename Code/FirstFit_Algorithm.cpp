#include "Definitions.cpp" // Including the Definitions file
using namespace std;

// Function to schedule a chunk using First Fit Algorithm
void vm_scheduling_ffa(Node *node, pair<int, int> chunk, int time_req, int d)
{
    // Getting the current chunk ID
    int current_chunk = job_set_ffa[chunk.first]->chunk_set[chunk.second].first->id;

    // Looping through each timeslot
    for (int t = 0; t < d; t++)
    {
        // If time requirement is met, exit the loop
        if (time_req == 0)
            break;

        // If the chunk is already scheduled in the timeslot, continue to the next timeslot
        if (node->vm_timeslot_chunks[t].count(current_chunk))
            continue;

        // Looping through each VM
        for (int m = 0; m < S; m++)
        {
            // If the timeslot is already occupied, continue to the next VM
            if (node->vm_schedule[m][t] != make_pair(-1, -1))
                continue;

            // Assigning the chunk to the timeslot in the VM
            node->vm_timeslot_chunks[t].insert(current_chunk);
            node->vm_schedule[m][t] = chunk;
            node->node_chunk.insert(current_chunk);
            time_req--; // Decrementing the time requirement
            break;      // Exiting the loop after assigning the chunk
        }
    }
    // Updating the time requirement for the chunk
    job_set_ffa[chunk.first]->chunk_set[chunk.second].second = time_req;
}

// Function to schedule chunks using First Fit Algorithm
void schedule_ffa(vector<pair<int, int>> &to_schedule, int d)
{
    int cur_tot_nodes = active_nodes_ffa.size();

    // Looping through each chunk to schedule
    for (int i = 0; i < to_schedule.size(); i++)
    {
        // Getting the current chunk ID
        int current_chunk = job_set_ffa[to_schedule[i].first]->chunk_set[to_schedule[i].second].first->id;

        // Looping through each active node
        for (int n = 0; n < cur_tot_nodes; n++)
        {
            Node *node = active_nodes_ffa[n]; // Getting the current node
            int time_slot_req = job_set_ffa[to_schedule[i].first]->chunk_set[to_schedule[i].second].second; // Getting the time requirement for the chunk

            // If the chunk is already scheduled in the node or the node has available space, schedule the chunk
            if (node->node_chunk.count(current_chunk) || node->node_chunk.size() < B)
            {
                vm_scheduling_ffa(node, to_schedule[i], time_slot_req, d);
            }
        }

        // Handling the case where the chunk cannot fit in any active node
        int time_slot_req = job_set_ffa[to_schedule[i].first]->chunk_set[to_schedule[i].second].second;
        while (time_slot_req > 0)
        {
            Node *node = new Node(); // Creating a new node
            vm_scheduling_ffa(node, to_schedule[i], time_slot_req, d); // Scheduling the chunk in the new node
            time_slot_req = job_set_ffa[to_schedule[i].first]->chunk_set[to_schedule[i].second].second; // Updating the time requirement
            active_nodes_ffa.push_back(node); // Adding the new node to the list of active nodes
        }
    }
}

// Function to find the chunk set of a job
vector<pair<int, int>> find_chunk_set_ffa(Job *job)
{
    vector<pair<int, int>> chunk_set;
    int chunks_len = job->chunk_set.size();
    int job_id = job->id;

    // Creating the chunk set for the job
    for (int i = 0; i < chunks_len; i++)
    {
        chunk_set.push_back({job_id, i});
    }

    return chunk_set;
}

// Function to perform First Fit Algorithm for chunk-based resource-aware scheduling
void first_fit()
{
    int job_num = job_set_ffa.size();

    // Looping through each job
    for (int i = 0; i < job_num; i++)
    {
        Job *job = job_set_ffa[i];
        int d = job->deadline;
        vector<pair<int, int>> to_schedule = find_chunk_set_ffa(job); // Finding the chunk set for the job
        schedule_ffa(to_schedule, d); // Scheduling the chunks using First Fit Algorithm
    }
}

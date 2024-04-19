#include "Definitions.cpp" // Including the Definitions file
using namespace std;

vector<int> scheduled; // Vector to keep track of scheduled jobs

// Function to make a deep copy of the job set using SJF algorithm
vector<Job *> copyJobSetSJF(const vector<Job *> &original)
{
    // Creating a new vector to store copied jobs
    vector<Job *> copiedJobSetSJF;

    // Looping through each job in the original job set
    for (const auto &jobPtr : original)
    {
        // Creating a deep copy of the job
        Job *copiedJob = new Job(*jobPtr);
        vector<pair<Chunk *, int>> copiedChunkSet;

        // Looping through each chunk in the job's chunk set
        for (const auto &chunkPair : jobPtr->chunk_set)
        {
            // Creating a deep copy of the chunk
            Chunk *copiedChunk = new Chunk(*chunkPair.first);
            copiedChunkSet.push_back({copiedChunk, chunkPair.second});
        }

        // Assigning the copied chunk set to the copied job
        copiedJob->chunk_set = copiedChunkSet;
        copiedJobSetSJF.push_back(copiedJob); // Adding the copied job to the copied job set
    }
    return copiedJobSetSJF; // Returning the copied job set
}

// Function to schedule a chunk using SJF algorithm
void vm_scheduling_sjf(Node *node, pair<int, int> chunk, int time_req, int d, vector<Job *> &job_set_sjf_copy)
{
    int current_chunk = job_set_sjf_copy[chunk.first]->chunk_set[chunk.second].first->id;

    // Looping through each timeslot
    for (int t = 0; t < d; t++)
    {
        if (time_req == 0)
        {
            break;
        }

        // If the chunk is already scheduled in the timeslot, continue to the next timeslot
        if (node->vm_timeslot_chunks[t].count(current_chunk))
        {
            continue;
        }

        // Looping through each VM
        for (int m = 0; m < S; m++)
        {
            // If the timeslot is already occupied, continue to the next VM
            if (node->vm_schedule[m][t] != make_pair(-1, -1))
            {
                continue;
            }
            if (time_req == 0)
            {
                break;
            }

            // Assigning the chunk to the timeslot in the VM
            node->vm_timeslot_chunks[t].insert(current_chunk);
            node->vm_schedule[m][t] = chunk;
            node->node_chunk.insert(current_chunk);
            time_req--; // Decrementing the time requirement
            break;      // Exiting the loop after assigning the chunk
        }
    }
    job_set_sjf_copy[chunk.first]->chunk_set[chunk.second].second = time_req; // Updating the time requirement for the chunk
}

// Function to schedule chunks using SJF algorithm
int schedule_sjf(vector<pair<int, int>> to_schedule, int d)
{
    vector<Node *> active_nodes_sjf_copy; // Vector to store copies of active nodes
    int extra_nodes = 0; // Counter for extra nodes required
    vector<Job *> job_set_sjf_copy = copyJobSetSJF(job_set_sjf); // Making a deep copy of the job set

    // Looping through each chunk to schedule
    for (int i = 0; i < to_schedule.size(); i++)
    {
        int current_chunk = job_set_sjf_copy[to_schedule[i].first]->chunk_set[to_schedule[i].second].first->id;

        // Looping through each active node copy
        for (int n = 0; n < active_nodes_sjf_copy.size(); n++)
        {
            // Creating a new node copy
            Node node_new = new Node(active_nodes_sjf_copy[n]);
            Node *node = &node_new;
            int time_slot_req = job_set_sjf_copy[to_schedule[i].first]->chunk_set[to_schedule[i].second].second;

            // If the chunk is already scheduled in the node or the node has available space, schedule the chunk
            if (node->node_chunk.count(current_chunk) || node->node_chunk.size() < B)
            {
                vm_scheduling_sjf(node, to_schedule[i], time_slot_req, d, job_set_sjf_copy);
            }
        }

        // Handling the case where the chunk cannot fit in any active node
        int time_slot_req = job_set_sjf_copy[to_schedule[i].first]->chunk_set[to_schedule[i].second].second;
        while (time_slot_req > 0)
        {
            Node *node = new Node(); // Creating a new node
            vm_scheduling_sjf(node, to_schedule[i], time_slot_req, d, job_set_sjf_copy); // Scheduling the chunk in the new node
            time_slot_req = job_set_sjf_copy[to_schedule[i].first]->chunk_set[to_schedule[i].second].second; // Updating the time requirement
            active_nodes_sjf_copy.push_back(node); // Adding the new node to the list of active nodes
            extra_nodes++; // Incrementing the extra nodes counter
        }
    }
    return extra_nodes; // Returning the number of extra nodes required
}

// Function to schedule a chunk using SJF algorithm in the actual scenario
void vm_scheduling_sjf_real(Node *node, pair<int, int> chunk, int time_req, int d)
{
    int current_chunk = job_set_sjf[chunk.first]->chunk_set[chunk.second].first->id;

    // Looping through each timeslot
    for (int t = 0; t < d; t++)
    {
        if (time_req == 0)
        {
            break;
        }

        // If the chunk is already scheduled in the timeslot, continue to the next timeslot
        if (node->vm_timeslot_chunks[t].count(current_chunk))
        {
            continue;
        }

        // Looping through each VM
        for (int m = 0; m < S; m++)
        {
            // If the timeslot is already occupied, continue to the next VM
            if (node->vm_schedule[m][t] != make_pair(-1, -1))
            {
                continue;
            }
            if (time_req == 0)
            {
                break;
            }

            // Assigning the chunk to the timeslot in the VM
            node->vm_timeslot_chunks[t].insert(current_chunk);
            node->vm_schedule[m][t] = chunk;
            node->node_chunk.insert(current_chunk);
            time_req--; // Decrementing the time requirement
            break;      // Exiting the loop after assigning the chunk
        }
    }
    job_set_sjf[chunk.first]->chunk_set[chunk.second].second = time_req; // Updating the time requirement for the chunk
}

// Function to schedule chunks using SJF algorithm in the actual scenario
void schedule_real(vector<pair<int, int>> to_schedule, int d)
{
    int cur_tot_nodes = active_nodes_sjf.size();

    // Looping through each chunk to schedule
    for (int i = 0; i < to_schedule.size(); i++)
    {
        int current_chunk = job_set_sjf[to_schedule[i].first]->chunk_set[to_schedule[i].second].first->id;

        // Looping through each active node
        for (int n = 0; n < active_nodes_sjf.size(); n++)
        {
            Node *node = active_nodes_sjf[n];
            int time_slot_req = job_set_sjf[to_schedule[i].first]->chunk_set[to_schedule[i].second].second;

            // If the chunk is already scheduled in the node or the node has available space, schedule the chunk
            if (node->node_chunk.count(current_chunk) || node->node_chunk.size() < B)
            {
                vm_scheduling_sjf_real(node, to_schedule[i], time_slot_req, d);
            }
        }

        // Handling the case where the chunk cannot fit in any active node
        int time_slot_req = job_set_sjf[to_schedule[i].first]->chunk_set[to_schedule[i].second].second;
        while (time_slot_req > 0)
        {
            Node *node = new Node(); // Creating a new node
            vm_scheduling_sjf_real(node, to_schedule[i], time_slot_req, d); // Scheduling the chunk in the new node
            time_slot_req = job_set_sjf[to_schedule[i].first]->chunk_set[to_schedule[i].second].second; // Updating the time requirement
            active_nodes_sjf.push_back(node); // Adding the new node to the list of active nodes
        }
    }
}

// Function to find the chunk set of a job
vector<pair<int, int>> find_chunk_set_sjf(Job *job)
{
    vector<pair<int, int>> chunk_set;
    int chunks_len = job->chunk_set.size();
    int job_id = job->id;

    // Creating the chunk set for the job
    for (int i = 0; i < chunks_len; i++)
    {
        chunk_set.push_back({job_id, i});
    }
    return chunk_set; // Returning the chunk set
}

// Function to calculate the extra nodes required for scheduling
vector<pair<int, int>> extra_nodes_required(vector<Job *> to_schedule)
{
    int extra = 0; // Counter for extra nodes required
    vector<pair<int, int>> extra_nodes; // Vector to store the number of extra nodes required for each job

    // Looping through each job to be scheduled
    for (int i = 0; i < to_schedule.size(); i++)
    {
        // If the job is not yet scheduled
        if (scheduled[i] == 0)
        {
            // Finding the chunk set for the job
            vector<pair<int, int>> chunk_set = find_chunk_set_sjf(to_schedule[i]);
            // Calculating the extra nodes required for scheduling
            extra = schedule_sjf(chunk_set, to_schedule[i]->deadline);
            extra_nodes.push_back({extra, i}); // Adding the number of extra nodes required to the vector
        }
        else
        {
            extra_nodes.push_back({1e9, i}); // If the job is already scheduled, adding a large value to indicate no extra nodes required
        }
    }
    return extra_nodes; // Returning the vector containing the number of extra nodes required for each job
}

// Function to count the number of jobs that are not yet scheduled
int count_not_scheduled()
{
    int count = 0;
    // Looping through the scheduled vector
    for (int i = 0; i < scheduled.size(); i++)
    {
        // If the job is not yet scheduled, increment the count
        if (scheduled[i] == 0)
            count++;
    }
    return count; // Returning the count of jobs not yet scheduled
}

// Function to implement SJF algorithm for scheduling
void sjf()
{
    // Initializing the scheduled vector
    for (int i = 0; i < job_set_sjf.size(); i++)
    {
        scheduled.push_back(0);
    }

    // Looping until all jobs are scheduled
    while (count_not_scheduled() > 0)
    {
        // Finding the extra nodes required for scheduling each job
        vector<pair<int, int>> extra_nodes = extra_nodes_required(job_set_sjf);

        // Sorting the extra nodes vector
        sort(extra_nodes.begin(), extra_nodes.end());

        // Finding the chunk set for the job with the minimum extra nodes required
        vector<pair<int, int>> chunks_milgaye = find_chunk_set_sjf(job_set_sjf[extra_nodes[0].second]);

        // Scheduling the chunks
        schedule_real(chunks_milgaye, job_set_sjf[extra_nodes[0].second]->deadline);

        // Marking the scheduled job as completed
        scheduled[extra_nodes[0].second] = 1;
    }
}

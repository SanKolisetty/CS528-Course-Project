#include "Definitions.cpp"
using namespace std;

vector<int> scheduled;

vector<Job *> copyJobSetSJF(const vector<Job *> &original)
{
    vector<Job *> copiedJobSetSJF;
    for (const auto &jobPtr : original)
    {
        Job *copiedJob = new Job(*jobPtr);
        vector<pair<Chunk *, int>> copiedChunkSet;

        for (const auto &chunkPair : jobPtr->chunk_set)
        {
            Chunk *copiedChunk = new Chunk(*chunkPair.first);
            copiedChunkSet.push_back({copiedChunk, chunkPair.second});
        }

        copiedJob->chunk_set = copiedChunkSet;
        copiedJobSetSJF.push_back(copiedJob);
    }
    return copiedJobSetSJF;
}

void vm_scheduling_sjf(Node *node, pair<int, int> chunk, int time_req, int d, vector<Job *> &job_set_sjf_copy)
{
    int current_chunk = job_set_sjf_copy[chunk.first]->chunk_set[chunk.second].first->id;
    for (int t = 0; t < d; t++)
    {
        if (time_req == 0)
        {
            break;
        }

        if (node->vm_timeslot_chunks[t].count(current_chunk))
        {
            continue;
        }

        for (int m = 0; m < S; m++)
        {
            if (node->vm_schedule[m][t] != make_pair(-1, -1))
            {
                continue;
            }
            if (time_req == 0)
            {
                break;
            }
            node->vm_timeslot_chunks[t].insert(current_chunk);
            node->vm_schedule[m][t] = chunk;
            node->node_chunk.insert(current_chunk);
            time_req--;
            break;
        }
    }
    job_set_sjf_copy[chunk.first]->chunk_set[chunk.second].second = time_req;
}

int schedule(vector<pair<int, int>> to_schedule, int d)
{
    vector<Node *> active_nodes_sjf_copy;
    int extra_nodes = 0;
    vector<Job *> job_set_sjf_copy = copyJobSetSJF(job_set_sjf);

    for (int i = 0; i < to_schedule.size(); i++)
    {
        int current_chunk = job_set_sjf_copy[to_schedule[i].first]->chunk_set[to_schedule[i].second].first->id;

        for (int n = 0; n < active_nodes_sjf_copy.size(); n++)
        {
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
            vm_scheduling_sjf(node, to_schedule[i], time_slot_req, d, job_set_sjf_copy);
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

    for (int i = 0; i < to_schedule.size(); i++)
    {
        int current_chunk = job_set_sjf[to_schedule[i].first]->chunk_set[to_schedule[i].second].first->id;

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

        while (time_slot_req > 0)
        {
            Node *node = new Node();
            vm_scheduling_sjf_real(node, to_schedule[i], time_slot_req, d);
            time_slot_req = job_set_sjf[to_schedule[i].first]->chunk_set[to_schedule[i].second].second;
            active_nodes_sjf.push_back(node);
        }
    }
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
        if (scheduled[i] == 0)
        {
            vector<pair<int, int>> chunk_set = find_chunk_set(to_schedule[i]);
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
    for (int i = 0; i < scheduled.size(); i++)
    {
        if (scheduled[i] == 0)
            count++;
    }
    return count;
}

void sjf()
{
    for (int i = 0; i < job_set_sjf.size(); i++)
    {
        scheduled.push_back(0);
    }

    while (count_not_scheduled() > 0)
    {
        vector<pair<int, int>> extra_nodes = extra_nodes_required(job_set_sjf);

        sort(extra_nodes.begin(), extra_nodes.end());

        vector<pair<int, int>> chunks_milgaye = find_chunk_set(job_set_sjf[extra_nodes[0].second]);

        schedule_real(chunks_milgaye, job_set_sjf[extra_nodes[0].second]->deadline);

        scheduled[extra_nodes[0].second] = 1;
    }
}

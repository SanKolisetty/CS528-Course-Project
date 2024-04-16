#include "Definitions.cpp"
using namespace std;

void vm_scheduling_ffa(Node *node, pair<int, int> chunk, int time_req, int d)
{

    int current_chunk = job_set[chunk.first]->chunk_set[chunk.second].first->id;

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
    job_set[chunk.first]->chunk_set[chunk.second].second = time_req;
}

void schedule_ffa(vector<pair<int, int>> &to_schedule, int d)
{
    int cur_tot_nodes = active_nodes_ffa.size();

    for (int i = 0; i < to_schedule.size(); i++)
    {
        int current_chunk = job_set_ffa[to_schedule[i].first]->chunk_set[to_schedule[i].second].first->id;
        for (int n = 0; n < cur_tot_nodes; n++)
        {
            Node *node = active_nodes_ffa[n];
            int time_slot_req = job_set_ffa[to_schedule[i].first]->chunk_set[to_schedule[i].second].second;
            if (node->node_chunk.count(current_chunk) || node->node_chunk.size() < B)
            {
                vm_scheduling_ffa(node, to_schedule[i], time_slot_req, d);
            }
        }
        int time_slot_req = job_set_ffa[to_schedule[i].first]->chunk_set[to_schedule[i].second].second;
        while (time_slot_req > 0)
        {
            Node *node = new Node();
            vm_scheduling_ffa(node, to_schedule[i], time_slot_req, d);
            time_slot_req = job_set_ffa[to_schedule[i].first]->chunk_set[to_schedule[i].second].second;
            active_nodes_ffa.push_back(node);
        }

    }
}

vector<pair<int, int>> find_chunk_set_ffa(Job *job)
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

void first_fit()
{
    int job_num = job_set_ffa.size();

    for (int i = 0; i < job_num; i++)
    {
        Job *job = job_set_ffa[i];
        int d = job->deadline;
        vector<pair<int, int>> to_schedule = find_chunk_set_ffa(job);
        schedule_ffa(to_schedule, d);
    }
}
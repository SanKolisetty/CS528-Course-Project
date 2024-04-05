#include "Definitions.cpp"
using namespace std;

bool comparator(pair<Chunk, int> a, pair<Chunk, int> b)
{
    if (a.second < b.second)
        return false;
    else
        return true;
}

vector<pair<int,int>> calculate_chunk_sum(vector <pair<int,int>> to_schedule)
{
    map <Chunk,int> total_time;

    for(int i = 0; i < to_schedule.size(); i++)
    {
        total_time[job_set[to_schedule[i].first].chunk_set[to_schedule[i].second].first] += job_set[to_schedule[i].first].chunk_set[to_schedule[i].second].second;
    }

    vector <pair<Chunk,int>> chunks_time;
    
    for(auto it : total_time)
    {
        chunks_time.push_back({it.first, it.second});
    }

    sort(chunks_time.begin(), chunks_time.end(), comparator);

    vector <pair<int,int>> temp;

    for(int i = 0; i < chunks_time.size(); i++)
    {
        for(int j = 0; j < to_schedule.size(); j++)
        {
            if(job_set[to_schedule[j].first].chunk_set[to_schedule[j].second].first.id == chunks_time[i].first.id)
            {
                temp.push_back(to_schedule[j]);
            }
        }
    }
    return temp;
}

int sum_timeslots(vector<pair<Chunk, int>> to_schedule, int i, int j)
{
    int sum = 0;
    for (int k = i; k <= min(j, int(to_schedule.size() - 1)); k++)
    {
        sum += to_schedule[k].second;
    }
    return sum;
}

pair<int, int> finding_tail(vector<pair<Chunk, int>> to_schedule, int d)
{
    int b = min(B, int(to_schedule.size()));
    int current_sum = 0;
    for (int i = to_schedule.size() - 1; i >= 0; i--)
    {
        current_sum = sum_timeslots(to_schedule, i - b + 1, i);
        if (current_sum > S * d)
        {
            return {i - b + 1, i};
        }
    }
}

int remaining_timeslots(Node *node, int d)
{
    int total_timeslots = 0;
    for (int i = 0; i < S; i++)
    {
        for (int j = 0; j < d; j++)
        {
            if (node->vm_schedule[i][j] == make_pair(-1, -1))
            {
                total_timeslots++;
            }
        }
    }
    return total_timeslots;
}

void remove_chunk(vector<pair<Chunk, int>> &to_schedule)
{
    for (int i = 0; i < to_schedule.size(); i++)
    {
        if (to_schedule[i].second == 0)
        {
            to_schedule.erase(to_schedule.begin() + i);
            i--;
        }
    }
}


void allot_slots(Node * node ,Job * job, Chunk chunk , int d,int time_slot_req)
{
    for(int i=0;i<S;i++)
    {
        for(int j = 0;j<d;j++)
        {
            if (node->vm_schedule[i][j] == make_pair(-1, -1))
            {
                // total_timeslots++;
            }
        }
    }
}

int cred(vector<pair<int, int>> &to_schedule, int d)
{
    int total_nodes = 0;
    while (to_schedule.size() > 0)
    {
        // sort(to_schedule.begin(), to_schedule.end(), comparator);
        // if (sum_timeslots(to_schedule, 0, B - 1) > S * d)
        // {
        //     pair<int, int> tail_indices = finding_tail(to_schedule, d);
        //     // schedule(tail_indices.first, tail_indices.second, d);
        //     total_nodes++;
        // }
        // else
        // {
        //     break;
        // }
    }
    while (to_schedule.size() > 0)
    {
        // schedule(0, min(B-1, int(to_schedule.size()-1)), d);
        total_nodes++;
    }
}

void schedule(Node *node, vector<pair<Chunk, int>> &to_schedule, int d)
{
    int NTS = remaining_timeslots(node, d);
    reverse(to_schedule.begin(), to_schedule.end());

    for (int i = 0; i < to_schedule.size(); i++)
    {
        Chunk current_chunk = to_schedule[i].first;
        int time_slot_req = to_schedule[i].second;

        if (node->node_chunk.count(current_chunk) || node->node_chunk.size() < B)
        {
            node->node_chunk.insert(current_chunk);
            if (time_slot_req > NTS)
            {
                time_slot_req = time_slot_req - NTS;
                NTS = 0;
                to_schedule[i].second = time_slot_req;
                break;
            }
            else
            {
                NTS -= time_slot_req;
                to_schedule[i].second = 0;
            }
        }
    }

    remove_chunk(to_schedule);
}

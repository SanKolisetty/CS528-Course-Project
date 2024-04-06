#include "Definitions.cpp"
using namespace std;

bool comparator(pair<Chunk, int> a, pair<Chunk, int> b)
{
    if (a.second < b.second)
        return false;
    else
        return true;
}

vector<pair<int, int>> calculate_chunk_sum(vector<pair<int, int>> to_schedule)
{
    map<Chunk, int> total_time;

    for (int i = 0; i < to_schedule.size(); i++)
    {
        total_time[job_set[to_schedule[i].first].chunk_set[to_schedule[i].second].first] += job_set[to_schedule[i].first].chunk_set[to_schedule[i].second].second;
    }

    vector<pair<Chunk, int>> chunks_time;

    for (auto it : total_time)
    {
        chunks_time.push_back({it.first, it.second});
    }

    sort(chunks_time.begin(), chunks_time.end(), comparator);

    vector<pair<int, int>> temp;

    for (int i = 0; i < chunks_time.size(); i++)
    {
        for (int j = 0; j < to_schedule.size(); j++)
        {
            if (job_set[to_schedule[j].first].chunk_set[to_schedule[j].second].first.id == chunks_time[i].first.id)
            {
                temp.push_back(to_schedule[j]);
            }
        }
    }
    return temp;
}

int sum_timeslots(vector<pair<int, int>> to_schedule, int i, int j)
{
    int sum = 0;
    for (int k = i; k <= min(j, int(to_schedule.size() - 1)); k++)
    {
        sum += job_set[to_schedule[i].first].chunk_set[to_schedule[i].second].second;
    }
    return sum;
}

pair<int, int> finding_tail(vector<pair<int, int>> to_schedule, int d)
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

void remove_chunk(vector<pair<int, int>> &to_schedule)
{
    for (int i = 0; i < to_schedule.size(); i++)
    {
        if (job_set[to_schedule[i].first].chunk_set[to_schedule[i].second].second == 0)
        {
            to_schedule.erase(to_schedule.begin() + i);
            i--;
        }
    }
}

void allot_slots(Node *node, Job *job, Chunk chunk, int d, int time_slot_req)
{
    for (int i = 0; i < S; i++)
    {
        for (int j = 0; j < d; j++)
        {
            if (node->vm_schedule[i][j] == make_pair(-1, -1))
            {
                // total_timeslots++;
            }
        }
    }
}

void schedule(Node *node, vector<pair<int, int>> &to_schedule, int d)
{
    int NTS = remaining_timeslots(node, d);
    reverse(to_schedule.begin(), to_schedule.end());

    for (int i = 0; i < to_schedule.size(); i++)
    {
        Chunk current_chunk = job_set[to_schedule[i].first].chunk_set[to_schedule[i].second].first;
        int time_slot_req = job_set[to_schedule[i].first].chunk_set[to_schedule[i].second].second;

        if (node->node_chunk.count(current_chunk) || node->node_chunk.size() < B)
        {
            node->node_chunk.insert(current_chunk);
            if (time_slot_req > NTS)
            {
                time_slot_req -= NTS;
                NTS = 0;
                job_set[to_schedule[i].first].chunk_set[to_schedule[i].second].second = time_slot_req;
                break;
            }
            else
            {
                NTS -= time_slot_req;
                job_set[to_schedule[i].first].chunk_set[to_schedule[i].second].second = 0;
            }
        }
    }
    remove_chunk(to_schedule);
}

void delete_indices(vector <pair<int,int>> &schedule, int i, int j)
{
    for(int k = i; k <= j; k++)
    {
        schedule.erase(schedule.begin()+k);
    }
}

int cred_s(vector<pair<int, int>> &to_schedule, int d)
{
    int total_nodes = 0;
    while (to_schedule.size() > 0)
    {
        to_schedule = calculate_chunk_sum(to_schedule);
        if (sum_timeslots(to_schedule, 0, B - 1) > S * d)
        {
            // Finding the tail and creating a list for scheduling
            pair<int, int> tail_indices = finding_tail(to_schedule, d);
            vector <pair<int,int>> tail_to_schedule(tail_indices.second - tail_indices.first+1);
            copy(to_schedule.begin() + tail_indices.first, to_schedule.begin() + tail_indices.second+1, tail_to_schedule.begin());

            // Delete the chunks sent for scheduling from the main list of chunks
            delete_indices(to_schedule,tail_indices.first, tail_indices.second);

            // Declare the node
            Node * node = new Node();
            
            // Schedule the node
            schedule(node, tail_to_schedule, d);

            // Increment and push the node
            total_nodes++;
            active_nodes.push_back(node);
            nodes_created[d].push_back(node);

            // Add the chunks which were not scheduled back to main list 
            for(int i = 0; i < tail_to_schedule.size(); i++)
            {
                to_schedule.push_back(tail_to_schedule[i]);
            }
        }
        else
        {
            break;
        }
    }
    while (to_schedule.size() > 0)
    {
        // Node * node = new Node();
        // schedule(node, to_schedule, d);
        // total_nodes++;
        // active_nodes.push_back(node);
        // nodes_created[d].push_back(node);

        // Finding the tail and creating a list for scheduling
        vector <pair<int,int>> tail_to_schedule(min(B, int(to_schedule.size())));
        copy(to_schedule.begin(), to_schedule.begin() + min(B, int(to_schedule.size())), tail_to_schedule.begin());

        // Delete the chunks sent for scheduling from the main list of chunks
        delete_indices(to_schedule,0, min(B, int(to_schedule.size()))-1);

        // Declare the node
        Node * node = new Node();
        
        // Schedule the node
        schedule(node, tail_to_schedule, d);

        // Increment and push the node
        total_nodes++;
        active_nodes.push_back(node);
        nodes_created[d].push_back(node);

        // Add the chunks which were not scheduled back to main list 
        for(int i = 0; i < tail_to_schedule.size(); i++)
        {
            to_schedule.push_back(tail_to_schedule[i]);
        }
    }
}

vector <pair<int,int>> find_jobs(int d)
{
    vector <pair<int,int>> jobs_deadline;
    for(int i = 0; i < job_set.size(); i++)
    {
        if(job_set[i].deadline == d)
        {
            for(int j = 0; j < job_set[i].chunk_set.size(); j++)
            {
                if(job_set[i].chunk_set[j].second != 0)
                {
                    jobs_deadline.push_back({i,j});
                }
            }
        }
    }
    return jobs_deadline;
}

int cred_m()
{
    int total_active_nodes = 0;
    int current_deadline_nodes = 0;
    for(auto it = deadlines.begin(); it != deadlines.end(); it++)
    {
        vector <pair<int,int>> jobs_given_deadline = find_jobs(*it);
        current_deadline_nodes = cred_s(jobs_given_deadline,*it);
        for(int i = 0; i < nodes_created[*it].size(); i++)
        {   
            for(auto it1 = it; it1 != deadlines.end(); it++)
            {
                vector <pair<int,int>> jobs_new_deadline = find_jobs(*it1);
                schedule(nodes_created[*it][i], jobs_new_deadline, *it1);
            }
        }
        total_active_nodes += current_deadline_nodes;
    }
}

#include "Definitions.cpp"
using namespace std;
bool comparator(pair<int, int> a, pair<int, int> b)
{
    if (a.second < b.second)
        return false;
    else
        return true;
}

// vector<pair<int, int>> calculate_chunk_sum(vector<pair<int, int>> to_schedule)
// {
//     map<int, int> total_time;

//     for (int i = 0; i < to_schedule.size(); i++)
//     {
//         total_time[job_set[to_schedule[i].first]->chunk_set[to_schedule[i].second].first->id] += job_set[to_schedule[i].first]->chunk_set[to_schedule[i].second].second;
//     }
    
//     // //if(d1 ==  13)
//     // //{
//     // //    cout <<"Mai idhar hu"<<endl;
//     // //}

//     vector<pair<int, int>> chunks_time;

//     for (auto it : total_time)
//     {
//         chunks_time.push_back({it.first, it.second});
//     }

//     // //if(d1 ==  13)
//     // //{
//     // //    cout <<"Mai idhar hu"<<endl;
//     // //}
//     sort(chunks_time.begin(), chunks_time.end(), comparator);

//     //if(d1 ==  13)
//     //{
//     //    cout <<"Mai idhar hu"<<endl;
//     //}

//     int tot_jobs = to_schedule.size();
//     int tot_chunks = chunks_time.size();
     
//     vector<pair<int, int>> temp (tot_jobs);

//     //if(d1 ==  13)
//     //{
//     //    cout <<"Mai yaha hu"<<endl;
//     //}
//     int idx = 0;
   
    
//     // //if(d1 ==  13)
//     // //{
//     // //    cout <<"Mai idhar hu"<<endl;
//     // //}
//     for (int i = 0; i < tot_chunks; i++)
//     {
//         for (int j = 0; j < tot_jobs; j++)
//         {
//             if (job_set[to_schedule[j].first]->chunk_set[to_schedule[j].second].first->id == chunks_time[i].first)
//             {
//                 // temp.push_back(to_schedule[j]);
//                 temp[idx++] = to_schedule[j];
//             }
//         }
//     }
//     // if(d ==  13)
//     // {
//     //     cout <<"Mai yaha hu"<<endl;
//     // }
//     return temp;
// }

void calculate_chunk_sum(vector<pair<int, int>> &to_schedule)
{
    map<int, int> total_time;

    for (int i = 0; i < to_schedule.size(); i++)
    {
        total_time[job_set[to_schedule[i].first]->chunk_set[to_schedule[i].second].first->id] += job_set[to_schedule[i].first]->chunk_set[to_schedule[i].second].second;
    }
    

    vector<pair<int, int>> chunks_time;

    for (auto it : total_time)
    {
        chunks_time.push_back({it.first, it.second});
    }

    total_time.clear();

    sort(chunks_time.begin(), chunks_time.end(), comparator);


    int tot_jobs = to_schedule.size();
    int tot_chunks = chunks_time.size();
     
    // vector<pair<int, int>> temp (tot_jobs);      
    vector<pair<int, int>> temp ;
   
    int idx = 0;

    for (int i = 0; i < tot_chunks; i++)
    {
        for (int j = 0; j < tot_jobs; j++)
        {
            if (job_set[to_schedule[j].first]->chunk_set[to_schedule[j].second].first->id == chunks_time[i].first)
            {
                temp.push_back(to_schedule[j]);
            }
        }
    }

    to_schedule = temp;

}

int sum_timeslots_head(vector<pair<int, int>> to_schedule, int d)
{
    set<int> chunks;
    int sum = 0;

    for (int i = 0; i < to_schedule.size(); i++)
    {
        chunks.insert(job_set[to_schedule[i].first]->chunk_set[to_schedule[i].second].first->id);
        if (chunks.size() <= B)
        {
            sum += min(job_set[to_schedule[i].first]->chunk_set[to_schedule[i].second].second,d);
        }
        else
        {
            break;
        }
    }

    return sum;
}

set<int> sum_timeslots_tail(vector<pair<int, int>> to_schedule, int d)
{
    map<int, int> total_time;

    for (int i = 0; i < to_schedule.size(); i++)
    {
        total_time[job_set[to_schedule[i].first]->chunk_set[to_schedule[i].second].first->id] += min(job_set[to_schedule[i].first]->chunk_set[to_schedule[i].second].second,d);
    }

    set<pair<int, int>> chunks;
    set<int> chunk_ids;
    int sum = 0;

    for (auto it : total_time)
    {
        chunks.insert({it.second, it.first});
    }

    auto it = chunks.begin();
    auto start_it = chunks.begin();

    while (chunk_ids.size() < B && it != chunks.end())
    {
        chunk_ids.insert(it->second);
        sum += it->first;
        it++;
    }

    if (chunks.size() < B)
    {
        return chunk_ids;
    }

    while (sum < S * d)
    {
        int id = start_it->second;
        int time_left = start_it->first;

        chunk_ids.erase(id);
        sum -= time_left;

        chunk_ids.insert(it->second);
        sum += it->first;

        it++;
        start_it++;
    }

    return chunk_ids;
}

vector<pair<int, int>> finding_tail(vector<pair<int, int>> to_schedule, set<int> chunk_ids)
{
    vector<pair<int, int>> tail_chunks;
    for (int i = 0; i < to_schedule.size(); i++)
    {
        if (chunk_ids.count(job_set[to_schedule[i].first]->chunk_set[to_schedule[i].second].first->id))
        {
            tail_chunks.push_back(to_schedule[i]);
        }
    }
    return tail_chunks;
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
        if (job_set[to_schedule[i].first]->chunk_set[to_schedule[i].second].second == 0)
        {
            to_schedule.erase(to_schedule.begin() + i);
            i--;
        }
    }
}

void vm_scheduling(Node *node, pair<int, int> chunk, int time_req, int d)
{
    int current_chunk = job_set[chunk.first]->chunk_set[chunk.second].first->id;

    for(int t=0;t<d;t++)
    {
        if(node->vm_timeslot_chunks[t].count(current_chunk))continue;
        for(int m=0;m<S;m++)
        {
            if(node->vm_schedule[m][t] != make_pair(-1,-1)) continue;
            if(time_req==0) break;
            node->vm_timeslot_chunks[t].insert(current_chunk);
            node->vm_schedule[m][t] = chunk;
            node->node_chunk.insert(current_chunk);
            time_req--;
            break;
        }
            if(time_req==0) break;
    }
    job_set[chunk.first]->chunk_set[chunk.second].second = time_req;
}

void schedule(Node *node, vector<pair<int, int>> &tail_chunks, int d)
{
    //int NTS = remaining_timeslots(node, d);
    // cout<< "NTS : "<<NTS<<endl;
    // if(d ==  13)
    // {
    //     cout <<"Mai yaha hu"<<endl;
    // }
    // tail_chunks = calculate_chunk_sum(tail_chunks);
    // tail_chunks = calculate_chunk_sum(tail_chunks);
    calculate_chunk_sum(tail_chunks);


    // if(d ==  13)
    // {
    //     cout <<"Mai beech hu"<<endl;
    // }
    reverse(tail_chunks.begin(), tail_chunks.end());
    // if(d ==  13)
    // {
    //     // cout <<"Mai idhar hu"<<endl;
    //    // cout<<"HELLOO "<<endl;
    // }
    // cout<<"HELLOO ";
    // display_toschedule(tail_chunks);

    int tail_size = tail_chunks.size();

    // if(d ==  13)
    // {
    //     // cout <<"Mai idhar hu"<<endl;
    //     //cout<<"HELLOO "<< tail_size << endl;;
    // }

    for (int i = 0; i < tail_size; i++)
    {
        
        int current_chunk = job_set[tail_chunks[i].first]->chunk_set[tail_chunks[i].second].first->id;
        int time_slot_req = job_set[tail_chunks[i].first]->chunk_set[tail_chunks[i].second].second;
        
        if (node->node_chunk.count(current_chunk) || node->node_chunk.size() < B)
        {
            vm_scheduling(node,tail_chunks[i],time_slot_req,d);
        }
       
    }
    remove_chunk(tail_chunks);
}

void delete_indices(vector<pair<int, int>> &schedule, set<int> chunk_ids)
{
    auto it = schedule.begin();
    while (it != schedule.end())
    {
        int id = job_set[it->first]->chunk_set[it->second].first->id;
        if (chunk_ids.count(id))
        {
            schedule.erase(it);
            it--;
        }
        it++;
    }
}

set<int> select_b_chunks(vector<pair<int, int>> &schedule)
{
    set<int> chunk_ids;

    for (int i = 0; i < schedule.size(); i++)
    {
        chunk_ids.insert(job_set[schedule[i].first]->chunk_set[schedule[i].second].first->id);
        if (chunk_ids.size() == B)
        {
            return chunk_ids;
        }
    }
    return chunk_ids;
}

void cred_s(vector<pair<int, int>> &to_schedule, int d)
{
    int total_nodes = 0;

    if(B < S)
    {
        while (to_schedule.size() > 0)
        {
            // to_schedule = calculate_chunk_sum(to_schedule);
            calculate_chunk_sum(to_schedule);

            set<int> chunk_ids = select_b_chunks(to_schedule);
            vector<pair<int, int>> tail_chunks = finding_tail(to_schedule, chunk_ids);

            // Delete the chunks sent for scheduling from the main list of chunks
            delete_indices(to_schedule, chunk_ids);

            // Declare the node
            Node *node = new Node();
            
            // Schedule the node
            schedule(node, tail_chunks, d);

            // Increment and push the node
            total_nodes++;
            active_nodes.push_back(node);
            nodes_created[d].push_back(node);

            // Add the chunks which were not scheduled back to main list
            for (int i = 0; i < tail_chunks.size(); i++)
            {
                to_schedule.push_back(tail_chunks[i]);
            }
        }
    }
    else if(B >= S)
    {
        while (to_schedule.size() > 0)
        {
            // to_schedule = calculate_chunk_sum(to_schedule);
            calculate_chunk_sum(to_schedule);
            
            if (sum_timeslots_head(to_schedule,d) > S * d)
            {
                set<int> chunk_ids = sum_timeslots_tail(to_schedule, d);
                vector<pair<int, int>> tail_chunks = finding_tail(to_schedule, chunk_ids);
                
                // Delete the chunks sent for scheduling from the main list of chunks
                delete_indices(to_schedule, chunk_ids);

                // Declare the node
                Node *node = new Node();
                
                // Schedule the node
                schedule(node, tail_chunks, d);

                // Increment and push the node
                total_nodes++;
                active_nodes.push_back(node);
                nodes_created[d].push_back(node);

                // Add the chunks which were not scheduled back to main list
                for (int i = 0; i < tail_chunks.size(); i++)
                {
                    to_schedule.push_back(tail_chunks[i]);
                }
            }
            else
            {
                while (to_schedule.size() > 0)
                {
                    // to_schedule = calculate_chunk_sum(to_schedule);
                    calculate_chunk_sum(to_schedule);

                    set<int> chunk_ids = select_b_chunks(to_schedule);
                    vector<pair<int, int>> tail_chunks = finding_tail(to_schedule, chunk_ids);

                    // Delete the chunks sent for scheduling from the main list of chunks
                    delete_indices(to_schedule, chunk_ids);

                    // Declare the node
                    Node *node = new Node();
                    
                    // Schedule the node
                    schedule(node, tail_chunks, d);

                    // Increment and push the node
                    total_nodes++;
                    active_nodes.push_back(node);
                    nodes_created[d].push_back(node);

                    // Add the chunks which were not scheduled back to main list
                    for (int i = 0; i < tail_chunks.size(); i++)
                    {
                        to_schedule.push_back(tail_chunks[i]);
                    }
                }
            }
        }
    }
}

vector<pair<int, int>> find_jobs(int d)
{
    vector<pair<int, int>> jobs_deadline;
    for (int i = 0; i < job_set.size(); i++)
    {
        if (job_set[i]->deadline == d)
        {
            for (int j = 0; j < job_set[i]->chunk_set.size(); j++)
            {
                if (job_set[i]->chunk_set[j].second != 0)
                {
                    jobs_deadline.push_back({i, j});
                }
            }
        }
    }
    return jobs_deadline;
}

void cred_m()
{

    for (auto deadline = deadlines.begin(); deadline != deadlines.end(); deadline++)
    {
        int current_deadline = *deadline;
        // cout << current_deadline << endl;

        vector<pair<int, int>> focus_jobs = find_jobs(current_deadline);

        // cout << "Jobs found" << endl;

        cred_s(focus_jobs, current_deadline);

        // cout << "Cred s hua : " << active_nodes.size() << endl;

        for (int n = 0; n < active_nodes.size(); n++)
        {
            auto it = deadline;
            it++;
            for (; it != deadlines.end(); it++)
            {
                focus_jobs = find_jobs(*it);
                // cout<<"Strting Schedule :( " << *it << endl;
                // d1 = *it;
                schedule(active_nodes[n], focus_jobs, *it);
            }
            // cout << "Extra sched hua : " << n << endl;
        }
    }
}
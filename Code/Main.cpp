// #include "Definitions.cpp"
// #include "CRED_Algorithm.cpp"
// #include "FirstFit_Algorithm.cpp"
//#include "ShortestJobFirst_Algorithm.cpp"
#include "ShortestDeadline_Algorithm.cpp"

using namespace std;

int main()
{

    cout << "Enter S,B" << endl;
    S = 1;
    B = 2;
    cin >> S >> B;

    cout << "Total Number of Chunks" << endl;
    C = 5;
    cin >> C;
    cloud_dfs.resize(C);
    for (int i = 0; i < C; i++)
    {
        Chunk *c = new Chunk(i);
        cloud_dfs[i] = c;
    }

    cout << "Total Number of Jobs" << endl;
    J = 2;
    cin >> J;
    job_set.resize(J);
    int processing_time;
    int deadline;
    vector<pair<Chunk *, int>> chunk_set;
    int chunk_set_size;
    int chunk_id;

    for (int i = 0; i < J; i++)
    {
        chunk_set.clear();
        cout << "Enter processing time and deadline" << endl;
        cin >> processing_time >> deadline;

        deadlines.insert(deadline);
        Latest_Deadline = max(Latest_Deadline, deadline);

        cout << "Enter total number of chunks required" << endl;
        cin >> chunk_set_size;

        chunk_set.resize(chunk_set_size);

        for (int j = 0; j < chunk_set_size; j++)
        {
            cout << "Enter Chunk id required : ";
            cin >> chunk_id;
            chunk_set[j] = {cloud_dfs[chunk_id], processing_time};
        }

        Job *job = new Job(i, chunk_set, processing_time, deadline);
        job_set[i] = job;
    }
    job_set_ffa = job_set;
    job_set_sda = job_set;
    job_set_sjf =  job_set;

    // cred_m();
    // Print(active_nodes);

    // first_fit();
    // Print(active_nodes_ffa);

   // sjf();
   // Print(active_nodes_sjf);

    shortest_deadline_first();
    Print(active_nodes_sda);
    /*
     try shortest_deadline_first() function with the following inputs:
        1. S = 1, B = 2, C = 5, J = 3
        2. Processing time and deadline for each job:
            Job 1: 6 2
            Job 2: 3 3
            Job 3: 1 4
        3. Total number of chunks required for each job:
            Job 1: 1
            Job 2: 1
            Job 3: 3
        4. Chunk id required for each job:
            Job 1: 0
            Job 2: 4
            Job 3: 1 2 3

    output:
        Total active nodes required are 6:-

                 |   T1   |    T2    |   T3    |   T4    |
        Node 1   |  J1C1  |   J1C1   |  J2C5   |         |
        Node 2   |  J1C1  |   J1C1   |  J2C5   |         |
        Node 3   |  J1C1  |   J1C1   |  J2C5   |         |
        Node 4   |  J3C2  |          |         |         |
        Node 5   |  J3C3  |          |         |         |
        Node 6   |  J3C4  |          |         |         |

    */
    // cout << "Total active nodes required are " << active_nodes.size() << endl;

    return 0;
}
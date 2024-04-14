// #include "Definitions.cpp"
// #include "CRED_Algorithm.cpp"
// #include "FirstFit_Algorithm.cpp"
#include "ShortestJobFirst_Algorithm.cpp"
using namespace std;

int main()
{

    cout << "Enter S,B" << endl;
    S = 1; B =2;
    cin >> S >> B;

    cout << "Total Number of Chunks" << endl;
    C = 5;
    cin >> C;
    cloud_dfs.resize(C);
    for (int i = 0; i < C; i++)
    {
        Chunk*c =  new Chunk (i);
        cloud_dfs[i] = c;
    }

    cout << "Total Number of Jobs" << endl;
    J = 2;
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

        Job*job =  new Job(i, chunk_set, processing_time, deadline);
        job_set[i] = job;
    }
    // job_set_ffa =  job_set;
    job_set_sjf =  job_set;

    // cred_m();
    // Print(active_nodes);

    // first_fit();
    // Print(active_nodes_ffa);

    sjf();
    Print(active_nodes_sjf);


    // cout << "Total active nodes required are " << active_nodes.size() << endl;

    return 0;
}
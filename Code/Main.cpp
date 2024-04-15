
#include "Definitions.cpp"
#include "CRED_Algorithm.cpp"
#include "FirstFit_Algorithm.cpp"
#include "ShortestJobFirst_Algorithm.cpp"
#include "ShortestDeadline_Algorithm.cpp"
using namespace std;

int main()
{
    // cout << "Enter S,B" << endl;
    // S = 1; B =2;
    // cin >> S >> B;

    // cout << "Total Number of Chunks" << endl;
    // C = 5;
    // cin >> C;
    // cloud_dfs.resize(C);
    // for (int i = 0; i < C; i++)
    // {
    //     Chunk*c =  new Chunk (i);
    //     cloud_dfs[i] = c;
    // }

    // cout << "Total Number of Jobs" << endl;
    // J = 2;
    // cin >> J;
    // job_set.resize(J);
    // int processing_time;
    // int deadline;
    // vector<pair<Chunk*, int>> chunk_set;
    // int chunk_set_size;
    // int chunk_id;

    // for (int i = 0; i < J; i++)
    // {
    //     chunk_set.clear();
    //     cout << "Enter processing time and deadline" << endl;
    //     cin >> processing_time >> deadline;

    //     deadlines.insert(deadline);
    //     Latest_Deadline = max(Latest_Deadline, deadline);

    //     cout << "Enter total number of chunks required"<<endl;
    //     cin >> chunk_set_size;

    //     chunk_set.resize(chunk_set_size);

    //     for (int j = 0; j < chunk_set_size; j++)
    //     {
    //         cout << "Enter Chunk id required : ";
    //         cin >> chunk_id;
    //         chunk_set[j] = {cloud_dfs[chunk_id], processing_time};
    //     }

    //     Job*job =  new Job(i, chunk_set, processing_time, deadline);
    //     job_set[i] = job;
    // }
    // // job_set_ffa =  job_set;
    // // job_set_sda =  job_set;
    //    job_set_sjf =  job_set;

    // cout << "Enter S,B" << endl;
    // S = 1; B =2;
    // cin >> S >> B;

    // cout << "Total Number of Chunks" << endl;
    // C = 5;
    // cin >> C;
    // cloud_dfs.resize(C);
    // for (int i = 0; i < C; i++)
    // {
    //     Chunk*c =  new Chunk (i);
    //     cloud_dfs[i] = c;
    // }

    // FILE *f1 = fopen("./console/input.txt", "r");
    ifstream fin("./console/input.txt");
    // ofstream fout("formatted_input.txt");

    S = 1; B =2;
    // fscanf(f1, "%d %d", &S, &B);
    fin >> S >> B;

    C = 30;
    // fscanf(f1, "%d", &C);
    fin >> C;
    cloud_dfs.resize(C);
    for (int i = 0; i < C; i++)
    {
        Chunk*c =  new Chunk (i);
        cloud_dfs[i] = c;
    }

    J = 2;
    // fscanf(f1, "%d",&J);
    fin >> J;
    job_set.resize(J);
    int processing_time;
    int deadline;
    vector<pair<Chunk*, int>> chunk_set;
    int chunk_set_size;
    int chunk_id;

    for (int i = 0; i < J; i++)
    {
        // cout << "Job " << i << " input starting" << endl;
        chunk_set.clear();
        // fscanf(f1, "%d %d",&processing_time, &deadline);
        fin >> processing_time >> deadline;
        // cout << processing_time << " " << deadline << endl;

        deadlines.insert(deadline);
        Latest_Deadline = max(Latest_Deadline, deadline);

        // fscanf(f1, "%d", &chunk_set_size);
        fin >> chunk_set_size;
        

        chunk_set.resize(chunk_set_size);

        for (int j = 0; j < chunk_set_size; j++)
        {
            // fscanf(f1, "%d", &chunk_id);
                fin >> chunk_id;
            chunk_set[j] = {cloud_dfs[chunk_id], processing_time};
        }

        Job*job =  new Job(i, chunk_set, processing_time, deadline);
        job_set[i] = job;
    }
    job_set_ffa = job_set;
    job_set_sda = job_set;
    job_set_sjf = job_set;

    cout << "Input taken successfully" << endl;
    
    // FILE *fp_cred = fopen("./console/Output_cred.txt", "w");
    // FILE *fp_ffa = fopen("./console/Output_ffa.txt", "w");
    // FILE *fp_sjf = fopen("./console/Output_sjf.txt", "w");
    // FILE *fp_sdf = fopen("./console/Output_sdf.txt", "w");

    // ofstream fp_cred("./console/Output_cred.txt");
    // ofstream fp_ffa("./console/Output_ffa.txt");
    // ofstream fp_sjf("./console/Output_sjf.txt");
    // ofstream fp_sdf("./console/Output_sdf.txt");
    // string x = "aa";
    // ofstream fp_cred("./console/Output_%s.txt",x);

    cred_m();
    Print(active_nodes,"cred");
    cout << "CRED done" << endl;

    first_fit();
    Print(active_nodes_ffa,"ffa");
    cout << "FFA done" << endl;

    // sjf();
    // Print(active_nodes_sjf,"sjf");
    // cout << "SJF done" << endl;

    // shortest_deadline_first();
    // Print(active_nodes_sda,"sdf");
    // cout << "SDA done" << endl;

    // cout << "Total active nodes required are " << active_nodes.size() << endl;

    return 0;
}

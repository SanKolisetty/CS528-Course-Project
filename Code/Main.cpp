#include "Definitions.cpp"
using namespace std;

int main(){

cout << "Enter S,B"<<endl;
cin >> S >> B;

cout<< "Total Number of Chunks"<<endl;
cin >> C;
cloud_dfs.resize(C);
for (int i = 0; i < C; i++)
{
    Chunk c (i);
    cloud_dfs[i] = c;
}

cout<< "Total Number of Jobs"<<endl;
cin >> J;
job_set.resize(J);
int processing_time;
int deadline;
vector <pair<Chunk,int>> chunk_set;
int chunk_set_size;
int chunk_id;

for (int i = 0; i < J; i++)
{   
    chunk_set.clear();
    cout << "Enter processing time and deadline"<<endl;
    cin >> processing_time >> deadline;
    Latest_Deadline = max(Latest_Deadline,deadline);
    cout << "Enter total number of chunks required";
    cin >> chunk_set_size;
    chunk_set.resize(chunk_set_size);
    for (int j = 0; j < chunk_set_size; j++)
    {
        cin >> chunk_id;
        chunk_set[j] = {cloud_dfs[chunk_id],processing_time};
    }

    Job job (i,chunk_set,processing_time,deadline);
    job_set[i] = job;
}

return 0;
}

#include "Definitions.cpp"
#include "CRED_Algorithm.cpp"
#include "FirstFit_Algorithm.cpp"
#include "Greedy_Algorithm.cpp"
#include "ShortestDeadline_Algorithm.cpp"
using namespace std;

int main()
{

    cout << "Do you want to enter input (enter 1) or use file defined input (enter 2)"<<endl;
    int type_of_input; cin >> type_of_input;

    if (type_of_input == 1)
    {
        take_user_input();
    }
    else
    {
        take_file_input();
    }
    
    
    cout << "Input taken successfully" << endl;
    
    cout << "What algorithm do you want to run?"<<endl;
    cout << "Enter 1 for CRED (Chunk-based Resource-aware Energy-efficient Deadline)"<<endl;
    cout << "Enter 2 for First Fit Algorithm"<<endl;
    cout << "Enter 3 for Greedy Algortihm"<<endl;
    cout << "Enter 4 for Shortest Deadline first"<<endl;

    int algo; cin>> algo;

    if(algo == 1)
    {
        cred_m();
        Print(active_nodes,"cred");
        cout << "CRED done, Nodes required are: " << active_nodes.size() << endl;
    }
    if(algo == 2)
    {
        first_fit();
        Print(active_nodes_ffa,"ffa");
        cout << "FFA done, Nodes required are: " << active_nodes_ffa.size() << endl;
    }
    if(algo == 3)
    {
        greedy();
        Print(active_nodes_greedy,"greedy");
        cout << "SJF done, Nodes required are: " << active_nodes_greedy.size() << endl;
    }
    if(algo == 4)
    {
        shortest_deadline_first();
        Print(active_nodes_sda,"sdf");
        cout << "SDA done, Nodes required are: " << active_nodes_sda.size()<< endl;
    }

    // first_fit();
    // Print(active_nodes_ffa,"ffa");
    // cout << "FFA done : " << active_nodes_ffa.size() << endl;

    // sjf();
    // Print(active_nodes_sjf,"sjf");
    // cout << "SJF done : " << active_nodes_sjf.size() << endl;

    // shortest_deadline_first();
    // Print(active_nodes_sda,"sdf");
    // cout << "SDA done : " << active_nodes_sda.size()<< endl;


    return 0;
}

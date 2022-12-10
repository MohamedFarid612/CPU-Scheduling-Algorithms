#include <iostream>
#include <bits/stdc++.h>

using namespace std;

void print_stats(string algo,map<char,pair<int,int>> mp,map<char,pair<int,int>> mp2,vector<char> processes,map<char,int> finish,int n)
{
    cout << fixed << setprecision(2);
    cout<<algo<<endl;
    cout<<"Process    ";
    for(int i=0; i<n; i++)
    {
        cout<<"|  "<<processes[i]<<"  ";

    }
    cout<<"|"<<endl;
    cout<<"Arrival    ";
    for(int i=0; i<n; i++)
    {
        if(mp[processes[i]].first<10)
            cout<<"|  "<<mp[processes[i]].first<<"  ";
        else
            cout<<"| "<<mp[processes[i]].first<<"  ";

    }
    cout<<"|"<<endl;
    cout<<"Service    ";
    for(int i=0; i<n; i++)
    {
        if(to_string(mp2[processes[i]].second).size()<2)
            cout<<"|  "<<mp2[processes[i]].second<<"  ";
        else
            cout<<"| "<<mp2[processes[i]].second<<"  ";


    }
    cout<<"| Mean|"<<endl;
    cout<<"Finish     ";
    for(int i=0; i<n; i++)
    {
        if(to_string(finish[processes[i]]).size()<2)
            cout<<"|  "<<finish[processes[i]]<<"  ";
        else
            cout<<"| "<<finish[processes[i]]<<"  ";

    }
    cout<<"|-----|"<<endl;
    cout<<"Turnaround ";
    int total_turnaround=0;
    for(int i=0; i<n; i++)
    {
        int temp=finish[processes[i]]-mp[processes[i]].first;
        total_turnaround+=temp;
        if(to_string(temp).size()<2)
            cout<<"|  "<<temp<<"  ";
        else
            cout<<"| "<<temp<<"  ";

    }
    double mean=double((1.0*total_turnaround)/n);
    if(mean<10.0)
        cout<<"| "<<mean<<"|"<<endl;
    else
        cout<<"|"<<mean<<"|"<<endl;
    cout<<"NormTurn   ";
    double total_normalized=0;
    for(int i=0; i<n; i++)
    {
        int temp1=finish[processes[i]]-mp[processes[i]].first;
        double temp2=(1.0*temp1)/mp2[processes[i]].second;
        total_normalized+=temp2;
        cout<<"| "<<temp2<<"";
    }
    cout<<"| "<<double(total_normalized/n)<<"|";
    cout<<endl<<endl;
}

void FCFS(string status,int time,int n,map<char,pair<int,int>> mp,vector<char> processes)
{
    map<char,int> finish;
    map<char,pair<int,int>> mp2;
    mp2=mp;
    vector<string> strings(n,"");
    queue<char> q;
    unordered_map<char,bool> waiting;
    for(int i=0; i<n; i++)
    {
        strings[i]+=processes[i];
        strings[i]+="     ";
    }
    string title="FCFS  ";
    string heading="------";
    for(int i=0; i<time; i++)
    {
        heading+="--";
        title+=to_string((i%10))+" ";
        for(int j=0; j<n; j++)
        {
            if(mp[processes[j]].first==i && q.empty())
            {
                q.push(processes[j]);
                strings[j]+="|*";
                mp[processes[j]].second--;
                waiting[processes[j]]=1;
            }
            else if(mp[processes[j]].first==i && !q.empty())
            {
                waiting[processes[j]]=1;
                q.push(processes[j]);
                strings[j]+="|.";
            }
            else if(q.front()==processes[j] && mp[processes[j]].second)
            {
                strings[j]+="|*";
                mp[processes[j]].second--;
            }
            else if(q.front()==processes[j] && !mp[processes[j]].second)
            {
                strings[j]+="| ";
                q.pop();
                waiting[processes[j]]=0;
                finish[processes[j]]=i;
            }
            else if(waiting[processes[j]]==1)
            {
                strings[j]+="|.";
            }
            else
            {
                strings[j]+="| ";
            }
            if(i==time-1)
            {
                if(finish[processes[j]]==0)
                    finish[processes[j]]=i+1;


                strings[j]+="| ";

            }
        }
        if(time-1==i)
        {
            heading+="--";
            title+=to_string(((i+1)%10))+" ";
        }
    }

    if(status=="trace")
    {
        cout<<title<<endl;
        cout<<heading<<endl;
        for(int i=0; i<n; i++)
        {
            cout<<strings[i]<<endl;
        }
        cout<<heading<<endl<<endl;
    }
    else
    {
        print_stats("FCFS",mp,mp2,processes,finish,n);
    }
}

void rr(int volt,string status,int time,int n,map<char,pair<int,int>> mp,vector<char> processes)
{
    map<char,int> finish;
    map<char,pair<int,int>> mp2;
    mp2=mp;
    vector<string> strings(n,"");
    deque<pair<char,int>> q;
    unordered_map<char,bool> waiting;
    for(int i=0; i<n; i++)
    {
        strings[i]+=processes[i];
        strings[i]+="     ";
    }

    string title="RR-"+to_string(volt)+"  ";
    string heading="------";
    bool inserted=false;
    map <char,int> insert;
    for(int i=0; i<time; i++)
    {
        heading+="--";
        title+=to_string((i%10))+" ";
        bool operation=false;
        for(int j=0; j<n; j++)
        {
            if(mp[processes[j]].first==i)
            {
                if(q.empty())
                {
                    if(mp[processes[j]].second>=volt)
                    q.push_back({processes[j],volt});
                    else
                    q.push_back({processes[j],mp[processes[j]].second});

                }
                else if( !insert[q.back().first])
                {
                    if(mp[processes[j]].second>=volt)
                    q.push_back({processes[j],volt});
                    else
                    q.push_back({processes[j],mp[processes[j]].second});
                }
                else
                {
                    pair<char,int> rear=q.back();
                    q.pop_back();
                    if(mp[processes[j]].second>=volt)
                    q.push_back({processes[j],volt});
                    else
                    q.push_back({processes[j],mp[processes[j]].second});

                    q.push_back(rear);
                }
                waiting[processes[j]]=1;
                insert.clear();
                insert[processes[j]]=1;
            }
        }
        insert.clear();
        for(int j=0; j<n; j++)
        {
            if(!q.empty() && q.front().first==processes[j] && operation==false)
            {
                strings[j]+="|*";
                q.front().second--;
                mp[processes[j]].second--;
                if(q.front().second==0)
                {
                    q.pop_front();
                    waiting[processes[j]]=0;
                    if(mp[processes[j]].second!=0)
                    {
                        if(mp[processes[j]].second>volt)
                            q.push_back({processes[j],volt});
                        else
                            q.push_back({processes[j],mp[processes[j]].second});

                        waiting[processes[j]]=1;
                        insert.clear();
                        insert[processes[j]]=1;
                    }
                    else
                    {
                        finish[processes[j]]=i+1;
                    }
                }
                operation=true;
            }
            else if(waiting[processes[j]])
            {
                strings[j]+="|.";
            }
            else
            {
                strings[j]+="| ";
            }
            if(i==time-1)
            {
                if(finish[processes[j]]==0)
                    finish[processes[j]]=i+1;
                strings[j]+="| ";

            }
        }
        if(time-1==i)
        {
            heading+="--";
            title+=to_string(((i+1)%10))+" ";
        }
    }

    if(status=="trace")
    {
        cout<<title<<endl;
        cout<<heading<<endl;
        for(int i=0; i<n; i++)
        {
            cout<<strings[i]<<endl;
        }
        cout<<heading<<endl<<endl;
    }
    else
    {
        print_stats("RR-"+to_string(volt),mp,mp2,processes,finish,n);
    }

}
void SPN(string status,int time,int n,map<char,pair<int,int>> mp,vector<char> processes, map<char,int> service_time)
{
    vector<string> strings(n,"");
    map<char,pair<int,int>> mp2;
    mp2=mp;
    map<char,bool> waiting ;
    map<char,int> finish_time;
    map<char,int> waiting_time;
    set<pair<int,char>> s;


    //we need to save the service time and serving time

    for(int i=0; i<n; i++)
    {
        strings[i]+=processes[i];
        strings[i]+="     ";
    }
    string title="SPN   ";
    string heading="------";
   // iterator front_set;
   char operating;
    for(int i=0;i<time;i++)
    {
        heading+="--";
        title+=to_string((i%10))+" ";
        for(int k=0;k<n;k++)
        {
            if(mp[processes[k]].first==i && s.empty())
            {
                waiting[processes[k]]=1;
                operating=processes[k];
                pair<int,char> extrap;
                extrap.first=mp[processes[k]].second;
                extrap.second=processes[k];
                s.insert(extrap);

            }else if(mp[processes[k]].first==i && !s.empty())
            {
                pair<int,char> extrap;
                extrap.first=mp[processes[k]].second;
                extrap.second=processes[k];
                s.insert(extrap);
                waiting[processes[k]]=1;

            } if (operating==processes[k] && mp[processes[k]].second==0)
            {
                if(mp[processes[k]].second==0)
                {
                    auto front_set=s.begin();
                    while(front_set!=s.end())
                    {
                        if((*front_set).second!=operating)
                        {
                            front_set++;
                        }else{
                            s.erase(front_set);
                            waiting[operating]=0;
                            finish_time[operating]=i;

                            break;
                        }
                    }
                    front_set=s.begin();
                    operating=(*front_set).second;
                }
            }
            if(i==time-1){
                            finish_time[operating]=time;
                        }
        }
        for(int j=0;j<n;j++)
        {
            if(operating==processes[j] && mp[processes[j]].second )
            {
                mp[processes[j]].second-=1;
                strings[j]+="|*";



            }

            else if(waiting[processes[j]]==1)
            {
                strings[j]+="|.";
                waiting_time[processes[j]]+=1;
            }
            else
            {
                strings[j]+="| ";
            }
            if(i==time-1)
            {
                strings[j]+="| ";

            }
        }
        if(time-1==i)
        {
            heading+="--";
            title+=to_string(((i+1)%10))+" ";
        }



        }
    if(status=="trace")
    {cout<<title<<endl;
    cout<<heading<<endl;
    for(int i=0; i<n; i++)
    {
        cout<<strings[i]<<endl;
    }
    cout<<heading<<endl<<endl;
    }else
    {

        print_stats("SPN",mp,mp2,processes,finish_time,n);
    }


}

void HRRN(string status,int time,int n,map<char,pair<int,int>> mp,vector<char> processes, map<char,int> service_time)
{
    vector<string> strings(n,"");
    map<char,pair<int,int>> mp2;
    mp2=mp;
    map<char,bool> waiting ;
    map<char,int> finish_time;
    map<char,int> waiting_time;
    deque<pair<float,char>> s;
    for(int i=0; i<n; i++)
    {
        waiting_time[processes[i]]=0;
    }

    //we need to save the service time and serving time

    for(int i=0; i<n; i++)
    {
        strings[i]+=processes[i];
        strings[i]+="     ";
    }
    string title="HRRN  ";
    string heading="------";
   // iterator front_set;
   char operating;
    for(int i=0;i<time;i++)
    {
        heading+="--";
        title+=to_string((i%10))+" ";
        for(int k=0;k<n;k++)
        {
            if(mp[processes[k]].first==i && s.empty())
            {
                waiting[processes[k]]=1;
                operating=processes[k];
                pair<float,char> extrap;
                float ratio_time=(waiting_time[processes[k]]+mp[processes[k]].second)/mp[processes[k]].second;
                extrap.first=ratio_time;
                extrap.second=processes[k];
                s.push_front(extrap);
                stable_sort(s.begin(),s.end());

            }else if(mp[processes[k]].first==i && !s.empty())
            {
                pair<float,char> extrap;
                float ratio_time=(waiting_time[processes[k]]+mp[processes[k]].second)/mp[processes[k]].second;
                extrap.first=ratio_time;
                extrap.second=processes[k];
                s.push_front(extrap);
                stable_sort(s.begin(),s.end());
                waiting[processes[k]]=1;

            } if (operating==processes[k] && mp[processes[k]].second==0)
            {
                    for(int f=0;f<s.size();f++)
                    {
                        pair<float,char> extraf=s.front();
                        if(extraf.second==operating)
                        {
                            s.pop_front();
                            waiting[operating]=0;
                            finish_time[operating]=i;
                            break;
                        }else{
                            s.pop_front();
                            s.push_back(extraf);
                        }
                    }

                    for(unsigned int y=0;y<s.size();y++)
                    {
                        pair<float,char> extraf=s.front();
                        s.pop_front();
                        float ratio_time=(waiting_time[extraf.second]+mp[extraf.second].second)/mp[extraf.second].second;
                        extraf.first=ratio_time;
                        s.push_back(extraf);

                    }

                    stable_sort(s.begin(),s.end());
                    pair<float,char> extraf=s.back();
                    operating =extraf.second;

            }

            if(i==time-1){
                            finish_time[operating]=time;
                        }
        }

        for(int j=0;j<n;j++)
        {
            if(operating==processes[j] && mp[processes[j]].second )
            {
                mp[processes[j]].second-=1;
                strings[j]+="|*";

            }

            else if(waiting[processes[j]]==1)
            {
                strings[j]+="|.";
                waiting_time[processes[j]]+=1;
            }
            else
            {
                strings[j]+="| ";
            }
            if(i==time-1)
            {
                strings[j]+="| ";

            }
        }
        if(time-1==i)
        {
            heading+="--";
            title+=to_string(((i+1)%10))+" ";
        }

        }
     if(status=="trace")
    {cout<<title<<endl;
    cout<<heading<<endl;
    for(int i=0; i<n; i++)
    {
        cout<<strings[i]<<endl;
    }
    cout<<heading<<endl<<endl;
    }
    else
    {
        print_stats("HRRN",mp,mp2,processes,finish_time,n);

    }


}




void SRT(string status,int time,int n,map<char,pair<int,int>> mp,vector<char> processes, map<char,int> service_time)
{

    vector<string> strings(n,"");
    map<char,pair<int,int>> mp2;
    mp2=mp;
    map<char,bool> waiting ;
    map<char,int> finish_time;
    multimap<int,char> s;
    //we need to save the service time and serving time

    for(int i=0; i<n; i++)
    {
        strings[i]+=processes[i];
        strings[i]+="     ";
    }
    string title="SRT   ";
    string heading="------";
   // iterator front_set;
    for(int i=0; i<time; i++)
    {
        heading+="--";
        title+=to_string((i%10))+" ";
        for(int k=0; k<n; k++)
        {

            if(mp[processes[k]].first==i && s.empty())
            {
                pair<int,char> extrap;
                extrap.first=mp[processes[k]].second;
                extrap.second=processes[k];
                s.insert(extrap);
                waiting[processes[k]]=1;

            }else if(mp[processes[k]].first==i && !s.empty())
            {
                pair<int,char> extrap;
                extrap.first=mp[processes[k]].second;
                extrap.second=processes[k];
                s.insert(extrap);
                waiting[processes[k]]=1;

            }

        }

        for(int j=0; j<n; j++)
        {
            auto front_set=s.begin();
            if((*front_set).second==processes[j])
            {

                mp[processes[j]].second--;
                strings[j]+="|*";

            }

            else if(waiting[processes[j]]==1)
            {
                strings[j]+="|.";
            }
            else
            {
                strings[j]+="| ";
            }


            if(i==time-1)
            {
                strings[j]+="| ";

            }

        }
        for(int k=0;k<n;k++)
        {
            if(!s.empty())
            {
                auto front_set=s.begin();
                if((*front_set).second==processes[k])
                {
                    s.erase(front_set);
                    pair<int,char> extrap;
                    extrap.first=mp[processes[k]].second;
                    extrap.second=processes[k];
                    if(mp[processes[k]].second>0)
                    {
                        s.insert(extrap);
                        finish_time[processes[k]]=i+1;
                    }else{
                        waiting[processes[k]]=0;
                        finish_time[processes[k]]=i+1;
                        if(i==time-1){
                            finish_time[(*front_set).second]=time;
                        }
                    }

                }


            }


        }
        if(time-1==i)
        {
            heading+="--";
            title+=to_string(((i+1)%10))+" ";
        }


    }
    if (status=="trace")
    {cout<<title<<endl;
    cout<<heading<<endl;
    for(int i=0; i<n; i++)
    {
        cout<<strings[i]<<endl;
    }
    cout<<heading<<endl<<endl;
    }
    else
    {
        print_stats("SRT",mp,mp2,processes,finish_time,n);
    }

}


void feedback1(string status,int time,int n,map<char,pair<int,int>> mp,vector<char> processes,map <int, int> arrivals)
{

    map<char,pair<int,int>> mp2;
    mp2=mp;
    map<char,int> finish;
    vector<string> strings(n,"");
    priority_queue<pair<int, char>, std::vector<pair<int,char>>, std::greater<pair<int,char>>> q;
    unordered_map<char,bool> waiting;
    for(int i=0; i<n; i++)
    {
        strings[i]+=processes[i];
        strings[i]+="     ";
    }
    string title="FB-1  ";
    string heading="------";
    for(int i=0; i<time; i++)
    {
        heading+="--";
        title+=to_string((i%10))+" ";
        bool operation=false;
        for(int j=0; j<n; j++)
        {
            if(i == mp[processes[j]].first)
            {
                q.push({0,processes[j]});
                waiting[processes[j]]=1;
            }
        }
        for(int j=0; j<n; j++)
        {
            if( !q.empty() && processes[j]==q.top().second && operation==false)
            {
                strings[j]+="|*";
                mp[processes[j]].second--;
                int level=q.top().first;
                q.pop();
                if(q.empty() && arrivals[i+1]!=1 && mp[processes[j]].second)
                {
                    q.push({level,processes[j]});
                }
                else if( (arrivals[i+1] && mp[processes[j]].second) || (!q.empty() && mp[processes[j]].second)   )
                {
                    q.push({level+1,processes[j]});
                }
                else
                {
                    waiting[processes[j]]=0;
                    finish[processes[j]]=i+1;
                }
                operation=true;
            }
            else if(waiting[processes[j]])
            {
                strings[j]+="|.";
            }
            else
            {
                strings[j]+="| ";
            }
            if(i==time-1)
            {
                if(finish[processes[j]]==0)
                    finish[processes[j]]=i+1;
                strings[j]+="| ";
            }
        }
        if(time-1==i)
        {
            heading+="--";
            title+=to_string(((i+1)%10))+" ";
        }
    }
    if(status=="trace")
    {

        cout<<title<<endl;
        cout<<heading<<endl;
        for(int i=0; i<n; i++)
        {
            cout<<strings[i]<<endl;
        }
        cout<<heading<<endl<<endl;
    }
    else
    {

        print_stats("FB-1",mp,mp2,processes,finish,n);
    }
}

void feedback2(string status,int time,int n,map<char,pair<int,int>> mp,vector<char> processes,map <int, int> arrivals)
{
    vector<string> strings(n,"");
    map<char,pair<int,int>> mp2;
    mp2=mp;
    map<char,int> finish;
    map<char,int> remaining;
    priority_queue<pair<int, char>, std::vector<pair<int,char>>, std::greater<pair<int,char>>> q;
    unordered_map<char,bool> waiting;
    for(int i=0; i<n; i++)
    {
        strings[i]+=processes[i];
        strings[i]+="     ";
    }
    string title="FB-2i ";
    string heading="------";
    char working='.';
    map<char,int> process_level;
    map<pair<int,char>,bool> etramet;
    for(int i=0; i<time; i++)
    {
        heading+="--";
        title+=to_string((i%10))+" ";
        bool operation=false;
        if(!q.empty() && etramet[ {q.top().first,q.top().second}])q.pop();
        for(int j=0; j<n; j++)
        {
            if(i == mp[processes[j]].first)
            {
                q.push({0,processes[j]});
                waiting[processes[j]]=1;
                remaining[processes[j]]=1;
                process_level[processes[j]]=0;
            }
        }
        for(int j=0; j<n; j++)
        {
            if( working==processes[j] && remaining[processes[j]] &&operation==false)
            {
                strings[j]+="|*";
                mp[processes[j]].second--;
                remaining[processes[j]]--;
                int level=process_level[processes[j]];
                if(remaining[processes[j]] && mp[processes[j]].second )
                {
                }
                else
                {
                    etramet[ {level,processes[j]}]=1;
                    working='.';
                    if(q.empty() && arrivals[i+1]!=1 && mp[processes[j]].second)
                    {
                        q.push({level,processes[j]});
                        if(mp[processes[j]].second>=(1<<level))
                            remaining[processes[j]]=1<<(level);
                        else
                            remaining[processes[j]]=mp[processes[j]].second;
                    }
                    else if( (arrivals[i+1] && mp[processes[j]].second) || (!q.empty() && mp[processes[j]].second)   )
                    {
                        q.push({level+1,processes[j]});
                        if(mp[processes[j]].second>=(1<<(level+1)))
                            remaining[processes[j]]=1<<(level+1);
                        else
                            remaining[processes[j]]=mp[processes[j]].second;
                        process_level[processes[j]]=level+1;
                    }
                    else
                    {
                        waiting[processes[j]]=0;
                        finish[processes[j]]=i+1;
                    }
                }
                operation=true;
            }
            else if( !q.empty() && processes[j]==q.top().second && operation==false && remaining[processes[j]])
            {
                working=processes[j];
                strings[j]+="|*";
                mp[processes[j]].second--;
                remaining[processes[j]]--;
                int level=process_level[processes[j]];
                if(remaining[processes[j]] && mp[processes[j]].second )
                {
                }
                else
                {
                    q.pop();
                    working='.';
                    if(q.empty() && arrivals[i+1]!=1 && mp[processes[j]].second)
                    {
                        q.push({level,processes[j]});
                        if(mp[processes[j]].second>=(1<<level))
                            remaining[processes[j]]=1<<(level);
                        else
                            remaining[processes[j]]=mp[processes[j]].second;
                    }
                    else if( (arrivals[i+1] && mp[processes[j]].second) || (!q.empty() && mp[processes[j]].second)   )
                    {
                        q.push({level+1,processes[j]});

                        if(mp[processes[j]].second>=(1<<(level+1)))
                            remaining[processes[j]]=1<<(level+1);
                        else
                            remaining[processes[j]]=mp[processes[j]].second;

                        process_level[processes[j]]=level+1;
                    }
                    else
                    {
                        waiting[processes[j]]=0;
                        finish[processes[j]]=i+1;

                    }
                }
                operation=true;
            }
            else if(waiting[processes[j]])
            {
                strings[j]+="|.";
            }
            else
            {
                strings[j]+="| ";
            }
            if(i==time-1)
            {
                if(!finish[processes[j]])
                    finish[processes[j]]=i+1;
                strings[j]+="| ";
            }
        }
        if(time-1==i)
        {
            heading+="--";
            title+=to_string(((i+1)%10))+" ";
        }
    }
    if(status=="trace")
    {
        cout<<title<<endl;
        cout<<heading<<endl;
        for(int i=0; i<n; i++)
        {
            cout<<strings[i]<<endl;
        }
        cout<<heading<<endl<<endl;
    }
    else
    {
        print_stats("FB-2i",mp,mp2,processes,finish,n);
    }
}
void Aging1(int q,string status,int time,int n,map<char,pair<int,int>> mp,vector<char> processes)
{
    map<char,int> quantum;
    vector<string> strings(n,"");
    map<char,bool> waiting ;
    map<char,pair<int,int>> mp2;
    map<char,int> waiting_time;
    deque<pair<int,char>> s;
    for(int i=0; i<n; i++)
    {
        quantum[processes[i]]=q;
        waiting[processes[i]]=0;
        waiting_time[processes[i]]=0;
    }
    mp2=mp;

    for(int i=0; i<n; i++)
    {
        strings[i]+=processes[i];
        strings[i]+="     ";
    }
    string title="Aging ";
    string heading="------";

    char operating='2';
    for(int i=0;i<time;i++)
    {
        heading+="--";
        title+=to_string((i%10))+" ";
        for(int k=0;k<n;k++)
        {
            if(mp[processes[k]].first==i && s.empty() && operating=='2')
            {
                waiting[processes[k]]=1;
                operating=processes[k];
            }
            else if(mp[processes[k]].first==i && s.empty() && operating!='2')
            {
                waiting[processes[k]]=1;
                pair<int,char> extraf;
                extraf.first=mp[processes[k]].second;
                extraf.second=processes[k];
                s.push_front(extraf);

            }
            else if(mp[processes[k]].first==i && !s.empty() && operating!='2')
            {
                waiting[processes[k]]=1;
                pair<int,char> extraf;
                extraf.first=mp[processes[k]].second+1;
                extraf.second=processes[k];
                s.push_front(extraf);
                stable_sort(s.begin(),s.end());
            }
        }
         for(int k=0;k<n;k++)
        {
             if (operating==processes[k] && quantum[processes[k]]==0)
            {
                   pair<int,char> extraf,temp,temp2;
                   quantum[processes[k]]=q;
                   extraf.first=mp[processes[k]].second;
                   extraf.second=processes[k];
                   s.push_front(extraf);
                   stable_sort(s.begin(),s.end());
                   int maximum=s.back().first;
                   int max_wait=-1;
                   int h=0;
                   while(s.back().first==maximum && h!=s.size()){
                        temp=s.back();
                        if(waiting_time[temp.second]>max_wait)
                        {
                            max_wait=waiting_time[temp.second];
                            temp2=s.back();
                        }

                         s.pop_back();
                         s.push_front(temp);
                         h++;
                   }
                   while(s.front()!=temp2 && h!=s.size()){
                    pair<int,char> temp3;
                    temp3=s.front();
                    s.pop_front();
                    s.push_back(temp3);
                   }
                   s.pop_front();
                   operating=temp2.second;
                   waiting_time[operating]=0;

            }

        }
        if(!s.empty())
        {
            for(unsigned int y=0;y<s.size();y++)
            {
                pair<int,char>extraf=s.front();
                s.pop_front();
                extraf.first+=1;
                s.push_back(extraf);
            }
            stable_sort(s.begin(),s.end());
        }
        for(int j=0;j<n;j++)
        {
            if(operating==processes[j] && quantum[processes[j]]>0 )
            {
                strings[j]+="|*";
                quantum[processes[j]]-=1;
            }

            else if(waiting[processes[j]]==1)
            {
                strings[j]+="|.";
                waiting_time[processes[j]]++;

            }
            else
            {
                strings[j]+="| ";
            }
            if(i==time-1)
            {
                strings[j]+="| ";

            }
        }

        if(time-1==i)
        {
            heading+="--";
            title+=to_string(((i+1)%10))+" ";
        }

    }

    cout<<title<<endl;
    cout<<heading<<endl;
    for(int i=0; i<n; i++)
    {
        cout<<strings[i]<<endl;
    }
    cout<<heading<<endl<<endl;
}


int main()
{
    string status;
    int time;
    string type;
    int n;
    cin>>status;
    cin>>type;
    int k=count(type.begin(),type.end(),',');
    vector<string> v;
    string temp="";
    for(int i=0; i<type.size(); i++)
    {
        if(i==(type.size()-1))
        {
            temp+=type[i];
            v.push_back(temp);
        }
        else
        {
            if(type[i]==',')
            {
                v.push_back(temp);
                temp="";
            }
            else
            {
                temp+=type[i];
            }
        }

    }

    cin>>time;
    cin>>n;
    vector<char> processes(n);
    map<char,pair<int,int>> mp;
    map <int, int> arrivals;
    map<char,int> service_time;


    for(int i=0; i<n; i++)
    {
        string a;
        cin>>a;
        processes[i]=a[0];
        mp[a[0]]= {a[2]-'0',a[4]-'0'};
        arrivals[a[2]-'0']=1;
        service_time[processes[i]]=mp[processes[i]].second;
    }

    for(int i=0; i<v.size(); i++)
    {
        if(v[i]=="1")
        {
            FCFS(status,time,n,mp,processes);
        }
        else if(v[i][0]=='2')
        {
            int volt=stoi(v[i].substr(2,v[i].size()-2));
            rr(volt,status,time,n,mp,processes);
        }else if(v[i]=="3")
        {
            SPN(status,time,n,mp,processes,service_time);
        }
        else if(v[i]=="4")
        {
            SRT(status,time,n,mp,processes,service_time);
        }
        else if(v[i]=="5")
        {
            HRRN(status,time,n,mp,processes,service_time);
        }
        else if(v[i]=="6")
        {
            feedback1(status,time,n,mp,processes,arrivals);
        }
        else if(v[i]=="7")
        {
            feedback2(status,time,n,mp,processes,arrivals);
        }else if(v[i][0]=='8')
        {
            int volt=stoi(v[i].substr(2,v[i].size()-2));
            Aging1(volt,status,time,n,mp,processes);
        }
    }
    return 0;
}

#include<iostream>
#include<vector>
#include<algorithm>
#include<numeric>
#include<climits>
using namespace std;
int calculate_v(int& v,const vector<int>& order,const vector<vector<int>>& processing_time)
{
    int n=order.size()-1;
    int m=processing_time[0].size()-1;
    vector<int> finish_time(m+1,0);
    for(int i=1;i<=n;++i)
    for(int j=1;j<=m;++j)
    {
        int temp;
        if(i==1) temp=finish_time[j-1]+processing_time[order[i]][j];
        else if(j!=m) temp=max(finish_time[j-1],finish_time[j+1]-processing_time[order[i-1]][j+1])+processing_time[order[i]][j];
            else temp=max(finish_time[j-1],finish_time[j])+processing_time[order[i]][j];
        v+=temp-finish_time[j];
        finish_time[j]=temp;
    }
    return finish_time[m];
}
vector<int> NEH_PI(const int lambada,const int n,vector<int>& Best_Sequence,const vector<vector<int>>& processing_time)
{
    vector<int> order;
    int best_v=0;
    int bestmakespan=calculate_v(best_v,Best_Sequence,processing_time);
    for(int i=n-lambada+1;i<=n;++i)
    {
        order=Best_Sequence;
        int indice=Best_Sequence[i];
        order.erase(order.begin()+i);
        for(int j=1;j<i;++j)
        {
            order.insert(order.begin()+j,indice);
            int v=0;
            int makespan=calculate_v(v,order,processing_time);
            if(v<best_v)
            {
                Best_Sequence=order;
                best_v=v;
                bestmakespan=makespan;
            }
        }
    }
    Best_Sequence[0]=bestmakespan;
    return Best_Sequence;
}
vector<int> Population_Initialization(const int lambada,const int k,const vector<vector<int>> processing_time,vector<int> indice)
{
    int n=indice.size()-1;
    vector<int> order(1,0);
    int i=1;
    order.insert(order.begin()+i,indice[k]);
    indice.erase(indice.begin()+k);
    while(indice.size()>1)
    {
        ++i;
        int min_v=INT_MAX;
        int best_indice=0;
        for(int j=1;j<indice.size();++j)
        {
            order.push_back(indice[j]);
            int v=0;
            calculate_v(v,order,processing_time);
            if(min_v>v)
            {
                best_indice=j;
                min_v=v;
            }
            order.pop_back();
        }
        order.push_back(indice[best_indice]);
        indice.erase(indice.begin()+best_indice);
    }
    return NEH_PI(lambada,n,order,processing_time);
}
int main()
{
    int n,m,lambada,x;
    printf("The number of components:");
    cin>>n;
    printf("The number of machines:");
    cin>>m;
    printf("lambada:");
    cin>>lambada;
    printf("x:");
    cin>>x;
    vector<vector<int>> processing_time(n+1,vector<int>(m+1,0));
    vector<int> total_processing_time(n+1,0);
    vector<int> indice(n+1,0);
    iota(indice.begin(),indice.end(),0);
    printf("The processing time of each component on different machines:\n");
    for(int i=1;i<=n;++i)
    for(int j=1;j<=m;++j)
    {
        cin>>processing_time[i][j];
        total_processing_time[i]+=processing_time[i][j];
    }
    sort(indice.begin()+1,indice.end(),[&total_processing_time](int a,int b){
        return total_processing_time[a]>total_processing_time[b];
    });
    vector<int> Best_sequence=Population_Initialization(lambada,1,processing_time,indice);
    for(int i=2;i<=x;++i)
    {
        vector<int> sequence=Population_Initialization(lambada,i,processing_time,indice);
        if(sequence[0]<Best_sequence[0])
            Best_sequence=sequence;
    }
    cout<<"sequence:"<< Best_sequence[1];
    for(int i=2;i<=n;++i) cout<<" "<<Best_sequence[i];
    cout<<endl<<"minmakespan:"<<Best_sequence[0]<<endl;
}
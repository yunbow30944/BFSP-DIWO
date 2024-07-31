#include<iostream>
#include<vector>
#include<algorithm>
#include<numeric>
#include<climits>
#include<string.h>
using namespace std;

int calculate(const vector<int>& order, const vector<vector<int> >& processing_time)
{
	int n = order.size() - 1;
	int m = processing_time[0].size() - 1;
	/*vector<int> finish_time(m+1,0);
	for(int i=1;i<=n;++i)
	for(int j=1;j<=m;++j)
		finish_time[j]=max(finish_time[j],finish_time[j-1])+processing_time[order[i]][j];
	return finish_time[m];*/
	
	int d[100][100];
	memset(d, 0, sizeof(d));
	d[order[1]][0] = 0;
	for (int i = 1; i <= n; i++) {
		for (int j = 0; j <= m; j++) {
			if (i == 1 && j != m)d[order[i]][j] = d[order[i]][j - 1] + processing_time[order[i]][j];
			else if (j == m)d[order[i]][j] = d[order[i]][j - 1] + processing_time[order[i]][j];
			else {
				if (j == 0)d[order[i]][j] = d[order[i - 1]][1];
				else {
					d[order[i]][j] = max(d[order[i]][j - 1] + processing_time[order[i]][j], d[order[i - 1]][j + 1]);
				}
			}

		}
	}
	return d[order[n]][m];

}
vector<int> NEH_algorithm(const int n, const vector<int>& indice, const vector<vector<int> >& processing_time)
{
	vector<int> order(1, 0);
	int bestmakespan;
	for (int i = 1; i <= n; ++i)
	{
		bestmakespan = INT_MAX;
		int bestposition = 0;
		for (int j = 1; j <= i; ++j)
		{
			vector<int> temp_order = order;
			temp_order.insert(temp_order.begin() + j, indice[i]);
			int makespan = calculate(temp_order, processing_time);
			if (makespan < bestmakespan)
			{
				bestposition = j;
				bestmakespan = makespan;
			}
		}
		order.insert(order.begin() + bestposition, indice[i]);
	}
	order[0] = bestmakespan;
	return order;
}
void PF_accelerate(vector<int>& order, const vector<vector<int> >& processing_time)
{
	int n = order.size() - 1;
	bool improved = true;
	while (improved)
	{
		improved = false;
		for (int i = 1; i < n; ++i)
		{
			swap(order[i], order[i + 1]);
			int makespan = calculate(order, processing_time);
			if (makespan >= order[0])
				swap(order[i], order[i + 1]);
			else
			{
				order[0] = makespan;
				improved = true;
			}
		}
	}
}
int main()
{
	int n, m;
	printf("The number of components:");
	cin >> n;
	printf("The number of machines:");
	cin >> m;
	vector<vector<int> > processing_time(n + 1, vector<int>(m + 1, 0));
	vector<int> total_processing_time(n + 1, 0);
	vector<int> indice(n + 1);
	iota(indice.begin(), indice.end(), 0);
	printf("The processing time of each component on different machines:\n");
	for (int i = 1; i <= n; ++i)
		for (int j = 1; j <= m; ++j)
		{
			cin >> processing_time[i][j];
			total_processing_time[i] += processing_time[i][j];
		}
	sort(indice.begin() + 1, indice.end(), [&total_processing_time](int a, int b) {
		return total_processing_time[a] > total_processing_time[b];
		});
	vector<int> sequence = NEH_algorithm(n, indice, processing_time);
	PF_accelerate(sequence, processing_time);
	cout << "sequence:" << sequence[1];
	for (int i = 2; i <= n; ++i) cout << " " << sequence[i];
	cout << endl << "minmakespan:" << sequence[0] << endl;
	return 0;
}
/*19 46 65
44 63 12
85 56 98
59 68 25
87 66 53
51 4 63*/

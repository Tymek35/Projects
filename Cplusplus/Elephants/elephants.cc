//For compilation I used: g++ -Wextra -Wall -O2 elephants.cc -o elephants

#include <iostream>
#include <vector>
#include <new>

//This infinity value is correct for this problem because
//elephant's mass can not be bigger than that.
#define infinity 1000000000

using namespace std;

//Loads array of given size from standard input.
void load_array(int size, int array[])
{
  for(int i = 0; i < size; ++i){
    cin >> array[i];
  }
}

//Sets every array's cell to false.
void set_false(int size, bool array[])
{
  for(int i = 0; i < size; ++i){
    array[i] = false;
  }
}

//Function returns smaller value.
int minimum_int(int a, int b)
{
  return (a < b) ? a : b;
}

//Function returns smaller value.
unsigned long long int minimum_ulli(unsigned long long int a, unsigned long long int b)
{
  return (a < b) ? a : b;
}

int main(void)
{
  ios_base::sync_with_stdio(false);//This line makes cin faster.

  //Loading input.
  int number_of_elephants;
  cin >> number_of_elephants;
  int *mass, *initial_arrangement, *final_arrangement;
  mass = new int[number_of_elephants];
  initial_arrangement = new int[number_of_elephants];
  final_arrangement = new int[number_of_elephants];

  load_array(number_of_elephants, mass);
  load_array(number_of_elephants, initial_arrangement);
  load_array(number_of_elephants, final_arrangement);

  //Constructing permutation.
  int *permutation;
  permutation = new int[number_of_elephants + 1];

  for(int i = 0; i < number_of_elephants; ++i){
    permutation[initial_arrangement[i]] = final_arrangement[i];
  }

  delete[] initial_arrangement;
  delete[] final_arrangement;

  //Distributing permutation on cycles.
  bool *visited;
  visited = new bool[number_of_elephants + 1];
  set_false(number_of_elephants + 1, visited);
  vector<vector<int>> cycles;
  int cycle_idx = -1, elephant;

  for(int i = 1; i < number_of_elephants; ++i){
    if(visited[i] == false){
      cycles.push_back(vector<int>());
      cycle_idx = cycle_idx + 1;
      elephant = i;

      while(visited[elephant] == false){
        visited[elephant] = true;
        cycles[cycle_idx].push_back(elephant);
        elephant = permutation[elephant];
      }

    }
  }

  delete[] permutation;
  delete[] visited;

  //Counting parameters of cycles.
  int min = infinity, number_of_cycles = cycles.size();
  int *cycle_min;
  cycle_min = new int[number_of_cycles];
  unsigned long long int *cycle_sum;
  cycle_sum = new unsigned long long int[number_of_cycles];

  for(int i = 0; i < number_of_cycles; ++i){
    unsigned long long int current_sum = 0;
    int current_min = infinity;
    auto iter = cycles[i].begin();
    auto end = cycles[i].end();

    while(iter != end){
      current_sum += mass[*iter - 1];
      current_min = minimum_int(current_min, mass[*iter - 1]);
      ++iter;
    }

    cycle_min[i] = current_min;
    cycle_sum[i] = current_sum;
    min = minimum_int(min, current_min);
  }

  delete[] mass;

  //Counting result.
  unsigned long long int result = 0, method1, method2;

  for(int i = 0; i < number_of_cycles; ++i){
    method1 = cycle_sum[i] + (cycles[i].size() - 2) * cycle_min[i];
    method2 = cycle_sum[i] + cycle_min[i] + (cycles[i].size() + 1) * min;
    result += minimum_ulli(method1, method2);
  }

  delete[] cycle_min;
  delete[] cycle_sum;

  cout << result << endl;

  return 0;
}

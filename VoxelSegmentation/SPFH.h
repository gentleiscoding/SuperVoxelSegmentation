#pragma once
#include <iostream>
#include <vector>
using namespace std;

typedef struct{ double alpha; double beta; double theta;} angles;
class SPFH
{
public:
	SPFH(void);
	SPFH(int);
	~SPFH(void);

public:
	int center_vertex;
	vector<int> adj_vertex;
	vector<angles>	list_angles;
	vector<double>	spfh_value;
};


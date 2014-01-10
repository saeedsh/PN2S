///////////////////////////////////////////////////////////
//  HSC_Solver.h
//  Implementation of the Class HSC_Solver
//  Created on:      26-Dec-2013 4:21:51 PM
//  Original author: Saeed Shariati
///////////////////////////////////////////////////////////

#if !defined(FCD33A96_9E58_4bec_BA66_91CF4FD383BD__INCLUDED_)
#define FCD33A96_9E58_4bec_BA66_91CF4FD383BD__INCLUDED_

#include "../Definitions.h"
#include "HSC_SolverData.h"
#include "HSC_SolverChannels.h"
#include "HSC_SolverComps.h"
#include "HSCModel.h"
#include "../HSC_Device.h"

class HSC_Solver
{
private:
	double _dt;
	map<uint, HSC_SolverData*> _modelToPackMap;
public:
	vector<HSC_SolverData> solverPacks;

	HSC_Solver();
	virtual ~HSC_Solver();

	double GetDt(){ return _dt;}
	void SetDt(double dt){ _dt = dt;}

	hscError Setup(double dt);
	hscError PrepareSolver( vector<HSCModel> & _models);
	HSC_SolverData* LocateDataByID(hscID_t id);
	void Process(HSC_SolverData* data, HSC_Device* d);
};

#endif // !defined(FCD33A96_9E58_4bec_BA66_91CF4FD383BD__INCLUDED_)
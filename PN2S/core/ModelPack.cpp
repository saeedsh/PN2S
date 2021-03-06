///////////////////////////////////////////////////////////
//  SolverData.cpp
//  Implementation of the Class SolverData
//  Created on:      27-Dec-2013 7:29:01 PM
//  Original author: Saeed Shariati
///////////////////////////////////////////////////////////

#include "ModelPack.h"


#include <assert.h>

using namespace pn2s;


ModelPack::ModelPack(): _dt(1), _st(0){

}

ModelPack::~ModelPack(){

}

size_t ModelPack::AllocateMemory(models::ModelStatistic s, cudaStream_t st){
	size_t val = 0;
	stat = s;
	_st = st;
	val += _compsSolver.AllocateMemory(stat, st);
	val += _chanSolver.AllocateMemory(stat, st);
	return val;
}

void ModelPack::PrepareSolvers(){
	_compsSolver.PrepareSolver(_chanSolver.GetFieldChannelCurrents());
	_chanSolver.PrepareSolver(_compsSolver.GetFieldVm());
	cudaDeviceSynchronize();
}


double ModelPack::Input()
{
	double t = 0;
	t += _compsSolver.Input();
//	t += _chanSolver.Input();
	return t;
}

double ModelPack::Process()
{
	double t = 0;
	t += _chanSolver.Process();
	t += _compsSolver.Process();
	return t;
}

double ModelPack::Output()
{
	double t = 0;
//	t +=_chanSolver.Output();
	t += _compsSolver.Output();
	return t;
}




///////////////////////////////////////////////////////////
//  SolverComps.h
//  Implementation of the Class SolverComps
//  Created on:      27-Dec-2013 7:57:50 PM
//  Original author: Saeed Shariati
///////////////////////////////////////////////////////////

#if !defined(EA_ABB95B66_E531_4681_AE2B_D1CE4B940FF6__INCLUDED_)
#define EA_ABB95B66_E531_4681_AE2B_D1CE4B940FF6__INCLUDED_

#include "../../headers.h"
#include "../models/Model.h"
#include "../NetworkAnalyzer.h"
#include "Field.h"

namespace pn2s
{
namespace solvers
{

class SolverComps
{
private:
	double _dt;
	uint nModel;
	uint nComp;
	vector<uint> _ids;

//	//Connection Fields
//	Field<T, arch>  _hm;	// Hines Matrices
//	Field<T, arch>  _rhs;	// Right hand side of the equation
//	Field<T, arch>  _Vm;	// Vm of the compartments
//	Field<TYPE_, arch>  _Cm;	// Cm of the compartments
//	Field<TYPE_, arch>  _Em;	// Em of the compartments
//	Field<TYPE_, arch>  _Rm;	// Rm of the compartments
//
	void  makeHinesMatrix(models::Model *model, TYPE_ * matrix);// float** matrix, uint nCompt);
	void getValues();
public:
	SolverComps();
	~SolverComps();
	Error_PN2S Input();
	Error_PN2S PrepareSolver(vector< models::Model > &models, NetworkAnalyzer &analyzer);
	Error_PN2S Process();
	Error_PN2S Output();
	Error_PN2S UpdateMatrix();

	double GetDt(){ return _dt;}
	void SetDt(double dt){ _dt = dt;}

//	TYPE_ GetA(int n,int i, int j){return _hm[n*nComp*nComp+i*nComp+j];}
//	TYPE_ GetRHS(int n,int i){return _rhs[n*nComp+i];}
//	TYPE_ GetVm(int n,int i){return _Vm[n*nComp+i];}
//	TYPE_ GetCm(int n,int i){return _Cm[n*nComp+i];}
//	TYPE_ GetRm(int n,int i){return _Rm[n*nComp+i];}
//	TYPE_ GetEm(int n,int i){return _Em[n*nComp+i];}

	//Setter and Getter
	enum Fields {CM_FIELD, EM_FIELD, RM_FIELD, RA_FIELD,INIT_VM_FIELD, VM_FIELD};

	static TYPE_ (*Fetch_Func) (uint id, Fields field);

};

}
}
#endif // !defined(EA_ABB95B66_E531_4681_AE2B_D1CE4B940FF6__INCLUDED_)

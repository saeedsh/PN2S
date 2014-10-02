///////////////////////////////////////////////////////////
//  Device.cpp
//  Implementation of the Class Device
//  Created on:      26-Dec-2013 4:18:01 PM
//  Original author: saeed
///////////////////////////////////////////////////////////

#include "Device.h"
#include "headers.h"
#include <cuda.h>
#include <cuda_runtime.h>
#include <assert.h>

//#define USE_TBB
#ifdef USE_TBB
#include "tbb/flow_graph.h"
#include "tbb/atomic.h"
#include "tbb/tick_count.h"
using namespace std;
using namespace tbb;
using namespace tbb::flow;
#endif

#include "core/models/ModelStatistic.h"
#include "DeviceManager.h"
using namespace pn2s;
using namespace pn2s::models;

extern std::map< unsigned int, pn2s::Location > locationMap; //Locates in ResourceManager


Device::Device(int _id): id(_id), _dt(1), nstreams(Parameters::MAX_STREAM_NUMBER){
	_modelPacks.clear();
	cudaSetDevice(id);
	/**
	 * Check configuration
	 */
	cudaDeviceProp deviceProp;
	cudaGetDeviceProperties(&deviceProp, _id);

	if ((1 == deviceProp.major) && (deviceProp.minor < 1))
	{
		printf("%s does not have Compute Capability 1.1 or newer.  Reducing workload.\n", deviceProp.name);
	}
//	cudaSetDeviceFlags(cudaDeviceBlockingSync | cudaDeviceMapHost);
//	nstreams = deviceProp.multiProcessorCount;
	nstreams = Parameters::MAX_STREAM_NUMBER;
	/**
	 * Device initialization
	 */
	streams = (cudaStream_t *) malloc(nstreams * sizeof(cudaStream_t));
	//Create Streams
	for (int i = 0; i < nstreams; ++i) {
		cudaStreamCreate(&streams[i]);
	}
}

Device::~Device(){
//	if(_modelPacks.size())
//		cudaDeviceReset();
}

void Device::Destroy(){
//	for (int i = 0; i < nstreams; i++)
//	{
//		cudaStreamDestroy(streams[i]);
//	}
}

Error_PN2S Device::AllocateMemory(vector<Model_pack_info> &mps, double dt ){

	if(mps.empty())
		return Error_PN2S::EMPTY;

	cudaSetDevice(id);
	_dt = dt;

	size_t nPack = mps.size();

	//Distribute Modelpacks over streams
	_modelPacks.resize(nPack);
	for (int pack = 0; pack < nPack; ++pack) {
		/**
		 * Create statistic and Allocate memory for Modelpacks
		 */
		size_t nCompt = mps[pack][0].nCompt;
		size_t nChannels = 0;
		size_t nGates = 0;
		for (Model_pack_info::iterator m = mps[pack].begin(); m != mps[pack].end(); m++)
		{
			nChannels += m->nChannel;
			nGates += m->nGates;
			_modelPacks[pack].models.push_back( m->id);
		}
		ModelStatistic stat(dt, mps[pack].size(), nCompt, nChannels, nGates);
		_modelPacks[pack].AllocateMemory(stat,streams[pack%nstreams]);
	}
	return Error_PN2S::NO_ERROR;
}

void Device::PrepareSolvers(){
	cudaSetDevice(id);
	for(vector<ModelPack>::iterator it = _modelPacks.begin(); it != _modelPacks.end(); ++it) {
		it->PrepareSolvers();
	}
}

#ifdef USE_TBB
/**
 * Multithread tasks section
 */


//#ifdef PN2S_DEBUG

struct input_body {
	ModelPack* operator()( ModelPack *m ) {
//		_D(std::cout<< "Input" << m<<endl<<flush);
		m->Input();
        return m;
    }
};

struct process_body{
	ModelPack* operator()( ModelPack* m) {
//		_D(std::cout<< "Process" << m<<endl<<flush);
		m->Process();
        return m;
    }
};

struct output_body{
	ModelPack* operator()( ModelPack* m ) {
//		_D(std::cout<< "Output" << m<<endl<<flush);
		m->Output();
        return m;
    }
};
#endif

void Device::Process()
{

	uint model_n = _modelPacks.size();
	if(model_n < 1)
		return;

#ifdef USE_TBB
	graph scheduler;

	broadcast_node<ModelPack*> broadcast(scheduler);
	function_node< ModelPack*, ModelPack* > input_node(scheduler, 1, input_body());
	function_node< ModelPack*, ModelPack* > process_node(scheduler, 1, process_body() );
	function_node< ModelPack*, ModelPack* > output_node(scheduler, 1, output_body() );
	queue_node< ModelPack* > iq_node(scheduler);
	queue_node< ModelPack* > oq_node(scheduler);
	make_edge( broadcast, input_node );
	make_edge( input_node, iq_node);
	make_edge( iq_node, process_node);
	make_edge( process_node, oq_node);
	make_edge( oq_node, output_node);


	for (vector<ModelPack >::iterator it = _modelPacks.begin(); it != _modelPacks.end(); ++it)
	{
		broadcast.try_put(&(*it));
	}
	scheduler.wait_for_all();
#else

	cudaSetDevice(id);
	double time = 0;
	for (vector<ModelPack>::iterator it = _modelPacks.begin(); it != _modelPacks.end(); ++it)
	{
		time += it->Input();
	}

	for (vector<ModelPack >::iterator it = _modelPacks.begin(); it != _modelPacks.end(); ++it)
	{
		time += it->Process();
	}
	for (vector<ModelPack >::iterator it = _modelPacks.begin(); it != _modelPacks.end(); ++it)
	{
		time += it->Output();
	}
//	cudaDeviceSynchronize();

//	cout << time << endl;
#endif
}

